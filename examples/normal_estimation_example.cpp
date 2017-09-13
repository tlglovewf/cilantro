#include <cilantro/normal_estimation.hpp>
#include <cilantro/ply_io.hpp>
#include <cilantro/visualizer.hpp>
#include <iostream>

#include <cilantro/voxel_grid.hpp>

#include <ctime>

int main(int argc, char ** argv) {
    clock_t begin, end;
    double kd_tree_time, estimation_time;

    PointCloud cloud;
    readPointCloudFromPLYFile(argv[1], cloud);

    cloud.normals.clear();

    VoxelGrid vg(cloud, 0.005);
    cloud = vg.getDownsampledCloud();

    begin = clock();
    KDTree3D tree(cloud.points);
    NormalEstimation ne(cloud, tree);
    end = clock();
    kd_tree_time = 1000.0*double(end - begin) / CLOCKS_PER_SEC;

    begin = clock();
    ne.estimateNormalsInPlace(KDTree3D::Neighborhood(KDTree3D::NeighborhoodType::KNN_IN_RADIUS, 7, 0.01));
//    ne.estimateNormalsInPlaceKNNInRadius(7, 0.01);
//    ne.estimateNormalsInPlaceRadius(0.01);
//    ne.estimateNormalsInPlaceKNN(7);
    end = clock();
    estimation_time = 1000.0*double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "kd-tree time: " << kd_tree_time << std::endl;
    std::cout << "Estimation time: " << estimation_time << std::endl;

    Visualizer viz("win", "disp");

    viz.addPointCloud("cloud_d", cloud);
    viz.addPointCloudNormals("normals_d", cloud);

    while (!viz.wasStopped()){
        viz.spinOnce();
    }

    return 0;
}
