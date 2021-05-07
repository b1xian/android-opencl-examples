//
// Created by b1xian on 2021/4/30.
//
#include <jni.h>
#include <string>

#include <CL/opencl.h>

#include "log_util.h"
#include "type.h"
#include "info.h"
#include "vec_add.h"
#include "conv.h"

#ifdef __cplusplus
    extern "C" {
#endif

const char *g_jni_class = "com/example/opencl/Introduce";


JNIEXPORT jstring show_info(JNIEnv *env, jobject jo) {
    LOGI(TAG, "show_info");

    // platform info
    cl_int err;
    cl_uint num_platforms;
    cl_platform_id *platform_ids;

    err = clGetPlatformIDs(0, NULL, &num_platforms);
    if (err != CL_SUCCESS) throw "Error: clGetPlatformIDs, err code: " + std::to_string(err);
    LOGI(TAG, "clGetPlatformIDs success!, number of platforms:%d", num_platforms);

    platform_ids = (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id));

    err = clGetPlatformIDs(num_platforms, platform_ids, NULL);
    if (err != CL_SUCCESS) throw "Error: clGetPlatformIDs, err code: " + std::to_string(err);
    LOGI(TAG, "clGetPlatformIDs success!, number of platforms:%d", num_platforms);

    std::string platform_info_str = show_platform_info(platform_ids, num_platforms);

    // device info
    cl_uint num_devices;
    cl_device_id devices_ids[1];
    err = clGetDeviceIDs(platform_ids[0], CL_DEVICE_TYPE_GPU, 0 , NULL, &num_devices);
    if (err != CL_SUCCESS) throw "Error: clGetDeviceIDs, err code: " + std::to_string(err);
    LOGI(TAG, "clGetDeviceIDs success!, number of devices:%d", num_devices);

    err = clGetDeviceIDs(platform_ids[0], CL_DEVICE_TYPE_GPU, 1, &devices_ids[0], NULL);
    if (err != CL_SUCCESS) throw "Error: clGetDeviceIDs, err code: " + std::to_string(err);

    std::string device_info_str = show_device_info(devices_ids[0]);
    return env->NewStringUTF(device_info_str.c_str());
}

JNIEXPORT void vec_add(JNIEnv* env, jobject jo) {
    vector_add();
}

JNIEXPORT void conv(JNIEnv* env, jobject jo) {
    convolution();
}

static JNINativeMethod jniNativeMethods[] = {
        {"showInfo", "()Ljava/lang/String;", (void *) show_info},
        {"vecAdd", "()V", (void *) vec_add},
        {"conv", "()V", (void *) conv},

};

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return JNI_ERR;
    }
    jclass clazz = env->FindClass(g_jni_class);
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    jint ret = (env)->RegisterNatives(clazz, jniNativeMethods,
                                      sizeof(jniNativeMethods) / sizeof(JNINativeMethod));
    if (ret != JNI_OK) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_4;
}

#ifdef __cplusplus
    }
#endif




