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
        //input
        struct URLProtocol **p_temp = &pup;
        avio_enum_protocols((void **)p_temp,0);//1 输出协议 其他输入协议
        while((*p_temp) != NULL){
            sprintf(info,"%s[IN ][%10s]\n",info,avio_enum_protocols((void **)p_temp,0));
        }
        //output
        pup = NULL;
        avio_enum_protocols((void **)p_temp,1);
        while((*p_temp) != NULL){
            sprintf(info,"%s[out][%10s]\n",info,avio_enum_protocols((void **)p_temp,1));
        }
        return env->NewStringUTF(info);
    }

    JNIEXPORT jstring JNICALL
    Java_com_tocel_jnitest_MainActivity_getAVCodec(JNIEnv *env, jobject instance) {
        // TODO 获取编码信息
        char info[40000] = {0};
        av_register_all();
        AVCodec *c_temp = av_codec_next(NULL);//null返回第一个  不是null就返回这个编码的下一个
        while(c_temp != NULL){//最后一个返回null
            if(c_temp->decode != NULL){
                sprintf(info,"%s[Dec]",info);
            }else{
                sprintf(info,"%s[Enc]",info);
            }
            switch(c_temp->type){
                case AVMEDIA_TYPE_AUDIO:
                    sprintf(info,"%s[Audio]",info);
                    break;
                case AVMEDIA_TYPE_VIDEO:
                    sprintf(info,"%s[Video]",info);
                    break;
                default:
                    sprintf(info,"%s[Other]",info);
                    break;
            }
            sprintf(info,"%s[%10s]\n",info,c_temp->name);
            c_temp = c_temp->next;
        }
        return env->NewStringUTF(info);
    }

    JNIEXPORT jstring JNICALL
    Java_com_tocel_jnitest_MainActivity_getAVFilter(JNIEnv *env, jobject instance) {
        // TODO 获取滤镜
        char info[40000] = {0};
        av_register_all();
        AVFilter *filter = (AVFilter *) avfilter_next(NULL);
        while(filter != NULL){
            sprintf(info,"%s[%10s]\n",info,filter->name);
        }
        return env->NewStringUTF(info);
    }

    JNIEXPORT jstring JNICALL
    Java_com_tocel_jnitest_MainActivity_getAVFormat(JNIEnv *env, jobject instance) {
        // TODO 获取封装格式
        char info[40000] = {0};
        av_register_all();
        AVInputFormat *inputFormat = av_iformat_next(NULL);
        AVOutputFormat *outputFormat = av_oformat_next(NULL);
        while(inputFormat != NULL){
            sprintf(info,"%s[%10s]\n",info,inputFormat->name);
            inputFormat = inputFormat->next;
        }
        while(outputFormat != NULL){
            sprintf(info,"%s[%10s]\n",info,outputFormat->name);
            outputFormat = outputFormat->next;
        }
        return env->NewStringUTF(info);
    }

    JNIEXPORT jstring JNICALL
    Java_com_tocel_jnitest_MainActivity_getConfigure(JNIEnv *env, jobject instance) {

        // TODO 配置信息
        char info[40000] = {0};
        av_register_all();
        sprintf(info,"%s\n",info,avcodec_configuration());
        return env->NewStringUTF(info);
    }
}

