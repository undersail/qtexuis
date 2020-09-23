#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBox>
#include <QButtonGroup>
#include <QLabel>

#include <diagramscene.h>
#include <qgraphicsviewex.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    DiagramScene* diagramScene;

private:
    Ui::MainWindow *ui;

    QString appPath;
    QString appName;
    QString appVersion;
    QString appIntro;

    QGraphicsViewEx *graphicsViewEx;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *zoomFitAction;

    QMenu *menuDiagram;
    QToolBox *toolBoxDiagram;
    QToolBar *toolBarDiagram;
    QButtonGroup *buttonGroupItem;
    QButtonGroup *buttonGroupPointer;
    QButtonGroup *buttonGroupBackground;

    QAction *addAction;
    QAction *deleteAction;
    QAction *toFrontAction;
    QAction *sendBackAction;

    QLabel *labelInfo1;
    QLabel *labelInfo2;

    void createExActions();
    void createExToolBox();
    void createExToolbars();
    QWidget* createBackgroundCellWidget(const QString &text, const QString &image);
    QWidget* createItemCellWidget(const QString &text, DiagramItem::DiagramType type, unsigned int index);

    void InitWidgets();
    void InitActions();

    void InstallEventFilters();
    void ConnectSlots();
    bool ShowSampleItem( int type, int id);
    void createStatusBar();
    void updateStatusBar(const QString& eventInfo, const QString& eventDetail);
    void changeSceneMode(int type);
    void setItemEnabled(int flag, bool b);


    bool eventFilter(QObject *taget, QEvent *event);
    void closeEvent(QCloseEvent *e);


private slots:

    void ZoomInImg();
    void ZoomOutImg();
    void ZoomToFitImg();
    void backgroundButtonGroupClicked(QAbstractButton *button);
    void itemButtonGroupClicked(int type);
    void pointerButtonGroupClicked(int);
    void deleteItem();
    void bringToFront();
    void sendToBack();
    void itemInserted(DiagramItem *item);
    void itemSelected(QGraphicsItem *item);


};
#endif // MAINWINDOW_H
