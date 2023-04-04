// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#include "mainwindow.h"
#include "ui_mainwindow.h"

// Note: We didn't seperate Box2D world and the View since the Box2D
// only provides simulations for visual component locations. The user
// will not interact with the Box2D and the core game mechanisms have
// nothing to do with Box2D.
///
/// \brief MainWindow::MainWindow - constructor used to set up drawing to the UI
/// \param parent
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow) {

    ui->setupUi(this);
    world = new b2World(b2Vec2(0.0f, 1.0f));
    model = new Model();
    paintTimer = new QTimer();
    gameStarted = false;
    paintTimer->start(1000 / fps);
    setUpEndScene();

    timeLeft = model->numRooms()*10;
    ui->timeLabel->setText("Time: " + QString::number(timeLeft));
    // hide the button
    ui->startoverButton->setVisible(false);
    ui->startoverButton->setEnabled(false);
    ui->RoomsVisitedLabel->setVisible(false);

    connect(this, &MainWindow::moveSignal, model, &Model::moveSlot);
    connect(this, &MainWindow::mouseClick, model, &Model::clickSlot);
    connect(this, &MainWindow::exitPuzzle, model, &Model::exitPuzzle);
    connect(this->paintTimer, &QTimer::timeout, this, &MainWindow::paintNewFrame);
    connect(ui->actionControls, &QAction::triggered, this, &MainWindow::showControls);
    connect(ui->actionInstructions, &QAction::triggered, this, &MainWindow::showInstructions);
    connect(this, &MainWindow::startGame, model, &Model::startGame);
    connect(model->gameTimer, &QTimer::timeout, this, &MainWindow::updateTimer);
    connect(model, &Model::timeLeft, this, &MainWindow::addPowerupTime);
    connect(model, &Model::gameEndedSignal, this, &MainWindow::gameEndedSlot);
    connect(this, &MainWindow::restartGame, model, &Model::restartGameSlot);
    connect(model, &Model::sendProgress, this, &MainWindow::updateProgress);
}

MainWindow::~MainWindow() {
    delete ui;
    delete world;
    delete model;
    delete paintTimer;
}

///
/// \brief MainWindow::paintNewFrame - method called to repaint the frame
///
void MainWindow::paintNewFrame() { repaint(); }

///
/// \brief MainWindow::paintEvent - this method when triggered will then draw the game
/// \param event
///
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    drawGame(&painter);
}

///
/// \brief MainWindow::drawGame - the methdod responsible for drawing the graph, the minimap, walls, door, and room based on the current state of the
/// game
/// \param painter - painter used to draw to the UI
///
void MainWindow::drawGame(QPainter *painter) {
    if (gameEnded){
        updateWorld();
        drawEndScene(painter);
    }
    else if (model->showPuzzle()) {
        int nodeNum = model->getPuzzleNodes();
        std::vector<QPoint*> vertices = model->getVertexPoints();
        // For debug, displays a matrix of edge weights. (1, 1) is the edge from
        // node 1 to node 2, etc.
        int w = 250/nodeNum;
        for (int i = 0; i < nodeNum; i++) {
            for (int j = 0; j < nodeNum; j++) {
                painter->drawText(QPoint(35 + w * i, 35 + w * j),
                                  QString::fromStdString(std::to_string(model->getPuzzleWeight(i, j))));
            }
        }
        painter->drawText(QPoint(1000, 50), QString::fromStdString(std::to_string(model->getPuzzleCost())));
        painter->drawText(QPoint(1050, 50), QString::fromStdString(std::to_string(model->getPuzzleMinCost())));

        //DRAW LEFT GRAPH
        for (int i=0;i<nodeNum;i++) {
            painter->drawEllipse(*vertices.at(i), 10, 10);
        }
        // Since the graph is un-directional, only draw half of the edge matrix,
        // excluding the diagonal. E.g., if a matrix is 3 * 3, then we only need to
        // look at (2, 1), (3, 1), (3, 2). Draw edges
        for (int y = 0; y < nodeNum - 1; y++) {
            for (int x = 1; x < nodeNum; x++) {
                int edgeWeight = model->getPuzzleWeight(x, y);
                if (edgeWeight != 0) {
                    painter->drawLine(*vertices.at(x), *vertices.at(y));
                    int lineXMid = (vertices.at(x)->x() + vertices.at(y)->x()) / 2;
                    int lineYMid = (vertices.at(x)->y() + vertices.at(y)->y()) / 2;
                    // Need to scramble random generator better so that text doesn't
                    // overlap
                    QPoint textPos = QPoint(lineXMid - 14, lineYMid + 7);
                    painter->drawText(textPos, QString::fromStdString(std::to_string(edgeWeight)));
                }
            }
        }

        vertices = model->getVertexPoints(QPoint(875, 550));
        //DRAW RIGHT GRAPH
        for (int i=0;i<nodeNum;i++) {
            painter->drawEllipse(*vertices.at(i), 10, 10);
        }
        // Since the graph is un-directional, only draw half of the edge matrix,
        // excluding the diagonal. E.g., if a matrix is 3 * 3, then we only need to
        // look at (2, 1), (3, 1), (3, 2). Draw edges
        for (int y = 0; y < nodeNum - 1; y++) {
            for (int x = 1; x < nodeNum; x++) {
                int edgeWeight = model->getPuzzleWeight(x, y);
                if (edgeWeight != 0 && model->isShown(x, y)) {
                    painter->drawLine(*vertices.at(x), *vertices.at(y));
                    int lineXMid = (vertices.at(x)->x() + vertices.at(y)->x()) / 2;
                    int lineYMid = (vertices.at(x)->y() + vertices.at(y)->y()) / 2;
                    // Need to scramble random generator better so that text doesn't
                    // overlap
                    QPoint textPos = QPoint(lineXMid - 14, lineYMid + 7);
                    painter->drawText(textPos, QString::fromStdString(std::to_string(edgeWeight)));
                }
            }
        }
    }
    //##############################################################
    //                   DRAW MINIMAP
    //##############################################################
    else if (model->showMinimap()) {
        std::vector<Room *> allRooms = model->getRooms();
        for (auto currRoom : allRooms) {
            if(currRoom->visited){
                std::tuple<QRect, QImage> drawRoom = miniMapHelper(currRoom);
                //paint the minimap
                painter->drawImage(std::get<0>(drawRoom), std::get<1>(drawRoom));
            }
        }
        ui->RoomsVisitedLabel->setVisible(true);
    }
    else {
        Room *currentRoom = model->getCurrentRoom();
        currentRoom->visited = true;

        painter->drawImage(currentRoom->getRect(), currentRoom->getImage());
        QImage pImage = model->getPlayer().getImage();
        painter->drawImage(model->getPlayer().getRect(), pImage);

        std::vector<doorType> doors = currentRoom->getDoorsToDraw();
        //##############################################################
        //                          DRAW WALLS
        //##############################################################

        for (Wall* w : currentRoom->getWalls()) {
            painter->drawImage(w->getRect(), w->getImage());
        }

        //##############################################################
        //                          DRAW DOORS
        //##############################################################

        int roomWidth = 13;
        int roomHeight = 9;
        int x = 40 + 80 * (roomWidth / 2);
        int y = 40;
        QImage rotated = Door::getImage(doors[1]).transformed(QTransform().rotate(0.0).scale(2, 2));
        QRect rect = rotated.rect();
        rect.moveTo(x, y);
        painter->drawImage(rect, rotated);

        x = 40 + 80 * (roomWidth / 2);
        y = 80 + 80 * (roomHeight);
        rotated = Door::getImage(doors[3]).transformed(QTransform().rotate(180.0).scale(2, 2));
        rect = rotated.rect();
        rect.moveTo(x, y);
        painter->drawImage(rect, rotated);

        x = 40;
        y = 40 + 80 * (roomHeight / 2);
        rotated = Door::getImage(doors[0]).transformed(QTransform().rotate(-90.0).scale(2, 2));
        rect = rotated.rect();
        rect.moveTo(x, y);
        painter->drawImage(rect, rotated);

        x = 80 + 80 * (roomWidth);
        y = 40 + 80 * (roomHeight / 2);
        rotated = Door::getImage(doors[2]).transformed(QTransform().rotate(90.0).scale(2, 2));
        rect = rotated.rect();
        rect.moveTo(x, y);
        painter->drawImage(rect, rotated);

        std::vector<powerUp> powerUps = model->getPowerUps();

        for(auto p: powerUps){
            if(p.roomID == currentRoom->getID() && (!p.died)){
                painter->drawImage(p.getRect(), p.getImage());
            }
        }
    }
    if(!model->showMinimap()){
        ui->RoomsVisitedLabel->setVisible(false);
    }
}

/**
 * @brief MainWindow::miniMapHelper Calculates the offset for the given room and returns
 *  the QRect and QImage with the proper dimensions
 * @param drawRoom - this is the current room the player is in
 * @return - returns a Tuple<QRect, QImage> used to draw the minimap room
 */
std::tuple<QRect, QImage> MainWindow::miniMapHelper(Room *drawRoom) {
    Room* currentRoom = model->getCurrentRoom();
    QRect currRoomRect = drawRoom->getRect();
    QImage currRoomImage = QImage(80, 80, QImage::Format_ARGB32);
    currRoomImage.fill(drawRoom->getImage().pixelColor(400, 360));
    currRoomRect.setSize(QSize(currRoomImage.width(), currRoomImage.height()));
    currRoomRect.setX(520 + drawRoom->xOffset * 80);
    currRoomRect.setY(360 + drawRoom->yOffset * 80);

    // is a door locked or open
    // color associated with locked or open
    // 4 different loops to draw based on what door
    // j, i is left side
    // i, j is top
    // 80-1-j, i right
    // i, 80-1-j bottom
    //##############################################################
    //                   DRAW DOORS FOR MINIMAP
    //##############################################################
    std::vector<Door*> allDoors = drawRoom->getAllDoors();

    for(auto door : allDoors){
        for (int i = 30; i < 50; i++) { // length
            for (int j = 0; j < 5; j++) { // depth
                switch(door->position){
                case LeftDoor:{
                    if(door->locked && drawRoom->getID() == door->parentRoom)
                        currRoomImage.setPixel(j, i, qRgb(0, 0, 0));
                    else if(door->visible && drawRoom->getID() == door->parentRoom)
                        currRoomImage.setPixel(j, i, qRgb(165, 42, 42));
                    break;
                }
                case RightDoor:{
                    if(door->locked && door->visible && drawRoom->getID() == door->parentRoom)
                        currRoomImage.setPixel(80-1-j, i, qRgb(0, 0, 0));
                    else if(door->visible && drawRoom->getID() == door->parentRoom)
                        currRoomImage.setPixel(80-1-j, i, qRgb(165, 42, 42));
                    break;
                }
                case BottomDoor:{
                    if(door->locked && door->visible && drawRoom->getID() == door->parentRoom)
                        currRoomImage.setPixel(i, 80-1-j, qRgb(0, 0, 0));
                    else if(door->visible && drawRoom->getID() == door->parentRoom)
                        currRoomImage.setPixel(i, 80-1-j, qRgb(165, 42, 42));
                    break;
                }
                case TopDoor:{
                    if(door->locked && door->visible && drawRoom->getID() == door->parentRoom)
                        currRoomImage.setPixel(i, j, qRgb(0, 0, 0));
                    else if(door->visible && drawRoom->getID() == door->parentRoom)
                        currRoomImage.setPixel(i, j, qRgb(165, 42, 42));
                    break;
                }
                case None:
                    break;
                }
            }
        }
    }
    //drawing the player in the current room
    if(currentRoom->getID() == drawRoom->getID()){
        for(int i = 35; i < 45; i++){
            for(int j = 35; j < 45; j++){
                currRoomImage.setPixel(i,j, qRgb(255,0,0));
            }
        }
    }
    return std::tuple<QRect, QImage>(currRoomRect, currRoomImage);
}

///
/// \brief MainWindow::drawEndScene - This method is called to draw the end game scene.
/// \param painter
///
void MainWindow::drawEndScene(QPainter *painter){
    ui->startoverButton->setVisible(true);
    ui->startoverButton->setEnabled(true);
    painter->save();
    painter->setFont(QFont("Arial", 100, QFont::Bold));
    if (gameWin){
        for (int i = 0; i < 4; i++){
            painter->drawText(*charCenters.at(i), winText[i]);
        }
    } else {
        for (int i = 0; i < 4; i++){
            painter->drawText(*charCenters.at(i), loseText[i]);
        }
    }
    painter->restore();
}

///
/// \brief MainWindow::mousePressEvent - triggered when the player clicks on the form. Used mainly for the puzzle playing
/// \param eventPress
///
void MainWindow::mousePressEvent(QMouseEvent *eventPress) {
    QPointF pos = eventPress->position();
    if (model->showPuzzle()) {
        emit mouseClick(pos);
    }
}

///
/// \brief MainWindow::keyPressEvent - this is an event handler that looks for a WSAD, arrow key, escape, or M press and handles each approppriately
/// \param event
///
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(!gameStarted){
        emit startGame();
        gameStarted = true;
    }

    if (model->showPuzzle()) {
        if (event->key() == Qt::Key_Escape) {
            emit exitPuzzle();
        }
        return;
    }
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        emit moveSignal(Left);
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        emit moveSignal(Right);
    } else if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
        emit moveSignal(Forward);
    } else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
        emit moveSignal(Backward);
    } else if (event->key() == Qt::Key_M) {
        model->callMinimap();
        emit mapSignal();
    }
}

///
/// \brief MainWindow::showControls - shows a message box with directions for controls
///
void MainWindow::showControls(){
    QMessageBox msgBox;
    msgBox.setText("WASD to move (or arrow keys) \nM toggles minimap \nescape closes the puzzle \n");
    msgBox.exec();
}

///
/// \brief MainWindow::showInstructions - shows a message box with directions for how to play the game
///
void MainWindow::showInstructions(){
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("Explore every room to win the game! To unlock doors solve the minimal spanning tree!\n"
                   "To solve the puzzle, click between the nodes and a line will appear if you are close enough to a direct "
                   "line between the nodes. There are two numbers displayed in the top right. The rightmost is the minimum weight that "
                   "you are trying to achieve, the left is the weight you are currently at.\n"
                   "<a href='https://en.wikipedia.org/wiki/Prim%27s_algorithm'>Learn how to find a minimal spanning tree</a>");
    msgBox.exec();
}

///
/// \brief MainWindow::updateTimer - updates the timer on the UI
///
void MainWindow::updateTimer(){
    timeLeft--;
    QString s = QString::number(timeLeft);
    ui->timeLabel->setText("Time: " + s );
}

///
/// \brief MainWindow::addPowerupTime - adds time to the timer on the UI when a powerup is picked up
/// \param time
///
void MainWindow::addPowerupTime(int time){
    timeLeft = time;
    QString s = QString::number(timeLeft);
    ui->timeLabel->setText("Time: " + s );
}

// Since the Box2D is only animating the end game scene, it is not part of the model.
// The user will cannont interact with the physics.
///
/// \brief MainWindow::updateWorld - Box2D eplosion when the player wins or loses
///
void MainWindow::updateWorld() {
    // Trigger explosion after 2.5 seconds.
    if (elapsedFrames == 150){
        explosion();
    }
    world->Step(1.0/60.0, 6, 3);
    // std::cout << std::to_string(position.x) << std::endl;
    for (int i = 0; i < 4; i++){
        b2Vec2 position = bodies[i]->GetPosition();
        charCenters[i]->setX(100 * position.x);
        charCenters[i]->setY(100 * position.y);
    }
    elapsedFrames++;
}

///
/// \brief MainWindow::setUpEndScene - This method does all necessary setups for the end game scene.
///
void MainWindow::setUpEndScene() {
    for (int i = 0; i < 4; i++){
        QPoint* temp = new QPoint(400 + 150 * i, 300);
        charCenters.push_back(temp);
    }
    winText = QString("WON!");
    loseText = QString("BRUH");

    // Set up the ground
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(0.0f, 10.5f);
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 1.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Set up the left boundary
    b2BodyDef leftBoundaryDef;
    leftBoundaryDef.type = b2_staticBody;
    leftBoundaryDef.position.Set(-1.5f, 5.5f);
    b2Body* leftBoundaryBody = world->CreateBody(&leftBoundaryDef);
    b2PolygonShape leftBoundaryBodyBox;
    leftBoundaryBodyBox.SetAsBox(1.0f, 50.0f);
    leftBoundaryBody->CreateFixture(&leftBoundaryBodyBox, 0.0f);

    // Set up the right boundary
    b2BodyDef rightBoundaryDef;
    rightBoundaryDef.type = b2_staticBody;
    rightBoundaryDef.position.Set(12.0f, 5.5f);
    b2Body* rightBoundaryBody = world->CreateBody(&rightBoundaryDef);
    b2PolygonShape rightBoundaryBodyBox;
    rightBoundaryBodyBox.SetAsBox(1.0f, 50.0f);
    rightBoundaryBody->CreateFixture(&rightBoundaryBodyBox, 0.0f);

    // Set up the top boundary
    b2BodyDef topBoundaryDef;
    topBoundaryDef.type = b2_staticBody;
    topBoundaryDef.position.Set(0.0f, 0.0f);
    b2Body* topBoundaryBody = world->CreateBody(&topBoundaryDef);
    b2PolygonShape topBoundaryBodyBox;
    topBoundaryBodyBox.SetAsBox(50.0f, 1.0f);
    topBoundaryBody->CreateFixture(&topBoundaryBodyBox, 0.0f);

    // Set up the bodyies for each char.
    for (int i = 0; i < 4; i++){
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(4.0f + 1.5 * i, 3.0f);
        b2Body* body = world->CreateBody(&bodyDef);

        b2CircleShape dynamicCircle;
        dynamicCircle.m_p.Set(0, 0);
        dynamicCircle.m_radius = 0.6;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicCircle;
        fixtureDef.density = 1.0f;

        fixtureDef.friction = 0.3f;
        fixtureDef.restitution = 0.9;
        body->CreateFixture(&fixtureDef);
        bodies.push_back(body);
    }
}

///
/// \brief MainWindow::explosion - This method simulates an explosion, plz refer to Box2D website for more details.
///
void MainWindow::explosion(){
    float blastPower = 150.0f;
    int numRays = 60;
    b2Vec2 center = b2Vec2(6.0f, 7.0f);
    float DEGTORAD = 0.0174f;
    for (int i = 0; i < numRays; i++) {
        float angle = (i / (float)numRays) * 360 * DEGTORAD;
        b2Vec2 rayDir( sinf(angle), cosf(angle) );

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.fixedRotation = true; // rotation not necessary
        bd.bullet = true; // prevent tunneling at high speed
        bd.linearDamping = 10; // drag due to moving through air
        bd.gravityScale = 0; // ignore gravity
        bd.position = center; // start at blast center
        bd.linearVelocity = blastPower * rayDir;
        b2Body* body = world->CreateBody( &bd );

        b2CircleShape circleShape;
        circleShape.m_radius = 0.05; // very small

        b2FixtureDef fd;
        fd.shape = &circleShape;
        fd.density = 150 / (float)numRays; // very high - shared across all particles
        fd.friction = 0; // friction not necessary
        fd.restitution = 0.99f; // high restitution to reflect off obstacles
        fd.filter.groupIndex = -1; // particles should not collide with each other
        body->CreateFixture( &fd );
    }
}

///
/// \brief MainWindow::cleanUp - This method clean up the end scene elements so that the end scene is new for the next play.
///
void MainWindow::cleanUp(){
    delete world;
    charCenters.clear();
    bodies.clear();
    elapsedFrames = 0;
    gameWin = false;
    gameEnded = false;
    gameStarted = false;
    ui->startoverButton->setVisible(false);
    ui->startoverButton->setEnabled(false);
}

///
/// \brief MainWindow::gameEndedSlot - slot triggered when the game is won or lost
/// \param isWin - sets the isWin variable based on if the player lost or won (false or true)
///
void MainWindow::gameEndedSlot(bool isWin){
    gameEnded = true;
    gameWin = isWin;
}

///
/// \brief MainWindow::on_startoverButton_clicked - resets the game to its initial state
///
void MainWindow::on_startoverButton_clicked()
{
    cleanUp();
    // Set up the new game.
    timeLeft= 150;
    ui->timeLabel->setText("Time: " + QString::number(timeLeft));
    paintTimer->start(1000 / fps);
    world = new b2World(b2Vec2(0.0f, 1.0f));
    setUpEndScene();
    emit restartGame();
}

///
/// \brief MainWindow::updateProgress - this updates the progress ratio on the minimap
///
void MainWindow::updateProgress(QString prog){
    progress = prog;
    ui->RoomsVisitedLabel->setText(progress);
}

