#version 330 core

out vec4 r_color;

in O_VS {
	vec2 tex_coord;
} i_fs;

uniform sampler2D	u_texture;
uniform float		u_gamma;

vec3 gamma(vec3 c, float g) { return pow(c, vec3(1.0 / g)); }

void main() {
	vec4 t_c = texture(u_texture, i_fs.tex_coord);
	r_color = vec4(gamma(t_c.rgb, u_gamma), t_c.a);
}


