#include <interlayer_gl.h>
#include <GL/glew.h>
#include <iostream>
namespace GL_layer{

bool Init_GLEW(){
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }
    return true;
}

template<> void PolygonMode<TYPE_POLYGON_MOD::POINT>(){glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);}
template<> void PolygonMode<TYPE_POLYGON_MOD::LINE>() {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
template<> void PolygonMode<TYPE_POLYGON_MOD::FILL>() {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
void Finish(){
      glFinish();
}
GLenum Get_Error(){
    return glGetError();
}

void * MapBuffer(GLenum n,GLenum type){
    return glMapBuffer(n, type);
}

void UnmapBuffer(GLenum n){
    glUnmapBuffer(n);
}

void Viewport(GLint x, GLint y, GLsizei width, GLsizei height){
    glViewport(x,y,width,height);
}

void ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha){
    glClearColor(red,green,blue,alpha);
}

void Clear(GLbitfield mask){
     glClear(mask);
}

template<> void Enable<TYPE_SETTING_GL::BLEND>()     {glEnable(GL_BLEND);};
template<> void Enable<TYPE_SETTING_GL::CULL_FACE>() {glEnable(GL_CULL_FACE);};
template<> void Enable<TYPE_SETTING_GL::DEPTH_TEST>(){glEnable(GL_DEPTH_TEST);};
template<> void Enable<TYPE_SETTING_GL::MULTISAMPLE>(){glEnable(GL_MULTISAMPLE);};

template<> void DepthFunc<TYPE_SETTING_DEPTH_FUN::NEVER>(){glDepthFunc(GL_NEVER);}
template<> void DepthFunc<TYPE_SETTING_DEPTH_FUN::LESS>(){glDepthFunc(GL_LESS);}
template<> void DepthFunc<TYPE_SETTING_DEPTH_FUN::EQUAL>(){glDepthFunc(GL_EQUAL);}
template<> void DepthFunc<TYPE_SETTING_DEPTH_FUN::LEQUAL>(){glDepthFunc(GL_LEQUAL);}
template<> void DepthFunc<TYPE_SETTING_DEPTH_FUN::GREATER>(){glDepthFunc(GL_GREATER);}
template<> void DepthFunc<TYPE_SETTING_DEPTH_FUN::NOTEQUAL>(){glDepthFunc(GL_NOTEQUAL);}
template<> void DepthFunc<TYPE_SETTING_DEPTH_FUN::GEQUAL>(){glDepthFunc(GL_GEQUAL);}
template<> void DepthFunc<TYPE_SETTING_DEPTH_FUN::ALWAYS>(){glDepthFunc(GL_ALWAYS);}

void BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha){
    glBlendFuncSeparate(sfactorRGB,dfactorRGB,sfactorAlpha,dfactorAlpha);
}


void copy_buffer_(GLenum readtarget,GLenum writetarget, GLsizeiptr size_byte,GLintptr read_offset, GLintptr write_offset){
    glCopyBufferSubData(readtarget, writetarget, read_offset, write_offset, size_byte);
}

void read_pixel(GLsizei x,GLsizei y,GLsizei weight,GLsizei height,GLsizei type_scheme_pixel,GLsizei type_byte,GLubyte *  data){
    glReadPixels(x,y,weight,height,type_scheme_pixel,type_byte,data);
}

template<> void Bind<TYPE_BUFFER::NULL_>                       (GLenum buffer){glBindBuffer(0, buffer);};

template<> void Bind<TYPE_BUFFER::SHADER_STORAGE_BUFFER>       (GLenum buffer){glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);};
template<> void Bind<TYPE_BUFFER::DRAW_INDIRECT_BUFFER>        (GLenum buffer){glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer);};

template<> void Bind<TYPE_BUFFER::COPY_READ_BUFFER>            (GLenum buffer){glBindBuffer(GL_COPY_READ_BUFFER, buffer);};
template<> void Bind<TYPE_BUFFER::COPY_WRITE_BUFFER>           (GLenum buffer){glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);};

template<> void Bind<TYPE_BUFFER::ARRAY_BUFFER>                (GLenum buffer){glBindBuffer(GL_ARRAY_BUFFER, buffer);};
template<> void Bind<TYPE_BUFFER::ELEMENT_ARRAY_BUFFER>        (GLenum buffer){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);};


template<> void Bind<TYPE_BUFFER::FRAMEBUFFER> (GLenum buffer){glBindFramebuffer(GL_FRAMEBUFFER, buffer);};
template<> void Bind<TYPE_BUFFER::RENDERBUFFER>(GLenum buffer){glBindRenderbuffer(GL_RENDERBUFFER, buffer);};
template<> void Bind<TYPE_BUFFER::VERTEX_ARRAY>(GLenum buffer){glBindVertexArray(buffer);};




//var int
void   Uniform(GLint id_variable, GLint v0, GLint v1, GLint v2, GLint v3){
  glUniform4i(id_variable,v0,v1,v2,v3);
}
void   Uniform(GLint id_variable, GLint v0, GLint v1, GLint v2){
  glUniform3i(id_variable,v0,v1,v2);
}
void   Uniform(GLint id_variable, GLint v0, GLint v1){
  glUniform2i(id_variable,v0,v1);
}
void   Uniform(GLint id_variable, GLint v0){
 glUniform1i(id_variable,v0);
}

//var float
void   Uniform(GLint id_variable, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
  glUniform4f(id_variable,v0,v1,v2,v3);
};
void   Uniform(GLint id_variable, GLfloat v0, GLfloat v1, GLfloat v2){
  glUniform3f(id_variable,v0,v1,v2);
};
void  Uniform(GLint id_variable, GLfloat v0, GLfloat v1){
  glUniform2f(id_variable,v0,v1);
};
void  Uniform(GLint id_variable, GLfloat v0){
  glUniform1f(id_variable,v0);
};

//array float
template<> void   Uniform<4>(GLint id_variable, GLsizei count, const GLfloat* value){
  glUniform4fv(id_variable,count,value);
};
template<> void   Uniform<3>(GLint id_variable, GLsizei count, const GLfloat* value){
  glUniform3fv(id_variable,count,value);
};
template<> void   Uniform<2>(GLint id_variable, GLsizei count, const GLfloat* value){
  glUniform2fv(id_variable,count,value);
};
template<> void   Uniform<1>(GLint id_variable, GLsizei count, const GLfloat* value){
  glUniform1fv(id_variable,count,value);
};


//mat float
template<> void  Uniform<4,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix4fv(id_variable, count, transpose,value);
};
template<> void  Uniform<4,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix4x3fv(id_variable, count, transpose,value);
};
template<> void  Uniform<4,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix4x2fv(id_variable, count, transpose,value);
};


template<> void  Uniform<3,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix3x4fv(id_variable, count, transpose,value);
};
template<> void  Uniform<3,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix3fv(id_variable, count, transpose,value);
};
template<> void  Uniform<3,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix3x2fv(id_variable, count, transpose,value);
};

template<> void  Uniform<2,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix2x4fv(id_variable, count, transpose,value);
};
template<> void  Uniform<2,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix2x3fv(id_variable, count, transpose,value);
};
template<> void  Uniform<2,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix2fv(id_variable, count, transpose,value);
};

//var int


//array int
template<> void   Uniform<4>(GLint id_variable, GLsizei count, const GLint* value){
    glUniform4iv(id_variable,count,value);
};
template<> void   Uniform<3>(GLint id_variable, GLsizei count, const GLint* value){
    glUniform3iv(id_variable,count,value);
};
template<> void   Uniform<2>(GLint id_variable, GLsizei count, const GLint* value){
    glUniform2iv(id_variable,count,value);
};
template<> void   Uniform<1>(GLint id_variable, GLsizei count, const GLint* value){
    glUniform1iv(id_variable,count,value);
};

//var double
void   Uniform(GLint id_variable, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3){
  glUniform4d(id_variable,v0,v1,v2,v3);
};
void   Uniform(GLint id_variable, GLdouble v0, GLdouble v1, GLdouble v2){
  glUniform3d(id_variable,v0,v1,v2);
};
void   Uniform(GLint id_variable, GLdouble v0, GLdouble v1){
  glUniform2d(id_variable,v0,v1);
};
void   Uniform(GLint id_variable, GLdouble v0){
  glUniform1d(id_variable,v0);
};

//array double
template<> void   Uniform<4>(GLint id_variable, GLsizei count, const GLdouble* value){
  glUniform4dv(id_variable,count,value);
};
template<> void   Uniform<3>(GLint id_variable, GLsizei count, const GLdouble* value){
  glUniform3dv(id_variable,count,value);
};
template<> void   Uniform<2>(GLint id_variable, GLsizei count, const GLdouble* value){
  glUniform2dv(id_variable,count,value);
};
template<> void   Uniform<1>(GLint id_variable, GLsizei count, const GLdouble* value){
  glUniform1dv(id_variable,count,value);
};


//mat double
template<> void  Uniform<4,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix4dv(id_variable, count, transpose,value);
};
template<> void  Uniform<4,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix4x3dv(id_variable, count, transpose,value);
};
template<> void  Uniform<4,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix4x2dv(id_variable, count, transpose,value);
};

template<> void  Uniform<3,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix3x4dv(id_variable, count, transpose,value);
};
template<> void  Uniform<3,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix3dv(id_variable, count, transpose,value);
};
template<> void  Uniform<3,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix3x2dv(id_variable, count, transpose,value);
};

template<> void  Uniform<2,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix2x4dv(id_variable, count, transpose,value);
};
template<> void  Uniform<2,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix2x3dv(id_variable, count, transpose,value);
};
template<> void  Uniform<2,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix2dv(id_variable, count, transpose,value);
};
}// namespace GL_layer
