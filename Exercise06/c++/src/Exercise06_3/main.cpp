#include <iostream>
#include <Open3D/Open3D.h>


int main (int argc, char** argv)
{
    const std::string data_directory = DATA_DIR;

    // loop for depth images on multiple view
    for(int i=0; i<7; i++){

        // Loading each point cloud from ply file
        std::string filename_ply = data_directory + "/dwarf0" + std::to_string(i) + ".ply";
        
    
        // Estimate normals and down-sample point clouds so that the point density is 5mm by using VoxelDownSample


    // Register 7 point clouds on multiple views using ICP
    for(int i=1; i<7; i++){

    
    // Visualize the registered and merged point clouds


}
