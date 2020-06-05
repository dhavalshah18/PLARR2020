#include <iostream>
#include <Core/Core.h>
#include <IO/IO.h>
#include <Visualization/Visualization.h>
#include <Registration/Registration.h>


int main (int argc, char** argv)
{
    
    std::string projectSrcDir = PROJECT_SOURCE_DIR;

    // loop for depth images on multiple view
    for(int i=0; i<7; i++){

        // Loading each point cloud from ply file
	std::string filename_ply = projectSrcDir + "/data/dwarf0" + std::to_string(i) + ".ply";
        
    
        // Estimate normals and down-sample point clouds so that the point density is 5mm by using VoxelDownSample


    // Register 7 point clouds on multiple views using ICP
    for(int i=1; i<7; i++){

    
    // Visualize the registered and merged point clouds


}
