
#include "Augmentations.h"

void myAugmentation0(
	uchar in_out_y[],
	int x_size,
	int y_size
) {
	
}


void adjustExposure(
    const uchar in_out_rgb[],
    uchar out_rbg[],
	int x_size,
	int y_size,
    float exposure
) {
    float pixel;
    for (int i = 0; i < x_size * y_size * 3; i += 3) {
        pixel = in_out_rgb[i] * exposure;
        if (pixel > 255) pixel = 255;
        if (pixel < 0) pixel = 0;
        out_rbg[i] = (uchar)pixel;

        pixel = in_out_rgb[i+1] * exposure;
        if (pixel > 255) pixel = 255;
        if (pixel < 0) pixel = 0;
        out_rbg[i+1] = (uchar)pixel;

        pixel = in_out_rgb[i+2] * exposure;
        if (pixel > 255) pixel = 255;
        if (pixel < 0) pixel = 0;
        out_rbg[i+2] = (uchar)pixel;
    }
 }

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
){
    qDebug() << "start_x = " << start_x;
    qDebug() << "start_y = " << start_y;
    qDebug() << "end_x = " << end_x;
    qDebug() << "end_y = " << end_y;
    qDebug() << "x_size = " << x_size;
    qDebug() << "y_size = " << y_size;
    // Copy input image to output image.
    memcpy(out_rbg, in_rgb, x_size*y_size*3*sizeof(uchar));
    int xbox_size = (end_x - start_x)/4;
    int ybox_size = (end_y - start_y)/4;
    srand(time(0));
    int brojac = 0;


    // Apply occlusion.
    for (int y = start_y + ybox_size; y < end_y - ybox_size; y++) {
        for (int x = start_x + xbox_size; x < end_x - xbox_size; x++) {
            int bytes_per_pixel = 3; // assuming 8-bit RGB format
            int row_stride = test;
            int pixel_index = y * row_stride + x * bytes_per_pixel;
            out_rbg[pixel_index + 0] = 0;
            out_rbg[pixel_index + 1] = 0;
            out_rbg[pixel_index + 2] = 0;
        }
    }

    while(brojac != 3)
    {
    int randx = rand() % x_size;

    if (randx > x_size/2)
    {
        //todo
        end_x = randx;
        start_x = end_x - (end_x - start_x)/4;
        end_y = end_x + (end_y - start_y)/4;
        start_y = end_y - (end_y - start_y)/4;
    } else
    {
        //todo
        start_x = randx;
        end_x = start_x + (end_x - start_x)/4;
        start_y = start_x + (end_y - start_y)/4;
        end_y = start_y + (end_y - start_y)/4;
    }

    // Apply occlusion.
    for (int y = start_y; y < end_y; y++) {
        for (int x = start_x; x < end_x; x++) {
            int bytes_per_pixel = 3; // assuming 8-bit RGB format
            int row_stride = test;
            int pixel_index = y * row_stride + x * bytes_per_pixel;
            out_rbg[pixel_index + 0] = 0;
            out_rbg[pixel_index + 1] = 0;
            out_rbg[pixel_index + 2] = 0;
        }
    }
    brojac++;
    }
}

void aspectRationCrop(
    const uchar in_rgb[],
    uchar out_rbg[],
    int x_size,
    int y_size,
    int n_x,
    int n_y
){
    int x_offset = (x_size - n_x)/2;
    int y_offset = (y_size - n_y)/2;
    for (int y = 0; y < n_y; y++) {
        for (int x = 0; x < n_x; x++) {
            int inputIndex = ((y + y_offset) * x_size + (x + x_offset)) * 3;
            int outputIndex = (y * n_x + x) * 3;
            out_rbg[outputIndex] = in_rgb[inputIndex];
            out_rbg[outputIndex + 1] = in_rgb[inputIndex + 1];
            out_rbg[outputIndex + 2] = in_rgb[inputIndex + 2];
        }
    }

}
