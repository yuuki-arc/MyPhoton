# This project file has been automatically generated.
# Manual edition is not recommended.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := demoLoadBalancing

WINGDB_FOLDER_NAME := "Source Files"

WINGDB_FOLDER_SRC_FILES := \
    ../../src/NetworkLogic.cpp \
    AndroidNetworkLogic.cpp \
    main.cpp \


LOCAL_SRC_FILES := \
    ../../src/NetworkLogic.cpp \
    AndroidNetworkLogic.cpp \
    main.cpp \


LOCAL_C_INCLUDES := ../../inc/
LOCAL_CFLAGS := -DEG_DEBUGGER -D__STDINT_LIMITS -D_EG_ANDROID_PLATFORM
LOCAL_STATIC_LIBRARIES := loadbalancing-cpp-static-prebuilt                            photon-cpp-static-prebuilt                            common-cpp-static-prebuilt
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(shell pwd)/../../../../LoadBalancing-cpp/android)
$(call import-add-path, $(shell pwd)/../../../../LoadBalancing-cpp)

$(call import-module,loadbalancing-cpp-prebuilt)
