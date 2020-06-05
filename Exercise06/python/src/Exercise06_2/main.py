import open3d as o3d
import numpy as np
from numpy.linalg import inv

if __name__ == '__main__':

    # Load point clouds and estimate their normals
    filename_ref='data/pointcloud_chair0.pcd'
    filename_tar='data/pointcloud_chair1.pcd'


    # down sample the point cloud pair


    # calculate FPFH features for each downsampled point cloud


    # Register two point clouds using feature matching with ransac
    

    # Transform the target point cloud using the estimated transform


    # Visualize the registered point clouds

    
    
    