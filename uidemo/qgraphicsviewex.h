#ifndef QGRAPHICSVIEWEX_H
#define QGRAPHICSVIEWEX_H

#include <QtWidgets/QGraphicsView>

class ThinkCamWorkstation;
class DiagramScene;
class QGraphicsViewEx : public QGraphicsView
{
	Q_OBJECT

public:

	QGraphicsViewEx(QWidget *parent);
	QGraphicsViewEx ( DiagramScene * scene, QWidget * parent = 0 );
	~QGraphicsViewEx();

	typedef enum eControlMode
	{
		CM_CTRLSCENE,
		CM_CTRLITEM
	}ControlMode;

	ThinkCamWorkstation *m_pParent;
	int m_nControlMode;
	DiagramScene* diagramScene;
	QPainter painter;

	double m_totalScaleFactor;
	
private:

	//Holds the current centerpoint for the view, used for panning and zooming
	QPointF CurrentCenterPoint;

	//From panning the view
	QPoint LastPanPoint;

	//scaleFactor

	//Set the current centerpoint in the
	void SetCenter(const QPointF& centerPoint);
	QPointF GetCenter() { return CurrentCenterPoint; }
	void paint(QImage& image);


protected:
	void	focusInEvent ( QFocusEvent * event );
	bool	focusNextPrevChild ( bool next );
	void	focusOutEvent ( QFocusEvent * event );
	void	inputMethodEvent ( QInputMethodEvent * event );
	void	keyPressEvent ( QKeyEvent * event );
	void	keyReleaseEvent ( QKeyEvent * event );
// 	void	mouseDoubleClickEvent ( QMouseEvent * event );
// 	void	mouseMoveEvent ( QMouseEvent * event );
//	void	mousePressEvent ( QMouseEvent * event );
// 	void	mouseReleaseEvent ( QMouseEvent * event );
// 	void	paintEvent ( QPaintEvent * event );
// 	void	scrollContentsBy ( int dx, int dy );
	void	resizeEvent ( QResizeEvent * event );
	void	showEvent ( QShowEvent * event );
	bool	viewportEvent ( QEvent * event );
	void	wheelEvent ( QWheelEvent * event );
	
};

#endif // QGRAPHICSVIEWEX_H
