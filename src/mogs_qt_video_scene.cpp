#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "mogs_qt_video_scene.h"

mogs_qt_video_scene::mogs_qt_video_scene(QWidget *parent)
{
    setParent(parent);
    rectangle_ready_ = false;
    rectanle_in_progress_ = false;
    rect = NULL;
}

mogs_qt_video_scene::~mogs_qt_video_scene()
{

}

void mogs_qt_video_scene::DrawPoint(	CvPoint in,
					QString name)
{
	QPen outlinePen(Qt::red);
	outlinePen.setWidth(3);
	addEllipse(in.x, in.y, 5, 5,outlinePen);
	
	if (name != "")
	{
		QGraphicsTextItem * io = new QGraphicsTextItem;
		io->setDefaultTextColor(Qt::red);
		io->setPos(in.x -20,in.y);
		io->setPlainText(name);
		addItem(io);
	}
}

void mogs_qt_video_scene::DrawRectangle()
{
    if(rectangle_ready_)
    {
        QPen outlinePen(Qt::black);
        outlinePen.setWidth(3);
        if (pressed_point_.x() > 0
                && pressed_point_.y()>0
                && pressed_point_.x()< width_
                && pressed_point_.y()< height_
                && released_point_.x()>0
                && released_point_.y()>0
                && released_point_.x()< width_
                && released_point_.y()< height_)

        rect = addRect(pressed_point_.x(), pressed_point_.y(),
                              released_point_.x()-pressed_point_.x(),released_point_.y() -pressed_point_.y(), outlinePen);
    }else if (rectanle_in_progress_)
    {
        if (pressed_point_.x() > 0
                && pressed_point_.y()>0
                && pressed_point_.x()< width_
                && pressed_point_.y()< height_
                && moving_point_.x()>0
                && moving_point_.y()>0
                && moving_point_.x()< width_
                && moving_point_.y()< height_)
        {
            QPen outlinePen(Qt::green);
            outlinePen.setWidth(3);
            rect = addRect(pressed_point_.x(), pressed_point_.y(),
                                  moving_point_.x()-pressed_point_.x(),moving_point_.y() -pressed_point_.y(), outlinePen);
        }
    }
}

bool mogs_qt_video_scene::get_rectangle_center( CvPoint & out)
{
	QRectF R = rect->rect();
	QPointF P = R.center();
	out.x = P.x();
	out.y = P.y();
	
	if ( out.x < 0 || out.x > width_ ||out.y < 0 || out.y > height_)
		return false;
	
	return true;
}

void mogs_qt_video_scene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    moving_point_ = mouseEvent->scenePos();
}

void mogs_qt_video_scene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if (rect)
        removeItem(rect);
    rectangle_ready_ = false;
    rectanle_in_progress_ = true;
    pressed_point_ = mouseEvent->scenePos();
    qDebug()<<" Pushed : " << pressed_point_;
}

void mogs_qt_video_scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    released_point_ = mouseEvent->scenePos();
    rectangle_ready_ = true;
    rectanle_in_progress_ = false;
    qDebug()<<" Released : " << released_point_;
    DrawRectangle();
}


