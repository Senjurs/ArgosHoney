import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';

class UIValidButton extends StatefulWidget {
  final Function onPressed;
  final String title;
  final Color? color;
  final Color? textColor;
  final Color? colorBorder;
  final double? widthBorder;

  const UIValidButton({Key? key, required this.onPressed, required this.title, this.color, this.textColor, this.colorBorder, this.widthBorder}) : super(key: key);

  @override
  _UIValidButtonState createState() => _UIValidButtonState();
}

class _UIValidButtonState extends State<UIValidButton> {
  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.fromLTRB(10, 5, 10, 0),
      child: Container(
        child: MaterialButton(
          elevation: 0,
          onPressed: () {
            widget.onPressed();
          },
          color: widget.color == null ? Color(0xFFEDFCF7) : widget.color,
          padding: EdgeInsets.zero,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(8.0),
            side: BorderSide(
              color: widget.colorBorder == null ? Color(0xFF5DE5B4) : widget.colorBorder!,
              width: widget.widthBorder == null ? 2 : widget.widthBorder!,
            ),
          ),
          child: Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Padding(
                padding: const EdgeInsets.fromLTRB(8, 10, 8, 10),
                child: Text(widget.title,
                    style: GoogleFonts.poppins(
                      textStyle: TextStyle(
                        fontWeight: FontWeight.w500,
                        color: widget.textColor == null ? Color(0xFF5DE5B4) : widget.textColor,
                        fontSize: 16,
                      ),
                    )),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
