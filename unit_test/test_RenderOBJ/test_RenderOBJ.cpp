//内存泄漏检测
#define _CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include "crtdbg.h"

#include <gtest/gtest.h>

#include <string>
#include <fstream>

#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp>


#include "hs_renderer/renderer/OBJData.hpp"
#include "hs_renderer/renderer/RenderOBJ.hpp"


namespace
{
  inline void EnableMemLeakCheck()
  {
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
  }
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

  int LoadShader(const char* vs, const char* fs);
  const char* vertex_shader_source = {
    "#version 330 core\n"
    "layout(location = 0) in vec4 setPosition;"
    "layout(location = 1) in vec3 vertexUV;"
    "out vec2 UV;"
    "uniform mat4 MVP;"
    "void main()"
    "{"
    "gl_Position =  MVP * setPosition;"
    "UV = vertexUV.xy;"
    "}"
  };

  const char* fragment_shader_source = {
    "#version 330 core\n"
    "in vec2 UV;"
    "out vec3 color;"
    "uniform sampler2D myTextureSampler;"
    "void main()"
    "{"
    "vec2 rUV = vec2(UV.x,1-UV.y);"
    "color = texture(myTextureSampler, rUV).rgb;"
    "}"
  };

  GLuint program_id_;
  GLuint location_mvp_;
  GLuint location_color_;

  static void error_callback(int error, const char* description)
  {
    fputs(description, stderr);
  }

  TEST(TestRenderOBJ, LoadOBJ_OPENGL)
  {
    EnableMemLeakCheck();
    //_CrtSetBreakAlloc(687);
    //   if (argc ==1)
    //   {
    //     std::cout << "Usage: hs_readOBJ.exe obj_name\n";
    //     return -1;
    //   }

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
      exit(EXIT_FAILURE);
    window = glfwCreateWindow(512, 512, "Simple example", NULL, NULL);
    if (!window)
    {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

     ASSERT_EQ(GLEW_OK, glewInit());

     OBJData obj_data;
     obj_data.LoadOBJ("lod_17.bin");

     RenderOBJ render_obj;
     render_obj.LoadOBJ_OPENGL(obj_data);

     program_id_ = LoadShader(vertex_shader_source, fragment_shader_source);
     glUseProgram(program_id_);
     location_mvp_ = glGetUniformLocation(program_id_, "MVP");
     location_color_  = glGetUniformLocation(program_id_, "myTextureSampler");
     glm::mat4 Projection = glm::ortho(-3.0f, 3.0f, -3.0f,3.0f,-100.0f,100.0f);
     glm::mat4 View = glm::lookAt(
       glm::vec3(1,1,1), //eye表示我们眼睛在"世界坐标系"中的位置
       glm::vec3(0,0,0), //center表示眼睛"看"的那个点的坐标,
       glm::vec3(0,1,0)  //up坐标表示观察者本身的方向
       );
     glm::mat4 Model = glm::mat4(1.0f);
     glm::mat4 MVP_ = Projection * View * Model;

     while (!glfwWindowShouldClose(window))
     {
       glClearColor(0.5f, 0.5f, 0.5f,0.0f);
       glClearDepth(1.0f);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       glUniformMatrix4fv(location_mvp_, 1, GL_FALSE, glm::value_ptr(MVP_));  //设置投影矩阵MVP 

       render_obj.Render();
       glFlush();
       glfwSwapBuffers(window);
       glfwPollEvents();
     }
     glfwTerminate();
     render_obj.ReleaseGLResource();
  }



  int LoadShader(const char* vs, const char* fs)
  {
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    GLint result = GL_FALSE;
    int info_log_length;
    glShaderSource(vertex_shader_id, 1, &vs, NULL);
    glCompileShader(vertex_shader_id);
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0)
    {
      std::vector<GLchar> vertex_shader_error_message(info_log_length + 1);
      glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL,
        &vertex_shader_error_message[0]);
      std::cout<<&vertex_shader_error_message[0]<<"\n";
    }

    glShaderSource(fragment_shader_id, 1, &fs, NULL);
    glCompileShader(fragment_shader_id);
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0)
    {
      std::vector<GLchar> fragment_shader_error_message(info_log_length + 1);
      glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL,
        &fragment_shader_error_message[0]);
      std::cout<<&fragment_shader_error_message[0]<<"\n";
    }

    program_id_ = glCreateProgram();
    glAttachShader(program_id_, vertex_shader_id);
    glAttachShader(program_id_, fragment_shader_id);
    glLinkProgram(program_id_);

    glGetProgramiv(program_id_, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0)
    {
      std::vector<GLchar> program_error_message(info_log_length + 1);
      glGetProgramInfoLog(program_id_, info_log_length, NULL,
        &program_error_message[0]);
      std::cout<<&program_error_message[0]<<"\n";
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id_;
  }

}