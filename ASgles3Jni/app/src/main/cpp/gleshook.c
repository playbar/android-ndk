

#include <stdint.h>
#include <jni.h>
#include <GLES3/gl3.h>
#include "mylog.h"
#include "inlineHook.h"

int (*old_puts)(const char *) = NULL;
int new_puts(const char *string)
{
    LOGI("new_puts");
    old_puts("inlineHook success");
}

void (*old_glShaderSource) (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) = NULL;
void MJ_glShaderSource (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
{
    LOGI("old_glShaderSource");
    old_glShaderSource(shader, count, string, length);
}

int hook(uint32_t target_addr, uint32_t new_addr, uint32_t **proto_addr)
{
    if (registerInlineHook(target_addr, new_addr, proto_addr) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) target_addr) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

int unHook(uint32_t target_addr)
{
    if (inlineUnHook(target_addr) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}


JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3Hook_initHook(JNIEnv* env, jobject obj)
{
//    puts("test");
    unHook((uint32_t) glShaderSource);
    LOGI("initHook");
//    hook((uint32_t) puts, (uint32_t) new_puts, (uint32_t **) &old_puts);
    hook((uint32_t) glShaderSource, (uint32_t) MJ_glShaderSource, (uint32_t **) &old_glShaderSource);
    LOGI("initHook after");
//    puts("test");
//    unHook((uint32_t) puts);
}

JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3Hook_hookTest(JNIEnv* env, jobject obj)
{
    puts("test");
}

JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3Hook_unInitHook(JNIEnv* env, jobject obj)
{
//    unHook((uint32_t) puts);
    unHook((uint32_t) glShaderSource);
}
