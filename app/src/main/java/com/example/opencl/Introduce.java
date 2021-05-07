package com.example.opencl;

public class Introduce {

    static {
        System.loadLibrary("native-opencl-lib");
    }

    public static native String showInfo();
    public static native void vecAdd();
    public static native void conv();
}
