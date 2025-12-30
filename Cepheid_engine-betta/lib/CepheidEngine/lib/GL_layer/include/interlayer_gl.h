#ifndef INTERLAYER_GL_H
#define INTERLAYER_GL_H
#include "GL_type_to_cpp.h"
namespace GL_layer{

bool Init_GLEW();

template <TYPE_POLYGON_MOD mod>
void PolygonMode();

void Finish();
GLenum Get_Error();



void * MapBuffer(GLenum n,GLenum type);
void UnmapBuffer(GLenum n);

template<TYPE_BUFFER n>
void  Bind(GLenum buffer);

void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);

void ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void Clear(GLbitfield mask);

template<TYPE_SETTING_GL cap>
void Enable();

template <TYPE_SETTING_DEPTH_FUN func>
void DepthFunc();

void BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

void copy_buffer_(GLenum readtarget,GLenum writetarget, GLsizeiptr size_byte,GLintptr read_offset=0, GLintptr write_offset=0);
void read_pixel(GLsizei x,GLsizei y,GLsizei weight,GLsizei height,GLsizei type_scheme_pixel,GLsizei type_byte,GLubyte *  data);
//var int
void   Uniform(GLint id_variable, GLint v0, GLint v1, GLint v2, GLint v3);
void   Uniform(GLint id_variable, GLint v0, GLint v1, GLint v2);
void   Uniform(GLint id_variable, GLint v0, GLint v1);
void   Uniform(GLint id_variable, GLint v0);

//var float
void   Uniform(GLint id_variable, GLfloat v0);
void   Uniform(GLint id_variable, GLfloat v0, GLfloat v1);
void   Uniform(GLint id_variable, GLfloat v0, GLfloat v1, GLfloat v2);
void   Uniform(GLint id_variable, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
//array float
template<int i>
void   Uniform(GLint id_variable, GLsizei count, const GLfloat* value);
//mat float
template<int i,int j>
void  Uniform(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value);

//array int
template<int i>
void  Uniform(GLint id_variable, GLsizei count, const GLint* value);

//var double
void   Uniform(GLint id_variable, GLdouble x);
void   Uniform(GLint id_variable, GLdouble x, GLdouble y);
void   Uniform(GLint id_variable, GLdouble x, GLdouble y, GLdouble z);
void   Uniform(GLint id_variable, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
//array double
template<int i>
void  Uniform(GLint id_variable, GLsizei count, const GLdouble* value);
//mat double
template<int i,int j>
void  Uniform(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value);


}
#endif // INTERLAYER_GL_H
