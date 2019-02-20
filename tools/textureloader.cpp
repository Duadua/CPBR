#include "textureloader.h"

bool TextureGen::gen_texture_txt(const std::string& res, TextureGenType type, CVector3D color, SourceType source_type) {
	return true;
}

// ===============================================================================================

SPTR_uchar TextureLoader::load_texture_txt(const std::string& path, uint& data_size, SourceType source_type) {
	return nullptr;
}

SPTR_uchar TextureLoader::load_texture_png(const std::string& path, uint& data_size) {
	std::ifstream fs;
	fs.open(path, std::ios::binary);
	if (!fs.is_open()) { return nullptr; }
	
	fs.seekg(0, std::ios::end);										// �����ļ�β
	data_size = fs.tellg();											// �Ի�����ݴ�С
	auto t_data = make_shared_array<uchar>(data_size + 1);			// �Կ�����Ӧ�����Ĵ洢�ռ�

	fs.seekg(0, std::ios::beg);			// �����ļ�ͷ
	fs.read((char*)t_data.get(), data_size);
	
	return t_data;
}
