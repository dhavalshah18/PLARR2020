#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

int main(int argc, char* argv[])
{
    // File path for input image
    const std::string data_directory = DATA_DIR;
    const std::string inputImagePath = data_directory + "/tiger.jpg";


    // ------------------------------------------------------------------
    // Task 1: Basics
    // ------------------------------------------------------------------

    // Step 1: Load image using cv::imread

    // Step 2: Display image using cv::imshow

    // Step 3: Convert to gray image using cv::cvtColor function

    // Step 4: Save gray image by using cv::imwrite


    // ------------------------------------------------------------------
    // Task 2: Point operators
    // ------------------------------------------------------------------

    // Step 1: Implement the invert operator for a color image

    // Step 2: Save the result


    // ------------------------------------------------------------------
    // Task 3: Local operators
    // ------------------------------------------------------------------


    // Step 1: Apply a Gaussian filter and save the result

    // TODO: <Replace with your oservation on the effect of sigma>

    // Step 2: Apply an edge preserving filter and save the result

    // Step 3: Apply an edge detection filter and save the result:
    // on the original image

    // on the result of step 1

    // on the result of step 2

    // TODO: <Replace with your oservations on Step 3>


    // ------------------------------------------------------------------
    // Task 4: Image Subtraction
    // ------------------------------------------------------------------

    // Step 1: load two images
    const std::string inputImagePathA = data_directory + "/imgA.jpg";
    const std::string inputImagePathB = data_directory + "/imgB.jpg";

    // Step 2: Subtract image A and B

    // Step 2: Save the result

    // BONUS Step 3: Threshold the difference

    // BONUS Step 4: Apply the threshold mask on imgA

    // BONUS Step 5: Save the result

    return 0;
}
