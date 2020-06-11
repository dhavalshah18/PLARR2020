import open3d as o3d
import time
import numpy as np
from numpy.linalg import inv
import copy

if __name__ == '__main__':

    # Load point clouds and estimate their normals
    filename_ref = '../../data/pointcloud_chair0.pcd'
    filename_tar = '../../data/pointcloud_chair1.pcd'

    voxel_size = 0.05

    pc_ref = o3d.io.read_point_cloud(filename_ref)
    pc_tar = o3d.io.read_point_cloud(filename_tar)

    pc_ref.estimate_normals(o3d.geometry.KDTreeSearchParamHybrid(radius=voxel_size*2, max_nn=30))
    pc_tar.estimate_normals(o3d.geometry.KDTreeSearchParamHybrid(radius=voxel_size*2, max_nn=30))

    # down sample the point cloud pair
    pc_ref_down = pc_ref.voxel_down_sample(voxel_size=voxel_size)
    pc_tar_down = pc_tar.voxel_down_sample(voxel_size=voxel_size)

    pc_ref_down.estimate_normals(o3d.geometry.KDTreeSearchParamHybrid(radius=voxel_size*2, max_nn=30))
    pc_tar_down.estimate_normals(o3d.geometry.KDTreeSearchParamHybrid(radius=voxel_size*2, max_nn=30))

    # calculate FPFH features for each downsampled point cloud
    search_param = o3d.geometry.KDTreeSearchParamHybrid(radius=voxel_size*5, max_nn=100)
    pc_ref_fpfh = o3d.registration.compute_fpfh_feature(pc_ref_down, search_param)
    pc_tar_fpfh = o3d.registration.compute_fpfh_feature(pc_tar_down, search_param)

    # Register two point clouds using feature matching with ransac
    start = time.time()
    result_ransac = o3d.registration.registration_ransac_based_on_feature_matching(
        pc_ref_down, pc_tar_down, pc_ref_fpfh, pc_tar_fpfh, voxel_size*1.5,
        o3d.registration.TransformationEstimationPointToPoint(False), 4,
        [o3d.registration.CorrespondenceCheckerBasedOnEdgeLength(0.9),
         o3d.registration.CorrespondenceCheckerBasedOnDistance(voxel_size*1.5)],
        o3d.registration.RANSACConvergenceCriteria(4000000, 500))
    end = time.time()

    print("RANSAC algorithm time: ", end-start)

    print(result_ransac)

    # Transform the target point cloud using the estimated transform
    pc_ref_transformed = copy.deepcopy(pc_ref)
    pc_ref_transformed.transform(result_ransac.transformation)

    # Visualize the registered point clouds
    o3d.visualization.draw_geometries([pc_ref_transformed, pc_tar])

    # Register two point clouds using feauture matching with fast
    start = time.time()
    result_fast = o3d.registration.registration_fast_based_on_feature_matching(
        pc_ref_down, pc_tar_down, pc_ref_fpfh, pc_tar_fpfh,
        o3d.registration.FastGlobalRegistrationOption(maximum_correspondence_distance=voxel_size*0.5))
    end = time.time()

    print("Fast algorithm time: ", end-start)

    print(result_fast)

    pc_ref_transformed_fast = copy.deepcopy(pc_ref)
    pc_ref_transformed_fast.transform(result_fast.transformation)

    o3d.visualization.draw_geometries([pc_ref_transformed_fast, pc_tar])
