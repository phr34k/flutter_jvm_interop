# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

# Project-level configuration.
set(PROJECT_NAME "native_jvm")
project(${PROJECT_NAME} LANGUAGES CXX)

# run cmake with environment variable to JAVA_SDK set to /usr/lib/jvm/java-1.16.0-openjdk-amd64/
if(DEFINED ENV{JAVA_SDK})	
	set(JAVA_SDK "$ENV{JAVA_SDK}" CACHE STRING "" FORCE)
endif()

if(NOT DEFINED CACHE{JAVA_SDK})
	message(FATAL_ERROR "Set JAVA_SDK=/usr/lib/jvm/java-1.16.0-openjdk-amd64/ to configure java installation")
else()
	message("Set JAVA_SDK=/usr/lib/jvm/java-1.16.0-openjdk-amd64/ to update java installation")
endif()

# This looks the path containing jni.h in ${JAVA_SDK}/include e.g. /usr/lib/jvm/java-1.16.0-openjdk-amd64/include
find_path(JNI_INCLUDE_PATH NAMES jni.h REQUIRED PATHS "${JAVA_SDK}" PATH_SUFFIXES include )

# This looks the path containing libjvm.so in ${JAVA_SDK}/lib/server e.g. /usr/lib/jvm/java-1.16.0-openjdk-amd64/lib/server
find_path(JVM_LIB NAMES libjvm.so REQUIRED PATHS  "${JAVA_SDK}lib/server"  )

# This looks for the java compiler ( javac )
find_program(JAVAC javac REQUIRED PATHS "${JAVA_SDK}bin/" NO_DEFAULT_PATH )

message("${JVM_LIB}")
message("${JAVAC}")

# This resolves the `/java` folder and then uses a cmake install rule to compile the main.java 
# with javac on every flutter run/build invocation similair how asset bundles are build.
get_filename_component(JAVA_SOURCE_PATH "${CMAKE_SOURCE_DIR}/../../java" ABSOLUTE)
install(CODE "execute_process(COMMAND ${JAVAC} -d ${JAVA_SOURCE_PATH} ${JAVA_SOURCE_PATH}/main.java)" RUNTIME_COMPONENT)

# This value is used when generating builds using this plugin, so it must
# not be changed.
set(PLUGIN_NAME "native_jvm_plugin")

# Define the plugin library target. Its name must not be changed (see comment
# on PLUGIN_NAME above).
#
# Any new source files that you add to the plugin should be added here.
add_library(${PLUGIN_NAME} SHARED
  "native_jvm_plugin.cc"
  "jvm.cpp"
  "jvm.h"  
)

# Apply a standard set of build settings that are configured in the
# application-level CMakeLists.txt. This can be removed for plugins that want
# full control over build settings.
apply_standard_settings(${PLUGIN_NAME})

# Symbols are hidden by default to reduce the chance of accidental conflicts
# between plugins. This should not be removed; any symbols that should be
# exported should be explicitly exported with the FLUTTER_PLUGIN_EXPORT macro.
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)

# Source include directories and library dependencies. Add any plugin-specific
# dependencies here.
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter)
target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::GTK)

target_include_directories(${PLUGIN_NAME} PRIVATE "${JNI_INCLUDE_PATH};${JNI_INCLUDE_PATH}/linux")
target_link_directories(${PLUGIN_NAME} PRIVATE "${JVM_LIB};${JVM_LIB}/../")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter)
target_link_libraries(${PLUGIN_NAME} PRIVATE "jvm")
target_link_libraries(${PLUGIN_NAME} PRIVATE "jawt")

# List of absolute paths to libraries that should be bundled with the plugin.
# This list could contain prebuilt libraries, or libraries created by an
# external build triggered from this build file.
set(native_jvm_bundled_libraries
  ""
  PARENT_SCOPE
)
