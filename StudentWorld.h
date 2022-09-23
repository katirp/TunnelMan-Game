#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <queue>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

struct Coordinate {
public:
    Coordinate(int x, int y);
    int getX();
    int getY();
private:
    int xPos;
    int yPos;
};

class StudentWorld : public GameWorld
{
public:
    //constructor + destructor
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	
    //check if there is earth and dig
    void digEarth(int x, int y);
    bool isSingleEarth(int x, int y);
        
    bool withinDistance(int x, int y, int target);
    bool canMove(int x, int y);
    
    //accessors
    bool isBoulderAt(int x, int y);
    bool isEarthAt(int x, int y);
    void getTunnelManPos(int& x, int& y);
    TunnelMan* getTunnelMan();

    //mutators
    void collectedBarrel();
    void createNewSquirt(Squirt* addSquirt);
    void createNewGold(GoldNugget* addNugget);
    void revealGoodies(int x, int y);
    void setDisplayText();
    
    bool bribeProtester(int x, int y);
    bool annoyProtestorSW(int x, int y, int HP, int score);
    bool fallingBoulder(int x, int y);
    
//    void createMaze();
//    bool solveMaze(int x, int y);

        
private:
    Earth* m_earth [VIEW_WIDTH][VIEW_HEIGHT];
    TunnelMan* m_TunnelMan;
    std::vector<Actor*> actors;
    int m_barrels;
    int m_ticksSinceLastProtester;
    int m_maze[64][64];
};

#endif // STUDENTWORLD_H_
