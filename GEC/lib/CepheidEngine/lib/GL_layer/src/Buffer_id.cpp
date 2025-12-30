#include "Buffer_id.h"
#include <GL/glew.h>
namespace GL_layer {

template<TYPE_BUFFER n>
Buffer_id<n>::Buffer_id(){
    if constexpr(n==TYPE_BUFFER::SHADER_STORAGE_BUFFER ||
                 n==TYPE_BUFFER::DRAW_INDIRECT_BUFFER  ||
                 n==TYPE_BUFFER::COPY_READ_BUFFER     ||
                 n==TYPE_BUFFER::COPY_WRITE_BUFFER    ||
                 n==TYPE_BUFFER::ARRAY_BUFFER          ||
                 n==TYPE_BUFFER::ELEMENT_ARRAY_BUFFER){
        glGenBuffers(1, &buffer);
    }
    if constexpr(n==TYPE_BUFFER::VERTEX_ARRAY){
        glGenVertexArrays(1,&buffer);
    }
    if constexpr(n==TYPE_BUFFER::FRAMEBUFFER){
        glGenFramebuffers(1,&buffer);
    }
    if constexpr(n==TYPE_BUFFER::RENDERBUFFER){
        glGenRenderbuffers(1,&buffer);
    }
}

template<TYPE_BUFFER n>
Buffer_id<n>::~Buffer_id(){
    if constexpr(n==TYPE_BUFFER::SHADER_STORAGE_BUFFER ||
                 n==TYPE_BUFFER::DRAW_INDIRECT_BUFFER  ||
                 n==TYPE_BUFFER::COPY_READ_BUFFER     ||
                 n==TYPE_BUFFER::COPY_WRITE_BUFFER    ||
                 n==TYPE_BUFFER::ARRAY_BUFFER          ||
                 n==TYPE_BUFFER::ELEMENT_ARRAY_BUFFER){
        glDeleteBuffers(1, &buffer);
     //std::cout<<glGetError()<<"/-delete-/"<<"///::"<<buffer<<std::endl;
    }
    if constexpr(n==TYPE_BUFFER::VERTEX_ARRAY){
        glDeleteVertexArrays(1,&buffer);
    }
    if constexpr(n==TYPE_BUFFER::FRAMEBUFFER){
        glDeleteFramebuffers(1,&buffer);
    }
    if constexpr(n==TYPE_BUFFER::RENDERBUFFER){
        glDeleteRenderbuffers(1,&buffer);
    }
}

template<TYPE_BUFFER n>
void Buffer_id<n>::BindBase(GLuint bind_base) const noexcept{
    if constexpr(n==TYPE_BUFFER::SHADER_STORAGE_BUFFER){
        glBindBufferBase(get_value_GL<n>(), bind_base, buffer);
    }
}

template<TYPE_BUFFER n>
void Buffer_id<n>::clear_buffer_noBind(){
    Bind_();
    glClearBufferData(get_value_GL<n>(),GL_R32UI,GL_RED_INTEGER,GL_UNSIGNED_INT,nullptr);
}


template<TYPE_BUFFER n>
void Buffer_id<n>::initialization_buffer_noBind(GLsizeiptr size_bate,const void * data,GL_layer::BUFFER_SETTING type_update){
    Bind_();
    if constexpr(n==TYPE_BUFFER::SHADER_STORAGE_BUFFER ||
                 n==TYPE_BUFFER::DRAW_INDIRECT_BUFFER  ||
                 n==TYPE_BUFFER::COPY_READ_BUFFER     ||
                 n==TYPE_BUFFER::COPY_WRITE_BUFFER    ||
                 n==TYPE_BUFFER::ARRAY_BUFFER          ||
                 n==TYPE_BUFFER::ELEMENT_ARRAY_BUFFER){
        glBufferData(get_value_GL<n>(),size_bate,data, get_value_GL(type_update));
    }
}
template<TYPE_BUFFER n>
void Buffer_id<n>::update_buffer_noBind(GLintptr begin,GLintptr end,GLsizeiptr size_byte,const void * data,GLuint offset){
    Bind_();
    glBufferSubData(get_value_GL<n>(),offset+(size_byte)*begin,offset+(size_byte)*(end-begin),data);
}

template<TYPE_BUFFER n>
void Buffer_id<n>::read_buffer_noBind(GLintptr begin,GLintptr end, GLsizeiptr size_byte,void * data) const{
    Bind_();
    glGetBufferSubData(get_value_GL<n>(),begin*size_byte,(end-begin)*size_byte,data);
}


template  class Buffer_id<TYPE_BUFFER::NULL_>;
template  class Buffer_id<TYPE_BUFFER::VERTEX_ARRAY>;
template  class Buffer_id<TYPE_BUFFER::ARRAY_BUFFER>;
template  class Buffer_id<TYPE_BUFFER::ELEMENT_ARRAY_BUFFER>;
template  class Buffer_id<TYPE_BUFFER::DRAW_INDIRECT_BUFFER>;
template  class Buffer_id<TYPE_BUFFER::SHADER_STORAGE_BUFFER>;

} // namespace GL_layer
