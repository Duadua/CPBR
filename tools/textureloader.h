#pragma once

#include "loader.h"

enum TextureGenType {
	SOLIDE,			// 纯色
	GRADUA,			// 渐变 -- 线性
	THE_TWO,		// 二种互补颜色相间

};

class TextureGen {
public:
	static bool gen_texture_txt(const std::string& res, TextureGenType type, CColor color = CColor(), uint depth = 8, SourceType source_type = SourceType::BY_FILE);

	~TextureGen() {}
private:
	TextureGen() {}

	static SPTR_uchar gen_solide(uint& data_size, uint width, uint heigh, CColor color);
	static SPTR_uchar gen_gradua(uint& data_size, uint width, uint heigh, CColor color);
	static SPTR_uchar gen_thetwo(uint& data_size, uint width, uint heigh, CColor color);
};

// ===============================================================================================

class TextureLoader {
public:

	static SPTR_uchar load_texture_txt(const std::string& path, uint& width, uint& heigh, SourceType source_type = SourceType::BY_FILE);
	static SPTR_uchar load_texture_png(const std::string& path, uint& data_size);
	static SPTR_uchar load_texture_x(const std::string& path, int& width, int& heigh, int& channel);	// load by stb_img
	static SPTR_float load_texture_x_hdr(const std::string& path, int& width, int& heigh, int& channel);// load by stb_img -- hdr
	static SPTR_uchar load_texture_dds(const std::string& path, int& width, int& heigh, int& channel);	// load dds file

	~TextureLoader() {}
private:
	TextureLoader() {}

};
