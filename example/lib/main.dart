import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:native_jvm/native_jvm.dart';
import 'dart:io';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';

  @override
  void initState() {
    super.initState();
    initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    try {
      await NativeJVM.started;
      await NativeJVM.start(
          "C:\\Program Files\\Microsoft\\jdk-17.0.2.8-hotspot",
          "${Directory.current.path}/../java",
          logDirectory: "jvm.txt");

      // Platform messages may fail, so we use a try/catch PlatformException.
      try {
        platformVersion = await NativeJVM.platformVersion;
      } on PlatformException {
        platformVersion = 'Failed to get platform version.';
      }

      await NativeJVM.stop;
    } on PlatformException {
      platformVersion = 'Failed to start clr.';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Text('Running on: $_platformVersion\n'),
        ),
      ),
    );
  }
}
