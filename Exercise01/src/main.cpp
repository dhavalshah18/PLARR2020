#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

int main(int argc, char* argv[]) {
    // File path for input image
    const std::string data_directory = DATA_DIR;
    const std::string inputImagePath = data_directory + "/tiger.jpg";

    // ------------------------------------------------------------------
    // Task 1: Basics
    // ------------------------------------------------------------------

    // Step 1: Load image using cv::imread
    auto img{cv::imread(inputImagePath)};

    // Step 2: Display image using cv::imshow
    cv::imshow("Tiger", img);
    cv::waitKey(0);

    // Step 3: Convert to gray image using cv::cvtColor function
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    // Display grayscale image
    cv::imshow("Grayscale Tiger", img_gray);
    cv::waitKey(0);

    // Step 4: Save gray image by using cv::imwrite
    cv::imwrite(data_directory + "/tiger_gray.jpg", img_gray);

    // ------------------------------------------------------------------
    // Task 2: Point operators
    // ------------------------------------------------------------------

    // Step 1: Implement the invert operator for a color image
    cv::Mat img_invert;
    cv::bitwise_not(img, img_invert);

    // Step 1.5: Display inverted image
    cv::imshow("Inverted Tiger", img_invert);
    cv::waitKey(0);

    // Step 2: Save the result
    cv::imwrite(data_directory + "/tiger_invert.jpg", img_invert);

    // ------------------------------------------------------------------
    // Task 3: Local operators
    // ------------------------------------------------------------------

    // Step 1: Apply a Gaussian filter and save the result
    cv::Mat img_blur;
    cv::GaussianBlur(img, img_blur, cv::Size(0, 0), 2.0);
    cv::imwrite(data_directory + "/tiger_gaussian.jpg", img_blur);

    // Step 1.5: Display blurred image
    cv::imshow("Blurred Tiger", img_blur);
    cv::waitKey(0);

    // <Observation on the effect of sigma>
    // Increasing sigma reduces noise and increases blurring on the image, gaussian blur filter acts as low-pass filter.

    // Step 2: Apply an edge preserving filter and save the result
    cv::Mat img_blur_bilateral;
    cv::bilateralFilter(img, img_blur_bilateral, 0, 50.0, 10.0);
    cv::imwrite(data_directory + "/tiger_bilateral.jpg", img_blur_bilateral);

    // Step 2.5: Display edge preserved filtered image
    cv::imshow("Edge Preserved Blurred Tiger", img_blur_bilateral);
    cv::waitKey(0);

    // Step 3: Apply an edge detection filter and save the result:
    // on the original image
    cv::Mat img_laplacian;
    cv::Laplacian(img, img_laplacian, 0, 5);
    cv::imwrite(data_directory + "/tiger_edge.jpg", img_laplacian);

    // Display edge detection on input image
    cv::imshow("Edge Detection on Tiger", img_laplacian);
    cv::waitKey(0);

    // on the result of step 1
    cv::Mat img_laplacian_blur;
    cv::Laplacian(img_blur, img_laplacian_blur, 0, 5);
    cv::imwrite(data_directory + "/tiger_gaussian_edge.jpg", img_laplacian_blur);

    // Display edge detection on gaussian blurred image
    cv::imshow("Edge Detection on Gaussian Blurred Tiger", img_laplacian_blur);
    cv::waitKey(0);

    // on the result of step 2
    cv::Mat img_laplacian_bilateral;
    cv::Laplacian(img_blur_bilateral, img_laplacian_bilateral, 0, 5);
    cv::imwrite(data_directory + "/tiger_bilateral_edge.jpg", img_laplacian_bilateral);

    // Display edge detection on bilateral filtered image
    cv::imshow("Edge Detection on Bilateral Filtered Tiger", img_laplacian_bilateral);
    cv::waitKey(0);

    // <Observations on Step 3>
    // From the laplacian filter we are able to detect edges on each image. The laplacian filtered input image is
    //      noisy and shows many edges.
    // The gaussian filtered (blurred) image does not preserve edges as we can see after the laplacian filter is applied.
    // The bilateral filter does remove some noise and is much better at preserving edges than the gaussian filter as
    //      can be seen from the laplacian filter applied after the bilateral filter.

    // ------------------------------------------------------------------
    // Task 4: Image Subtraction
    // ------------------------------------------------------------------

    // Step 1: load two images
    const std::string inputImagePathA = data_directory + "/imgA.jpg";
    const std::string inputImagePathB = data_directory + "/imgB.jpg";

    cv::Mat img_a = cv::imread(inputImagePathA);
    cv::Mat img_b = cv::imread(inputImagePathB);

    // Step 2: Subtract image A and B
    cv::Mat img_out(img_a.size(), img_a.type());
    img_out = abs(img_a - img_b);

    // Step 2.5: Display pixel-wise difference
    cv::imshow("Pixel-wise difference", img_out);
    cv::waitKey(0);

    // Step 2: Save the result
    cv::imwrite(data_directory + "/img_diff.jpg", img_out);

    // BONUS Step 3: Threshold the difference
    cv::Mat img_out_thresh;
    cv::cvtColor(img_out, img_out, cv::COLOR_BGR2GRAY);
    cv::threshold(img_out, img_out_thresh, 150, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imwrite(data_directory + "img_out_threshold.jpg", img_out_thresh);
    
    cv::imshow("Threshold pixel-wise difference", img_out_thresh);
    cv::waitKey(0);

    // BONUS Step 4: Apply the threshold mask on imgA
    cv::Mat img_masked;
    // Copy img_a to img_masked with img_out_thresh as mask
    img_a.copyTo(img_masked, img_out_thresh);

    cv::imshow("Masked original image", img_masked);
    cv::waitKey(0);

    // BONUS Step 5: Save the result
    cv::imwrite(data_directory + "/img_extracted.jpg", img_masked);

    return 0;
}
