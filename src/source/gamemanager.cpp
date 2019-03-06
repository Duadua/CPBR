#include "gameobject.h"
#include "freecamera.h"
#include "gamemanager.h"
#include "assetmanager.h"
#include "inputmanager.h"
#include <assert.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLWidget>
 
void GameManager::init(QOpenGLWidget* gl) {

	// init gl_core
	core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

	// load default asset
	AssetManager::load_materials("resources/materials/txt/default_material.txt");
	AssetManager::load_texture("resources/textures/txt/texture_default.txt");
	AssetManager::load_texture("resources/textures/txt/texture_default.png");

	load_asset();						// 加载资源
	begin_play(gl);						// 设置模型等
	main_camera = set_main_camera();	// 绑定主相机

	main_begin_play();		

	map_input();						// 绑定输入
	main_bind_input();

	// gl 状态初始化
	background_color = CColor(205, 220, 232);
	set_depth_test();

	//core->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	core->glPolygonMode(GL_BACK, GL_LINE);
	//core->glEnable(GL_CULL_FACE);
}
void GameManager::draw(QOpenGLWidget* gl) {

	// exec input
	{
		InputManager::exec_action();
		InputManager::exec_axis_key_pressing();
	}

	// update
	{
		tick();
		main_tick();
	}
	
	// draw
	{
		// gl init
		core->glClearColor(background_color.r_f(), background_color.g_f(), background_color.b_f(), background_color.a_f());
		core->glClear(GL_COLOR_BUFFER_BIT);
		if (b_depth_test) { core->glClear(GL_DEPTH_BUFFER_BIT); }

		main_shader = "triangle";
		auto t_shader = AssetManager::get_shader(main_shader)->use();
		t_shader->set_mat4("u_view", main_camera->get_view_mat());
		t_shader->set_vec3("u_view_pos", main_camera->get_location());

		// render
		main_draw();
	}
}

void GameManager::pre_init(QOpenGLWidget* gl) {

	// init gl mouse cursor
	gl->setCursor(Qt::CrossCursor);
	gl->setFocusPolicy(Qt::StrongFocus);

	// init input manager
	InputManager::init(gl);
}
void GameManager::resize(QOpenGLWidget* gl, int w, int h) {
	core->glViewport(0, 0, w, h);

	CMatrix4x4 projection;
	float t_ratio = (float) gl->width() / gl->height();
	projection.perspective(45.0f, t_ratio, 0.1f, 100.0f);
	//projection.frustum(-0.1f, 0.1f, -0.1f / t, 0.1f / t, 0.1f, 100.0f);
	//projection.ortho(-10.0f, 10.0f, -10.0f / t, 10.0f / t, 0.01f, 1000.0f);
	//projection.ortho(20.0f, t, 0.01f, 100.0f);
	//projection.ortho_2d(0.0f, 20.0f, 0.0f / t, 20.0f /t);

	auto t_shader = AssetManager::get_shader("triangle")->use();
	t_shader->set_mat4("u_projection", projection);
}
void GameManager::exit(QOpenGLWidget* gl) {
	InputManager::exit();
}

SPTR_CameraComponent GameManager::set_main_camera() {
	auto free_camera = CREATE_CLASS(FreeCamera);
	add_game_object("free_camera", free_camera);
	free_camera->get_root_component()->set_location(0.0f, 1.5f, -10.0f);
	free_camera->get_root_component()->set_rotation(0.0f, 0.0f, 0.0f);

	return free_camera->get_camera_component();
}

void GameManager::set_depth_test(bool enable, uint depth_mask, uint depth_func) {
	if (enable) { core->glEnable(GL_DEPTH_TEST); b_depth_test = true; }
	else { core->glDisable(GL_DEPTH_TEST); b_depth_test = false; }

	core->glDepthMask(depth_mask);
	core->glDepthFunc(depth_func);
}

// ===========================================================================

GameManager* GameManager::instance = nullptr;
GameManager::GameManager() {
	assert(instance == nullptr);
	instance = this;
	game_objects.clear();
}
GameManager::~GameManager() {
	assert(instance == this);
	instance = nullptr;
}
GameManager* GameManager::get_instance() {
	assert(instance);
	return instance;

}

void GameManager::add_game_object(const QString& key, SPTR_GameObject value) {
	if (game_objects.count(key)) return;
	game_objects.insert(key, value);
}

void GameManager::main_bind_input() {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it)->bind_input();
	}
}
void GameManager::main_begin_play() {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it)->begin_play();
	}
}
void GameManager::main_tick() {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it)->tick();
	}
}
void GameManager::main_draw() {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it)->draw(main_shader);
	}
}

void GameManager::exec_mouse_wheeeel_event(QWheelEvent* event, QOpenGLWidget* gl) { InputManager::exec_mouse_wheeeel_event(event, gl); }
void GameManager::exec_mouse_moveeee_event(QMouseEvent* event, QOpenGLWidget* gl) { InputManager::exec_mouse_moveeee_event(event, gl); }
void GameManager::exec_mouse_pressed_event(QMouseEvent* event, QOpenGLWidget* gl) { InputManager::exec_mouse_pressed_event(event, gl); }
void GameManager::exec_mouse_release_event(QMouseEvent* event, QOpenGLWidget* gl) { InputManager::exec_mouse_release_event(event, gl); }
void GameManager::mouse_pressed_over() { InputManager::mouse_pressed_over(); }

void GameManager::exec_key_pressed_event(QKeyEvent* event, QOpenGLWidget* gl)	{ InputManager::exec_key_pressed_event(event, gl); }
void GameManager::exec_key_release_event(QKeyEvent* event, QOpenGLWidget* gl)	{ InputManager::exec_key_release_event(event, gl); }
void GameManager::key_pressed_over()											{ InputManager::key_pressed_over(); }

