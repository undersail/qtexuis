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


#include "nodeitem.h"
#include "arrow.h"
#include "info.h"

//! [0]
NodeItem::NodeItem(unsigned int nodeIndex, QMenu *contextMenu,QGraphicsItem *parent, QGraphicsScene *scene)
     : QGraphicsPolygonItem(parent/*, scene*/)
{
	m_nodeIndex = nodeIndex;
    m_contextMenu = contextMenu;
	m_nodeColor = Qt::red;

    QPainterPath path;
	path.addEllipse(-10,-10,20,20);
	m_polygonF = path.toFillPolygon();
    setPolygon(m_polygonF);
	
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}
//! [0]


void NodeItem::setBrush(QColor color)
{
	m_nodeColor = color;
}


//! [1]
void NodeItem::removeArrow(Arrow *arrow)
{
    int index = m_arrows.indexOf(arrow);
    if (index != -1)
        m_arrows.removeAt(index);
}
//! [1]

//! [2]
void NodeItem::removeArrows()
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
void NodeItem::addArrow(Arrow *arrow)
{
    m_arrows.append(arrow);
}
//! [3]

//! [1]
void NodeItem::removeInfo(Info *info)
{
	int index = m_infos.indexOf(info);
	if (index != -1)
		m_infos.removeAt(index);
}
//! [1]

//! [2]
void NodeItem::removeInfos()
{
	foreach (Info *info, m_infos) {
		info->baseItem()->removeInfo(info);
		scene()->removeItem(info);
		delete info;
	}
}
//! [2]

//! [3]
void NodeItem::addInfo(Info *info)
{
	m_infos.append(info);
}
//! [3]

//! [4]
QPixmap NodeItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(m_nodeColor, 8));
    painter.translate(125, 125);
    painter.drawPolyline(m_polygonF);
    return pixmap;
}
//! [4]

//! [5]
void NodeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    m_contextMenu->exec(event->screenPos());
}
//! [5]

//! [6]
QVariant NodeItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, m_arrows) {
            arrow->updatePosition();
        }
		foreach (Info *info, m_infos) {
			info->updatePosition();
		}
    }
    return value;
}
//! [6]

void NodeItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setPen(QPen(m_nodeColor, 2));
	painter->setBrush(m_nodeColor);
	painter->drawPolyline(m_polygonF);
	qreal r = 10 * 0.707; 
	painter->drawLine(QPointF(-r,-r),QPointF(r,r));
	painter->drawLine(QPointF(-r,r),QPointF(r,-r));
}
