#include "draw_program.h"
#include <GL/glew.h>
#include <iostream>

namespace GL_layer {


template <>  inline void draw_fun<TYPE_RENDERING_INSTANSE>(int type_object,int type_byte,const DrawArraysIndirectCommand * command, int count){
    glDrawArraysInstanced(type_object,command[0].first,command[0].count,command[0].primCount);
}

template <> void draw_fun<TYPE_RENDERING_INSTANSE | TYPE_RENDERING_BASE_INSTANSE>(int type_object,int type_byte,const DrawArraysIndirectCommand * command, int count){
    glDrawArraysInstancedBaseInstance(type_object,command[0].first,command[0].count,command[0].primCount,command[0].baseInstance);
}

template <> void draw_fun<TYPE_RENDERING_INDERECT>(int type_object,int type_byte,const DrawArraysIndirectCommand * command, int count){
    glDrawArraysIndirect(type_object,command);
}
template <> void draw_fun<TYPE_RENDERING_INDERECT | TYPE_RENDERING_MYLTI>(int type_object,int type_byte,const DrawArraysIndirectCommand * command, int count){
    glMultiDrawArraysIndirect(type_object,command,count,0);
}

template <> void draw_fun<TYPE_RENDERING_MYLTI>(int type_object,int type_byte,const DrawArraysIndirectCommand * command, int count){
    //glMultiDrawArraysBaseVertex(type_object);
}
template <> void draw_fun<TYPE_RENDERING_MYLTI | TYPE_RENDERING_BASE_VERTEX>(int type_object,int type_byte,const DrawArraysIndirectCommand * command, int count){
    // glMultiDrawArrays(type_object);
}

template <> void draw_fun<TYPE_RENDERING_BASE_VERTEX>(int type_object,int type_byte,const DrawArraysIndirectCommand * command, int count){
    // glDrawArraysBaseVertex(type_object,command.first,command.count,command.instanceCount);
}

template <> void draw_fun<0>(int type_object,int type_byte,const DrawArraysIndirectCommand * command, int count){
    glDrawArrays(type_object,command[0].first,command[0].count);
}


template <> void draw_fun<TYPE_RENDERING_INSTANSE>(int type_object,int type_byte,const DrawElementsIndirectCommand * command, int count){
    glDrawElementsInstanced(type_object,command[0].count,type_byte,nullptr,command[0].instanceCount);
}
template <> void draw_fun<TYPE_RENDERING_INSTANSE | TYPE_RENDERING_BASE_INSTANSE>(int type_object,int type_byte,const DrawElementsIndirectCommand * command, int count){
    glDrawElementsInstancedBaseInstance(type_object,command[0].count,type_byte,nullptr,command[0].instanceCount,command[0].baseInstance);
}
template <> void draw_fun<TYPE_RENDERING_INSTANSE | TYPE_RENDERING_BASE_VERTEX>(int type_object,int type_byte,const DrawElementsIndirectCommand * command, int count){
    glDrawElementsInstancedBaseVertex(type_object,command[0].count,type_byte,nullptr,command[0].instanceCount,command[0].baseVertex);
}
template <> void draw_fun<TYPE_RENDERING_INSTANSE| TYPE_RENDERING_BASE_VERTEX | TYPE_RENDERING_BASE_INSTANSE>(int type_object,int type_byte,const DrawElementsIndirectCommand * command, int count){
    glDrawElementsInstancedBaseVertexBaseInstance(type_object,command[0].count,type_byte,nullptr,command[0].instanceCount,command[0].baseVertex,command[0].baseInstance);
}

template <> void draw_fun<TYPE_RENDERING_INDERECT>(int type_object,int type_byte,const DrawElementsIndirectCommand * command, int count){
    glDrawElementsIndirect(type_object,type_byte,command);
}
template <> void draw_fun<TYPE_RENDERING_MYLTI | TYPE_RENDERING_INDERECT>(int type_object,int type_byte,const DrawElementsIndirectCommand * command, int count){
    glMultiDrawElementsIndirect(type_object,type_byte,command,count,0);
}
template <> void draw_fun<TYPE_RENDERING_MYLTI | TYPE_RENDERING_BASE_VERTEX>(int type_object,int type_byte,const DrawElementsIndirectCommand * command, int count){
    //glMultiDrawElements(type_object, ,type_byte,nullptr,count,command[0].baseVertex);
}
template <> void draw_fun<TYPE_RENDERING_MYLTI >(int type_object,int type_byte,const DrawElementsIndirectCommand * command, int count){
    //glMultiDrawElements(type_object, ,type_byte,nullptr,count);
}
template <> void draw_fun<TYPE_RENDERING_BASE_VERTEX >(int type_object,int type_byte,const DrawElementsIndirectCommand * command, int count){
    glDrawElementsBaseVertex(type_object,command[0].count,type_byte,nullptr,command[0].baseVertex);
}
template <> void draw_fun<0>(int type_object,int type_byte,const DrawElementsIndirectCommand * command, int count){
    glDrawElements(type_object,command[0].count,type_byte,nullptr);
}
}// namespace GL_layer
