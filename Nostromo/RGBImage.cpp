//
//  RGBImage.cpp
//  CGPrak1
//
//  Created by David Herzog on 22.03.16.
//  Copyright © 2016 David Herzog. All rights reserved.
//

#include "RGBImage.hpp"

RGBImage::RGBImage(unsigned int Width, unsigned int Height) {
    this->m_Width = Width;
    this->m_Height = Height;
    this->m_Image = new Color*[Width];
    for (int i = 0; i < Width; i++) {
        this->m_Image[i] = new Color[Height];
    }
    for (int i = 0; i < Width; i++) {
        for (int j = 0; j < Height; j++) {
            this->m_Image[i][j] = Color();
        }
    }
    
}

RGBImage::~RGBImage() {
    for (int i = 0; i < this->m_Width; i++) {
        delete [] this->m_Image[i];
        //std::cout << "LÖSCH Height" << std::endl;
    }
    //std::cout << "LÖSCH Width" << std::endl;
    delete [] this->m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c) {
    
    if (x >= this->m_Width) {
        x = this->m_Width-1;
    }
    
    y = this->m_Height-y-1;
    
    this->m_Image[x][y].R = c.R;
    this->m_Image[x][y].G = c.G;
    this->m_Image[x][y].B = c.B;
    
    /*std::cout << "R: " << this->m_Image[x][y].R << " G: " << this->m_Image[x][y].G << " B: " << this->m_Image[x][y].B << std::endl;*/
}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned int y) const {
    if (x >= this->m_Width) {
        x = this->m_Width-1;
    }
    
    y = this->m_Height-y-1;
    
   
    
    /*std::cout << "R: " << this->m_Image[x][y].R << " G: " << this->m_Image[x][y].G << " B: " << this->m_Image[x][y].B << std::endl;*/
    return this->m_Image[x][y];
}

bool RGBImage::saveToDisk(const char* Filename) {
   
    FILE* bmpFile = fopen(Filename, "w+b");
/*
    BITMAPINFOHEADER bmpInfoHeader;
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = this->width();
    bmpInfoHeader.biHeight = this->height();
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24;
    bmpInfoHeader.biCompression = 0;
    bmpInfoHeader.biSizeImage = 0;
    bmpInfoHeader.biXPelsPerMeter = 0;
    bmpInfoHeader.biYPelsPerMeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;
    
    BITMAPFILEHEADER bmpFileHeader;
    bmpFileHeader.bfType = 0x4D42;
    bmpFileHeader.bfSize = this->height() * this->width() * (bmpInfoHeader.biBitCount/8) + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;
    bmpFileHeader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
    
    fwrite((char*)&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
    fwrite((char*)&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFile);
*/
    
    //Stackoverflow anfang
    unsigned char file[14] = {
        'B','M', // magic
        0,0,0,0, // size in bytes
        0,0, // app data
        0,0, // app data
        40+14,0,0,0 // start of data offset
    };
    
    unsigned char info[40] = {
        40,0,0,0, // info hd size
        0,0,0,0, // width
        0,0,0,0, // heigth
        1,0, // number color planes
        24,0, // bits per pixel
        0,0,0,0, // compression is none
        0,0,0,0, // image bits size
        0,0,0,0, // horz resoluition in pixel / m
        0,0,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
        0,0,0,0, // #colors in pallete
        0,0,0,0, // #important colors
    };
    
    file[ 2] = (unsigned char)( this->height() * this->width() * (24/8) + sizeof(file) + sizeof(info)    );
    file[ 3] = (unsigned char)( (this->height() * this->width() * (24/8) + sizeof(file) + sizeof(info))>> 8);
    file[ 4] = (unsigned char)( (this->height() * this->width() * (24/8) + sizeof(file) + sizeof(info))>>16);
    file[ 5] = (unsigned char)( (this->height() * this->width() * (24/8) + sizeof(file) + sizeof(info))>>24);
    
    info[ 4] = (unsigned char)( this->width()   );
    info[ 5] = (unsigned char)( this->width()>> 8);
    info[ 6] = (unsigned char)( this->width()>>16);
    info[ 7] = (unsigned char)( this->width()>>24);
    
    info[ 8] = (unsigned char)( this->height()    );
    info[ 9] = (unsigned char)( this->height()>> 8);
    info[10] = (unsigned char)( this->height()>>16);
    info[11] = (unsigned char)( this->height()>>24);
    //Stackoverflow ende
    
    
    fwrite((char*)file, sizeof(file), 1, bmpFile);
    fwrite((char*)info, sizeof(info), 1, bmpFile);
    
    //Erst Blau, dann Grün, dann Rot (bei 24bpp)
    //Jede Zeile muss mit Nullen aufgefüllt werden, sodass sie durch 4 teilbar ist! (bzw '\0')
    
    int offset = this->width()*24/8;
    offset %= 4;
    char lueckenfueller = 0;
    
    for (int i = this->height()-1; i >= 0; i--) {
        for (int j = 0; j < this->width(); j++) {
            Color tmpCol = this->getPixelColor(j, i);
 
            unsigned char blue = this->convertColorChannel(tmpCol.B);
            unsigned char green = this->convertColorChannel(tmpCol.G);
            unsigned char red = this->convertColorChannel(tmpCol.R);
            
    
            fwrite((unsigned char*)&blue, sizeof(unsigned char), 1, bmpFile);
            fwrite((unsigned char*)&green, sizeof(unsigned char), 1, bmpFile);
            fwrite((unsigned char*)&red, sizeof(unsigned char), 1, bmpFile);
            
        }
        for (int k = 0; k < offset; k++) {
            fwrite(&lueckenfueller, 1, 1, bmpFile);
        }
    }
    
    fclose(bmpFile);
    
    return true;
}

unsigned int RGBImage::width() const {
    return this->m_Width;
}

unsigned int RGBImage::height() const {
    return this->m_Height;
}

unsigned char RGBImage::convertColorChannel(float f) {
    /*std::cout << "Übergabe foat: " << f << std::endl;*/
    unsigned char colorChannel = 255;
    if (f < 0.0f) {
        f = 0.0f;
    } else if (f > 1.0f) {
        f = 1.0f;
    }
    colorChannel = colorChannel * f;
    return colorChannel;
    
}