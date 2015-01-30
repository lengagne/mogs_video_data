#include "mogs_video_widget.h"
#include "ui_mogs_video_widget.h"

#include <iostream>
#include <QDebug>
#include <QStringListModel>
#include "qt_get_point_name.h"
#include "qt_rep_name.h"

mogs_video_widget::mogs_video_widget(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::mogs_video_widget),project_(NULL),scene(NULL)
{
	ui->setupUi(this);

	connect(ui->actionInit, SIGNAL(triggered()), this, SLOT(initialisation()));
	connect(ui->actionNouveau_projet, SIGNAL(triggered()), this, SLOT(new_project()));
	connect(ui->actionOuvrir_projet, SIGNAL(triggered()), this, SLOT(open_project()));
	connect(ui->actionAjouter_point, SIGNAL(triggered()), this, SLOT(add_point()));
	connect(ui->actionAjouter_video, SIGNAL(triggered()), this, SLOT(add_video()));
	connect(ui->actionSupprimer_point, SIGNAL(triggered()), this, SLOT(remove_point()));
	connect(ui->actionSupprimer_video, SIGNAL(triggered()), this, SLOT(remove_video()));
	connect(ui->actionFermer_2, SIGNAL(triggered()), this, SLOT(close()));

	ui->horizontalScrollBar->setMaximum(1000);
	ui->horizontalScrollBar->setSliderPosition(0);

	connect(ui->horizontalScrollBar, SIGNAL(sliderMoved(int)), this, SLOT(scroll_bar(int)));
	ui->label->setText("frame: 0/ max frame");
	ui->Label_selected_video->setText("Selected Video : ");
 
	images_.clear();

	startTimer(10);
	pause_ = true;
}

mogs_video_widget::~mogs_video_widget()
{
	delete ui;
	
	if (project_)
	{
		project_->save_data();
		delete project_;
	}
	std::cout<<"Closing mogs_video_data"<<std::endl;
}

void mogs_video_widget::add_point()
{
	if (project_)
	{
		qt_get_point_name *window = new qt_get_point_name();
		QString name;
		window->set_string(&name);
		window->show();
		window->exec();		
		qDebug()<<"Add point : "<< name;
		project_->add_point_to_project(name.toStdString());
		update_list_point();
		delete window;
	}    
}

void mogs_video_widget::add_video()
{
	qt_rep_name *window = new qt_rep_name();
	QString video_file, video_name;
	window->set_path_and_name(&video_file,&video_name);
	window->show();
	window->exec();
	project_->add_video_to_project(video_file.toStdString(), video_name.toStdString());
	update_list_video();
}

void mogs_video_widget::new_project()
{
	qt_rep_name *window = new qt_rep_name();
	QString path;
	window->set_path_and_name(&path,&project_name);
	window->show();
	window->exec();
	QString total = path + "/" + project_name;
	project_ = new video_interface ();
	project_->new_project(total.toStdString());
}

void mogs_video_widget::on_listView_2_clicked(const QModelIndex &index)
{
	QString video_name = ui->listView_2->get_active_video_name();
	if ( video_name != "")
	{
		ui->Label_selected_video->setText("Selected Video : " + video_name);
		ui->tableView->set_active_video(video_name);
		count_ = 0;
		project_->get_images(video_name.toStdString(), images_, & video_fps_);
		pause_ = true;
		ui->label->setText("frame: "+QString::number(count_)+"/ "+ QString::number(images_.size()));
		ui->horizontalScrollBar->setMaximum(images_.size());
		
		scene = new mogs_qt_video_scene(this);
		scene->set_height(images_[0]->height);
		scene->set_width(images_[0]->width);
		ui->OpenCVWindow->setScene(scene);
	}
}

void mogs_video_widget::on_play_pause_button_clicked()
{
    qDebug()<<"Push on play/pause ";
    pause_ = !pause_;
}

void mogs_video_widget::on_pushButton_clicked()
{
    qDebug()<<"Manual editing pressed";
}

void mogs_video_widget::on_pushButton_2_clicked()
{
    qDebug()<<"Automatic editing pressed";
}

void mogs_video_widget::open_project()
{   
	if (project_)
	{
		delete project_;
	}
	project_ = new video_interface();
	
    qDebug()<<"Open project";
    
    List_repo *w = new List_repo();
    w->set_file(&project_name);
    w->show();
    w->exec();

    if ( project_->read(project_name.toStdString()))
	qDebug()<<" Project reading done";
    else
	qDebug()<<" Project reading failed";
    
    update_list_point();
    update_list_video();
}

void mogs_video_widget::remove_point()
{
	if (project_)
	{
		QString name;
		ui->tableView->get_selected_name(name);
		qDebug()<<"Remove point : "<< name;
		project_->remove_point_to_project(name.toStdString());
		update_list_point();
	}
}

void mogs_video_widget::remove_video()
{
	if (project_)
	{
		QString name;
		name = ui->listView_2->get_active_video_name();
		qDebug()<<"Remove video : "<< name;
		project_->remove_video_to_project(name.toStdString());
		update_list_video();
	}
}

void mogs_video_widget::scroll_bar(int value)
{
	count_ = value;
	QString val = QString(value);
	ui->label->setText("frame: "+QString::number(value)+"/ "+ QString::number(images_.size()));
}

void mogs_video_widget::timerEvent(QTimerEvent*)
{
	if (scene)
		scene->clear();
	if (!pause_)
	{
		count_ ++;
		if (count_ > images_.size()-1)
			count_ = images_.size()-1;
		ui->horizontalScrollBar->setValue(count_);
		ui->label->setText("frame: "+QString::number(count_)+"/ "+ QString::number(images_.size()));
		
	}	
	
	if (images_.size() >0 && count_>=0 && count_ < images_.size())
	{
		if (scene)
		{
			IplImage* img = images_[count_];
			scene->addPixmap(QPixmap::fromImage(ConvertImage(images_[count_])));
		}
	}	
	if (scene)
	{
		scene->DrawRectangle();
		// scale the video
		ui->OpenCVWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
	}
}

void mogs_video_widget::update_list_point()
{
	if (project_)
	{
		delete project_;
		project_ = new video_interface();
		if ( project_->read(project_name.toStdString()))
			qDebug()<<" Project reading done";
		else
			qDebug()<<" Project reading failed";
		std::vector<std::string> points = project_->get_points_list();
		ui->tableView->set_list(points);
	}
}

void mogs_video_widget::update_list_video()
{
	if (project_)
	{
		delete project_;
		project_ = new video_interface();
		if ( project_->read(project_name.toStdString()))
			qDebug()<<" Project reading done";
		else
			qDebug()<<" Project reading failed";
	
		std::vector<std::string> videos = project_->get_videos_list();
		ui->listView_2->set_list(videos);	
	}
}
