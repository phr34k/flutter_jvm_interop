cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME "native_jvm")
project(${PROJECT_NAME} LANGUAGES CXX)

find_path(JNI_INCLUDE_PATH NAMES jni.h REQUIRED PATHS  "C:\\Program Files\\Microsoft\\jdk-17.0.2.8-hotspot" "C:\\Program Files (x86)\\Java\\jdk1.7.0_55" PATH_SUFFIXES include )
find_path(JVM_LIB NAMES jvm.lib REQUIRED PATHS  "${JNI_INCLUDE_PATH}/../lib"  )




# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "native_jvm_plugin")

add_library(${PLUGIN_NAME} SHARED
  "native_jvm_plugin.cpp"
  "clr.cpp"
  "clr.h"
)

message("${JVM_LIB}")

apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(${PLUGIN_NAME} PRIVATE "${JNI_INCLUDE_PATH};${JNI_INCLUDE_PATH}/win32")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin mscoree)
target_link_libraries(${PLUGIN_NAME} PRIVATE "${JVM_LIB}/jvm.lib")
target_link_libraries(${PLUGIN_NAME} PRIVATE "${JVM_LIB}/jawt.lib")




# List of absolute paths to libraries that should be bundled with the plugin
set(native_jvm_bundled_libraries
  ""
  PARENT_SCOPE
)
