//
//  ShaderProgram.hpp
//  CGPrak6
//
//  Created by David Herzog on 08.06.16.
//  Copyright © 2016 HS Osnabrueck. All rights reserved.
//

#ifndef ShaderProgram_hpp
#define ShaderProgram_hpp

#include <stdio.h>
#include <string>
#include <GLUT/GLUT.h>
#include "Matrix.h"
#include "color.h"

class ShaderProgram {

public:
    ShaderProgram();
    ~ShaderProgram();
    bool load(const char* vertexShader, const char* fragmentShader);
    bool loadVertexShader(const char* vertexShader);
    bool loadFragmentShader(const char* fragmentShader);
    bool compile(std::string* compileError=NULL);
    
    GLint getParameterId(const char* parameterName) const;
    
    void setParameter(GLint id, float param);
    void setParameter(GLint id, int param);
    void setParameter(GLint id, const Vector& param);
    void setParameter(GLint id, const Color& param);
    void setParameter(GLint id, const Matrix& param);
    
    void activate();
    void deactivate();
    
    bool checkForError(std::string* compileError);

private:
    GLuint m_VertexShader;
    GLuint m_FragmentShader;
    GLuint m_ShaderProgram;

};

#endif /* ShaderProgram_hpp */
