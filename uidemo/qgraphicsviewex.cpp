#include "qgraphicsviewex.h"
#include "diagramscene.h"

//Qt includes
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QTextStream>

QGraphicsViewEx::QGraphicsViewEx(QWidget *parent)
	: QGraphicsView(parent)
{
	m_nControlMode = CM_CTRLSCENE;
 	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	//Set-up the scene
	QGraphicsScene *scene = new QGraphicsScene(this);
	setScene(scene);

	//Set-up the view
	setSceneRect(0, 0, 1000, 1000);
	SetCenter(QPointF(500.0, 500.0)); //A modified version of centerOn(), handles special cases
	setCursor(Qt::OpenHandCursor);
}

QGraphicsViewEx::QGraphicsViewEx ( DiagramScene * scene, QWidget * parent)
	: QGraphicsView(parent)
{
	m_nControlMode = CM_CTRLITEM;
 	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
 
 	//Set-up the scene
 	diagramScene = scene;
 	setScene((QGraphicsScene*)diagramScene);
	m_totalScaleFactor = 1;

 	//Set-up the view
// 	setSceneRect(0, 0, 1000, 1000);
// 	SetCenter(QPointF(500.0, 500.0)); //A modified version of centerOn(), handles special cases
//  setCursor(Qt::OpenHandCursor);
}

QGraphicsViewEx::~QGraphicsViewEx()
{

}

void QGraphicsViewEx::focusInEvent ( QFocusEvent * event )
{

}

bool QGraphicsViewEx::focusNextPrevChild ( bool next )
{

	return QGraphicsView::focusNextPrevChild(next);
}

void QGraphicsViewEx::focusOutEvent ( QFocusEvent * event )
{

}

void QGraphicsViewEx::inputMethodEvent ( QInputMethodEvent * event )
{

}

void QGraphicsViewEx::keyPressEvent ( QKeyEvent * event )
{

}

void QGraphicsViewEx::keyReleaseEvent ( QKeyEvent * event )
{

}


/*
void QGraphicsViewEx::mouseDoubleClickEvent ( QMouseEvent * event )
{
	double scaleFactorX = diagramScene->visibleRect().width()/diagramScene->sceneRect().width();
	double scaleFactorY = diagramScene->visibleRect().height()/diagramScene->sceneRect().height();
	double scaleFactor = scaleFactorX < scaleFactorY ? scaleFactorX : scaleFactorY;
	if(scaleFactor <= 0) {
		return;
	}

	scale(scaleFactor, scaleFactor);
}

void QGraphicsViewEx::mouseMoveEvent ( QMouseEvent * event )
{
	if(!LastPanPoint.isNull()) {
		//Get how much we panned
		QPointF delta = mapToScene(LastPanPoint) - mapToScene(event->pos());
		LastPanPoint = event->pos();

		//Update the center ie. do the pan
		SetCenter(GetCenter() + delta);
	}
}

void QGraphicsViewEx::mousePressEvent ( QMouseEvent * event )
{
	QGraphicsEllipseItem *item = new QGraphicsEllipseItem();
	item->setBrush(Qt::white);
	item->setPos(event->posF());
	item->setRect(event->posF().x(),event->posF().y(),10,10);
	Scene->addItem(item);
	Scene->update();
	update();

	QGraphicsRectItem *item2 = new QGraphicsRectItem();
	item2->setBrush(Qt::white);
 	item2->setPos(event->posF());
	item2->setRect(event->posF().x(),event->posF().y(),10,10);
	Scene->addItem(item2);
	Scene->update();
	update();

	//For panning the view
	LastPanPoint = event->pos();
	setCursor(Qt::ClosedHandCursor);
}

void QGraphicsViewEx::mouseReleaseEvent ( QMouseEvent * event )
{
	setCursor(Qt::OpenHandCursor);
	LastPanPoint = QPoint();
}

void QGraphicsViewEx::paintEvent ( QPaintEvent * event )
{

}

void QGraphicsViewEx::scrollContentsBy ( int dx, int dy )
{

}

*/


void QGraphicsViewEx::resizeEvent ( QResizeEvent * event )
{
	//Get the rectangle of the visible area in scene coords
	QRectF visibleArea = mapToScene(rect()).boundingRect();
	SetCenter(visibleArea.center());

	//Call the subclass resize so the scrollbars are updated correctly
	QGraphicsView::resizeEvent(event);
}

void QGraphicsViewEx::showEvent ( QShowEvent * event )
{

}

bool QGraphicsViewEx::viewportEvent ( QEvent * event )
{

	return QGraphicsView::viewportEvent(event);
}

void QGraphicsViewEx::wheelEvent ( QWheelEvent * event )
{

	//Get the position of the mouse before scaling, in scene coords
	QPointF pointBeforeScale(mapToScene(event->pos()));

	//Get the original screen centerpoint
	QPointF screenCenter = GetCenter(); //CurrentCenterPoint; //(visRect.center());

	//Scale the view ie. do the zoom
	double scaleFactor = 1.15; //How fast we zoom
  	double scaleFactorX = diagramScene->backgroundRect().width()/diagramScene->sceneRect().width();
	if(event->delta() > 0 && scaleFactorX > 0.2) {
		//Zoom in
		scale(scaleFactor, scaleFactor);
		m_totalScaleFactor *= scaleFactor;
	} else if(event->delta() <= 0 && scaleFactorX < 5){
		//Zooming out
		scale(1.0 / scaleFactor, 1.0 / scaleFactor);
		m_totalScaleFactor /= scaleFactor;
	}

	//Get the position after scaling, in scene coords
	QPointF pointAfterScale(mapToScene(event->pos()));

	//Get the offset of how the screen moved
	QPointF offset = pointBeforeScale - pointAfterScale;

	//Adjust to the new center for correct zooming
	QPointF newCenter = screenCenter + offset;
// 	SetCenter(newCenter);
}


/**
* Sets the current centerpoint.  Also updates the scene's center point.
* Unlike centerOn, which has no way of getting the floating point center
* back, SetCenter() stores the center point.  It also handles the special
* sidebar case.  This function will claim the centerPoint to sceneRec ie.
* the centerPoint must be within the sceneRec.
*/
//Set the current centerpoint in the
void QGraphicsViewEx::SetCenter(const QPointF& centerPoint) {
	//Get the rectangle of the visible area in scene coords
	QRectF visibleArea = mapToScene(rect()).boundingRect();

	//Get the scene area
	QRectF sceneBounds = sceneRect();

	double boundX = visibleArea.width() / 2.0;
	double boundY = visibleArea.height() / 2.0;
	double boundWidth = sceneBounds.width() - 2.0 * boundX;
	double boundHeight = sceneBounds.height() - 2.0 * boundY;

	//The max boundary that the centerPoint can be to
	QRectF bounds(boundX, boundY, boundWidth, boundHeight);

	if(bounds.contains(centerPoint)) {
		//We are within the bounds
		CurrentCenterPoint = centerPoint;
	} else {
		//We need to clamp or use the center of the screen
		if(visibleArea.contains(sceneBounds)) {
			//Use the center of scene ie. we can see the whole scene
			CurrentCenterPoint = sceneBounds.center();
		} else {

			CurrentCenterPoint = centerPoint;

			//We need to clamp the center. The centerPoint is too large
			if(centerPoint.x() > bounds.x() + bounds.width()) {
				CurrentCenterPoint.setX(bounds.x() + bounds.width());
			} else if(centerPoint.x() < bounds.x()) {
				CurrentCenterPoint.setX(bounds.x());
			}

			if(centerPoint.y() > bounds.y() + bounds.height()) {
				CurrentCenterPoint.setY(bounds.y() + bounds.height());
			} else if(centerPoint.y() < bounds.y()) {
				CurrentCenterPoint.setY(bounds.y());
			}

		}
	}

	//Update the scrollbars
	centerOn(CurrentCenterPoint);
}