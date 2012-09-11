LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := humanity
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := \
	../../src/io/file.cpp \
	../../src/io/directory.cpp \
	../../src/io/path.cpp \
	../../src/string_utils.cpp
LOCAL_CFLAGS     := 
LOCAL_LDFLAGS    := 
LOCAL_LDLIBS     := -llog

include $(BUILD_SHARED_LIBRARY)

