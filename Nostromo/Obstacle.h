#pragma once

#include "vector.h"
#include "Material.h"
#include "Matrix.h"
#include "RGBImage.hpp"
#include "Texture.h"

#include <vector>

using namespace std;

struct TerrainVertex {
	Vector pos;
	Vector normal;
	float u1, v1;
	float u2, v2;
};


class Obstacle {
public:
	Obstacle();
	~Obstacle();
	bool load(const char* obstacleWidth, const char* obstacleHeight, const Vector& startPos);
	void draw();

	void setLocation(const Vector& pos) {
		this->location = pos;
	}
	Vector& getLocation() {
		return this->location;
	}

protected:
	Vector location;

	Texture g_obstacleWidth_Tex;
	Texture g_obstacleHeight_Tex;
	const RGBImage* g_obstacleWidth_RGB;
	const RGBImage* g_obstacleHeight_RGB;

	GLuint m_VertexBuffer;
	GLuint m_IndexBuffer;

	int g_elementsToDraw;

	float g_obstacleWidth_RGB_width;
	float g_obstacleWidth_RGB_height;
	float g_obstacleHeight_RGB_width;
	float g_obstacleHeight_RGB_height;

	vector<float>MyObstacles = {};

};
