#ifndef IMGCONVERT_H
#define IMGCONVERT_H

#include <opencv/cv.h>
#include <QPixmap>
#include <QImage>

#define V_MIRROR 1
#define H_MIRROR 2

QImage MirrorImage(const QImage source, int side);
QImage ConvertImage(IplImage *source);

#endif
