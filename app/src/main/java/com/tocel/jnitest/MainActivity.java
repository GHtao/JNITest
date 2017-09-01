package com.tocel.jnitest;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import butterknife.ButterKnife;
import butterknife.InjectView;
import butterknife.OnClick;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("ffmpeg");
    }

    @InjectView(R.id.sample_text)
    TextView sampleText;
    @InjectView(R.id.bt_configure)
    Button btConfigure;
    @InjectView(R.id.bt_urlPro)
    Button btUrlPro;
    @InjectView(R.id.bt_avformat)
    Button btAvformat;
    @InjectView(R.id.bt_avcodec)
    Button btAvcodec;
    @InjectView(R.id.bt_avFilter)
    Button btAvFilter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.inject(this);
//        setStringToJNI("12345");

    }
    @OnClick({R.id.bt_avcodec,R.id.bt_avFilter,R.id.bt_avformat,R.id.bt_configure,R.id.bt_urlPro})
    void doClick(View v){
        String info = "";
        switch (v.getId()){
            case R.id.bt_avcodec:
                info = getAVCodec();
            break;
            case R.id.bt_avFilter:
                info = getAVFilter();
            break;
            case R.id.bt_avformat:
                info = getAVFormat();
            break;
            case R.id.bt_configure:
                info = getConfigure();
            break;
            case R.id.bt_urlPro:
                info = getUrlPro();
            break;

            default:
                break;
        }
        sampleText.setText(info);
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void setStringToJNI(String str);

    public native String getAVCodec();
    public native String getAVFilter();
    public native String getAVFormat();
    public native String getConfigure();
    public native String getUrlPro();

}