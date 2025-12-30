#ifndef CURSOR_WIN_H
#define CURSOR_WIN_H
#include <string>
#include <unordered_map>
class GLFWcursor;
class GLFWwindow;

namespace Win_layer {
class Cursor_win{
public:
    double lastX = 400;
    double lastY = 300;
    double xpos=0.0;
    double ypos=0.0;
    bool firstMouse = true;
    std::unordered_map<std::string,GLFWcursor*> cursors;

    Cursor_win();
    void set_cursor(GLFWwindow* window,std::string type);
};
} // namespace Win_layer
#endif // CURSOR_WIN_H
