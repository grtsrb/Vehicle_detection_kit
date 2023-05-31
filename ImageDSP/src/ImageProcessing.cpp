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

    // Exposure
    float exposure = params[0];

    // Occlusion
    int start_x = (params[1] - params[3]/2)*x_size;
    int start_y = (params[2] - params[4]/2)*y_size;
    int end_x = (params[1] + params[3]/2)*x_size;
    int end_y = (params[2] + params[4]/2)*y_size;
    int num_of_occlusions = params[8];

    // Aspect Ratio + Cut
    int asp1 = params[5];
    int asp2 = params[6];
    float aspect_ratio = (asp1 / float(asp2));

    // Histogram equalization
    uchar hist_arg = params[7];

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
    }
    // Histogram equalization
    else if(prog_name == "Augmentation 1")
    {
        qDebug() << "Histogram argument = " << hist_arg;
        out_img = QImage(x_size, y_size, in_img.format());
        bytesPerLine = out_img.bytesPerLine();

        equalizeHistogram(in_img.bits(), x_size, y_size, bytesPerLine, out_img.bits(), hist_arg);

    }
    // Exposure
    else if(prog_name == "Augmentation 2"){
        qDebug() << "Exposure = " << exposure;
        out_img = QImage(x_size, y_size, in_img.format());

        adjustExposure(in_img.bits(), out_img.bits(), x_size, y_size, exposure);
    }
    // Aspect ratio + cut
    else if (prog_name == "Augmentation 3")
    {
        qDebug() << "aspect_ratio = " << aspect_ratio;
        int n_x, n_y;
        float currentAspectRatio = (float)x_size / y_size;
        if (currentAspectRatio > aspect_ratio)
        {
            // Wider input image
            n_x = y_size * aspect_ratio;
            qDebug() << "n+x = " << n_x;
            n_y = y_size;
        } else
        {
            // Taller input image
            n_x = x_size;
            n_y = y_size / aspect_ratio;
            qDebug() << "n_y = " << n_y;
        }

        out_img = QImage(n_x, n_y, in_img.format());
        aspectRationCrop(in_img.bits(), out_img.bits(), x_size, y_size, n_x, n_y);
    }
    // Occlusion
    else if (prog_name == "Augmentation 4")
    {
        qDebug() << "start_x = " << start_x;
        qDebug() << "start_y = " << start_y;
        qDebug() << "end_x = " << end_x;
        qDebug() << "end_y = " << end_y;
        out_img = QImage(x_size, y_size, in_img.format());
        bytesPerLine = out_img.bytesPerLine();
        applyOcclusion(in_img.bits(), out_img.bits(), x_size, y_size, start_x, start_y, end_x, end_y, bytesPerLine, num_of_occlusions);
    }

}
