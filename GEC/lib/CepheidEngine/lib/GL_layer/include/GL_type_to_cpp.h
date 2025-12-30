#ifndef GL_TYPE_TO_CPP_H
#define GL_TYPE_TO_CPP_H
#include <utility>
namespace GL_layer {
using   GLboolean = bool;
using   GLubyte = unsigned char;
using   GLint=   int;
using   GLsizei= int;

using   GLuint =     unsigned int;
using   GLenum =     unsigned int;
using   GLbitfield = unsigned int;

using   GLfloat  = float;
using   GLclampf = float;

using   GLdouble= double;

using GLsizeiptr=std::ptrdiff_t;
using GLintptr=std::ptrdiff_t;

enum class TYPE_POLYGON_MOD:int{
    POINT,
    LINE,
    FILL
};

enum class SHEME_TYPE_GL:int{
    UNSIGNED_BYTE,
    UNSIGNED_INT
};


template <SHEME_TYPE_GL type_object>
int get_value_GL();

enum class SHEME_COLOR_GL:int{
    RGB,
    RGBA,
    RED
};
template <SHEME_COLOR_GL type_object>
int get_value_GL();
int get_value_GL(SHEME_COLOR_GL type_object);

enum class TYPE_SETTING_GL:int{
    DEPTH_TEST,
    BLEND,
    CULL_FACE,
    MULTISAMPLE
};

enum class TYPE_SETTING_DEPTH_FUN{
    NEVER,
    LESS,
    EQUAL,
    LEQUAL,
    GREATER,
    NOTEQUAL,
    GEQUAL,
    ALWAYS
};

enum class TYPE_SETTING_DEPTH_FUN_BLEND:int{
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    ONE,
    ZERO
};

template <TYPE_SETTING_DEPTH_FUN_BLEND type_object>
int get_value_GL();


enum class TYPE_SETTING_CLEAR:int{
    COLOR_BUFFER_BIT,
    DEPTH_BUFFER_BIT,
    STENCIL_BUFFER_BIT
};

template <TYPE_SETTING_CLEAR type_object>
int get_value_GL();

enum class TYPE_OBJECT_DRAW:int{
    TYPE_OBJECT_POINTS=0,
    TYPE_OBJECT_LINES,
    TYPE_OBJECT_LINE_STRIP,
    TYPE_OBJECT_LINE_LOOP,

    TYPE_LINE_STRIP_ADJACENCY,
    TYPE_LINES_ADJACENCY,

    TYPE_OBJECT_TRIANGLES,
    TYPE_OBJECT_TRIANGLE_STRIP,
    TYPE_OBJECT_TRIANGLE_FAN,

    TYPE_OBJECT_QUADS,
    TYPE_OBJECT_QUAD_STRIP,
    TYPE_OBJECT_POLYGON
};

template <TYPE_OBJECT_DRAW type_object>
int get_value_GL();

enum class BUFFER_SETTING:int {
    STATIC,
    DYNAMIC,
    STREAM
};

template <BUFFER_SETTING type_object>
int get_value_GL();
int get_value_GL(BUFFER_SETTING type_object);

enum class TYPE_TEXTURE:int {
    TEXTURE_1D,
    TEXTURE_2D,
    TEXTURE_3D,
    TEXTURE_2D_ARRAY,
    TEXTURE_CUBE_MAP
};

template <TYPE_TEXTURE type_object>
int get_value_GL();
int get_value_GL(TYPE_TEXTURE type_object);


enum class TYPE_BUFFER:int {
    NULL_,
    VERTEX_ARRAY,
    ARRAY_BUFFER,
    ELEMENT_ARRAY_BUFFER,
    DRAW_INDIRECT_BUFFER,
    COPY_READ_BUFFER,
    COPY_WRITE_BUFFER,
    SHADER_STORAGE_BUFFER,
    FRAMEBUFFER,
    RENDERBUFFER
};
template <TYPE_BUFFER type_object>
int get_value_GL();

enum class TYPE_MAP_ACCESS{
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE
};

template <TYPE_MAP_ACCESS type_object>
int get_value_GL();


enum class TYPE_ARGUMENT_SETTING_TEXTURE:int{
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
    MIRRORED_REPEAT,
    REPEAT,
    MIRROR_CLAMP_TO_EDGE,



    NEAREST,
    LINEAR
};

template <TYPE_ARGUMENT_SETTING_TEXTURE type_object>
int get_value_GL();
int get_value_GL(TYPE_ARGUMENT_SETTING_TEXTURE type_object);

enum class TYPE_SETTING_TEXTURE:int{
    TEXTURE_WRAP_R,
    TEXTURE_WRAP_T,
    TEXTURE_WRAP_S,
    TEXTURE_SWIZZLE_RGBA,
    TEXTURE_SWIZZLE_A,
    TEXTURE_SWIZZLE_B,
    TEXTURE_SWIZZLE_G,
    TEXTURE_SWIZZLE_R,
    TEXTURE_MAX_LEVEL,
    TEXTURE_MAX_LOD,
    TEXTURE_MIN_LOD,
    TEXTURE_MAG_FILTER,
    TEXTURE_MIN_FILTER,
    TEXTURE_LOD_BIAS,
    TEXTURE_COMPARE_MODE,
    TEXTURE_COMPARE_FUNC,
    TEXTURE_BORDER_COLOR,
    TEXTURE_BASE_LEVEL,
    DEPTH_STENCIL_TEXTURE_MODE
   };

template <TYPE_SETTING_TEXTURE type_object>
int get_value_GL();

enum class TYPE_SHADER:GLenum{
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER,
    TESS_CONTROL_SHADER,
    TESS_EVALUATION_SHADER,
    COMPUTE_SHADER
};


template <TYPE_SHADER type_object>
int get_value_GL();


//GL_POINTS — каждая вершина задает точку
//GL_LINES — каждая отдельная пара вершин задает линию
//GL_LINE_STRIP — каждая пара вершин задает линию (т.е. конец предыдущей линии является началом следующей)
//GL_LINE_LOOP — аналогично предыдущему за исключением того, что последняя вершина соединяется с первой и получается замкнутая фигура
//GL_TRIANGLES — каждая отдельная тройка вершин задает треугольник
//GL_TRIANGLE_STRIP — каждая следующая вершина задает треугольник вместе с двумя предыдущими (получается лента из треугольников)
//GL_TRIANGLE_FAN — каждый треугольник задается первой вершиной и последующими парами (т.е. треугольники строятся вокруг первой вершины, образуя нечто похожее на диафрагму)
//GL_QUADS — каждые четыре вершины образуют четырехугольник
//GL_QUAD_STRIP — каждая следующая пара вершин образует четырехугольник вместе с парой предыдущих
//GL_POLYGON — задает многоугольник с количеством углов равным количеству заданных вершин

inline constexpr int TYPE_RENDERING_ARRAY=0X1;
inline constexpr int TYPE_RENDERING_ELEMENT=0X4;
inline constexpr int TYPE_RENDERING_MYLTI=0X8;
inline constexpr int TYPE_RENDERING_INDERECT=0X10;
inline constexpr int TYPE_RENDERING_INSTANSE=0X20;
inline constexpr int TYPE_RENDERING_BASE_INSTANSE=0X40;
inline constexpr int TYPE_RENDERING_BASE_VERTEX=0X80;
inline constexpr int TYPE_RENDERING_BIND_BUFFER=0X100;

} // namespace GL_layer
#endif // GL_TYPE_TO_CPP_H
