
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
    // imgLeft_ is scene, imgRight is object
    std::vector<cv::KeyPoint> keypointsObj, keypointsScene;
    cv::Mat descriptorsObj, descriptorsScene;

    cv::Ptr<cv::BRISK> detector = cv::BRISK::create();
    detector->detectAndCompute(imgRight, cv::noArray(), keypointsObj, descriptorsObj);
    detector->detectAndCompute(imgLeft_, cv::noArray(), keypointsScene, descriptorsScene);

    descriptorsObj.convertTo(descriptorsObj, CV_32F);
    descriptorsScene.convertTo(descriptorsScene, CV_32F);

    // -------------------------------------------------------------------------
    // Task b) Keypoint matching
    // -------------------------------------------------------------------------
//    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE_HAMMING);
    cv::FlannBasedMatcher matcher;

    std::vector<cv::DMatch> matches;
    matcher.match(descriptorsObj, descriptorsScene, matches);

    cv::Mat imgMatches;
    cv::drawMatches(imgRight, keypointsObj, imgLeft_, keypointsScene, matches, imgMatches);

    // -------------------------------------------------------------------------
    // Task c) Homography
    // -------------------------------------------------------------------------
    std::vector<cv::Point2f> obj, scene;
    for (int i = 0; i < matches.size(); i++) {
        obj.push_back(keypointsObj[matches[i].queryIdx].pt);
        scene.push_back(keypointsScene[matches[i].trainIdx].pt);
    }

    cv::Mat matchesMask, homography;
    homography = cv::findHomography(obj, scene, cv::RANSAC, 3, matchesMask);


    // -------------------------------------------------------------------------
    // Task d) Stitch the right image on left image
    // -------------------------------------------------------------------------
    cv::Mat imgTransformed;
    cv::warpPerspective(imgRight, imgTransformed, homography, imgLeft.size());

    // Overlay right image onto left image and save it as "bonus.png"
    cv::Mat imgStitched;
    imgLeft.copyTo(imgStitched);

    for (int r = 0; r < imgStitched.rows; r++) {
        for (int c = 0; c < imgStitched.cols; c++) {
            if (imgStitched.at<double>(r, c) == 0){
                // Replace background with actual scene
                imgStitched.at<double>(r, c) = imgTransformed.at<double>(r, c);
            }
        }
    }

    cv::imwrite(output_directory + "/bonus.png", imgStitched);
    cv::imwrite(data_directory + "/bonus.png", imgStitched);
    return 0;
}
