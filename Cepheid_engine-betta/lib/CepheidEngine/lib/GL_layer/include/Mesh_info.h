#ifndef MESH_INFO_H
#define MESH_INFO_H
#include<array>

using uint =unsigned int;
namespace GL_layer{
const uint COUT_TYPE=4;
const uint COUT_TYPE_TEX_COORDS=8;
enum class TYPE_data:std::size_t{position=0,normal=1,tangent=2,bitangent=3,tex_coords=4,color=12};

class Mesh_info{
    std::array<uint,COUT_TYPE>            elements_cout{0};
    std::array<uint,COUT_TYPE_TEX_COORDS> tex_cord_cout{0};
    uint offset=0;

    void update_offset();
public:
    bool operator==(Mesh_info & info);
    inline uint get_offset() const{
        return offset;
    }

    template<TYPE_data type>
    constexpr inline  uint get() const{
        return elements_cout[static_cast<std::size_t>(type)];
    }

    inline  uint get(TYPE_data type) const{
        return elements_cout[static_cast<std::size_t>(type)];
    }

    template<std::size_t type_texture>
    constexpr inline  uint get_texture() const{
        return tex_cord_cout[type_texture];
    }

    inline  uint get_texture(std::size_t type_texture) const{
        return tex_cord_cout[type_texture];
    }

    template<TYPE_data type>
    void set(uint cout){
        elements_cout[static_cast<std::size_t>(type)]=cout;
        update_offset();
    }

    template<std::size_t type_texture>
    void set_texture(uint cout){
        tex_cord_cout[type_texture]=cout;
        update_offset();
    }

    void set_atribute_Mesh_buffer() const;
};

}// namespace CeEngine
#endif // MESH_INFO_H
