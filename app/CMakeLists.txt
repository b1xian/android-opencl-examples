project(test_opencl)

cmake_minimum_required(VERSION 3.6.0-rc2)

# opencl
include_directories(${CMAKE_SOURCE_DIR}/src/main/jni/include/opencl12)
SET(LIB_NAME native-opencl-lib)
SET(opencl_lib_dir ${CMAKE_SOURCE_DIR}/libs/opencl/libOpenCL.so)
ADD_LIBRARY(opencl_lib STATIC IMPORTED)
SET_TARGET_PROPERTIES(opencl_lib PROPERTIES IMPORTED_LOCATION ${opencl_lib_dir})

# src
aux_source_directory(src/main/jni/cpp SRC_DIR)
add_library(${LIB_NAME}
            SHARED
            ${SRC_DIR}
        )
target_link_libraries(${LIB_NAME} log opencl_lib)