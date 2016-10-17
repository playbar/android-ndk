#include "MainActivity.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

extern "C" {
#include "avilib/wavlib.h"
}

static const char* JAVA_LANG_IOEXCEPTION = "java/lang/IOException";
static const char* JAVA_LANG_OUTOFMEMORYERROR = "java/lang/OutOfMemoryError";

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

/**
 * Player context.
 */
struct PlayerContext
{
	SLObjectItf engineObject;
	SLEngineItf engineEngine;
	SLObjectItf outputMixObject;
	SLObjectItf audioPlayerObject;
	SLAndroidSimpleBufferQueueItf audioPlayerBufferQueue;
	SLPlayItf audioPlayerPlay;
	WAV wav;

	unsigned char* buffer;
	size_t bufferSize;

	PlayerContext()
	: engineObject(0)
	, engineEngine(0)
	, outputMixObject(0)
	, audioPlayerBufferQueue(0)
	, audioPlayerPlay(0)
	, wav(0)
	, bufferSize(0)
	{}
};

/**
 * Throw exception with given class and message.
 *
 * @param env JNIEnv interface.
 * @param className class name.
 * @param message exception message.
 */
static void ThrowException(
		JNIEnv* env,
		const char* className,
		const char* message)
{
	// Get the exception class
	jclass clazz = env->FindClass(className);

	// If exception class is found
	if (0 != clazz)
	{
		// Throw exception
		env->ThrowNew(clazz, message);

		// Release local class reference
		env->DeleteLocalRef(clazz);
	}
}

/**
 * Open the given WAVE file.
 *
 * @param env JNIEnv interface.
 * @param fileName file name.
 * @return WAV file.
 * @throws IOException
 */
static WAV OpenWaveFile(
		JNIEnv* env,
		jstring fileName)
{
	WAVError error = WAV_SUCCESS;
	WAV wav = 0;

	// Get the file name as a C string
	const char* cFileName = env->GetStringUTFChars(fileName, 0);
	if (0 == cFileName)
		goto exit;

	// Open the WAVE file
	wav = wav_open(cFileName, WAV_READ, &error);

	// Release the file name
	env->ReleaseStringUTFChars(fileName, cFileName);

	// Check error
	if (0 == wav)
	{
		ThrowException(env,
				JAVA_LANG_IOEXCEPTION,
				wav_strerror(error));
	}

exit:
	return wav;
}

/**
 * Close the given WAVE file.
 *
 * @param wav WAV file.
 * @throws IOException
 */
static void CloseWaveFile(
		WAV wav)
{
	if (0 != wav)
	{
		wav_close(wav);
	}
}

/**
 * Convert OpenSL ES result to string.
 *
 * @param result result code.
 * @return result string.
 */
static const char* ResultToString(SLresult result)
{
	const char* str = 0;

	switch (result)
	{
	case SL_RESULT_SUCCESS:
		str = "Success";
		break;

	case SL_RESULT_PRECONDITIONS_VIOLATED:
		str = "Preconditions violated";
		break;

	case SL_RESULT_PARAMETER_INVALID:
		str = "Parameter invalid";
		break;

	case SL_RESULT_MEMORY_FAILURE:
		str = "Memory failure";
		break;

	case SL_RESULT_RESOURCE_ERROR:
		str = "Resource error";
		break;

	case SL_RESULT_RESOURCE_LOST:
		str = "Resource lost";
		break;

	case SL_RESULT_IO_ERROR:
		str = "IO error";
		break;

	case SL_RESULT_BUFFER_INSUFFICIENT:
		str = "Buffer insufficient";
		break;

	case SL_RESULT_CONTENT_CORRUPTED:
		str = "Success";
		break;

	case SL_RESULT_CONTENT_UNSUPPORTED:
		str = "Content unsupported";
		break;

	case SL_RESULT_CONTENT_NOT_FOUND:
		str = "Content not found";
		break;

	case SL_RESULT_PERMISSION_DENIED:
		str = "Permission denied";
		break;

	case SL_RESULT_FEATURE_UNSUPPORTED:
		str = "Feature unsupported";
		break;

	case SL_RESULT_INTERNAL_ERROR:
		str = "Internal error";
		break;

	case SL_RESULT_UNKNOWN_ERROR:
		str = "Unknown error";
		break;

	case SL_RESULT_OPERATION_ABORTED:
		str = "Operation aborted";
		break;

	case SL_RESULT_CONTROL_LOST:
		str = "Control lost";
		break;

	default:
		str = "Unknown code";
	}

	return str;
}

/**
 * Checks if the result is an error, and throws
 * and IOException with the error message.
 *
 * @param env JNIEnv interface.
 * @param result result code.
 * @return error occurred.
 * @throws IOException
 */
static bool CheckError(
		JNIEnv* env,
		SLresult result)
{
	bool isError = false;

	// If an error occurred
	if (SL_RESULT_SUCCESS != result)
	{
		// Throw IOException
		ThrowException(env,
				JAVA_LANG_IOEXCEPTION,
				ResultToString(result));

		isError = true;
	}

	return isError;
}

/**
 * Creates an OpenGL ES engine.
 *
 * @param env JNIEnv interface.
 * @param engineObject object to hold engine. [OUT]
 * @throws IOException
 */
static void CreateEngine(
		JNIEnv* env,
		SLObjectItf& engineObject)
{
	// OpenSL ES for Android is designed to be thread-safe,
	// so this option request will be ignored, but it will
	// make the source code portable to other platforms.
	SLEngineOption engineOptions[] = {
		{ (SLuint32) SL_ENGINEOPTION_THREADSAFE, (SLuint32) SL_BOOLEAN_TRUE }
	};

	// Create the OpenSL ES engine object
	SLresult result = slCreateEngine(
			&engineObject,
			ARRAY_LEN(engineOptions),
			engineOptions,
			0, // no interfaces
			0, // no interfaces
			0); // no required

	// Check error
	CheckError(env, result);
}

/**
 * Realize the given object. Objects needs to be
 * realized before using them.
 *
 * @param env JNIEnv interface.
 * @param object object instance.
 * @throws IOException
 */
static void RealizeObject(
		JNIEnv* env,
		SLObjectItf object)
{
	// Realize the engine object
	SLresult result = (*object)->Realize(
			object,
			SL_BOOLEAN_FALSE); // No async, blocking call

	// Check error
	CheckError(env, result);
}

/**
 * Destroys the given object instance.
 *
 * @param object object instance. [IN/OUT]
 */
static void DestroyObject(SLObjectItf& object)
{
	if (0 != object)
		(*object)->Destroy(object);

	object = 0;
}

/**
 * Gets the engine interface from the given engine object
 * in order to create other objects from the engine.
 *
 * @param env JNIEnv interface.
 * @param engineObject engine object.
 * @param engineEngine engine interface. [OUT]
 * @throws IOException
 */
static void GetEngineInterface(
		JNIEnv* env,
		SLObjectItf& engineObject,
		SLEngineItf& engineEngine)
{
	// Get the engine interface
	SLresult result = (*engineObject)->GetInterface(
			engineObject,
			SL_IID_ENGINE,
			&engineEngine);

	// Check error
	CheckError(env, result);
}

/**
 * Creates and output mix object.
 *
 * @param env JNIEnv interface.
 * @param engineEngine engine engine.
 * @param outputMixObject object to hold the output mix. [OUT]
 * @throws IOException
 */
static void CreateOutputMix(
		JNIEnv* env,
		SLEngineItf engineEngine,
		SLObjectItf& outputMixObject)
{
	// Create output mix object
	SLresult result = (*engineEngine)->CreateOutputMix(
			engineEngine,
			&outputMixObject,
			0, // no interfaces
			0, // no interfaces
			0); // no required

	// Check error
	CheckError(env, result);
}

/**
 * Free the player buffer.
 *
 * @param buffers buffer instance. [OUT]
 */
static void FreePlayerBuffer(unsigned char*& buffers)
{
	if (0 != buffers)
	{
		delete buffers;
		buffers = 0;
	}
}

/**
 * Initializes the player buffer.
 *
 * @param env JNIEnv interface.
 * @param wav WAVE file.
 * @param buffers buffer instance. [OUT]
 * @param bufferSize buffer size. [OUT]
 */
static void InitPlayerBuffer(
		JNIEnv* env,
		WAV wav,
		unsigned char*& buffer,
		size_t& bufferSize)
{
	// Calculate the buffer size
	bufferSize = wav_get_channels(wav) * wav_get_rate(wav) * wav_get_bits(wav);

	// Initialize buffer
	buffer = new unsigned char[bufferSize];
	if (0 == buffer)
	{
		ThrowException(env,
				JAVA_LANG_OUTOFMEMORYERROR,
				"buffer");
	}
}

/**
 * Creates buffer queue audio player.
 *
 * @param wav WAVE file.
 * @param engineEngine engine interface.
 * @param outputMixObject output mix.
 * @param audioPlayerObject audio player. [OUT]
 * @throws IOException
 */
static void CreateBufferQueueAudioPlayer(
		WAV wav,
		SLEngineItf engineEngine,
		SLObjectItf outputMixObject,
		SLObjectItf& audioPlayerObject)
{
	// Android simple buffer queue locator for the data source
	SLDataLocator_AndroidSimpleBufferQueue dataSourceLocator = {
		SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, // locator type
		1                                        // buffer count
	};

	// PCM data source format
	SLDataFormat_PCM dataSourceFormat = {
		SL_DATAFORMAT_PCM,        // format type
		wav_get_channels(wav),    // channel count
		wav_get_rate(wav) * 1000, // samples per second in millihertz
		wav_get_bits(wav),        // bits per sample
		wav_get_bits(wav),        // container size
		SL_SPEAKER_FRONT_CENTER,  // channel mask
		SL_BYTEORDER_LITTLEENDIAN // endianness
	};

	// Data source is a simple buffer queue with PCM format
	SLDataSource dataSource = {
		&dataSourceLocator, // data locator
		&dataSourceFormat   // data format
	};

	// Output mix locator for data sink
	SLDataLocator_OutputMix dataSinkLocator = {
		SL_DATALOCATOR_OUTPUTMIX, // locator type
		outputMixObject           // output mix
	};

	// Data sink is an output mix
	SLDataSink dataSink = {
		&dataSinkLocator, // locator
		0                 // format
	};

	// Interfaces that are requested
	SLInterfaceID interfaceIds[] = {
		SL_IID_BUFFERQUEUE
	};

	// Required interfaces. If the required interfaces
	// are not available the request will fail
	SLboolean requiredInterfaces[] = {
		SL_BOOLEAN_TRUE // for SL_IID_BUFFERQUEUE
	};

	// Create audio player object
	SLresult result = (*engineEngine)->CreateAudioPlayer(
			engineEngine,
			&audioPlayerObject,
			&dataSource,
			&dataSink,
			ARRAY_LEN(interfaceIds),
			interfaceIds,
			requiredInterfaces);
}

/**
 * Gets the audio player buffer queue interface.
 *
 * @param env JNIEnv interface.
 * @param audioPlayerObject audio player object instance.
 * @param audioPlayerBufferQueue audio player buffer queue. [OUT]
 * @throws IOException
 */
static void GetAudioPlayerBufferQueueInterface(
		JNIEnv* env,
		SLObjectItf audioPlayerObject,
		SLAndroidSimpleBufferQueueItf& audioPlayerBufferQueue)
{
	// Get the buffer queue interface
	SLresult result = (*audioPlayerObject)->GetInterface(
			audioPlayerObject,
			SL_IID_BUFFERQUEUE,
			&audioPlayerBufferQueue);

	// Check error
	CheckError(env, result);
}

/**
 * Destroy the player context.
 *
 * @param ctx player context.
 */
static void DestroyContext(PlayerContext*& ctx)
{
	// Destroy audio player object
	DestroyObject(ctx->audioPlayerObject);

	// Free the player buffer
	FreePlayerBuffer(ctx->buffer);

	// Destroy output mix object
	DestroyObject(ctx->outputMixObject);

	// Destroy the engine instance
	DestroyObject(ctx->engineObject);

    // Close the WAVE file
	CloseWaveFile(ctx->wav);

	// Free context
	delete ctx;
	ctx = 0;
}

/**
 * Gets called when a buffer finishes playing.
 *
 * @param audioPlayerBufferQueue audio player buffer queue.
 * @param context player context.
 */
static void PlayerCallback(
		SLAndroidSimpleBufferQueueItf audioPlayerBufferQueue,
		void* context)
{
	// Get the player context
	PlayerContext* ctx = (PlayerContext*) context;

	// Read data
	ssize_t readSize = wav_read_data(
			ctx->wav,
			ctx->buffer,
			ctx->bufferSize);

	// If data is read
	if (0 < readSize)
	{
		(*audioPlayerBufferQueue)->Enqueue(
				audioPlayerBufferQueue,
				ctx->buffer,
				readSize);
	}
	else
	{
		DestroyContext(ctx);
	}
}

/**
 * Registers the player callback.
 *
 * @param env JNIEnv interface.
 * @param audioPlayerBufferQueue audio player buffer queue.
 * @param ctx player context.
 * @throws IOException
 */
static void RegisterPlayerCallback(
		JNIEnv* env,
		SLAndroidSimpleBufferQueueItf audioPlayerBufferQueue,
		PlayerContext* ctx)
{
	// Register the player callback
	SLresult result = (*audioPlayerBufferQueue)->RegisterCallback(
			audioPlayerBufferQueue,
			PlayerCallback,
			ctx); // player context

	// Check error
	CheckError(env, result);
}

/**
 * Gets the audio player play interface.
 *
 * @param env JNIEnv interface.
 * @param audioPlayerObject audio player object instance.
 * @param audioPlayerPlay play interface. [OUT]
 * @throws IOException
 */
static void GetAudioPlayerPlayInterface(
		JNIEnv* env,
		SLObjectItf audioPlayerObject,
		SLPlayItf& audioPlayerPlay)
{
	// Get the play interface
	SLresult result = (*audioPlayerObject)->GetInterface(
			audioPlayerObject,
			SL_IID_PLAY,
			&audioPlayerPlay);

	// Check error
	CheckError(env, result);
}

/**
 * Sets the audio player state playing.
 *
 * @param env JNIEnv interface.
 * @param audioPlayerPlay play interface.
 * @throws IOException
 */
static void SetAudioPlayerStatePlaying(
		JNIEnv* env,
		SLPlayItf audioPlayerPlay)
{
	// Set audio player state to playing
	SLresult result = (*audioPlayerPlay)->SetPlayState(
			audioPlayerPlay,
			SL_PLAYSTATE_PLAYING);

	// Check error
	CheckError(env, result);
}

void Java_com_apress_wavplayer_MainActivity_play(
		JNIEnv* env,
		jobject obj,
		jstring fileName)
{
	PlayerContext* ctx = new PlayerContext();

	// Open the WAVE file
	ctx->wav = OpenWaveFile(env, fileName);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Create OpenSL ES engine
	CreateEngine(env, ctx->engineObject);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Realize the engine object
	RealizeObject(env, ctx->engineObject);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Get the engine interface
	GetEngineInterface(
			env,
			ctx->engineObject,
			ctx->engineEngine);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Create output mix object
	CreateOutputMix(
			env,
			ctx->engineEngine,
			ctx->outputMixObject);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Realize output mix object
	RealizeObject(env, ctx->outputMixObject);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Initialize buffer
	InitPlayerBuffer(
			env,
			ctx->wav,
			ctx->buffer,
			ctx->bufferSize);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Create the buffer queue audio player object
	CreateBufferQueueAudioPlayer(
			ctx->wav,
			ctx->engineEngine,
			ctx->outputMixObject,
			ctx->audioPlayerObject);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Realize audio player object
	RealizeObject(env, ctx->audioPlayerObject);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Get audio player buffer queue interface
	GetAudioPlayerBufferQueueInterface(
			env,
			ctx->audioPlayerObject,
			ctx->audioPlayerBufferQueue);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Registers the player callback
	RegisterPlayerCallback(
			env,
			ctx->audioPlayerBufferQueue,
			ctx);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Get audio player play interface
	GetAudioPlayerPlayInterface(
			env,
			ctx->audioPlayerObject,
			ctx->audioPlayerPlay);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Set the audio player state playing
	SetAudioPlayerStatePlaying(env, ctx->audioPlayerPlay);
	if (0 != env->ExceptionOccurred())
		goto exit;

	// Enqueue the first buffer to start
	PlayerCallback(ctx->audioPlayerBufferQueue, ctx);

exit:
	// Destroy if exception occurred
	if (0 != env->ExceptionOccurred())
		DestroyContext(ctx);
}
