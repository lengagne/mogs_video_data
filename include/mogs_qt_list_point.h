#ifndef MOGS_QT_LIST_POINT_H
#define MOGS_QT_LIST_POINT_H

#include <QTableView>
#include <QDialog>
#include <QStringListModel>
#include <QStandardItem>

class mogs_qt_list_point : public QTableView
{
	Q_OBJECT
public:
	explicit mogs_qt_list_point(QWidget *parent = 0);
	~mogs_qt_list_point();

	/** Return true if a point is selected
	 * and return the name*/
	bool get_selected_name(std::vector<QString> & name);

	void set_active_video(QString &video_name);

	void set_list(const std::vector<QString> &list);

signals:

public slots:

protected:
	void mousePressEvent(QMouseEvent *mouseEvent);

private:
	QStandardItemModel *model;
	QStringList list_point;
	std::vector<QString> select_names_;
	QStandardItem * name_item;
	std::vector<QStandardItem *> Items_;
};

#endif // MOGS_QT_LIST_POINT_H

