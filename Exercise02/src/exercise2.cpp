
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <string>
#include <cmath>

/**
 * @brief Draw rectangle of estimated homography in the scene image
 * @param [in]    imgObjSize : image size of object image
 * @param [in]    homography : homography matrix (3x3)
 * @param [inout] imgMatches : scene image
 */
void drawHomographyRect(cv::Size imgObjSize, const cv::Mat& homography, const cv::Mat& imgMatches)
{
	if (homography.empty() || imgMatches.cols == 0) {
		return;
	}

	// define object corners in the object image
	std::vector<cv::Point2d> cornersObj{ cv::Point2d(0, 0),
										cv::Point2d(imgObjSize.width, 0),
										cv::Point2d(imgObjSize.width, imgObjSize.height),
										cv::Point2d(0, imgObjSize.height) };

	std::vector<cv::Point2d> cornersScene(4);

	// compute object corners in the scene image using homography
    cv::perspectiveTransform(cornersObj, cornersScene, homography);
	
	// draw rectangle of detected object
    cv::line( imgMatches, cornersScene[0] +
       cv::Point2d( imgObjSize.width, 0), cornersScene[1] + cv::Point2d( imgObjSize.width, 0),
       cv::Scalar(0, 255, 0), 4 );
    cv::line( imgMatches, cornersScene[1] +
       cv::Point2d( imgObjSize.width, 0), cornersScene[2] + cv::Point2d( imgObjSize.width, 0),
       cv::Scalar( 0, 255, 0), 4 );
    cv::line( imgMatches, cornersScene[2] +
       cv::Point2d( imgObjSize.width, 0), cornersScene[3] + cv::Point2d( imgObjSize.width, 0),
       cv::Scalar( 0, 255, 0), 4 );
    cv::line( imgMatches, cornersScene[3] +
       cv::Point2d( imgObjSize.width, 0), cornersScene[0] + cv::Point2d( imgObjSize.width, 0),
       cv::Scalar( 0, 255, 0), 4 );

}

cv::Mat overlayObject(cv::Size imgObjSize, const cv::Mat& homography, const cv::Mat& imgScene, const cv::Mat& imgObjOverlay) {
    // define object corners in the object image
    std::vector<cv::Point2d> cornersObj{ cv::Point2d(0,                0),
                                         cv::Point2d(imgObjSize.width, 0),
                                         cv::Point2d(imgObjSize.width, imgObjSize.height),
                                         cv::Point2d(0,                imgObjSize.height) };

    std::vector<cv::Point2d> cornersScene(4);

    // compute object corners in the scene image using homography
    cv::perspectiveTransform(cornersObj, cornersScene, homography);

    // Apply transform to new object
    // New mat has same size as scene
    cv::Mat imgObj2Transformed;
    cv::warpPerspective(imgObjOverlay, imgObj2Transformed, homography, imgScene.size());

    // Loop to overlay new object on scene
    for (int r = 0; r < imgScene.rows; r++) {
        for (int c = 0; c < imgScene.cols; c++) {
            // Check if background
            if (imgObj2Transformed.at<double>(r, c) == 0){
                // Replace background with actual scene
                imgObj2Transformed.at<double>(r, c) = imgScene.at<double>(r, c);
            }
        }
    }

    return imgObj2Transformed;
}

int main(int argc, char* argv[])
{
    const std::string data_directory = DATA_DIR;
	const std::string output_directory = OUTPUT_DIR;

	// Load images
    cv::Mat imgObj = cv::imread(data_directory + "/B1_original.jpg");
    cv::Mat imgScene = cv::imread(data_directory + "/scene.jpg");

    // -------------------------------------------------------------------------
    // Task a) Feature Detection and description
    // -------------------------------------------------------------------------

    // Step 1: Detect keypoints and extrahamming distancect descriptors
    std::vector<cv::KeyPoint> keypointsObj, keypointsScene;
    cv::Mat descriptorsObj, descriptorsScene;

    cv::Ptr<cv::ORB> detector = cv::ORB::create(2000);
    detector->detectAndCompute(imgObj, cv::noArray(), keypointsObj, descriptorsObj);
    detector->detectAndCompute(imgScene, cv::noArray(), keypointsScene, descriptorsScene);
	
    // Step 2: Draw keypoints on the images and save them as
	//         obj -> "task_a_1.png" and scene -> "task_a_2.png"
	cv::Mat imgKeypointsObj, imgKeypointsScene;
    cv::drawKeypoints(imgObj, keypointsObj, imgKeypointsObj);
    cv::drawKeypoints(imgScene, keypointsScene, imgKeypointsScene);
    cv::imwrite(output_directory + "/task_a_1.png", imgKeypointsObj);
    cv::imwrite(output_directory + "/task_a_2.png", imgKeypointsScene);

    cv::imwrite(data_directory + "/task_a_1.png", imgKeypointsObj);
    cv::imwrite(data_directory + "/task_a_2.png", imgKeypointsScene);


    // -------------------------------------------------------------------------
    // Task b) Keypoint matching
    // -------------------------------------------------------------------------

    // Step 1: Match descriptors
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE_HAMMING);
//    cv::FlannBasedMatcher matcher;
    std::vector<cv::DMatch> matches;
    matcher->match(descriptorsObj, descriptorsScene, matches);


    // Step 2: Draw corresponding pair and save it as "task_b.png"
    cv::Mat imgMatches;
    cv::drawMatches(imgObj, keypointsObj, imgScene, keypointsScene, matches, imgMatches);
    cv::imwrite(output_directory + "/task_b.png", imgMatches);

    cv::imwrite(data_directory + "/task_b.png", imgMatches);


    // -------------------------------------------------------------------------
    // Task c) Estimate homography 
    // -------------------------------------------------------------------------

	// Step 1: Compute homography with RANSAC
	// Localize object
	std::vector<cv::Point2f> obj, scene;
	for (int i = 0; i < matches.size(); i++) {
	    obj.push_back(keypointsObj[matches[i].queryIdx].pt);
        scene.push_back(keypointsScene[matches[i].trainIdx].pt);
	}

    cv::Mat matchesMask, homography;
    homography = cv::findHomography(obj, scene, cv::RANSAC, 3, matchesMask);

    // Step 2: Display inlier matches
    cv::Mat imgInliers;
    cv::drawMatches(imgObj, keypointsObj, imgScene, keypointsScene, matches, imgInliers,
            cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char> (matchesMask));

    cv::imshow("Inlier matches", imgInliers);
    cv::waitKey(0);

    // Step 3: Draw object rectangle in the scene using drawHomographyRect() and
	//         save it as "task_c.png"
    drawHomographyRect(imgObj.size(), homography, imgInliers);
    cv::imwrite(output_directory + "/task_c.png", imgInliers);
    cv::imwrite(data_directory + "/task_c.png", imgInliers);

    cv::imshow("Object", imgInliers);
    cv::waitKey(0);

    // -------------------------------------------------------------------------
    // Task d) AR object
    // -------------------------------------------------------------------------

    // Step 1: Overlay another object on the detected object in the scene for AR
	//         and save it as "task_d.png"
    cv::Mat imgObjA2 = cv::imread(data_directory + "/A2_original.JPG");
    cv::Mat imgSceneOverlayed;
    imgSceneOverlayed = overlayObject(imgObj.size(), homography, imgScene, imgObjA2);

    cv::imshow("Overlay", imgSceneOverlayed);
    cv::waitKey(0);

    cv::imwrite(data_directory + "/task_d.png", imgSceneOverlayed);
    cv::imwrite(output_directory + "/task_d.png", imgSceneOverlayed);

    // -------------------------------------------------------------------------
    // Task e) Augment on video
    // -------------------------------------------------------------------------

    // Augment the video sequence using the same Object.
	
	cv::VideoCapture capture;
    capture.open(data_directory + "/sequence.mp4");
	cv::VideoWriter writer(output_directory + "/task_e.mp4",
	        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
	        10, cv::Size(852,480));
    cv::VideoWriter writer2(data_directory + "/task_e.mp4",
                           cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                           10, cv::Size(852,480));
	
	cv::Mat imgVideo;

	while (capture.read(imgVideo))
    {
        obj.clear();
        scene.clear();

        // Detect the planer object, using the code from Task a) to d).

        detector->detectAndCompute(imgObj, cv::noArray(), keypointsObj, descriptorsObj);
        detector->detectAndCompute(imgVideo, cv::noArray(), keypointsScene, descriptorsScene);

        matcher->match(descriptorsObj, descriptorsScene, matches);

        for (int i = 0; i < matches.size(); i++) {
            obj.push_back(keypointsObj[matches[i].queryIdx].pt);
            scene.push_back(keypointsScene[matches[i].trainIdx].pt);
        }

        homography = cv::findHomography(obj, scene, cv::RANSAC, 3, matchesMask);

        cv::drawMatches(imgObj, keypointsObj, imgVideo, keypointsScene, matches, imgMatches,
                        cv::Scalar::all(-1), cv::Scalar::all(-1),
                        std::vector<char> (matchesMask));

        drawHomographyRect(imgObj.size(), homography, imgMatches);

        imgVideo = overlayObject(imgObj.size(), homography, imgVideo, imgObjA2);

        writer.write(imgVideo);
        writer2.write(imgVideo);

//        cv::imshow("Scene image", imgVideo);
//
        int key = cv::waitKey(1);
        if (key=='q' || key==27){
            break;
        }

    }

    capture.release();
    writer.release();
    writer2.release();


	return 0;
}
