#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <string>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>
#include <math.h>
using namespace std;
class Actor;
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp
//ADD DESTRUCTOR
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_player = nullptr;
}

double StudentWorld::closestZombie(double X, double Y){
    list<Actor*>::iterator it = m_actors.begin();
    int min = 100000;
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(temp->spits()){
            if(distance(X, Y, temp->getX(), temp->getY()) < min){
                min =distance(X, Y, temp->getX(), temp->getY());
            }
            
        
        }
        ++it;
    }
    return min;
}

bool StudentWorld::checkColumn(double X){
    list<Actor*>::iterator it = m_actors.begin();
    if(X == m_player->getX())
        return true;
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(temp->affectedByVomit() && X == temp->getX()){
            return true;
        }
        ++it;
    }
    return false;
}
bool StudentWorld::checkRow(double Y){
    list<Actor*>::iterator it = m_actors.begin();
    if(Y == m_player->getY())
        return true;
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(temp->affectedByVomit() && Y == temp->getY()){
            return true;
        }
        it++;
    }
    return false;
}


bool StudentWorld::checkBlock( Actor* temp ,double X, double Y ,Direction dir){ // new one
    if(dir == Actor::down){
        if(((temp->getX() < X+ SPRITE_WIDTH-1) && (X < temp->getX()+SPRITE_WIDTH-1))
           && (Y < temp->getY()+SPRITE_HEIGHT ) &&(Y >temp->getY() ) ){
            return true;
        }
    }
    else if(dir == Actor::up) {
        if(((temp->getX() < X+SPRITE_WIDTH-1 ) && (X < temp->getX()+SPRITE_WIDTH-1))  &&
           (Y > temp->getY()-SPRITE_HEIGHT ) &&(Y < temp->getY() ) ){
            return true;
        }
    }
    else if(dir == Actor::left) {
        if(((temp->getY() < Y +SPRITE_HEIGHT-1) && (Y < temp->getY()+SPRITE_HEIGHT-1))
           && (X < temp->getX()+SPRITE_WIDTH) && (X > temp->getX())){
            return true;
        }
    }
    else if(dir == Actor::right) {
        if( ((temp->getY() < Y + SPRITE_HEIGHT-1) && (Y <temp->getY()+SPRITE_HEIGHT-1))
           && (X > temp->getX()-SPRITE_WIDTH) && (X < temp->getX())){
            return true;
        }
    }
    return  false;
}



bool StudentWorld::checkBlockPlayer(Actor* player){
    list<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(temp->Blocks()){
            return checkBlock( temp, player->getX(), player->getY() , player->getDirection());
        }
        it++;
    }
    return false;
}

bool StudentWorld::blockMovement(double X, double Y, Actor* player, Direction dir){
    if(checkBlock(m_player,X,Y, player->getDirection()))
        return true;
    list<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(temp->Blocks()){
            if(checkBlock(temp,X,Y, dir)){
               return true;
            }
        }
        it++;
    }
    return false;
}

double StudentWorld::findClosest(Actor *zombie, double & X, double & Y){
    double min = distance(zombie->getX(), zombie->getY(), m_player->getX(), m_player->getY()); 
    X =m_player->getX();
    Y = m_player->getY();
    list<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(temp->affectedByVomit()) {
            double X2 = temp->getX() ;
            double Y2 = temp->getY();
            if(distance(zombie->getX(), zombie->getY(), X2, Y2) < min){
                min =distance(zombie->getX(), zombie->getY(), X2, Y2);
                X = X2;
                Y = Y2;
            }
        }
        it++;
    }
    return min;
}


double StudentWorld::distance(double X1,double Y1, double X2, double Y2){
    double yCord = (Y1 + SPRITE_HEIGHT/2);
    double xCord = (X1 + SPRITE_WIDTH/2);
    double yCord1 = (Y2 + SPRITE_HEIGHT/2);
    double xCord1 = (X2+ SPRITE_WIDTH/2);
    double yDist = yCord - yCord1;
    double xDist = xCord - xCord1;
    return (pow(xDist, 2) + pow(yDist, 2));
}

bool StudentWorld::distance1(double X1,double Y1){ // calculating for citizen
    list<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(temp->Blocks()){
            if(temp->affectedByVomit()) {
                double X2 = temp->getX() ;
                double Y2 = temp->getY();
                if(distance(X1, Y1, X2, Y2) <= 100)
                    return true;
            }
        }
        it++;
    }
    return false;
}


bool StudentWorld::objectOverlap(Actor* player, Actor*temp){
    if(distance(temp->getX(), temp->getY(), player->getX(), player->getY())  <=100)
        return true;
    return false;
}

bool StudentWorld::objectOverlap(double X, double Y){ // for throwing vaccine
    list<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(distance(X, Y, temp->getX(), temp->getY()) <= 100){
                return false;
        }
            it++;
    }
        return true;
}

void StudentWorld::objectOverlap1(Actor* player){ //for vomit
    list<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(temp->affectedByVomit()) {
            double X2 = temp->getX() ;
            double Y2 = temp->getY();
            if(distance(player->getX(), player->getY(), X2, Y2) <= 100){
                temp->setInfection();
            }
        }
        
        it++;
    }
    
}




bool StudentWorld::citizenOverlapExit(Actor* temp){
    list<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        Actor* temp1 = *it;
        if(temp1->affectedByVomit() && temp1->Blocks()) { // citizen
            if(distance(temp->getX(), temp->getY(), temp1->getX(), temp1->getY()) <= 100){
            temp1->setDead(); // come back to fix
            temp1->setSaved();
            increaseScore(500);
            decreseCitizens();
            return true;
            }
        }
        
        it++;
    }
    return false;
}




bool StudentWorld::objectOverlapMine(Actor* Mine){
    list<Actor*>::iterator it = m_actors.begin();
    if(distance(m_player->getX(), m_player->getY(), Mine->getX(), Mine->getY())  <=100){
        m_player->setDead();
        playSound(SOUND_PLAYER_DIE);
        return true;
    }
    
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(temp->Overlaps() && temp->Moves()){ // means moves and overlaps
            if(distance(temp->getX(), temp->getY(), Mine->getX(), Mine->getY())  <=100){
                temp->setDead();
                if(temp->affectedByVomit()){
                    increaseScore(-500);
                    playSound(SOUND_CITIZEN_DIE);
                }
                else if(temp->follows()){
                    increaseScore(2000);
                    playSound(SOUND_ZOMBIE_DIE);
                }
                else{
                    increaseScore(1000);
                    playSound(SOUND_ZOMBIE_DIE);
                }
                return true;
            }
        }
        it++;
    }
    
    return false;
}

bool StudentWorld::checkAddObjectAt(double X, double Y){ // check overlap with falmes 
    list<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        Actor* temp = *it;
        if(temp->Blocks() && !temp->damagedByFlame()){
            if( distance(temp->getX(), temp->getY(), X, Y)  <=100){
                return true;
            }
        }
        if(temp->Blocks() && temp->damagedByFlame()){
            if( distance(temp->getX(), temp->getY(), X, Y)  <=100){
                temp->setDead(); //
                return false;
            }
        }
        if (temp->damagedByFlame()){
            if( distance(temp->getX(), temp->getY(), X, Y)  <=100){
                temp->setDead(); // sets stuff overlapped by fire dead
                return false;
            }
        }
        it++;
    }
    return false;
}

void StudentWorld::addFlames(double X, double Y, Direction dir){
    m_actors.push_back(new Flame(X,Y, dir, this));
}
void StudentWorld::addMine(double X, double Y){
    m_actors.push_back(new Mine(X,Y, this));
}
void StudentWorld::addPit(double X, double Y){
    m_actors.push_back(new Pit(X,Y, this));
}

void StudentWorld::addVomit(double X, double Y){
    m_actors.push_back(new Vomit(X,Y, this));
}

void StudentWorld::addVax(double X, double Y){
    m_actors.push_back(new VaccineGoodie(X,Y, this));
}

void StudentWorld::addDumbZombie(double X, double Y){
    m_actors.push_back(new DumbZombie(X,Y,this));
}
void StudentWorld::addSmartZombie(double X, double Y){
    m_actors.push_back(new SmartZombie(X,Y,this));
}

int StudentWorld::init()
{
    
    Level lev(assetPath());
    string levelFile = "level0";
    levelFile += to_string(getLevel());
    levelFile += ".txt";
    //string levelFile = "level02.txt";
    Level::LoadResult result = lev.loadLevel(levelFile);
    if(result == Level::load_fail_file_not_found){
        cerr<< "Could not find level " << levelFile << endl;
        return GWSTATUS_PLAYER_WON;
    }
    else if(result == Level::load_fail_bad_format)
        cerr<< "Your level was improperely formated" << endl;
    else if( result == Level::load_success){
        for(int i = 0; i < SPRITE_WIDTH ; i++){
            for(int j = 0; j < SPRITE_HEIGHT; j++){
                
                Level:: MazeEntry ge;
                ge = lev.getContentsOf(i, j);
                switch(ge){
                    case Level::empty:
                        break;
                    case Level::player:
                        m_player = new Penelope(IID_PLAYER, i*SPRITE_WIDTH,j*SPRITE_HEIGHT, this);
                        break;
                    case Level::dumb_zombie:
                        m_actors.push_back(new DumbZombie(i*SPRITE_WIDTH,j*SPRITE_HEIGHT, this));
                        break;
                    case Level::smart_zombie:
                        m_actors.push_back(new SmartZombie(i*SPRITE_WIDTH,j*SPRITE_HEIGHT, this));
                        break;
                    case Level::citizen:
                        m_actors.push_back(new Citizen(i*SPRITE_WIDTH,j*SPRITE_HEIGHT, this));
                        num_citizens++;
                        break;
                    case Level::wall:
                       m_actors.push_back(new Wall(i*SPRITE_WIDTH,j*SPRITE_HEIGHT, this));
                        break;
                    case Level::exit:
                        m_actors.push_back(new Exit(i*SPRITE_WIDTH,j*SPRITE_HEIGHT, this));
                        break;
                    case Level::pit:
                         m_actors.push_back(new Pit(i*SPRITE_WIDTH,j*SPRITE_HEIGHT, this));
                        break;
                    case Level::vaccine_goodie:
                        m_actors.push_back(new VaccineGoodie(i*SPRITE_WIDTH,j*SPRITE_HEIGHT, this));
                        break;
                    case Level::gas_can_goodie:
                        m_actors.push_back(new gasCan(i*SPRITE_WIDTH,j*SPRITE_HEIGHT, this));
                        break;
                    case Level::landmine_goodie:
                        m_actors.push_back(new LandMine(i*SPRITE_WIDTH,j*SPRITE_HEIGHT, this));
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
    oss  << setw(6) << getScore();
    oss << "  Level: ";
    oss  << getLevel();
    oss << "  Lives: ";
    oss  << getLives();
    oss << "  Vaccines: ";
    oss  << m_player->getShots();
    oss << "  Flames: ";
    oss << m_player->getFlameCharges();
    oss << "  Mines: ";
    oss  << m_player->getMines();
    oss << "  Infected: ";
    oss  << m_player->infectionTotal();
    string s = oss.str();
    setGameStatText(s);
    m_player->doSomething();
    list<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        Actor* act = *it;
        act->doSomething();

        it++;
    }
    if(m_player->isComplete())
        return GWSTATUS_FINISHED_LEVEL;
    
    list<Actor*>::iterator ix = m_actors.begin();
    while(ix != m_actors.end()){
        Actor* act = *ix;
        if(!act->isAlive()){ // is not alive
            delete *ix;
            ix= m_actors.erase(ix);
        }
        else
            ++ix;
    }
    if(!m_player->isAlive()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    return GWSTATUS_CONTINUE_GAME; // fox this
}

void StudentWorld::cleanUp() // make this int a destruxtor
{
    
    delete m_player;
    m_player = nullptr;
    list<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        delete *it;
        it = m_actors.erase(it);
        //it++;
    }
}
