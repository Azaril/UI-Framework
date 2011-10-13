@echo off

mkdir headers

fxc /T ps_2_0 /O3 /Fh "headers/ps_2_0.h" /Vn g_ps_2_0 pixel_shader.hlsl
fxc /T ps_3_0 /O3 /Fh "headers/ps_3_0.h" /Vn g_ps_3_0 pixel_shader.hlsl
fxc /T ps_4_0 /O3 /Fh "headers/ps_4_0.h" /Vn g_ps_4_0 pixel_shader.hlsl

fxc /T vs_2_0 /O3 /Fh "headers/vs_2_0.h" /Vn g_vs_2_0 vertex_shader.hlsl
fxc /T vs_3_0 /O3 /Fh "headers/vs_3_0.h" /Vn g_vs_3_0 vertex_shader.hlsl
fxc /T vs_4_0 /O3 /Fh "headers/vs_4_0.h" /Vn g_vs_4_0 vertex_shader.hlsl