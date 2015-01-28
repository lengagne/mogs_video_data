#ifndef MOGS_QT_LIST_POINT_H
#define MOGS_QT_LIST_POINT_H

#include <QTableView>
#include <QDialog>
#include <QStringListModel>
#include <QStandardItem>

class mogs_qt_list_point : public QTableView
{
    //Q_OBJECT
public:
    explicit mogs_qt_list_point(QWidget *parent = 0);
    ~mogs_qt_list_point();

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {

    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {

    }

    void set_active_video(QString &video_name);


signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *mouseEvent);

private:

    QStandardItemModel *model;
    QStringList list_point;

    QStandardItem * name;

    std::vector<QStandardItem *> Items_;
};

#endif // MOGS_QT_LIST_POINT_H

