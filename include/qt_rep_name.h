#ifndef QT_REP_NAME_H
#define QT_REP_NAME_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class qt_rep_name;
}

class qt_rep_name : public QDialog
{
    Q_OBJECT

public:
    explicit qt_rep_name(QWidget *parent = 0);
    ~qt_rep_name();

    void set_path_and_name(QString* p, QString* n)
    {
        path = p;
        name = n;
    }

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::qt_rep_name *ui;

    QFileSystemModel *model;

    QString *path;
    QString *name;
};

#endif // QT_REP_NAME_H
