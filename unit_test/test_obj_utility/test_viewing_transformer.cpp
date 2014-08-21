//内存泄漏检测
#define _CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include "crtdbg.h"

#include <string>
#include <iostream>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "gtest/gtest.h"
#include "hs_math/linear_algebra/eigen_macro.hpp"
#include "hs_math/geometry/rotation.hpp"

#include "hs_renderer/obj_utility/viewing_transformer.hpp"

inline void EnableMemLeakCheck()
{
  _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace
{
  typedef float Scalar;
  typedef hs::renderer::ViewingTransformer<Scalar> ViewingTransformer;
  typedef EIGEN_MATRIX(Scalar, 4, 4) TransformMatrix;
  typedef EIGEN_VECTOR(Scalar, 2) Vector2;
  typedef EIGEN_VECTOR(Scalar, 3) Vector3;
  //typedef EIGEN_MATRIX(Scalar, 8, 3) Cube;
  typedef EIGEN_MATRIX(Scalar, 3, 8) Cube;



  ViewingTransformer vt;
  TransformMatrix projection_matrix;

  GLuint vertexBuffer;
  GLuint indexBuffer;
  GLuint vaoObject;
  GLuint ProgramID;
  GLuint LocationMVP;
  GLuint LocationMVP1;

  void GenerateCube(const Vector3& min, const Vector3& max, Cube& vd)
  {
    int x = 0, y = 1, z = 2;
    vd << min[x],
          min[x],
          max[x],
          max[x],

          min[x],
          min[x],
          max[x],
          max[x],

          min[y],
          max[y],
          max[y],
          min[y],

          min[y],
          max[y],
          max[y],
          min[y],

          max[z],
          max[z],
          max[z],
          max[z],

          min[z],
          min[z],
          min[z],
          min[z]; 


//     vd << min[x], min[y], max[z],
//           min[x], max[y], max[z],
//           max[x], max[y], max[z],
//           max[x], min[y], max[z],
// 
//           min[x], min[y], min[z],
//           min[x], max[y], min[z],
//           max[x], max[y], min[z],
//           max[x], min[y], min[z];      
  }

  const GLfloat vertexData[] = 
  {
    1.0f,0.0f,0.0f,
    0.0f,0.0f,-1.0f,
   -1.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f
  };
  GLsizeiptr vDsize = 4*3*sizeof(GLfloat);

  const GLfloat colorData[] = 
  {
    1.0f,0.0f,0.0f,
    1.0f,1.0f,0.0f,
    1.0f,0.0f,1.0f,
    1.0f,0.5f,0.0f,
    0.5f,1.0f,0.3f,
    0.7f,0.2f,0.9f,
    0.6f,0.2f,0.2f,
    0.3f,0.7f,0.6f,
  };
  GLsizeiptr cDsize = 8*3*sizeof(GLfloat);

  const GLuint indexData[] = 
  {
    0,1,3,1,2,3,
    0,3,4,3,4,7,
    0,1,4,1,4,5,
    1,2,5,2,5,6,
    2,3,7,2,6,7,
    4,5,6,4,6,7

  };
  GLsizei ivDcount = 12*3;
  GLsizeiptr ivDsize = ivDcount*sizeof(GLuint);

  static const char* vShader = {
    "#version 330 core\n"
    "layout(location = 0) in vec3 setPosition;"
    "layout(location = 1) in vec3 setColor;"
    "uniform mat4 MVP;"
    "out vec3 thecolor;"
    "void main()"
    "{"
    "gl_Position =  MVP * vec4(setPosition,1);"
    //"gl_Position =  vec4(setPosition,1);"
    "thecolor = setColor;"
    "}"
  };

  static const char* fShader = {
    "#version 330 core\n"
    "in vec3 thecolor;;"
    "out vec3 color;"
    "void main()"
    "{"
    "color = thecolor;"
    "}"
  };

  GLuint LoadShaders(const char * VertexShaderCode,
    const char * FragmentShaderCode)
  {
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    //printf("Compiling shader : VertexShaderCode\n");
    glShaderSource(VertexShaderID, 1, &VertexShaderCode , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
      glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
      printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    //printf("Compiling shader : FragmentShaderCode\n");
    glShaderSource(FragmentShaderID, 1, &FragmentShaderCode , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
      glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
      printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    // printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      std::vector<char> ProgramErrorMessage(InfoLogLength+1);
      glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
      printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
  }

  void InitializeVertexBuffer(const Cube& cube)
  {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, cDsize+cDsize ,NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER,0,cDsize,cube.data());
    glBufferSubData(GL_ARRAY_BUFFER,cDsize,cDsize,colorData);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ivDsize ,indexData, GL_STATIC_DRAW);

  }

  void InitializeVertexArrayObjects()
  {
    glGenVertexArrays(1,&vaoObject);
    glBindVertexArray(vaoObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)cDsize);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
  }

  void init()
  {
    Cube cube;
    Vector3 min, max;
    min << -1.0f, -1.0f, -1.0f;
    max << 1.0f, 1.0f, 1.0f;
    GenerateCube(min,max,cube);
    std::cout << "cube:\n" << cube << "\n\n";

    ProgramID = LoadShaders(vShader,fShader);
    LocationMVP = glGetUniformLocation(ProgramID, "MVP");
    glUseProgram(ProgramID);

    InitializeVertexBuffer(cube);
    InitializeVertexArrayObjects();


  }

  void display()
  {
    glClearColor(0.2f, 0.2f, 0.2f,1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::cout << "projection_matrix:\n" << vt.ProjectionMatrix() << "\n\n";

    TransformMatrix transform_matrix = vt.ViewingTransformMatrix();
    float mvp[16] = { 0.0f };

//     TransformMatrix ViewMatrix;
//     ViewMatrix << 1.0f,0.0f,0.0f,0.0f,
//                   0.0f,1.0f,0.0f,0.0f,
//                   0.0f,0.0f,1.0f,-10.0f,
//                   0.0f,0.0f,0.0f,1.0f;
// 
//     std::cout << "ViewMatrix:\n" << ViewMatrix << "\n\n";
// 
// 
//     TransformMatrix ProjetMatrix = transform_matrix * ViewMatrix;

    Scalar viewing_unit_per_pixel =
      std::abs((vt.right() - vt.left()) /
      Scalar(vt.viewport_width()));

    std::cout << "viewing_unit_per_pixel:\n" << viewing_unit_per_pixel << "\n\n";

    glUniformMatrix4fv(LocationMVP, 1, GL_FALSE, transform_matrix.data());

    glBindVertexArray(vaoObject);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(GL_TRIANGLES, ivDcount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glFlush();
  }

  void keyboard(unsigned char key, int x, int y)
  {

    switch (key)
    {
    case '-':
      vt.Scale2D(2);
      break;
    case '=':
      vt.Scale2D(0.5);
      break;

    case 'a':
      vt.RotationByView(0.5f, Vector3(1.0f,0.0f,0.0f));
      break;
    case 'd':
      vt.RotationByView(-0.5f, Vector3(1.0f,0.0f,0.0f));
      break;

    case 'w':
      vt.RotationByView(0.5f, Vector3(0.0f,1.0f,0.0f));
      break;
    case 's':
      vt.RotationByView(-0.5f, Vector3(0.0f,1.0f,0.0f));
      break;


    default:
      break;
    }

    glutPostRedisplay();
  }

  double mid_button_last_x = std::numeric_limits<double>::min();
  double mid_button_last_y = std::numeric_limits<double>::min();
  double mid_button_current_x = std::numeric_limits<double>::min();
  double mid_button_current_y = std::numeric_limits<double>::min();

  void mouseCB(int button, int state, int x, int y)
  {
    Vector2 translate;
    if (button == GLUT_MIDDLE_BUTTON)
    {
      switch (state)
      {
      case GLUT_DOWN:
        mid_button_current_x = (double)x;
        mid_button_current_y = (double)y;
        break;
      case GLUT_UP:
        mid_button_last_x =  (double)x;
        mid_button_last_y =  (double)y;
        std::cout << "DOWN x= " << mid_button_current_x << " y= " << mid_button_current_y << "\n";
        std::cout << "GLUT_UP x= " << mid_button_last_x << " y= " << mid_button_last_y << "\n";
        translate << Scalar(-(mid_button_last_x - mid_button_current_x)),
                     Scalar(mid_button_last_y - mid_button_current_y);
        vt.Translate2DByView(translate);
        std::cout << "translate:\n" << translate << "\n\n";
        break;
      default:
        std::cout << "default:\n";
        break;
      }
    }
    glutPostRedisplay();
  }

  void reshape(int width, int height)
  {
    glViewport(0, 0,(GLsizei)width, (GLsizei)height);
    std::cout << "width:" << width << " height:" << height << "\n";
    vt.SetViewport(0,0,width,height);
  }


  TEST(ViewingTransformer, ProjectionMatrix)
  {
    EnableMemLeakCheck();
    //_CrtSetBreakAlloc(765);

    Vector2 min, max;
    min << -1,-1;
    max << 1,1;
    //vt.SetProjectionBoundingBox2D(min, max);
    vt.SetPerspective(false);
    vt.left() = -10.5f;
    vt.right() = 10.5f;
    vt.bottom() = -10.5f;
    vt.top() = 10.5f;
    vt.n() = -10;
    vt.f() = 10;
    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseCB);
    glutReshapeFunc(reshape);

    // If the GLUT_ACTION_ON_WINDOW_CLOSE option has been set to
    //GLUT_ACTION_CONTINUE_EXECUTION, control will return to the function 
    //which called glutMainLoop; otherwise the application will exit.
    //该设置将返回给gtest而不直接exit(0);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION);
    glutMainLoop();
  }

}