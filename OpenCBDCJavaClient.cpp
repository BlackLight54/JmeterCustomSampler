#include "hu_bme_mit_opencbdc_OpenCBDCJavaClient.h"
#include <string>

JNIEXPORT jstring JNICALL Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_send(JNIEnv *env, jobject obj, jobjectArray arr)
{
    std::string result;
    for (size_t i = 0; i < env->GetArrayLength(arr); i++)
    {
        result.append(env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arr, i), NULL));
        result.append(" ");
        /* code */
    }
    return env->NewStringUTF(result.c_str());
    }

JNIEXPORT jstring JNICALL Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_mint(JNIEnv *env, jobject obj, jobjectArray arr)
{

    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_fan(JNIEnv *env, jobject obj, jobjectArray arr)
{
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_sync(JNIEnv *env, jobject obj, jobjectArray arr)
{
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_newAddress(JNIEnv *env, jobject obj, jobjectArray arr)
{
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_info(JNIEnv *env, jobject obj, jobjectArray arr)
{
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_importInput(JNIEnv *env, jobject obj, jobjectArray arr)
{
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_confirmTx(JNIEnv *env, jobject obj, jobjectArray arr)
{
    return env->NewStringUTF("");
}
