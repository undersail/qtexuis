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

#ifndef NODEITEM_H
#define NODEITEM_H

//#include <QGraphicsPixmapItem>
#include <QList>

#include <QtWidgets/QGraphicsPolygonItem>

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
class Info;

//! [0]
class NodeItem : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };
    NodeItem(unsigned int nodeIndex, QMenu *contextMenu,QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

	void setBrush(QColor color);
    void removeArrow(Arrow *arrow);
    void removeArrows();
	void removeInfo(Info *info);
	void removeInfos();
    QPolygonF polygon() const
        { return m_polygonF; }
    void addArrow(Arrow *arrow);
	void addInfo(Info *info);
    QPixmap image() const;
    int type() const
        { return Type;}
	unsigned int nodeIndex() const {
		return m_nodeIndex;
	}
	QList<Info *> infos() const
	{ return m_infos;}


protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

private:
    QPolygonF m_polygonF;
    QMenu *m_contextMenu;
    QList<Arrow *> m_arrows;
	QList<Info *> m_infos;
	unsigned int m_nodeIndex;
	QColor m_nodeColor;
};
//! [0]

#endif
