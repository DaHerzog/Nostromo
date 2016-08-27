#include "Spacecraft.h"

Spacecraft::Spacecraft() {}

Spacecraft::~Spacecraft() {}

bool Spacecraft::load(const char* shipModel, const Vector& StartPos, const char* VertexShader, const char* FragmentShader) {

	this->setLocation(StartPos);
	g_shipModel.load(shipModel,VertexShader,FragmentShader);
	return true;
}

bool movement_blocked;

void Spacecraft::navigate(int ForwardBackward, int LeftRight) {
	
	movement_leftRight = LeftRight;

	movement_upDown = ForwardBackward;
}


void Spacecraft::update_Movement(float DeltaTime, bool buttonPressed_leftRight, bool buttonPressed_upDown) {

	Vector movement;
	Vector rotation;
	Vector scaling;


	if (buttonPressed_leftRight) {

		if (g_movement_factor_leftRight > 0.0f && movement_leftRight != movement_leftRight_prev) {
			g_movement_factor_leftRight--;
			movement_leftRight = movement_leftRight_prev;
		} else {
			if (g_movement_factor_leftRight < g_movement_speedBorder) {
				g_movement_factor_leftRight++;
				movement_leftRight_prev = movement_leftRight;
			}
		}	
	} else {
		if (g_movement_factor_leftRight > 0.0f) {
			g_movement_factor_leftRight -= 0.5f;
			movement_leftRight = movement_leftRight_prev;
		}
	}

	if (buttonPressed_upDown) {
		if (g_movement_factor_upDown > 0.0f && movement_upDown != movement_upDown_prev) {
			g_movement_factor_upDown--;
			movement_upDown = movement_upDown_prev;
		}
		else {
			if (g_movement_factor_upDown < g_movement_speedBorder) {
				g_movement_factor_upDown++;
				movement_upDown_prev = movement_upDown;
			}
		}
	} else {
		if (g_movement_factor_upDown > 0.0f) {
			g_movement_factor_upDown -= 0.5f;
			movement_upDown = movement_upDown_prev;
		}
	}


	rotation.X = movement_leftRight * g_movement_factor_leftRight / 2000;
	rotation.Y = -movement_upDown * g_movement_factor_upDown / 1000;
	rotation.Z = movement_leftRight * g_movement_factor_leftRight / 1000;

	
	if (((location.X > g_border_right && movement_leftRight == 1) || (location.X < g_border_left && movement_leftRight == -1)) && g_movement_factor_leftRight > 0.0f) {
		g_movement_factor_leftRight -= 2.0f;
	}

	if (((location.Y > g_border_top && movement_upDown == 1) || (location.Y < g_border_bottom && movement_upDown == -1)) && g_movement_factor_upDown > 0.0f ) {
		g_movement_factor_upDown -= 2.0f;
	}
	

	movement.X = movement_leftRight * g_movement_factor_leftRight;
	movement.Y = movement_upDown * g_movement_factor_upDown;
	movement.Z = 0;



	scaling.X = scaling.Y = scaling.Z = 0.002f;

	location = movement * (DeltaTime / g_movement_accelerator) + location;

	Matrix_rotation.rotationYawPitchRoll(rotation);
	Matrix_translation.translation(location);
	Matrix_scaling.scale(scaling);

	Matrix_final =  Matrix_translation * Matrix_rotation * Matrix_scaling;

	//camera movement

}

void Spacecraft::update_Camera(float DeltaTime, bool buttonPressed_leftRight, bool buttonPressed_upDown) {
	g_Camera->pan(-location.X * 0.5, -location.Y * 0.5);
}

void Spacecraft:: draw() {
	glPushMatrix();
	glMultMatrixf(Matrix_final);
	g_shipModel.drawTriangles();
	glPopMatrix();
}