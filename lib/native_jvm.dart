import 'dart:async';

import 'package:flutter/services.dart';

class NativeJVM {
  static const MethodChannel _channel = const MethodChannel('native_jvm');

  static Future<bool> get started async {
    final bool version = await _channel.invokeMethod('started');
    return version;
  }

  static Future<bool> get ready async {
    final bool version = await _channel.invokeMethod('ready');
    return version;
  }

  static Future<bool> start(String assemblyName, String appDomainManager,
      {String logDirectory = 'logs'}) async {
    final bool version = await _channel.invokeMethod('start', {
      'assemblyName': assemblyName,
      'appDomainManager': appDomainManager,
      'logDirectory': logDirectory
    });
    return version;
  }

  static Future<bool> get stop async {
    final bool version = await _channel.invokeMethod('stop');
    return version;
  }

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  static Future<bool> setProperty(String key, String value) async {
    return _channel
        .invokeMethod<bool>('setProperty', {'key': key, 'value': value});
  }
}
