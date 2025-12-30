#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in float S;

void main() {
    float x0=gl_in[0].gl_Position.x;
    float y0=gl_in[0].gl_Position.y;
    float x1=gl_in[1].gl_Position.x;
    float y1=gl_in[1].gl_Position.y;

    float m=(y1-y0)*(y1-y0)/((x1-x0)*(x1-x0));

    float y_=S*inversesqrt(1.0+m);
    float x_=S*inversesqrt(1.0+1.0/m);

	
    gl_Position =vec4(x_+x0, -y_+y0, 0.0, 0.0);    // 2:bottom-right
    EmitVertex();   
    gl_Position =vec4(-x_+x0, y_+y0, 0.0, 0.0);    // 1:bottom-left
    EmitVertex();
    gl_Position =vec4(-x_+x1, y_+y1, 0.0, 0.0);    // 3:top-left
    EmitVertex();
    gl_Position =vec4(x_+x1, -y_+y1, 0.0, 0.0);    // 4:top-right
    EmitVertex();
    EndPrimitive();
}
