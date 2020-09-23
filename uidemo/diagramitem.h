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

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QtWidgets/QGraphicsPixmapItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class Arrow;
class NodeItem;


#define MIN_NODE_DISTANCE 10


//! [0]
class DiagramItem : public QGraphicsItemGroup
{
public:
    enum { Type = UserType + 25 };
    enum DiagramType { DT_POINT, DT_LINE, DT_PATH, DT_RECT, DT_TRAPEZIUM, DT_POLYGON, DT_EMPTYMESH, DT_FULLMESH };

	DiagramItem(DiagramItem::DiagramType itemType,unsigned int itemIndex, QMenu *contextMenu,
        QGraphicsItem *parent = nullptr, QGraphicsScene *scene = nullptr);

	void setBrush(QColor color);
    void removeArrow(Arrow *arrow);
    void removeArrows();
	void addArrow(Arrow *arrow);
	bool createItem(QList<QPointF *> nodePos);
	void selectNode(const QPointF nodePos);
	bool modifyItem(const QPointF nodePos);

    QPolygonF polygon() const
        { return m_polygonF; }
    QPixmap image() const;
    int type() const
        { return Type;}
	DiagramType itemType() const
		{ return m_itemType;}
	unsigned int itemIndex() const {
		return m_itemIndex;
	}

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	DiagramItem::DiagramType m_itemType;
	unsigned int m_itemIndex;

    QPolygonF m_polygonF;
    QMenu *m_contextMenu;
    QList<Arrow *> m_arrows;
	QColor m_itemColor;

	void connectItems(QList<QGraphicsItem *> items,bool needClose, bool withArrow, bool fillCross = false);
};
//! [0]

#endif
