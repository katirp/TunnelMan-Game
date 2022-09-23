#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Actor implementations
///////////////////////////////////////////////////////////////////////////

Actor::Actor(StudentWorld* swp, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) : GraphObject(imageID, startX, startY, dir, size, depth) {
    setVisible(true);
    m_StudentWorld = swp;
    m_state = stable;
}

Actor::~Actor(){}

void Actor::doSomething(){}

int Actor::getState(){
    return m_state;
}

void Actor::setState(int state){
    m_state = state;
}

void Actor::dirMoveTo(int x, int y, Direction dir){
    if (dir == left){
        moveTo(x-1, y);
    }
    else if (dir == right){
        moveTo(x+1, y);
    }
    else if (dir == up){
        moveTo(x, y+1);
    }
    else if (dir == down){
        moveTo(x, y-1);
    }
}

bool Actor::noBoulderOrEarth(int x, int y, Direction dir){
    if (dir == right){
        if (!getWorld()->isEarthAt(getX()+1, getY()) && !getWorld()->isBoulderAt(getX()+1, getY()) && getX()+1 <= 60){
            return true;
        }
    }
    else if (dir == left){
        if (!getWorld()->isEarthAt(getX()-1, getY()) && !getWorld()->isBoulderAt(getX()-1, getY()) && getX()-1 >= 0){
            return true;
        }
    }
    else if (dir == up){
        if (!getWorld()->isEarthAt(getX(), getY()+1) && !getWorld()->isBoulderAt(getX(), getY()+1) && getY()+1 <= 60){
            return true;
        }
    }
    else if (dir == down){
        if (!getWorld()->isEarthAt(getX(), getY()-1) && !getWorld()->isBoulderAt(getX(), getY()-1) && getY()-1 >= 0){
            return true;
        }
    }
    else if (dir == none){
        if (!getWorld()->isEarthAt(getX(), getY()) && !getWorld()->isBoulderAt(getX(), getY())){
            return true;
        }
    }
    return false;
}

StudentWorld* Actor::getWorld() {
   return m_StudentWorld;
}

///////////////////////////////////////////////////////////////////////////
//  Earth implementations
///////////////////////////////////////////////////////////////////////////

Earth::Earth(StudentWorld* swp, int startX, int startY) : Actor (swp, TID_EARTH, startX, startY, right, 0.25, 3){}

Earth::~Earth(){}

void Earth::doSomething(){}

///////////////////////////////////////////////////////////////////////////
//  Human implementations
///////////////////////////////////////////////////////////////////////////

Human::Human(StudentWorld* swp, int hitpoints, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) : Actor (swp, imageID, startX, startY, dir, size, depth) {
    m_hitpoints = hitpoints;
}

Human::~Human(){}

void Human::doSomething(){}

int Human::getHP() const { return m_hitpoints; }

void Human::decHP(int howMuch){
    m_hitpoints -= howMuch;
}

///////////////////////////////////////////////////////////////////////////
//  Tunnel Man implementations
///////////////////////////////////////////////////////////////////////////

TunnelMan::TunnelMan(StudentWorld* swp) : Human(swp, 10, TID_PLAYER, 30, 60, right, 1.0, 0) {
    m_squirtCount = 5;
    m_nuggets = 0;
    m_sonar = 1;
}

TunnelMan::~TunnelMan(){}

int TunnelMan::getSquirts() const{
    return m_squirtCount;
}

int TunnelMan::getNuggets() const{
    return m_nuggets;
}

int TunnelMan::getSonar() const{
    return m_sonar;
}

void TunnelMan::incNuggets(){
    m_nuggets++;
}

void TunnelMan::incSonar(){
    m_sonar++;
}

void TunnelMan::incSquirts(){
    m_squirtCount += 5;
}

void TunnelMan::doSomething(){
    if (getHP() == 0){
        return;
    }
    int ch;
    if (getWorld()->getKey(ch) == true){
        if (ch == KEY_PRESS_ESCAPE){
             decHP(1000);
        }
        //squirt
        if (ch == KEY_PRESS_SPACE){
            if (m_squirtCount > 0){
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                if (getDirection()==left){
                    Squirt* addSquirt = new Squirt(getWorld(), getX()-4, getY(), getDirection());
                    getWorld()->createNewSquirt(addSquirt);
                } else if (getDirection()==right){
                    Squirt* addSquirt = new Squirt(getWorld(), getX()+4, getY(), getDirection());
                    getWorld()->createNewSquirt(addSquirt);
                } else if (getDirection()==down){
                    Squirt* addSquirt = new Squirt(getWorld(), getX(), getY()-4, getDirection());
                    getWorld()->createNewSquirt(addSquirt);
                } else if (getDirection()==up){
                    Squirt* addSquirt = new Squirt(getWorld(), getX(), getY()+4, getDirection());
                    getWorld()->createNewSquirt(addSquirt);
                }
                m_squirtCount--;
            }
        }
        if (ch == KEY_PRESS_LEFT){
            if (getDirection() != left){
                setDirection(left);
            } else if (getWorld()->canMove(getX()-1, getY())){
                moveTo(getX()-1, getY());
            }
        }
        if (ch == KEY_PRESS_RIGHT){
            if (getDirection() != right){
                setDirection(right);
            } else if (getWorld()->canMove(getX()+1, getY())){
                moveTo(getX()+1, getY());
            }
        }
        if (ch == KEY_PRESS_UP){
            if (getDirection() != up){
                setDirection(up);
            } else if (getWorld()->canMove(getX(), getY()+1)){
                moveTo(getX(), getY()+1);
            }
        }
        if (ch == KEY_PRESS_DOWN){
            if (getDirection() != down){
                setDirection(down);
            } else if (getWorld()->canMove(getX(), getY()-1)){
                moveTo(getX(), getY()-1);
            }
        }
        if (ch == 'z' || ch == 'Z'){
            if (m_sonar > 0){
                m_sonar--;
                getWorld()->revealGoodies(getX(), getY());
            }
        }
        if (ch == KEY_PRESS_TAB){
            if (m_nuggets > 0){
                m_nuggets--;
                GoldNugget* addNugget = new GoldNugget(getWorld(), getX(), getY(), true, temporary);
                getWorld()->createNewGold(addNugget);
            }
        }
    }
    
    if (getWorld()->isEarthAt(getX(), getY()) == true){
        getWorld()->digEarth(getX(), getY());
    }
}

///////////////////////////////////////////////////////////////////////////
//  Boulder implementations
///////////////////////////////////////////////////////////////////////////

Boulder::Boulder(StudentWorld* swp, int x, int y) : Actor(swp, TID_BOULDER, x, y, down, 1.0, 1) {
    m_timer = 0;
}

Boulder::~Boulder(){}

void Boulder::doSomething(){
    if (getState() == dead){
        return;
    }
    if (getState() == stable){
        int earth_count = 0;
        for (int i = 0; i < 4; i++){
            if (getWorld()->isSingleEarth(getX()+i, getY()-1)){
                earth_count++;
            }
        }
        if (earth_count == 0){
            setState(waiting);
            return;
        }
    }
    if (getState() == waiting && m_timer < 30){
        m_timer++;
        return;
    }
    if (getState() == waiting && m_timer == 30){
        setState(falling);
        getWorld()->playSound(SOUND_FALLING_ROCK);
        return;
    }
    if (getState() == falling){
        if (!noBoulderOrEarth(getX(), getY(), down)){
            setState(dead);
            return;
        }
        else {
        moveTo(getX(), getY()-1);
        getWorld()->annoyProtestorSW(getX(), getY(), 100, 500);
        }
    }
}
    
    
///////////////////////////////////////////////////////////////////////////
//  Squirt implementations
///////////////////////////////////////////////////////////////////////////

Squirt::Squirt(StudentWorld* swp, int x, int y, Direction dir) : Actor(swp, TID_WATER_SPURT, x, y, dir, 1.0, 1.0){
    m_distanceTravelled = 0;
}

Squirt::~Squirt(){}

void Squirt::doSomething(){
    if (getState() == dead){
        return;
    }
    if (getWorld()->annoyProtestorSW(getX(), getY(), 2, 100) == true){
        setState(dead);
        return;
    }
    if (m_distanceTravelled >= 4){
        setState(dead);
        return;
    }
    if (getDirection() == left){
        if (getWorld()->isSingleEarth(getX()-1, getY()) || getWorld()->isBoulderAt(getX()-1, getY())) {
            setState(dead);
        } else {
            moveTo(getX()-1, getY());
            m_distanceTravelled++;
        }
    }
    else if (getDirection() == right){
        for (int i = 1; i < 5; i++){
            if (getWorld()->isSingleEarth(getX()+i, getY()) || getWorld()->isBoulderAt(getX()+i, getY())) {
                setState(dead);
            }
        }
        if (getState() != dead) {
            moveTo(getX()+1, getY());
            m_distanceTravelled++;

        }
    }
    else if (getDirection() == up){
        for (int i = 1; i < 5; i++){
            if (getWorld()->isSingleEarth(getX(), getY()+i) || getWorld()->isBoulderAt(getX(), getY()+i)) {
                setState(dead);
            }
        }
        if (getState() != dead) {
            moveTo(getX(), getY()+1);
            m_distanceTravelled++;
        }
    }
    else if (getDirection() == down){
        if (getWorld()->isSingleEarth(getX(), getY()-1) || getWorld()->isBoulderAt(getX(), getY()-1)) {
            setState(dead);
        } else {
            moveTo(getX(), getY()-1);
            m_distanceTravelled++;
        }
    } 
}

///////////////////////////////////////////////////////////////////////////
//  Goodie implementations
///////////////////////////////////////////////////////////////////////////

Goodie::Goodie(StudentWorld* swp, int imageID, int x, int y, Direction dir, double size, unsigned int depth) : Actor(swp, imageID, x, y, dir, size, depth){

}

Goodie::~Goodie(){}



void Goodie::doSomething(){}

void Goodie::collect(int sound, int points){
    setState(dead);
    getWorld()->playSound(sound);
    getWorld()->increaseScore(points);
}

///////////////////////////////////////////////////////////////////////////
//  Barrel implementations
///////////////////////////////////////////////////////////////////////////

Barrel::Barrel(StudentWorld* swp, int x, int y) : Goodie(swp, TID_BARREL, x, y, right, 1.0, 2){
    setVisible(false);
}

Barrel::~Barrel(){}

void Barrel::doSomething(){
    if (getState()==dead){
        return;
    }
    int x = 0;
    int y = 0;
    getWorld()->getTunnelManPos(x, y);
    int distance = sqrt(pow(getX() - x, 2) + pow(getY() - y, 2));
    if (!isVisible() && distance <= 4){
        setVisible(true);
        return;
    } else if (distance <= 3){
        collect(SOUND_FOUND_OIL, 1000);
        getWorld()->collectedBarrel();
    }
}

///////////////////////////////////////////////////////////////////////////
//  Gold Nugget implementations
///////////////////////////////////////////////////////////////////////////

GoldNugget::GoldNugget(StudentWorld* swp, int x, int y, bool invisible, int state) : Goodie(swp, TID_GOLD, x, y, right, 1.0, 2){
    setVisible(invisible);
    setState(state);
    m_ticks = 100;
}

GoldNugget::~GoldNugget(){}

void GoldNugget::doSomething(){
    if (getState()==dead){
        return;
    }
    int x = 0;
    int y = 0;
    getWorld()->getTunnelManPos(x, y);
    int distance = sqrt(pow(getX() - x, 2) + pow(getY() - y, 2));
    
    if (!isVisible() && distance <= 4){
        setVisible(true);
        return;
    } else if (getState() == permanent && distance <= 3){
        collect(SOUND_GOT_GOODIE, 10);
        getWorld()->getTunnelMan()->incNuggets();
    } else if (getState() == temporary){
        if (getWorld()->bribeProtester(getX(), getY()) == true){
            setState(dead);
        }
        m_ticks--;
    }
    if (getState() == temporary && m_ticks <= 0){
        setState(dead);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Sonar Kit implementations
///////////////////////////////////////////////////////////////////////////

SonarKit::SonarKit(StudentWorld* swp, int x, int y) : Goodie(swp, TID_SONAR, x, y, right, 1.0, 2){
    setState(temporary);
    int t = 300 - 10*(getWorld()->getLevel());
    m_ticks = max(100, t);
}

SonarKit::~SonarKit(){}

void SonarKit::doSomething(){
    if (getState()==dead){
        return;
    }
    int x = 0;
    int y = 0;
    getWorld()->getTunnelManPos(x, y);
    int distance = sqrt(pow(getX() - x, 2) + pow(getY() - y, 2));
    if (distance <= 3){
        collect(SOUND_GOT_GOODIE, 75);
        getWorld()->getTunnelMan()->incSonar();
    }
    m_ticks--;
    if (m_ticks <= 0){
        setState(dead);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Water Pool implementations
///////////////////////////////////////////////////////////////////////////

WaterPool::WaterPool(StudentWorld* swp, int x, int y) : Goodie(swp, TID_WATER_POOL, x, y, right, 1.0, 2){
    int t = 300 - 10*(getWorld()->getLevel());
    m_ticks = max(100, t);
}

WaterPool::~WaterPool(){}

void WaterPool::doSomething(){
    if (getState()==dead){
        return;
    }
    int x = 0;
    int y = 0;
    getWorld()->getTunnelManPos(x, y);
    int distance = sqrt(pow(getX() - x, 2) + pow(getY() - y, 2));
    if (distance <= 3){
        collect(SOUND_GOT_GOODIE, 100);
        getWorld()->getTunnelMan()->incSquirts();
    }
    m_ticks--;
    if (m_ticks <= 0){
        setState(dead);
    }
}


///////////////////////////////////////////////////////////////////////////
//  Protestor implementations
///////////////////////////////////////////////////////////////////////////

Protestor::Protestor(StudentWorld* swp, int HP, int imageID) : Human(swp, HP, imageID, 60, 60){
    setDirection(left);
    m_SquaresToMoveInCurrentDirection = rand() % 8 + 52;
    int l = 3 - (getWorld()->getLevel())/4;
    m_ticksToWaitBetweenMoves = max(0, l);
    m_restCount = 0;
    m_nonRestingTickCount = 0;
    m_ticksSinceLastTurn = 0;
}

Protestor::~Protestor(){}

int Protestor::getRestCount() const{
    return m_restCount;
}

int Protestor::getTicksToWaitBetweenMoves() const {
    return m_ticksToWaitBetweenMoves;
}

int Protestor::getTicksSinceLastShout() const{
    return m_nonRestingTickCount;
}

int Protestor::getSquaresToMoveInCurrentDirection() const{
    return m_SquaresToMoveInCurrentDirection;
}

int Protestor::getTicksSinceLastTurn() const{
    return m_ticksSinceLastTurn;
}

void Protestor::incRestCount(){
    m_restCount++;
}

void Protestor::setRestCount(int n){
    m_restCount = n;
}

void Protestor::incNonRestingTickCount(){
    m_nonRestingTickCount++;
}

void Protestor::clearNonRestingTickCount(){
    m_nonRestingTickCount = 0;
}

void Protestor::setSquaresToMoveInCurrentDirection(int n){
    m_SquaresToMoveInCurrentDirection = n;
}

void Protestor::decSquaresToMoveInCurrentDirection(){
    m_SquaresToMoveInCurrentDirection--;
}

void Protestor::setTicksSinceLastTurn(int n){
    m_ticksSinceLastTurn = n;
}

void Protestor::incTicksSinceLastTurn(){
    m_ticksSinceLastTurn++;
}

bool Protestor::canSeeTunnelMan(){
    int tunnelManX = getWorld()->getTunnelMan()->getX();
    int tunnelManY = getWorld()->getTunnelMan()->getY();
    if (getX() == tunnelManX){
        if (getY() >= tunnelManY){
            for (int i = tunnelManY; i < getY(); i++){
                if (getWorld()->isEarthAt(getX(), i) || getWorld()->isBoulderAt(getX(), i)){
                    return false;
                }
            }
        } else if (tunnelManY > getY()){
            for (int i = getY(); i < tunnelManY; i++){
                if (getWorld()->isEarthAt(getX(), i) || getWorld()->isBoulderAt(getX(), i)){
                    return false;
                }
            }
        }
    }
    else if (getY() == tunnelManY){
        if (getX() >= tunnelManX){
            for (int i = tunnelManX; i < getX(); i++){
                if (getWorld()->isEarthAt(i, getY()) || getWorld()->isBoulderAt(i, getY())){
                    return false;
                }
            }
        } else if (tunnelManX > getX()){
            for (int i = getX(); i < tunnelManX; i++){
                if (getWorld()->isEarthAt(i, getY()) || getWorld()->isBoulderAt(i, getY())){
                    return false;
                }
            }
        }
    }
    return true;
}

bool Protestor::isFacing(int x, int y, Direction dir){
    if (dir == left){
        if (x <= getX()) {return true;}
        else {return false;}
    }
    else if (dir == right){
        if (x >= getX()) {return true;}
        else {return false;}
    }
    else if (dir == up){
        if (y >= getY()) {return true;}
        else {return false;}
    }
    else {
        if (y <= getY()) {return true;}
        else {return false;}
    }
}

bool Protestor::isAtIntersection(){
    if (getDirection() == left || getDirection() == right){
        if (noBoulderOrEarth(getX(), getY(), up) || noBoulderOrEarth(getX(), getY(), down)){
            return true;
        }
    }
    else if (getDirection() == up || getDirection() == down){
        if (noBoulderOrEarth(getX(), getY(), left) || noBoulderOrEarth(getX(), getY(), right)){
            return true;
        }
    }
    return false;
}

void Protestor::annoyProtestor(){
    if (getHP() > 0){
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        int e = 100 - getWorld()->getLevel() * 10;
        int extendRest = max(50, e);
        if (getState() != rest){setState(rest);}
        setRestCount(getRestCount() - extendRest);
    } else if (getHP() <= 0){
        setState(leaving);
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setRestCount(0);
    }
}

void Protestor::doSomething(){}

///////////////////////////////////////////////////////////////////////////
//  Regular Protestor implementations
///////////////////////////////////////////////////////////////////////////

RegularProtestor::RegularProtestor(StudentWorld* swp) : Protestor(swp, 5, TID_PROTESTER){

}

RegularProtestor::~RegularProtestor(){}

void RegularProtestor::doSomething(){
    if (getState() == dead){
        return;
    }
    if (getState() == rest){
        if (getRestCount() >= getTicksToWaitBetweenMoves() - 1){
            setState(stable);
            setRestCount(0);
            return;
        }
        else {
            incRestCount();
            return;
        }
    }
    if (getState() == leaving){
//        if (getX() == 60 && getY() == 60){
//            setState(dead);
//        }
        setState(dead);
        return;
    }
    int x = 0;
    int y = 0;
    getWorld()->getTunnelManPos(x, y);
    int distance = sqrt(pow(getX() - x, 2) + pow(getY() - y, 2));
    if (distance <= 4 && isFacing(x, y, getDirection()) && getTicksSinceLastShout() >= 15){
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        getWorld()->getTunnelMan()->decHP(2);
        if (getWorld()->getTunnelMan()->getHP() <= 0) {
            getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        }
        clearNonRestingTickCount();
        setState(rest);
        return;
    }
    //if tunnel man is in sight move towards him
    if (getX() == x || getY() == y){
        if (canSeeTunnelMan() && distance > 4){
            if (x == getX()){
                if (y < getY()){
                    setDirection(down);
                } else {
                    setDirection(up);
                }
            }
            else if (y == getY()){
                if (x < getX()){
                    setDirection(left);
                } else {
                    setDirection(right);
                }
            }
            if (noBoulderOrEarth(getX(), getY(), getDirection())){
                dirMoveTo(getX(), getY(), getDirection());
            }
            setSquaresToMoveInCurrentDirection(0);
            setState(rest);
            incNonRestingTickCount();
            incTicksSinceLastTurn();
            return;
        }
    }
    decSquaresToMoveInCurrentDirection();
    if (getSquaresToMoveInCurrentDirection() <= 0){
        Direction chooseDir = static_cast<Direction>(rand() % 4 + 1);
        bool blocked = true;
        while (blocked == true){
            if (noBoulderOrEarth(getX(), getY(), chooseDir)){
                blocked = false;
            }
            else {
                chooseDir = static_cast<Direction>(rand() % 4 + 1);
            }
        }
        setDirection(chooseDir);
        setSquaresToMoveInCurrentDirection(rand() % 8 + 52);
    }
    //at an intersection
    else if (isAtIntersection() && getTicksSinceLastTurn() > 200){
        if (getDirection() == left || getDirection() == right){
            if (noBoulderOrEarth(getX(), getY(), up) && noBoulderOrEarth(getX(), getY(), down)){
                setDirection(static_cast<Direction>(rand() % 2 + 1));
            } else if (noBoulderOrEarth(getX(), getY(), up)){
                setDirection(up);
            } else {
                setDirection(down);
            }
        }
        else if (getDirection() == up || getDirection() == down){
            if (noBoulderOrEarth(getX(), getY(), right) && noBoulderOrEarth(getX(), getY(), left)){
                setDirection(static_cast<Direction>(rand() % 2 + 3));
            } else if (noBoulderOrEarth(getX(), getY(), right)){
                setDirection(right);
            } else {
                setDirection(left);
            }
        }
        setSquaresToMoveInCurrentDirection(rand() % 8 + 52);
        setTicksSinceLastTurn(0);
    }
    //if the current direction to move in is not blocked, move one square in that direction, otherwise, set m_SquaresToMoveInCurrentDirection to 0
    if (noBoulderOrEarth(getX(), getY(), getDirection())){
        dirMoveTo(getX(), getY(), getDirection());
        setState(rest);
        incNonRestingTickCount();
        incTicksSinceLastTurn();
    }
    else {
        setSquaresToMoveInCurrentDirection(0);
        setState(rest);
        incNonRestingTickCount();
        incTicksSinceLastTurn();
    }
    
}

///////////////////////////////////////////////////////////////////////////
//  Hardcore Protestor implementations
///////////////////////////////////////////////////////////////////////////

HardcoreProtestor::HardcoreProtestor(StudentWorld* swp) : Protestor(swp, 20, TID_HARD_CORE_PROTESTER){

}

HardcoreProtestor::~HardcoreProtestor(){}

void HardcoreProtestor::doSomething(){
    if (getState() == dead){
        return;
    }
    if (getState() == rest){
        if (getRestCount() >= getTicksToWaitBetweenMoves() - 1){
            setState(stable);
            setRestCount(0);
            return;
        }
        else {
            incRestCount();
            return;
        }
    }
    if (getState() == leaving){
//        if (getX() == 60 && getY() == 60){
//            setState(dead);
//        }
        setState(dead);
        return;
    }
    int x = 0;
    int y = 0;
    getWorld()->getTunnelManPos(x, y);
    int distance = sqrt(pow(getX() - x, 2) + pow(getY() - y, 2));
    if (distance <= 4 && isFacing(x, y, getDirection()) && getTicksSinceLastShout() >= 15){
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        getWorld()->getTunnelMan()->decHP(2);
        if (getWorld()->getTunnelMan()->getHP() <= 0) {
            getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        }
        clearNonRestingTickCount();
        setState(rest);
        return;
    }
    //if tunnel man is in sight move towards him
    if (getX() == x || getY() == y){
        if (canSeeTunnelMan() && distance > 4){
            if (x == getX()){
                if (y < getY()){
                    setDirection(down);
                } else {
                    setDirection(up);
                }
            }
            else if (y == getY()){
                if (x < getX()){
                    setDirection(left);
                } else {
                    setDirection(right);
                }
            }
            if (noBoulderOrEarth(getX(), getY(), getDirection())){
                dirMoveTo(getX(), getY(), getDirection());
            }
            setSquaresToMoveInCurrentDirection(0);
            setState(rest);
            incNonRestingTickCount();
            incTicksSinceLastTurn();
            return;
        }
    }
    decSquaresToMoveInCurrentDirection();
    if (getSquaresToMoveInCurrentDirection() <= 0){
        Direction chooseDir = static_cast<Direction>(rand() % 4 + 1);
        bool blocked = true;
        while (blocked == true){
            if (noBoulderOrEarth(getX(), getY(), chooseDir)){
                blocked = false;
            }
            else {
                chooseDir = static_cast<Direction>(rand() % 4 + 1);
            }
        }
        setDirection(chooseDir);
        setSquaresToMoveInCurrentDirection(rand() % 8 + 52);
    }
    //at an intersection
    else if (isAtIntersection() && getTicksSinceLastTurn() > 200){
        if (getDirection() == left || getDirection() == right){
            if (noBoulderOrEarth(getX(), getY(), up) && noBoulderOrEarth(getX(), getY(), down)){
                setDirection(static_cast<Direction>(rand() % 2 + 1));
            } else if (noBoulderOrEarth(getX(), getY(), up)){
                setDirection(up);
            } else {
                setDirection(down);
            }
        }
        else if (getDirection() == up || getDirection() == down){
            if (noBoulderOrEarth(getX(), getY(), right) && noBoulderOrEarth(getX(), getY(), left)){
                setDirection(static_cast<Direction>(rand() % 2 + 3));
            } else if (noBoulderOrEarth(getX(), getY(), right)){
                setDirection(right);
            } else {
                setDirection(left);
            }
        }
        setSquaresToMoveInCurrentDirection(rand() % 8 + 52);
        setTicksSinceLastTurn(0);
    }
    //if the current direction to move in is not blocked, move one square in that direction, otherwise, set m_SquaresToMoveInCurrentDirection to 0
    if (noBoulderOrEarth(getX(), getY(), getDirection())){
        dirMoveTo(getX(), getY(), getDirection());
        setState(rest);
        incNonRestingTickCount();
        incTicksSinceLastTurn();
    }
    else {
        setSquaresToMoveInCurrentDirection(0);
        setState(rest);
        incNonRestingTickCount();
        incTicksSinceLastTurn();
    }
}
