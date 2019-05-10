#version 330 core

// ================================================================================
// in & out

out vec4 r_color;

in O_VS {
	vec3 tex_coord;
} i_fs;

// ================================================================================
// uniform

uniform samplerCube	u_texture;

// ================================================================================

void main() {
	vec3 tex = normalize(i_fs.tex_coord);
	r_color = textureLod(u_texture, vec3(tex.x, tex.y, tex.z), 0);
}

// ================================================================================


