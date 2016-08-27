//
//  RGBImage.hpp
//  CGPrak1
//
//  Created by David Herzog on 22.03.16.
//  Copyright © 2016 David Herzog. All rights reserved.
//

#ifndef RGBImage_hpp
#define RGBImage_hpp

#include <stdio.h>
#include <iostream>
#include "color.h"

struct BITMAPFILEHEADER {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
};

struct BITMAPINFOHEADER {
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned int biXPelsPerMeter;
    unsigned int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};

class RGBImage {
public:
    RGBImage(unsigned int Width, unsigned int Height);
    ~RGBImage();
    void setPixelColor(unsigned int x, unsigned int y, const Color& c);
    const Color& getPixelColor(unsigned intx, unsigned int y) const;
    bool saveToDisk(const char* Filename);
    unsigned int width() const;
    unsigned int height() const;
    
    static unsigned char convertColorChannel(float f);

protected:
    Color** m_Image;
    unsigned int m_Height;
    unsigned int m_Width;
};

#endif /* RGBImage_hpp */
