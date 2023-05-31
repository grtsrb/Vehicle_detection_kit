
#include "Augmentations.h"

void myAugmentation0(
	uchar in_out_y[],
	int x_size,
	int y_size
) {
	
}


void adjustExposure(
    const uchar in_rgb[],
    uchar out_rgb[],
	int x_size,
	int y_size,
    float exposure
) {
    float pixel;
    for (int i = 0; i < x_size * y_size * 3; i += 3) {
        pixel = in_rgb[i] * exposure;
        if (pixel > 255) pixel = 255;
        if (pixel < 0) pixel = 0;
        out_rgb[i] = (uchar)pixel;

        pixel = in_rgb[i+1] * exposure;
        if (pixel > 255) pixel = 255;
        if (pixel < 0) pixel = 0;
        out_rgb[i+1] = (uchar)pixel;

        pixel = in_rgb[i+2] * exposure;
        if (pixel > 255) pixel = 255;
        if (pixel < 0) pixel = 0;
        out_rgb[i+2] = (uchar)pixel;
    }
 }

void applyOcclusion(
    const uchar in_rgb[],
    uchar out_rgb[],
    int x_size,
    int y_size,
    int start_x,
    int start_y,
    int end_x,
    int end_y,
    int bytesPerLine,
    int num_of_occlusions
){
    // Copy input image to output image.
    memcpy(out_rgb, in_rgb, x_size*y_size*3*sizeof(uchar));
    int xbox_size = (end_x - start_x)/3;
    int ybox_size = (end_y - start_y)/3;
    srand(time(0));

    // Apply occlusion.
    for (int y = start_y + ybox_size; y < end_y - ybox_size; y++) {
        for (int x = start_x + xbox_size; x < end_x - xbox_size; x++) {
            int bytes_per_pixel = 3;
            int row_stride = bytesPerLine;
            int pixel_index = y * row_stride + x * bytes_per_pixel;
            out_rgb[pixel_index + 0] = 0;
            out_rgb[pixel_index + 1] = 0;
            out_rgb[pixel_index + 2] = 0;
        }
    }
    int counter = 0;

    while(counter != num_of_occlusions)
    {
    int randx = rand() % x_size;

    if (randx > x_size/2)
    {
        end_x = randx;
        start_x = end_x - (end_x - start_x)/4;
        end_y = end_x - (end_y - start_y)/4;
        start_y = end_y - (end_y - start_y)/4;
    } else
    {
        start_x = randx;
        end_x = start_x + (end_x - start_x)/4;
        start_y = start_x + (end_y - start_y)/4;
        end_y = start_y + (end_y - start_y)/4;
    }

    // Apply occlusion.
    for (int y = start_y; y < end_y; y++) {
        for (int x = start_x; x < end_x; x++) {
            int bytes_per_pixel = 3; // assuming 8-bit RGB format
            int row_stride = bytesPerLine;
            int pixel_index = y * row_stride + x * bytes_per_pixel;
            out_rgb[pixel_index + 0] = 0;
            out_rgb[pixel_index + 1] = 0;
            out_rgb[pixel_index + 2] = 0;
        }
    }
    counter++;
    }
}

void aspectRationCrop(
    const uchar in_rgb[],
    uchar out_rgb[],
    int x_size,
    int y_size,
    int n_x,
    int n_y
){
    // Calculate the offset of input in order to center it with output
    int x_offset = (x_size - n_x)/2;
    int y_offset = (y_size - n_y)/2;
    for (int y = 0; y < n_y; y++) {
        for (int x = 0; x < n_x; x++) {
            // Calculate the correspoding pixel of input image
            int inputIndex = ((y + y_offset) * x_size + (x + x_offset)) * 3;
            // Calculate the corresponding pixel of output image
            int outputIndex = (y * n_x + x) * 3;
            out_rgb[outputIndex] = in_rgb[inputIndex];
            out_rgb[outputIndex + 1] = in_rgb[inputIndex + 1];
            out_rgb[outputIndex + 2] = in_rgb[inputIndex + 2];
        }
    }

}

void equalizeHistogram(
    const uchar input[],
    int x_size,
    int y_size,
    int test,
    uchar output[],
    uchar L
) {
    int histogram[256][3]; // Separate histograms for each channel
    int cdf[256][3]; // Separate cumulative histograms for each channel

    int bytes_per_pixel = 3; // assuming 8-bit RGB format
    int row_stride = static_cast<int>(ceil(test / 4.0)) * 4;



    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 3; ++j) {
            histogram[i][j] = 0;
            cdf[i][j] = 0;
        }
    }


    // Compute histograms
    for (int y = 0; y < y_size; y++){
        for (int x = 0; x < x_size; x++){
            int pixel_index = y * row_stride + x * bytes_per_pixel;
            histogram[input[pixel_index + 0]][0]++;
            histogram[input[pixel_index + 1]][1]++;
            histogram[input[pixel_index + 2]][2]++;
        }
    }

    // Compute cumulative histograms
    for (int j = 0; j < 3; ++j) {
        int pom = 0;
        for (int i = 0; i < 256; ++i) {
            pom += histogram[i][j];
            cdf[i][j] = pom;
        }
    }

    // Find minimum non-zero CDF value
    int cdf_min[3];
    for (int j = 0; j < 3; ++j) {
        cdf_min[j] = 0;
        for (int i = 0; i < 256; ++i) {
            if (cdf[i][j] > 0) {
                cdf_min[j] = cdf[i][j];
                break;
            }
        }
    }

    // Perform histogram equalization
    for (int y = 0; y < y_size; y++){
        for (int x = 0; x < x_size; x++){
            int pixel_index = y * row_stride + x * bytes_per_pixel;
            output[pixel_index + 0] = static_cast<uchar>((cdf[input[pixel_index + 0]][0] - cdf_min[0]) * (L - 1) / (x_size * y_size - cdf_min[0]));
            output[pixel_index + 1] = static_cast<uchar>((cdf[input[pixel_index + 1]][1] - cdf_min[1]) * (L - 1) / (x_size * y_size - cdf_min[1]));
            output[pixel_index + 2] = static_cast<uchar>((cdf[input[pixel_index + 2]][2] - cdf_min[2]) * (L - 1) / (x_size * y_size - cdf_min[2]));
        }
    }
}
