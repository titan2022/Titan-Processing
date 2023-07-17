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

ApriltagDetector::ApriltagDetector(int streamId, bool showWindow, ConfigReader config, NetworkingClient client) : client(client)
{
    this->streamId = streamId;
    this->showWindow = showWindow;
    this->config = config;
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
    cap.set(cv::CAP_PROP_FRAME_WIDTH, config.width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, config.height);
}

void ApriltagDetector::detect(void (*handle)(const Apriltag &, NetworkingClient &))
{
    // TODO: turn this function into a thread

    cv::TickMeter meter;
    meter.start();

    apriltag_family_t *tf = tag16h5_create();
    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_detector_add_family(td, tf);

    td->nthreads = config.threads;
    td->quad_decimate = config.quadDecimate; //1
    td->quad_sigma = config.quadSigma; //2
    td->refine_edges = true;
    td->decode_sharpening = config.decodeSharpening; //0.25
    td->debug = false;

    apriltag_detection_info_t info;
    info.tagsize = 4 * Unit::INCH;
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

        // Split YUYV into channels to get the grayscale image without extra processing
        std::vector<cv::Mat> channels(3);
        split(frame, channels);
        cv::Mat gray = channels[0];

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

            double* t = pose.t->data;
            double* R = pose.R->data;

            Vector3D position(t);
            Vector3D rotation(R);
            Apriltag tag(det->id, position, rotation);

            // Ignore false positives with IDs outside range
            if (det->id > 8) {
                continue;
            }

            handle(tag, client);

            if (!this->showWindow) {
                continue;
            }

            // Render lines and window for debugging
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
            cv::imshow("Apriltag Debug Window", frame);
            if (cv::waitKey(27) >= 0) // ESC key
                break;
        }
    }

    apriltag_detector_destroy(td);
    tag16h5_destroy(tf);
}