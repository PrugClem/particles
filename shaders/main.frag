#version 460 core

in vertex_shader_output
{
    vec4 frag_color;
    vec2 frag_tex;
    vec3 frag_pos;
} fs_in;

out vec4 result_color;

void main()
{
    result_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}