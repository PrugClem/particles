#version 460 core

in geometry_shader_output
{
    vec4 frag_color;
    vec2 frag_tex_coord;
} fs_in;

uniform sampler2D tex;

out vec4 res_color;

void main()
{
    res_color = texture(tex, fs_in.frag_tex_coord) * fs_in.frag_color;
}
