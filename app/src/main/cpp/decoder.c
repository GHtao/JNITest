#include <jni.h>
#include <string.h>

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/log.h"

#include <android/log.h>
#define  LOGE(format,...) __android_log_print(ANDROID_LOG_ERROR,"gt",format, ##__VA_ARGS__);
JNIEXPORT jint JNICALL
Java_com_tocel_jnitest_MainActivity_decoder(JNIEnv *env, jobject instance, jstring input_,
                                            jstring output_) {
    const char *input = (*env)->GetStringUTFChars(env,input_, 0);
    const char *output = (*env)->GetStringUTFChars(env,output_, 0);
    // TODO 视频解码 mp4->yuv



    (*env)->ReleaseStringUTFChars(env,input_, input);
    (*env)->ReleaseStringUTFChars(env,output_, output);
    LOGE("%s",input);
    LOGE("%s",output);
    jint i = 10;
    return i;
}

