extern "C" {
#include "avilib/avilib.h"
}

#include <android/native_window_jni.h>
#include <android/native_window.h>

#include "Common.h"
#include "NativeWindowPlayerActivity.h"

void Java_com_apress_aviplayer_NativeWindowPlayerActivity_init(
		JNIEnv* env,
		jclass clazz,
		jlong avi,
		jobject surface)
{
	// Get the native window from the surface
	ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
	int w = ANativeWindow_getWidth(nativeWindow);
	int h = ANativeWindow_getHeight(nativeWindow);
	int format = ANativeWindow_getFormat(nativeWindow);
	if (0 == nativeWindow)
	{
		ThrowException(env, "java/io/RuntimeException",
				"Unable to get native window from surface.");
		goto exit;
	}

	// Set the buffers geometry to AVI movie frame dimensions
	// If these are different than the window's physical size
	// then the buffer will be scaled to match that size.
	if (0 > ANativeWindow_setBuffersGeometry(nativeWindow,
			AVI_video_width((avi_t*) avi),
			AVI_video_height((avi_t*) avi),
			WINDOW_FORMAT_RGB_565))
	{
		ThrowException(env, "java/io/RuntimeException",
				"Unable to set buffers geometry.");
	}

	// Release the native window
	ANativeWindow_release(nativeWindow);
	nativeWindow = 0;

exit:
	return;
}

jboolean Java_com_apress_aviplayer_NativeWindowPlayerActivity_render(
		JNIEnv* env,
		jclass clazz,
		jlong avi,
		jobject surface)
{
	jboolean isFrameRead = JNI_FALSE;

	long frameSize = 0;
	int keyFrame = 0;

	// Get the native window from the surface
	ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
	if (0 == nativeWindow)
	{
		ThrowException(env, "java/io/RuntimeException",
				"Unable to get native window from surface.");
		goto exit;
	}

	// Lock the native window and get access to raw buffer
	ANativeWindow_Buffer windowBuffer;
	if (0 > ANativeWindow_lock(nativeWindow, &windowBuffer, 0))
	{
		ThrowException(env, "java/io/RuntimeException",
				"Unable to lock native window.");
		goto release;
	}

	// Read AVI frame bytes to raw buffer
	frameSize = AVI_read_frame((avi_t*) avi,
			(char*) windowBuffer.bits,
			&keyFrame);

	// Check if frame is successfully read
	if (0 < frameSize)
	{
		isFrameRead = JNI_TRUE;
	}

	// Unlock and post the buffer for displaying
	if (0 > ANativeWindow_unlockAndPost(nativeWindow))
	{
		ThrowException(env, "java/io/RuntimeException",
				"Unable to unlock and post to native window.");
		goto release;
	}

release:
	// Release the native window
	ANativeWindow_release(nativeWindow);
	nativeWindow = 0;

exit:
	return isFrameRead;
}

