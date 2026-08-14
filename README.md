1. 原始Khronos运行方式请切换到ROS2分支，查看原版README.

2. main分支上仅保留了算法核心逻辑，用于进行算法部分的开发。适合于进行纯数学与几何计算的数值实验。

3. main的使用方式：先运行/home/gdy/Khronos/khronos/third_party/build_all.sh，然后回退一级，cmake && make khronos。

4. smoke test用于测试整个pipeline能否跑通。

5. 目前已有的数据集：
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