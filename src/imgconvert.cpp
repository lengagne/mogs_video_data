#include "imgconvert.h"

QImage MirrorImage(QImage source, int side)
{
    int x, y;
    QImage ret;

    ret = QImage(source); //source.width(), source.height(), source.format());
    for(x=0; x<source.width(); x++)
    {
        for(y=0; y<source.height(); y++)
        {
            ret.setPixel(((side & V_MIRROR) ? source.width() - (x + 1) : x), ((side & H_MIRROR) ? source.height() - (y + 1) : y), source.pixel(x, y));
        }
    }

    return ret;
}

QImage ConvertImage(IplImage *source)
{
    int cvIndex, cvLineStart;
    QImage ret;

    if(source->depth != IPL_DEPTH_8U || source->nChannels != 3)
        return ret;

    QImage temp(source->width, source->height, QImage::Format_RGB32);
    ret = temp;
    cvIndex = 0;
    cvLineStart = 0;
    for (int y = 0; y < source->height; y++)
    {
        unsigned char red,green,blue;
        cvIndex = cvLineStart;
        for (int x = 0; x < source->width; x++)
        {
            red = source->imageData[cvIndex+2];
            green = source->imageData[cvIndex+1];
            blue = source->imageData[cvIndex+0];

            ret.setPixel(x,y,qRgb(red, green, blue));
            cvIndex += 3;
        }
        cvLineStart += source->widthStep;
    }

    return ret;
}


/*QImage ConvertImage(IplImage *source)
{
    int cvIndex, cvLineStart;
    QImage ret;

    switch (source->depth)
    {
        case IPL_DEPTH_8U:
            switch (source->nChannels)
            {
                case 3:
                    QImage temp(source->width, source->height, QImage::Format_RGB32);
                    ret = temp;
                    cvIndex = 0; cvLineStart = 0;
                    for (int y = 0; y < source->height; y++)
                    {
                        unsigned char red,green,blue;
                        cvIndex = cvLineStart;
                        for (int x = 0; x < source->width; x++)
                        {
                            red = source->imageData[cvIndex+2];
                            green = source->imageData[cvIndex+1];
                            blue = source->imageData[cvIndex+0];

                            ret.setPixel(x,y,qRgb(red, green, blue));
                            cvIndex += 3;
                        }
                        cvLineStart += source->widthStep;
                    }
                    break;
                default:
                    return ret;
                    break;
            }
            break;
        default:
            return ret;
            break;
    }
    return ret;
}
*/
