#include <QDebug>
#include <QMouseEvent>
#include <iostream>
#include "mogs_qt_list_point.h"

mogs_qt_list_point::mogs_qt_list_point(QWidget *parent)
{
	model = new QStandardItemModel(list_point.size(),1,this);
	model->setHorizontalHeaderItem(0, new QStandardItem(QString("Point name")));
	name_item = new QStandardItem(QString("Selected Video"));
	setModel(model);
	select_names_.clear();
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
	if (mouseEvent->button() == Qt::LeftButton)
	{
		select_names_.clear();
		QModelIndexList indexes = selectedIndexes();
		QModelIndex index;
		foreach(index, indexes) {
			int row = index.row();
			if (row != -1)
			{
				select_names_.push_back(list_point[row]);
			}
		}
	}
	if (mouseEvent->button() == Qt::RightButton)
	{
//		qDebug()<<"Point Clic on list with right button";
	}
}

bool mogs_qt_list_point::get_selected_name(std::vector<QString> & video_name)
{
	if (select_names_.size() == 0)
		return false;
	video_name = select_names_;
	return true;
}

void mogs_qt_list_point::set_active_video(QString &video_name)
{
	qDebug()<<"SetActiveVideo";
	qDebug()<<"name_item = "<<name_item;
	qDebug()<<"video_name = "<<video_name;
	name_item->setText(video_name);
	qDebug()<<"SetActiveVideo Done";
}

void mogs_qt_list_point::set_list(const std::vector<QString> & list)
{
	// Remove previous list
	list_point.clear();
	for (int i=0;i<Items_.size();i++)
		delete Items_[i];
	Items_.clear();
	model->clear();
	model->setHorizontalHeaderItem(0, new QStandardItem(QString("Point name")));
	name_item = new QStandardItem(QString("Selected Video"));
	model->setHorizontalHeaderItem(1, name_item );
	QStandardItem * name2 = new QStandardItem(QString("On the frame"));
	model->setHorizontalHeaderItem(2, name2 );

	for (int i=0;i<list.size();i++)
	{
		list_point << list[i];
		QStandardItem *currentItem = new QStandardItem(list[i]);
		model->setItem(i,0,currentItem);
		model->setItem(i,1,new QStandardItem(QString("no")));
		model->setItem(i,2,new QStandardItem(QString("no")));
		Items_.push_back(currentItem);
	}
}
