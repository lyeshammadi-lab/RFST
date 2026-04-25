add_executable(gsctracking mains_test.cpp)


target_include_directories(gsctracking PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${EIGEN3_INCLUDE_DIR}
)