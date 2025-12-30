#include "Buffer_id_texture.h"
#include "tuple_utl.h"
#include <GL/glew.h>
#include "type_traits"

namespace GL_layer{
GLenum Buffer_id_texture::create_texture_buffer(){
    GLuint texture=0;
    glGenTextures(1,&texture);
    return texture;
}

uint64_t Buffer_id_texture::Texture_handles::create_Handle(GLenum tex){
    return glGetTextureHandleARB(tex);
}

void Buffer_id_texture::Texture_handles::Make_Resident(){
    if(!isResident){
        glMakeTextureHandleResidentARB(id_texture);
        isResident=true;
    }
}
void Buffer_id_texture::Texture_handles::Make_No_Resident(){
    if(isResident){
        glMakeTextureHandleNonResidentARB(id_texture);
        isResident=false;
    }
}


Buffer_id_texture::~Buffer_id_texture(){
    glDeleteTextures(1,&tex);
}


void Buffer_id_texture::Bind_() const noexcept{
    if(curent_Bind!=tex){
        glBindTexture(POINT_TEX, tex);
    }
    curent_Bind=tex;
}

void Buffer_id_texture::active_texture(int base) const{
    glActiveTexture(static_cast<unsigned int>(GL_TEXTURE0+base));
}

void Buffer_id_texture::generate_mipmap(){
    Bind_();
    glGenerateMipmap(POINT_TEX);
}

void Buffer_id_texture::set_atribute_texture_buffer(Type_filter_atribute_texture filter_atribute){
    Bind_();
    tutl::TupleForeach(filter_atribute.get_data() ,[this](auto t){
        if(t.data.has_value()){
            if constexpr(std::is_same_v<std::remove_reference_t<decltype (*t.data )>,int>){
                glTexParameteri(POINT_TEX,get_value_GL<decltype (t)::type>(),*t.data );
            }else {
                glTexParameteri(POINT_TEX,get_value_GL<decltype (t)::type>(),get_value_GL(*t.data));
            }

        }
    });
}

void Buffer_id_texture::TexImage2D(GLsizei width,GLsizei height,GLenum type_scheme_pixel,void *pixels,int level,int board){
    glTexImage2D(POINT_TEX, level,type_scheme_pixel , width, height, board,type_scheme_pixel, GL_UNSIGNED_BYTE, pixels);
}

}// namespace GL_layer
