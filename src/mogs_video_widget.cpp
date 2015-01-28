#include "mogs_video_widget.h"
#include "ui_mogs_video_widget.h"

#include <iostream>
#include <QDebug>
#include <QStringListModel>

mogs_video_widget::mogs_video_widget(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::mogs_video_widget)
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
}

void mogs_video_widget::add_point()
{
    qDebug()<<"Add point";
}

void mogs_video_widget::add_video()
{
    qDebug()<<"Add video";
}

void mogs_video_widget::initialisation()
{
    qDebug()<<"Initialisation";
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
    qDebug()<<"Open project";
}

void mogs_video_widget::remove_point()
{
    qDebug()<<"Remove point";
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
