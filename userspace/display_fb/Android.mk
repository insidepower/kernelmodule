LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := kn_fb
LOCAL_CFLAGS += -O3
LOCAL_SRC_FILES := main.c
include $(BUILD_EXECUTABLE)

