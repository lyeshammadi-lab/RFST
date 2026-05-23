# tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(DSP_Sandbox VERSION 1.0)

# Il doit trouver Eigen tout seul puisqu'il est indépendant
find_package(Eigen3 REQUIRED)

# Ton exécutable de test
add_executable(tracking_main test_tracking.cpp)

# Les inclusions
target_include_directories(tracking_main PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${EIGEN3_INCLUDE_DIR}
)