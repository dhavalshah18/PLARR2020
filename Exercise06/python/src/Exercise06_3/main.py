import open3d as o3d
import numpy as np
from numpy.linalg import inv

if __name__ == '__main__':
   
    pcds=[]

    # loop for depth images on multiple view
    for i in range(0,7):
        # Loading each point cloud from ply file
        filename_ply = "data/dwarf%02d.ply"%(i)
        
    
        # Estimate normals and down-sample point clouds so that the point density is 5mm by using voxel_down_sample


    # Register 7 point clouds on multiple views using ICP
    for i in range(1,7):

    
    # Visualize the registered and merged point clouds


    
    
    



