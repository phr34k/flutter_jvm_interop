#include "include/native_jvm/native_jvm_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <locale>
#include <codecvt>
#include <functional>
#include <optional>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include "clr.h"

namespace {

class NativeJVMPlugin : public flutter::Plugin {
 public:
  flutter::WindowProcDelegate _delegate;
  clr::container* _container = nullptr;
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  NativeJVMPlugin();

  virtual ~NativeJVMPlugin();

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  std::optional<LRESULT> HandleWindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);



};

// static
void NativeJVMPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "native_jvm",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<NativeJVMPlugin>();
  registrar->RegisterTopLevelWindowProcDelegate(plugin.get()->_delegate);  

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

NativeJVMPlugin::NativeJVMPlugin() {
    _delegate = [&](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
        return HandleWindowsProc(hwnd, message, wparam, lparam);
    };
}

NativeJVMPlugin::~NativeJVMPlugin() {

}

void NativeJVMPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (method_call.method_name().compare("started") == 0) {
        if (_container == nullptr)
        {
            result->Success(flutter::EncodableValue(false));
        }
        else
        {
            result->Success(flutter::EncodableValue(true));
        }
    }
  else if (method_call.method_name().compare("ready") == 0) {
    if (_container != nullptr) {
        clr _clr;
        bool v = _clr.ready(_container);
        result->Success(flutter::EncodableValue(v));      
    }
    else
    {
        result->Error("E0002", "CLR was not yet initialized");
    }    
  }
  else if (method_call.method_name().compare("start") == 0) {
      if (_container == nullptr)
      {
          const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());
          std::string _assemblyName = static_cast<std::string>( std::get<std::string>(arguments->at(flutter::EncodableValue("assemblyName"))) );
          std::string _appDomainManager = static_cast<std::string>( std::get<std::string>(arguments->at(flutter::EncodableValue("appDomainManager"))) );
          std::string _logDirectory = static_cast<std::string>( std::get<std::string>(arguments->at(flutter::EncodableValue("logDirectory"))) );
          std::wstring assemblyName( _assemblyName.begin(), _assemblyName.end() );
          std::wstring appDomainManager( _appDomainManager.begin(), _appDomainManager.end() );

          clr _clr;
          _clr.start(_container, _logDirectory.c_str(), _assemblyName.c_str(), _appDomainManager.c_str());
          result->Success(flutter::EncodableValue(true));
      }
      else
      {
          result->Error("E0001", "CLR was already initialized");
      }
  }
  else if (method_call.method_name().compare("stop") == 0) 
  {
      if (_container != nullptr)
      {
          clr _clr;
          _clr.stop(_container);
          _container = nullptr;
          result->Success(flutter::EncodableValue(true));
      }
      else
      {
          result->Error("E0002", "CLR was not yet initialized");
      }
  }
  else if (method_call.method_name().compare("setProperty") == 0) 
  {
      if (_container != nullptr)
      {
            const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());
            std::string _width = static_cast<std::string>( std::get<std::string>(arguments->at(flutter::EncodableValue("key"))) );
            std::string _height = static_cast<std::string>( std::get<std::string>(arguments->at(flutter::EncodableValue("value"))) );

          clr _clr;
          auto res = _clr.setProperty(_container, _width.c_str(), _height.c_str());
          _container = nullptr;
          result->Success(flutter::EncodableValue(res));
      }
      else
      {
          result->Error("E0002", "CLR was not yet initialized");
      }
  }  
  else if (method_call.method_name().compare("getPlatformVersion") == 0) 
  {
      if (_container != nullptr)
      {            
          clr _clr;
          auto res = _clr.get_version(_container);
          result->Success(flutter::EncodableValue(res));
      }
      else
      {
          result->Error("E0002", "CLR was not yet initialized");
      }


/*
    std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    */

    


  } else {
    result->NotImplemented();
  }
}

std::optional<LRESULT> NativeJVMPlugin::HandleWindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{   
    /*    
    if (_container != nullptr)
    {
        clr _clr;
        if (_clr.wndproc(_container, hwnd, message, wparam, lparam) == true)
        {
            return 0;
        }
    }
    */
    
    return{};
}

}  // namespace

void NativeJvmPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  NativeJVMPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
