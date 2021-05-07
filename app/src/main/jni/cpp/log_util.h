//
// Created by baiduiov on 2021/5/6.
//

#ifndef ANDROID_OPENCL_LOG_UTIL_H
#define ANDROID_OPENCL_LOG_UTIL_H
#include <string>

#include <android/log.h>

#define TAG "OpenCL_Naive"
//#define WRAP_TAG(tag) (std::string("OpenCL-Naive-") + tag).c_str()
#define LOGI(tag, ...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(tag, ...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGD(tag, ...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(tag, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGF(tag, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGV(tag, ...)
#endif //ANDROID_OPENCL_LOG_UTIL_H
