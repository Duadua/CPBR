#include "textureloader.h"

bool TextureGen::gen_texture_txt(const std::string& res, TextureGenType type, CColor color, uint depth, SourceType source_type) {
	depth = CMath::clamp(depth, 1u, 10u);
	uint width = CMath::pow(2u, depth);
	uint heigh = CMath::pow(2u, depth);

	// ���ļ�
	std::ostream* out;
	std::ofstream fs;
	std::stringstream ss;
	if (source_type == SourceType::BY_FILE) {
		fs.open("resources/textures/txt/" + res, std::ios::binary | std::ios::trunc | std::ios::out);
		if (!fs.is_open()) { return false; }
		out = &fs;
	}
	else if (source_type == SourceType::BY_STRING) { ss.clear(); out = &ss; }

	// ��������
	SPTR_uchar t_data;
	uint t_size;
	switch (type) {
	case TextureGenType::SOLIDE: t_data = gen_solide(t_size, width, heigh, color); break;
	case TextureGenType::GRADUA: t_data = gen_gradua(t_size, width, heigh, color); break;
	case TextureGenType::THE_TWO: t_data = gen_thetwo(t_size, width, heigh, color); break;
	default:break;
	}

	if(t_data != nullptr) out->write((char*)t_data.get(), t_size);

	return true;
}

SPTR_uchar TextureGen::gen_solide(uint& data_size, uint width, uint heigh, CColor color) {
	data_size = width * 4 * heigh + sizeof(uint)*2;
	auto t_data = make_shared_array<uchar>(data_size + 1);

	memcpy(t_data.get(), &width, sizeof(uint));
	memcpy(t_data.get() + sizeof(uint), &heigh, sizeof(uint));
	
	for (uint i = 0; i < heigh; ++i) {
		for (uint j = 0; j < width; ++j) {
			uint t_c = color.get_uint();
			memcpy(t_data.get() + sizeof(uint) * 2 + i * width * 4 + j * 4, &t_c, sizeof(uint));
		}
	}
	return t_data;
}
SPTR_uchar TextureGen::gen_gradua(uint& data_size, uint width, uint heigh, CColor color) {
	data_size = width * 4 * heigh + sizeof(uint) * 2;
	auto t_data = make_shared_array<uchar>(data_size + 1);

	memcpy(t_data.get(), &width, sizeof(uint));
	memcpy(t_data.get() + sizeof(uint), &heigh, sizeof(uint));

	for (uint i = 0; i < heigh; ++i) {
		for (uint j = 0; j < width; ++j) {
			float t_v = (float)j / width;
			uint t_r = CMath::interp_linear(t_v, color.r(), 255.0f);
			uint t_g = CMath::interp_linear(t_v, color.g(), 255.0f);
			uint t_b = CMath::interp_linear(t_v, color.b(), 255.0f);
			//uint t_a = CMath::interp_linear(t_v, color.a(), 0.0f);
			CColor t_color(t_r, t_g, t_b);
			uint t_c = t_color.get_uint();
			memcpy(t_data.get() + sizeof(uint) * 2 + i * width * 4 + j * 4, &t_c, sizeof(uint));
		}
	}
	return t_data;
}
SPTR_uchar TextureGen::gen_thetwo(uint& data_size, uint width, uint heigh, CColor color) {
	data_size = width * 4 * heigh + sizeof(uint) * 2;
	auto t_data = make_shared_array<uchar>(data_size + 1);

	memcpy(t_data.get(), &width, sizeof(uint));
	memcpy(t_data.get() + sizeof(uint), &heigh, sizeof(uint));

	for (uint i = 0; i < heigh; ++i) {
		for (uint j = 0; j < width; ++j) {
			uint t_i = i / 32;
			uint t_j = j / 32;
			CColor t_color;
			if ((t_i + t_j) & 1) { t_color = color; }
			else { t_color = CColor(255 - color.r(), 255 - color.g(), 255 - color.b()); }
			uint t_c = t_color.get_uint();
			memcpy(t_data.get() + sizeof(uint) * 2 + i * width * 4 + j * 4, &t_c, sizeof(uint));
		}
	}
	return t_data;

}

// ===============================================================================================

SPTR_uchar TextureLoader::load_texture_txt(const std::string& path, uint& width, uint& heigh, SourceType source_type) {
	std::istream* in;
	std::ifstream fs;
	std::istringstream ss;
	if (source_type == SourceType::BY_FILE) {
		fs.open(path, std::ios::binary | std::ios::in);
		if (!fs.is_open()) { return nullptr; }
		in = &fs;
	}
	else if (source_type == SourceType::BY_STRING) { ss.clear(); ss.str(path); in = &ss; }

	uint t_size;
	in->seekg(0, std::ios::end);										// �����ļ�β
	t_size = in->tellg();												// �Ի�����ݴ�С
	auto t_data = make_shared_array<uchar>(t_size + 1);					// �Կ�����Ӧ�����Ĵ洢�ռ�

	in->seekg(0, std::ios::beg);										// �����ļ�ͷ
	in->read((char*)t_data.get(), t_size);
	
	memcpy(&width, t_data.get(), sizeof(uint));
	memcpy(&heigh, t_data.get()+sizeof(uint), sizeof(uint));
	uint data_size = width * 4 * heigh;

	auto t_res = make_shared_array<uchar>(data_size + 1);
	memcpy(t_res.get(), t_data.get() + sizeof(uint) * 2, data_size);
	return t_res;
}
SPTR_uchar TextureLoader::load_texture_png(const std::string& path, uint& data_size) {
	std::ifstream fs;
	fs.open(path, std::ios::binary);
	if (!fs.is_open()) { return nullptr; }
	
	fs.seekg(0, std::ios::end);										// �����ļ�β
	data_size = fs.tellg();											// �Ի�����ݴ�С
	auto t_data = make_shared_array<uchar>(data_size + 1);			// �Կ�����Ӧ�����Ĵ洢�ռ�

	fs.seekg(0, std::ios::beg);										// �����ļ�ͷ
	fs.read((char*)t_data.get(), data_size);
	
	return t_data;
}
