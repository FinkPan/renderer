#include <iostream>
#include <vector>
#include <stack>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "hs_renderer/renderer/OBJLoader.hpp"

std::string file_path = "list.txt";
OBJLoader obj_loader;

void init(void)
{
   obj_loader.LoadOBJ(file_path);
   obj_loader.Start();
}


void keyboard(unsigned char key, int x, int y)
{
//   switch (key)
//   {
//   case 'a':
//     view_transformer.Translate(-1.0f, 0.0f, 0.0f);
//     break;
//   case 'd':
//     view_transformer.Translate(1.0f, 0.0f, 0.0f);
//     break;
// 
//   case 'w':
//     view_transformer.Translate(0.0f, 1.0f, 0.0f);
//     break;
//   case 's':
//     view_transformer.Translate(0.0f, -1.0f, 0.0f);
//     break;
// 
//   case 'q':
//     view_transformer.Translate(0.0f, 0.0f, 1.0f);
//     break;
//   case 'e':
//     view_transformer.Translate(0.0f, 0.0f, -1.0f);
//     break;
// 
//   case 'j':
//     view_transformer.Rotate(10.0f,-1.0f, 0.0f, 0.0f);
//     break;
//   case 'l':
//     view_transformer.Rotate(10.0f,1.0f, 0.0f, 0.0f);
//     break;
// 
//   default:
//     break;
//   }

  glutPostRedisplay();
}

void SpecialKeysBoard(int key, int x, int y)
{
//   switch (key)
//   {
//   case GLUT_KEY_LEFT:
//     view_transformer.RotateR(10.0f,0.0f, 1.0f, 0.0f);
//     break;
//   case GLUT_KEY_RIGHT:
//     view_transformer.RotateR(10.0f,0.0f, -1.0f, 0.0f);
// 
//     break;
//   case GLUT_KEY_UP:
//     view_transformer.Rotate(10.0f, 1.0f, 0.0f, 0.0f);
//     break;
//   case GLUT_KEY_DOWN:
//     view_transformer.Rotate(10.0f, -1.0f, 0.0f, 0.0f);
//     break;
// 
//   default:
//     break;
//   }

  glutPostRedisplay();

}

void mouseCB(int button, int state, int x, int y)
{
  glutPostRedisplay();

}

void mouseMotionCB(int x, int y)
{
  glutPostRedisplay();
}

void mouseMidCB(int wheel, int direction, int x, int y)
{
//   if (direction == 1)  //向上滚动
//   {
//     view_transformer.Zoom_Out(1.2f);
//   }
//   else                 //向下滚动
//   {
//     view_transformer.Zoom_Out(0.8f);
//   }
  glutPostRedisplay();
}

void display(void)
{

  glFlush();
}

void reshape(int width, int height)
{
//   glViewport(0, 0,(GLsizei)width, (GLsizei)height);
//   std::cout << "width:" << width << " height:" << height << "\n";
//   view_transformer.SetViewport(0,0,width,height);
}

void Cleanup()
{
}

int main(int argc, char** argv)
{
  int window_width = 512, window_height = 512;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow("test!"); 
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(SpecialKeysBoard);
  glutMouseFunc(mouseCB);
  glutMotionFunc(mouseMotionCB);
  glutMouseWheelFunc(mouseMidCB);
  glutReshapeFunc(reshape);
  glutCloseFunc(Cleanup);
  glutMainLoop();


  return 0;
}