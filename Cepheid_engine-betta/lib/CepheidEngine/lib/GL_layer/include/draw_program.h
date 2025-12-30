#ifndef DRAW_PROGRAM_H
#define DRAW_PROGRAM_H
#include "GL_type_to_cpp.h"
namespace GL_layer {

struct DrawArraysIndirectCommand{
    GLuint  count=6;
    GLuint  primCount=1;
    GLuint  first=0;
    GLuint  baseInstance=0;
} ;

struct DrawElementsIndirectCommand{
    GLuint  count=6;
    GLuint  instanceCount=1;
    GLuint  firstIndex=0;
    GLuint  baseVertex=0;
    GLuint  baseInstance=0;
};

class DrawElementsIndirectCommand_menedger{
public:
    DrawElementsIndirectCommand cmd;
    std::size_t id=0;
};

class DrawArraysIndirectCommand_menedger{
public:
    DrawArraysIndirectCommand cmd;
    std::size_t id=0;
};

template <int type_draw,typename T>
void draw_fun(int type_object,int type_byte,const T * command, int count);


template<TYPE_OBJECT_DRAW type_object,int type_draw,typename T>
inline void Draw(const T * command, int count=1,int type_byte=get_value_GL<SHEME_TYPE_GL::UNSIGNED_INT>()){
    constexpr int t0=type_draw & TYPE_RENDERING_ARRAY      ? type_draw^TYPE_RENDERING_ARRAY       :type_draw;
    constexpr int t1=t0        & TYPE_RENDERING_ELEMENT    ? t0       ^TYPE_RENDERING_ELEMENT     :t0;
    constexpr int t2=t1        & TYPE_RENDERING_BIND_BUFFER? t1       ^TYPE_RENDERING_BIND_BUFFER :t1;

    draw_fun<t2>(get_value_GL<type_object>(),type_byte,command,count);
}

}// namespace GL_layer
#endif // DRAW_PROGRAM_H
