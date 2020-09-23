/****************************************************************************
**
** Copyright (C) 2007-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QMenu>
#include <QtWidgets/QGraphicsSceneContextMenuEvent>

#include "diagramitem.h"
#include "arrow.h"
#include "info.h"

//! [0]
DiagramItem::DiagramItem(DiagramItem::DiagramType itemType,unsigned int itemIndex, QMenu *contextMenu,
             QGraphicsItem *parent, QGraphicsScene *scene)
{
	m_itemType =  itemType;
	m_itemIndex = itemIndex;
    m_contextMenu = contextMenu;
	m_itemColor = Qt::red;
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	QPainterPath path;
	switch (m_itemType) {
		case DT_POINT:
			path.addEllipse(-5,-5,10,10);
			m_polygonF = path.toFillPolygon();
			break;
		case DT_LINE:
			path.moveTo(-100, 0);
			path.lineTo(100, 0);
			m_polygonF = path.toFillPolygon();
			break;
		case DT_PATH:
			path.moveTo(-100, -100);
			path.lineTo(-100, 100);
			path.moveTo(-100, 100);
			path.lineTo(100, 100);
			m_polygonF = path.toFillPolygon();
			break;
		case DT_RECT:
			m_polygonF << QPointF(-100, -100) << QPointF(-100, 100)
				<< QPointF(100, 100) << QPointF(100, -100)
				<< QPointF(-100, -100);
			break;
		case DT_TRAPEZIUM:
			m_polygonF << QPointF(-60, -110) << QPointF(-100, 100)
				<< QPointF(100, 80) << QPointF(110, -80)
				<< QPointF(-60, -110);
			break;
		case DT_POLYGON:
			m_polygonF << QPointF(-110, -20) << QPointF(0, -100) << QPointF(110,-20)
				<< QPointF(60, 100) << QPointF(-60, 100)
				<< QPointF(-110, -20);
			break;
		default:
			m_polygonF << QPointF(-100, -100) << QPointF(-100, 100)
				<< QPointF(100, 100) << QPointF(100, -100)
				<< QPointF(-100, -100);
			setFlag(QGraphicsItem::ItemIsMovable, false);
			break;
	}

}
//! [0]

void DiagramItem::setBrush(QColor color)
{
	m_itemColor = color;
}

//! [1]
void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = m_arrows.indexOf(arrow);

    if (index != -1)
        m_arrows.removeAt(index);
}
//! [1]

//! [2]
void DiagramItem::removeArrows()
{
    foreach (Arrow *arrow, m_arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
//! [2]

//! [3]
void DiagramItem::addArrow(Arrow *arrow)
{
    m_arrows.append(arrow);
}
//! [3]

//! [4]
QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(m_polygonF);
    return pixmap;
}
//! [4]

//! [5]
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    m_contextMenu->exec(event->screenPos());
}
//! [5]

//! [6]
QVariant DiagramItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, m_arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}
//! [6]

bool DiagramItem::createItem(QList<QPointF *> nodePos)
{
    for(int i = 0; i < nodePos.count(); i++ ) {
		QPointF *pos = nodePos.at(i);
        NodeItem *node = new NodeItem(i, m_contextMenu, this);
		node->setBrush(m_itemColor);
 		node->setPos(pos->x(), pos->y());
        node->setFlag(QGraphicsItem::ItemIsMovable, true);
		node->setFlag(QGraphicsItem::ItemIsSelectable, true);
		addToGroup(node);
	}

	switch (m_itemType) {
		case DT_POINT:
			if (nodePos.count() < 1) {
				return false;
			}
			break;
		case DT_LINE:
			if (nodePos.count() < 2) {
				return false;
			}
			connectItems(this->childItems(),false,false);
			break;
		case DT_PATH:
			if (nodePos.count() < 3) {
				return false;
			}
			connectItems(this->childItems(),false,false);
			break;
		case DT_RECT:
			if (nodePos.count() < 4) {
				return false;
			}
			connectItems(this->childItems(),true,false);
			break;
		case DT_TRAPEZIUM:
			if (nodePos.count() < 4) {
				return false;
			}
			connectItems(this->childItems(),true,false);
			break;
		case DT_POLYGON:
			if (nodePos.count() < 3) {
				return false;
			}
			connectItems(this->childItems(),true,false);
			break;
		case DT_EMPTYMESH:
			if (nodePos.count() < 4) {
				return false;
			}
			//connectItems(this->childItems(),true,false);
			break;
		case DT_FULLMESH:
			if (nodePos.count() < 4) {
				return false;
			}
			connectItems(this->childItems(),true,false,true);
			break;
		default:
			break;
	}

	return true;
}

void DiagramItem::selectNode(const QPointF nodePos)
{
	foreach(QGraphicsItem *item,this->childItems()) {
		if (item->isUnderMouse()){
			item->setSelected(true);
		} else{
			item->setSelected(false);
		}
	}
}

bool DiagramItem::modifyItem(const QPointF nodePos)
{
	if (m_itemType == DT_RECT)
	{
		NodeItem *itemMain = NULL;
		NodeItem *itemX = NULL;
		NodeItem *itemY = NULL;
		foreach(QGraphicsItem *item,this->childItems()) {
			if (item->isSelected() && ((NodeItem *)item)->type() == NodeItem::Type){
				itemMain = (NodeItem *)item;
				break;
			}
		}

		foreach(QGraphicsItem *item,this->childItems()) {
			if (itemX != NULL && itemY != NULL) {
				break;
			}
			if (((NodeItem *)item)->type() == NodeItem::Type){
				if (!item->isSelected() && itemMain != NULL && item->x() == itemMain->x()){
					itemX = (NodeItem *)item;
				}
				if (!item->isSelected() && itemMain != NULL && item->y() == itemMain->y()){
					itemY = (NodeItem *)item;
				}
			}
		}

		if (itemMain != NULL && itemX != NULL && itemY != NULL && 
			abs(mapFromScene(nodePos).y() - itemX->y()) >= MIN_NODE_DISTANCE &&
			abs(mapFromScene(nodePos).x() - itemY->x()) >= MIN_NODE_DISTANCE)
		{
			itemMain->setPos(mapFromScene(nodePos));
			itemX->setX(mapFromScene(nodePos).x());
			itemY->setY(mapFromScene(nodePos).y());
			foreach (Info *info, ((NodeItem *)itemMain)->infos()) {
				info->updatePosition();
			}
			return true;
		}
		return false;
	} else if( m_itemType != DT_POINT) {
		foreach(QGraphicsItem *item,this->childItems()) {
			if (item->isSelected() && ((NodeItem *)item)->type() == NodeItem::Type){
				item->setPos(mapFromScene(nodePos));
				foreach (Info *info, ((NodeItem *)item)->infos()) {
 					info->updatePosition();
				}
				return true;
			}
		}
		return false;
	}

	return false;
}

void DiagramItem::connectItems(QList<QGraphicsItem *> items,bool needClose,bool withArrow, bool fillCross)
{
	if (items.count() < 2)
	{
		return;
	}

	for (int i = 0; i<items.count() - 1;i++)
	{
		NodeItem *startItem =
			qgraphicsitem_cast<NodeItem *>(items.at(i));
		NodeItem *endItem =
			qgraphicsitem_cast<NodeItem *>(items.at(i+1));
		Arrow *arrow = new Arrow(startItem, endItem, withArrow);
 		arrow->setColor(m_itemColor);
		startItem->addArrow(arrow);
		endItem->addArrow(arrow);
		arrow->setZValue(-1000.0);
		addToGroup(arrow);
		arrow->updatePosition();

		Info *info = new Info(startItem);
		info->setColor(m_itemColor);
		startItem->addInfo(info);
		info->setZValue(1000.0);
		addToGroup(info);
		info->updatePosition();
	}

	NodeItem *endItem =
		qgraphicsitem_cast<NodeItem *>(items.at(items.count() - 1));
	Info *info = new Info(endItem);
	info->setColor(m_itemColor);
	endItem->addInfo(info);
	info->setZValue(1000.0);
	addToGroup(info);
	info->updatePosition();

	if (needClose)
	{
		NodeItem *startItem =
			qgraphicsitem_cast<NodeItem *>(items.at(items.count() - 1));
		NodeItem *endItem =
			qgraphicsitem_cast<NodeItem *>(items.at(0));
		Arrow *arrow = new Arrow(startItem, endItem, withArrow);
		arrow->setColor(m_itemColor);
		startItem->addArrow(arrow);
		endItem->addArrow(arrow);
		arrow->setZValue(-1000.0);
		addToGroup(arrow);
		arrow->updatePosition();		
	}

	if(fillCross && items.count() >= 4)
	{
		for (int i = 0; i<items.count() - 2;i++)
		{
			//¶Ô½ÇÏß
			NodeItem *startItem =
				qgraphicsitem_cast<NodeItem *>(items.at(i));
			NodeItem *endItem =
				qgraphicsitem_cast<NodeItem *>(items.at(i+2));
			Arrow *arrow = new Arrow(startItem, endItem, false);
			arrow->setColor(m_itemColor);
			startItem->addArrow(arrow);
			endItem->addArrow(arrow);
			arrow->setZValue(-1000.0);
			addToGroup(arrow);
			arrow->updatePosition();
		}
	}
}
