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
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include "diagramscene.h"
#include "arrow.h"
#include "qgraphicsviewex.h"

DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
	m_pParent = (QGraphicsViewEx *)parent;
    m_itemMenu = itemMenu;
    m_diagramMode = MoveItem;
    m_lineItem = nullptr;
    m_textItem = nullptr;
    m_colorItem = Qt::red;
    m_colorText = Qt::red;
    m_colorLine = Qt::red;
    m_curItem = nullptr;
    m_pImage = nullptr;
	m_nWidth = 0;
	m_nHeight = 0;
	m_itemIsDrawing = false;
	m_drawingNodeIndex = 0;

	m_backgroundRect;
	m_imgCropRect;
	m_imgResizeRect = QRectF(0,0,400,300);
	m_imgActualRect;

	connect(this,SIGNAL(selectionChanged()),this,SLOT(curItemChanged()));
}


void DiagramScene::setGraphicsView(QGraphicsViewEx *pParent)
{
	if (pParent != NULL)
	{
		m_pParent = pParent;
	}
}


void DiagramScene::setLineColor(const QColor &color)
{
    m_colorLine = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item =
            qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(m_colorLine);
        update();
    }
}


void DiagramScene::setTextColor(const QColor &color)
{
    m_colorText = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item =
            qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(m_colorText);
    }
}


void DiagramScene::setItemColor(const QColor &color)
{
    m_colorItem = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item =
            qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(m_colorItem);
    }
}


void DiagramScene::setTextFont(const QFont &font)
{
    m_fontDiagram = font;
    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item =
            qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setFont(m_fontDiagram);
    }
}


void DiagramScene::setMode(Mode mode)
{
	Mode lastMode = m_diagramMode;
    m_diagramMode = mode;
	if (m_diagramMode == RedrawItem)
	{
		foreach (QGraphicsItem *item, items()) {
			if ( item->type() == DiagramItem::Type) {
				item->setCursor(Qt::CrossCursor);
			}
		}
	}
	else if (m_diagramMode == MoveItem)
	{
		foreach (QGraphicsItem *item, items()) {
			if ( item->type() == DiagramItem::Type) {
				item->setCursor(Qt::OpenHandCursor);
			}
		}
		if (m_diagramMode != lastMode)
		{
			m_itemIsDrawing = false;
			emit itemFinished(m_curItemType,m_curItemIndex);
		}
	}
	else if (m_diagramMode == ModifyItem)
	{
		foreach (QGraphicsItem *item, items()) {
			if ( item->type() == NodeItem::Type) {
				item->setCursor(Qt::DragMoveCursor);
			}
		}
		if (m_diagramMode != lastMode)
		{
			m_itemIsDrawing = false;
			emit itemFinished(m_curItemType,m_curItemIndex);
		}
	}
}

bool DiagramScene::SetCurItemInfo(DiagramItem::DiagramType itemType,
                                  unsigned int itemIndex,
                                  QList<QPointF *> nodePos)
{
	switch (DiagramItem::DiagramType(itemType))
	{
	case DiagramItem::DT_POINT:
		if (nodePos.count() < 1) {
			return false;
		}
		break;
	case DiagramItem::DT_LINE:
		if (nodePos.count() < 2) {
			return false;
		}
		break;
	case DiagramItem::DT_PATH:
		if (nodePos.count() < 3) {
			return false;
		}
		break;
	case DiagramItem::DT_RECT:
		if (nodePos.count() < 4) {
			return false;
		}
		break;
	case DiagramItem::DT_TRAPEZIUM:
		if (nodePos.count() < 4) {
			return false;
		}
		break;
	case DiagramItem::DT_POLYGON:
		if (nodePos.count() < 3) {
			return false;
		}
		break;
	default:
		break;
	}

	m_curItemType = itemType;
	m_curItemIndex = itemIndex;

	foreach (QPointF *pos, m_curNodePos) {
		delete pos;
	}
	m_curNodePos.clear();
	foreach (QPointF *pos, nodePos) {
		m_curNodePos.append(new QPointF(*pos));
	}
	UpdateCurItemPos(false);
	return true;
}

bool DiagramScene::GetCurItemInfo(DiagramItem::DiagramType &itemType,unsigned int &itemIndex,
										   QList<QPointF *> &nodePos)
{
	itemType = m_curItemType;
	itemIndex = m_curItemIndex;
 	UpdateCurItemPos(true);

	foreach (QPointF *pos, nodePos) {
		delete pos;
	}
	nodePos.clear();
	foreach (QPointF *pos, m_curNodePos) {
		nodePos.append(new QPointF(*pos));
	}
	return true;
}

void DiagramScene::UpdateCurItemPos(bool b)
{
	if(b) {
		foreach (QPointF *pos, m_curNodePos) {
			delete pos;
		}
		m_curNodePos.clear();

		QGraphicsItem *item;
		foreach (item, items()) {
			if ( item->type() == DiagramItem::Type &&
				((DiagramItem *)item)->itemType() == m_curItemType && 
				((DiagramItem *)item)->itemIndex() == m_curItemIndex) {
                foreach(QGraphicsItem *node,item->childItems()) {
                    if (((NodeItem *)node)->type() == NodeItem::Type){
                        QPointF *pos = new QPointF(item->mapToScene(item->mapFromItem(node,0,0)));
                        verifyPoint(pos,m_imgCropRect);
                        m_curNodePos.append(pos);
                    }
                }
			}
		}
	} else {
		foreach (QGraphicsItem *item, items()) {
			item->setSelected(false);
			if ( item->type() == DiagramItem::Type &&
				((DiagramItem *)item)->itemType() == m_curItemType && 
				((DiagramItem *)item)->itemIndex() == m_curItemIndex) {
 					qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
					removeItem(item);
			}
		}

		DiagramItem *item = new DiagramItem(m_curItemType,m_curItemIndex,m_itemMenu);
		qreal centerX = 0, centerY = 0;
		foreach(QPointF *pos,m_curNodePos)
		{
			centerX += pos->x();
			centerY += pos->y();
		}
		centerX /= m_curNodePos.count();
		centerY /= m_curNodePos.count();

		QList<QPointF*> newNodePos;
		foreach(QPointF *pos,m_curNodePos)
		{
			QPointF *newPos = new QPointF(pos->x() - centerX, pos->y() - centerY);
			newNodePos.append(newPos);
		}

        m_curItemColor = QColor(Qt::blue);
		item->setBrush(m_curItemColor);
		item->createItem(newNodePos);
		foreach(QPointF *pos,newNodePos) {
			delete pos;
		}
		item->setPos(centerX,centerY);
		addItem(item);
		item->setSelected(true);
		m_curItem = item;
		emit itemInserted(item);
	}
}

void DiagramScene::RemoveItem(DiagramItem::DiagramType itemType,unsigned int itemIndex)
{
	foreach (QGraphicsItem *item, items()) {
		if ( item->type() == DiagramItem::Type &&
			((DiagramItem *)item)->itemType() == itemType && 
			((DiagramItem *)item)->itemIndex() == itemIndex) {
				qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
				removeItem(item);
		}
	}
}

void DiagramScene::RemoveAllItems()
{
	foreach (QGraphicsItem *item, items()) {
		if ( item->type() == DiagramItem::Type ) {
			qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
			removeItem(item);
		}
	}
}

void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->button() == Qt::RightButton) {
		if (m_diagramMode == RedrawItem && m_itemIsDrawing)
		{
			while(m_curItemType == DiagramItem::DT_POLYGON &&
				m_drawingNodeIndex >= 3 && m_curNodePos.count() > m_drawingNodeIndex) {
					delete m_curNodePos.last();
					m_curNodePos.pop_back();
			}
			m_itemIsDrawing = false;
			UpdateCurItemPos(false);
		}
		if (m_curItem != NULL)
		{
			emit itemFinished(m_curItemType,m_curItemIndex);
		}
	}

	if (mouseEvent->button() != Qt::LeftButton)
		return;

	if(!pointInRect(mouseEvent->scenePos(),m_imgCropRect))
		return;

	if (m_curItemType == DiagramItem::DT_POLYGON &&
		m_diagramMode == RedrawItem &&
		pointClosePath(mouseEvent->scenePos(),m_curNodePos))
	{
		return;
	}

    switch (m_diagramMode) {
        case InsertItem:
			UpdateCurItemPos(false);
            break;
        case InsertLine:
            m_lineItem = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            m_lineItem->setPen(QPen(m_colorLine, 2));
            addItem(m_lineItem);
            break;
        case InsertText:
            m_textItem = new DiagramTextItem();
            m_textItem->setFont(m_fontDiagram);
            m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            m_textItem->setZValue(1000.0);
            connect(m_textItem, SIGNAL(lostFocus(DiagramTextItem *)),
                    this, SLOT(editorLostFocus(DiagramTextItem *)));
            connect(m_textItem, SIGNAL(selectedChange(QGraphicsItem *)),
                    this, SIGNAL(itemSelected(QGraphicsItem *)));
            addItem(m_textItem);
            m_textItem->setDefaultTextColor(m_colorText);
            m_textItem->setPos(mouseEvent->scenePos());
            emit textInserted(m_textItem);
			break;
		case RedrawItem:
			if(!m_itemIsDrawing) {
				m_itemIsDrawing = true;
				m_drawingNodeIndex = 0;
				while(m_curItemType == DiagramItem::DT_POLYGON &&
					m_curNodePos.count() > 0) {
						delete m_curNodePos.last();
						m_curNodePos.pop_back();
				}
				UpdateCurItemPos(false);
			}
			if (m_curItemType == DiagramItem::DT_POLYGON) {
				if(m_drawingNodeIndex < m_curNodePos.count()) {
					*m_curNodePos.at(m_drawingNodeIndex) = mouseEvent->scenePos();
					m_drawingNodeIndex++;
				} else if(m_drawingNodeIndex < MAX_POLYGON_POINT_NUM) {
					QPointF *newNode = new QPointF(mouseEvent->scenePos());
					m_curNodePos.append(newNode);
					m_drawingNodeIndex++;
				}
				if (m_drawingNodeIndex >= MAX_POLYGON_POINT_NUM) {
					while(m_drawingNodeIndex >= 0 &&
						m_curNodePos.count() > m_drawingNodeIndex) {
						delete m_curNodePos.last();
						m_curNodePos.pop_back();
					}
					m_itemIsDrawing = false;
					emit itemFinished(m_curItemType,m_curItemIndex);
				}

			} else if(m_curItemType == DiagramItem::DT_RECT) {
				if(m_drawingNodeIndex == 0) {
					*m_curNodePos.at(0) = mouseEvent->scenePos();
					m_curNodePos.at(1)->setX(mouseEvent->scenePos().x());
					m_curNodePos.at(3)->setY(mouseEvent->scenePos().y());
					m_drawingNodeIndex++;
				} else {
					*m_curNodePos.at(2) = mouseEvent->scenePos();
					m_curNodePos.at(1)->setY(mouseEvent->scenePos().y());
					m_curNodePos.at(3)->setX(mouseEvent->scenePos().x());
					m_itemIsDrawing = false;
					emit itemFinished(m_curItemType,m_curItemIndex);
				}				
			} else {
				if(m_drawingNodeIndex < m_curNodePos.count()) {
					*m_curNodePos.at(m_drawingNodeIndex) = mouseEvent->scenePos();
					m_drawingNodeIndex++;
				}
				if (m_drawingNodeIndex >= m_curNodePos.count()) {
					m_itemIsDrawing = false;
					emit itemFinished(m_curItemType,m_curItemIndex);
				}
			}
			UpdateCurItemPos(false);
			break;
		case MoveItem:
			break;
		case ModifyItem:
			foreach (QGraphicsItem *item, selectedItems()) {
				if (((DiagramItem *)item)->itemIndex() == m_curItemIndex) {
					((DiagramItem *)item)->selectNode(mouseEvent->scenePos());
					break;
				}
			}
			break;
    default:
        break;
    }
	update();
    QGraphicsScene::mousePressEvent(mouseEvent);
}


void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if(!pointInRect(mouseEvent->scenePos(),m_imgCropRect))
		return;

	if (m_curItemType == DiagramItem::DT_POLYGON &&
		m_diagramMode == RedrawItem &&
		pointClosePath(mouseEvent->scenePos(),m_curNodePos))
	{
		return;
	}

    if (m_diagramMode == InsertLine && m_lineItem != 0) {
        QLineF newLine(m_lineItem->line().p1(), mouseEvent->scenePos());
        m_lineItem->setLine(newLine);
    } else if (m_diagramMode == MoveItem) {
 		emit itemChanging(m_curItemType,m_curItemIndex);
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    } else if (m_diagramMode == ModifyItem && m_curItem != NULL) {
		m_curItem->modifyItem(mouseEvent->scenePos());
 		emit itemChanging(m_curItemType,m_curItemIndex);
    }
	update();
}


void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if(!pointInRect(mouseEvent->scenePos(),m_imgCropRect))
		return;

	if (m_curItemType == DiagramItem::DT_POLYGON &&
		m_diagramMode == RedrawItem &&
		pointClosePath(mouseEvent->scenePos(),m_curNodePos))
	{
		return;
	}

    m_lineItem = 0;
	if (m_diagramMode == RedrawItem || m_diagramMode == ModifyItem) {
 		UpdateCurItemPos(true);
		UpdateCurItemPos(false);
	}
	if (m_curItem != NULL && mouseEvent->button() == Qt::LeftButton)
	{
		emit itemChanged(m_curItemType,m_curItemIndex);
		if ( m_diagramMode == MoveItem || m_diagramMode == ModifyItem)
		{
			emit itemFinished(m_curItemType,m_curItemIndex);
		}
	}
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}


bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

//判断点坐标是否在矩形内部
bool DiagramScene::pointInRect(const QPointF point, const QRectF rect)
{
	if (point.x() >= rect.left() && point.x() <= rect.right() &&
		point.y() >= rect.top() && point.y() <= rect.bottom())
	{
		return true;
	}

	return false;
}

//判断如果将点连接到折线末尾是否会将折线闭合　返回值：0(不会闭合) 1(刚好闭合) 2(有交叉)
int DiagramScene::pointClosePath(const QPointF point,const QList<QPointF *> path)
{
	if (path.count() == 0)
	{
		return 0;
	}

	if (point == *path[0])
	{
		return 1;
	}

	QLineF line1 = QLineF(*path[0],point);
	QLineF line2 = QLineF(*path[path.count() - 1],point);
	for (int i = 0; i < path.count()-1; i++ )
	{
		QPointF intersectionPoint1;
		QPointF intersectionPoint2;
		QLineF lineOld = QLineF(*path[i],*path[i+1]);
		if ((line1.intersect(lineOld,&intersectionPoint1) == QLineF::BoundedIntersection && intersectionPoint1 != *path[0]) || 
			(line2.intersect(lineOld,&intersectionPoint2) == QLineF::BoundedIntersection && intersectionPoint2 != *path[path.count() - 1]))
		{
			return 2;
		}
	}

	return 0;
}


void DiagramScene::verifyPoint(QPointF *point, QRectF rect)
{
	if (point->x() < rect.left()) point->setX(rect.left());
	if (point->x() > rect.right()) point->setX(rect.right());
	if (point->y() < rect.top()) point->setY(rect.top());
	if (point->y() > rect.bottom()) point->setY(rect.bottom());
}


//////////////////////////////////////////////////////////////////////////

void DiagramScene::curItemChanged()
{
	m_curItem = NULL;
	foreach (QGraphicsItem *item, selectedItems()) {
		if (item->type() == DiagramItem::Type) {
			m_curItem = (DiagramItem *)item;
			m_curItemType = m_curItem->itemType();
			m_curItemIndex = m_curItem->itemIndex();
			break;
		}
	}
}

void DiagramScene::drawBackground ( QPainter * painter, const QRectF & rect )
{
	m_backgroundRect = rect;
	m_imgCropRect = m_backgroundRect;
	if (m_backgroundRect.left() < 0) m_imgCropRect.setLeft(0);
	if (m_backgroundRect.top() < 0) m_imgCropRect.setTop(0);
	if (m_backgroundRect.right() > sceneRect().right()) m_imgCropRect.setRight( sceneRect().right());
	if (m_backgroundRect.bottom() > sceneRect().bottom()) m_imgCropRect.setBottom( sceneRect().bottom());

	m_imgResizeRect = m_imgCropRect;
	if (m_pParent->m_totalScaleFactor < 1 && m_pParent->m_totalScaleFactor > 0)
	{
		m_imgResizeRect.setRect(
			m_imgCropRect.left() * m_pParent->m_totalScaleFactor,
			m_imgCropRect.top() * m_pParent->m_totalScaleFactor,
			m_imgCropRect.width() * m_pParent->m_totalScaleFactor,
			m_imgCropRect.height() * m_pParent->m_totalScaleFactor);
	}

	if (m_pImage != NULL)
	{
 		painter->drawImage(m_imgCropRect, *m_pImage, m_imgActualRect);  
	}

	QGraphicsScene::drawBackground(painter,rect);
}
