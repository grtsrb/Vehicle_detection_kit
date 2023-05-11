
#include "ImageProcessing.h"
#include "ColorSpaces.h"
#include "Augmentations.h"

#include <QDebug>

void imageProcessingFun(
	const QString& prog_name,
	QImage& out_img,
	const QImage& in_img,
	const QVector<double>& params
) {
	int x_size = in_img.width();
	int y_size = in_img.height();
    float exposure = params[0];
    int start_x = (params[1] - params[3]/2)*x_size;
    int start_y = (params[2] - params[4]/2)*y_size;
    int end_x = (params[1] + params[3]/2)*x_size;
    int end_y = (params[2] + params[4]/2)*y_size;
    int asp1 = params[5];
    int asp2 = params[6];
    float aspect_ratio = (asp1 / float(asp2));
    qDebug() << "aspect_ratio = " << aspect_ratio;
    int bytesPerLine;
	if(prog_name == "Augmentation 0"){
		// Create empty output image.
		out_img = QImage(x_size, y_size, in_img.format());
		
		// Create buffers for YUV image.
		uchar* Y_buff = new uchar[x_size*in_img.height()];
		char* U_buff = new char[x_size*in_img.height()/4];
		char* V_buff = new char[x_size*in_img.height()/4];
		// Convert input image to YUV420 image.
		RGBtoYUV420(in_img.bits(), x_size, y_size, Y_buff, U_buff, V_buff);

		myAugmentation0(Y_buff, x_size, y_size);
		
		// Zero out U and V component.
		procesing_YUV420(Y_buff, U_buff, V_buff, x_size, y_size, 1, 1, 1);

		// Convert YUV image back to RGB.
		YUV420toRGB(Y_buff, U_buff, V_buff, x_size, y_size, out_img.bits());

		// Delete used memory buffers.
		delete[] Y_buff;
		delete[] U_buff;
		delete[] V_buff;
    }else if(prog_name == "Augmentation 2"){
        // Create empty output image.
        out_img = QImage(x_size, y_size, in_img.format());

        adjustExposure(in_img.bits(), out_img.bits(), x_size, y_size, exposure);
    }else if(prog_name == "Augmentation 1")
    {
        //Image Histogram - TODO
        //..

    }

    else if (prog_name == "Augmentation 3")
    {
        int n_x, n_y;
        float currentAspectRatio = (float)x_size / y_size;
        if (currentAspectRatio > aspect_ratio)
        {
            n_x = y_size * aspect_ratio;
            qDebug() << "n+x = " << n_x;
            n_y = y_size;
        } else
        {
            n_x = x_size;
            n_y = y_size / aspect_ratio;
            qDebug() << "n_y = " << n_y;
        }

        out_img = QImage(n_x, n_y, in_img.format());
        aspectRationCrop(in_img.bits(), out_img.bits(), x_size, y_size, n_x, n_y);
    }
    else if (prog_name == "Augmentation 4")
    {
        out_img = QImage(x_size, y_size, in_img.format());
        bytesPerLine = out_img.bytesPerLine();
        applyOcclusion(in_img.bits(), out_img.bits(), x_size, y_size, start_x, start_y, end_x, end_y, bytesPerLine);
    }

}
