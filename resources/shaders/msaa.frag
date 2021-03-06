#version 330 core

// ================================================================================
// in & out

layout(location = 0) out vec4 r_color;
layout(location = 1) out vec4 r_high_color;

in O_VS {
	vec2 tex_coord;
} i_fs;

// ================================================================================
// uniform

uniform sampler2DMS u_texture_msaa;				// 多重采样纹理
uniform sampler2DMS u_texture_msaa_high;		// 多重采样纹理
uniform int u_msaa_num;							// 采样数 -- 默认为4

// ================================================================================

vec4 msaa();
vec4 msaa_high();

// ================================================================================

void main() {
	r_color = msaa();
	r_high_color = msaa_high();
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

vec4 msaa_high() {
	vec4 res = vec4(0.0);

	ivec2 tex_size = textureSize(u_texture_msaa_high);
	ivec2 t_size = ivec2(i_fs.tex_coord * tex_size);

	if(0 == u_msaa_num) { res = texelFetch(u_texture_msaa_high, t_size, 0); }
	else {
		for(int i = 0; i < u_msaa_num; ++i) { res += texelFetch(u_texture_msaa_high, t_size, i);  }
		res =  res / u_msaa_num;  
	}
    float t_filter = dot(res.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (t_filter > 2.0) {
        res = vec4(res.rgb, 1.0);
    }
    else
	{
        res = vec4(0.0);
    }
	return res;
}

// ================================================================================
