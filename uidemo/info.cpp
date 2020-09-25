#include <QtGui>

#include "info.h"

Info::Info(NodeItem *baseItem,QGraphicsItem *parent, QGraphicsScene *scene)
	: QGraphicsTextItem(parent/*, scene*/)
{
	m_baseItem = baseItem;
	setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_infoColor = Qt::black;
 	setPen(QPen(m_infoColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}


//! [1]
QRectF Info::boundingRect() const
{
	int yOffset = -25;
	if (m_baseItem->pos().y() > 0){
        yOffset = 0;
	}

	return QRectF(m_baseItem->pos(), QSizeF(0,0))
		.normalized()
		.adjusted(-30, yOffset, 30, 25);
}
//! [1]

//! [2]
QPainterPath Info::shape() const
{
	QPainterPath path = QGraphicsTextItem::shape();
	path.addPolygon(m_infoHead);
	return path;
}
//! [2]

//! [3]
void Info::updatePosition()
{
	QPointF point = mapFromItem(m_baseItem,0,0);
 	setPos(point);
}
//! [3]

//! [4]
void Info::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
				  QWidget *)
{
	QPen myPen = pen();
	myPen.setColor(m_infoColor);
	painter->setPen(myPen);
	painter->setBrush(m_infoColor);

	QString strInfo;
	QPointF infoPos = mapFromItem(m_baseItem,0,0);
	QPointF infoPosShow = mapToScene(mapFromItem(m_baseItem,0,0));
	strInfo.sprintf("%d,%d",(int)infoPosShow.x(),(int)infoPosShow.y());

	int yOffset = -15;
	if (m_baseItem->pos().y() > 0){
		yOffset = 20;
	}
	painter->drawText(infoPos.x() - 25,infoPos.y() + yOffset, strInfo);
	painter->drawPolygon(m_infoHead);
	if (isSelected()) {
		painter->setPen(QPen(m_infoColor, 1, Qt::DashLine));
	}
}
