TODO 后续整理可以一键运行的脚本。

1. 原始代码运行方式请切换到ROS2分支，查看原版README.

2. main分支作为clean version，仅保留了算法核心逻辑，用于进行方法部分的开发，使用cmake && make方法。适合于进行纯数学与几何计算的数值实验。实际部署运行请将变动更新到ros2并构建ros2 workspace进行部署，使用colcon进行编译和安装。main中删除掉的有：evaluation、visualization、ros2.

3. main的使用方式：先运行/third_party/build_all.sh，然后回退一级，cmake && make *。

4. smoke test用于测试整个pipeline能否跑通。

5. 目前已有的数据集
    ```
    # /tesse/odom: nav_msgs/msg/Odometry (8724 msgs)
    # /tesse/odom/noisy: nav_msgs/msg/Odometry (8724 msgs)
    # /tesse/left_cam/camera_info: sensor_msgs/msg/CameraInfo (5235 msgs)
    # /tesse/right_cam/camera_info: sensor_msgs/msg/CameraInfo (1745 msgs)
    # /tesse/right_cam/mono/image_raw: sensor_msgs/msg/Image (1745 msgs)
    # /tesse/depth_cam/mono/image_raw: sensor_msgs/msg/Image (1745 msgs)
    # /tesse/seg_cam/converted/image_raw: sensor_msgs/msg/Image (1745 msgs)
    # /tesse/seg_cam/rgb/image_raw: sensor_msgs/msg/Image (1745 msgs)
    # /tesse/left_cam/mono/image_raw: sensor_msgs/msg/Image (1745 msgs)
    # /tesse/right_cam/rgb/image_raw: sensor_msgs/msg/Image (1745 msgs)
    # /tesse/left_cam/rgb/image_raw: sensor_msgs/msg/Image (1745 msgs)
    # /tesse/imu/clean/imu: sensor_msgs/msg/Imu (8723 msgs)
    # /tesse/imu/noisy/imu: sensor_msgs/msg/Imu (8723 msgs)
    # /tesse/imu/noisy/biases/gyro: geometry_msgs/msg/Vector3Stamped (8723 msgs)
    # /tesse/imu/noisy/biases/accel: geometry_msgs/msg/Vector3Stamped (8723 msgs)
    # /tf_static: tf2_msgs/msg/TFMessage (1 msgs)
    # /clock: rosgraph_msgs/msg/Clock (8740 msgs)
    ```

6. 关于dependency list 参考 /third_party/dependency_list.txt
    ```
    1- git:
        local-name: config_utilities
        uri: GitHub - MIT-SPARK/config_utilities: Automatic C++ config structs and tools.
        version: main
    2- git:
        local-name: hydra
        uri: GitHub - MIT-SPARK/Hydra: A system for building 3D Scene Graphs from sensor data in real-time
        version: main
    3- git:
        local-name: hydra_ros
        uri: GitHub - MIT-SPARK/Hydra-ROS: Hydra ROS Interface
        version: main
    4- git:
        local-name: kimera_pgmo
        uri: GitHub - MIT-SPARK/Kimera-PGMO
        version: ros2
    5- git:
        local-name: kimera_rpgo
        uri: GitHub - MIT-SPARK/Kimera-RPGO: Robust Pose Graph Optimization
        version: develop
    6- git:
        local-name: pose_graph_tools
        uri: GitHub - MIT-SPARK/pose_graph_tools
        version: ros2
    7- git:
        local-name: semantic_inference
        uri: GitHub - MIT-SPARK/semantic_inference: ROS interface to closed and open-set semantic segmentation mo
        version: main
    8- git:
        local-name: spark_dsg
        uri: GitHub - MIT-SPARK/Spark-DSG: Scene Graph API (C++ and Python)
        version: main
    9- git:
        local-name: spatial_hash
        uri: GitHub - MIT-SPARK/Spatial-Hash: Minimal C++ library for spatial data structures based on voxel-bloc
        version: main
    10- git:
        local-name: teaser_plusplus
        uri: GitHub - MIT-SPARK/TEASER-plusplus: A fast and robust point cloud registration library
        version: master
    11- git:
        local-name: ianvs
        uri: GitHub - MIT-SPARK/Ianvs: ROS2 Transition Package
        version: main1

    ```
