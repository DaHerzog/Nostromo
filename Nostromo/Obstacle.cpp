#include "Obstacle.h"

#include <vector>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


Obstacle::Obstacle() {}

Obstacle::~Obstacle() {}

bool Obstacle::load(const char* obstacleWidth, const char* obstacleHeight, const Vector& startPos) {


	this->setLocation(startPos);


	g_obstacleWidth_Tex.LoadFromBMP(obstacleWidth);
	g_obstacleHeight_Tex.LoadFromBMP(obstacleHeight);

	g_obstacleWidth_RGB = g_obstacleWidth_Tex.getRGBImage();
	g_obstacleHeight_RGB = g_obstacleHeight_Tex.getRGBImage();

	g_obstacleWidth_RGB_height = g_obstacleWidth_RGB->height();
	g_obstacleWidth_RGB_width = g_obstacleWidth_RGB->width();

	//height auslesen und im vector speichern
	for (unsigned int k = 0; k < g_obstacleWidth_RGB_height; k++) {
		for (unsigned int l = 0; l < g_obstacleWidth_RGB_width; l++) {
			MyObstacles.push_back(g_obstacleWidth_RGB->getPixelColor(l, k).R);
		}
	}


	return true;
}

void Obstacle::draw() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// setup position & normal pointers
	glVertexPointer(3, GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(0));
	glNormalPointer(GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(12));

	// we draw our plane

	glDrawElements(GL_TRIANGLES, g_elementsToDraw, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	// disable states in reverse order
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glClientActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // set modulate as default behaviour for unit 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
