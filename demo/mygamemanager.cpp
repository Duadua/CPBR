#include "freecamera.h"
#include "inputmanager.h"
#include "assetmanager.h"
#include "mygamemanager.h"
#include "meshcomponent.h"
#include "lightobject.h"

#include "cubeobject.h"
#include "sphereobject.h"
#include "cylinderobject.h"

#include "plane.h"
#include "street.h"
#include "mansion.h"
#include "flowsphere.h"
#include "rotatecylinder.h"

void MyGameManager::load_asset() {
	// shader

	// mesh
	AssetManager::load_mesh_x("elf_mansion", "resources/models/obj/Elf_Mansion/Elf_Mansion.obj");
	AssetManager::load_mesh_x("street", "resources/models/obj/Street/Street_environment_V01.obj");
	//std::string path = "resources/models/obj/Elf_Mansion/Elf_Mansion.obj";

	// material
	AssetManager::load_materials("resources/materials/txt/cube_material.txt"); 

	// texture
	AssetManager::load_texture_x("resources/textures/wood.png");
	AssetManager::load_texture("resources/textures/txt/texture_solide_white.txt");
	AssetManager::load_texture("resources/textures/txt/texture_solide_gray.txt");
	AssetManager::load_texture("resources/textures/txt/texture_gradua_red.txt");

}

void MyGameManager::begin_play() {
	// 生成 plan
	{
		auto plane = CREATE_CLASS(PlaneObject);
		add_game_object("plane", plane);
		//plane->get_root_component()->set_scale(2.0f);
		plane->set_material("brickwall");
	}

	// 添加 城堡 mesh
	{
		auto mansion = CREATE_CLASS(Mansion);
		add_game_object("mansion", mansion);
		mansion->get_root_component()->set_location(3.0f, 5.0f, 15.0f);
		mansion->get_root_component()->set_rotation(0.0f, 180.0f, 0.0f);
	}
	{
		auto street = CREATE_CLASS(Street);
		add_game_object("street", street);
		street->get_root_component()->set_location(0.0f, 0.0f, -58.0f);
	}

	// 生成 cylinder -- test rb3d
	{
		for (int j = 0; j < 2; ++j) {
			for (int i = 0; i < 6; ++i) {
				{
					auto t_mo = CREATE_CLASS(RotateCylinder);
					std::string t_name = "cylinder" + int_to_string(j) + "_" + int_to_string(i);
					add_game_object(t_name, t_mo);
					t_mo->get_root_component()->set_location(2.0f*(j - 0.5f), 1.001f, 2.0f * (i - 3));
					t_mo->get_root_component()->set_scale(0.5f, 2.0f, 0.5f);
					t_mo->update_origin_rotation();
					if ((i+j)&1) t_mo->set_material("emerald");
					else t_mo->set_material("jade");
					t_mo->set_material("default", 2);
				}
				{
					auto t_mo = CREATE_CLASS(FlowSphere);
					std::string t_name = "sphere" + int_to_string(j) + "_" + int_to_string(i);
					add_game_object(t_name, t_mo);
					t_mo->get_root_component()->set_location(2.0f*(j - 0.5f), 2.35f, 2.0f * (i - 3));
					t_mo->get_root_component()->set_scale(0.3f);
					t_mo->update_origin_location();
					if ((i+j)&1) t_mo->set_material("jade");
					else t_mo->set_material("emerald");
				
				}
	
			}
		}
	}

	// 生成 gameobject
	/*
	{
		auto cube_object = CREATE_CLASS(GameObject);
		add_game_object("cube01", cube_object);
		// gameobject 的 root
		auto mc = CREATE_CLASS(MeshComponent);
		mc->set_mesh("cube");
		cube_object->set_root_component(mc);
		auto t_ms = AssetManager::get_mesh(mc->get_mesh());
		t_ms->get_render_datas()[0]->set_material_name("cube_wood");

		// gameobject 的另一个显示组件
		auto mcc = CREATE_CLASS(MeshComponent);
		mcc->set_mesh("cone");
		mcc->attach_to(mc);
		mcc->set_location(2.0f, 0.0f, 0.0f);
		mcc->set_scale(0.5f, 0.5f, 0.5f);
		//auto t_m = AssetManager::get_mesh(mcc->get_mesh());
		//t_m->get_render_datas()[0]->set_material_name("cube_wood");

		auto mccc = CREATE_CLASS(MeshComponent);
		mccc->set_mesh("cylinder");
		mccc->attach_to(mcc);
		mccc->set_location(0.0f, 3.0f, 0.0f);
		mccc->set_scale(1.5f, 1.5f, 1.5f);
		auto t_m = AssetManager::get_mesh(mccc->get_mesh());
		t_m->get_render_datas()[2]->set_material_name("");
	}*/

	// shader 静态参数赋值
    CMatrix4x4 projection;
    float ratio  = 1.0f * get_viewport_info().heigh / get_viewport_info().width;
    projection.perspective(45.0f, ratio, 0.1f, 1000.0f);
	if (main_shader != nullptr) {
		main_shader->use();
		main_shader->set_mat4("u_projection", projection);
		//main_shader->set_float("u_near", 0.1f);
		//main_shader->set_float("u_far", 100.0f);
	}

	auto t_s = AssetManager::get_shader("solid_color");
	if (t_s != nullptr) {
		t_s->use();
		t_s->set_mat4("u_projection", projection);
	}

	auto p_s = AssetManager::get_shader("pick");
	if (p_s != nullptr) {
		p_s->use();
		p_s->set_mat4("u_projection", projection);
	}
	

	// use direct light
	{
		auto d_light = CREATE_CLASS(DirectLightObject);
        d_light->use(main_shader->get_name());
	}

	// use point light
	{
		auto p_light = CREATE_CLASS(PointLightObject);
		p_light->get_light_component()->set_intensity(3.0f);
		//p_light->use(main_shader->get_name());
	}
	{
		auto p_light = CREATE_CLASS(PointLightObject);
		p_light->get_root_component()->set_location(3.0f, 6.0f, 15.0f);
		p_light->get_light_component()->set_att_radius(50.0f);
		p_light->get_light_component()->set_color(CVector3D(1.0f, 0.0f, 0.0f));
		p_light->use(main_shader->get_name());
	}
	
	// use spot light
	{
		auto s_light = CREATE_CLASS(SpotLightObject);
		s_light->get_root_component()->set_location(2.0f, 8.0f, 15.0f);
		s_light->get_root_component()->set_rotation(-45.0f, -90.0f, 0.0f);
		s_light->get_light_component()->set_color(CVector3D(0.0f, 0.0f, 1.0f));
		s_light->get_light_component()->set_intensity(10.0f);
		s_light->get_light_component()->set_inner(15.0f);
		s_light->get_light_component()->set_outer(20.0f);

		s_light->use(main_shader->get_name());
	}

	 //vr_delta = 0.05f;

}

void MyGameManager::tick() {

}

void MyGameManager::map_input() {
	InputState is;
	is.mouse_pressing = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_X;
	InputManager::map_axis("turn", is);
	is = InputState();
	is.mouse_released = Qt::RightButton;
	InputManager::map_action("turn_over", is);
	is = InputState();
	is.modifiers = Qt::ControlModifier;
	is.mouse_released = Qt::RightButton;
	InputManager::map_action("turn_over", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.axis_types = InputAxisType::MOUSE_X;
	InputManager::map_axis("turn", is);
	is = InputState();
	is.mouse_released = Qt::LeftButton;
	InputManager::map_action("turn_over", is);
	is = InputState();
	is.modifiers = Qt::ControlModifier;
	is.mouse_released = Qt::LeftButton;
	InputManager::map_action("turn_over", is);

	// look_up
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -1.0f;
	InputManager::map_axis("look_up", is);

	// move_forward
	is = InputState();
	is.axis_types = InputAxisType::WHEEL;
	is.axis_scale = 0.01f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_forward_plane", is);
	// key
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_W);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_W);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_W);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_S);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_S);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_S);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_forward", is);

	// move right
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.axis_types = InputAxisType::MOUSE_X;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_right", is);
	// key
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_D);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_D);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_D);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_A);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_A);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_A);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_right", is);

	// move up
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_up", is);
	// key
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_E);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_E);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_E);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_Q);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_Q);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_Q);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_up", is);
}


SPTR_Shader	MyGameManager::set_main_shader() {
	auto t_shader = AssetManager::get_shader("");
	if (t_shader == nullptr) { return GameManager::set_main_shader(); }
	t_shader->use();
	t_shader->set_vec4("u_solid_color", CVector4D(CColor(221, 161, 18).rgba_f()));
	t_shader->set_int("u_texture", 0);
	AssetManager::get_texture("texture_default.png")->bind();

	return t_shader;
}
