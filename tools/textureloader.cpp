#include "textureloader.h"

// ===============================================================================================


bool TextureLoader::load_texture_png(const std::string& path, SPTR_uchar& data, uint& data_size) {
	std::ifstream fs;
	fs.open(path, std::ios::binary);
	if (!fs.is_open()) { return false; }
	
	fs.seekg(0, std::ios::end);										// �����ļ�β
	data_size = fs.tellg();											// �Ի�����ݴ�С
	auto t_data = make_shared_array<uchar>(data_size + 1);			// �Կ�����Ӧ�����Ĵ洢�ռ�
	data = t_data;

	fs.seekg(0, std::ios::beg);			// �����ļ�ͷ
	fs.read((char*)data.get(), data_size);
	
	return true;
}
