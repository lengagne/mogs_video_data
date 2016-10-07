#include <QDebug>
#include <QMouseEvent>
#include <QList>
#include <QModelIndex>
#include <QMenu>
#include "mogs_qt_list_video.h"

mogs_qt_list_video::mogs_qt_list_video(QWidget *parent)
{
    model_video = new QStringListModel();
    setModel(model_video);

    active_video_ = "";
}

mogs_qt_list_video::~mogs_qt_list_video()
{

}

void mogs_qt_list_video::mousePressEvent(QMouseEvent *mouseEvent)
{
	QListView::mousePressEvent(mouseEvent);
	QModelIndex index;
	index = currentIndex();
	int row  = index.row();

	if (mouseEvent->button() == Qt::LeftButton)
	{
		qDebug()<<" row = "<< row;
		qDebug()<<" list_video.size() = "<< list_video.size();
		if (row >= 0 && row < list_video.size())
		{
			qDebug()<<"Video Clic on list with left button : "<<list_video[row];
			active_video_ = list_video[row];
		}
	}
	if(mouseEvent->button() == Qt::RightButton)
	{
		qDebug()<<"Video Clic on list with right button";
	}
}

QString mogs_qt_list_video::get_active_video_name()
{
    return active_video_;
}

void mogs_qt_list_video::set_list(const std::vector<QString> & list)
{
	list_video.clear();
	model_video->removeRows(0,model_video->rowCount());
	for (int i=0;i<list.size();i++)
	{
		list_video << list[i];
	}
	model_video->setStringList(list_video);
}
