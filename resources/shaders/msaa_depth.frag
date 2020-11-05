#version 330 core

// ================================================================================
// in & out

in O_VS {
	vec2 tex_coord;
} i_fs;

// ================================================================================
// uniform

uniform sampler2DMS u_texture_msaa;				// ���ز�������
uniform int u_msaa_num;							// ������ -- Ĭ��Ϊ4

// ================================================================================

vec4 msaa();

// ================================================================================

void main() {
	gl_FragDepth = msaa().r;
}

// ================================================================================

vec4 msaa() {
	vec4 res = vec4(0.0);

	ivec2 tex_size = textureSize(u_texture_msaa);
	ivec2 t_size = ivec2(i_fs.tex_coord * tex_size);

	if(0 == u_msaa_num) { res = texelFetch(u_texture_msaa, t_size, 0); }
	else {
		for(int i = 0; i < u_msaa_num; ++i) { res += texelFetch(u_texture_msaa, t_size, i);  }
		res =  res / u_msaa_num;  
	}

	return res;
}

// ================================================================================
