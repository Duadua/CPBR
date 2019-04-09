#version 330 core

out vec4 r_color;

in O_VS {
	vec3 tex_coord;
} i_fs;

uniform samplerCube	u_texture;

void main() {
	vec3 tex = normalize(i_fs.tex_coord);
	r_color = texture(u_texture, vec3(tex.x, tex.y, tex.z));
}



