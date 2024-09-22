#ifndef POSE_FILTER
#define POSE_FILTER

#include <string>
#include <map>
#include <unordered_map>
#include <vector>

#include "../util/ConfigReader.hpp"
#include "../util/Vector3D.hpp"
#include "../apriltag/Apriltag.hpp"


namespace titan
{
    /**
    * @brief Kalman filter for estimating robot pose in field coordinates. Designed around the use of Apriltags
    */
    class PoseFilter
    {
        public:
            PoseFilter(ConfigReader &config);
            void predict(double dt);
            void step(double dt);
            void updateTag(Apriltag &tag, double tagDist, double dt);
            void addControlInput(double x_vel, double y_vel, double rot_vel);

            Vector3D position;
            Vector3D rotation;

            double test1 = 0;
            double test2 = 0;

        private:
            ConfigReader &config;

            bool init = false;

            cv::Mat x; // Previous mean input matrix
            std::unordered_map<int, cv::Mat> P; // Covariance matrices
            cv::Mat R; // _____
            cv::Mat Q; // _____
            cv::Mat u; // Control input (default is no control)
            cv::Mat B; // Control input model (default is no control)
            cv::Mat H; // Measurement function
            cv::Mat I; // _x_ identity matrix
    };
}

#endif