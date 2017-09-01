#include <jni.h>
#include <string>



extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavfilter/avfilter.h>
    std::string hello = "Hello from C++";
    struct URLProtocol;
    JNIEXPORT void JNICALL
    Java_com_tocel_jnitest_MainActivity_setStringToJNI(JNIEnv *env, jobject instance, jstring str_) {
        const char *str = env->GetStringUTFChars(str_, 0);
        // TODO 设置数据
        env->ReleaseStringUTFChars(str_, str);
        hello = str;
    }

    JNIEXPORT jstring JNICALL
    Java_com_tocel_jnitest_MainActivity_stringFromJNI(
            JNIEnv *env,
            jobject /* this */) {
//        获取数据
        return env->NewStringUTF(hello.c_str());
    }

    JNIEXPORT jstring JNICALL
    Java_com_tocel_jnitest_MainActivity_getUrlPro(JNIEnv *env, jobject instance) {
        // TODO 获取url协议
        char info[40000] = {0};
        av_register_all();//初始化所有的信息
        struct URLProtocol *pup = NULL;
        //
        struct URLProtocol **p_temp = &pup;

//        struct URLProtocol *pup = NULL;
//        //Input
//        struct URLProtocol **p_temp = &pup;
//        avio_enum_protocols((void **)p_temp, 0);
//        while ((*p_temp) != NULL){
//            sprintf(info, "%s[In ][%10s]\n", info, avio_enum_protocols((void **)p_temp, 0));
//        }
//        pup = NULL;
//        //Output
//        avio_enum_protocols((void **)p_temp, 1);
//        while ((*p_temp) != NULL){
//            sprintf(info, "%s[Out][%10s]\n", info, avio_enum_protocols((void **)p_temp, 1));
//        }

        //LOGE("%s", info);
        return env->NewStringUTF(info);

//        return env->NewStringUTF(returnValue);
    }

    JNIEXPORT jstring JNICALL
    Java_com_tocel_jnitest_MainActivity_getAVCodec(JNIEnv *env, jobject instance) {

        // TODO 获取编码信息


        return env->NewStringUTF(returnValue);
    }

    JNIEXPORT jstring JNICALL
    Java_com_tocel_jnitest_MainActivity_getAVFilter(JNIEnv *env, jobject instance) {

        // TODO 获取滤镜


        return env->NewStringUTF(returnValue);
    }

    JNIEXPORT jstring JNICALL
    Java_com_tocel_jnitest_MainActivity_getAVFormat(JNIEnv *env, jobject instance) {

        // TODO 获取封装格式


        return env->NewStringUTF(returnValue);
    }

    JNIEXPORT jstring JNICALL
    Java_com_tocel_jnitest_MainActivity_getConfigure(JNIEnv *env, jobject instance) {

        // TODO 配置信息


        return env->NewStringUTF(returnValue);
    }
}

