#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>

#include <Eigen/Dense>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Helper functions

const std::string GetFileName(const std::string& path, const int frame_id) {
    std::stringstream ss;
    ss << path << "/frame-" << std::setw(6) << std::setfill('0') << frame_id;
    return ss.str();
}

struct Intrinsics {
    float fx;
    float fy;
    float cx;
    float cy;
    int width;
    int height;
};

Intrinsics LoadIntrinsics(const std::string& path) {
    Intrinsics intrinsics;
    std::ifstream file(path + "/intrinsics.txt");
    if (file.is_open()) {
        file >> intrinsics.fx;
        file >> intrinsics.fy;
        file >> intrinsics.cx;
        file >> intrinsics.cy;
        file >> intrinsics.width;
        file >> intrinsics.height;
        file.close();
    }
    return intrinsics;
}

const Eigen::Matrix4f LoadPose(const std::string& pose_file) {
    Eigen::Matrix4f pose{Eigen::Matrix4f::Identity()};
    // Read camera pose (it's the transformation from the camera to the world coordiante system).
    std::ifstream file(pose_file);
    if (file.is_open()) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                file >> pose(i, j);
        file.close();
    }
    return pose;
}

struct PointCloud {
    std::vector<Eigen::Vector3f> vertices;
    std::vector<cv::Vec3b> colors;

    PointCloud(int max_size) {
        vertices.reserve(max_size);
        colors.reserve(max_size);
    }
};

bool savePointCloudsPLY(std::string filename, PointCloud& pc) {
    int pointNum = pc.vertices.size();
    if (pointNum == 0)
        return false;

    std::ofstream fout;
    fout.open(filename.c_str(), std::ofstream::binary);
    if (fout.fail()){
        std::cerr << "file open error:" << filename << std::endl;
        return false;
    }

    fout << "ply\n";
    fout << "format ascii 1.0\n";
    fout << "element vertex " << pointNum << "\n";
    fout << "property float x\n";
    fout << "property float y\n";
    fout << "property float z\n";
    fout << "property uchar red\n";
    fout << "property uchar green\n";
    fout << "property uchar blue\n";
    fout << "end_header\n";

    for (int i = 0; i < pointNum; i++){
        Eigen::Vector3f& vertex = pc.vertices[i];
        cv::Vec3b& col = pc.colors[i];

        fout << vertex[0] << " " << vertex[1] << " " << vertex[2] << " " << static_cast<int>(col[2]) << " " << static_cast<int>(col[1]) << " " << static_cast<int>(col[0]) << "\n";
    }

    fout.close();
    return true;
}

void GeneratePointCloud() {
    std::string data_dir = DATA_DIR;

    // Load intrinsics
    Intrinsics intrinsics = LoadIntrinsics(data_dir);

    // Read depth
    // The depth stores the distance in millimeters as a 16bit.
    cv::Mat depth = cv::imread(GetFileName(data_dir, 0) + ".depth.png", cv::IMREAD_UNCHANGED);

    // Read color
    // 8-bit 3-channel color image
    cv::Mat RGB = cv::imread(GetFileName(data_dir, 0) + ".color.jpg", cv::IMREAD_UNCHANGED);

    PointCloud pc(intrinsics.width * intrinsics.height);

    // #######################################################################################
    // TODO: Create a pointcloud from the depth map by back-projecting
    //       it using the camera intrinsics
    // #######################################################################################
    for (int u = 0; u < intrinsics.width; u++) {
        for (int v = 0; v < intrinsics.height; v++) {
            auto z = depth.at<ushort>(v, u);
            auto x = (u - intrinsics.cx)*z/intrinsics.fx;
            auto y = (v - intrinsics.cy)*z/intrinsics.fy;

            pc.vertices.emplace_back(Eigen::Vector3f(x, y, z));

            pc.colors.emplace_back(RGB.at<cv::Vec3b>(v, u));

        }
    }

    std::cout << "Saving pointcloud ..." << std::endl;
    if (savePointCloudsPLY(data_dir + "/a.ply", pc))
        std::cout << "Done saving pointcloud." << std::endl;
}

void GeneratePointCloudFused() {
    // General parameters
    int num_frames = 10;
    std::string data_dir = DATA_DIR;

    // Load intrinsics
    Intrinsics intrinsics = LoadIntrinsics(data_dir);

    PointCloud pc(num_frames * intrinsics.width * intrinsics.height);

    // #######################################################################################
    // TODO: Create merged pointcloud of frames 0-9 in the global coordinate system using the camera poses
    //       Your can use the following helper function to load the poses for frame i
    //       Eigen::Matrix4f pose_camera2world = LoadPose(GetFileName(data_dir, i) + ".transform.txt");
    // #######################################################################################
    for (int i = 0; i < num_frames; i++) {
        cv::Mat depth = cv::imread(GetFileName(data_dir, i) + ".depth.png", cv::IMREAD_UNCHANGED);
        cv::Mat RGB = cv::imread(GetFileName(data_dir, i) + ".color.jpg", cv::IMREAD_UNCHANGED);
        Eigen::Matrix4f pose_camera2world = LoadPose(GetFileName(data_dir, i) + ".transform.txt");

        for (int u = 0; u < intrinsics.width; u++) {
            for (int v = 0; v < intrinsics.height; v++) {
                auto z_ = depth.at<ushort>(v, u);
                auto x_ = (u - intrinsics.cx)*z_/intrinsics.fx;
                auto y_ = (v - intrinsics.cy)*z_/intrinsics.fy;

                Eigen::Vector4f world_point = pose_camera2world*Eigen::Vector4f(x_, y_, z_, 1);

                pc.vertices.emplace_back(world_point.head<3>());

                pc.colors.emplace_back(RGB.at<cv::Vec3b>(v, u));
            }
        }
    }

    std::cout << "Saving fused pointcloud ..." << std::endl;
    if (savePointCloudsPLY(data_dir + "/b.ply", pc)) 
        std::cout << "Done saving pointcloud." << std::endl;
}

void GenerateLabelImage() {
    // General parameters
    int num_frames = 10;
    std::string data_dir = DATA_DIR;

    // Load intrinsics
    Intrinsics intrinsics = LoadIntrinsics(data_dir);

    cv::Mat RGB = cv::imread(GetFileName(data_dir, 0) + ".color.jpg", cv::IMREAD_UNCHANGED);

    cv::Mat labels_101(cv::Size(intrinsics.width, intrinsics.height), RGB.type(), cv::Scalar(0, 0, 0));
    cv::Mat labels_106(cv::Size(intrinsics.width, intrinsics.height), RGB.type(), cv::Scalar(0, 0, 0));

    // #######################################################################################
    // TODO: Merge point cloud of frames 0-9 with semantic labels
    // TODO: Project point cloud into frame 101
    // TODO: Project point cloud into frame 106
    // #######################################################################################
    PointCloud pc(num_frames * intrinsics.width * intrinsics.height);

    // Sementaic labels point cloud
    for (int i = 0; i < num_frames; i++) {
        cv::Mat depth = cv::imread(GetFileName(data_dir, i) + ".depth.png", cv::IMREAD_UNCHANGED);
        cv::Mat label = cv::imread(GetFileName(data_dir, i) + ".label.png", cv::IMREAD_UNCHANGED);
        Eigen::Matrix4f pose_camera2world = LoadPose(GetFileName(data_dir, i) + ".transform.txt");

        for (int u = 0; u < intrinsics.width; u++) {
            for (int v = 0; v < intrinsics.height; v++) {
                auto z_ = depth.at<ushort>(v, u);
                auto x_ = (u - intrinsics.cx)*z_/intrinsics.fx;
                auto y_ = (v - intrinsics.cy)*z_/intrinsics.fy;

                Eigen::Vector4f world_point = pose_camera2world*Eigen::Vector4f(x_, y_, z_, 1);

                pc.vertices.emplace_back(world_point.head<3>());
                pc.colors.emplace_back(label.at<cv::Vec3b>(v, u));
            }
        }
    }

    std::vector<Eigen::Matrix4f> poses;
    poses.emplace_back(LoadPose(GetFileName(data_dir, 101) + ".transform.txt").inverse());
    poses.emplace_back(LoadPose(GetFileName(data_dir, 106) + ".transform.txt").inverse());
    int num_current{0};

    for (const auto& loaded_pose : poses) {
        for (int i = 0; i < pc.vertices.size(); i++) {
            Eigen::Vector4f homogenous_point{pc.vertices.at(i)(0), pc.vertices.at(i)(1),
                                             pc.vertices.at(i)(2), 1};
            Eigen::Vector4f img_point = loaded_pose*homogenous_point;

            cv::Vec3b current_label = pc.colors.at(i);
            auto z = img_point(2);
            auto u = ((intrinsics.fx/z)*img_point(0)) + intrinsics.cx;
            auto v = ((intrinsics.fy/z)*img_point(1)) + intrinsics.cy;

            if ((u < intrinsics.width && u > 0) && (v < intrinsics.height && v > 0)) {
                if ((num_current == 0) &&
                        (labels_101.at<cv::Vec3b>(v, u) == cv::Vec3b(0, 0, 0))){
                    labels_101.at<cv::Vec3b>(v, u) = current_label;
                }
                else if ((num_current == 1) &&
                        (labels_106.at<cv::Vec3b>(v, u) == cv::Vec3b(0, 0, 0))){
                    labels_106.at<cv::Vec3b>(v, u) = current_label;
                }
            }
        }

        ++num_current;
    }

    cv::imshow("101", labels_101);
    cv::imshow("106", labels_106);
    cv::waitKey(0);

    if (!labels_101.empty() && !labels_106.empty()) {
        cv::imwrite(data_dir + "/c_101.png", labels_101);
        cv::imwrite(data_dir + "/c_106.png", labels_106);
    }
}

void GenerateDenseLabelImage() {
    // General parameters
    int num_frames = 10;
    std::string data_dir = DATA_DIR;

    // Load intrinsics
    Intrinsics intrinsics = LoadIntrinsics(data_dir);

    cv::Mat labels_101 = cv::imread(data_dir + "/c_101.png", cv::IMREAD_UNCHANGED);
    cv::Mat labels_106 = cv::imread(data_dir + "/c_106.png", cv::IMREAD_UNCHANGED);

    // #######################################################################################
    // TODO: Try to think of a way, how to generate a more dense label map for frame 106
    // #######################################################################################
    // Try opening morpholigical operation
    cv::Mat element = cv::getStructuringElement(0, cv::Size(3, 3));

    cv::morphologyEx(labels_101, labels_101, cv::MORPH_CLOSE, element);
    cv::morphologyEx(labels_106, labels_106, cv::MORPH_CLOSE, element);

    cv::imshow("101", labels_101);
    cv::imshow("106", labels_106);
    cv::waitKey(0);

    if (!labels_106.empty())
        cv::imwrite(data_dir + "/d.png", labels_106);
}

int main(int argc, char **argv) {
    // (a) Generate colored pointcloud from depth and rgb image
    GeneratePointCloud();

    // (b) Generate fused pointcloud in world coordinate system
    GeneratePointCloudFused();

    // (c) generate the label image for frame 101 (only pose is given) - projection
    GenerateLabelImage();
    
    // (d) Bonus: Generate dense label image
    GenerateDenseLabelImage();

    return 0;
}
