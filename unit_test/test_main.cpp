#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

GTEST_API_ int main(int argc, char **argv) 
{

#if defined(_DEBUG) && defined(WIN32)

  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

#endif

  int window_width = 512, window_height = 512;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow("test!"); 
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  testing::InitGoogleTest(&argc, argv);
  int gtestRst = RUN_ALL_TESTS();

  return gtestRst;
}
