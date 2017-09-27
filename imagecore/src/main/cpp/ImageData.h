//
// Created by Duke
//

#ifndef UNREALIMAGE_IMAGEDATA_H
#define UNREALIMAGE_IMAGEDATA_H
#include <android/bitmap.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} ui_argb;


typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} ui_rgb;

#endif //UNREALIMAGE_IMAGEDATA_H
