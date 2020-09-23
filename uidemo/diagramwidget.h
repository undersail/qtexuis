#ifndef DIAGRAMWIDGET_H
#define DIAGRAMWIDGET_H

#include <QtGui>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>


#include "diagramscene.h"


class DiagramWidget : public QWidget
{
	Q_OBJECT

public:
	DiagramWidget(QWidget *parent);
	DiagramWidget(DiagramItem::DiagramType itemType,unsigned int itemIndex,
        DiagramScene *diagramScene,QMenu *contextMenu,QWidget *parent = nullptr);
	~DiagramWidget();

    bool setValue(const QList<QPointF*>& itemPos);
    bool getValue(DiagramItem::DiagramType& itemType, unsigned int& itemIndex, QList<QPointF*>& itemPos);

public slots:
	void finishWidget(DiagramItem::DiagramType itemType, unsigned int itemIndex);
	void itemButtonClicked(bool checked);

signals:
	void valueChanged();

private:
	
	QMenu *m_contextMenu;
	DiagramScene *m_diagramScene;
	DiagramItem::DiagramType m_itemType;
	unsigned int m_itemIndex;
	QToolButton *m_button;
	QLabel *m_label;
	QList<QPointF *> m_itemPos;

};

#endif // DIAGRAMWIDGET_H
