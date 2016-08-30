//
//  MyTerrain.cpp
//  Nostromo
//
//  Created by David Herzog on 30.08.16.
//  Copyright © 2016 HS Osnabrueck. All rights reserved.
//

#include "MyTerrain.hpp"


MyTerrain::MyTerrain() {
    g_theTerrain = new Model();
}

MyTerrain::~MyTerrain() {
    if (g_theTerrain) {
        delete g_theTerrain;
    }
    if (location) {
        delete location;
    }
}

void MyTerrain::load(const char* filename, const Vector& startPos, const char* vertexShader, const char* fragmentShader) {
    this->setStartPos(startPos);
    g_theTerrain->load(filename, vertexShader, fragmentShader);
}

void MyTerrain::parallax(float deltaTime, float speed) {
    Vector direction(0,0,-1);
    
    *location += direction * (deltaTime / speed);
    Matrix_translation.translation(*location);
    std::cout << "location terrain:" << location->X << ", " << location->Y << ", " << location->Z << std::endl;
}

void MyTerrain::draw() {
    glPushMatrix();
    glMultMatrixf(Matrix_translation);
    g_theTerrain->drawTriangles();
    glPopMatrix();
}

void MyTerrain::setStartPos(const Vector& startPos) {
    this->location = new Vector(startPos.X, startPos.Y, startPos.Z);
}