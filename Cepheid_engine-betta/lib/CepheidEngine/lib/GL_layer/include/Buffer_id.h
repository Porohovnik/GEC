#ifndef BUFFER_ID_H
#define BUFFER_ID_H
#include "GL_type_to_cpp.h"
#include "interlayer_gl.h"

namespace GL_layer {
template<TYPE_BUFFER n>
class Buffer_id{
    template<typename T>
    class Map_buffer_guard{
        void * ptr=nullptr;
    public:
        template<TYPE_MAP_ACCESS type>
        inline explicit Map_buffer_guard()noexcept :ptr(MapBuffer(get_value_GL<n>(), get_value_GL<type>())) {}

        inline ~Map_buffer_guard() noexcept{
            UnmapBuffer(get_value_GL<n>());
        }

        inline T * data() const{
            return static_cast<T*>(ptr);
        }

        inline T& operator [](GLsizeiptr index){
            return static_cast<T*>(ptr)[index];
        }

        Map_buffer_guard(const Map_buffer_guard&) = delete;
        Map_buffer_guard& operator=(const Map_buffer_guard&) = delete;
    };

    GLenum buffer=0;
public:
    Buffer_id(const Buffer_id& )=delete;
    Buffer_id(Buffer_id&& )=delete;
    Buffer_id & operator=(Buffer_id && buf)=delete;
    Buffer_id & operator=(const Buffer_id &)=delete;

    Buffer_id();

    Buffer_id(GLsizeiptr size_bate,const void * data,GL_layer::BUFFER_SETTING type_update=GL_layer::BUFFER_SETTING::STATIC):Buffer_id(){
        initialization_buffer_noBind(size_bate,data,type_update);
    }

    ~Buffer_id();

    template<TYPE_BUFFER n_any>
    inline void Bind_set_point() const noexcept{
        Bind<n_any>(buffer);
    }

    inline void Bind_()  const noexcept{
        Bind<n>(buffer);//пока так
    }

    void BindBase(GLuint bind_base) const noexcept;

    template<typename T>
    auto && get_map_guard(GLenum type){
        Bind_();
        return Map_buffer_guard<T>(type);
    }
    void clear_buffer_noBind();
    //void clear_buffer_noBind(const void *data=nullptr,GLenum internalformat=GL_R32UI, GLenum format=GL_RED_INTEGER, GLenum type=GL_UNSIGNED_INT);
    void initialization_buffer_noBind(GLsizeiptr size_bate,const void * data,GL_layer::BUFFER_SETTING type_update=GL_layer::BUFFER_SETTING::STATIC);
    void update_buffer_noBind(GLintptr begin,GLintptr end,GLsizeiptr size_byte,const void * data,GLuint offset=0);
    void read_buffer_noBind(GLintptr begin,GLintptr end, GLsizeiptr size_byte,void * data) const;


    template<TYPE_BUFFER n2>
    inline void swap(Buffer_id<n2> &b2){
        GLenum s=b2.buffer;
        b2.buffer=buffer;
        buffer=s;
    }
};

template<TYPE_BUFFER readtarget,TYPE_BUFFER writetarget>
inline void copy_buffer( GLsizeiptr size_byte,GLintptr read_offset=0, GLintptr write_offset=0){
    copy_buffer_(get_value_GL<readtarget>(), get_value_GL<writetarget>(), size_byte, read_offset, write_offset);
}
}
#endif // BUFFER_ID_H
