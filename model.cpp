// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#include "model.h"
using namespace std;

///
/// \brief Model::Model - this is the constructor that does all the needed setup for the game
/// \param parent
///
Model::Model(QObject *parent)
    : QObject{parent}
{

    currentRoom = 0;

    connectRooms(0, 1, RightDoor, false);
    rooms.at(0)->xOffset =0;
    rooms.at(0)->yOffset =0;

    connectRooms(0, 2, TopDoor, false);
    connectRooms(2, 3, RightDoor, true);
    connectRooms(1, 4, RightDoor, true);
    connectRooms(0, 5, BottomDoor, true);
    connectRooms(0, 6, LeftDoor, false);
    connectRooms(6, 7, TopDoor, true);
    connectRooms(7, 8, TopDoor, false);
    connectRooms(8, 9, RightDoor, true);
    connectRooms(9, 10, RightDoor, true);
    connectRooms(3, 11, RightDoor, true);
    connectRooms(11, 12, TopDoor, false);
    connectRooms(4, 13, BottomDoor, false);
    connectRooms(13, 14, LeftDoor, true);

    totalRooms = rooms.size();
    remainingTime = totalRooms*10;
    isPlayingPuzzle = false;
    seeMinimap = false;
    puzzleDoor=None;
    puzzleCost=0;
    generatePowerUps();
    connect(this->gameTimer, &QTimer::timeout, this, &Model::updateRemainingTime);
}

Model::~Model(){
    delete gameTimer;
}

///
/// \brief Model::connectRooms - this method creates new rooms
/// \param parentID - id of the parent room
/// \param childID - id of the child room
/// \param pos - door through which the room can be accessed
/// \param isLocked - bool of if the room is locked or not
///
void Model::connectRooms(int parentID, int childID, doorPosition pos, bool isLocked) {
    if ((int) rooms.size() <= parentID) {
        rooms.push_back(+new Room(parentID));
    }
    if ((int) rooms.size() <= childID) {
        rooms.push_back(new Room(childID));
    }
    std::tuple<int,int> xyOffset = roomOffset(pos);
    //setting the offset for the minimap.

    rooms.at(childID)->xOffset = rooms.at(parentID)->xOffset+std::get<0>(xyOffset);
    rooms.at(childID)->yOffset = rooms.at(parentID)->yOffset+std::get<1>(xyOffset);

    //adding doors to world.
    rooms.at(parentID)->addChildDoor(childID, pos, isLocked);
    rooms.at(childID)->addParentDoor(parentID, pos);

    //adding doors to lockedDoor vector so that they can be locked in the restarted game.
    if (isLocked){
       lockedDoors.push_back(rooms.at(parentID)->getLastAddedDoor());
    }
}

///
/// \brief Model::roomOffset - calculates an offset for the rooms when drawing the minimap. This is done when the player moves into new rooms
/// \param pos - this is the door through which the player entered, giving direction for where the room offset should be
/// \return - this is the offset of the room from the center of the minimap
///
std::tuple<int,int> Model::roomOffset(doorPosition pos){
    int x=0;
    int y =0;
    switch(pos){
    case LeftDoor:{
        x=-1;
        break;   //draw on the left wall
    }
    case RightDoor:{
        x=1;
        break;
    }
    case BottomDoor:{
        y=1;
        break;
    }
    case TopDoor:{
        y=-1;
        break;
    }
    case None:
        break;
    }
    return std::tuple<int,int>(x,y);
}

int Model::numRooms() {
    return rooms.size();
}

bool Model::showMinimap(){
    return seeMinimap;
}

///
/// \brief Model::callMinimap - this method triggers the drawing or disappearing of the minimap
///
void Model::callMinimap(){
    if (seeMinimap){
        seeMinimap = false;
    }
    else{
    int tempVisit = 0;
        for(auto r : rooms){
            if(r->visited){
            tempVisit++;
            }
        }
        roomsVisited = tempVisit;
        seeMinimap = true;
        QString progress = "Rooms Visited: " + QString::number(roomsVisited) + " / " + QString::number(numRooms());
        emit sendProgress(progress);
    }
}

///
/// \brief Model::moveSlot - this method is responsible for moving the player between rooms and for picking up powerups
/// \param move - this is the kind of move the player made
///
void Model::moveSlot(moveType move){
    doorPosition pos = player.move(move);
    int nextRoom = getCurrentRoom()->getAdjRoomID(pos);
    if (nextRoom != -1) {
        if (getCurrentRoom()->isLocked(pos)) {
            isPlayingPuzzle = true;
            puzzleDoor = pos;
            shownEdges.clear();
            int nodeNum = getPuzzleNodes();
            for (int y = 0; y < nodeNum - 1; y++) {
                for (int x = 1; x < nodeNum; x++) {
                    int edgeWeight = getPuzzleWeight(x, y);
                    if (edgeWeight != 0) {
                        shownEdges[std::tuple<int, int>(x, y)] = false;
                    }
                }
            }
            puzzleCost = 0;
        }
        else {
            if(!rooms[nextRoom]->visited){
                roomsVisited++;
            }
            currentRoom = nextRoom;
            player.enterNewRoom(move);
        }
    }
    std::tuple<int,int> playerpos = player.getPlayerPos();
    std::vector<powerUp> powerCopy = powerups;
    for(auto pUP : powerCopy){
        int x = std::get<0>(playerpos);
        int y = std::get<1>(playerpos);
        if(((pUP.x-1 == x && pUP.y-1 == y ) && pUP.roomID == currentRoom) && !pUP.died){
            powerups[pUP.pID].died = true;
            addPowerupTime();
        }
    }
}

///
/// \brief Model::addPowerupTime - this adds the time to the clock when a powerup is picked up
///
void Model::addPowerupTime(){
    remainingTime = remainingTime + 20;
    emit timeLeft(remainingTime);
}

///
/// \brief Model::generatePowerUps - this generates random powerups throughout the game rooms
///
void Model::generatePowerUps(){
    QRandomGenerator gen;
    for(int i = 0; i < floor(2*totalRooms/3); i++){
        int x = gen.generate() % 13 ;
        int y = gen.generate() % 9 ;
        int roomID = gen.generate() % totalRooms;

        powerUp newP(i, roomID, x,y, false, true);
        powerups.push_back(newP);
    }
}

std::vector<powerUp> Model::getPowerUps(){
    return powerups;
}

///
/// \brief Model::startGame - start the game timer
///
void Model::startGame(){
    gameTimer->start(1000);
}

///
/// \brief Model::updateRemainingTime - this method updates the time for the game
///
void Model::updateRemainingTime(){
    remainingTime--;
    // Since the player used up all the time, the player loses the game.
    if (remainingTime == 0){
        emit gameEndedSignal(false);
        gameTimer->stop();
    }

    if(roomsVisited == rooms.size()){
        emit gameEndedSignal(true);
    }

}

///
/// \brief Model::clickSlot - this method is used to draw the puzzle when the player clicks between nodes
/// \param pos - where the player clicked
///
void Model::clickSlot(QPointF pos) {    
    if (showPuzzle()) {
        int minDist = 20000;
        int minX = 0;
        int minY = 0;
        QPoint center = QPoint(875, 550);
        int nodeNum = getPuzzleNodes();
        vector<QPoint*> vertices = getVertexPoints(center);
        for (int y = 0; y < nodeNum - 1; y++) {
            for (int x = 1; x < nodeNum; x++) {
                int edgeWeight = getPuzzleWeight(x, y);
                if (edgeWeight != 0) {
                    float dist = calcDistFromLine(*vertices.at(x), *vertices.at(y), pos);
                    if  (dist < minDist) {
                        minDist = dist;
                        minX = x;
                        minY = y;
                    }
                }
            }
        }
        if (minDist < 15) {
            shownEdges[std::tuple<int, int>(minX, minY)] = !shownEdges[std::tuple<int, int>(minX, minY)];
            if (shownEdges[std::tuple<int, int>(minX, minY)]) {
                puzzleCost += getPuzzleWeight(minX, minY);
            } else {
                puzzleCost -= getPuzzleWeight(minX, minY);
            }
            if (puzzleCost == getPuzzleMinCost()) {
                isPlayingPuzzle = false;
                getCurrentRoom()->unlock(puzzleDoor);
            }
        }
    }
}

///
/// \brief Model::calcDistFromLine - this method is used to detect clicks between nodes to see if the line is trying to be drawn
/// \param l1 - this is node one
/// \param l2 - this is node two
/// \param pos - this is where the player clicked
/// \return - this is how far from the line between nodes that the player clicked
///
float Model::calcDistFromLine(QPoint l1, QPoint l2, QPointF pos) {
    float numer = abs((l2.x()-l1.x())*(l1.y()-pos.y())-((l1.x()-pos.x())*(l2.y()-l1.y())));
    float denom = sqrt(pow(l2.x()-l1.x(), 2)+pow(l2.y()-l1.y(), 2));
    return numer/denom;
}

///
/// \brief Model::isShown - a method used to return if the edge should be shown or not on the graph
/// \param a - the first node
/// \param b - the second node
/// \return - the returns a map where the edge is the key and bool representing if it should be shown is the value
///
bool Model::isShown(int a, int b) {
    return shownEdges[std::tuple<int, int>(a, b)];
}

///
/// \brief Model::getVertexPoints Return the locations of vertices so that the view knows where to draw. The center is 350, 550.
/// \return
///
vector<QPoint*> Model::getVertexPoints() {
    // constants
    QPoint center = QPoint(350, 550);
    return getVertexPoints(center);
}

///
/// \brief Model::getVertexPoints Return the locations of vertices based on the given center so that the view knows where to draw.
/// \param center - this
/// \return
///
vector<QPoint*> Model::getVertexPoints(QPoint center) {
    // constants
    double radius = 225;
    vector<QPoint*> returned;
    int n = getPuzzleNodes();
    for (int vertex=0;vertex<n;vertex++)  {
        double angle = vertex*(2*3.14159265358/getPuzzleNodes());
        returned.push_back(new QPoint(center.x()+(sin(angle)*radius), (center.y() + cos(angle)*radius)));
    }
    return returned;
}

///
/// \brief Model::getPuzzleCost - returns the current puzzle cost so that the player knows what's their current puzzle cost.
/// \return
///
int Model::getPuzzleCost() {
    return puzzleCost;
}
int Model::getPuzzleMinCost() {
    return getCurrentRoom()->getPuzzleMinValue(puzzleDoor);
}

///
/// \brief Model::showPuzzle - this is a method used to know if the puzzle is being played and should be drawn
/// \return - true for the puzzle is being played, false is not
///
bool Model::showPuzzle() {
    return isPlayingPuzzle;
}

int Model::getPuzzleNodes() {
    return getCurrentRoom()->getPuzzleSize(puzzleDoor);
}

///
/// \brief Model::getPuzzleWeight - this is will get the weight of the edge between the x, y parameters
/// \param x - this is the first node
/// \param y - this is the second node
/// \return - the weight of the edge
///
int Model::getPuzzleWeight(int x, int y) {
    return getCurrentRoom()->getNodeWeight(puzzleDoor, x, y);
}

void Model::exitPuzzle() {
    isPlayingPuzzle = false;
}

std::vector<Room*> Model::getRooms() {
    return rooms;
}

Player Model::getPlayer() {
    return player;
}

Room* Model::getCurrentRoom() {
    return rooms.at(currentRoom);
}

///
/// \brief Model::restartGameSlot - this restarts the game doing all the necessary game setup again. Note: the puzzles and the rooms will not be re-generate.
///
void Model::restartGameSlot(){
    // Clean
    remainingTime = 150;
    roomsVisited= 0;
    currentRoom =0;
    isPlayingPuzzle = false;
    seeMinimap = false;
    puzzleDoor=None;
    puzzleCost=0;
    while (powerups.size() != 0){
        powerups.pop_back();
    }
    for(auto r : rooms){
        r->visited = false;
    }
    // Start fresh
    gameTimer->start(1000);
    player.resetPlayerPos();
    generatePowerUps();
    for(auto d : lockedDoors){
        d->locked = true;
    }
}
