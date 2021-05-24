#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 14) out;

in VS_OUT {
    vec4 fragColor;
} gs_in[];

out vec4 fragColor;

const float twicePi = 6.28318530717958647692; //2*3.14

void build_house(vec4 position)
{
    /*    
    uint numberOfSides = 12;
    uint nPointsOfCircle = 14; //numberOfSides + 2;
    float radius = 0.08;
    fragColor = gs_in[0].fragColor;
    fragColor.a = 1.0;
    gl_Position = position;
    EmitVertex();
    
    for(int i = 1; i < nPointsOfCircle; i++){
        fragColor = gs_in[0].fragColor * 0.1;
        gl_Position.x = position.x + (radius * cos(i * twicePi / numberOfSides));
        gl_Position.y = position.y + (radius * sin(i * twicePi / numberOfSides));
        EmitVertex();
    }

    EndPrimitive();
*/
    fragColor = gs_in[0].fragColor;
     for (int i = 0; i <= 13; i++) {
        float ang = twicePi / 13.0 * i;

        vec4 offset = vec4(cos(ang) * 0.1, -sin(ang) * 0.2, 0.0, 0.0);
        gl_Position = position + offset;

        EmitVertex();
    }

    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
}

