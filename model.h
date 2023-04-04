// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "moveType.h"
#include <QImage>
#include <QEvent>
#include <iostream>
#include "player.h"
#include "room.h"
#include "doorPosition.h"
#include "doorType.h"
#include <QTimer>
#include "powerup.h"
#include <QRandomGenerator>

//#############################################################################################
//                                           Model
//
// Model is the "Model" of our project. The model is responsible for storing all information
// about the game's current state, as well as using input from the View (MainWindow) to update that state.
// The model is also responsible for building the game's initial state.
// The model has methods for the View to access its members for the purpose of drawing the game.
//#############################################################################################

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    ~Model();

    std::vector<Room*> getRooms();
    std::vector<QPoint*> getVertexPoints();
    std::vector<QPoint*> getVertexPoints(QPoint center);
    std::vector<powerUp> getPowerUps();
    Player getPlayer();
    Room* getCurrentRoom();
    int numRooms();

    int getPuzzleNodes();
    int getPuzzleMinCost();
    int getPuzzleWeight(int x, int y);
    int getPuzzleCost();
    bool showPuzzle();
    bool showMinimap();
    bool isShown(int a, int b);
    void callMinimap();
    QTimer* gameTimer = new QTimer();

private:
    void connectRooms(int r1, int r2, doorPosition pos, bool isLocked);
    void addPowerupTime();
    void generatePowerUps();
    std::tuple<int,int> roomOffset(doorPosition pos);
    float calcDistFromLine(QPoint l1, QPoint l2, QPointF pos);
    doorPosition puzzleDoor;
    std::map<std::tuple<int, int>, bool> shownEdges;
    std::vector<Room*> rooms;
    std::vector<powerUp> powerups;
    Player player;
    int currentRoom;
    int puzzleCost;
    int remainingTime;
    bool isPlayingPuzzle;
    bool seeMinimap;
    int totalRooms;
    int roomsVisited = 0;
    std::vector<Door*> lockedDoors;

signals:
    void gameEndedSignal(bool isWin);
    void timeLeft(int);
    void sendProgress(QString);

public slots:
    void moveSlot(moveType move);
    void clickSlot(QPointF pos);
    void exitPuzzle();
    void startGame();
    void restartGameSlot();

private slots:
    void updateRemainingTime();
};

#endif // MODEL_H
