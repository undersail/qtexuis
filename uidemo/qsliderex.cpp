#include "qsliderex.h"
#include <QWheelEvent>

QSliderEx::QSliderEx(Qt::Orientation orientation, QWidget *parent)
	: QSlider(parent)
{
	setOrientation(orientation);
}

QSliderEx::~QSliderEx()
{

}

void QSliderEx::wheelEvent ( QWheelEvent * e )
{
	if (!hasFocus())
	{
 		e->ignore();
	}
}

