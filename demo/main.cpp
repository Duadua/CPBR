#include "cmainwindow.h"
#include "mygamemanager.h"
#include <QtWidgets/QApplication>

/*#include "tools/meshloader.h"
#include "tools/materialloader.h"
#include "tools/textureloader.h"
#include "tools/qtimagehelper.h"
*/

#include "timemanager.h"
#include "singleton.h"

#ifndef NDEBUG
#include "cdebuger.h"
#endif // !NDEBUG

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
		
#ifdef C_DEBUG
	// test c_debuger()
	{ c_debuger() << "[debug]start"; }
#endif // C_DEBUG

	/* test reflex
	CObject* obj = ClassFactory::create_object("CObject");
	c_debuger() << obj->get_class_info()->get_class_name();
	
	CObject* gobj = ClassFactory::create_object("GameObject");
	c_debuger() << gobj->get_class_info()->get_class_name();
	*/

	// gen mesh txt
	/*QString path = "hhh.txt";
	if (!MeshTxtGen::gen_mesh_txt(path.toStdString(), MeshTxtGenType::CONE)) {
		c_debuger() << "cannot gen mesh txt";
	}*/
	
	// gen material txt
	/*QString path = "default_material.txt";
	MaterialData md("default", CVector3D(0.1f), CVector3D(0.7f), CVector3D(0.2f), 32.0f, "texture_default.txt", "texture_default.txt");
	if (!MaterialGen::gen_material_txt(path.toStdString(), md)) {
		c_debuger() << "cannot gen material txt";
	}
	*/

	// repair png
	//std::string str = "resources";
	//QtImageHelper::repair_png(str);

	// texture load test
	/*{
		std::string path = "resources/textures/wood.png";
		std::string path2 = "resources/textures/wood2.png";
		//TextureGen::gen_texture_txt(path, TextureGenType::THE_TWO, CColor(80, 80, 80, 255));
		int w, h, n;
		auto t_data = TextureLoader::load_texture_x(path, w, h, n);
		QtImageHelper::data_to_png(path2, t_data, w, h);
	}
	*/

	// change txt to png
	/*
	uint width, heigh;
	std::string path = "resources/textures/txt/texture_default.txt";
	QtImageHelper::text_to_png(path);
	*/

	/*
	{
		CMatrix4x4 t;
		t.rotate(45.0f, 1.0f, 0.0f, 0.0f);
		c_debuger() << "use zhoujiao " + t.to_string();

	}
	{
		CMatrix4x4 t;
		t.rotate_euler(45.0f, 0.0f, 0.0f);
		c_debuger() << "use euler " + t.to_string();
	}
	*/

	// test timemanager
	/*{
		c_debuger() << TimeManager_ins().cur_time_data().to_string();
		c_debuger() << std::to_string(TimeManager_ins().cur_time_msconds());
		c_debuger() << std::to_string(TimeManager_ins().cur_runtime_msconds());
	}*/
	

	MyGameManager gm;
	CMainWindow cmw;
	cmw.show();

	return a.exec();

}
