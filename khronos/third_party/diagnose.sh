for d in config_utilities/config_utilities Spatial-Hash Spark-DSG pose_graph_tools/pose_graph_tools Kimera-RPGO Kimera-PGMO/kimera_pgmo gtsam; do
  echo "=== $d ==="
  ls ~/Khronos/khronos/third_party/$d/build/install_manifest.txt 2>/dev/null && echo "已装" || echo "未装/找不到"
done
