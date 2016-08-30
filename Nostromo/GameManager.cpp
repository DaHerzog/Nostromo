//
//  GameManager.cpp
//  Nostromo
//
//  Created by David Herzog on 30.08.16.
//  Copyright © 2016 HS Osnabrueck. All rights reserved.
//

#include "GameManager.hpp"

GameManager::GameManager() {
    this->Movement_prevTime = 0;
    this->Movement_leftright = 0;
    this->Movement_updown = 0;
    this->buttonPressed_leftRight = false;
    this->buttonPressed_upDown = false;;
}

void GameManager::drawAll() {
    this->Movement_elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    
    this->g_Camera->apply();
    this->g_ship->navigate(this->Movement_updown, this->Movement_leftright);
    
    this->g_ship->update_Movement(Movement_elapsedTime - Movement_prevTime, this->buttonPressed_leftRight, this->buttonPressed_upDown);
    this->g_ship->update_Camera(Movement_elapsedTime - Movement_prevTime, this->buttonPressed_leftRight, this->buttonPressed_upDown);
    
    this->Movement_prevTime = Movement_elapsedTime;
    
    this->g_ship->draw();
}

void GameManager::updatePositions() {
    
}

Spacecraft& GameManager::getShip() {
    return *(this->g_ship);
}

void GameManager::setShip(Spacecraft *pShip) {
    this->g_ship = pShip;
}

void GameManager::setCamera(Camera *pCamera) {
    this->g_Camera = pCamera;
}

Camera& GameManager::getCamera() {
    return *(this->g_Camera);
}

void GameManager::setMovementUpDown(int pMov) {
    this->Movement_updown = pMov;
}

int GameManager::getMovementUpDown() {
    return this->Movement_updown;
}

void GameManager::setMovementLeftRight(int pMov) {
    this->Movement_leftright = pMov;
}

int GameManager::getMovementLeftRight() {
    return this->Movement_leftright;
}

void GameManager::setButtonPressedLeftRight(bool pBool) {
    this->buttonPressed_leftRight = pBool;
}

bool GameManager::isButtonPressedLeftRight() {
    return this->buttonPressed_leftRight;
}

void GameManager::setButtonPressedUpDown(bool pBool) {
    this->buttonPressed_upDown = pBool;
}

bool GameManager::isButtonPressedUpDown() {
    return this->buttonPressed_upDown;
}
