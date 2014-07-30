#include "hs_renderer/renderer/OBJRenderer.hpp"

OBJRenderer::OBJRenderer()
{
}

OBJRenderer::~OBJRenderer()
{
}

void OBJRenderer::init()
{
  program_id_ = LoadShader(vertex_shader_source, fragment_shader_source);
  glUseProgram(program_id_);
  location_mvp_ = glGetUniformLocation(program_id_, "MVP");
  location_color_ = glGetUniformLocation(program_id_, "setColor");
  glUseProgram(0);

  glm::mat4 Projection = glm::ortho(-3.0f, 3.0f, -3.0f,3.0f,-100.0f,100.0f);
  glm::mat4 View = glm::lookAt(
    glm::vec3(1,1,1), //eye表示我们眼睛在"世界坐标系"中的位置
    glm::vec3(0,0,0), //center表示眼睛"看"的那个点的坐标,
    glm::vec3(0,1,0)  //up坐标表示观察者本身的方向
    );
  glm::mat4 Model = glm::mat4(1.0f);
//  MVP_ = Projection * View * Model;

}


int OBJRenderer::LoadShader(const char* vs, const char* fs)
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


const char* OBJRenderer::vertex_shader_source = {
  "#version 330 core\n"
  "layout(location = 0) in vec3 setPosition;"
  "out vec3 thecolor;"
  "uniform mat4 MVP;"
  "uniform vec3 setColor;"
  "void main()"
  "{"
  "gl_Position =  MVP * vec4(setPosition,1);"
  "thecolor = setColor;"
  "}"
};

const char* OBJRenderer::fragment_shader_source = {
  "#version 330 core\n"
  "in vec3 thecolor;"
  "out vec3 color;"
  "void main()"
  "{"
  "color = thecolor;"
  "}"
};
