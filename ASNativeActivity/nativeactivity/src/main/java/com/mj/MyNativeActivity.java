package com.mj;

import android.app.NativeActivity;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.InputQueue;
import android.view.SurfaceHolder;
import android.view.inputmethod.InputMethodManager;

import java.io.File;

public class MyNativeActivity extends NativeActivity
{

    public static final String META_DATA_LIB_NAME = "android.app.lib_name";
    public static final String META_DATA_FUNC_NAME = "android.app.func_name";
    private static final String KEY_NATIVE_SAVED_STATE = "android:native_state";

    private InputMethodManager mIMM;

    private long mNativeHandle;

    private InputQueue mCurInputQueue;
    private SurfaceHolder mCurSurfaceHolder;

    final int[] mLocation = new int[2];
    int mLastContentX;
    int mLastContentY;
    int mLastContentWidth;
    int mLastContentHeight;

    private boolean mDispatchingUnhandledKey;

    private boolean mDestroyed;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Logger.printTime();
    }

    private static String getAbsolutePath(File file) {
        return (file != null) ? file.getAbsolutePath() : null;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
    }

    public void surfaceCreated(SurfaceHolder holder) {
        super.surfaceCreated( holder);
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        super.surfaceChanged(holder, format, width, height);
    }

    public void surfaceRedrawNeeded(SurfaceHolder holder) {
        super.surfaceRedrawNeeded(holder);
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        super.surfaceDestroyed(holder);
    }

    public void onInputQueueCreated(InputQueue queue) {
        super.onInputQueueCreated(queue);
    }

    public void onInputQueueDestroyed(InputQueue queue) {
        super.onInputQueueDestroyed( queue);
    }

    public void onGlobalLayout() {
        super.onGlobalLayout();
    }


}
