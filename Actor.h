#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// constants for state of objects
const int stable = 0;
const int waiting = 1;
const int falling = 2;
const int dead = 3;
const int temporary = 4;
const int permanent = 5;
const int rest = 6;
const int leaving = 7;

class Actor: public GraphObject {
public:
    //constructor + destructor
    Actor(StudentWorld* swp, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    virtual ~Actor();
    
    virtual void doSomething();
    
    virtual int getState();
    
    virtual void setState(int state);
    
    virtual void dirMoveTo(int x, int y, Direction dir);
    
    virtual bool noBoulderOrEarth(int x, int y, Direction dir);
    
    StudentWorld* getWorld();
        
private:
    StudentWorld* m_StudentWorld;
    int m_state;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Earth: public Actor {
public:
    //constructor + destructor
    Earth(StudentWorld* swp, int startX, int startY);
    virtual ~Earth();
    
    virtual void doSomething();
    
private:
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Human: public Actor {
public:
    //constructor + destructor
    Human(StudentWorld* swp, int hitpoints, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    virtual ~Human();
    
    //Accessor
    virtual int getHP() const;
    
    //Mutator
    virtual void decHP(int howMuch);
    
    virtual void doSomething();
    
private:
    int m_hitpoints;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TunnelMan: public Human {
public:
    //constructor + destructor
    TunnelMan(StudentWorld* swp);
    virtual ~TunnelMan();
    
    virtual void doSomething();
    
    //accessors
    int getSquirts() const;
    int getNuggets() const;
    int getSonar() const;
    
    //mutators
    void incNuggets();
    void incSonar();
    void incSquirts();
    
private:
    int m_squirtCount;
    int m_nuggets;
    int m_sonar;
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Boulder: public Actor {
public:
    Boulder(StudentWorld* swp, int x, int y);
    virtual ~Boulder();
    
    virtual void doSomething();
    
private:
    int m_timer;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Squirt: public Actor {
public:
    Squirt(StudentWorld* swp, int x, int y, Direction dir);
    virtual ~Squirt();
    
    virtual void doSomething();
    
private:
    int m_distanceTravelled;
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Goodie: public Actor {
public:
    Goodie(StudentWorld* swp, int imageID, int x, int y, Direction dir, double size, unsigned int depth);
    virtual ~Goodie();
    
    virtual void collect(int sound, int points);
    
    virtual void doSomething();

private:
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Barrel: public Goodie {
public:
    Barrel(StudentWorld* swp, int x, int y);
    virtual ~Barrel();
    
    virtual void doSomething();
    
private:
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GoldNugget: public Goodie {
public:
    GoldNugget(StudentWorld* swp, int x, int y, bool invisible, int state);
    virtual ~GoldNugget();
    
    virtual void doSomething();
    
private:
    int m_ticks;
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SonarKit: public Goodie {
public:
    SonarKit(StudentWorld* swp, int x, int y);
    virtual ~SonarKit();
    
    virtual void doSomething();
    
private:
    int m_ticks;
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WaterPool: public Goodie {
public:
    WaterPool(StudentWorld* swp, int x, int y);
    virtual ~WaterPool();
    
    virtual void doSomething();
    
private:
    int m_ticks;
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Protestor: public Human {
public:
    Protestor(StudentWorld* swp, int HP, int imageID);
    virtual ~Protestor();
    
    //accessors
    int getRestCount() const;
    int getTicksSinceLastShout() const;
    int getSquaresToMoveInCurrentDirection() const;
    int getTicksSinceLastTurn() const;
    int getTicksToWaitBetweenMoves() const;
    
    //mutators
    void incRestCount();
    void setRestCount(int n);
    void incNonRestingTickCount();
    void clearNonRestingTickCount();
    void setSquaresToMoveInCurrentDirection(int n);
    void decSquaresToMoveInCurrentDirection();
    void setTicksSinceLastTurn(int n);
    void incTicksSinceLastTurn();
        
    bool canSeeTunnelMan();
    bool isFacing(int x, int y, Direction dir);
    bool isAtIntersection();
    
    virtual void doSomething();
    
    virtual void annoyProtestor();

    
private:
    int m_SquaresToMoveInCurrentDirection;
    int m_ticksToWaitBetweenMoves;
    int m_restCount;
    int m_nonRestingTickCount;
    int m_ticksSinceLastTurn;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RegularProtestor: public Protestor {
public:
    RegularProtestor(StudentWorld* swp);
    virtual ~RegularProtestor();
    
    virtual void doSomething();
    
private:
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class HardcoreProtestor: public Protestor {
public:
    HardcoreProtestor(StudentWorld* swp);
    virtual ~HardcoreProtestor();
    
    virtual void doSomething();
    
private:
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif // ACTOR_H_
