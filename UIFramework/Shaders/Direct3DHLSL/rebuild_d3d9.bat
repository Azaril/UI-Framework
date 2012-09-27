@echo off

mkdir headers
mkdir headers\d3d

fxc /T ps_2_0 /O3 /Fh "headers/d3d/ps_2_0.h" /Vn g_ps_2_0 pixel_shader.hlsl
fxc /T ps_3_0 /O3 /Fh "headers/d3d/ps_3_0.h" /Vn g_ps_3_0 pixel_shader.hlsl
fxc /T ps_4_0 /O3 /Fh "headers/d3d/ps_4_0.h" /Vn g_ps_4_0 pixel_shader.hlsl

fxc /T vs_2_0 /O3 /Fh "headers/d3d/vs_2_0.h" /Vn g_vs_2_0 vertex_shader.hlsl
fxc /T vs_3_0 /O3 /Fh "headers/d3d/vs_3_0.h" /Vn g_vs_3_0 vertex_shader.hlsl
fxc /T vs_4_0 /O3 /Fh "headers/d3d/vs_4_0.h" /Vn g_vs_4_0 vertex_shader.hlsl