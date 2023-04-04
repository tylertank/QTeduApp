// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"
#include "moveType.h"
#include <QEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include "doorType.h"
#include "door.h"
#include <qmessagebox.h>
#include "Box2D/Box2D.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//########################################################################################
//                            MainWindow
//
// The MainWindow class is the "View" of our project. It is responsible for drawing the game
// and passing user input to the Model.
//########################################################################################

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    std::tuple<QRect, QImage> miniMapHelper(Room*);
    void drawGame(QPainter *painter);
    void updatePlayer(Player player);
    void showInstructions();
    void showControls();
    void setUpEndScene();
    void initializeWorld();
    void drawEndScene(QPainter *painter);
    void explosion();
    void cleanUp();

    Ui::MainWindow *ui;
    b2World* world;
    std::vector<b2Body*> bodies;
    std::vector<QPoint*> charCenters;
    Model* model;
    QTimer* paintTimer;
    QString winText;
    QString loseText;
    bool gameStarted;
    bool gameWin;
    bool gameEnded = false;
    static const int fps=60;
    int elapsedFrames = 0;
    int timeLeft;
    QString progress = "";

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent*);
    void paintEvent(QPaintEvent *event);

signals:
    void moveSignal(moveType move);
    void mouseClick(QPointF pos);
    void exitPuzzle();
    void mapSignal();
    void startGame();
    void restartGame();

private slots:
    void paintNewFrame();
    void updateTimer();
    void updateWorld();
    void addPowerupTime(int);
    void on_startoverButton_clicked();
    void updateProgress(QString);

public slots:
    void gameEndedSlot(bool isWin);
};
#endif // MAINWINDOW_H
