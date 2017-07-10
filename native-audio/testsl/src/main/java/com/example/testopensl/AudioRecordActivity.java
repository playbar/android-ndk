package com.example.testopensl;

import java.io.File;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;

import com.example.audio.R;

public class AudioRecordActivity extends Activity implements OnClickListener {

    static {
        System.loadLibrary("TestAudio");
    }
    private boolean mRecording;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        createEngine();
        setContentView(R.layout.open_sl_audio_record);
        findViewById(R.id.start).setOnClickListener(this);
        findViewById(R.id.stop).setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.start:
                File file = new File(getCacheDir(), "audio.pcm");
                createAudioRecord(file.getAbsolutePath());
                mRecording = true;
                break;
            case R.id.stop:
                stop();
                mRecording = false;
                break;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mRecording) {
            stop();
        }
        shutdown();
    }

    /** Native methods, implemented in jni folder */
    public native void createEngine();

    public native void createAudioRecord(String uri);

    public native void stop();

    public native void shutdown();

}
