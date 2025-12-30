#include "Shader_program.h"
#include <GL/glew.h>

namespace GL_layer {
void Shader_program::Shader::test_shader(){
    GLchar infoLog[512];
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

Shader_program::Shader::Shader(GLenum type,std::string & code):shader(glCreateShader(type)){
    const GLchar * code_=code.c_str();
    glShaderSource(shader, 1,&code_, nullptr);
    glCompileShader(shader);

    test_shader();
}

void Shader_program::Shader::Attach(GLuint Program){
    glAttachShader(Program,shader);
}

Shader_program::Shader::~Shader(){
    std::cout << "glDeleteShader"<< std::endl;
    glDeleteShader(shader);
}


void Shader_program::test_program(){
    GLchar infoLog[512];
    GLint success;
    glGetProgramiv(Program, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(Program, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

GLuint Shader_program::create_program(){
    return glCreateProgram();
}
void Shader_program::Link_program(){
    glLinkProgram(Program);
};

Shader_program::~Shader_program(){
    std::cout << "glDeleteProgram"<< std::endl;
    glDeleteProgram(Program);
}

void Shader_program::Use() const{
    glUseProgram(Program);
}

GLint Shader_program::Uniform_Location(const char * name){
    return glGetUniformLocation(Program,name);
}

}// namespace GL_layer
