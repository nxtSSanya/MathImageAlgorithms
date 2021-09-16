#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QtGlobal>
#include <QDateTime>
#include <QTimer>
#include <QPaintEngine>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setFixedWidth(TILE_WIDTH);
        this->setCentralWidget(m_view = new QGraphicsView());
        m_scene = new QGraphicsScene;
        m_view->setScene(m_scene);

        QPixmap p(TILE_WIDTH, TILE_HEIGHT);
        p.fill(Qt::red);
        m_tiles.append(new QGraphicsPixmapItem(p));
        m_tiles.last()->setPos(0,0);
        m_scene->addItem(m_tiles.last());

        qsrand(QDateTime::currentMSecsSinceEpoch());

        this->startTimer(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *)
{
    // grab a new line
    QPixmap linePix = generateLine();
    int y = m_tiles.size()*TILE_HEIGHT + m_lineBuffer.size()*1;

    // append it to the line buffer
    m_lineBuffer.append(new QGraphicsPixmapItem(linePix));

    // add it to the scene
    m_scene->addItem(m_lineBuffer.last());
    m_lineBuffer.last()->setPos(0, y);

    // scroll it into view
    m_view->ensureVisible(m_lineBuffer.last());

    if(m_lineBuffer.size() >= TILE_HEIGHT)
    {
        // when the line buffer is "full"
        // or ready to be made into a tile

        // compile all the qpixmaps into a single "tile"
        static QRectF source(0,0, TILE_WIDTH, 1);
        QPixmap tile(TILE_WIDTH, TILE_HEIGHT);
        QPainter painter;
        painter.begin(&tile);
        for(int i = 0; i < m_lineBuffer.size(); i++)
        {
            painter.drawPixmap(QRectF(0, i, TILE_WIDTH, 1),
                               m_lineBuffer.at(i)->pixmap(),
                                       source);
        }
        painter.end();

        // add it into the tiles list
        m_tiles.append(new QGraphicsPixmapItem(tile));

        // add it to the scene
        m_tiles.last()->setPos(0, (m_tiles.size() - 1)*TILE_HEIGHT);
        m_scene->addItem(m_tiles.last());

        // scroll it into view
        m_view->ensureVisible(m_tiles.last());

        // Clean up the line buffer
        foreach(QGraphicsPixmapItem * pi, m_lineBuffer)
        {
            m_scene->removeItem(pi);
            delete pi;
        }
        m_lineBuffer.clear();
    }
    // busy = false;
}

QPixmap MainWindow::generateLine()
{
    // create a random pixmap of TILE_WIDTH x 1
    static int img_width = 1024;
    QImage img(img_width,1, QImage::Format_RGB16);
    for(int i = 0; i< img_width; i++)
    {
        img.setPixel(i, 0, qrand()%65536);
    }
    return QPixmap::fromImage(img);
}
