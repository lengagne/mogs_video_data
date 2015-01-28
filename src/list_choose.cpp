#include "list_choose.h"
#include "ui_list_choose.h"
#include <QDebug>

List_choose::List_choose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::List_choose)
{
    ui->setupUi(this);
}

List_choose::~List_choose()
{
    delete ui;
}


void List_choose::on_pushButton_clicked()
{
    QModelIndex index;
    index = ui->listView->currentIndex();
    int row  = index.row();

    qDebug()<<" Ouvrir clicked : "<<list_[row];
    *text = list_[row];
    close();
}

void List_choose::on_pushButton_2_clicked()
{
    qDebug()<<" Annuler clicked";
    *text = "";
    close();
}

void List_choose::setList(std::vector<std::string> list)
{
    list_model_ = new QStringListModel();

    for(int i=0;i<list.size();i++)
        list_ << QString(list[i].c_str());
    list_model_->setStringList(list_);
    ui->listView->setModel(list_model_);
}
