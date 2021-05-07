//
// Created by baiduiov on 2021/5/6.
//

#ifndef ANDROID_OPENCL_VEC_ADD_H
#define ANDROID_OPENCL_VEC_ADD_H

#include <string>

#include <CL/opencl.h>

#include "log_util.h"


const char *kernel_source_vec_add =
        "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n"
        "__kernel void vecAdd(  __global float *a,\n"
        "                       __global float *b,\n"
        "                       __global float *c,\n"
        "                       const unsigned int n)\n"
        "{\n"
        "    int id = get_global_id(0);\n"
        "    if (id < n) {\n"
        "        c[id] = a[id] * b[id];\n"
        "    }\n"
        "}\n";

void vector_add() {
    // Length of vectors
    unsigned int n = 100000;

    size_t bytes = n * sizeof(float);
    // Host input vectors
    float *h_a = new float[n];
    float *h_b = new float[n];
    // Host output vector
    float *h_c = new float[n];

    // Initialize vectors on host
    int i;
    for( i = 0; i < n; i++ ) {
        h_a[i] = 0 + i;
        h_b[i] = 0 - i;
    }


    // Device input buffers
    cl_mem d_a;
    cl_mem d_b;
    // Device output buffer
    cl_mem d_c;


    size_t globalSize, localSize;// Number of work items in each local work group
    localSize = 64;
    // Number of total work items - localSize must be devisor
    globalSize = 100032; // hardcoded to prevent needing math.h


    cl_int err;
    int gpu = 1;
    cl_uint num_platforms;
    cl_device_id device_id;             // compute device id
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel

    cl_platform_id platform;
    err = clGetPlatformIDs(1, &platform, &num_platforms);
    if (err != CL_SUCCESS) {
        LOGI(TAG, "Error: Failed to get platform id!");
    } else {
        LOGI(TAG, "get platform id success!, total platforms:%d", num_platforms);
    }

    // err = clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS) {
        LOGI(TAG, "Error: Failed to create a device group!");
    } else {
        LOGI(TAG, "create device group success!");
    }

    context = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &err);
    if (err != CL_SUCCESS) {
        LOGI(TAG, "Error: Failed to create context!");
    } else {
        LOGI(TAG, "create context success!");
    }

    cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &err);
    if (err != CL_SUCCESS) {
        LOGI(TAG, "Error: Failed to create queue!");
    } else {
        LOGI(TAG, "create queue success!");
    }

    clock_t start;
    start = clock();
    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1, &kernel_source_vec_add, NULL, &err);
    if (program == nullptr) {
        LOGI(TAG, "Error: Failed to create Program!");
    } else {
        LOGI(TAG, "create Program success!");
    }

    // Build the program executable
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        LOGI(TAG, "Error: Failed to Build Program!");
    } else {
        LOGI(TAG, "Build Program success!");
    }

    // Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "vecAdd", nullptr);
    if (kernel == nullptr) {
        LOGI(TAG, "Error: Failed to create Kernel!");
    } else {
        LOGI(TAG, "create Kernel success!");
    }

    d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, h_a, NULL);
    d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, h_b, NULL);
    d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, NULL);

    // Set the arguments to our compute kernel function
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c);
    err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &n);

    // Execute the kernel over the entire range of the data set
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize,
                                 0, NULL, NULL);

    // Wait for the command queue to get serviced before reading back results
    clFinish(queue);

    // Read the results from the device
    clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0,
                        bytes, h_c, 0, NULL, NULL );

    // Sum up vector c and print result divided by n, this should equal 0 within error
//    float sum = 0;
//    for(i = 0; i < n; i++)
//        sum += h_c[i];
//    LOGI("final result: %f\n", sum / (float)n);

    // release OpenCL resources
    clReleaseMemObject(d_a);
    clReleaseMemObject(d_b);
    clReleaseMemObject(d_c);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    //release host memory
    delete[] h_a;
    delete[] h_b;
    delete[] h_c;
    LOGI(TAG, "OpenCL add cost:%f", (double)(clock() - start) / CLOCKS_PER_SEC * 1000);
}

#endif //ANDROID_OPENCL_VEC_ADD_H
