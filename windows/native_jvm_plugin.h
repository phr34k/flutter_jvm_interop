#ifndef FLUTTER_PLUGIN_NATIVE_JVM_PLUGIN_H_
#define FLUTTER_PLUGIN_NATIVE_JVM_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace native_jvm {

class NativeJvmPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  NativeJvmPlugin();

  virtual ~NativeJvmPlugin();

  // Disallow copy and assign.
  NativeJvmPlugin(const NativeJvmPlugin&) = delete;
  NativeJvmPlugin& operator=(const NativeJvmPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace native_jvm

#endif  // FLUTTER_PLUGIN_NATIVE_JVM_PLUGIN_H_
