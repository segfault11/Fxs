//
//  main.c
//  Image
//
//  Created by Arno in Wolde Luebke on 18.04.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include "Image.h"

int main(int argc, const char * argv[])
{
    FxsImagePtr img = NULL;
    unsigned char color[3] = {255, 0, 0};
    int i = 0;
    
    img = FxsImageCreateWithFilename("SamuraiChamploo.png");
    
    if (!img) {
        printf("failed\n");
    }
    
    if (!FxsImageSave(img, "Test.bmp")) {
        printf("save failed\n");
    }
    
    if (!FxsImageSave(img, "Test.png")) {
        printf("save failed\n");
    }
    
    if (!FxsImageSave(img, "Test.tga")) {
        printf("save failed\n");
    }
    
    for (i = 0; i < 200; i++) {
        FxsImageSet(img, i, 100, color);
    }
    
   for (i = 0; i < 200; i++) {
        FxsImageSet(img, 100, i, color);
    }
    
    if (!FxsImageSave(img, "Test2.bmp")) {
        printf("save failed\n");
    }
    
    FxsImageDestroy(&img);

    return 0;
}

