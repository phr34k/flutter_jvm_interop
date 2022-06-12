#include "include/native_jvm/native_jvm_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>
#include <iostream>
#include "jvm.h"

#define NATIVE_JVM_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), native_jvm_plugin_get_type(), \
                              NativeJvmPlugin))

struct _NativeJvmPlugin {
  GObject parent_instance;
  clr::container* _container = nullptr;  
};

G_DEFINE_TYPE(NativeJvmPlugin, native_jvm_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void native_jvm_plugin_handle_method_call(
    NativeJvmPlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);

  if (strcmp(method, "started") == 0) {

      std::cout << "started" << std::endl;
        if (self->_container == nullptr)
        {
            g_autoptr(FlValue) result = fl_value_new_bool(false);
            response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));          
        }
        else
        {
            g_autoptr(FlValue) result = fl_value_new_bool(true);
            response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));          
        }
        
    }
  else if (strcmp(method, "ready") == 0) {
    
      std::cout << "ready" << std::endl;    
    if (self->_container != nullptr) {
        clr _clr;
        bool v = _clr.ready(self->_container);
        //result->Success(flutter::EncodableValue(v));   
        g_autoptr(FlValue) result = fl_value_new_bool(v);   
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));  
    }
    else
    {
        response = FL_METHOD_RESPONSE(fl_method_error_response_new("E0002", "CLR was not yet initialized", nullptr));  
    }  

    
  }
else if (strcmp(method, "start") == 0) {
      
      std::cout << "start" << std::endl;
      FlValue* args = fl_method_call_get_args(method_call);
      if (self->_container == nullptr && args != nullptr)
      {
          FlValue* __assemblyName = fl_value_lookup_string(args, "assemblyName");
          FlValue* __logDirectory = fl_value_lookup_string(args, "logDirectory");
          FlValue* __appDomainManager = fl_value_lookup_string(args, "appDomainManager");
          
          std::string _assemblyName = fl_value_get_string(__assemblyName);
          std::string _logDirectory = fl_value_get_string(__logDirectory);
          std::string _appDomainManager = fl_value_get_string(__appDomainManager);


          clr _clr;
          _clr.start(self->_container, _logDirectory.c_str(), _assemblyName.c_str(), _appDomainManager.c_str());
          g_autoptr(FlValue) result = fl_value_new_bool(true);   
          response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));  
      }
      else
      {
          response = FL_METHOD_RESPONSE(fl_method_error_response_new("E0001", "CLR was already initialized", nullptr));  
      }      
  }
  else if (strcmp(method, "stop") == 0) 
  {
      if (self->_container != nullptr)
      {
          clr _clr;
          _clr.stop(self->_container);
          self->_container = nullptr;
          g_autoptr(FlValue) result = fl_value_new_bool(true);   
          response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));  
      }
      else
      {
          response = FL_METHOD_RESPONSE(fl_method_error_response_new("E0002", "CLR was not yet initialized", nullptr));  
      }


  }
  else if (strcmp(method, "setProperty") == 0) 
  {
      std::cout << "setProperty" << std::endl;
      FlValue* args = fl_method_call_get_args(method_call);
      if (self->_container != nullptr && args != nullptr)
      {
        /*
            const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());
            std::string _width = static_cast<std::string>( std::get<std::string>(arguments->at(flutter::EncodableValue("key"))) );
            std::string _height = static_cast<std::string>( std::get<std::string>(arguments->at(flutter::EncodableValue("value"))) );
            std::wstring width( _width.begin(), _width.end() );
            std::wstring height( _height.begin(), _height.end() );



          clr _clr;
          auto res = _clr.setProperty(self->_container, width.c_str(), height.c_str());
          self->_container = nullptr;
          //result->Success(flutter::EncodableValue(res));
          */
          g_autoptr(FlValue) result = fl_value_new_bool(true);   
          response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));  
      }
      else
      {
          response = FL_METHOD_RESPONSE(fl_method_error_response_new("E0002", "CLR was not yet initialized", nullptr));  
      }

      
  }    

  else if (strcmp(method, "getPlatformVersion") == 0) {
    std::cout << "getPlatformVersion" << std::endl;
    struct utsname uname_data = {};
    uname(&uname_data);
    g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
    g_autoptr(FlValue) result = fl_value_new_string(version);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void native_jvm_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(native_jvm_plugin_parent_class)->dispose(object);
}

static void native_jvm_plugin_class_init(NativeJvmPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = native_jvm_plugin_dispose;
}

static void native_jvm_plugin_init(NativeJvmPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  NativeJvmPlugin* plugin = NATIVE_JVM_PLUGIN(user_data);
  native_jvm_plugin_handle_method_call(plugin, method_call);
}

void native_jvm_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  NativeJvmPlugin* plugin = NATIVE_JVM_PLUGIN(
      g_object_new(native_jvm_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "native_jvm",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
