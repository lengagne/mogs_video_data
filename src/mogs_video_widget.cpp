#include "mogs_video_widget.h"
#include "ui_mogs_video_widget.h"

#include <iostream>
#include <QDebug>
#include <QStringListModel>

mogs_video_widget::mogs_video_widget(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::mogs_video_widget),project_(NULL)
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

    camera =  cvCreateCameraCapture(0);
    assert(camera);
    IplImage * image=cvQueryFrame(camera);
    assert(image);
    scene = new mogs_qt_video_scene(this);
    qDebug()<<"Image depth=" << image->depth;
    qDebug()<<"Image nChannels="<< image->nChannels;
    scene->set_height(image->height);
    scene->set_width(image->width);
    qDebug()<<"Image width="<< image->width;
    ui->OpenCVWindow->setScene(scene);

    startTimer(100);
    pause_ = true;
}

mogs_video_widget::~mogs_video_widget()
{
	cvReleaseCapture(&camera);
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
    qDebug()<<"Add point";
}

void mogs_video_widget::add_video()
{
    qDebug()<<"Add video";
}

void mogs_video_widget::new_project()
{
    qDebug()<<"New project";
}

void mogs_video_widget::on_listView_2_clicked(const QModelIndex &index)
{
    QString video_name = ui->listView_2->get_active_video_name();
    ui->Label_selected_video->setText("Selected Video : " + video_name);
    ui->tableView->set_active_video(video_name);
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
    QString text;
    w->set_file(&text);
    w->show();
    w->exec();

    qDebug()<<" text  = "<< text;
    if ( project_->read(text.toStdString()))
	qDebug()<<" Project reading done";
    else
	qDebug()<<" Project reading failed";
    
    update_list_point();
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
    qDebug()<<"Remove video";
}

void mogs_video_widget::scroll_bar(int value)
{
    qDebug()<<value;
    QString val = QString(value);
    ui->label->setText("frame: "+QString::number(value)+"/ max frame");
}

void mogs_video_widget::timerEvent(QTimerEvent*)
{
    if (pause_)
    {
        image=cvQueryFrame(camera);
    }

    scene->clear();
    scene->addPixmap(QPixmap::fromImage(ConvertImage(image)));
    scene->DrawRectangle();
    // scale the video
    ui->OpenCVWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

void mogs_video_widget::update_list_point()
{
	std::vector<std::string> points = project_->get_points_list();
	ui->tableView->set_list(points);	
}
