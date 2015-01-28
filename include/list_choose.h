#ifndef LIST_CHOOSE_H
#define LIST_CHOOSE_H

#include <QDialog>
#include <QString>
#include <QStringListModel>

namespace Ui {
class List_choose;
}

class List_choose : public QDialog
{
    Q_OBJECT

public:
    explicit List_choose(QWidget *parent = 0);
    ~List_choose();

    void setList(std::vector<std::string> list);

    void setString(QString * val)
    {
        text = val;
    }

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::List_choose *ui;
    QString * text;

    QStringListModel *list_model_;
    QStringList list_;
};

#endif // LIST_CHOOSE_H
