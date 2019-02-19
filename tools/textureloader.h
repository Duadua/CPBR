#pragma once

#include "loader.h"

enum TextureGenType {
	SOLIDE,			// ��ɫ
	GRADUA,			// ���� -- ����
	THE_TWO,		// ���ֻ�����ɫ���

};

class TextureGen {
public:
	static bool gen_texture_txt(const std::string& path, TextureGenType type, CVector3D color = CVector3D(255.0f), SourceType source_type = SourceType::BY_FILE);

	~TextureGen() {}
private:
	TextureGen() {}

};

// ===============================================================================================

class TextureLoader {
public:

	static bool load_texture_txt(const std::string& path, SPTR_uchar& data, uint& data_size, SourceType source_type = SourceType::BY_FILE);
	static bool load_texture_png(const std::string& path, SPTR_uchar& data, uint& data_size);

	~TextureLoader() {}
private:
	TextureLoader() {}

};
