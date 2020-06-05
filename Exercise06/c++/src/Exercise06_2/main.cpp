#include <iostream>
#include <Open3D/Open3D.h>

int main (int argc, char** argv)
{
    const std::string data_directory = DATA_DIR;
    
    // Load point clouds and estimate their normals
    std::string fileNameRef = data_directory + "/pointcloud_chair0.pcd";
    std::string fileNameTar = data_directory + "/pointcloud_chair1.pcd";
    

    // down sample the point cloud pair


    // calculate FPFH features for each downsampled point cloud


    // Register two point clouds using feature matching with ransac
    

    // Transform the target point cloud using the estimated transform


    // Visualize the registered point clouds

    
    return 0;
}


