#ifndef INCOMING_SIGNALS_FROM_USER_H
#define INCOMING_SIGNALS_FROM_USER_H
#include <filesystem>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "Cursor_win.h"


class GLFWwindow;
namespace Win_layer {
struct Setting_key{
   int key;
   int scancode;
   int action;
   int mode;
};
class Incoming_signals_from_user{
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void character_callback(GLFWwindow* window, unsigned int codepoint);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void drop_callback(GLFWwindow* window, int count, const char** paths);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
public:

    template<typename T>
    struct Cord{
        T x=0;
        T y=0;
    };


    std::map<int,Setting_key> keys;
    unsigned int input_code_text;

    Cursor_win setting_curs;
    std::vector<std::string> path_drop{};
    //синхронизация времени и частоты кадров
    double deltaTime = 0.0;
    double lastFrame = 0.0;

    int width=0;
    int height=0;
    bool isChangeSize=true;//при создание окна можно считать что оно изменила свои размеры)

    double yoffset=0.0;

    std::size_t id_detector=0;//для определение id под мышкой...или иного
    void  new_frame();
    Incoming_signals_from_user(GLFWwindow *window);


    inline Cord<float> move_to_cursor(){
        float x=(static_cast<int>(setting_curs.xpos+0.5)-static_cast<int>(setting_curs.lastX+0.5))*2/static_cast<float>(width);
        float y=(static_cast<int>(setting_curs.ypos+0.5)-static_cast<int>(setting_curs.lastY+0.5))*2/static_cast<float>(height);
        //std::cout<<x<<":"<<*inf.x_cursor<<"|"<<*inf.last_x_cursor<<std::endl;

        return Cord<float>{x,-y};
    }

    inline Cord<float> cursor_position(){
        float x=(((-width+static_cast<int>(setting_curs.xpos)*2)/static_cast<float>(width)));
        float y=(((height-static_cast<int>(setting_curs.ypos)*2)/static_cast<float>(height)));

        return Cord<float>{x,y};
    }

    inline Cord<int> otn_to_absolute_position(float x,float y){
        std::cout<<width<<"|"<<height<<std::endl;
        int x_a=static_cast<int>(width*(x+1)/2.0f);
        int y_a=static_cast<int>(height*(y+1)/2.0f);

        return Cord<int>{x_a,y_a};
    }

    inline Cord<int> size_otn_to_absolute_position(float size_x,float size_y){
        int x_a=static_cast<int>(width*size_x/2.0f);
        int y_a=static_cast<int>(height*size_y/2.0f);

        return Cord<int>{x_a,y_a};
    }

    inline int get_point_cursor_left_down_riding(){
        int point=(height-static_cast<int>(setting_curs.ypos+0.5))*width+static_cast<int>(setting_curs.xpos+0.5);
        if(point<0){
            return 0;
        }else {
            return point;
        }
    }
};
}
#endif // INCOMING_SIGNALS_FROM_USER_H
