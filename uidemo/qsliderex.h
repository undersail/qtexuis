#ifndef QSLIDEREX_H
#define QSLIDEREX_H

#include <QtWidgets/QSlider>

class QSliderEx : public QSlider
{
	Q_OBJECT

public:
	QSliderEx(Qt::Orientation orientation, QWidget *parent);
	~QSliderEx();

private:
	
protected:
	void wheelEvent ( QWheelEvent * e );

};

#endif // QSLIDEREX_H
