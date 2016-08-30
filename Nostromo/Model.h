//
//  Model.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Model__
#define __RealtimeRending__Model__

#ifdef WIN32
#include <Windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif

#include <iostream>
#include "vector.h"
#include "color.h"
#include <string>
#include <map>
#include <vector>
#include "material.h"
#include "ShaderProgram.hpp"

using namespace std;

struct Vertex
{
    Vertex();
    Vertex( const Vector& p, const Vector& n, float TexS, float TexT);
    Vector Position;
    Vector Normal;
    float  TexcoordS;
    float  TexcoordT;
};

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox( const Vector& min, const Vector& max);
    Vector Min;
    Vector Max;
};

struct TextureGroup {
	unsigned int size, texid;
};

class Model
{
public:
    Model();
    ~Model();
    const BoundingBox& boundingBox() const;
    bool load( const char* Filename, const char* VertexShader, const char* FragmentShader);
	void applyMaterial(const char* Filename);
    void drawTriangles() const;
protected:
	void createModel(const char* Filename);
    Material* m_pMaterials;
    unsigned int m_MaterialCount;
    Vertex* m_pVertices;
    unsigned int* m_pIndices;
    unsigned int m_VertexCount;
	TextureGroup* m_tGroups;
	unsigned int m_TextureGroupCount;
    BoundingBox m_Box;
	ShaderProgram m_ShaderProgram;
    GLuint m_VertexBuffer;
    GLuint m_IndexBuffer;

};

#endif /* defined(__RealtimeRending__Model__) */
