import 'package:flutter/material.dart';

class Param extends StatefulWidget {
  const Param({Key? key}) : super(key: key);

  @override
  _ParamState createState() => _ParamState();
}

class _ParamState extends State<Param> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        iconTheme: IconThemeData(
            color: Colors.black
        ),
        backgroundColor: Color(0xFFF9F9F9),
        elevation: 0.0,
        title: Text("Guardian System", style: TextStyle(color: Colors.black),),
      ),

    );
  }
}
