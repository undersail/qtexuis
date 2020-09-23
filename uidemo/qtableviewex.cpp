#include "qtableviewex.h"

QTableViewEx::QTableViewEx(QWidget *parent)
	: QTableView(parent)
{

}

QTableViewEx::~QTableViewEx()
{

}

void QTableViewEx::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	emit currentItemChanged(current,previous);
	scrollTo(current);
}