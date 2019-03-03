#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <list>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld(){
       // cout << "called" << endl;
        cleanUp();
    }

    
    //bool blockFlame(double X, double Y, Direction dir);
    //bool objectOverlap(Actor* ob);
   

    void addFlames(double x, double y, Direction dir);
    void addMine(double X, double Y);
    void addPit(double X, double Y);
    void addVomit(double X, double Y);
    void addDumbZombie(double X, double y);
    void addSmartZombie(double X, double y);
    bool objectOverlap(Actor* player, Actor*temp);
    bool objectOverlap(double X, double Y);
    void objectOverlap1(Actor* player);
    bool checkAddObjectAt(double X, double Y);
    bool objectOverlapMine(Actor* Mine);
    double distance(double X1,double Y1, double X2, double Y2);
    bool checkBlock( Actor* temp ,double X, double Y, Direction dir);
    
   // bool checkBlock( Actor* player, Actor* temp ,double X, double Y);
    bool blockMovement(double X, double Y, Actor* player, Direction dir);
    bool distance1(double X1,double Y1);
    void addVax(double X, double Y);
    bool checkBlockPlayer(Actor* player);
    double findClosest(Actor* zombie, double &X, double &Y);
    bool checkColumn(double X);
    bool checkRow(double Y);
    double closestZombie(double X, double Y);
    bool citizenOverlapExit(Actor* temp);
    int numCitizens(){return num_citizens;}
    void decreseCitizens(){ num_citizens--;}
     Penelope* returnPlayer(){ return m_player;}
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
private:
    Penelope* m_player;
    list<Actor*> m_actors;
    int num_citizens=0;
    //1+ rand()%10;
};

#endif // STUDENTWORLD_H_
