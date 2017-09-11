#include <jni.h>
#include "stdio.h"
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <android/log.h>

#define  LOGE(format,...) __android_log_print(ANDROID_LOG_ERROR,"gt",format, ##__VA_ARGS__);
//输入的像素数据读取完成后调用此函数。用于输出编码器中剩余的AVPacket。
int flush_encoder(AVFormatContext *fmt_cxt, unsigned int stream_index){
    int ret;
    int got_frame;
    AVPacket enc_pkt;
    if(!(fmt_cxt->streams[stream_index]->codec->codec->capabilities & CODEC_CAP_DELAY)){
        return 0;
    }
    while(1){
        enc_pkt.data = NULL;
        enc_pkt.size = 0;
        av_init_packet(&enc_pkt);
        ret = avcodec_encode_video2(fmt_cxt->streams[stream_index]->codec,&enc_pkt,NULL,&got_frame);
        if(ret < 0){
            break;
        }
        if(!got_frame){
            ret = 0;
            break;
        }
        LOGE("Flush encoder: success to encoder 1 frame!\tsize:%5d\n",enc_pkt.size);
        ret = av_write_frame(fmt_cxt,&enc_pkt);
        if(ret < 0){
            break;
        }
    }
    return ret;
}

JNIEXPORT jint JNICALL
Java_com_tocel_jnitest_MainActivity_encoder(JNIEnv *env, jobject instance, jstring input_,
                                            jstring output_) {
    const char *input = (*env)->GetStringUTFChars(env,input_, 0);
    const char *output = (*env)->GetStringUTFChars(env,output_, 0);
    // TODO 编码yuv-h264
    AVFormatContext *pFormatCtx;
    AVOutputFormat *fmt;
    AVStream *video_st;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVPacket pkt;
    uint8_t* picture_buf;
    AVFrame *pFrame;
    int picture_size;
    int y_size;
    int frameCnt = 0;
    FILE *in_file = fopen(input,"rb");
    int in_w = 1280;
    int in_h = 720;
    int framenum = 70;

    av_register_all();
    pFormatCtx = avformat_alloc_context();
    fmt = av_guess_format(NULL,output,NULL);
    pFormatCtx->oformat = fmt;

    if(avio_open(&pFormatCtx->pb,output,AVIO_FLAG_READ_WRITE)<0){
        LOGE("Failed to open output file! \n");
        return -1;
    }
    video_st = avformat_new_stream(pFormatCtx,0);
    //video_st->time_base.num = 1;
    //video_st->time_base.den = 25;
    if(video_st == NULL){
        return -1;
    }
    //Param that must set
    pCodecCtx = video_st->codec;
    pCodecCtx->codec_id = fmt->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = in_w;
    pCodecCtx->height = in_h;
    pCodecCtx->bit_rate = 400000;
    pCodecCtx->gop_size = 250;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;

    //H264
    //pCodecCtx->me_range = 16;
    //pCodecCtx->max_qdiff = 4;
    //pCodecCtx->qcompress = 0.6;
    pCodecCtx->qmin = 10;
    pCodecCtx->qmax = 51;
    //Optional Param
    pCodecCtx->max_b_frames = 3;

    AVDictionary *param = 0;
    if(pCodecCtx->codec_id == AV_CODEC_ID_H264){//h264 params
        av_dict_set(&param,"preset","slow",0);
        av_dict_set(&param,"tune","zerolatency",0);
    }
    if(pCodecCtx->codec_id == AV_CODEC_ID_H265){
        av_dict_set(&param,"preset","ultrafast",0);
        av_dict_set(&param,"tune","zero-latency",0);
    }
    //Show some Information
    av_dump_format(pFormatCtx,0,output,1);
    //
    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if(!pCodec){
        LOGE("Can not find encoder! \n");
        return -1;
    }
    if(avcodec_open2(pCodecCtx,pCodec,&param)<0){
        LOGE("Failed to open encoder! \n");
        return -1;
    }
    pFrame = av_frame_alloc();
    picture_size = avpicture_get_size(pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height);
    picture_buf = (uint8_t *) av_malloc(picture_size);
    avpicture_fill((AVPicture *) pFrame, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
    //Write File Header
    avformat_write_header(pFormatCtx,NULL);
    av_new_packet(&pkt,picture_size);
    y_size = pCodecCtx->width * pCodecCtx->height;
    for(int i=0;i<framenum;i++){
        //Read raw YUV data
        if(fread(picture_buf,1,y_size*3/2,in_file)<=0){
            LOGE("Failed to read raw data! \n");
            return -1;
        }else if(feof(in_file)){
            break;
        }
        pFrame->data[0] = picture_buf;//Y
        pFrame->data[1] = picture_buf+y_size;//U
        pFrame->data[2] = picture_buf+y_size*5/4;//V
        //PTS
        //pFrame->pts=i;
        pFrame->pts = i*(video_st->time_base.den)/((video_st->time_base.num)*25);
        int got_picture = 0;
        //Encoder
        int ret = avcodec_encode_video2(pCodecCtx,&pkt,pFrame,&got_picture);
        if(ret < 0){
            LOGE("Failed to encode! \n");
            return -1;
        }
        if(got_picture == 1){
            LOGE("Succeed to encode frame: %5d\tsize:%5d\n",frameCnt,pkt.size);
            frameCnt++;
            pkt.stream_index = video_st->index;
            ret = av_write_frame(pFormatCtx,&pkt);
            av_free_packet(&pkt);
        }
    }
    //Flush Encoder
    int ret = flush_encoder(pFormatCtx,0);
    if(ret < 0){
        LOGE("Flushing encoder failed\n");
        return -1;
    }
    //Write file trailer
    av_write_trailer(pFormatCtx);
    //clean
    if(video_st){
        avcodec_close(video_st->codec);
        av_free(pFrame);
        av_free(picture_buf);
    }
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);
    fclose(in_file);

    (*env)->ReleaseStringUTFChars(env,input_, input);
    (*env)->ReleaseStringUTFChars(env,output_, output);
    return 0;
}

