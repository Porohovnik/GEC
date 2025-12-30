#include "Cursor_win.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
namespace Win_layer{
Cursor_win::Cursor_win(){
    cursors[std::string{"STANDART_ARROW"}]=glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    cursors[std::string{"STANDART_IBEAM"}]=glfwCreateStandardCursor(GLFW_IBEAM_CURSOR  );
    cursors[std::string{"STANDART_CROSSHAIR"}]=glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR );
    cursors[std::string{"STANDART_HAND"}]=glfwCreateStandardCursor(GLFW_HAND_CURSOR  );
    cursors[std::string{"STANDART_HRESIZE"}]=glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR );
    cursors[std::string{"STANDART_CURSOR"}]=glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
}

void Cursor_win::set_cursor(GLFWwindow* window,std::string type){
    glfwSetCursor(window, cursors[type]);
}
}// namespace Win_layer
