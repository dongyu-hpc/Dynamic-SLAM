cmake_minimum_required(VERSION 3.22.1)
project(khronos_smoke_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(khronos REQUIRED)
find_package(config_utilities REQUIRED)

add_executable(smoke_test main.cpp)
target_link_libraries(smoke_test PRIVATE khronos::khronos config_utilities::config_utilities)
