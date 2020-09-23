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

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QtWidgets/QGraphicsScene>
#include "diagramitem.h"
#include "diagramtextitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsViewEx;
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

#define MAX_POLYGON_POINT_NUM 10

//! [0]
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, RedrawItem, MoveItem, ModifyItem };

    DiagramScene(QMenu *itemMenu, QObject *parent = nullptr);
	void setGraphicsView(QGraphicsViewEx *pParent);

    QFont font() const
        { return m_fontDiagram; }
    QColor textColor() const
        { return m_colorText; }
    QColor itemColor() const
        { return m_colorItem; }
    QColor lineColor() const
        { return m_colorLine; }

	QRectF backgroundRect() const
	{ return m_backgroundRect; }
	QRectF imgCropRect() const
	{ return m_imgCropRect; }
	QRectF imgResizeRect() const
	{ return m_imgResizeRect; }
	QRectF imgActualRect() const
	{ return m_imgActualRect; }
	Mode diagramMode() const
	{ return m_diagramMode; }

    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setTextFont(const QFont &font);
	bool SetCurItemInfo(
        DiagramItem::DiagramType itemType, unsigned int itemIndex, QList<QPointF *> nodePos);
	bool GetCurItemInfo(
        DiagramItem::DiagramType &itemType, unsigned int &itemIndex, QList<QPointF *> &nodePos);
	void UpdateCurItemPos(bool b);
    void RemoveItem(DiagramItem::DiagramType itemType, unsigned int itemIndex);
	void RemoveAllItems();

	//图像参数
	QImage *m_pImage;
	unsigned int m_nWidth;
	unsigned int m_nHeight;

public slots:
    void setMode(Mode mode);
    void editorLostFocus(DiagramTextItem *item);
	void curItemChanged();

signals:
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
	void itemChanging(DiagramItem::DiagramType itemType,unsigned int itemIndex);
	void itemChanged(DiagramItem::DiagramType itemType,unsigned int itemIndex);
	void itemFinished(DiagramItem::DiagramType itemType,unsigned int itemIndex);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void drawBackground (QPainter * painter, const QRectF & rect);

private:
    bool isItemChange(int type);
	//判断点坐标是否在矩形内部
	bool pointInRect(const QPointF point, const QRectF rect);
	//判断如果将点连接到折线末尾是否会将折线闭合　返回值：0(不会闭合) 1(刚好闭合) 2(有交叉)
	int pointClosePath(const QPointF point,const QList<QPointF *> path);
	void verifyPoint(QPointF *point, QRectF rect);

	QGraphicsViewEx *m_pParent;
	DiagramItem::DiagramType m_curItemType;
	unsigned int m_curItemAlgType;
	unsigned int m_curItemIndex;
	QColor m_curItemColor;
	QList<QPointF *> m_curNodePos;
	DiagramItem *m_curItem;

	bool m_itemIsDrawing;
	int m_drawingNodeIndex;
	QRectF m_backgroundRect;
	QRectF m_imgCropRect;
	QRectF m_imgResizeRect;
	QRectF m_imgActualRect;

    QMenu *m_itemMenu;
    Mode m_diagramMode;
    bool m_bLeftButtonDown;
    QPointF m_pointStart;
    QGraphicsLineItem *m_lineItem;
    QFont m_fontDiagram;
    DiagramTextItem *m_textItem;
	QColor m_colorItem;
    QColor m_colorText;
    QColor m_colorLine;
};
//! [0]

#endif
