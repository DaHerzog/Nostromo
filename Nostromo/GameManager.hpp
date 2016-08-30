//
//  GameManager.hpp
//  Nostromo
//
//  Created by David Herzog on 30.08.16.
//  Copyright © 2016 HS Osnabrueck. All rights reserved.
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <stdio.h>

#include "Spacecraft.h"
#include "MyTerrain.hpp"

class GameManager {
    
public:
    GameManager();
    void drawAll();
    void updatePositions();
    
    
    void setShip(Spacecraft *pShip);
    Spacecraft& getShip();
    void setCamera(Camera *pCamera);
    Camera& getCamera();
    void setTerrain(MyTerrain *pTerrain);
    void setMovementUpDown(int pMov);
    int getMovementUpDown();
    void setMovementLeftRight(int pMov);
    int getMovementLeftRight();
    void setButtonPressedLeftRight(bool pBool);
    bool isButtonPressedLeftRight();
    void setButtonPressedUpDown(bool pBool);
    bool isButtonPressedUpDown();
    
private:
    Spacecraft* g_ship;
    Camera* g_Camera;
    MyTerrain* g_terrain;
    
    int Movement_elapsedTime;
    int Movement_prevTime;
    int Movement_leftright;
    int Movement_updown;
    bool buttonPressed_leftRight;
    bool buttonPressed_upDown;
    float paralaxSpeed;
};

#endif /* GameManager_hpp */
