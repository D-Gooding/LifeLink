import 'package:flutterflow_ui/flutterflow_ui.dart';
// Remember to copy the import above and check for any dependencies in the pubspec view

import 'package:flutter_flow/flutter_flow_widgets.dart';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';

class PlaygroundPageWidget extends StatefulWidget {
  const PlaygroundPageWidget({Key? key}) : super(key: key);

  @override
  _PlaygroundPageWidgetState createState() => _PlaygroundPageWidgetState();
}

class _PlaygroundPageWidgetState extends State<PlaygroundPageWidget> {
  final scaffoldKey = GlobalKey<ScaffoldState>();

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      key: scaffoldKey,
      appBar: AppBar(
        backgroundColor: Color(0xFF4B39EF),
        automaticallyImplyLeading: true,
        actions: [],
        centerTitle: true,
        elevation: 4,
      ),
      backgroundColor: Color(0xFF1F3353),
      drawer: Drawer(
        elevation: 16,
      ),
      body: SafeArea(
        child: GestureDetector(
          onTap: () => FocusScope.of(context).unfocus(),
          child: Padding(
            padding: EdgeInsetsDirectional.fromSTEB(10, 10, 10, 10),
            child: Column(
              mainAxisSize: MainAxisSize.max,
              crossAxisAlignment: CrossAxisAlignment.stretch,
              children: [
                Align(
                  alignment: AlignmentDirectional(0, 0.05),
                  child: Padding(
                    padding: EdgeInsetsDirectional.fromSTEB(0, 0, 0, 20),
                    child: Container(
                      width: 220,
                      height: 41,
                      decoration: BoxDecoration(
                        color: Color(0xFFEEEEEE),
                      ),
                    ),
                  ),
                ),
                Expanded(
                  child: Padding(
                    padding: EdgeInsetsDirectional.fromSTEB(45, 0, 45, 0),
                    child: GridView(
                      padding: EdgeInsets.zero,
                      gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                        crossAxisCount: 3,
                        crossAxisSpacing: 8,
                        mainAxisSpacing: 8,
                        childAspectRatio: 1,
                      ),
                      scrollDirection: Axis.vertical,
                      children: [
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                        FFButtonWidget(
                          onPressed: () {
                            print('Button pressed ...');
                          },
                          text: 'Button',
                          options: FFButtonOptions(
                            width: 130,
                            height: 40,
                            color: Color(0xFF4B39EF),
                            textStyle: GoogleFonts.getFont(
                              'Poppins',
                              color: Colors.white,
                              fontWeight: FontWeight.w600,
                              fontSize: 16,
                            ),
                            borderSide: BorderSide(
                              color: Colors.transparent,
                              width: 1,
                            ),
                            borderRadius: 12,
                          ),
                        ),
                      ],
                    ),
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
