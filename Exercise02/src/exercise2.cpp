
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
	std::vector<cv::Point2d> cornersObj{ cv::Point2d(0,                0),
										cv::Point2d(imgObjSize.width, 0),
										cv::Point2d(imgObjSize.width, imgObjSize.height),
										cv::Point2d(0,                imgObjSize.height) };

	std::vector<cv::Point2d> cornersScene(4);

	// compute object corners in the scene image using homography

	
	// draw rectangle of detected object

}

int main(int argc, char* argv[])
{
    const std::string data_directory = DATA_DIR;
	const std::string output_directory = OUTPUT_DIR;

	// Load images
    cv::Mat imgObj = cv::imread(data_directory + "/B1_original.JPG");
    cv::Mat imgScene = cv::imread(data_directory + "/scene.jpg");

    // -------------------------------------------------------------------------
    // Task a) Feature Detection and description
    // -------------------------------------------------------------------------

    // Step 1: Detect keypoints and extrahamming distancect descriptors

	
    // Step 2: Draw keypoints on the images and save them as
	//         obj -> "task_a_1.png" and scene -> "task_a_2.png"


    // -------------------------------------------------------------------------
    // Task b) Keypoint matching
    // -------------------------------------------------------------------------

    // Step 1: Match descriptors


    // Step 2: Draw corresponding pair and save it as "task_b.png"


    // -------------------------------------------------------------------------
    // Task c) Estimate homography 
    // -------------------------------------------------------------------------

	// Step 1: Compute homography with RANSAC


    // Step 2: Display inlier matches


    // Step 3: Draw object rectangle in the scene using drawHomographyRect() and
	//         save it as "task_c.png"


    // -------------------------------------------------------------------------
    // Task d) AR object
    // -------------------------------------------------------------------------

    // Step 1: Overlay another object on the detected object in the scene for AR
	//         and save it as "task_d.png"


#if 0
    // -------------------------------------------------------------------------
    // Task e) Augment on video
    // -------------------------------------------------------------------------

    // Augmnet the video sequence using the same Object.
	
	cv::VideoCapture capture;
    capture.open(data_directory + "/sequence.mp4");
	cv::VideoWriter writer(output_directory + "/task_e.mp4", CV_FOURCC('M', 'J', 'P', 'G'), 10, cv::Size(852,480));
	
	cv::Mat imgVideo;
	while (capture.read(imgVideo))
    {
        // Detect the planer object, using the code from Task a) to d).

		cv::imshow("Scene image", imgVideo);
		
        int key = cv::waitKey(1);
        if (key=='q' || key==27){
            break;
        }
    }

    capture.release();
    writer.release();

#endif
	

	return 0;
}
