
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <string>

int main(int argc, char* argv[])
{
    const std::string data_directory = DATA_DIR;
    const std::string output_directory = OUTPUT_DIR;

    // Load images
    cv::Mat imgLeft_ = cv::imread(data_directory + "/img_left.jpg");
    cv::Mat imgRight = cv::imread(data_directory + "/img_right.jpg");

    // Pad some margine on Left image for stitching
    cv::Size AugmentedImgSize = cv::Size(imgLeft_.size().width * 1.5, imgLeft_.size().height);
    cv::Mat imgLeft = cv::Mat(AugmentedImgSize, imgLeft_.type(), cv::Scalar(0, 0, 0));
    cv::copyMakeBorder(imgLeft_, imgLeft, 0, 0, 0, imgLeft_.size().width * 0.5, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
  
    // -------------------------------------------------------------------------
    // Task a) Feature Detection and description
    // -------------------------------------------------------------------------


    // -------------------------------------------------------------------------
    // Task b) Keypoint matching
    // -------------------------------------------------------------------------


    // -------------------------------------------------------------------------
    // Task c) Homography
    // -------------------------------------------------------------------------


    // -------------------------------------------------------------------------
    // Task d) Stitch the right image on left image
    // -------------------------------------------------------------------------

    // Overlay right image onto left image and save it as "bonus.png"


	return 0;
}
