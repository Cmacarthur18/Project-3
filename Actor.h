#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
//#include "StudentWorld.h"
#include "GameWorld.h"
#include <cstdlib>
using namespace std;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class GameWorld;
//int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0
class Actor: public GraphObject{
public:
    Actor(int imageID, double start_x, double start_y, Direction dir, int depth, StudentWorld* newWorld): m_status(true),
    new_World(newWorld),GraphObject(imageID, start_x, start_y, dir, depth){}
    virtual void doSomething(){}; // Implement this
    virtual bool Blocks(){ return false;} // object blocks another
    virtual bool Overlaps() {return true;} // implement this more
    StudentWorld* getWorld1() const;
    bool isAlive(){return m_status;}
    void setDead() { m_status = false;}
    virtual bool spits() {return false;}
    virtual bool damagedByFlame(){ return false;}
    virtual bool Moves(){return false;}
    virtual bool affectedByVomit(){return false;}
    virtual void setInfection(){infection_status = true;}
    virtual bool getInfectStatus(){ return infection_status;}
    void cure(){ // ios this only for penelope
        infection_status = false;
        infection = 0;
    }
    virtual void increase() { infection++;}
    virtual int infectionTotal() {return infection;}
    virtual void setSaved(){saved = true;}
    virtual bool isSaved(){return saved;}
    virtual bool follows(){return false;}
    
private:
    StudentWorld* new_World;
    bool saved = false;
    bool m_status;
    bool infection_status = false;;
    int infection = 0; // change this lster to only be set to 0 for human ond citizen
};
// ACTORS//////////////////////////////////////////////////////////////////////////////////////////

class Vomit: public Actor{
public:
    Vomit(double start_x, double start_y, StudentWorld* newWorld): time(0), Actor(IID_VOMIT, start_x, start_y, right, 0 , newWorld){}
    void doSomething();
    
private:
    int time;
};

class MoveableActor: public Actor{
public:
    
    MoveableActor(int imageID, double start_x, double start_y, Direction dir, int depth, StudentWorld* newWorld):Actor(imageID, start_x, start_y, dir, depth, newWorld){}
    virtual bool Blocks(){ return true;}
    virtual bool damagedByFlame(){return true;}
    virtual bool Moves(){return true;}
    
};

class Penelope: public MoveableActor{
public:
    Penelope(int imageID, double start_x, double start_y, StudentWorld* newWorld): MoveableActor(imageID, start_x, start_y, right, 0, newWorld), m_charges(0), m_shots(0), m_mines(0), is_completed(false){}
    virtual void doSomething();
    int getFlameCharges();
    void decFlameCharge();
    void addFLameCharge();
    int getShots();
    void decShots();
    void addShots();
    int getMines();
    void decMines();
    virtual bool affectedByVomit(){return true;}
    void addMines();
    void Setcompleted(bool exited){
        is_completed = exited;
    }
    bool isComplete(){ return is_completed;}
private:
    int m_charges;
    int m_shots;
    int m_mines;
    bool is_completed;
};

class Citizen: public MoveableActor{
public:
    Citizen( double start_x, double start_y, StudentWorld* newWorld): MoveableActor(IID_CITIZEN, start_x, start_y, right, 0, newWorld), m_time(0){}
    virtual bool affectedByVomit(){return true;}
    virtual void doSomething();
    bool follows(){return true;}
    void checkMove();

private:
    int m_time;

};

class Zombie: public MoveableActor{
public:
    Zombie( int imageID, double start_x, double start_y, Direction dir, int depth, StudentWorld* newWorld):MoveableActor(imageID, start_x, start_y, dir, depth, newWorld){}
    bool computeMove();
    bool computevomit();
    void update();
    virtual bool spits(){return true;}
private:
    
};

class DumbZombie: public Zombie{
public:
    DumbZombie( double start_x, double start_y, StudentWorld* newWorld): Zombie(IID_ZOMBIE, start_x, start_y, right, 0, newWorld){
        m_plan = 3+rand()%8;
        m_ticks = -1;
        m_vaxChance = 5;
        m_vomitChance = rand()%3;
    }
    void doSomething();

private:
    int m_plan;
    int m_ticks;
    int m_vaxChance;
    int m_vomitChance;
    
};

class SmartZombie: public Zombie{
public:
    SmartZombie( double start_x, double start_y, StudentWorld* newWorld): Zombie(IID_ZOMBIE, start_x, start_y, right, 0, newWorld){
        m_plan = 3+rand()%8;
        m_ticks = -1;
        m_vaxChance = 5;
        m_vomitChance = rand()%3;
    }
    bool follows(){return true;}
    void doSomething();
    
private:
    int m_plan;
    int m_ticks;
    int m_vaxChance;
    int m_vomitChance;
    
};



// ENVIRONMENT///////////////////////////////////////////////////////////////////////////////////

class Environment: public Actor{
public:
    Environment(int imageID, double start_x, double start_y, Direction dir, int depth,StudentWorld* newWorld):
        Actor( imageID, start_x, start_y, right, 1, newWorld){}
    
};

///////Goodies/////////////////////////////////////////////////////////////////////////////////////
class Goodie: public Environment{
public:
    Goodie(int imageID, double start_x, double start_y, Direction dir, int depth,StudentWorld* newWorld):
    Environment( imageID, start_x, start_y, right, depth, newWorld){}
    virtual void doSomething(){};
    virtual bool damagedByFlame(){return true;}
    
    // we need to initialize it to alive
private:
    // we need to have an alive state
    //virtual void doSomething(){};
};


class gasCan: public Goodie{
public:
    gasCan(double start_x, double start_y, StudentWorld* newWorld):  Goodie(IID_GAS_CAN_GOODIE, start_x, start_y, right, 1, newWorld){}
    virtual void doSomething();
    
};

class VaccineGoodie: public Goodie{
public:
    VaccineGoodie(double start_x, double start_y, StudentWorld* newWorld):  Goodie(IID_VACCINE_GOODIE, start_x, start_y, right, 1, newWorld){}
    virtual void doSomething();
    
};

class LandMine: public Goodie{
public:
    LandMine(double start_x, double start_y, StudentWorld* newWorld):  Goodie(IID_LANDMINE_GOODIE, start_x, start_y, right, 1, newWorld){}
    virtual void doSomething();
    
};

// OBSTACLES///////////////////////////////////////////////////////////////////////////////////////////////////
class Obstacles: public Environment{
public:
    Obstacles(int imageID, double start_x, double start_y, Direction dir, int depth,StudentWorld* newWorld):
        Environment(imageID, start_x, start_y, right, depth, newWorld){}
    
private:
    //Penelope* penelope;
    
};
class Exit: public Obstacles{
public:
    Exit(double start_x, double start_y, StudentWorld* newWorld):Obstacles(IID_EXIT, start_x, start_y, right, 1 , newWorld){}
    bool Overlaps(){ return true;}
    virtual void doSomething();
private:
};

class Wall: public Obstacles{
public:
    Wall(double start_x, double start_y, StudentWorld* newWorld): Obstacles(IID_WALL, start_x, start_y, right, 1 , newWorld){}
    virtual void doSomething(){ // do something does nothing
    }
    virtual bool Blocks() { return true;}
    virtual bool Overlaps() { return false;}
};

class Mine: public Obstacles{
public:
    Mine(double start_x, double start_y, StudentWorld* newWorld): time(60), Obstacles(IID_LANDMINE, start_x, start_y, right, 1 , newWorld){}
    virtual void doSomething(); // do something does nothing
private:
    int time;
    
    
    //CHANGE THESE
    virtual bool Blocks() { return false;}
    //virtual bool Overlaps() { return false;}
};

class Pit: public Obstacles{
public:
    Pit(double start_x, double start_y, StudentWorld* newWorld): Obstacles(IID_PIT, start_x, start_y, right, 1 , newWorld){}
    virtual bool Blocks() { return false;}
    virtual bool Overlaps() { return true;}
    virtual void doSomething();
};


class Flame: public Actor{
public:
    Flame(double start_x, double start_y, Direction dir, StudentWorld* newWorld): time(0), Actor(IID_FLAME, start_x, start_y, right, 0 , newWorld){}
    void doSomething();
    
private:
    int time;

    
};

#endif // ACTOR_H_
