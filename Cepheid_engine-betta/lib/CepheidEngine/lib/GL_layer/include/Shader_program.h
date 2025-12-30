#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include "GL_type_to_cpp.h"
#include <iostream>
#include <list>
#include <string>
namespace GL_layer {
class Shader_program{
    GLuint Program=0;
    class Shader{
        GLuint shader=0;
        void test_shader();
    public:
        Shader(GLenum type,std::string & code);

        void Attach(GLuint Program);
        ~Shader();
    };

    void test_program();
public:
    GLuint create_program();
    void Link_program();

    template<typename Data_Program>
    Shader_program(Data_Program data_program):Program(create_program()){
        std::list<Shader> shader_storage;
        for(auto &[Type,Code]:data_program.get_data()){
            shader_storage.emplace_back(static_cast<GLenum>(Type),Code);
            shader_storage.back().Attach(Program);
        }

        Link_program();
        test_program();
        std::cout<<"^Shader: "<<Program<<std::endl;
    }
    ~Shader_program();

    void Use() const;
    GLint Uniform_Location(const char * name);
};
}// namespace GL_layer
#endif // SHADER_PROGRAM_H
