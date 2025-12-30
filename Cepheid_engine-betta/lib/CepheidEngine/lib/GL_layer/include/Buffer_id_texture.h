#ifndef BUFFER_ID_TEXTURE_H
#define BUFFER_ID_TEXTURE_H
#include "GL_type_to_cpp.h"
#include "Atribute_texture.h"

namespace GL_layer{
class Buffer_id_texture{
    inline static GLenum curent_Bind=0;
    GLenum tex=0;
    GLenum POINT_TEX=0;

    GLenum create_texture_buffer();
public:
    class Texture_handles{
        uint64_t id_texture=0;
        bool isResident=false;

        uint64_t create_Handle(GLenum tex);

    public:
        Texture_handles(GLenum tex):id_texture(create_Handle(tex)){
            Make_Resident();
        }

        uint64_t get_id() const{
            return id_texture;
        }

        void Make_Resident();
        void Make_No_Resident();

        ~Texture_handles(){
            Make_No_Resident();
        }
    };


    Buffer_id_texture(const Buffer_id_texture& )=delete;
    Buffer_id_texture(Buffer_id_texture&& )=delete;
    Buffer_id_texture & operator=(Buffer_id_texture && buf)=delete;
    Buffer_id_texture & operator=(const Buffer_id_texture &)=delete;

    Buffer_id_texture(GLenum POINT_TEX_):tex(create_texture_buffer()),POINT_TEX(POINT_TEX_){}
    ~Buffer_id_texture();

    Texture_handles get_handles(){
        return Texture_handles(tex);
    }

    void Bind_() const noexcept;

    void active_texture(int base=0) const;
    void generate_mipmap();
    void set_atribute_texture_buffer(Type_filter_atribute_texture filter_atribute);

    void TexImage2D(GLsizei width,GLsizei height,GLenum type_scheme_pixel,void *pixels,int level=0,int board=0);

    template<typename Image>
    void TexImage2D(Image & image, int level=0,int board=0){
        Bind_();
        TexImage2D(image.get_width(),image.get_height(),GL_layer::get_value_GL(image.get_setting().type_scheme_pixel),image.get_data()->get(),level,board);
    }

    template<typename Image>
    void TexImage2D_cube(Image & image, int level=0,int board=0){
        Bind_();
        for (std::size_t i=0;i<6;i++) {
            std::size_t offset=i*image.get_width()*image.get_height()*image.get_pixel_size();
            TexImage2D(image.get_width(),image.get_height(),GL_layer::get_value_GL(image.get_setting().type_scheme_pixel),image.get_data()->get()+offset,level,board);
        }
    }
};
}// namespace GL_layer

#endif // BUFFER_ID_TEXTURE_H
