#include <QDebug>
#include <QMouseEvent>

#include "mogs_qt_list_point.h"

mogs_qt_list_point::mogs_qt_list_point(QWidget *parent)
{
    list_point << "Point_A" << "Point_B" << "Point_C";
    QStandardItemModel *model = new QStandardItemModel(list_point.size(),3,this); //2 Rows and 3 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Point name")));
    name = new QStandardItem(QString("Selected Video"));
    model->setHorizontalHeaderItem(1, name );
    QStandardItem * name2 = new QStandardItem(QString("On the frame"));
    model->setHorizontalHeaderItem(2, name2 );

    Items_.clear();
    for (int i=0;i<list_point.size();i++)
    {
        QStandardItem *currentItem = new QStandardItem(QString(list_point[i]));
        model->setItem(i,0,currentItem);
        model->setItem(i,1,new QStandardItem(QString("no")));
        model->setItem(i,2,new QStandardItem(QString("no")));
        Items_.push_back(currentItem);
    }
    setModel(model);
}

mogs_qt_list_point::~mogs_qt_list_point()
{
    for (int i=0;i<Items_.size();i++)
        delete Items_[i];
    Items_.clear();
}

void mogs_qt_list_point::mousePressEvent(QMouseEvent *mouseEvent)
{
    QTableView::mousePressEvent(mouseEvent);
    QModelIndex index;
    index = currentIndex();
    int row = index.row();
    int col = index.column();

    if (mouseEvent->button() == Qt::LeftButton)
    {
        qDebug()<<"Point Clic on list with left button"<<row;
        if (row != -1)
            qDebug()<<"Point Clic on list with left button : "<<list_point[row]<<" Column = "<<col;

    }
    if (mouseEvent->button() == Qt::RightButton)
    {
        qDebug()<<"Point Clic on list with right button";
    }
}

void mogs_qt_list_point::set_active_video(QString &video_name)
{
    qDebug()<<"SetActiveVideo";
    name->setText(video_name);
}
