#include "diagramwidget.h"

#include <QtWidgets/QGridLayout>


DiagramWidget::DiagramWidget(QWidget *parent)
	: QWidget(parent)
{

}

DiagramWidget::DiagramWidget(DiagramItem::DiagramType itemType, unsigned int itemIndex,
                             DiagramScene *diagramScene, QMenu *contextMenu, QWidget *parent)
	: QWidget(parent)

{
	m_itemType = itemType;
	m_itemIndex = itemIndex;
	m_contextMenu = contextMenu;
	m_diagramScene = diagramScene;

	DiagramItem item(m_itemType,m_itemIndex,m_contextMenu);
	QIcon icon(item.image());

	m_button = new QToolButton;
	m_button->setIcon(icon);
	m_button->setIconSize(QSize(24,24));
	m_button->setCheckable(true);

	m_label = new QLabel("0,0;0,0");
	m_label->setFrameShape(QFrame::Box);
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(m_button, 0, 0, Qt::AlignVCenter);
	layout->addWidget(m_label, 0, 1, Qt::AlignVCenter);
	setLayout(layout);

	connect(m_diagramScene,SIGNAL(itemChanged(DiagramItem::DiagramType,unsigned int)),
		this,SLOT(updateWidget(DiagramItem::DiagramType,unsigned int)));
	connect(m_diagramScene,SIGNAL(itemFinished(DiagramItem::DiagramType,unsigned int)),
		this,SLOT(finishWidget(DiagramItem::DiagramType,unsigned int)));
	connect(m_button,SIGNAL(clicked( bool)),this,SLOT(itemButtonClicked(bool)));
}

DiagramWidget::~DiagramWidget()
{

}

bool DiagramWidget::setValue(const QList<QPointF*>& itemPos)
{
	int i = 0;
	QString strTemp;
	QString strInfo;
	foreach (QPointF *pos, m_itemPos) {
		delete pos;
	}
	m_itemPos.clear();
	switch(m_itemType)
	{
	case DiagramItem::DT_POINT:
        m_itemPos.append(itemPos.at(0));
        strInfo.sprintf("%d, %d;", (short)itemPos.at(0)->x(), (short)itemPos.at(0)->y());
		break;
	case DiagramItem::DT_LINE:
        m_itemPos.append(itemPos.at(0));    // start
        m_itemPos.append(itemPos.at(1));    // end
        strInfo.sprintf("%d, %d; %d, %d",
                        (short)itemPos.at(0)->x(), (short)itemPos.at(0)->y(),
                        (short)itemPos.at(1)->x(), (short)itemPos.at(1)->y());
		break;
	case DiagramItem::DT_RECT:
        m_itemPos.append(itemPos.at(0));    // left-top
        m_itemPos.append(itemPos.at(1));    // left-bottom
        m_itemPos.append(itemPos.at(2));    // right-top
        m_itemPos.append(itemPos.at(3));    // right-bottom
        strInfo.sprintf("%d, %d; %d, %d",
                        (short)itemPos.at(0)->x(), (short)itemPos.at(0)->y(),
                        (short)itemPos.at(3)->x(), (short)itemPos.at(3)->y());

		break;
	case DiagramItem::DT_POLYGON:
        for (i = 0; i < itemPos.size(); i++){
            m_itemPos.append(itemPos[i]);
            strTemp.sprintf("%d, %d;", itemPos[i]->x(), itemPos[i]->y());
			strInfo += strTemp;
		}
		break;
	default:
		return false;
	}

 	if (m_button->isChecked()) {
        m_diagramScene->SetCurItemInfo(m_itemType,m_itemIndex,m_itemPos);
		finishWidget(m_itemType,m_itemIndex);
 	}
	m_label->setText(strInfo);
	return true;
}

bool DiagramWidget::getValue(DiagramItem::DiagramType& itemType, unsigned int& itemIndex, QList<QPointF*>& itemPos)
{
    return m_diagramScene->GetCurItemInfo(itemType, itemIndex, itemPos);

}

void DiagramWidget::finishWidget(DiagramItem::DiagramType itemType, unsigned int itemIndex)
{
	if (itemType != m_itemType || itemIndex != m_itemIndex) {
		return;
	}

	emit valueChanged();
}

void DiagramWidget::itemButtonClicked(bool checked)
{
	if (checked) {
        m_diagramScene->SetCurItemInfo(m_itemType,m_itemIndex,m_itemPos);
	}
	else {
		m_diagramScene->RemoveItem(m_itemType,m_itemIndex);
	}
}
