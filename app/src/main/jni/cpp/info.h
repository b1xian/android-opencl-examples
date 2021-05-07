//
// Created by baiduiov on 2021/5/6.
//

#ifndef ANDROID_OPENCL_INFO_H
#define ANDROID_OPENCL_INFO_H

#include <string>

#include <CL/opencl.h>

#include "log_util.h"
#include "type.h"

std::string display_platform_info(cl_platform_id platform_id,
                                  cl_platform_info platform_info_type, std::string info_name) {
    size_t size;
    cl_int err;
    err = clGetPlatformInfo(platform_id, platform_info_type, 0, NULL, &size);
    if (err != CL_SUCCESS) {
        LOGI(TAG, "clGetPlatformInfo failed : %s", info_name.c_str());
        return "";
    }
    char *info = (char*)malloc(sizeof(char) * size);
    clGetPlatformInfo(platform_id, platform_info_type, size, info, NULL);
    LOGI(TAG, "clGetPlatformInfo %s : %s", info_name.c_str(), info);
    std::string str(info);
    free(info);
    return str;
}

std::string show_platform_info(cl_platform_id *platform_ids, cl_uint num_platforms) {
    std::string info_str;
    info_str.append("platform_info:\t\n");
    for (int i = 0; i < num_platforms; ++i) {
        LOGI(TAG, "show_platform_info platform %d", i);
        std::string name = display_platform_info(platform_ids[i], CL_PLATFORM_NAME, "name");
        info_str.append("name:").append(name).append("\n");
        std::string vendor = display_platform_info(platform_ids[i], CL_PLATFORM_VENDOR, "vendor");
        info_str.append("vendor:").append(vendor).append("\n");
        std::string version = display_platform_info(platform_ids[i], CL_PLATFORM_VERSION, "version");
        info_str.append("version:").append(version).append("\n");
        std::string profile = display_platform_info(platform_ids[i], CL_PLATFORM_PROFILE, "profile");
        info_str.append("profile:").append(profile).append("\n");
        std::string extensions = display_platform_info(platform_ids[i], CL_PLATFORM_EXTENSIONS, "extensions");
        info_str.append("extensions:").append(extensions).append("\n");
    }
    return info_str;
}


std::string display_device_info(cl_device_id device_id,
                                cl_device_info device_info_type,
                                std::string info_name,
                                InfoType info_type) {

    cl_int err;

    if (info_type == InfoType::type_int) {
        cl_int int_info;
        err = clGetDeviceInfo(device_id, device_info_type,
                              sizeof(cl_uint), &int_info, NULL);
        if (err == CL_SUCCESS) {
            LOGI(TAG, "clGetDeviceInfo, %s: %d", info_name.c_str(), int_info);
            return std::to_string(int_info);
        }
    } else {
        size_t size;
        err = clGetDeviceInfo(device_id, device_info_type, 0, NULL, &size);
        if (err != CL_SUCCESS) {
            LOGI(TAG, "clGetPlatformInfo failed : %s", info_name.c_str());
            return "";
        }
        char* str_info = (char *)malloc(sizeof(char) * size);
        err = clGetDeviceInfo(device_id, device_info_type,
                              size, str_info, NULL);
        if (err == CL_SUCCESS) {
            LOGI(TAG, "clGetDeviceInfo, %s: %s", info_name.c_str(), str_info);
        }
        std::string info(str_info);
        free(str_info);
        return info;
    }
    if (err != CL_SUCCESS) {
        LOGI(TAG, "clGetDeviceInfo failed : %s", info_name.c_str());
        return "";
    }
}

std::string show_device_info(cl_device_id device_id) {
    LOGI(TAG, "show_device_info");
    std::string info_str;
    info_str.append("device_info:\t\n");
    std::string name = display_device_info(device_id, CL_DEVICE_NAME, "name", InfoType::type_str);
    info_str.append("name:").append(name).append("\n");
    std::string vendor = display_device_info(device_id, CL_DEVICE_VENDOR, "vendor", InfoType::type_str);
    info_str.append("vendor:").append(vendor).append("\n");
    std::string device_version = display_device_info(device_id, CL_DEVICE_VERSION, "device version", InfoType::type_str);
    info_str.append("device_version:").append(device_version).append("\n");
    std::string driver_version = display_device_info(device_id, CL_DRIVER_VERSION, "driver version", InfoType::type_str);
    info_str.append("driver_version:").append(driver_version).append("\n");
    std::string profile = display_device_info(device_id, CL_DEVICE_PROFILE, "profile", InfoType::type_str);
    info_str.append("profile:").append(profile).append("\n");
    std::string extensions = display_device_info(device_id, CL_DEVICE_EXTENSIONS, "extensions", InfoType::type_str);
    info_str.append("extensions:").append(extensions).append("\n");
    std::string max_compute_units = display_device_info(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, "max compute units", InfoType::type_int);
    info_str.append("max_compute_units:").append(max_compute_units).append("\n");
    return info_str;
}

#endif //ANDROID_OPENCL_INFO_H
