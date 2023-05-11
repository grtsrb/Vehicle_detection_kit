
#pragma once

#include <QDebug>
#include <math.h>

void myAugmentation0(
	uchar in_out_y[],
	int x_size,
	int y_size
);

void adjustExposure(
    const uchar in_out_rgb[],
    uchar out_rbg[],
    int x_size,
	int y_size,
    float exposure
);
void applyOcclusion(
    const uchar in_rgb[],
    uchar out_rbg[],
    int x_size,
    int y_size,
    int start_x,
    int start_y,
    int end_x,
    int end_y,
    int test
);
void aspectRationCrop(
    const uchar in_rgb[],
    uchar out_rbg[],
    int x_size,
    int y_size,
    int n_x,
    int n_y
);
