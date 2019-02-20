#pragma once

#include "loader.h"

enum TextureGenType {
	SOLIDE,			// ��ɫ
	GRADUA,			// ���� -- ����
	THE_TWO,		// ���ֻ�����ɫ���

};

class TextureGen {
public:
	static bool gen_texture_txt(const std::string& res, TextureGenType type, CColor color = CColor(), uint depth = 8, SourceType source_type = SourceType::BY_FILE);

	~TextureGen() {}
private:
	TextureGen() {}

	static SPTR_uchar gen_solide(uint& data_size, uint width, uint heigh, CColor color);

};

// ===============================================================================================

class TextureLoader {
public:

	static SPTR_uchar load_texture_txt(const std::string& path, uint& width, uint& heigh, SourceType source_type = SourceType::BY_FILE);
	static SPTR_uchar load_texture_png(const std::string& path, uint& data_size);

	~TextureLoader() {}
private:
	TextureLoader() {}

};
