#include <iostream>
#include <iomanip>

#include "opencv2/opencv.hpp"

extern "C"
{
    // fixed by `sudo apt-get install libatlas-base-dev` ???
    #include "apriltag.h"
    #include "apriltag_pose.h"
    #include "tag16h5.h"
}

#include "apriltag/Apriltag.hpp"
#include "apriltag/ApriltagDetector.hpp"
#include "helper/Unit.hpp"

ApriltagDetector::ApriltagDetector(int streamId, bool showWindow)
{
    this->streamId = streamId;
    this->showWindow = showWindow;
}

void ApriltagDetector::startStream(){
    cv::VideoCapture cap(this->streamId);
    this->cap = cap;
    if (!cap.isOpened())
    {
        std::cerr << "Couldn't open video capture device" << std::endl;
        return;
    }

    cap.set(cv::CAP_PROP_FPS, config.fps);
}

void ApriltagDetector::detect(void (*handle)(const Apriltag &))
{
    // TODO: fix grub bruh (irrelevant)
    // TODO: turn this function into a thread

    cv::TickMeter meter;
    meter.start();

    apriltag_family_t *tf = tag16h5_create();
    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_detector_add_family(td, tf);

    td->nthreads = this->config.threads;
    td->quad_decimate = 4;
    td->quad_sigma = 2;
    td->refine_edges = true;
    td->decode_sharpening = 0.25;
    td->debug = false;

    apriltag_detection_info_t info;
    info.tagsize = 6 * Unit::INCH;
    info.fx = config.focalX;
    info.fy = config.focalY;
    info.cx = config.centerX;
    info.cy = config.centerY;

    std::cout << "Detector 16h5 initialized in " << std::fixed << std::setprecision(3) << meter.getTimeSec() << " seconds" << std::endl;
    std::cout << "  " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << "x" << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << " @" << cap.get(cv::CAP_PROP_FPS) << "FPS" << std::endl;

    meter.stop();
    meter.reset();

    cv::Mat frame, gray;
    while (true)
    {
        errno = 0;
        cap >> frame;

        cv::Mat gray, ch2, ch3;

        std::vector<cv::Mat> channels(3);

        split(frame, channels);

        gray = channels[0];
        ch2 = channels[1];
        ch3 = channels[2];

        image_u8_t im = {gray.cols, gray.rows, gray.cols, gray.data};

        zarray_t *detections = apriltag_detector_detect(td, &im);

        for (int i = 0; i < zarray_size(detections); i++)
        {
            apriltag_detection_t *det;
            zarray_get(detections, i, &det);

            info.det = det;
            info.cx = det->c[0];
            info.cy = det->c[1];

            apriltag_pose_t pose;
            double err = estimate_tag_pose(&info, &pose);

            matd_t* t = pose.t;
            double* d = t->data;
            int l = sizeof(d);

            Vector3D position(l, det->c[1], det->c[1]);
            Vector3D rotation(det->c[0], det->c[1], det->c[1]);
            Apriltag tag(det->id, &position, &rotation);

            //handle(tag);

            if (!this->showWindow) {
                continue;
            }

            std::cout << d[0] << ", " << d[1] << ", " << d[2] << ", " << d[3] << ", " << d[4] << ", " << d[5] << ", " << d[6] << ", " << d[7] << std::endl;

            line(frame, cv::Point(det->p[0][0], det->p[0][1]),
                 cv::Point(det->p[1][0], det->p[1][1]),
                 cv::Scalar(0, 0xff, 0), 2);
            line(frame, cv::Point(det->p[0][0], det->p[0][1]),
                 cv::Point(det->p[3][0], det->p[3][1]),
                 cv::Scalar(0, 0, 0xff), 2);
            line(frame, cv::Point(det->p[1][0], det->p[1][1]),
                 cv::Point(det->p[2][0], det->p[2][1]),
                 cv::Scalar(0xff, 0, 0), 2);
            line(frame, cv::Point(det->p[2][0], det->p[2][1]),
                 cv::Point(det->p[3][0], det->p[3][1]),
                 cv::Scalar(0xff, 0, 0), 2);

            std::stringstream ss;
            ss << det->id;
            cv::String text = ss.str();
            int fontface = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
            double fontscale = 1.0;
            int baseline;
            cv::Size textsize = cv::getTextSize(text, fontface, fontscale, 2,
                                                &baseline);
            putText(frame, text, cv::Point(det->c[0] - textsize.width / 2, det->c[1] + textsize.height / 2),
                    fontface, fontscale, cv::Scalar(0xff, 0x99, 0), 2);
        }
        
        apriltag_detections_destroy(detections);

        if (this->showWindow) {
           cv::imshow("Tag Detections", frame);
            if (cv::waitKey(15) >= 0)
                break;
        }
    }

    apriltag_detector_destroy(td);
    tag16h5_destroy(tf);
}