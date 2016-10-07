#ifndef mogs_qt_list_video_H
#define mogs_qt_list_video_H

#include <QListView>
#include <QDialog>
#include <QStringListModel>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QListWidgetItem>
#include <QString>

class mogs_qt_list_video : public QListView
{
	Q_OBJECT
public:
	explicit mogs_qt_list_video(QWidget *parent = 0);
	~mogs_qt_list_video();

	QString get_active_video_name();

	void set_list(const std::vector<QString> &list);

signals:

public slots:

protected:
	void mousePressEvent(QMouseEvent *mouseEvent);

private:

	QStringListModel *model_video;
	QStringList list_video;
	QString active_video_;
};

#endif // mogs_qt_list_video_H

