#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in float S;

void main() {
    float x0=gl_in[0].gl_Position.x;
    float y0=gl_in[0].gl_Position.y;
    float x1=gl_in[1].gl_Position.x;
    float y1=gl_in[1].gl_Position.y;

    float a=acos(abs(x1-x0)/distance(gl_in[0].gl_Position, gl_in[1].gl_Position));

    float y_=S*sin(a);
    float x_=S*cos(a);
	
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
