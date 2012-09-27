@echo off

mkdir headers
mkdir headers\d3dx

fxc /T ps_2_0 /Fh "headers/d3dx/ps_2_0.h" /Vn g_ps_2_0 pixel_shader.hlsl
fxc /T ps_3_0 /Fh "headers/d3dx/ps_3_0.h" /Vn g_ps_3_0 pixel_shader.hlsl
fxc /T ps_4_0 /Fh "headers/d3dx/ps_4_0.h" /Vn g_ps_4_0 pixel_shader.hlsl

fxc /T vs_2_0 /Fh "headers/d3dx/vs_2_0.h" /Vn g_vs_2_0 vertex_shader.hlsl
fxc /T vs_3_0 /Fh "headers/d3dx/vs_3_0.h" /Vn g_vs_3_0 vertex_shader.hlsl
fxc /T vs_4_0 /Fh "headers/d3dx/vs_4_0.h" /Vn g_vs_4_0 vertex_shader.hlsl