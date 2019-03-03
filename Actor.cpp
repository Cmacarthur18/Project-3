#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

StudentWorld* Actor:: getWorld1() const{
    return new_World;
}


void Penelope::doSomething(){
    int keyPress;
    if(Actor::isAlive()){
        
        if(getInfectStatus()== true){ // if she is infected
            if(infectionTotal() == 0)
                getWorld1()->playSound(SOUND_CITIZEN_INFECTED);
            increase();
            if(infectionTotal() >= 500){
                setDead(); // set dead when infection level is 500 or over
                return;
            }
        }
        if(getWorld1()->getKey(keyPress)){
                if(keyPress == KEY_PRESS_DOWN){
                    setDirection(down);
                    if(!getWorld1()->checkBlockPlayer(this)){
                        if( getWorld1()->blockMovement(getX(), getY()-4,this,getDirection())) // check location she wants to move
                            return;
                        moveTo(getX(), getY()-4);
                    }
                }
                else if(keyPress == KEY_PRESS_UP){
                    setDirection(up);
                    if(getWorld1()->blockMovement(getX(), getY()+4,this,getDirection())) // check location she wants to move
                        return;
                    moveTo(getX(), getY()+4);
                }
                else if(keyPress == KEY_PRESS_RIGHT){
                    setDirection(right);
                    if( getWorld1()->blockMovement(getX()+4, getY(),this,getDirection())) // check location she wants to move
                        return;
                    moveTo(getX()+4, getY());
                }
                else if(keyPress == KEY_PRESS_LEFT){
                    setDirection(left);
                    if( getWorld1()->blockMovement(getX()-4, getY(),this,getDirection())) // check location she wants to move
                        return;
                    moveTo(getX()-4, getY());
                }
                else if (keyPress == KEY_PRESS_SPACE&& getFlameCharges() > 0){
                    getWorld1()->playSound(SOUND_PLAYER_FIRE);
                    for(int i = 1; i <=3; i++){
                            // this adds one flame at a time, checkting to see if any of them would be blocked and then adds them if it can
                        if(getDirection() == right){
                    //cout << "called" << endl;
                            if(getWorld1()->checkAddObjectAt(getX()+i*SPRITE_WIDTH, getY()))
                        break;
                            getWorld1()->addFlames(getX()+i*SPRITE_WIDTH, getY(), getDirection());
                        }
                        if(getDirection() == left){
                            if(getWorld1()->checkAddObjectAt(getX()-i*SPRITE_WIDTH, getY()))
                                break;
                            getWorld1()->addFlames(getX()-i*SPRITE_WIDTH, getY(), getDirection());
                        }
                        if(getDirection() == down){
                            if(getWorld1()->checkAddObjectAt(getX(), getY()-i*SPRITE_WIDTH))
                                break;
                            getWorld1()->addFlames(getX(), getY()-i*SPRITE_WIDTH, getDirection());
                        }
                        if(getDirection() == up){
                            if(getWorld1()->checkAddObjectAt(getX(), getY()+i*SPRITE_WIDTH))
                                break;
                            getWorld1()->addFlames(getX(), getY()+i*SPRITE_WIDTH, getDirection());
                        }
                    }
                    decFlameCharge(); // decrease flames
                }
                else if(keyPress == KEY_PRESS_TAB && getMines()>0){
                    //cout << "here" << endl;
                    getWorld1()->addMine(getX(), getY());
                    decMines();
                }
                else if (keyPress == KEY_PRESS_ENTER && getShots() > 0 ){
                    decShots();
                    cure();
                }
            }//key press
            
        }// is alive here
        else
            getWorld1()->playSound(SOUND_PLAYER_DIE);
}

int Penelope::getFlameCharges(){
    return m_charges;
}
void Penelope::decFlameCharge(){
    m_charges--;
}
void Penelope:: addFLameCharge(){
    m_charges+=5;
}
void Penelope::decShots(){
    m_shots--;
}
void Penelope:: addShots(){
    m_shots++;
}

int Penelope::getShots(){
    return m_shots;
}
void Penelope::decMines(){
    m_mines--;
}
void Penelope:: addMines(){
    m_mines++;
}
int Penelope::getMines(){
    return m_mines;
}


///////////////////////////////////////////////////////////////////////////////////////////////////



void Exit::doSomething(){
     //check for citizens first and citizen saved soubnd
    if(getWorld1()->citizenOverlapExit(this)){ // if a player overlaps with an exit
        getWorld1()->increaseScore(500);
        return;
    }
    //overlaps wtih penelope
   if(getWorld1()->numCitizens() == 0){ // when no citizens remain, penelope can exit the level
        if(getWorld1()->objectOverlap(this,getWorld1()->returnPlayer())){
        getWorld1()->returnPlayer()->Setcompleted(true);
        getWorld1()->playSound(SOUND_LEVEL_FINISHED);
        return;
        }
    }
}

//ADD that theyt can be damaged by flames and vomit
void gasCan::doSomething(){
    if(!Actor::isAlive())
        return;
    
    if(getWorld1()->objectOverlap(this,getWorld1()->returnPlayer())){ // if it overlaps with a player, give penelope her flame charges and increase score
        getWorld1()->playSound(SOUND_GOT_GOODIE);
        Actor::setDead();
        getWorld1()->returnPlayer()->addFLameCharge();
        getWorld1()->increaseScore(50);
        return;
    }
}
void VaccineGoodie::doSomething(){
    if(!Actor::isAlive())
        return;
    if(getWorld1()->objectOverlap(this,getWorld1()->returnPlayer())){// if it overlaps with a player, give penelope her Vaccines charges and increase score
        getWorld1()->playSound(SOUND_GOT_GOODIE);
        Actor::setDead();
        getWorld1()->returnPlayer()->addShots();
        getWorld1()->increaseScore(50);
        return;
    }
}
void LandMine::doSomething(){
    if(!Actor::isAlive())
        return;
    if(getWorld1()->objectOverlap(this,getWorld1()->returnPlayer() )){// if it overlaps with a player, give penelope her fland mine and increase score
        getWorld1()->playSound(SOUND_GOT_GOODIE);
        Actor::setDead();
        getWorld1()->returnPlayer()->addMines();
        getWorld1()->increaseScore(50);
        return;
    }
}

void Mine::doSomething(){
    // deactive state??
    time--; // decrease the time so when it is less thant or equal to 0 it will explode
    if(time <= 0){ // change this after
        if(/*getWorld1()->objectOverlap(this, getWorld1()->returnPlayer())||*/
           getWorld1()->objectOverlapMine(this)){
            Actor::setDead();
            getWorld1()->playSound(SOUND_LANDMINE_EXPLODE);
            getWorld1()->addPit(getX(), getY());
            //Check to see if the falmes added in all of the directions would not be blocked
            if(!getWorld1()->checkAddObjectAt(getX(), getY()))
                getWorld1()->addFlames(getX(), getY(), up);
            
            if(!getWorld1()->checkAddObjectAt(getX()+SPRITE_WIDTH, getY()))
                getWorld1()->addFlames(getX()+SPRITE_WIDTH, getY(), up);
            
            if(!getWorld1()->checkAddObjectAt(getX()-SPRITE_WIDTH, getY()))
                getWorld1()->addFlames(getX()-SPRITE_WIDTH, getY(), up);
            
            if(!getWorld1()->checkAddObjectAt(getX(), getY()+SPRITE_HEIGHT))
                getWorld1()->addFlames(getX(), getY()+SPRITE_HEIGHT, up);
            
            if(!getWorld1()->checkAddObjectAt(getX(), getY()-SPRITE_HEIGHT))
                getWorld1()->addFlames(getX(), getY()-SPRITE_HEIGHT, up);
            
            if(!getWorld1()->checkAddObjectAt(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT))
                getWorld1()->addFlames(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT, up);
            
            if(!getWorld1()->checkAddObjectAt(getX()-SPRITE_WIDTH, getY()-SPRITE_HEIGHT))
                getWorld1()->addFlames(getX()-SPRITE_WIDTH, getY()-SPRITE_HEIGHT, up);
            
            if(!getWorld1()->checkAddObjectAt(getX()+SPRITE_WIDTH, getY()-SPRITE_HEIGHT))
                getWorld1()->addFlames(getX()+SPRITE_WIDTH, getY()-SPRITE_HEIGHT, up);
            
            if(!getWorld1()->checkAddObjectAt(getX()-SPRITE_WIDTH, getY()+SPRITE_HEIGHT))
                getWorld1()->addFlames(getX()-SPRITE_WIDTH, getY()+SPRITE_HEIGHT, up);
            return;
        
        
        }

    }
}

//ADD A PIT DO SOMETHING
    //CHECK TO SEE IF FALLEN IN


// Flame////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Flame::doSomething(){
    time++;
    
    getWorld1()->checkAddObjectAt(getX(), getY());//check if flame kills stuff
    if(getWorld1()->objectOverlap(this, getWorld1()->returnPlayer())){
        getWorld1()->returnPlayer()->setDead(); // set penelope dead
        return;
    }
    if(time >= 2){
        setDead(); // remove flame after 2 seconds
    }

}

void Vomit::doSomething(){
    if(!Actor::isAlive())
        return;
    time++;
    if(time >= 2)
        setDead(); // remove after its been on the ground for 2 ticks
    if(getWorld1()->objectOverlap(this, getWorld1()->returnPlayer())){
        getWorld1()->returnPlayer()->setInfection(); // if an object over laps with it, infect the actor
        return;
    }
    getWorld1()->objectOverlap1(this); // see if a citizen overlaps with the vomit
}

void Pit::doSomething(){
//    if(getWorld1()->objectOverlapMine(this)){
//        //cout << "crossed" << endl;
//        getWorld1()->returnPlayer()->setDead(); // change this to lose life;
//        return;
//    }
    getWorld1()->objectOverlapMine(this); // check to see if any player overlaps with the mine

}

bool Zombie::computeMove(){ // do more tmrw
    if(getDirection() == up && !getWorld1()->blockMovement(getX(), getY()+1,this,getDirection())){ // check move up
        moveTo(getX(), getY()+1);

        return true;
    }
    if(getDirection() == down && !getWorld1()->blockMovement(getX(), getY()-1,this,getDirection())){ // move doewn
        moveTo(getX(), getY()-1);

        return true;
    }
   
    if(getDirection() == left  &&!getWorld1()->blockMovement(getX()-1, getY(),this,getDirection())){ // move let
        moveTo(getX()-1, getY());

        return true;
    }
    if(getDirection() == right && !getWorld1()->blockMovement(getX()+1, getY(),this,getDirection())){ // move right
        moveTo(getX()+1, getY());

        return true;
    }
    else
        return false;
        //cout << "blocked" << endl;
}
bool Zombie::computevomit(){ // will need to make this for a player as well, maybe is damaged by vomit
    double vX;
    double vY;
    if(getDirection() == right && getWorld1()->blockMovement(getX()+1, getY(),this,getDirection())){ //if direction is right and not blocked
        vX = getX()+SPRITE_WIDTH;
        vY = getY();
        if(getWorld1()->distance(vX, vY, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) <= 100
           || getWorld1()->distance1(vX, vY)){ // checks to see if any actor that can be infected is with in 10 pixels
            getWorld1()->addVomit(vX, vY); // add vomit
            getWorld1()->playSound(SOUND_ZOMBIE_VOMIT);
            return true;
        }
    }
    if(getDirection() == left && getWorld1()->blockMovement(getX()-1, getY(),this,getDirection())){ // if some one is at the left of the zombie
        vX = getX()-SPRITE_WIDTH;
        vY = getY();
        if(getWorld1()->distance(vX, vY, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) <= 100
           || getWorld1()->distance1(vX, vY)){
            getWorld1()->addVomit(vX, vY);
            getWorld1()->playSound(SOUND_ZOMBIE_VOMIT);
            return true;
        }
    }
   
    if(getDirection() == up && getWorld1()->blockMovement(getX(), getY()+1,this,getDirection())){ // if anyone is to the right of the zombie
        vX = getX();
        vY = getY()+SPRITE_HEIGHT;
        if(getWorld1()->distance(vX, vY, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) <= 100
           || getWorld1()->distance1(vX, vY)){
            getWorld1()->addVomit(vX, vY);
            getWorld1()->playSound(SOUND_ZOMBIE_VOMIT);
            return true;
        }
    }
    if(getDirection() == down && getWorld1()->blockMovement(getX(), getY()-1,this,getDirection())){ // if anyone is below the zombie
        vX = getX();
        vY = getY()-SPRITE_HEIGHT;
        if(getWorld1()->distance(vX, vY, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) <= 100
           || getWorld1()->distance1(vX, vY) ) {
            getWorld1()->addVomit(vX, vY);
            getWorld1()->playSound(SOUND_ZOMBIE_VOMIT);
            return true;
        }
    }
    return false;
}

void Zombie::update(){ //new movement plan and direction
    int newDirection = rand()%4 +1;
    //m_plan = rand()%8+3;
    switch (newDirection) { // asign the direction for zombie to move randomnly
        case 1:
            setDirection(up);
            break;
        case 2:
            setDirection(down);
            break;
        case 3:
            setDirection(left);
            break;
        case 4:
            setDirection(right);
            break;
    }
    return;
}



void DumbZombie::doSomething(){
    m_ticks++;
    int chance = rand()%3;

    if(isAlive()){
        if( m_ticks %2 ==0){
            if(m_vomitChance == chance){
                if(computevomit()){ // if it is the 1/3 chance for the zombie to vomit
                    return;
                }
            }
            if(m_plan <= 0){
                m_plan = rand()%8+3; // gove new movement plan when it runs out
                update();
                return;
            }
            else{
                if(computeMove()){ // compute the movement
                    m_plan--;
                    return;
                }
                else{
                    update();
                    //m_plan = rand()%8+3;
                    return;
                }
            }
        }
    }
    else{
        getWorld1()->playSound(SOUND_ZOMBIE_DIE);
        getWorld1()->increaseScore(1000);
        int chance1 = rand()%10;
        if(chance1 == m_vaxChance){ // check ther vaxcine
            if(getDirection() == up){
                if(!getWorld1()->objectOverlap(getX(), getY()+16)) // this all checks to see if a zombie can throw a vaccine when it dies
                getWorld1()->addVax(getX(), getY()+16);
            }
            if(getDirection() == down){
                if(!getWorld1()->objectOverlap(getX(), getY()-16))
                    getWorld1()->addVax(getX(), getY()-16);
            }
            if(getDirection() == right){
                if(!getWorld1()->objectOverlap(getX()+16, getY()))
                    getWorld1()->addVax(getX()+16, getY());
            }
            if(getDirection() == left){
                if(!getWorld1()->objectOverlap(getX()-16, getY()))
                    getWorld1()->addVax(getX()-16, getY());
            }
            return;
        }
    }
}

void SmartZombie::doSomething(){
    m_ticks++;
    //cout << "DoSomething" << endl;
    
    int chance = rand()%3;
    if(!isAlive()){
        getWorld1()->increaseScore(2000); // when one dies, increase the score
        getWorld1()->playSound(SOUND_ZOMBIE_DIE);
    }
    if(isAlive()){
        if(m_ticks%2==0){
            if(m_vomitChance == chance){
                if(computevomit()){ // cumpute and add vomit if appropriate
                    getWorld1()->playSound(SOUND_ZOMBIE_VOMIT);
                    return;
                }
            }
            if(m_plan <= 0){
                m_plan = rand()%8+3;
                double X,Y;
                int selectDistance = getWorld1()->findClosest(this,X,Y); // find the closest actor to attack
                if(selectDistance <= 6400){
                    //SAME COLUMN//////////
                    if(getWorld1()->checkColumn(getX())){
                        if(getWorld1()->distance(getX(),getY()+1,X, Y) < selectDistance){
                                setDirection(up);
                                return;
                                }
                        setDirection(down);
                        return;
                    }
                   //// SAME ROW////////////
                    if(getWorld1()->checkRow(getY())){
                        if(getWorld1()->distance(getX()+1, getY(), X, Y) < selectDistance){
                            setDirection(right);
                            return;
                        }
                        setDirection(left);
                        return;
                    }
                    int r = rand()%+2;
                    if(getWorld1()->distance(getX(), getY()+1, X, Y) < selectDistance){
                        //UP//////////////////////////////////////////
                        if(getWorld1()->distance(getX()+1, getY()+1, X, Y) < getWorld1()->distance(getX()-1, getY()+1, X, Y)){
                            if(r == 0)
                                setDirection(up); // randomly chose to move right or up if it gets the zombie closer
                            else
                                setDirection(right);
                            return;
                            
                        }else{
                            if(r == 0)
                                setDirection(up); // randomly chose up or left if it gets it closer
                            else
                                setDirection(left);
                            return;
                        }
                    }
                    if(getWorld1()->distance(getX(), getY()-1, X, Y) < selectDistance){
                        //DOWN//////////////////////////////////////////
                        if(getWorld1()->distance(getX()+1, getY()-1, X, Y) < getWorld1()->distance(getX()-1, getY()+1, X, Y)){
                            if(r == 0)
                                setDirection(down); // randomly chose down or right
                            else
                                setDirection(right);
                            return;
                        }else{
                            if(r == 0)
                                setDirection(down); // randomly cchose down or ledft
                            else
                                setDirection(left);
                            return;
                        }
                    }
                    
                    update(); // update movement plan
                    return;
                }
               else{
                    update();
                    return;
                }
            }
            else{
                if(computeMove()){ // compute move if there is no player or penelope to follow
                    m_plan--;
                    return;
                }
                else{
                    update();
                    return;
                }
            }

        }
    }

}

void Citizen::doSomething(){
    m_time++;
    if(isAlive()){
        if(getInfectStatus()== true){ //if it is infected start increasing infection
            if(infectionTotal() == 0) // only play infectted sounf when it is first infected
                getWorld1()->playSound(SOUND_CITIZEN_INFECTED);
            increase();
            if(infectionTotal() >= 500){
                setDead();
                int r = 1+rand()%10;
                if( r < 8) // randomly to chose add a dumb or smart zombie
                    getWorld1()->addDumbZombie(getX(), getY());
                else
                    getWorld1()->addSmartZombie(getX(), getY());
                getWorld1()->playSound(SOUND_ZOMBIE_BORN);
                return;
            }
        }
    // change it so that the right sound is played when a citizen is saved or the correct points show
    }
    if(!isAlive() && !isSaved()){
        getWorld1()->increaseScore(-1000); // decrease citizens and score if it dies when it hasnt been saved yet
        getWorld1()->decreseCitizens();
    }
    if(m_time %2 ==0){
        checkMove();
    }
}

void Citizen::checkMove(){
    int dist_p = getWorld1()->distance(getX(), getY(), getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY());
    int dist_z = getWorld1()->closestZombie(getX(), getY());
    if( dist_p < dist_z && dist_p <= 6400){
        if(getX() == getWorld1()->returnPlayer()->getX()){ // same column
            if(getWorld1()->distance(getX(), getY()+2, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) < dist_p){
                setDirection(up);
                if(!getWorld1()->blockMovement(getX(), getY()+2,this,getDirection())){
                    setDirection(up);
                    moveTo(getX(), getY()+2);
                    return;
                }
            }else{
                setDirection(down);
                if(!getWorld1()->blockMovement(getX(), getY()-2,this,getDirection())){
                    setDirection(down);
                    moveTo(getX(), getY()-2);
                    return;
                }
            }
        }
        if(getY() == getWorld1()->returnPlayer()->getY()){ // same row
            if(getWorld1()->distance(getX()+2, getY(), getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) < dist_p){
                setDirection(right);
                if(!getWorld1()->blockMovement(getX()+2, getY(),this,getDirection())){
                    setDirection(right);
                    moveTo(getX()+2, getY());
                    return;
                }
            }else{
                //cout << "here" <<endl;
                setDirection(left);
                if(!getWorld1()->blockMovement(getX()-2, getY(),this,getDirection())){
                    setDirection(left);
                    moveTo(getX()-2, getY());
                    return;
                }
            }
        }
        // UP/////////////////////////////////////////////////////
        int r = rand()%+2;
        if(getWorld1()->distance(getX(), getY()+2, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) < dist_p){ // if moving it the citizen up would get it closer to penelope
            //right
            if(getWorld1()->distance(getX()+2, getY()+2, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) < dist_p){ // ifmoving up/ right would get citzen closer
                if(r == 0){
                    setDirection(up);
                    if(!getWorld1()->blockMovement(getX(), getY()+2, this,getDirection())){ // check before it moves to see if it can
                        setDirection(up);
                        moveTo(getX(), getY()+2);
                    }
                    else{
                        setDirection(right);
                        if(!getWorld1()->blockMovement(getX()+2, getY(), this,getDirection())){
                            setDirection(right);
                            moveTo(getX()+2, getY());
                        }
                    }
                    return;
                }
                else{
                    setDirection(right);
                    if(!getWorld1()->blockMovement(getX()+2, getY(), this,getDirection())){
                        setDirection(right);
                        moveTo(getX()+2, getY());
                    }
                    else{
                        setDirection(up);
                        if(!getWorld1()->blockMovement(getX(), getY()+2, this,getDirection())){
                            setDirection(up);
                            moveTo(getX(), getY()+2);
                        }
                    }
                    return;
                }

            }
            //left
            if(getWorld1()->distance(getX()-2, getY()+2, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) < dist_p){ // this means that moving up/left would get it closer
                if(r == 0){
                    setDirection(left);
                    if(!getWorld1()->blockMovement(getX()-2, getY(), this,getDirection())){
                        setDirection(left);
                        moveTo(getX()-2, getY());
                    }
                    else{
                        setDirection(up);
                        if(!getWorld1()->blockMovement(getX(), getY()+2, this,getDirection())){
                            setDirection(up);
                            moveTo(getX(), getY()+2);
                        }
                    }
                    return;
                }
                else{
                    setDirection(up);
                    if(!getWorld1()->blockMovement(getX(), getY()+2, this,getDirection())){
                        setDirection(up);
                        moveTo(getX(), getY()+2);
                    }
                    else{
                        setDirection(left);
                        if(!getWorld1()->blockMovement(getX()-2, getY(), this,getDirection())){
                            setDirection(left);
                            moveTo(getX()-2, getY());
                        }
                    }
                    return;
                }
            }
        }
        ////DOWN///////////////////////////////////////////////
        if(getWorld1()->distance(getX(), getY()-2, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) < dist_p){ // this checks to see if moving down would get it closer to penelope

            if(getWorld1()->distance(getX()+2, getY()-2, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) < dist_p){ // down/ right would get it closer
                if(r == 0){
                    if(!getWorld1()->blockMovement(getX(), getY()-2, this,getDirection())){
                        setDirection(down);
                        moveTo(getX(), getY()-2);
                    }
                    else if(!getWorld1()->blockMovement(getX()+2, getY(), this,getDirection())){
                        setDirection(right);
                        moveTo(getX()+2, getY());
                    }
                }
                else{
                    if(!getWorld1()->blockMovement(getX()+2, getY(), this,getDirection())){
                        setDirection(right);
                        moveTo(getX()+2, getY());
                    }
                    else if(!getWorld1()->blockMovement(getX(), getY()-2, this,getDirection())){
                        setDirection(down);
                        moveTo(getX(), getY()-2);
                    }
                }
                return;
            }
            //left
            if(getWorld1()->distance(getX()-2, getY()-2, getWorld1()->returnPlayer()->getX(), getWorld1()->returnPlayer()->getY()) < dist_p){ // down left would get it closer
                if(r == 0){
                    if(!getWorld1()->blockMovement(getX(), getY()-2, this,getDirection())){
                        setDirection(down);
                        moveTo(getX(), getY()-2);
                    }
                    else if(!getWorld1()->blockMovement(getX()-2, getY(), this,getDirection())){
                        setDirection(left);
                        moveTo(getX()-2, getY());
                    }
                }
                else{
                    if(!getWorld1()->blockMovement(getX()-2, getY(), this,getDirection())){
                        setDirection(left);
                        moveTo(getX()-2, getY());
                    }
                    else if(!getWorld1()->blockMovement(getX(), getY()-2, this,getDirection())){
                        setDirection(down);
                        moveTo(getX(), getY()-2);
                    }
                }
                return;
            }
        }

    }
    if( dist_z <= 6400 ){
        double dist_zUp = -1; // make all the distances t0 -1 meaning they have not been checked
        double dist_zDown = -1;
        double dist_zRight= -1;
        double dist_zLeft =-1;
        if(!getWorld1()->blockMovement(getX(), getY()+2, this, up)){
             dist_zUp = getWorld1()->closestZombie(getX(), getY()+2);
        }
        if(!getWorld1()->blockMovement(getX(), getY()-2, this, down)){
             dist_zDown = getWorld1()->closestZombie(getX(), getY()-2);
        }
        if(!getWorld1()->blockMovement(getX()+2, getY(), this, right)){
             dist_zRight = getWorld1()->closestZombie(getX()+2, getY());
        }
        if(!getWorld1()->blockMovement(getX()-2, getY(), this, left)){
             dist_zLeft = getWorld1()->closestZombie(getX()-2, getY());
        }
        if( dist_zUp == -1 && dist_zDown == -1 && dist_zRight == -1 && dist_zLeft == -1)
            return;
        // add only the distances which are valid meaning greater than 1
        vector<double> distances;
        if(dist_zUp > -1)
            distances.push_back(dist_zUp);
        if(dist_zDown > -1)
            distances.push_back(dist_zDown);
        if(dist_zLeft > -1)
            distances.push_back(dist_zLeft);
        if(dist_zRight > -1)
            distances.push_back(dist_zRight);
        double distmin = distances[0];
        for(int i = 1; i < distances.size(); i++){
            if(distances[i] > distmin){
                distmin = distances[i];
            }
        }
        if(distmin > dist_z){
            // chose which direction takes it further away
            if(distmin == dist_zUp){
                setDirection(up);
                moveTo(getX(), getY()+2);
                return;
            }
            if(distmin == dist_zDown){
                setDirection(down);
                moveTo(getX(), getY()-2);
                return;
            }
            if(distmin == dist_zLeft){
                setDirection(left);
                moveTo(getX()-2, getY());
                return;
            }
            if(distmin == dist_zRight){
                setDirection(right);
                moveTo(getX()+2, getY());
                return;
            }
        }
        
        

        
        
    }
}

