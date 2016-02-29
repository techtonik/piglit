#version 150
#extension GL_ARB_gpu_shader_fp64 : require

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform ${from_type} from;
uniform ${to_type} to;

in vec4 vertex_to_gs[3];
out vec4 fs_color;

#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)

void main()
{
    ${to_type} converted = ${converted_from};
    bool match = converted == to;
    fs_color = match ? GREEN : RED;

    for (int i = 0; i < 3; i++) {
        gl_Position = vertex_to_gs[i];
        EmitVertex();
    }
}
