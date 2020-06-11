import open3d as o3d
import numpy as np
from numpy.linalg import inv
import copy

if __name__ == '__main__':
   
    pcds = []
    voxel_size = 0.005
    threshold = 0.001
    regs = []
    pcs = []
    pc_merged = []

    # loop for depth images on multiple view
    for i in range(0, 7):
        # Loading each point cloud from ply file
        filename_ply = "../../data/dwarf%02d.ply" % i
        
        pc = o3d.io.read_point_cloud(filename_ply)

        # Estimate normals and down-sample point clouds so that the point density is 5mm by using voxel_down_sample
        pc_down = pc.voxel_down_sample(voxel_size=voxel_size)
        pc_down.estimate_normals(o3d.geometry.KDTreeSearchParamHybrid(radius=voxel_size*2, max_nn=30))

        pcs.append(pc)
        pcds.append(pc_down)

    # Register 7 point clouds on multiple views using ICP
    search_param = o3d.geometry.KDTreeSearchParamHybrid(radius=voxel_size*5, max_nn=500)

    pc_merged.append(pcs[0])

    for i in range(1, 7):
        pc_ref_fpfh = o3d.registration.compute_fpfh_feature(pcds[i], search_param)
        best_score = -1

        for pc in pcds:
            pc_tar_fpfh = o3d.registration.compute_fpfh_feature(pc)
            result = o3d.registration.registration_fast_based_on_feature_matching(
                pcds[i], pc, pc_ref_fpfh, pc_tar_fpfh,
                o3d.registration.FastGlobalRegistrationOption(maximum_correspondence_distance=voxel_size*0.5))

            trans_init = result.transformation

            reg_p2p = o3d.registration.registration_icp(
                pcds[i], pc, threshold, init=trans_init,
                estimation_method=o3d.registration.TransformationEstimationPointToPlane(),
                criteria=o3d.registration.ICPConvergenceCriteria(max_iteration=50))

        pc_temp = copy.deepcopy(pcds[i])
        pc_merged.append(pc_temp.transform(reg_p2p.transformation))

        # o3d.visualization.draw_geometries([pcds[0], pcds[i]])
        # o3d.visualization.draw_geometries([pcds[0], pc_merged[i]])

        # print("ICP")
        # print(reg_p2p)
        # print()

    # Visualize the registered and merged point clouds
    # o3d.visualization.draw_geometries(pcs)
    o3d.visualization.draw_geometries(pc_merged)

    
    
    



