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

    savePointCloudsPLY(data_dir + "/a.ply", pc);
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

    std::cout << "Saving fused pointcloud ..." << std::endl;
    if (savePointCloudsPLY(data_dir + "/b.ply", pc)) 
        std::cout << "Done saving pointloud." << std::endl;
}

void GenerateLabelImage() {
    // General parameters
    int num_frames = 10;
    std::string data_dir = DATA_DIR;

    // Load intrinsics
    Intrinsics intrinsics = LoadIntrinsics(data_dir);

    cv::Mat labels_101, labels_106;

    // #######################################################################################
    // TODO: Merge point cloud of frames 0-9 with semantic labels
    // TODO: Project point cloud into frame 101
    // TODO: Project point cloud into frame 106
    // #######################################################################################
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

    cv::Mat labels_106;

    // #######################################################################################
    // TODO: Try to think of a way, how to generate a more dense label map for frame 106
    // #######################################################################################
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
