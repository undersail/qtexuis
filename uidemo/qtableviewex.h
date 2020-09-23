#ifndef QTABLEVIEWEX_H
#define QTABLEVIEWEX_H

#include <QtWidgets/QTableView>
#include <QStandardItemModel>

class QTableViewEx : public QTableView
{
	Q_OBJECT

public:
	QTableViewEx(QWidget *parent);
	~QTableViewEx();

private:
	QStandardItemModel *model;

protected:
	virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);

signals:
	void currentItemChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // QTABLEVIEWEX_H
