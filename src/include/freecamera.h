#pragma once

#include "cameraobject.h"

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

class FreeCamera : public CameraObject {
	DECLARE_CLASS(FreeCamera)
public:
	FreeCamera();
	~FreeCamera();

	virtual void bind_input() override;

protected:

	void turn(float offset);
	void turn_over();							// �ƶ���������ʾ���
	void look_up(float offset);

	void move_forward(float offset);
	void move_forward_plane(float offset);
	void move_right(float offset);
	void move_up(float offset);

};
DECLARE_AUTO_PTR(FreeCamera)