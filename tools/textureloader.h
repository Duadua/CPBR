#pragma once

#include "loader.h"

enum TextureGenType {
	SOLIDE,			// ��ɫ
	GRADUA,			// ���� -- ����
	THE_TWO,		// ���ֻ�����ɫ���

};

class TextureGen {
public:
	static bool gen_texture_txt(const std::string& res, TextureGenType type, CVector3D color = CVector3D(255.0f), SourceType source_type = SourceType::BY_FILE);

	~TextureGen() {}
private:
	TextureGen() {}

};

// ===============================================================================================

class TextureLoader {
public:

	static SPTR_uchar load_texture_txt(const std::string& path, uint& data_size, SourceType source_type = SourceType::BY_FILE);
	static SPTR_uchar load_texture_png(const std::string& path, uint& data_size);

	~TextureLoader() {}
private:
	TextureLoader() {}

};
