#ifndef MOGS_QT_VIDEO_SCENE_H
#define MOGS_QT_VIDEO_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDialog>
#include <QGraphicsItem>


class mogs_qt_video_scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit mogs_qt_video_scene(QWidget *parent = 0);
    ~mogs_qt_video_scene();

    void DrawRectangle();

    void set_height(int h)
    {
        height_ = h;
    }

    void set_width(int w)
    {
        width_ = w;
    }

signals:

public slots:

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QPointF moving_point_;
    QPointF pressed_point_;
    QPointF released_point_;

    bool rectanle_in_progress_;
    bool rectangle_ready_;
    QGraphicsRectItem *rect;

    int width_, height_;
};

#endif // MOGS_QT_VIDEO_SCENE_H

