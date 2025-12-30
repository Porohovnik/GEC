#include "Mesh_info.h"
#include <GL/glew.h>
namespace GL_layer  {
void Mesh_info::update_offset(){
    offset=0;
    for(auto &T:elements_cout){
        offset+=T;
    }
    for(auto &T:tex_cord_cout){
        offset+=T;
    }
}

bool Mesh_info::operator==(Mesh_info & info){
    std::size_t sum=0;
    for(uint i=0;i<COUT_TYPE;i++){
       sum+= this->elements_cout[i]==info.elements_cout[i];
    }

    for(uint i=0;i<COUT_TYPE_TEX_COORDS;i++){
        sum+= this->tex_cord_cout[i]==info.tex_cord_cout[i];
    }

    if(sum!=(COUT_TYPE+COUT_TYPE_TEX_COORDS)){
        return false;
    }
    return true;
}

void Mesh_info::set_atribute_Mesh_buffer() const{
    uint cout_offset=0;

    for(uint i=0;i<COUT_TYPE;i++){
        if(elements_cout[i]){
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i,elements_cout[i], GL_FLOAT, GL_FALSE, offset*sizeof(float),reinterpret_cast<void*>(cout_offset*sizeof(float)));
            cout_offset+=elements_cout[i];
        }
    }

    for(uint i=0;i<COUT_TYPE_TEX_COORDS;i++){
        if(tex_cord_cout[i]){
            glEnableVertexAttribArray(i+COUT_TYPE);
            glVertexAttribPointer(i+COUT_TYPE,tex_cord_cout[i], GL_FLOAT, GL_FALSE, offset*sizeof(float),reinterpret_cast<void*>(cout_offset*sizeof(float)));
            cout_offset+=tex_cord_cout[i];
        }
    }
    //std::cout<<elements_cout[0]<<"|"<<tex_cord_cout[0]<<" cout_offset : "<<cout_offset<<std::endl;
}
}// namespace GL_layer
