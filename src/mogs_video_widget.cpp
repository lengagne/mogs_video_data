#include "mogs_video_widget.h"
#include "ui_mogs_video_widget.h"

#include <iostream>
#include <QDebug>
#include <QStringListModel>
#include <unistd.h>
#include <fstream>
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
	connect(ui->actionSupprimer_video, SIGNAL(triggered()), this, SLOT(remove_video()));
	connect(ui->actionExporter_video, SIGNAL(triggered()), this, SLOT(export_video()));
	connect(ui->actionExporter_donn_es, SIGNAL(triggered()), this, SLOT(export_txt()));

	ui->horizontalScrollBar->setMaximum(1000);
	ui->horizontalScrollBar->setSliderPosition(0);

	connect(ui->horizontalScrollBar, SIGNAL(sliderMoved(int)), this, SLOT(scroll_bar(int)));
	ui->label->setText("frame: 0/ max frame");
	ui->Label_selected_video->setText("Selected Video : ");

	images_.clear();

	timer_id_ = startTimer(100);
	pause_ = true;

	count_ = 0;
}

mogs_video_widget::~mogs_video_widget()
{
	delete ui;

	if (project_)
	{
		if (project_->read_data())	// check if there was no error for reading
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
		project_->add_point_to_project(name);
		update_list_point();
		delete window;
	}
}

void mogs_video_widget::add_video()
{
	qt_rep_name *window = new qt_rep_name();
	QString video_file;
	window->set_path_and_name(&video_file,&video_name_);
	window->show();
	window->exec();
	if (video_name_ == "")
	{
		std::cout<<"Please specify a video name"<<std::endl;
	}else if (project_)
	{
		project_->add_video_to_project(video_file, video_name_);

	}
	else
		std::cout<<"Please open project first "<<std::endl;
	update_list_video();
}


void mogs_video_widget::export_txt()
{
	qt_rep_name *window = new qt_rep_name();
	QString path, text_name;
	window->set_path_and_name(&path,&text_name);
	window->show();
	window->exec();
	QString name = path + "/" + text_name;


	std::ofstream myfile;
	myfile.open (name.toStdString());
	myfile << "NbFrame\tTime\t";
	if (ui->tableView->get_selected_name(points_name_))
	for (int i=0;i<points_name_.size();i++)
		myfile<<"\t"<<points_name_[i].toStdString()+".X"<<"\t"<<points_name_[i].toStdString()+".Y";
	myfile<<std::endl;
	int w = scene->get_width()/2;
	int h = scene->get_height()/2;
	for (count_=0;count_<images_.size();count_++)
	{
		myfile<<count_<<"\t"<<count_*1./video_fps_ ;
		if (ui->tableView->get_selected_name(points_name_))
		{
			CvPoint visu_point;
			for (int i=0;i<points_name_.size();i++)
				if (project_->get_point(video_name_, points_name_[i], count_, visu_point))
				{
					scene->DrawPoint(visu_point,points_name_[i]);
					myfile<<"\t"<<(1.0*visu_point.x - w)/(w)<<"\t"<<(1.0*visu_point.y-h)/h;
				}else
				{
					// std::cout<<"no found point "<< points_name_[i] <<std::endl;
					myfile<<"\t?\t?";
				}
			myfile<<std::endl;
		}
	}
	myfile.close();
	std::cout<<std::endl<<"Text exported"<<std::endl;
}

void mogs_video_widget::export_video()
{
	qt_rep_name *window = new qt_rep_name();
	QString path, video_name;
	window->set_path_and_name(&path,&video_name);
	window->show();
	window->exec();
	QString name = path + "/" + video_name;

	if (!scene)
	{
		std::cerr<<"Error in export_video"<<std::endl;
		return;
	}

	std::string NAME = name.toStdString();
	if(NAME.substr(NAME.find_last_of(".") + 1) != "avi") {
		NAME += ".avi";
	}
	std::cout<<"Export video to "<< NAME <<std::endl;

    cv::VideoWriter outputVideo(NAME, CV_FOURCC('D','I','V','3') , video_fps_, cv::Size(scene->get_width(), scene->get_height()), true);
    if (!outputVideo.isOpened())
    {
        std::cerr  << "Could not open the output video for write: "<< NAME<< std::endl;
        return ;
    }

	for (count_=0;count_<images_.size();count_++)
	{
		update_image();
			QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
			QPainter painter(&image);
			scene->render(&painter);
			image.save("file_name.jpg");
			cv::Mat  res;
			res = cv::imread("file_name.jpg", CV_LOAD_IMAGE_COLOR);
			outputVideo.write(res);
			std::cout<<"adding image "<< count_ <<" / "<< images_.size() <<"\r";
	}
	int dummy = system("rm file_name.jpg");
	std::cout<<std::endl<<"Video exported"<<std::endl;
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
	project_name = project_->new_project(total);
	if ( project_->read(project_name))
		qDebug()<<" Project reading done";
	else
		qDebug()<<" Project reading failed";
}

void mogs_video_widget::on_listView_2_clicked(const QModelIndex &index)
{
	video_name_ = ui->listView_2->get_active_video_name();
	if ( video_name_ != "")
	{
		ui->Label_selected_video->setText("Selected Video : " + video_name_);
		ui->tableView->set_active_video(video_name_);
		count_ = 0;
		project_->get_images(video_name_, images_, & video_fps_);
		killTimer(timer_id_);
		timer_id_ = startTimer(1000./video_fps_);
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
	CvPoint Center;
	if (scene->get_rectangle_center(Center) && points_name_.size()== 1)
	{
		while ( project_->edit_data(video_name_, points_name_[0],count_,Center,Center, MANUAL_EDITING));
		count_ ++;
		if (project_)
		{
			delete project_;
			project_ = new video_interface();
			if ( project_->read(project_name))
				qDebug()<<" Project reading done";
			else
				qDebug()<<" Project reading failed";
		}
	}else
	{
		qDebug()<<"Manual editing asked but you do not selected one point.";
	}
}

void mogs_video_widget::on_pushButton_2_clicked()
{
	qDebug()<<"Automatic editing ";
	this->setCursor(QCursor(Qt::WaitCursor));
	CvPoint Pt1,Pt2;
	if (scene->get_rectangle(Pt1,Pt2) && points_name_.size()== 1)
	{
		int mem = count_;
		while ( project_->edit_data(video_name_, points_name_[0],count_,Pt1,Pt2, TLD_EDITING,images_[count_]))
		{
			count_ ++;
			if (count_ >= images_.size())
				break;
		}
		count_ = mem;

		if (project_)
		{
			delete project_;
			project_ = new video_interface();
			if ( project_->read(project_name))
				qDebug()<<" Project reading done";
			else
				qDebug()<<" Project reading failed";
		}
	}else
	{
		qDebug()<<"Automatic editing asked but you do not selected one point.";
	}
	this->setCursor(QCursor(Qt::ArrowCursor));
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

    if ( project_->read(project_name))
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
		std::vector<QString> names;
		ui->tableView->get_selected_name(names);
		for (int i=0;i<names.size();i++)
			project_->remove_point_to_project(names[i]);
		update_list_point();
	}
}

void mogs_video_widget::remove_video()
{
	if (project_)
	{
		QString name;
		name = ui->listView_2->get_active_video_name();
		project_->remove_video_to_project(name);
		update_list_video();
	}
}

void mogs_video_widget::scroll_bar(int value)
{
	count_ = value;
	update_image();
}

void mogs_video_widget::timerEvent(QTimerEvent*)
{
	if (!pause_)
	{
		count_ ++;
	}
	update_image();
	if (scene)
	{
		scene->DrawRectangle();
		// scale the video
		ui->OpenCVWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
	}
}

void mogs_video_widget::update_image()
{
	if (scene)
		scene->clear();

	if (count_ > images_.size()-1)
	{
		count_ = images_.size()-1;
		pause_ = true;
	}
	ui->horizontalScrollBar->setValue(count_);
	ui->label->setText("frame: "+QString::number(count_)+"/ "+ QString::number(images_.size()));

	if (images_.size() >0 && count_>=0 && count_ < images_.size())
	{
		if (scene)
		{
			IplImage* img = cvCloneImage(images_[count_]);
			scene->addPixmap(QPixmap::fromImage(ConvertImage(img)));
			if (ui->tableView->get_selected_name(points_name_))
			{
				CvPoint visu_point;
				for (int i=0;i<points_name_.size();i++)
				if (project_->get_point(video_name_, points_name_[i], count_, visu_point))
				{
					scene->DrawPoint(visu_point,points_name_[i]);
				}else
				{
					// std::cout<<"no found point "<< points_name_[i] <<std::endl;
				}
			}
		}
	}
}

void mogs_video_widget::update_list_point()
{
	if (project_)
	{
		delete project_;
		project_ = new video_interface();
		if (!project_->read(project_name))
			qDebug()<<" Project reading failed";
		std::vector<QString> points = project_->get_points_list();
		ui->tableView->set_list(points);
	}
}

void mogs_video_widget::update_list_video()
{
	if (project_)
	{
		delete project_;
		project_ = new video_interface();
		if (!project_->read(project_name))
			qDebug()<<" Project reading failed";
		std::vector<QString> videos = project_->get_videos_list();
		ui->listView_2->set_list(videos);
	}
}
