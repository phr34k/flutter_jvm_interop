#include "include/native_jvm/native_jvm_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "native_jvm_plugin.h"

void NativeJvmPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  native_jvm::NativeJvmPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
