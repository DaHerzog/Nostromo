//
//  MyTerrain.hpp
//  Nostromo
//
//  Created by David Herzog on 30.08.16.
//  Copyright © 2016 HS Osnabrueck. All rights reserved.
//

#ifndef MyTerrain_hpp
#define MyTerrain_hpp

#include <stdio.h>

#include "Model.h"
#include "Matrix.h"

#endif /* MyTerrain_hpp */

class MyTerrain {

public:
    MyTerrain();
    ~MyTerrain();
    
    void load(const char* filename, const Vector& startPos, const char* vertexShader, const char* fragmentShader);
    void parallax(float deltaTime, float speed);
    void draw();
    void setStartPos(const Vector& startPos);
private:
    Vector* location;
    Model* g_theTerrain;
    Matrix Matrix_final;
    Matrix Matrix_translation;
};
