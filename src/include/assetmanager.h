#pragma once

#include "mesh.h"
#include "shader.h"
#include <QMap>
#include <map>
#include <QString>
#include <QVector>

class AssetManager {
public:
	static QMap<QString, SPTR_Shader> map_shaders;
	static QMap<QString, SPTR_Mesh> map_meshs;
public:
	static SPTR_Shader load_shader(const QString& key, const QString& v_path, const QString& f_path, const QString& g_path = nullptr);
	static SPTR_Shader get_shader(const QString& key);
	static bool delete_shader(const QString& key);
	static bool clear_shaders();

	static SPTR_Mesh load_mesh(const QString& key, const QString path);
	static SPTR_Mesh get_mesh(const QString& key);

private:
	AssetManager() {}
};

