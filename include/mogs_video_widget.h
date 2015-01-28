#ifndef MOGS_VIDEO_WIDGET_H
#define MOGS_VIDEO_WIDGET_H

#include <QMainWindow>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "imgconvert.h"
#include "mogs_qt_video_scene.h"
#include "mogs_qt_list_point.h"
#include "mogs_qt_list_video.h"

namespace Ui {
class mogs_video_widget;
}

class mogs_video_widget : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mogs_video_widget(QWidget *parent = 0);
    ~mogs_video_widget();

protected:
    void timerEvent(QTimerEvent*);
    CvCapture *camera;

private slots:

    void add_point();
    void add_video();
    void initialisation();
    void new_project();
    void on_listView_2_clicked(const QModelIndex &index);
    void on_play_pause_button_clicked();
    void open_project();
    void remove_point();
    void remove_video();
    void scroll_bar(int value);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::mogs_video_widget *ui;
    bool pause_;

    mogs_qt_video_scene *scene;
    IplImage *image;

};

#endif // MOGS_VIDEO_WIDGET_H
