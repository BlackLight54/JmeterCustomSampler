find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

project (JmeterCustomSampler)

# compile java files to class files
set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.6" "-target" "1.6")
add_jar(OpenCBDCCustomSampler JmeterCustomSampler/src/main/java/hu/bme/mit/opencbdc/JMeterCustomSamplerOpenCBDCSend.java JmeterCustomSampler/src/main/java/hu/bme/mit/opencbdc/OpenCBDCJavaClient.java)
get_target_property(_jarFile OpenCBDCCustomSampler JAR_FILE)
get_target_property(_classDir OpenCBDCCustomSampler CLASSDIR)

# generate OpenCBDCJavaClient.h stub
set (_stubDir "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_command(
    OUTPUT OpenCBDCJavaClient.h
    COMMAND ${Java_JAVAH_EXECUTABLE} -verbose
        -classpath ${_classDir}
        -d ${_stubDir}
        -jni OpenCBDCJavaClient
    DEPENDS OpenCBDCCustomSampler
    )

# generate libfoo.jnilib
include_directories(${JNI_INCLUDE_DIRS} ${_classDir} ${_stubDir})
add_library(client-jni MODULE OpenCBDCJavaClient.c OpenCBDCJavaClient.h)
set_target_properties(client-jni PROPERTIES SUFFIX ".jnilib")
target_link_libraries(client-jni ${JNI_LIBRARIES})