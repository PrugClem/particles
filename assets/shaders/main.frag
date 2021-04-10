#version 460 core

in vertex_shader_output
{
    vec4 frag_color;
    vec2 frag_tex;
    vec3 frag_pos;
} fs_in;

out vec4 result_color;

uniform sampler2D tex;

void main()
{
    if( isnan(fs_in.frag_tex.x) || isnan(fs_in.frag_tex.y) )
        result_color = fs_in.frag_color;
    else
        result_color = texture(tex, fs_in.frag_tex) * fs_in.frag_color;
    //result_color = fs_in.frag_color;
}
