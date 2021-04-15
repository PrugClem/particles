#version 460 core

layout (location = 0) in vec2 aTexCoord;
layout (location = 1) in vec4 aColorRGBA;
layout (location = 2) in vec3 aPos;
layout (location = 3) in vec3 aNormal;

out vertex_shader_output
{
    vec4 frag_color;
    vec2 frag_tex;
    vec3 frag_pos;
} vs_out;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0f);
    vs_out.frag_color = aColorRGBA;
    vs_out.frag_pos = aPos;
    vs_out.frag_tex = aTexCoord;
}