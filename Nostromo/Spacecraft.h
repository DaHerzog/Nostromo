#pragma once

#include "vector.h"
#include "Model.h"
#include "Matrix.h"
#include "Camera.h"



class Spacecraft {
public:
	Spacecraft();
	~Spacecraft();
	bool load(const char* shipModel, const Vector& startPos, const char* VertexShader, const char* FragmentShader);
	void navigate(int ForwardBackward, int LeftRight);
	void update_Movement(float DeltaTime, bool buttonPressed_leftRight, bool buttonPressed_upDown);
	void update_Camera(float DeltaTime, bool buttonPressed_leftRight, bool buttonPressed_upDown);
	void draw();

	void setLocation(const Vector& pos) {
		this->location = pos;
	}
	Vector& getLocation() {
		return this->location;
	}
	void setMovementUpDown(const int value) {
		this->movement_upDown = value;
	}
	int getMovementUpDown() {
		return this->movement_upDown;
	}
	void setMovementLeftRight(const int value) {
		this->movement_leftRight = value;
	}
	int getMovementLeftRight() {
		return this->movement_leftRight;
	}
	Model& getShipModel() {
		return this->g_shipModel;
	}
	void setShipModel(const Model& model) {
		this->g_shipModel = model;
	}

	void setCamera(Camera* camera) {
		this->g_Camera = camera;
	}

private:
	Vector location;
	Model g_shipModel;
	Camera* g_Camera;
	int movement_upDown;
	int movement_leftRight;
	int movement_upDown_prev;
	int movement_leftRight_prev;

	//Matrix movements
	Matrix Matrix_final;
	Matrix Matrix_translation;
	Matrix Matrix_rotation;
	Matrix Matrix_scaling;

	float g_movement_factor_leftRight = 0.0f;
	float g_movement_factor_upDown = 0.0f;
	float g_movement_accelerator = 5000.0f;
	float g_movement_speedBorder = 1000.0f;

	float g_border_left = -4.0f;
	float g_border_right = 4.0f;
	float g_border_bottom = -4.0f;
	float g_border_top = 4.0f;
};