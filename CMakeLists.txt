find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

project (JmeterCustomSampler)

# # generate OpenCBDCJavaClient.h stub
# set (_stubDir "${CMAKE_CURRENT_BINARY_DIR}")
# message (STATUS "${_classDir}")
# add_custom_command(
#     OUTPUT OpenCBDCJavaClient.h
#     COMMAND ${Java_JAVAH_EXECUTABLE} -verbose
#         -classpath target/classes
#         -d ${_stubDir}
#         -jni hu.bme.mit.opencbdc.OpenCBDCJavaClient
#     )

# generate libfoo.jnilib
include_directories(${JNI_INCLUDE_DIRS} ${_classDir} ${_stubDir})
add_library(client-jni MODULE OpenCBDCJavaClient.cpp hu_bme_mit_opencbdc_OpenCBDCJavaClient.h)
set_target_properties(client-jni PROPERTIES SUFFIX ".jnilib")
target_link_libraries(  client-jni 
                        ${JNI_LIBRARIES} 
                        client
                        atomizer
                        bech32
                        atomizer
                        watchtower
                        sentinel
                        sentinel_interface
                        transaction
                        locking_shard
                        coordinator
                        rpc
                        network
                        common
                        serialization
                        crypto
                        secp256k1
                        ${NURAFT_LIBRARY}
                        ${CMAKE_THREAD_LIBS_INIT})
message(STATUS "Jar file ${_jarFile}")
message(STATUS "Class compiled to ${_classDir}")