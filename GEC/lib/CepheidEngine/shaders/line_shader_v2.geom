#version 400 core
layout (lines_adjacency ) in;//входные данные, две точки (x0;y0) и (x1;y1)

//входные данные, точки прямоугольника
//(прямоугольник рисуется с помощью двух треугольников,
//strip значит, что для построения второго треугольника будут использованны две вершины предыдущего)
layout (triangle_strip, max_vertices = 80) out;

in  float S[];//передаём ширину линии

//транзит цвета линии через геометрический шейдер
in  vec4 mat_color_[];
out  vec4 mat_color;

vec2[4] get_point_quard(vec2 P0, vec2 P1){
    vec2 k[4];

    //получаем смещения
    float L =(distance(P0,P1));

    float K=S[0]/L;

    float x_=-K*(P1.y-P0.y);
    float y_= K*(P1.x-P0.x);

    //вычесляем точки
    k[0]=vec2( x_+P0.x, y_+P0.y);
    k[1]=vec2(-x_+P0.x,-y_+P0.y);

    k[2]=vec2( x_+P1.x, y_+P1.y);
    k[3]=vec2(-x_+P1.x,-y_+P1.y);

    return k;
}

vec2 get_point_corner(vec2 P1,vec2 P2,vec2 P3,vec2 P4){
    float t= (P1.x-P2.x)*(P3.y-P4.y) - (P3.x-P4.x)*(P1.y-P2.y);
    float a=P1.x*P2.y-P2.x*P1.y;
    float b=P3.x*P4.y-P4.x*P3.y;

    float x=(a*(P3.x-P4.x)-b*(P1.x-P2.x))/t;
    float y=(a*(P3.y-P4.y)-b*(P1.y-P2.y))/t;
    return vec2(x,y);
}

void bild_quard(vec2[4] point){
    gl_Position =vec4(point[0], 0.0, 1.0);    // 1:bottom-left
    EmitVertex();
    gl_Position =vec4(point[1], 0.0, 1.0);    // 2:bottom-right
    EmitVertex();
    gl_Position =vec4(point[2], 0.0, 1.0);    // 3:top-left
    EmitVertex();
    gl_Position =vec4(point[3], 0.0, 1.0);    // 4:top-right
    EmitVertex();
}

void oblique_connection(vec2 P0, vec2 P1){
    gl_Position =vec4(P0, 0.0, 1.0);    // 5
    EmitVertex();
    gl_Position =vec4(P1, 0.0, 1.0);    // 6
    EmitVertex();
}

void direct_connection(vec2 T_0, vec2 T_1,vec2 P0, vec2 P1){
    gl_Position =vec4(T_0.xy, 0.0, 1.0);
    EmitVertex();
    gl_Position =vec4(T_1.xy, 0.0, 1.0);
    EmitVertex();
    gl_Position =vec4(P0, 0.0, 1.0);
    EmitVertex();
    gl_Position =vec4(P1, 0.0, 1.0);
    EmitVertex();
}

void bezier_connection(vec2 T_0, vec2 T_1,vec2 P0, vec2 P1,vec2 P2, vec2 P3){
    float t=0.0;
    int count=16;
    for(int z = 0; z < count+1; z++){
        vec2 B_0=(1-t)*(1-t)*P0+2*t*(1-t)*T_0+t*t*P2;
        vec2 B_1=(1-t)*(1-t)*P1+2*t*(1-t)*T_1+t*t*P3;

        gl_Position =vec4(B_0.xy, 0.0, 1.0);
        EmitVertex();
        gl_Position =vec4(B_1.xy, 0.0, 1.0);
        EmitVertex();

        t+=1.0/count;
    }
}

void main() {
    //транзит цвета
    mat_color=mat_color_[0];

    vec2 quard_1[4]=get_point_quard(gl_in[0].gl_Position.xy,gl_in[1].gl_Position.xy);
    vec2 quard_2[4]=get_point_quard(gl_in[1].gl_Position.xy,gl_in[2].gl_Position.xy);

    //ищём точку угла
    vec2 t_= get_point_corner(quard_1[0],quard_1[2],quard_2[0],quard_2[2]);
    vec2 t_1=get_point_corner(quard_1[1],quard_1[3],quard_2[1],quard_2[3]);

    //строим прямоугольник-линию
    bild_quard(quard_1);

    //строим соединнение линии с другой
    //oblique_connection(quard_2[0],quard_2[1]);
    //direct_connection(t_,t_1,quard_2[0],quard_2[1]);

    bezier_connection(t_,t_1,quard_1[2],quard_1[3],quard_2[0],quard_2[1]);
    EndPrimitive();
}


