import open3d as o3d

if __name__ == '__main__':

    # Load .pcd file to open3d.geometry.PointCloud
    filename_pcd = '../../data/bun0.pcd'
    pc = o3d.io.read_point_cloud(filename_pcd)

    # Visualize the point clouds using draw_geometries
    o3d.visualization.draw_geometries([pc])

    # Compute normals of the point clouds
    pc.estimate_normals()

    # Visualize the point clouds with computed normals
    o3d.visualization.draw_geometries([pc], point_show_normal=True)
