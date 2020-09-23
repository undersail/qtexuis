#ifndef INFO_H
#define INFO_H

#include <QtWidgets/QGraphicsTextItem>
#include <QPen>
#include "nodeitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class Info : public QGraphicsTextItem
{
public:
    enum { Type = UserType + 10 };

	Info(NodeItem *baseItem, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

	int type() const
	{ return Type; }
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void setColor(const QColor &color)
	{ m_infoColor = color; }
	void setPen ( const QPen & pen )
	{ m_pen = pen; }
    QPen pen() const
	{ return m_pen; }
	NodeItem *baseItem() const
	{ return m_baseItem; }

public slots:
	void updatePosition();

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);

private:
	NodeItem *m_baseItem;
	QColor m_infoColor;
	QPolygonF m_infoHead;
	QPen m_pen;
};

#endif // INFO_H
