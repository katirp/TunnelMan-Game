#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

Coordinate::Coordinate(int x, int y){
    xPos = x;
    yPos = y;
}

int Coordinate::getX() {
    return xPos;
}

int Coordinate::getY() {
    return yPos;
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir) {}

StudentWorld::~StudentWorld(){}


int StudentWorld::init(){
    // create oil field
    // left side
    for (int i = 0; i < 30; i++){
        for (int j = 0; j < 60; j++){
            m_earth[i][j] = new Earth(this,i,j);
        }
    }
    // right side
    for (int i = 34; i < 64; i++){
        for (int j = 0; j < 60; j++){
            m_earth[i][j] = new Earth(this,i,j);
        }
    }
    // bottom bit below tunnel
    for (int i = 30; i < 34; i++){
        for (int j = 0; j < 4; j++){
            m_earth[i][j] = new Earth(this,i,j);
        }
    }
    // set top empty area to nullptr
    for (int i = 0; i < 64; i++){
        for (int j = 60; j < 64; j++){
            m_earth[i][j] = nullptr;
        }
    }
    // set the initial tunnel to nullptr
    for (int i = 30; i < 34; i++){
        for (int j = 4; j < 60; j++){
            m_earth[i][j] = nullptr;
        }
    } // end creating oil field
    
    // create TunnelMan
    m_TunnelMan = new TunnelMan(this);
    
    // number of boulders to create
    int level = getLevel();
    int num_Boulders = min(level/2 + 2, 9);
    //populate oil field with boulders
    for (int i = 0; i < num_Boulders; i++){
        // pick a random pos on left side of tunnel
        int x1 = rand() % 25 + 1;
        // pick a random pos on right side of tunnel
        int x2 = rand() % 25 + 35;
        int pickone = rand() % 2;
        int x;
        // pick x1 or x2 randomly
        if (pickone == 1)
            x = x1;
        else
            x = x2;
        int y = rand() % 36 + 20;
        while (withinDistance(x, y, 6) == true){
            x1 = rand() % 25 + 1;
            x2 = rand() % 25 + 35;
            pickone = rand() % 2;
            if (pickone == 1)
                x = x1;
            else
                x = x2;
            y = rand() % 36 + 20;
        }
        Boulder* add_Boulder = new Boulder(this, x, y);
        actors.push_back(add_Boulder);
        // delete the earth underneath the boulder
        for (int j = 0; j < 4; j++){
            for (int k = 0; k < 4; k++){
                delete m_earth[x+j][y+k];
                m_earth[x+j][y+k] = nullptr;
            }
        }
    }
    //populate oil field with barrels
    int bleh = (2 + getLevel());
    m_barrels = min(bleh, 21);
    for (int i = 0; i < m_barrels; i++){
        // pick a random pos on left side of tunnel
        int x1 = rand() % 25 + 1;
        // pick a random pos on right side of tunnel
        int x2 = rand() % 25 + 35;
        int pickone = rand() % 2;
        int x;
        // pick x1 or x2 randomly
        if (pickone == 1)
            x = x1;
        else
            x = x2;
        int y = rand() % 36 + 20;
        while (withinDistance(x, y, 6) == true){
            x1 = rand() % 25 + 1;
            x2 = rand() % 25 + 35;
            pickone = rand() % 2;
            if (pickone == 1)
                x = x1;
            else
                x = x2;
            y = rand() % 56;
        }
        Barrel* add_barrel = new Barrel(this, x, y);
        actors.push_back(add_barrel);
    }
    
    int G = 5 - getLevel()/2;
    int nuggetsToAdd = max(G, 2);
    for (int i = 0; i < nuggetsToAdd; i++){
        // pick a random pos on left side of tunnel
        int x1 = rand() % 25 + 1;
        // pick a random pos on right side of tunnel
        int x2 = rand() % 25 + 35;
        int pickone = rand() % 2;
        int x;
        // pick x1 or x2 randomly
        if (pickone == 1)
            x = x1;
        else
            x = x2;
        int y = rand() % 36 + 20;
        while (withinDistance(x, y, 6) == true){
            x1 = rand() % 25 + 1;
            x2 = rand() % 25 + 35;
            pickone = rand() % 2;
            if (pickone == 1)
                x = x1;
            else
                x = x2;
            y = rand() % 56;
        }
        GoldNugget* add_nugget = new GoldNugget(this, x, y, false, permanent);
        actors.push_back(add_nugget);
    }
    m_ticksSinceLastProtester = 0;
    int h = getLevel() * 10 + 30;
    int probabilityOfHardcore = min(90, h);
    int prot = rand() % probabilityOfHardcore;
    if (prot == 1){
        actors.push_back(new HardcoreProtestor(this));
    } else {
        actors.push_back(new RegularProtestor(this));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    //call doSomething on all objects
    m_TunnelMan->doSomething();
    for (int i = 0; i < actors.size(); i++){
        actors[i]->doSomething();
    }
    //add new sonar or water pool
    int addGoodie = rand() % (getLevel() * 25 + 300);
    if (addGoodie == 1){
        int SonarOrWater = rand() % 5;
        if (SonarOrWater == 1) {actors.push_back(new SonarKit(this, 0, 60));}
        else {
            int x = rand() % 60;
            int y = rand() % 56;
            while (isEarthAt(x, y)){
                x = rand() % 60;
                y = rand() % 56;
            }
            actors.push_back(new WaterPool(this, x, y));
        }
    }
    //add new protestor
    int tt = 200 - getLevel();
    int T = max(25, tt);
    int pp = 2 + getLevel() * 1.5;
    int targetProtesters = min(15, pp);
    int numProtesters = 0;
    for (int i = 0; i < actors.size(); i++){
        if (actors[i]->getID() == TID_PROTESTER || actors[i]->getID() == TID_HARD_CORE_PROTESTER){
            numProtesters++;
        }
    }
    if (m_ticksSinceLastProtester >= T && numProtesters < targetProtesters){
        int h = getLevel() * 10 + 30;
        int probabilityOfHardcore = min(90, h);
        int prot = rand() % probabilityOfHardcore;
        if (prot == 1){
            actors.push_back(new HardcoreProtestor(this));
        } else {
            actors.push_back(new RegularProtestor(this));
        }
        m_ticksSinceLastProtester = 0;
    }
    
    
    //remove dead objects
    for (int i = 0; i < actors.size(); i++){
        if (actors[i]->getState() == dead){
            delete actors[i];
            actors.erase(actors.begin()+i);
        }
    }
    
 
    
    setDisplayText();
    m_ticksSinceLastProtester++;
    if (m_TunnelMan->getHP() <= 0){
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    if (m_barrels <= 0){
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //delete all remaining earth in the earth array
    for(int i = 0; i < VIEW_WIDTH; i++){
        for (int j = 0; j < 60; j++) {
            delete m_earth[i][j];
            m_earth[i][j] = nullptr;
        }
    }
    
    //delete the TunnelMan
    delete m_TunnelMan;
    
    //delete actors in vector
    vector<Actor*>::iterator it;
    it = actors.begin();
    while (it != actors.end()){
        delete *it;
        it = actors.erase(it);
    }

}

// check if there is a block of earth at position x,y
bool StudentWorld::isSingleEarth(int x, int y){
    if (m_earth[x][y] != nullptr){
        return true;
    }
    return false;
}

// delete the earth at the 4x4 position where TunnelMan is and set appropriate nullptrs
void StudentWorld::digEarth(int x, int y){
    playSound(SOUND_DIG);
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++) {
            if (isSingleEarth(x+i, y+j)){
                delete m_earth[x+i][y+j];
                m_earth[x+i][y+j] = nullptr;
            }
        }
    }
}

// check if there is earth at any 4x4 position with x,y as base bottom-left 
bool StudentWorld::isEarthAt(int x, int y){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++) {
            if (m_earth[x+i][y+j] != nullptr){
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::withinDistance(int x, int y, int target){
    for (int i = 0; i < actors.size(); i++) {
        int distance = sqrt(pow(actors[i]->getX() - x, 2) + pow(actors[i]->getY() - y, 2));
        if (distance <= target)
            return true;
    }
    return false;
}

// return true if the 4x4 area with x,y as base point in the bottom-left corner is not out of bounds and does not overlap with any boulders
bool StudentWorld::canMove(int x, int y){
    if (x < 0 || x > 60 || y > 60 || y < 0) {
        return false;
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (isBoulderAt(x+i, y+j)){
                return false;
            }
        }
    }
    return true;
}

//return true if the position x,y is occupied by any part of an existing boulder
bool StudentWorld::isBoulderAt(int x, int y){
    for (int i = 0; i < actors.size() && actors[i]->getID() == TID_BOULDER; i++){
        for (int n = 0; n < 4; n++){
            if (actors[i]->getY()+n == y){
                for (int k = 0; k < 4; k++){
                    if (actors[i]->getX()+k == x){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void StudentWorld::createNewSquirt(Squirt* addSquirt){
    if (!isSingleEarth(addSquirt->getX(), addSquirt->getY()) && !isBoulderAt(addSquirt->getX(), addSquirt->getY())){
        actors.push_back(addSquirt);
    } else {
        delete addSquirt;
    }
};

void StudentWorld::createNewGold(GoldNugget *addNugget){
    actors.push_back(addNugget);
}

void StudentWorld::getTunnelManPos(int& x, int& y){
    x = m_TunnelMan->getX();
    y = m_TunnelMan->getY();
}

void StudentWorld::collectedBarrel(){
    m_barrels--;
}

TunnelMan* StudentWorld::getTunnelMan(){
    return m_TunnelMan;
}

void StudentWorld::revealGoodies(int x, int y){
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->getID() == TID_BARREL || actors[i]->getID() == TID_GOLD){
            int distance = sqrt(pow(actors[i]->getX() - x, 2) + pow(actors[i]->getY() - y, 2));
            if (distance <= 12){
                actors[i]->setVisible(true);
            }
        }
    }
}

void StudentWorld::setDisplayText(){
    int level = getLevel();
    int lives = getLives();
    int health = m_TunnelMan->getHP() * 10;
    int squirts = m_TunnelMan->getSquirts();
    int nuggets = m_TunnelMan->getNuggets();
    int barrelsLeft = m_barrels;
    int sonar = m_TunnelMan->getSonar();
    int score = getScore();
  
    
    int length = 0;
    int temp = score;
    if (temp <= 999999){
        while (temp > 0){
            temp /= 10;
            length++;
        }
    }
    length = 6 - length;
    string zeros;
    if (score == 0){
        zeros = "00000";
    }
    else {
        while (length > 0){
        zeros += "0";
        length--;
        }
    }
    
    
    string s = " Lvl:  " + to_string(level) + " Lives: " + to_string(lives) + " Hlth " + to_string(health) + "% Wtr: " + to_string(squirts) + " Gld:  " + to_string(nuggets) + " Oil left:  " + to_string(barrelsLeft) + " Sonar:  " + to_string(sonar) + " Scr: " + zeros + to_string(score);
    setGameStatText(s);
}

bool StudentWorld::bribeProtester(int x, int y){
    for (int i = 0; i < actors.size(); i++){
        if (actors[i]->getID() == TID_PROTESTER || actors[i]->getID() == TID_HARD_CORE_PROTESTER){
            if (actors[i]->getX() == x && actors[i]->getY() == y){
                playSound(SOUND_PROTESTER_FOUND_GOLD);
                increaseScore(25);
                actors[i]->setState(leaving);
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::annoyProtestorSW(int x, int y, int HP, int score){
    if (score == 500){
        if (m_TunnelMan->getX() == x && m_TunnelMan->getY() == y){
            m_TunnelMan->decHP(100);
            playSound(SOUND_PLAYER_GIVE_UP);
        }
    }
    for (int i = 0; i < actors.size(); i++){
        if (actors[i]->getID() == TID_PROTESTER || actors[i]->getID() == TID_HARD_CORE_PROTESTER){
            int distance = sqrt(pow(actors[i]->getX() - x, 2) + pow(actors[i]->getY() - y, 2));
            if (distance <= 3){
                Protestor* ProtestorPtr = dynamic_cast<Protestor*>(actors[i]);
                ProtestorPtr->decHP(HP);
                ProtestorPtr->annoyProtestor();
                if (ProtestorPtr->getHP() <= 0){
                    //boulder
                    if (score == 500){
                        increaseScore(score);
                    }
                    //water squirt
                    else if (score == 100){
                        if (ProtestorPtr->getID() == TID_HARD_CORE_PROTESTER){
                            increaseScore(250);
                        } else {
                            increaseScore(score);
                        }
                    }
                }
                return true;
            }
        }
    }
    return false;
}

//void StudentWorld::createMaze(){
//    for (int i = 0; i < 64; i++){
//        for (int j = 0; j < 64; j++){
//            if (m_earth[i][j] == nullptr){
//                m_maze[i][j] = 1;
//            } else {
//                m_maze[i][j] = 0;
//            }
//        }
//    }
//    for (int i = 0; i < actors.size(); i++){
//        if (actors[i]->getID() == TID_BOULDER) {
//            int x = actors[i]->getX();
//            int y = actors[i]->getY();
//            for (int i = 0; i < 4; i++){
//                for (int j = 0; j < 4; j++){
//                    m_maze[x+i][y+j] = 0;
//                }
//            }
//        }
//    }
//}



//bool StudentWorld::solveMaze(int x, int y){
//    if (x == 60 && y == 60){
//        return true;
//    }
//    return false;
//}
