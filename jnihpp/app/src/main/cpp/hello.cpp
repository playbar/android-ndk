#include <jni/jni.hpp>
#include "javahello.h"

// The global references created during registration are purposefully leaked. Due to
// the design of Java/JNI, there is virtually no way to reliably release them. See
// http://bleaklow.com/2006/02/18/jni_onunload_mostly_useless.html for details.

static void RegisterLowLevel(JavaVM* vm)
{
    jni::JNIEnv& env = jni::GetEnv(*vm);

    static jni::jclass* systemClass = jni::NewGlobalRef(env, &jni::FindClass(env, "java/lang/System")).release();
    static jni::jclass* printStreamClass = jni::NewGlobalRef(env, &jni::FindClass(env, "java/io/PrintStream")).release();

    static jni::jfieldID& outFieldID = jni::GetStaticFieldID(env, *systemClass, "out", "Ljava/io/PrintStream;");
    static jni::jmethodID& printlnMethodID = jni::GetMethodID(env, *printStreamClass, "println", "(Ljava/lang/String;)V");

    auto greet = [] (jni::JNIEnv* env, jni::jobject*, jni::jarray<jni::jobject>* args)
    {
        jni::NullCheck(*env, args);

        jni::jstring& name = jni::SafeDereference(*env, reinterpret_cast<jni::jstring*>(
                jni::GetObjectArrayElement(*env, *args, 0)));

        auto greeting = std::u16string(u"Hello, ") +
                        std::get<0>(jni::GetStringChars(*env, name)).get() +
                        u" (Native Low-Level)";

        jni::CallMethod<void>(*env,
                              jni::GetStaticField<jni::jobject*>(*env, *systemClass, outFieldID),
                              printlnMethodID,
                              jni::NewString(*env, greeting));
    };

    jni::RegisterNatives(env, jni::FindClass(env, "com/example/LowLevelGreeter"),
                         jni::MakeNativeMethod("greet", "([Ljava/lang/String;)V", greet));
}

static void RegisterHighLevel(JavaVM* vm)
{
    struct Greeter     { static constexpr auto Name() { return "com/example/HighLevelGreeter"; } };
    struct System      { static constexpr auto Name() { return "java/lang/System"; } };
    struct PrintStream { static constexpr auto Name() { return "java/io/PrintStream"; } };

    jni::JNIEnv& env { jni::GetEnv(*vm) };

    static auto system = *jni::Class<System>::Find(env).NewGlobalRef(env).release();
    static auto printStream = *jni::Class<PrintStream>::Find(env).NewGlobalRef(env).release();

    static auto out = system.GetStaticField<jni::Object<PrintStream>>(env, "out");
    static auto println = printStream.GetMethod<void (jni::String)>(env, "println");

    auto greet = [] (jni::JNIEnv& env, jni::Object<Greeter>, jni::Array<jni::String> args)
    {
        system.Get(env, out).Call(env, println, jni::Make<jni::String>(env, u"Hello, " + jni::Make<std::u16string>(env, args.Get(env, 0)) + u" (Native High-Level)"));
    };

    jni::RegisterNatives(env, jni::Class<Greeter>::Find(env),
                         jni::MakeNativeMethod("greet", greet));
}

static void RegisterPeer(JavaVM* vm)
{
    struct Calculator
    {
        static constexpr auto Name() { return "com/example/Calculator"; }

        Calculator(JNIEnv&) { std::cout << "Native peer initialized" << std::endl; }
        Calculator(const Calculator&) = delete; // noncopyable
        ~Calculator() { std::cout << "Native peer finalized" << std::endl; }

        jni::jlong Add(jni::JNIEnv&, jni::jlong a, jni::jlong b) { return a + b; }
        jni::jlong Subtract(jni::JNIEnv&, jni::jlong a, jni::jlong b) { return a - b; }
    };

    jni::JNIEnv& env { jni::GetEnv(*vm) };

#define METHOD(MethodPtr, name) jni::MakeNativePeerMethod<decltype(MethodPtr), (MethodPtr)>(name)

    jni::RegisterNativePeer<Calculator>(env, jni::Class<Calculator>::Find(env), "peer",
                                        std::make_unique<Calculator, JNIEnv&>,
                                        "initialize",
                                        "finalize",
                                        METHOD(&Calculator::Add, "add"),
                                        METHOD(&Calculator::Subtract, "subtract"));
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*)
{
    JNIEnv* env = NULL;
    vm->GetEnv( (void**)&env, JNI_VERSION_1_6);
    registerNatives(env);
    RegisterPeer(vm);
    RegisterLowLevel(vm);
    RegisterHighLevel(vm);
    return jni::Unwrap(jni::jni_version_1_6);
}
