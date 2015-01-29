#include "qt_get_point_name.h"
#include "ui_qt_get_point_name.h"

#include <QDebug>

qt_get_point_name::qt_get_point_name(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qt_get_point_name),name(NULL)
{
    ui->setupUi(this);
}

qt_get_point_name::~qt_get_point_name()
{
    delete ui;
}

void qt_get_point_name::on_pushButton_clicked()
{
    *name = ui->textEdit->toPlainText();
    qDebug()<<"name = "<<name;
    close();
}

void qt_get_point_name::on_pushButton_2_clicked()
{
    *name = "";
    close();
}
