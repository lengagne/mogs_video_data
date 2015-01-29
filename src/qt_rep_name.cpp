#include "qt_rep_name.h"
#include "ui_qt_rep_name.h"

qt_rep_name::qt_rep_name(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qt_rep_name),path(NULL),name(NULL)
{
    ui->setupUi(this);
    model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index("/"));
    ui->treeView->setCurrentIndex(model->index(QDir::currentPath()));
    ui->treeView->setColumnWidth(0,400);
}

qt_rep_name::~qt_rep_name()
{
    delete ui;
}

void qt_rep_name::on_pushButton_2_clicked()
{
    *name = ui->textEdit->toPlainText();
    close();
}

void qt_rep_name::on_pushButton_clicked()
{
    *name = "";
    *path = "";
    close();
}

void qt_rep_name::on_treeView_clicked(const QModelIndex &index)
{
       *path = model->filePath(index);
}
