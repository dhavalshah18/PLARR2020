#include <iostream>
#include <Core/Core.h>
#include <IO/IO.h>
#include <Visualization/Visualization.h>
#include <Registration/Registration.h>


int main (int argc, char** argv)
{
    std::string projectSrcDir = PROJECT_SOURCE_DIR;
    
    // Load point clouds and estimate their normals
    std::string fileNameRef = projectSrcDir + "/data/pointcloud_chair0.pcd";
    std::string fileNameTar = projectSrcDir + "/data/pointcloud_chair1.pcd";
    

    // down sample the point cloud pair


    // calculate FPFH features for each downsampled point cloud


    // Register two point clouds using feature matching with ransac
    

    // Transform the target point cloud using the estimated transform


    // Visualize the registered point clouds

    
    return 0;
}


