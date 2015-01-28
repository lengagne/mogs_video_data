#include <QDebug>
#include <QMouseEvent>
#include <QList>
#include <QModelIndex>
#include <QMenu>
#include "mogs_qt_list_video.h"

mogs_qt_list_video::mogs_qt_list_video(QWidget *parent)
{
    model_video = new QStringListModel();

    list_video << "Video_A" << "Video_B" << "Video_C";
    model_video->setStringList(list_video);
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
        qDebug()<<"Video Clic on list with left button"<<row;
        if (row != -1)
            qDebug()<<"Video Clic on list with left button : "<<list_video[row];
        active_video_ = list_video[row];
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
