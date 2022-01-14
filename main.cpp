#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;
#define WINDOW_X 800
#define WINDOW_Y 600
class MovingObject : public Sprite
{
    vector<Sprite*> parts;   //a vector containing elements of type sprite so that the elements such as coins, bombs,
    double vx, vy;           //magnets, can be stored together
    double ax, ay;
    bool paused;
    void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true)   // function to initialize the moving objects such as
    {
        // coins, bombs, magnets, etc.
        vx=argvx;
        vy=argvy;
        ax=argax;
        ay=argay;
        paused=argpaused;
    }
public:
    MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)   //constructor which initializes objects directly while
        : Sprite()                                                                              //creation rather than using initMo exclusively
    {
        initMO(argvx, argvy, argax, argay, argpaused);
    }
    MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite()  //constructor which converts speed
    {
        // into x and y components and then
        double angle_rad = angle_deg*PI/180.0;                                                                        // initializes the object
        double argvx = speed*cos(angle_rad);
        double argvy = -speed*sin(angle_rad);
        initMO(argvx, argvy, argax, argay, argpaused);
    }
    void set_vx(double argvx)    // sets velocity vx to argvx
    {
        vx = argvx;
    }
    void set_vy(double argvy)   // sets velocity vy to argvy
    {
        vy = argvy;
    }
    void set_ax(double argax)   // sets acceleration ax to argax
    {
        ax = argax;
    }
    void set_ay(double argay)   // sets acceleration ay to argay
    {
        ay = argay;
    }
    double getXPos();           // function to return x position of object
    double getYPos();           // function to return y position of object
    void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);
    // resets position of object to initial position
    void pause()            // function to pause an object in motion
    {
        paused = true;
    }
    void unpause()          // function to unpause an object which is paused
    {
        paused = false;
    }
    bool isPaused()          // returns whether an object is paused or unpaused
    {
        return paused;
    }

    void addPart(Sprite* p)   // adds an object of type Sprite to vector parts at the end of the vector
    {
        parts.push_back(p);
    }
    void nextStep(double t);  // function indicating the next step to be taken after a specific operation is performed
    void getAttachedTo(MovingObject *m);   // function which attaches object to the lasso
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t)     // function which moves all objects with velocity vx and vy and acceleration ax and ay
{
    if(paused)
    {
        return;
    }


    for(size_t i=0; i<parts.size(); i++)
    {
        parts[i]->move(vx*t, vy*t);
    }
    vx += ax*t;
    vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos()        // return x position of an object
{
    return (parts.size() > 0) ? parts[0]->getX() : -1;
}//End MovingObject::getXPos()

double MovingObject::getYPos()        // return y position of an object
{
    return (parts.size() > 0) ? parts[0]->getY() : -1;
}// End MovingObject::getYPos()

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta)
{
    // resets an object to its initial position at time of creation
    for(size_t i=0; i<parts.size(); i++)
    {
        parts[i]->moveTo(argx, argy);
    }
    double angle_rad = angle_deg*PI/180.0;
    double argvx = speed*cos(angle_rad);
    double argvy = -speed*sin(angle_rad);
    vx = argvx;
    vy = argvy;
    ax = argax;
    ay = argay;
    paused = argpaused;

} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m)   // attaches an object to the lasso
{
    double xpos = m->getXPos();
    double ypos = m->getYPos();
    for(size_t i=0; i<parts.size(); i++)
    {
        parts[i]->moveTo(xpos, ypos);
    }
    initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}// End MovingObject::getAttachedTo(MovingObject *m)

//coin.h
#ifndef __COIN_H__
#define __COIN_H__

class Coin : public MovingObject  // class coin for creation of a coin
{
    double coin_start_x;
    double coin_start_y;
    double release_speed;
    double release_angle_deg;
    double coin_ax;
    double coin_ay;

    // Moving parts
    Circle coin_circle;

public:


    Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta,int spec) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta)
    {
        // a constructor to create an object of type Coin without initializing
        release_speed = speed;
        release_angle_deg = angle_deg;
        coin_ax = argax;
        coin_ay = argay;
    }
    Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, bool special=false) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta)
    {
        // a constructor to create an object of type Coin
        release_speed = speed;
        release_angle_deg = angle_deg;
        coin_ax = argax;
        coin_ay = argay;
        initCoin(special);
    }

    void initCoin(bool special);
    void resetCoin();

}; // End class Coin

#endif
int lifes=5;

//class Bomb
class Bomb :  public MovingObject       // class Bomb to create a Bomb object
{
public:
    double bomb_start_x;
    double bomb_start_y;
    double acc_y;
    double acc_x;

    Rectangle bomb_rec;

public:
    Bomb(double speed, double accy, bool def): MovingObject(speed,45,0,acc_y,true)      // constructor to create a bomb without initializing it
    {
        acc_x=0;
        acc_y=accy;
    }
    Bomb(double speed, double accy): MovingObject(speed,45,0,acc_y,true)                //// constructor to create a bomb with initializing it
    {
        acc_x=0;
        acc_y=accy;
        initBomb();
    }
    void initBomb();    //initializes a bomb
    void resetBomb();   //resets a bomb

};//End of Class Bomb

//class Magnet
class Magnet :  public MovingObject   //class Magnet to create an object magnet
{
public:
    double magnet_start_x;
    double magnet_start_y;
    double acc_y;
    double acc_x;

    Rectangle magnet_rec;

public:

    Magnet(double speed, double accy): MovingObject(speed,150,0,acc_y,true)  // constructor without initializing on screen
    {
        acc_x=0;
        acc_y=accy;
    }
    Magnet(double speed, double accy, bool repel): MovingObject(speed,0,0,acc_y,true)  // constructor without initialization
    {
        acc_x=0;
        acc_y=accy;
        initMagnet(repel);
    }
    void initMagnet(bool repel);   // initializes a bomb
    void resetMagnet();            // resets a bomb

};//End of Class Magnet

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1070
//#define WINDOW_Y 800
/*#define WINDOW_X 800
#define WINDOW_Y 600*/

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 75 // #define COIN_ANGLE_DEG 90 Abhijeet

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

#define BOMB_START_Y 50
#define BOMB_SPEED 100
#define BOMB_G 5
#define BOMB_GAP 7

#define MAGNET_START_Y 50
#define MAGNET_SPEED 100
#define MAGNET_G 5
bool special=false;
class Lasso : public MovingObject  //class Lasso to create a lasso
{
    double lasso_start_x;
    double lasso_start_y;
    double release_speed;
    double release_angle_deg;
    double lasso_ax;
    double lasso_ay;

    // Moving parts
    Circle lasso_circle;
    Circle lasso_loop;

    // Non-moving parts
    Line lasso_line;
    Line lasso_band;

    // State info
    bool lasso_looped;
    Coin *the_coin;
    Bomb *the_bomb;
    Magnet *the_magnet;
    int num_coins;

    void initLasso();   //function to initialize lasso
public:
    Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta)
    {
        // constructor to initialize lasso according to given arguments
        release_speed = speed;
        release_angle_deg = angle_deg;
        lasso_ax = argax;
        lasso_ay = argay;
        initLasso();
    }

    void draw_lasso_band();        // draws lasso band
    void yank();                   // yanks the lasso
    void loopit();
    void addAngle(double angle_deg); // adds angle to lasso to change its angle
    void addSpeed(double speed);     // increases or decreases the speed of the lasso

    void nextStep(double t);         //function to call nextStep after initial step is completed
    void check_for_coin(Coin *coin);  // function which checks if coin is present inside lasso
    void check_for_bomb(Bomb *bomb);  // function which checks if bomb is present inside lasso
    void check_for_magnet(Magnet *magnet, bool flag);  // function which checks if magnet is present inside lasso
    int getNumCoins()               //returns number of coins captured by lasso till it is called
    {
        return num_coins;
    }
    int getLifes()       // returns number of lives remaining
    {
        return lifes;
    }
    void resetcoinpoint();//resets coin pointer

}; // End class Lasso

#endif

//coin.h

void Coin::initCoin(bool special)  // initializes coin
{
    coin_start_x = (PLAY_X_START+50)/2;
    coin_start_y = PLAY_Y_HEIGHT;
    coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
    if (special) coin_circle.setColor(COLOR("green"));
    else
        coin_circle.setColor(COLOR("yellow"));
    coin_circle.setFill(true);
    addPart(&coin_circle);
}  //End  Coin::initCoin(bool special)

void Coin::resetCoin()      // resets coin to initial position
{
    double coin_speed = COIN_SPEED;
    double coin_angle_deg = COIN_ANGLE_DEG;
    coin_ax = 0;
    coin_ay = COIN_G;
    bool paused = true, rtheta = true;
    reset_all(coin_start_x+rand()%WINDOW_X/2, coin_start_y-rand()%WINDOW_Y/2, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}//End  Coin::resetCoin()

//bomb.h
void Bomb::initBomb()    // initializes bomb
{

    bomb_start_x=PLAY_X_START+100;
    bomb_start_y=BOMB_START_Y;
    bomb_rec.reset(bomb_start_x+100, bomb_start_y, 30,30);
    bomb_rec.setColor(COLOR("red"));
    bomb_rec.setFill(true);

    addPart(&bomb_rec);

} //End Bomb::initBomb()

void Bomb::resetBomb()     // resets bomb to original position
{
    double bomb_speed = BOMB_SPEED;
    acc_y = BOMB_G;
    bool paused = true, rtheta = true;
    reset_all(rand()%WINDOW_X, bomb_start_y, bomb_speed, 270, 0, acc_y, paused, rtheta);
}  // End Bomb::resetBomb()

//Magnet.h
void Magnet::initMagnet(bool repel)    // initializes magnet
{

    magnet_start_x=PLAY_X_START+350;
    magnet_start_y=MAGNET_START_Y;
    magnet_rec.reset(magnet_start_x, magnet_start_y, 30,30);
    if (repel) magnet_rec.setColor(COLOR(20,10,60));
    else
        magnet_rec.setColor(COLOR("blue"));
    magnet_rec.setFill(true);

    addPart(&magnet_rec);

} // End Magnet::initMagnet()

void Magnet::resetMagnet()   // resets magnet to original position
{
    double magnet_speed = MAGNET_SPEED;
    acc_y = MAGNET_G;
    bool paused = true, rtheta = true;
    reset_all(rand()%WINDOW_X, magnet_start_y, magnet_speed, 270, 0, acc_y, paused, rtheta);
} //End Magnet::resetMagnet()

//lasso.cpp

void Lasso::draw_lasso_band()    // draws lasso band on screen
{
    double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
    double arad = release_angle_deg*PI/180.0;
    double xlen = len*cos(arad);
    double ylen = len*sin(arad);
    lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
    lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso()      // initializes lasso
{
    lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
    lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
    lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
    lasso_circle.setColor(COLOR("red"));
    lasso_circle.setFill(true);
    lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
    lasso_loop.setColor(COLOR("blue"));
    lasso_loop.setFill(true);
    addPart(&lasso_circle);
    addPart(&lasso_loop);
    lasso_looped = false;
    the_coin = NULL;
    the_bomb=NULL;
    the_magnet=NULL;
    num_coins = 0;

    lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
    lasso_line.setColor(COLOR("red"));

    lasso_band.setColor(COLOR("blue"));
    draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank()   // yanks lasso to pivot
{
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
    lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
    lasso_loop.setFill(true);
    lasso_looped = false;
    if(the_coin != NULL)
    {
        if (special) lifes++;
        num_coins++;
        the_coin->resetCoin();
        the_coin=NULL;
    }
    if (the_bomb!=NULL)
    {
        lifes--;
        the_bomb->resetBomb();
        the_bomb=NULL;

    }

} // End Lasso::yank()

void Lasso::loopit()   // loops lasso to capture an object
{
    if(lasso_looped)
    {
        return;    // Already looped
    }
    lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
    lasso_loop.setFill(false);
    lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg)    // increases angle of lasso
{
    release_angle_deg += angle_deg;
    if(release_angle_deg < MIN_RELEASE_ANGLE_DEG)
    {
        release_angle_deg = MIN_RELEASE_ANGLE_DEG;
    }
    if(release_angle_deg > MAX_RELEASE_ANGLE_DEG)
    {
        release_angle_deg = MAX_RELEASE_ANGLE_DEG;
    }
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed)   // increases speed of lasso
{
    release_speed += speed;
    if(release_speed < MIN_RELEASE_SPEED)
    {
        release_speed = MIN_RELEASE_SPEED;
    }
    if(release_speed > MAX_RELEASE_SPEED)
    {
        release_speed = MAX_RELEASE_SPEED;
    }
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime)   // yanks the lasso when it reaches x axis
{
    draw_lasso_band();
    MovingObject::nextStep(stepTime);
    if(getYPos() > PLAY_Y_HEIGHT)
    {
        yank();
    }
    lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr)   // checks if coin is looped inside lasso
{
    double lasso_x = getXPos();
    double lasso_y = getYPos();
    double coin_x = coinPtr->getXPos();
    double coin_y = coinPtr->getYPos();
    double xdiff = (lasso_x - coin_x);
    double ydiff = (lasso_y - coin_y);
    double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
    if(distance <= LASSO_RADIUS)
    {
        the_coin = coinPtr;
        the_coin->getAttachedTo(this);
    }
} // End Lasso::check_for_coin()
void Lasso::check_for_bomb(Bomb *bombPtr)   // checks if bomb is looped inside lasso
{
    double lasso_x = getXPos();
    double lasso_y = getYPos();
    double bomb_x = bombPtr->getXPos();
    double bomb_y = bombPtr->getYPos();
    double xdiff = (lasso_x - bomb_x);
    double ydiff = (lasso_y - bomb_y);
    double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
    if(distance <= LASSO_RADIUS-15*sqrt(2))
    {
        the_bomb = bombPtr;
        the_bomb->getAttachedTo(this);
    }
} //End Lasso:: check_for_bomb

void Lasso::check_for_magnet(Magnet *magnetPtr, bool flag )  // checks if magnet is looped inside lasso
{
    double lasso_x = getXPos();
    double lasso_y = getYPos();
    double magnet_x = magnetPtr->getXPos();
    double magnet_y = magnetPtr->getYPos();
    double xdiff = (lasso_x - magnet_x);
    double ydiff = (lasso_y - magnet_y);
    double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
    if(distance <= 2*LASSO_RADIUS)
    {
        the_magnet = magnetPtr;
        the_magnet->getAttachedTo(this);
        if (flag)
        {
        lifes++;
        }
        else
        {
            num_coins=num_coins+5;
        }
    }
} // End Lasso check_for_magnet

void Lasso::resetcoinpoint()    // resets coin pointer to NULL
{
    the_coin=NULL;
} // End Lasso::resetcoinpoint()


main_program
{

    initCanvas("Lasso", WINDOW_X, WINDOW_Y);    // initializes canvas "Lasso" of size WINDOW_X   x  WINDOW_y
    Rectangle r0(WINDOW_X/2, 50,120,30);
    Text t0(WINDOW_X/2, 50,"KEY CONTROLS");     // displays "KEY CONTROLS" on Screen

    Rectangle r1(WINDOW_X/5+40, 100, 30,20);
    Text t1(WINDOW_X/5, 100,"Press");                  // displays "Press t to start lasso from pivot" on screen
    Text t11(WINDOW_X/5+40, 100, "t");
    Text t12(WINDOW_X/5+170, 100, "to start the lasso from the pivot");

    Rectangle r2(WINDOW_X/5+40, 150, 30,20);
    Text t2(WINDOW_X/5, 150,"Press");                     // displays "Press l to loop coin to lasso. Beware of BOMBS" on Screen
    Text t21(WINDOW_X/5+40, 150, "l");
    Text t22(WINDOW_X/5+220, 150, "to loop the coin to the lasso.  Beware of BOMBS");

    Rectangle r3(WINDOW_X/5+40, 200, 30,20);
    Text t3(WINDOW_X/5, 200,"Press");
    Text t31(WINDOW_X/5+40, 200, "y");                    // displays "Press y to yank the lasso" on screen
    Text t32(WINDOW_X/5+120, 200, "to yank the lasso");



    Rectangle r4(WINDOW_X/5+40, 250, 30,20);
    Text t4(WINDOW_X/5, 250,"Press");                     // displays "Press q to Quit" on screen
    Text t41(WINDOW_X/5+40, 250, "q");
    Text t42(WINDOW_X/5+90, 250, "to Quit");

    Circle c1(WINDOW_X/5, 300, 10);
    c1.setColor(COLOR("yellow"));                    // Displays yellow coin which gives 1 point
    c1.setFill(true);
    Text t5(WINDOW_X/5+270, 300, "Represents the coin you have to yank. Each coin gives 1 Point each");


    Circle c2(WINDOW_X/5, 350, 10);
    c2.setColor(COLOR("green"));                  // displays green coin which is necessary to collect else a life is lost
    c2.setFill(true);
    Text t9(WINDOW_X/5+260, 350, "Coin needs to be necessarily collected. Otherwise a life will be lost!");

    Rectangle r5(WINDOW_X/5, 400, 30,30);
    r5.setColor(COLOR("red"));                     // displays bomb which on collecting costs a life as well as 5 points
    r5.setFill(true);
    Text t6(WINDOW_X/5+260, 400, "Represents BOMB. Yanking a bomb will lead to reduction in 1 life!!");

    Rectangle r6(WINDOW_X/5, 440, 30,30);
    r6.setColor(COLOR("blue"));                     // displays bomb which on collecting costs a life as well as 5 points
    r6.setFill(true);
    Text t8(WINDOW_X/5+205, 440, "Represents attracting magnet will attract all coins");


    Rectangle r7(WINDOW_X/5, 480, 30,30);
    r7.setColor((COLOR(20,10,60)));                     // displays bomb which on collecting costs a life as well as 5 points
    r7.setFill(true);
    Text t33(WINDOW_X/5+205, 480, "Represents LIFE BOX !!! A life will increase");

    Text inst(WINDOW_X/2, 550, "CLICK FOR FURTHER INSTRUCTIONS ");
    getClick();
    inst.hide();
    r7.hide();
    t33.hide();
    r6.hide();
    t8.hide();
    t9.hide();
    t9.hide();
    c2.hide();
    r0.hide();
    r1.hide();
    r2.hide();
    r3.hide();
    r4.hide();
    r5.hide();
    t1.hide();
    t11.hide();
    t12.hide();                         // Hides all the instructions displayed initially on the lasso screen
    t2.hide();                          // Using closeCanvas() and initializing new canvas displayed blank canvas so had to hide all objects
    t3.hide();
    t4.hide();
    t5.hide();
    t6.hide();
    t21.hide();
    t22.hide();
    t31.hide();
    t32.hide();
    t41.hide();
    t42.hide();
    c1.hide();
    Text o1(WINDOW_X/2, 150, "Press i to increase release speed of lasso");
    Text o2(WINDOW_X/2, 200, "Press r to decrease release speed of lasso");
    Text o3(WINDOW_X/2, 250, "Press a to increase releasse angle of lasso");
    Text o4(WINDOW_X/2, 300, "Press d to decrease release angle of lasso");
    Text o5(WINDOW_X/2, 350, "You have to complete the TARGET displayed on top to complete a level");
    Text o6(WINDOW_X/2, 400, "Bombs, special coins and magnets will be added in each level gradually");

    Text t7(WINDOW_X/2, 500, "CLICK TO START THE GAME ");
    getClick();
    o5.hide();
    o6.hide();
    t0.hide();
    o1.hide();
    o2.hide();
    o3.hide();
    o4.hide();
    t7.hide();

    int stepCount = 0;          // initialize stepCount to 0
    float stepTime = STEP_TIME;   // initialize stepTime to STEP_TIME
    float runTime = 180;  // initialize runTime to 180 units
    float currTime = 0;   // initialize currTime to 0
    int level=1;          // initialize level to 1


    // Draw lasso at start position
    double release_speed = INIT_RELEASE_SPEED; // initialize release speed in m/s
    double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // initialize release angle in degrees
    double lasso_ax = 0;      // initialize x acceleration of lasso to 0
    double lasso_ay = LASSO_G;  // initialize y acceleration of lasso to LASSO_G
    bool paused = true;    // lasso is initially paused
    bool rtheta = true;    // rtheta is initially true
    Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);  //creating object lasso

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR("blue"));                           //creating x and y axes
    Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
    b2.setColor(COLOR("red"));

    string msg("Cmd: _");               // a string which displays what command is being typed
    Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);

    char coinScoreStr[256];     // a string which stores the score and displays it on the screen
    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

    char lifesleftStr[256];        // a string which stores lifes left and displays it on the screen
    sprintf(lifesleftStr, "LIVES LEFT: %d", lasso.getLifes());
    Text life(PLAY_X_START+350,PLAY_Y_HEIGHT+20,lifesleftStr);

    char levelstr[256];           // a string which stores the levels and displays it on the screen
    sprintf(levelstr, "LEVEL: %d", level);
    Text levelstring(WINDOW_X-50, 50, levelstr);

    char timestr[256];            // a string which stores the time left and displays it on the screen
    sprintf(timestr, "TIME LEFT :  %d" ,  int(runTime-currTime));
    Text timestring(WINDOW_X-300, 50, levelstr);

    char targetstr[256];        // a string which stores target to complete a level
    sprintf(targetstr, "TARGET :  %d", 5*level);
    Text target(PLAY_X_START+120,50,targetstr);


    paused = true;
    rtheta = true;
    double coin_speed = COIN_SPEED;
    double coin_angle_deg = COIN_ANGLE_DEG;
    double coin_ax = 0;
    double coin_ay = COIN_G;                                                   // creating coin object
    Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta, false);  // normal coin


    Coin coin1(coin_speed-40, coin_angle_deg-20, coin_ax, coin_ay, paused, rtheta, false); // normal coin
    Coin coin2(coin_speed-60, coin_angle_deg+60, coin_ax, coin_ay, paused, rtheta, false);  // normal coin
    Coin coin3(coin_speed+40, coin_angle_deg-40, coin_ax, coin_ay, paused, rtheta, false);  // normal coin
    Coin spec1(coin_speed+60, coin_angle_deg-20, coin_ax, coin_ay, paused, rtheta,1);    //special coin

    // After every COIN_GAP sec, make the coin jump
    double last_coin_jump_end = 8;

    Bomb bomb(BOMB_SPEED,BOMB_G, true);   // creating bomb object

    Magnet attract(MAGNET_SPEED, MAGNET_G);   // creating magnet objects
    Magnet repel(MAGNET_SPEED, MAGNET_G);

    // When t is pressed, throw lasso
    // If lasso within range, make coin stick
    // When y is pressed, yank lasso
    // When l is pressed, loop lasso
    // When q is pressed, quit
    // When  d is pressed,decrease angle of lasso
    // When a is pressed, increase angle of lasso
    // When r is pressed, reduce speed of lasso
    // When i is pressed, increased speed of lasso
    do{

        currTime=0;
        for(;;)
        {
            if (level>1&& currTime==0) bomb.initBomb();  // if level is greater than one initialize bomb

            if (level>2 && currTime==0) spec1.initCoin(true);





            if (level>3 && int(currTime)==0)    // if level is greater than three initialize attracting magnet
            {
                attract.initMagnet(false);
            }

            if (level>4 && int(currTime)%290==0)    // if level is greater than four initialize repelling magnet
            {
                repel.initMagnet(true);
            }

            if((runTime > 0) && (currTime > runTime))  // end time slot when currTime become more than runTime
            {
                Text texta(500,300,"TIMES UP!!");
                lifes--;                               // display message of TIMES UP!!! and reduce a life
                runTime=300;
                if (lifes<0)
                    Text textb(500,500,"WELL PLAYED!! BETTER LUCK NEXT TIME :)");  // if life becomes less than 0 display well played
                wait(10);

                break;
            }
            if ((lasso.getNumCoins()>=5*level))          // move to next level if number of coins greater than 5*level          {
            {
                level++;
                lifes=5;

                Text Textb (300,300,"LEVEL COMPLETE!!");
                Text Textc (300,400,"Starting next level!!"); // display level complete and starting next level
                wait(10);

                runTime=300;    // set runTime to 300 units
                break;
            }
            XEvent e;
            bool pendingEv = checkEvent(e);
            if(pendingEv)
            {
                char c = charFromEvent(e);       // multitasking event to display commands on screen
                msg[msg.length()-1] = c;
                charPressed.setMessage(msg);
                switch(c)
                {
                case 't':
                    lasso.unpause();        // if t is pressed unpause lasso from pivot
                    break;
                case 'y':
                    lasso.yank();            // if y is pressed yank lasso
                    break;
                case 'l':
                    lasso.loopit();          // if l is pressed loop lasso and check for coin bomb and magnets
                    lasso.check_for_coin(&coin);
                    lasso.check_for_coin(&coin1);
                    lasso.check_for_coin(&coin2);
                    lasso.check_for_coin(&coin3);
                    lasso.check_for_bomb(&bomb);
                    lasso.check_for_coin(&spec1);


                    lasso.check_for_magnet(&attract, false);


                    lasso.check_for_magnet(&repel, true);

                    wait(STEP_TIME*5);
                    break;
                case 'd':             // if d is pressed reduce angle of lasso
                    if(!lasso.isPaused())
                    {
                        lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);
                    }
                    break;
                case 'a':         // if a is pressed increase angle of lasso
                    if(!lasso.isPaused())
                    {
                        lasso.addAngle(+RELEASE_ANGLE_STEP_DEG);
                    }
                    break;
                case 'r':             // if r is pressed reduce speed of lasso
                    if(!lasso.isPaused())
                    {
                        lasso.addSpeed(-RELEASE_SPEED_STEP);
                    }
                    break;
                case 'i':              // if i is pressed increase speed of lasso
                    if(!lasso.isPaused())
                    {
                        lasso.addSpeed(+RELEASE_SPEED_STEP);
                    }
                    break;
                case 'q':
                {
                    Text ending1(400,400, "GAME OVER!!!!");   // if q is pressed display GAME OVER!! and quit the game
                    wait (5);
                    exit(0);
                    break;
                }
                default:
                    break;
                }
            }
            if (lifes==-1) break;      // if life less than 0 GAME OVER
            lasso.nextStep(stepTime);

            coin.nextStep(stepTime);
            coin1.nextStep(stepTime);
            coin2.nextStep(stepTime);   // perform nextSteps of coins, bombs and magnets
            coin3.nextStep(stepTime);
            spec1.nextStep(stepTime);
            bomb.nextStep(stepTime);
            attract.nextStep(stepTime);
            repel.nextStep(stepTime);
            if(coin.isPaused())
            {
                if((currTime-last_coin_jump_end) >= COIN_GAP)
                {
                    coin.unpause();
                }
            }

            if(coin.getYPos() > PLAY_Y_HEIGHT)
            {
                coin.resetCoin();                                      // if coin reaches x axis reset coin
                last_coin_jump_end = currTime;
            }
            if(coin1.isPaused())
            {
                if(coin1.getYPos() <= PLAY_Y_HEIGHT) coin1.unpause();
            }

            if(coin1.getYPos() > PLAY_Y_HEIGHT)                         // if coin reaches x axis reset coin
            {
                coin1.resetCoin();
                last_coin_jump_end = currTime;
            }

            if(coin2.isPaused())
            {
                if(coin2.getYPos() <= PLAY_Y_HEIGHT) coin2.unpause();
            }

            if(coin2.getYPos() > PLAY_Y_HEIGHT)                         // if coin reaches x axis reset coin
            {
                coin2.resetCoin();
                last_coin_jump_end = currTime;
            }
            if(coin3.isPaused())
            {
                if(coin3.getYPos() <= PLAY_Y_HEIGHT) coin3.unpause();
            }

            if(coin3.getYPos() > PLAY_Y_HEIGHT)
            {
                coin3.resetCoin();                                        // if coin reaches x axis reset coin
                last_coin_jump_end = currTime;
            }
            if(spec1.isPaused())
            {
                if(spec1.getYPos() <= PLAY_Y_HEIGHT) spec1.unpause();
            }

            if(spec1.getYPos() > PLAY_Y_HEIGHT)                           // if coin reaches x axis reset coin and reduce a life!!
            {
                lifes--;
                spec1.resetCoin();
                last_coin_jump_end = currTime;
            }
            if(bomb.isPaused())
            {
                if(bomb.getYPos() <= PLAY_Y_HEIGHT) bomb.unpause();     // if bomb reaches x axis reset bomb
            }
            if(bomb.getYPos() > PLAY_Y_HEIGHT)
            {
                bomb.resetBomb();

            }

            if(attract.isPaused())
            {
                if(attract.getYPos() <= PLAY_Y_HEIGHT) attract.unpause();    // if magnet reaches x axis reset magnet
            }
            if(attract.getYPos() > PLAY_Y_HEIGHT)
            {
                attract.resetMagnet();

            }

            if(repel.isPaused())
            {
                if(repel.getYPos() <= PLAY_Y_HEIGHT) repel.unpause();     // if life box reaches x axis reset magnet
            }
            if(repel.getYPos() > PLAY_Y_HEIGHT)
            {
                repel.resetMagnet();

            }

            sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());    // displays coins collected
            coinScore.setMessage(coinScoreStr);

            sprintf(lifesleftStr, "LIVES LEFT: %d", lasso.getLifes());    // display lives left
            life.setMessage(lifesleftStr);

            sprintf(levelstr, "LEVEL: %d", level);                     // display LEVEL of game
            levelstring.setMessage(levelstr);

            sprintf(timestr, "TIME LEFT: %d", int(runTime-currTime) ); // display time left
            timestring.setMessage(timestr);

            sprintf(targetstr, "TARGET :  %d", 5*level ); // display target to achieve
            target.setMessage(targetstr);

            if (lifes<0) break;
            stepCount++;                           // if lifes is less than 0 break and GAME OVER!!
            currTime += stepTime;
            wait(stepTime);
            currTime++;

        } // End for(;;)

    }
    while (level>0&&lifes>=0);    // run loop while lifes is greater than equal to 0
    Text ending(400,400, "GAME OVER!!!!");
    Text ending2(400, 500, "Press q to quit ");    // display game over and press q to quit after all lifes have been exhausted
    wait(6);
} // End main_program
