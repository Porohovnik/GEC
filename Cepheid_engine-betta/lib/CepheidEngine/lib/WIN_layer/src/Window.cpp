#include "window.h"
#include <iostream>
// GLEW нужно подключать до GLFW.
//#define GLFW_INCLUDE_NONE
//#include<GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
namespace Win_layer {
GLFWwindow* Win::create_win(std::string name_okno_,int WIDTH, int HEIGHT){
    std::cout<<"L"<<std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    std::cout<<"L1"<<std::endl;
    window=glfwCreateWindow(WIDTH, HEIGHT, name_okno_.data(), nullptr, nullptr);//<-тут создался конткст окна и всё с него

//    GLFWimage image;

//    glfwSetWindowIcon(window,0,&image);


    if (window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    return window;
}

Win::Win(std::string name_okno_,int WIDTH, int HEIGHT):
    window(create_win(name_okno_,WIDTH, HEIGHT)),ISFU(this->window){
    std::cout<<"||"<<window<<std::endl;

    glfwMakeContextCurrent(window);
    //glfwSwapInterval(0);
}

Win::~Win(){
    std::cout<<"eeeeeeeeeeeeeeeeeeeeeeee"<<std::endl;
    glfwDestroyWindow(window);
}

void Win::swap_buffer(){
    glfwSwapBuffers(window);
}

void Win::make_context(){
    glfwMakeContextCurrent(window);
}

bool Win::isOpen() const{
    return (!glfwWindowShouldClose(window) && active);
}

void Win::set_cursor(std::string type){
    ISFU.setting_curs.set_cursor(this->window,type);
}

int Win::GLFW_Init(){
    return  glfwInit();
};

void Win::Terminate(){
    glfwTerminate();
};
}// namespace Win_layer
