//
// Created by baiduiov on 2021/5/7.
//

#ifndef ANDROID_OPENCL_CONV_H
#define ANDROID_OPENCL_CONV_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <CL/opencl.h>

const char *kernel_source_conv = ""
                                 "__kernel void convolve(\n"
                                 "    const __global uint * const input,\n"
                                 "    __constant uint * const mask,\n"
                                 "    __global uint * const output,\n"
                                 "    const int input_width,\n"
                                 "    const int mask_width) {\n"
                                 "    const int x = get_global_id(0);\n"
                                 "    const int y = get_global_id(1);\n"
                                 "\n"
                                 "    uint sum = 0;\n"
                                 "    for (int r = 0; r < mask_width; r++) {\n"
                                 "        const int idx_int_mp = (y + r) * input_width + x;\n"
                                 "        for (int c = 0; c < mask_width; c++) {\n"
                                 "            sum += mask[(r * mask_width) + c] * input[idx_int_mp + c];\n"
                                 "        }\n"
                                 "    }\n"
                                 "    output[y * get_global_size(0) + x] = sum;\n"
                                 "}\n";

const unsigned int input_signal_width = 8;
const unsigned int input_signal_height = 8;

unsigned int input_signal[input_signal_width][input_signal_height] = {
        {3, 1, 1, 4, 8, 2, 1, 3},
        {4, 2, 1, 1, 2, 1, 2, 3},
        {4,4,4,4,3,2,2,2},
        {9,8,3,8,9,0,0,0},
        {9,3,3,9,0,0,0,0},
        {0,9,0,8,0,0,0,0},
        {3,0,8,8,9,4,4,4},
        {5,9,8,1,8,1,1,1}
};

const unsigned int output_signal_width = 6;
const unsigned int output_signal_height = 6;

unsigned int output_signal[output_signal_width][output_signal_height];

const unsigned int mask_width = 3;
const unsigned int mask_height = 3;

unsigned int mask[mask_width][mask_height] = {
        {1,1,1},{1,0,1},{1,1,1}
};

inline void check_err(cl_int err, const char *name) {
    if (err != CL_SUCCESS) {
        LOGI(TAG, "ERROR:%s %d", name, err);
        exit(EXIT_FAILURE);
    }
}

void CL_CALLBACK context_call_back(const char *err_info, const void *private_info,
        size_t cb, void *user_data) {
    LOGI(TAG, "error during context use:%s", err_info);
    exit(EXIT_FAILURE);
}

void convolution() {
    LOGI(TAG, "conv..");
    cl_int err_num;
    cl_uint num_platforms;
    cl_uint num_devices;
    cl_context context = NULL;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem input_signal_buffer;
    cl_mem output_signal_buffer;
    cl_mem mask_buffer;

    cl_platform_id platform;
    cl_device_id device_id;
    err_num = clGetPlatformIDs(1, &platform, &num_platforms);
    check_err(err_num, "clGetPlatformIDs");

    err_num = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    check_err(err_num, "clGetDeviceIDs");

    context = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &err_num);
    check_err(err_num, "clCreateContext");

//    std::ifstream src_file("./conv.cl");
//    check_err(src_file.is_open() ? CL_SUCCESS : -1, "read src file failed");
//    std::string src_prog(std::istreambuf_iterator<char>(src_file),
//                         (std::istreambuf_iterator<char>()));
//
//    const char* src = src_prog.c_str();
//    size_t length = src_prog.length();

    program = clCreateProgramWithSource(context, 1, &kernel_source_conv, NULL, &err_num);
    check_err(err_num, "clCreateProgramWithSource");

    err_num = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    check_err(err_num, "clBuildProgram");

    kernel = clCreateKernel(program, "convolve", &err_num);
    check_err(err_num, "clCreateKernel");

    input_signal_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                                         sizeof(cl_uint) * input_signal_height * input_signal_width,
                                         static_cast<void*>(input_signal), &err_num);
    check_err(err_num, "clCreateBuffer input_signal_buffer");
    mask_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                                 sizeof(cl_uint) * mask_height * mask_width,
                                 static_cast<void*>(mask), &err_num);
    check_err(err_num, "clCreateBuffer mask_buffer");
    output_signal_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                          sizeof(cl_uint) * output_signal_height * output_signal_width,
                                          NULL, &err_num);
    check_err(err_num, "clCreateBuffer output_signal_buffer");

    queue = clCreateCommandQueue(context, device_id, 0, &err_num);
    check_err(err_num, "clCreateCommandQueue");

    err_num = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_signal_buffer);
    err_num = clSetKernelArg(kernel, 1, sizeof(cl_mem), &mask_buffer);
    err_num = clSetKernelArg(kernel, 2, sizeof(cl_mem), &output_signal_buffer);
    err_num = clSetKernelArg(kernel, 3, sizeof(cl_uint), &input_signal_width);
    err_num = clSetKernelArg(kernel, 4, sizeof(cl_uint), &mask_width);
    check_err(err_num, "clSetKernelArg");

    const size_t global_work_size[1] = {output_signal_width * output_signal_height};
    const size_t local_work_size[1] = {1};

    err_num = clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
                                     global_work_size, local_work_size, 0, NULL, NULL);
    check_err(err_num, "clEnqueueNDRangeKernel");

    err_num = clEnqueueReadBuffer(queue, output_signal_buffer, CL_TRUE, 0,
                                  sizeof(cl_uint) * output_signal_height * output_signal_width,
                                  output_signal, 0, NULL, NULL);
    check_err(err_num, "clEnqueueReadBuffer");

    for(int y = 0; y < output_signal_height; ++y) {
        for(int x = 0; x < output_signal_width; ++x) {
            LOGI(TAG, "%d", output_signal[x][y]);
        }
        LOGI(TAG, "\n");
    }

    // release OpenCL resources
    clReleaseMemObject(input_signal_buffer);
    clReleaseMemObject(output_signal_buffer);
    clReleaseMemObject(mask_buffer);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    //release host memory
//    delete[] input_signal;
//    delete[] output_signal;
//    delete[] mask;
}

#endif //ANDROID_OPENCL_CONV_H
