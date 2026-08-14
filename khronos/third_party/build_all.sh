#!/bin/bash

# 开启严格模式：遇到任何命令执行失败立即退出
set -e

# 强制单线程编译，最大限度降低内存占用
JOBS=1

echo "=========================================="
echo " 开始批量编译 third_party 依赖库..."
echo " ⚠️ 当前并发数: -j$JOBS (单线程安全模式)"
echo "=========================================="

# 1. 编译 config_utilities
echo "[1/9] 正在编译 config_utilities..."
cd config_utilities/config_utilities
rm -rf build && mkdir -p build && cd build
cmake .. && make -j$JOBS && sudo make install
cd ../../..
echo "[1/9] config_utilities 编译成功！"

# 2. 编译 Spatial-Hash
echo "[2/9] 正在编译 Spatial-Hash..."
cd Spatial-Hash
rm -rf build && mkdir -p build && cd build
cmake .. && make -j$JOBS && sudo make install
cd ../../
echo "[2/9] Spatial-Hash 编译成功！"

# 3. 编译 gtsam-develop (关闭示例和测试以节省内存)
echo "[3/9] 正在编译 gtsam..."
cd gtsam
rm -rf build && mkdir -p build && cd build
cmake -DGTSAM_BUILD_EXAMPLES_ALWAYS=OFF -DGTSAM_BUILD_TESTS=OFF .. && make -j1$JOBS && sudo make install
cd ../../
echo "[3/9] gtsam-develop 编译成功！"

# 4. 编译 Spark-DSG
echo "[4/9] 正在编译 Spark-DSG..."
cd Spark-DSG
rm -rf build && mkdir -p build && cd build
cmake .. && make -j$JOBS && sudo make install
cd ../../
echo "[4/9] Spark-DSG 编译成功！"

# 5. 编译 pose_graph_tools
echo "[5/9] 正在编译 pose_graph_tools..."
cd pose_graph_tools/pose_graph_tools
rm -rf build && mkdir -p build && cd build
cmake .. && make -j$JOBS && sudo make install
cd ../../..
echo "[5/9] pose_graph_tools 编译成功！"

# 6. 编译 Kimera-RPGO (新增的底层依赖)
echo "[6/9] 正在编译 Kimera-RPGO..."
cd Kimera-RPGO
rm -rf build && mkdir -p build && cd build
cmake .. && make -j$JOBS && sudo make install
cd ../../
echo "[6/9] Kimera-RPGO 编译成功！"

# 7. 编译 Kimera-PGMO (依赖 Kimera-RPGO)
echo "[7/9] 正在编译 Kimera-PGMO..."
cd Kimera-PGMO/kimera_pgmo
rm -rf build && mkdir -p build && cd build
cmake .. && make -j$JOBS && sudo make install
cd ../../..
echo "[7/9] Kimera-PGMO 编译成功！"

# 8. 编译 TEASER++ (Hydra 的依赖)
echo "[8/9] 正在编译 TEASER++..."
cd ~/Khronos/khronos/third_party/TEASER-plusplus/build
rm -rf *
cmake -DBUILD_TESTING=OFF \
      -DFETCHCONTENT_SOURCE_DIR_PMC=/home/gdy/Khronos/khronos/third_party/pmc \
      -DFETCHCONTENT_SOURCE_DIR_TINYPLY=/home/gdy/Khronos/khronos/third_party/tinyply \
      -DFETCHCONTENT_SOURCE_DIR_SPECTRA=/home/gdy/Khronos/khronos/third_party/spectra \
      ..
make -j1
sudo make install
echo "[8/9] TEASER++ 编译成功！"

# 9. 编译 Hydra (记得把原来的 8/8 改成 9/9)
echo "[9/9] 正在编译 Hydra..."
cd Hydra
rm -rf build && mkdir -p build && cd build
cmake .. && make -j$JOBS && sudo make install
cd ../../
echo "[9/9] Hydra 编译成功！"

echo "=========================================="
echo " 🎉 所有依赖库编译并安装完成！"
echo "=========================================="