#include "shader.h"
#include <iostream>

using namespace std;

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // 1. retrive vertex ,fragment shader codes from file path
  string vertexShaderCode;
  string fragmentShaderCode;
  ifstream vShaderFile;
  ifstream fShaderFile;
  vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
  fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    // read file's buffer content into stream
    stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexShaderCode = vShaderStream.str();
    fragmentShaderCode = fShaderStream.str();
  } catch (ifstream::failure e) {
    cout << "SHADER FILE NOT READ SUCCESSFULLY " << endl;
  }

  const char *vShaderCode = vertexShaderCode.c_str();
  const char *fShaderCode = fragmentShaderCode.c_str();

  // 2. compile vertex,fragment shaders
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  };

  int success2;
  char infoLog2[512];

  // fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success2);
  if (!success2) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog2);
    std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n"
              << infoLog2 << std::endl;
  };

  int success3;
  char infoLog3[512];

  // SHADER PROGRAM
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  // print linking errors if any
  glGetProgramiv(ID, GL_LINK_STATUS, &success3);
  if (!success3) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog3);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog3 << std::endl;
  }

  // delete the shaders as they're linked into our program now and no longer
  // necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(ID); }

void Shader::setBool(const string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setFloat(const string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const string &name, glm::vec3 val) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(val));
}

void Shader::setVec2(const string &name, glm::vec2 val) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(val));
}