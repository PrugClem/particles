#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vertex_shader_output 
{
    vec4 geom_color;
    float geom_width;
} gs_in[];  

out geometry_shader_output
{
    vec4 frag_color;
    vec2 frag_tex_coord;
} gs_out;

uniform mat4 view, projection;

void main()
{
    mat4 VP = projection * view;
    vec3 cam_right = vec3(view[0][0], view[1][0], view[2][0]);  // get right vector
    vec3 cam_up = vec3(view[0][1], view[1][1], view[2][1]);     // get up vector
    vec3 pos = gl_in[0].gl_Position.xyz;

    // first vertex
    gl_Position = VP * vec4(pos + cam_right * gs_in[0].geom_width*+0.5f + cam_up * gs_in[0].geom_width*-0.5f, 1.0f);
    gs_out.frag_color = gs_in[0].geom_color;
    gs_out.frag_tex_coord = vec2(1.0f, 0.0f);
    EmitVertex();

    // second vertex
    gl_Position = VP * vec4(pos + cam_right * gs_in[0].geom_width*+0.5f + cam_up * gs_in[0].geom_width*+0.5f, 1.0f);
    gs_out.frag_color = gs_in[0].geom_color;
    gs_out.frag_tex_coord = vec2(1.0f, 1.0f);
    EmitVertex();

    // third vertex
    gl_Position = VP * vec4(pos + cam_right * gs_in[0].geom_width*-0.5f + cam_up * gs_in[0].geom_width*-0.5f, 1.0f);
    gs_out.frag_color = gs_in[0].geom_color;
    gs_out.frag_tex_coord = vec2(0.0f, 0.0f);
    EmitVertex();

    // forth vertex
    gl_Position = VP * vec4(pos + cam_right * gs_in[0].geom_width*-0.5f + cam_up * gs_in[0].geom_width*+0.5f, 1.0f);
    gs_out.frag_color = gs_in[0].geom_color;
    gs_out.frag_tex_coord = vec2(0.0f, 1.0f);
    EmitVertex();

    EndPrimitive();
}
