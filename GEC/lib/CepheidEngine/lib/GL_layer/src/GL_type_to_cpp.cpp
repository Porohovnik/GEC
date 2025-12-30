#include "GL_type_to_cpp.h"
#include<GL/glew.h>
namespace GL_layer {


template <> int get_value_GL<SHEME_TYPE_GL::UNSIGNED_BYTE>(){return GL_UNSIGNED_BYTE;};
template <> int get_value_GL<SHEME_TYPE_GL::UNSIGNED_INT>(){return GL_UNSIGNED_INT;};

template <> int get_value_GL<SHEME_COLOR_GL::RGB>(){return GL_RGB;};
template <> int get_value_GL<SHEME_COLOR_GL::RGBA>(){return GL_RGBA;};
template <> int get_value_GL<SHEME_COLOR_GL::RED>(){return GL_RED;};
int get_value_GL(SHEME_COLOR_GL type_object){
    switch (type_object) {
        case SHEME_COLOR_GL::RGB  : return get_value_GL<SHEME_COLOR_GL::RGB>();
        case SHEME_COLOR_GL::RGBA : return get_value_GL<SHEME_COLOR_GL::RGBA>();
        case SHEME_COLOR_GL::RED  : return get_value_GL<SHEME_COLOR_GL::RED>();
    }
    return 0;
}

template <> int get_value_GL<TYPE_SETTING_CLEAR::COLOR_BUFFER_BIT>(){  return  GL_COLOR_BUFFER_BIT;};
template <> int get_value_GL<TYPE_SETTING_CLEAR::DEPTH_BUFFER_BIT>(){  return  GL_DEPTH_BUFFER_BIT;};
template <> int get_value_GL<TYPE_SETTING_CLEAR::STENCIL_BUFFER_BIT>(){return  GL_STENCIL_BUFFER_BIT;};

template <> int get_value_GL<TYPE_SETTING_DEPTH_FUN_BLEND::SRC_ALPHA>(){ return  GL_SRC_ALPHA;}
template <> int get_value_GL<TYPE_SETTING_DEPTH_FUN_BLEND::ONE_MINUS_SRC_ALPHA>(){ return  GL_ONE_MINUS_SRC_ALPHA;}
template <> int get_value_GL<TYPE_SETTING_DEPTH_FUN_BLEND::ONE>(){ return  GL_ONE;}
template <> int get_value_GL<TYPE_SETTING_DEPTH_FUN_BLEND::ZERO>(){ return  GL_ZERO;}


template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_OBJECT_POINTS>(){ return GL_POINTS;}
template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_OBJECT_LINES>(){ return GL_LINES;}
template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_OBJECT_LINE_STRIP>(){ return GL_LINE_STRIP;}
template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_OBJECT_LINE_LOOP>(){ return GL_LINE_LOOP;}

template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_LINES_ADJACENCY>(){ return GL_LINES_ADJACENCY;}
template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_LINE_STRIP_ADJACENCY>(){ return GL_LINE_STRIP_ADJACENCY;}


template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_OBJECT_TRIANGLES>(){ return GL_TRIANGLES;}
template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_OBJECT_TRIANGLE_STRIP>(){ return GL_TRIANGLE_STRIP;}
template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_OBJECT_TRIANGLE_FAN>(){ return GL_TRIANGLE_FAN;}
template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_OBJECT_QUADS>(){ return GL_QUADS;}
template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_OBJECT_QUAD_STRIP>(){ return GL_QUAD_STRIP;}
template <> int get_value_GL<TYPE_OBJECT_DRAW::TYPE_OBJECT_POLYGON>(){ return GL_POLYGON;}



template <> int get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::CLAMP_TO_EDGE>(){return  GL_CLAMP_TO_EDGE;};
template <> int get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::CLAMP_TO_BORDER>(){return  GL_CLAMP_TO_BORDER;};
template <> int get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::MIRRORED_REPEAT>(){return  GL_MIRRORED_REPEAT;};
template <> int get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::REPEAT>(){return  GL_REPEAT;};
template <> int get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::MIRROR_CLAMP_TO_EDGE>(){return  GL_MIRROR_CLAMP_TO_EDGE;};
template <> int get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::NEAREST>(){return  GL_NEAREST;};
template <> int get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::LINEAR>(){return  GL_LINEAR;};


int get_value_GL(TYPE_ARGUMENT_SETTING_TEXTURE type_object){
    switch (type_object) {
        case TYPE_ARGUMENT_SETTING_TEXTURE::CLAMP_TO_EDGE: return get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::CLAMP_TO_EDGE>();
        case TYPE_ARGUMENT_SETTING_TEXTURE::CLAMP_TO_BORDER : return get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::CLAMP_TO_BORDER>();
        case TYPE_ARGUMENT_SETTING_TEXTURE::MIRRORED_REPEAT  : return get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::MIRRORED_REPEAT>();
        case TYPE_ARGUMENT_SETTING_TEXTURE::REPEAT  : return get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::REPEAT>();
        case TYPE_ARGUMENT_SETTING_TEXTURE::MIRROR_CLAMP_TO_EDGE  : return get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::MIRROR_CLAMP_TO_EDGE>();
        case TYPE_ARGUMENT_SETTING_TEXTURE::NEAREST  : return get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::NEAREST>();
        case TYPE_ARGUMENT_SETTING_TEXTURE::LINEAR  : return get_value_GL<TYPE_ARGUMENT_SETTING_TEXTURE::LINEAR>();
    }
    return 0;
}

template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_WRAP_R>(){ return  GL_TEXTURE_WRAP_R;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_WRAP_T>(){ return  GL_TEXTURE_WRAP_T;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_WRAP_S>(){ return  GL_TEXTURE_WRAP_S;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_SWIZZLE_RGBA>(){ return  GL_TEXTURE_SWIZZLE_RGBA;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_SWIZZLE_A>(){ return  GL_TEXTURE_SWIZZLE_A;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_SWIZZLE_B>(){ return  GL_TEXTURE_SWIZZLE_B;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_SWIZZLE_G>(){ return  GL_TEXTURE_SWIZZLE_G;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_SWIZZLE_R>(){ return  GL_TEXTURE_SWIZZLE_R;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_MAX_LEVEL>(){ return  GL_TEXTURE_MAX_LEVEL;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_MAX_LOD>(){ return  GL_TEXTURE_MAX_LOD;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_MIN_LOD>(){ return  GL_TEXTURE_MIN_LOD;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_MAG_FILTER>(){ return  GL_TEXTURE_MAG_FILTER;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_MIN_FILTER>(){ return  GL_TEXTURE_MIN_FILTER;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_LOD_BIAS>(){ return  GL_TEXTURE_LOD_BIAS;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_COMPARE_MODE>(){ return  GL_TEXTURE_COMPARE_MODE;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_COMPARE_FUNC>(){ return  GL_TEXTURE_COMPARE_FUNC;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_BORDER_COLOR>(){ return  GL_TEXTURE_BORDER_COLOR;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::TEXTURE_BASE_LEVEL>(){ return  GL_TEXTURE_BASE_LEVEL;};
template <> int get_value_GL<TYPE_SETTING_TEXTURE::DEPTH_STENCIL_TEXTURE_MODE>(){ return  GL_DEPTH_STENCIL_TEXTURE_MODE;};


template <> int get_value_GL<TYPE_SHADER::VERTEX_SHADER>(){return GL_VERTEX_SHADER;};
template <> int get_value_GL<TYPE_SHADER::FRAGMENT_SHADER>(){return GL_FRAGMENT_SHADER;};
template <> int get_value_GL<TYPE_SHADER::GEOMETRY_SHADER>(){return GL_GEOMETRY_SHADER;};
template <> int get_value_GL<TYPE_SHADER::TESS_CONTROL_SHADER>(){return GL_TESS_CONTROL_SHADER;};
template <> int get_value_GL<TYPE_SHADER::TESS_EVALUATION_SHADER>(){return GL_TESS_EVALUATION_SHADER;};
template <> int get_value_GL<TYPE_SHADER::COMPUTE_SHADER>(){return GL_COMPUTE_SHADER;};


template <> int get_value_GL<BUFFER_SETTING::STATIC>(){return GL_STATIC_DRAW;};
template <> int get_value_GL<BUFFER_SETTING::DYNAMIC>(){return GL_DYNAMIC_DRAW;};
template <> int get_value_GL<BUFFER_SETTING::STREAM>(){return GL_STREAM_DRAW;};

int get_value_GL(BUFFER_SETTING type_object){
    switch (type_object) {
        case BUFFER_SETTING::STATIC  : return get_value_GL<BUFFER_SETTING::STATIC>();
        case BUFFER_SETTING::DYNAMIC : return get_value_GL<BUFFER_SETTING::DYNAMIC>();
        case BUFFER_SETTING::STREAM  : return get_value_GL<BUFFER_SETTING::STREAM>();
    }
    return 0;
}


template <> int get_value_GL<TYPE_TEXTURE::TEXTURE_1D>(){return GL_TEXTURE_1D;};
template <> int get_value_GL<TYPE_TEXTURE::TEXTURE_2D>(){return GL_TEXTURE_2D;};
template <> int get_value_GL<TYPE_TEXTURE::TEXTURE_3D>(){return GL_TEXTURE_3D;};
template <> int get_value_GL<TYPE_TEXTURE::TEXTURE_2D_ARRAY>(){return GL_TEXTURE_2D_ARRAY;};
template <> int get_value_GL<TYPE_TEXTURE::TEXTURE_CUBE_MAP>(){return GL_TEXTURE_CUBE_MAP;};


int get_value_GL(TYPE_TEXTURE type_object){
    switch (type_object) {
        case TYPE_TEXTURE::TEXTURE_1D: return get_value_GL<TYPE_TEXTURE::TEXTURE_1D>();
        case TYPE_TEXTURE::TEXTURE_2D : return get_value_GL<TYPE_TEXTURE::TEXTURE_2D>();
        case TYPE_TEXTURE::TEXTURE_3D  : return get_value_GL<TYPE_TEXTURE::TEXTURE_3D>();
        case TYPE_TEXTURE::TEXTURE_2D_ARRAY  : return get_value_GL<TYPE_TEXTURE::TEXTURE_2D_ARRAY>();
        case TYPE_TEXTURE::TEXTURE_CUBE_MAP  : return get_value_GL<TYPE_TEXTURE::TEXTURE_CUBE_MAP>();
    }
    return 0;
}

template <> int get_value_GL<TYPE_BUFFER ::NULL_>(){return 0;};
template <> int get_value_GL<TYPE_BUFFER ::VERTEX_ARRAY>(){return GL_VERTEX_ARRAY;};
template <> int get_value_GL<TYPE_BUFFER ::ARRAY_BUFFER>(){return GL_ARRAY_BUFFER;};
template <> int get_value_GL<TYPE_BUFFER ::ELEMENT_ARRAY_BUFFER>(){return GL_ELEMENT_ARRAY_BUFFER;};
template <> int get_value_GL<TYPE_BUFFER ::DRAW_INDIRECT_BUFFER>(){return GL_DRAW_INDIRECT_BUFFER;};
template <> int get_value_GL<TYPE_BUFFER ::COPY_READ_BUFFER>(){return GL_COPY_READ_BUFFER;};
template <> int get_value_GL<TYPE_BUFFER ::COPY_WRITE_BUFFER>(){return GL_COPY_WRITE_BUFFER;};
template <> int get_value_GL<TYPE_BUFFER ::SHADER_STORAGE_BUFFER>(){return GL_SHADER_STORAGE_BUFFER;};
template <> int get_value_GL<TYPE_BUFFER ::FRAMEBUFFER>(){return GL_FRAMEBUFFER;};
template <> int get_value_GL<TYPE_BUFFER ::RENDERBUFFER>(){return GL_RENDERBUFFER;};



template <> int get_value_GL<TYPE_MAP_ACCESS::READ_ONLY>(){return GL_READ_ONLY;};
template <> int get_value_GL<TYPE_MAP_ACCESS::WRITE_ONLY>(){return GL_WRITE_ONLY;};
template <> int get_value_GL<TYPE_MAP_ACCESS::READ_WRITE>(){return GL_READ_WRITE;};

} // namespace GL_layer
