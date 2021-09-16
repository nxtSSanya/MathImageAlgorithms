#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimerEvent>
#include <QEvent>

#define TILE_HEIGHT 128
#define TILE_WIDTH 1024

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPixmap generateLine();
public slots:
    void timerEvent(QTimerEvent *);

private:
    Ui::MainWindow *ui;
    QGraphicsView * m_view;
    QGraphicsScene * m_scene;
    QVector <QGraphicsPixmapItem *> m_tiles;
    QVector <QGraphicsPixmapItem *> m_lineBuffer;
};
#endif // MAINWINDOW_H
