extern "C" {
#include "avilib/avilib.h"
}

#include <GLES2/gl.h>
#include <GLES2/glext.h>

#include <malloc.h>

#include "Common.h"
#include "OpenGLPlayerActivity.h"

struct Instance
{
	char* buffer;
	GLuint texture;

	Instance():
		buffer(0),
		texture(0)
	{

	}
};

jlong Java_com_apress_aviplayer_OpenGLPlayerActivity_init(
		JNIEnv* env,
		jclass clazz,
		jlong avi)
{
	Instance* instance = 0;

	long frameSize = AVI_frame_size((avi_t*) avi, 0);
	if (0 >= frameSize)
	{
		ThrowException(env, "java/io/RuntimeException",
				"Unable to get the frame size.");
		goto exit;
	}

	instance = new Instance();
	if (0 == instance)
	{
		ThrowException(env, "java/io/RuntimeException",
				"Unable to allocate instance.");
		goto exit;
	}

	instance->buffer = (char*) malloc(frameSize);
	if (0 == instance->buffer)
	{
		ThrowException(env, "java/io/RuntimeException",
				"Unable to allocate buffer.");
		delete instance;
		instance = 0;
	}

exit:
	return (jlong) instance;
}

void Java_com_apress_aviplayer_OpenGLPlayerActivity_initSurface(
		JNIEnv* env,
		jclass clazz,
		jlong inst,
		jlong avi)
{
	Instance* instance = (Instance*) inst;

	// Enable textures
	glEnable(GL_TEXTURE_2D);

	// Generate one texture object
	glGenTextures(1, &instance->texture);

	// Bind to generated texture
	glBindTexture(GL_TEXTURE_2D, instance->texture);

	int frameWidth = AVI_video_width((avi_t*) avi);
	int frameHeight = AVI_video_height((avi_t*) avi);

	// Crop the texture rectangle
	GLint rect[] = {0, frameHeight, frameWidth, -frameHeight};
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, rect);

	// Full color
	glColor4f(1.0, 1.0, 1.0, 1.0);

	// Generate an empty texture
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGB,
			frameWidth,
			frameHeight,
			0,
			GL_RGB,
			GL_UNSIGNED_SHORT_5_6_5,
			0);
}

jboolean Java_com_apress_aviplayer_OpenGLPlayerActivity_render(
		JNIEnv* env,
		jclass clazz,
		jlong inst,
		jlong avi)
{
	Instance* instance = (Instance*) inst;

	jboolean isFrameRead = JNI_FALSE;
	int keyFrame = 0;

	// Read AVI frame bytes to bitmap
	long frameSize = AVI_read_frame((avi_t*) avi,
			instance->buffer,
			&keyFrame);

	// Check if frame read
	if (0 >= frameSize)
	{
		goto exit;
	}

	// Frame read
	isFrameRead = JNI_TRUE;

	// Update the texture with the new frame
	glTexSubImage2D(GL_TEXTURE_2D,
			0,
			0,
			0,
			AVI_video_width((avi_t*) avi),
			AVI_video_height((avi_t*) avi),
			GL_RGB,
			GL_UNSIGNED_SHORT_5_6_5,
			instance->buffer);

	// Draw texture
	glDrawTexiOES(0, 0, 0,
			AVI_video_width((avi_t*) avi),
			AVI_video_height((avi_t*) avi));

exit:
	return isFrameRead;
}

void Java_com_apress_aviplayer_OpenGLPlayerActivity_free(
		JNIEnv* env,
		jclass clazz,
		jlong inst)
{
	Instance* instance = (Instance*) inst;

	if (0 != instance)
	{
		free(instance->buffer);
		delete instance;
	}
}
