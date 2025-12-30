#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include "incoming_signals_from_user.h"
class GLFWwindow;

namespace Win_layer {
class Win{
    GLFWwindow * window=nullptr;
protected:
    bool active=true;

    GLFWwindow* create_win(std::string name_okno_,int WIDTH, int HEIGHT);
public:
    Win_layer::Incoming_signals_from_user  ISFU;

    Win(std::string name_okno_,int WIDTH, int HEIGHT);
    ~Win();

    void swap_buffer();
    void make_context();
    bool isOpen() const;
    void set_cursor(std::string type);

    static int GLFW_Init();
    static void Terminate();
};
}// namespace Win_layer
#endif // WINDOW_H
