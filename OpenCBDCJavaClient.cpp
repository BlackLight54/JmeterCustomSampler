#include "atomizer_client.hpp"
#include "bech32/bech32.h"
#include "bech32/util/strencodings.h"
#include "client.hpp"
#include "crypto/sha256.h"
#include "hu_bme_mit_opencbdc_OpenCBDCJavaClient.h"
#include "twophase_client.hpp"
#include "uhs/transaction/messages.hpp"
#include "util/common/config.hpp"
#include "util/serialization/util.hpp"

#include <future>
#include <iostream>
#include <string>

static constexpr auto bits_per_byte = 8;
static constexpr auto bech32_bits_per_symbol = 5;

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_send(JNIEnv* env,
                                                 jobject obj,
                                                 jobjectArray arr) {
    std::ostringstream strCout;
    

    std::vector<std::string> args = {};
    jsize argc = env->GetArrayLength(arr);
    strCout << "transaction params: ";
    for(int i = 0; i < argc; i++) {
        args.push_back(
            env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arr, i),
                                   NULL));
    }
    for(int i = 0; i < argc; i++) {
        strCout << args[i];
        strCout << " ";
    }
    strCout << std::endl;

    return env->NewStringUTF(strCout.str().c_str());
}
/* 
JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_mint(JNIEnv* env,
                                                 jobject obj,
                                                 jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_fan(JNIEnv* env,
                                                jobject obj,
                                                jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_sync(JNIEnv* env,
                                                 jobject obj,
                                                 jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_newAddress(JNIEnv* env,
                                                       jobject obj,
                                                       jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_info(JNIEnv* env,
                                                 jobject obj,
                                                 jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_importInput(JNIEnv* env,
                                                        jobject obj,
                                                        jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_confirmTx(JNIEnv* env,
                                                      jobject obj,
                                                      jobjectArray arr) {
    return env->NewStringUTF("");
}
 */