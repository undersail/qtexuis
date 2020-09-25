#include <QToolBar>
#include <QToolButton>
#include <QGroupBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

const int InsertTextButton = 10;

QString trx(const char *str)
{
    return QString::fromLocal8Bit(str);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    appPath = QApplication::applicationDirPath();
    appName = QApplication::applicationDisplayName();
    appVersion = QApplication::applicationVersion();
    appIntro = trx("Qt UI Demo by Jack");

    createExActions();
    createExToolbars();
    createExToolBox();
    InitWidgets();
    InitActions();
    InstallEventFilters();
    ConnectSlots();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createExToolBox()
{
    buttonGroupItem = new QButtonGroup;
    buttonGroupItem->setExclusive(false);
    connect(buttonGroupItem, SIGNAL(buttonClicked(int)),
        this, SLOT(itemButtonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createItemCellWidget(trx("Point"),
        DiagramItem::DT_POINT, 1), 0, 0);
    layout->addWidget(createItemCellWidget(trx("Line"),
        DiagramItem::DT_LINE, 1),1, 0);
    layout->addWidget(createItemCellWidget(trx("Path"),
        DiagramItem::DT_PATH, 1), 2, 0);
    layout->addWidget(createItemCellWidget(trx("Rect"),
        DiagramItem::DT_RECT, 1), 3, 0);
    layout->addWidget(createItemCellWidget(trx("Trapezium"),
        DiagramItem::DT_TRAPEZIUM, 1), 4, 0);
    layout->addWidget(createItemCellWidget(trx("Polygon"),
        DiagramItem::DT_POLYGON, 1), 5, 0);
    layout->setRowStretch(6, 10);
    layout->setColumnStretch(1, 10);
    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    buttonGroupBackground = new QButtonGroup;
    connect(buttonGroupBackground, SIGNAL(buttonClicked(QAbstractButton *)),
        this, SLOT(backgroundButtonGroupClicked(QAbstractButton *)));
    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(trx("Blue Grid"), trx(":/background1.png")), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(trx("White Grid"), trx(":/background2.png")), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(trx("Gray Grid"), trx(":/background3.png")), 2, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(trx("No Grid"), trx(":/background4.png")), 3, 0);
    backgroundLayout->setRowStretch(4, 10);
    backgroundLayout->setColumnStretch(1, 10);
    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);

    toolBoxDiagram = new QToolBox;
    toolBoxDiagram->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBoxDiagram->setMinimumWidth(itemWidget->sizeHint().width());
    toolBoxDiagram->addItem(itemWidget, trx("图形"));
    toolBoxDiagram->addItem(backgroundWidget, trx("背景"));
}

void MainWindow::createExToolbars()
{
    QToolButton *toolButtonRedraw = new QToolButton;
    toolButtonRedraw->setCheckable(true);
    toolButtonRedraw->setChecked(true);
    toolButtonRedraw->setIcon(QIcon(trx(":/linepointer.png")));
    toolButtonRedraw->setToolTip(trx("重绘图形"));
    QToolButton *toolButtonMove = new QToolButton;
    toolButtonMove->setCheckable(true);
    toolButtonMove->setChecked(true);
    toolButtonMove->setIcon(QIcon(trx(":/pointer.png")));
    toolButtonMove->setToolTip(trx("移动图形"));
    QToolButton *toolButtonModify = new QToolButton;
    toolButtonModify->setCheckable(true);
    toolButtonModify->setIcon(QIcon(trx(":/Modify.png")));
    toolButtonModify->setToolTip(trx("调整图形"));

    buttonGroupPointer = new QButtonGroup;
    buttonGroupPointer->addButton(toolButtonRedraw,int(DiagramScene::RedrawItem));
    buttonGroupPointer->addButton(toolButtonMove, int(DiagramScene::MoveItem));
    buttonGroupPointer->addButton(toolButtonModify, int(DiagramScene::ModifyItem));
    // setItemEnabled(1,false);

    connect(buttonGroupPointer, SIGNAL(buttonClicked(int)),
        this, SLOT(pointerButtonGroupClicked(int)));

    toolBarDiagram = addToolBar(trx("Diagram"));
    toolBarDiagram->addWidget(toolButtonRedraw);
    toolBarDiagram->addWidget(toolButtonMove);
    toolBarDiagram->addWidget(toolButtonModify);
    toolBarDiagram->addSeparator();
    toolBarDiagram->addAction(zoomInAction);
    toolBarDiagram->addAction(zoomOutAction);
    toolBarDiagram->addAction(zoomFitAction);
    toolBarDiagram->addSeparator();
    toolBarDiagram->addAction(deleteAction);
    toolBarDiagram->addAction(toFrontAction);
    toolBarDiagram->addAction(sendBackAction);
}

void MainWindow::createExActions()
{
    zoomInAction = new QAction(QIcon(trx(":/ZoomIn.png")), trx("放大"), this);
    zoomInAction->setShortcut(trx("Ctrl+I"));
    zoomInAction->setStatusTip(trx("放大图形"));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(ZoomInImg()));

    zoomOutAction = new QAction(QIcon(trx(":/ZoomOut.png")), trx("缩小"), this);
    zoomOutAction->setShortcut(trx("Ctrl+O"));
    zoomOutAction->setStatusTip(trx("缩小图形"));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(ZoomOutImg()));

    zoomFitAction = new QAction(QIcon(trx(":/ZoomToFit.png")), trx("适应窗口"), this);
    zoomFitAction->setShortcut(trx("Ctrl+T"));
    zoomFitAction->setStatusTip(trx("使图形适应窗口"));
    connect(zoomFitAction, SIGNAL(triggered()), this, SLOT(ZoomToFitImg()));

    toFrontAction = new QAction(QIcon(trx(":/bringtofront.png")), trx("前置"), this);
    toFrontAction->setShortcut(trx("Ctrl+F"));
    toFrontAction->setStatusTip(trx("将图形对象移到前方"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    sendBackAction = new QAction(QIcon(trx(":/sendtoback.png")), trx("后置"), this);
    sendBackAction->setShortcut(trx("Ctrl+B"));
    sendBackAction->setStatusTip(trx("将图形对象移到后方"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(trx(":/delete.png")), trx("删除"), this);
    deleteAction->setShortcut(trx("Delete"));
    deleteAction->setStatusTip(trx("将图形对象删除"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));
}


QWidget *MainWindow::createBackgroundCellWidget(const QString &text,
                                                const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroupBackground->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}


QWidget *MainWindow::createItemCellWidget(const QString &text,
                                      DiagramItem::DiagramType type, unsigned int index)
{
    DiagramItem item(type,index,menuDiagram);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroupItem->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}


void MainWindow::InitWidgets()
{
    this->setWindowTitle(appName + " " + appVersion);
    this->setWhatsThis(appIntro);


    QLabel *labelLogo = new QLabel(this);
    labelLogo->setPixmap(QPixmap(trx(":/View.png")));
    ui->menubar->setCornerWidget(labelLogo);
// 	connect(labelLogo,SIGNAL(clicked()), this, SLOT(ShowAboutInfo()));

    ui->centralwidget->setLayout(ui->gridLayoutAll);
    ui->gridLayoutAll->addWidget(ui->dockWidgetDiagram);
    menuDiagram = new QMenu((trx("Diagram &Item")));
    menuDiagram->addAction(zoomInAction);
    menuDiagram->addAction(zoomOutAction);
    menuDiagram->addAction(zoomFitAction);
    menuDiagram->addSeparator();
    menuDiagram->addAction(deleteAction);
    menuDiagram->addAction(toFrontAction);
    menuDiagram->addAction(sendBackAction);

    diagramScene = new DiagramScene(menuDiagram);
    diagramScene->setSceneRect(QRectF(0, 0, 1600, 1200));

    connect(diagramScene, SIGNAL(itemInserted(DiagramItem *)),
        this, SLOT(itemInserted(DiagramItem *)));
    connect(diagramScene, SIGNAL(textInserted(QGraphicsTextItem *)),
        this, SLOT(textInserted(QGraphicsTextItem *)));
    connect(diagramScene, SIGNAL(itemSelected(QGraphicsItem *)),
        this, SLOT(itemSelected(QGraphicsItem *)));


    ui->dockWidgetTool->setWidget(toolBoxDiagram);
    ui->dockWidgetTool->setFeatures(QDockWidget::NoDockWidgetFeatures);
    ui->gridLayoutAll->addWidget(ui->dockWidgetTool,0,0);

    graphicsViewEx = new QGraphicsViewEx(diagramScene,this);
    diagramScene->setGraphicsView(graphicsViewEx);
    graphicsViewEx->setObjectName((trx("GVE")));							// 设置对象句,相当于css里的id
    graphicsViewEx->setStyleSheet((trx("#GVE {background-color:black;}")));	// 设置id对应元素的背景色
    ui->dockWidgetDiagram->setFeatures(QDockWidget::NoDockWidgetFeatures);
    ui->dockWidgetDiagram->setWidget(graphicsViewEx);
    ui->gridLayoutAll->addWidget(ui->dockWidgetDiagram,0,1);

    createStatusBar();
}



void MainWindow::InitActions()
{
    zoomInAction->setToolTip(QString(trx("放大图像")));
    zoomInAction->setStatusTip(QString(trx("放大图像")));
    zoomOutAction->setToolTip(QString(trx("缩小图像")));
    zoomOutAction->setStatusTip(QString(trx("缩小图像")));
    zoomFitAction->setToolTip(QString(trx("图像自适应窗口")));
    zoomFitAction->setStatusTip(QString(trx("图像自适应窗口")));
}


void MainWindow::ZoomInImg()
{
    double scaleFactorX = diagramScene->backgroundRect().width()/diagramScene->sceneRect().width();
    if (scaleFactorX < 0.2){
        return;
    }
    double scaleFactor = 1.15; //How fast we zoom
    graphicsViewEx->scale(scaleFactor, scaleFactor);
    graphicsViewEx->m_totalScaleFactor *= scaleFactor;

}

void MainWindow::ZoomOutImg()
{
    double scaleFactorX = diagramScene->backgroundRect().width()/diagramScene->sceneRect().width();
    if (scaleFactorX > 5){
        return;
    }
    double scaleFactor = 1.15; //How fast we zoom
    graphicsViewEx->scale(1/scaleFactor, 1/scaleFactor);
    graphicsViewEx->m_totalScaleFactor /= scaleFactor;
}

void MainWindow::ZoomToFitImg()
{
    double scaleFactorX = diagramScene->backgroundRect().width()/diagramScene->sceneRect().width();
    double scaleFactorY = diagramScene->backgroundRect().height()/diagramScene->sceneRect().height();
    double scaleFactor = scaleFactorX < scaleFactorY ? scaleFactorX : scaleFactorY;
    if(scaleFactor <= 0) {
        return;
    }

    if(scaleFactor > 0.9 && scaleFactor < 1.1)
    {
        graphicsViewEx->scale(1/graphicsViewEx->m_totalScaleFactor, 1/graphicsViewEx->m_totalScaleFactor);
        graphicsViewEx->m_totalScaleFactor = 1;
    }
    else
    {
        graphicsViewEx->scale(scaleFactor, scaleFactor);
        graphicsViewEx->m_totalScaleFactor *= scaleFactor;
    }
}


void MainWindow::changeSceneMode(int type)
{
    QList<QAbstractButton *> buttons = buttonGroupPointer->buttons();
    foreach (QAbstractButton *button, buttons) {
        button->setChecked(buttonGroupPointer->button(type) == button);
    }
    diagramScene->setMode(DiagramScene::Mode(buttonGroupPointer->checkedId()));
}

void MainWindow::setItemEnabled(int flag, bool b)
{
    if(flag == 1)
    {
        QList<QAbstractButton *> buttons = buttonGroupPointer->buttons();
        foreach (QAbstractButton *button, buttons) {
            button->setEnabled(b);
        }
        menuDiagram->setEnabled(b);
        deleteAction->setEnabled(b);
        toFrontAction->setEnabled(b);
        sendBackAction->setEnabled(b);
        changeSceneMode(DiagramScene::MoveItem);
    }
}


void MainWindow::deleteItem()
{
    foreach (QGraphicsItem *item, diagramScene->selectedItems()) {
        if (item->type() == DiagramItem::Type) {
            qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
        }
        diagramScene->removeItem(item);
    }
}

void MainWindow::bringToFront()
{
    if (diagramScene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = diagramScene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue &&
            item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::sendToBack()
{
    if (diagramScene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = diagramScene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue &&
            item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::itemInserted(DiagramItem *item)
{
    diagramScene->setMode(DiagramScene::Mode(buttonGroupPointer->checkedId()));
// 	buttonGroupItem->button(int(item->itemType()))->setChecked(false);
}

void MainWindow::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *m_textItem =
        qgraphicsitem_cast<DiagramTextItem *>(item);
    QFont font = m_textItem->font();
    QColor color = m_textItem->defaultTextColor();
}

void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button)
{
    QList<QAbstractButton *> buttons = buttonGroupBackground->buttons();
    foreach (QAbstractButton *myButton, buttons) {
        if (myButton != button)
            button->setChecked(false);
    }

    QString text = button->text();
    if (text == trx("Blue Grid"))
        diagramScene->setBackgroundBrush(QPixmap(trx(":/background1.png")));
    else if (text == trx("White Grid"))
        diagramScene->setBackgroundBrush(QPixmap(trx(":/background2.png")));
    else if (text == trx("Gray Grid"))
        diagramScene->setBackgroundBrush(QPixmap(trx(":/background3.png")));
    else
        diagramScene->setBackgroundBrush(QPixmap(trx(":/background4.png")));
    diagramScene->update();
    graphicsViewEx->update();
}

void MainWindow::itemButtonGroupClicked(int type)
{
    QList<QAbstractButton *> buttons = buttonGroupItem->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (buttonGroupItem->button(type) != button)
            button->setChecked(false);
    }

    if (type == InsertTextButton) {
        diagramScene->setMode(DiagramScene::InsertText);
    } else {
        if (buttonGroupItem->button(type)->isChecked())
        {
            ShowSampleItem(type, 0);
        }
        else {
            diagramScene->RemoveItem(DiagramItem::DiagramType(type),0);
        }
    }
}

void MainWindow::pointerButtonGroupClicked(int)
{
    diagramScene->setMode(DiagramScene::Mode(buttonGroupPointer->checkedId()));
}



void MainWindow::InstallEventFilters()
{
    ui->dockWidgetDiagram->installEventFilter(this);
    graphicsViewEx->installEventFilter(this);
    this->installEventFilter(this);
}

void MainWindow::ConnectSlots()
{
    connect(zoomInAction, SIGNAL(triggered()),this,SLOT(ZoomInImg()));
    connect(zoomOutAction,SIGNAL(triggered()),this,SLOT(ZoomOutImg()));
    connect(zoomFitAction,SIGNAL(triggered()),this,SLOT(ZoomToFitImg()));
}


bool MainWindow::ShowSampleItem( int type, int id)
{
    QList<QPointF*> itemPos;
    switch (DiagramItem::DiagramType(type))
    {
    case DiagramItem::DT_POINT:
        itemPos.append(new QPointF(300,200));
        break;
    case DiagramItem::DT_LINE:
        itemPos.append(new QPointF(300,200));
        itemPos.append(new QPointF(500,200));
        break;
    case DiagramItem::DT_PATH:
        itemPos.append(new QPointF(300,200));
        itemPos.append(new QPointF(300,300));
        itemPos.append(new QPointF(400,300));
        break;
    case DiagramItem::DT_RECT:
        itemPos.append(new QPointF(300,300));
        itemPos.append(new QPointF(300,400));
        itemPos.append(new QPointF(400,400));
        itemPos.append(new QPointF(400,300));
        break;
    case DiagramItem::DT_TRAPEZIUM:
        itemPos.append(new QPointF(300,200));
        itemPos.append(new QPointF(260,410));
        itemPos.append(new QPointF(460,390));
        itemPos.append(new QPointF(470,230));
        break;
    case DiagramItem::DT_POLYGON:
        itemPos.append(new QPointF(300,200));
        itemPos.append(new QPointF(410,120));
        itemPos.append(new QPointF(520,200));
        itemPos.append(new QPointF(470,320));
        itemPos.append(new QPointF(350,320));
        break;
    default:
        break;
    }

    diagramScene->SetCurItemInfo(DiagramItem::DiagramType(type), id, itemPos);
    foreach(QPointF *pos, itemPos) {
        delete pos;
    }
    return true;
}


void MainWindow::createStatusBar()
{
    labelInfo1 = new QLabel((trx(" EventInfo ")));
    labelInfo1->setAlignment(Qt::AlignLeft);
    labelInfo1->setMinimumSize(labelInfo1->sizeHint());

    labelInfo2 = new QLabel((trx(" EventDetail ")));
    labelInfo2->setAlignment(Qt::AlignHCenter);
    labelInfo2->setMinimumSize(labelInfo2->sizeHint());

    statusBar()->insertWidget(1,labelInfo1,1);
    statusBar()->insertWidget(2,labelInfo2);

    //初始化
    updateStatusBar(trx("初始化状态栏"),trx("初始化状态栏"));
}


void MainWindow::updateStatusBar(const QString& eventInfo, const QString& eventDetail)
{
    labelInfo1->setText(eventInfo);
    labelInfo2->setText(eventDetail);
}


//////////////////////////////////////////////////////////////////////////


void MainWindow::closeEvent(QCloseEvent *e)
{

    updateStatusBar(trx("事件捕获"), trx("窗口关闭事件"));
}

bool MainWindow::eventFilter(QObject *taget, QEvent *event)
{
    if (taget == this && event->type() == QEvent::Close)
    {
        //此处屏蔽关闭事件不管用
    }
    else if(taget == ui->dockWidgetDiagram && event->type() == QEvent::Close)
    {
        event = new QEvent(QEvent::Hide);
    }

    return QWidget::eventFilter(taget,event);
}
