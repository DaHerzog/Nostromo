//
//  main.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 22.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//


#include <iostream>
#include <math.h>

#include "Camera.h"
#include "model.h"
#include "texture.h"
#include "vector.h"
#include "ShaderProgram.hpp"
#include "Spacecraft.h"
#include "Obstacle.h"
#include "GameManager.hpp"

// models
const char* g_Object_ship = "models//spaceship_own.obj";

//bitmaps
const char* g_obstacle_height = "Bitmap//courseWidth.bmp";
const char* g_obstacle_width = "Bitmap//courseHeight.bmp";

//shaders
const char* g_Shader_vertex_alpha = "Shader//Vertexshader_red.glsl";
const char* g_Shader_fragment_alpha = "Shader//Pixelshader_red.glsl";
const char* g_Shader_vertex_beta = "Shader//Vertexshader_blue.glsl";
const char* g_Shader_fragment_beta = "Shader//Pixelshader_blue.glsl";

//Pfade für Mac OS X
const char* g_shader_mac_vertex_red = "/Users/davidherzog/Documents/XCode/Nostromo/Nostromo/Vertexshader_red.glsl";
const char* g_shader_mac_fragment_red = "/Users/davidherzog/Documents/XCode/Nostromo/Nostromo/Pixelshader_red.glsl";
const char* g_object_ship_mac = "/Users/davidherzog/Documents/XCode/Nostromo/Nostromo/spaceship_own.obj";
const char* g_myTerrain_mac = "/Users/davidherzog/Documents/XCode/Nostromo/Nostromo/myTerrain.obj";
const char* g_shader_mac_vertex_blue = "/Users/davidherzog/Documents/XCode/Nostromo/Nostromo/Vertexshader_blue.glsl";
const char* g_shader_mac_fragment_blue = "/Users/davidherzog/Documents/XCode/Nostromo/Nostromo/Pixelshader_blue.glsl";




// window x and y size
const unsigned int g_WindowWidth=1024;
const unsigned int g_WindowHeight=768;

// positions
const Vector g_LightPos = Vector( 0,5,0);
const Vector g_LocalPos = Vector( 0,0,0);

Camera g_Camera;
Spacecraft g_ship;
Obstacle g_Model_obstacle;
GameManager g_GameManager;
Model g_myTerrainTest;

int g_MouseButton = 0;
int g_MouseState = 0;

//movement parameter
int Movement_leftright = 0;
int Movement_updown = 0;

//time for movement
bool buttonPressed_leftRight = false;
bool buttonPressed_upDown = false;
int Movement_elapsedTime;
int Movement_prevTime = 0;


void SetupDefaultGLSettings();
void DrawScene();
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);
void KeyboardCallback(unsigned char key, int x, int y);
void KeyboardUpCallback(unsigned char key, int x, int y);
void KeyboardArrowCallback(int key, int x, int y);
void KeyboardArrowUpCallback(int key, int x, int y);


int main(int argc, char * argv[]) {

    // initialize the glut system and create a window
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Nostromo");
#ifdef WIN32
	glewInit();
#endif

    SetupDefaultGLSettings();
    
    glutSpecialFunc(KeyboardArrowCallback);
    glutSpecialUpFunc(KeyboardArrowUpCallback);
    glutDisplayFunc(DrawScene);
    glutMouseFunc(MouseCallback);
    glutKeyboardFunc(KeyboardCallback);
	glutKeyboardUpFunc(KeyboardUpCallback);
    glutMotionFunc(MouseMoveCallback);

	//g_Model.applyMaterial("E://ComputerGrafik//PraktikumProjekte//praktikum_3//Computergrafik//models//sibenik//sibenik.mtl");


    g_ship.load(g_object_ship_mac, g_LocalPos, g_shader_mac_vertex_red, g_shader_mac_fragment_red);
    g_myTerrainTest.load(g_myTerrain_mac, g_shader_mac_vertex_blue, g_shader_mac_fragment_blue);
	g_ship.setCamera(&g_Camera);
    g_GameManager.setShip(&g_ship);
    g_GameManager.setCamera(&g_Camera);

	//g_Model_obstacle.load(g_obstacle_width, g_obstacle_height, g_LocalPos);
    
    glutMainLoop();
}


void SetupDefaultGLSettings() {

    glClearColor(0, 0, 0, 255);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (double)g_WindowWidth/(double)g_WindowHeight, 0.045f, 1000.0f);    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void DrawGroundGrid() {

    const float GridSize=10.0f;
    const unsigned int GridSegments=20;
    const float GridStep=GridSize/(float)GridSegments;
    const float GridOrigin=-GridSize*0.5f;
    
    glDisable( GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        for( unsigned int i=0; i<GridSegments+1; i++) {

            float itpos=GridOrigin + GridStep*(float)i;
            glVertex3f(itpos, GridOrigin, 0);
            glVertex3f(itpos, GridOrigin+GridSize, 0);
        
            glVertex3f( GridOrigin, itpos,0);
            glVertex3f( GridOrigin+GridSize, itpos,0);

        }
    glEnd();
    glEnable( GL_LIGHTING);

}

void MouseCallback(int Button, int State, int x, int y) {

    g_MouseButton = Button;
    g_MouseState = State;
    g_Camera.mouseInput(x,y,Button,State);
}

void MouseMoveCallback( int x, int y) {

    g_Camera.mouseInput(x,y,g_MouseButton,g_MouseState);
}

void KeyboardCallback( unsigned char key, int x, int y) {


	switch (key) {
        case 'w':
            g_GameManager.setMovementUpDown(1);
            g_GameManager.setButtonPressedUpDown(true);
            break;
        case 'a':
            g_GameManager.setMovementLeftRight(1);
            g_GameManager.setButtonPressedLeftRight(true);
            break;
        case 's':
            g_GameManager.setMovementUpDown(-1);
            g_GameManager.setButtonPressedUpDown(true);
            break;
        case 'd':
            g_GameManager.setMovementLeftRight(-1);
            g_GameManager.setButtonPressedLeftRight(true);
            break;
        default:
            break;
	}
}

void KeyboardArrowCallback(int key, int x, int y) {
    
    switch (key) {
        case GLUT_KEY_UP:
            g_GameManager.setMovementUpDown(1);
            g_GameManager.setButtonPressedUpDown(true);
            break;
        case GLUT_KEY_LEFT:
            g_GameManager.setMovementLeftRight(1);
            g_GameManager.setButtonPressedLeftRight(true);
            break;
        case GLUT_KEY_DOWN:
            g_GameManager.setMovementUpDown(-1);
            g_GameManager.setButtonPressedUpDown(true);
            break;
        case GLUT_KEY_RIGHT:
            g_GameManager.setMovementLeftRight(-1);
            g_GameManager.setButtonPressedLeftRight(true);
            break;
        default:
            break;
    }
    
}

void KeyboardUpCallback(unsigned char key, int x, int y) {

	/*buttonPressed_leftRight = false;
	buttonPressed_upDown = false;

	Movement_updown = 0;
	Movement_leftright = 0;*/
    
    g_GameManager.setMovementLeftRight(0);
    g_GameManager.setButtonPressedLeftRight(false);
    g_GameManager.setMovementUpDown(0);
    g_GameManager.setButtonPressedUpDown(false);
    
    
}

void KeyboardArrowUpCallback(int key, int x, int y) {
    g_GameManager.setMovementLeftRight(0);
    g_GameManager.setButtonPressedLeftRight(false);
    g_GameManager.setMovementUpDown(0);
    g_GameManager.setButtonPressedUpDown(false);
}

void DrawScene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    
    GLfloat lpos[4];
    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    
    g_GameManager.drawAll();
    DrawGroundGrid();
    g_myTerrainTest.drawTriangles();
    
    glutSwapBuffers();
    glutPostRedisplay();
    
}
