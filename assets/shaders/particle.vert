#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in float aWidth;

out vertex_shader_output 
{
    vec4 geom_color;
    float geom_width;
} vs_out;

void main()
{
    gl_Position = vec4(aPos, 1.0f);
    vs_out.geom_color = aColor;
    vs_out.geom_width = aWidth;
}
