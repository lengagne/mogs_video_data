#ifndef QT_GET_POINT_NAME_H
#define QT_GET_POINT_NAME_H

#include <QDialog>

namespace Ui {
class qt_get_point_name;
}

class qt_get_point_name : public QDialog
{
    Q_OBJECT

public:
    explicit qt_get_point_name(QWidget *parent = 0);
    ~qt_get_point_name();

    void set_string(QString *in)
    {
        name = in;
    }

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::qt_get_point_name *ui;
    QString *name;
};

#endif // QT_GET_POINT_NAME_H
