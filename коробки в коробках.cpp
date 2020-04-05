#include "TXLib.h"

double Lerp (double x1, double x2, double t);

const int LENGTH = GetSystemMetrics (SM_CXSCREEN) - 5;
const int WIDTH  = GetSystemMetrics (SM_CYSCREEN) - 68;

const int MAX_NUMBER = 50;
const int MIN_NUMBER = 10;

struct MissPoint
{
    double x_,  y_;
    double vx_, vy_;
    double colorR_, colorG_, colorB_;

    public: MissPoint();

    virtual void Draw    () = 0;
    virtual void Physics () = 0;
};

struct Box : public MissPoint
{
    int number_;
    int x, y;

    MissPoint* MyLittleBox_ [MAX_NUMBER];

    public: Box ();

    void Fill     ();
    void Mechanic ();
    void Control  ();
    void Walls    ();

    virtual void Draw    ();
    virtual void Physics ();
};

struct Circle : public Box
{
    double rx_, ry_;

    Circle ();

    virtual void Draw    ();
    virtual void Physics ();
};

struct Rectan : public Box
{
    double a_, b_;

    Rectan ();

    virtual void Draw    ();
    virtual void Physics ();
};

struct Triang : public Box
{
    double a_;

    Triang ();

    virtual void Draw    ();
    virtual void Physics ();
};

struct Letter : public Box
{
    int number;
    int r_;

    Letter ();

    virtual void Draw    ();
    virtual void Physics ();
};

void DrawEverything     (MissPoint* obj);
void PhysicsEverything  (MissPoint* obj);

typedef void (*DrawFunction) (MissPoint* obj);

const int Func_N = 2;
DrawFunction Mechanics[Func_N] = {PhysicsEverything, DrawEverything};

int main ()
{
    srand (time(0));

    txTextCursor (false);
    txCreateWindow (LENGTH, WIDTH, false);

    int box_N = 3;

    Box boxes[box_N];

    for (int i = 0; i < box_N; i ++) boxes[i].Fill ();

    while (!GetAsyncKeyState (VK_ESCAPE))
    {
        txClearConsole ();

        txBegin ();

        if (!GetAsyncKeyState (VK_SPACE))
        {
            txSetFillColor (TX_BLACK);
            txClear ();
        }

        for (int i = 0; i < box_N; i ++)
        {
            boxes[i].Control  ();
            boxes[i].Draw     ();
            //boxes[i].Mechanic ();
        }

        txEnd ();
    }

    return 0;
}

void DrawEverything (MissPoint* obj)
{
    obj -> Draw ();
}

void PhysicsEverything (MissPoint* obj)
{
    obj -> Physics ();
}

void Box::Control ()
{
    printf ("%d ", number_);

    if (GetAsyncKeyState (VK_RETURN))
    {
        txSleep (100);
        while (!GetAsyncKeyState (VK_RETURN)) {}
    }
    if (GetAsyncKeyState ('C')) MyLittleBox_[rand() % number_] = new Circle;
    if (GetAsyncKeyState ('R')) MyLittleBox_[rand() % number_] = new Rectan;
    if (GetAsyncKeyState ('T')) MyLittleBox_[rand() % number_] = new Triang;
    if (GetAsyncKeyState ('L')) MyLittleBox_[rand() % number_] = new Letter;
    if (GetAsyncKeyState ('N') && number_ < MAX_NUMBER)
    {
        number_ += 1;
        int type = rand() % 4;

        if (type == 0) MyLittleBox_[number_ - 1] = new Circle;
        if (type == 1) MyLittleBox_[number_ - 1] = new Rectan;
        if (type == 2) MyLittleBox_[number_ - 1] = new Triang;
        if (type == 3) MyLittleBox_[number_ - 1] = new Letter;

    }
    if (GetAsyncKeyState ('D') && number_ > MIN_NUMBER) number_ -= 1;
    if (GetAsyncKeyState ('B'))
    {
        x = rand() % (LENGTH / 2 - 50);
        y = rand() % (WIDTH  / 2 - 50);
    }
}

void Box::Fill ()
{
    for (int i = 0; i < number_; i ++)
    {
        int type = rand() % 5;

        if (type == 0) MyLittleBox_[i] = new Circle;
        if (type == 1) MyLittleBox_[i] = new Rectan;
        if (type == 2) MyLittleBox_[i] = new Triang;
        if (type == 3) MyLittleBox_[i] = new Letter;
        if (type == 4) MyLittleBox_[i] = new Box;
    }
}

void Box::Mechanic ()
{
    for (int i = 0; i < number_; i ++)
    for (int j = 0; j < Func_N; j ++)
    Mechanics[j] (MyLittleBox_[i]);
}

void Box::Draw ()
{
    for (int i = 0; i < MAX_NUMBER; i ++)
    {
        x = x_ - x;
        y = y_ - y;
    }

    txSetColor (TX_WHITE);

    txLine (x_ - x, y_ - y, x_ - x, y_ + y);
    txLine (x_ - x, y_ - y, x_ + x, y_ - y);
    txLine (x_ + x, y_ - y, x_ + x, y_ + y);
    txLine (x_ - x, y_ + y, x_ + x, y_ + y);
}

void Circle::Draw ()
{
    for (double t = 0; t < 1; t += 0.05)
    {
        txSetColor (RGB(Lerp(0, colorR_, t), Lerp(0, colorG_, t), Lerp(0, colorB_, t)));
        txSetFillColor (RGB(Lerp(0, colorR_, t), Lerp(0, colorG_, t), Lerp(0, colorB_, t)));

        txEllipse   (x_ - Lerp(0, rx_, 1 - t), y_ - Lerp(0, ry_, 1 - t),
                     x_ + Lerp(0, rx_, 1 - t), y_ + Lerp(0, ry_, 1 - t));
    }
}

void Rectan::Draw ()
{
    for (double t = 0; t < 1; t += 0.05)
    {
        txSetColor (RGB(Lerp(0, colorR_, t), Lerp(0, colorG_, t), Lerp(0, colorB_, t)));
        txSetFillColor (RGB(Lerp(0, colorR_, t), Lerp(0, colorG_, t), Lerp(0, colorB_, t)));

        txRectangle   (x_ - Lerp(0, a_, 1 - t), y_ - Lerp(0, b_, 1 - t),
                       x_ + Lerp(0, a_, 1 - t), y_ + Lerp(0, b_, 1 - t));
    }
}

void Triang::Draw ()
{
    for (double t = 0; t < 1; t += 0.01)
    {
        txSetColor (RGB (Lerp (0, colorR_, t), Lerp (0, colorG_, t), Lerp (0, colorB_, t)));
        txSetFillColor (RGB (Lerp (0, colorR_, t), Lerp (0, colorG_, t), Lerp (0, colorB_, t)));

        txLine   (x_ - Lerp (0, a_, 1 - t), y_ + Lerp (0, a_, 1 - t),
                  x_ + Lerp (0, a_, 1 - t), y_ + Lerp (0, a_, 1 - t));
        txLine   (x_ - Lerp (0, a_, 1 - t), y_ + Lerp (0, a_, 1 - t),
                  x_                      , y_ - Lerp (0, a_, 1 - t));
        txLine   (x_ + Lerp (0, a_, 1 - t), y_ + Lerp (0, a_, 1 - t),
                  x_                      , y_ - Lerp (0, a_, 1 - t));
    }
}

void Letter::Draw ()
{
    char a[32] = "ABCDEFGHIJKLMNOPQRSTUWVXyZ";

    char b[3] = "";

    sprintf (b, "%c", a[number_]);

    for (double t = 0; t < 1; t += 0.2)
    {
        txSetColor (RGB (Lerp (0, colorR_, t), Lerp (0, colorG_, t), Lerp (0, colorB_, t)));

        txDrawText (x_ - Lerp (0, r_, 1-t), y_ + Lerp (0, r_, 1-t),
                    x_ + r_, y_ - r_, b);
    }
}

void Circle::Physics ()
{
    double r_min = rx_;

    x_  +=  vx_;
    y_  +=  vy_;

    vx_ *= 0.9999;
    vy_ *= 0.9999;

    if (x_ + rx_ >= x)
    {
        vx_ *= -0.99;
        if (rx_ >= r_min)
        {
            rx_ -= 0.1;
            ry_ += 0.01;
        }
        x_ = x - rx_;
    }
    if (x >= x_ - rx_)
    {
        vx_ *= -0.99;
        if (rx_ >= r_min)
        {
            rx_ -= 0.1;
            ry_ += 0.01;
        }
        x_ = x + rx_;
    }

    if (y_ + ry_ >= y)
    {
        vy_ *= -0.99;
        if (ry_ >= r_min)
        {
            ry_ -= 0.1;
            rx_ += 0.01;
        }
        y_ = y - ry_;
    }
    if (y >= y_ - ry_)
    {
        vy_ *= -0.99;
        if (ry_ >= r_min)
        {
            ry_ -= 0.1;
            rx_ += 0.01;
        }
        y_ = y + ry_;
    }
}

void Rectan::Physics ()
{
    double r_min = a_;

    x_  +=  vx_;
    y_  +=  vy_;

    vx_ *= 0.9999;
    vy_ *= 0.9999;

    if (x_ + a_ >= x)
    {
        vx_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            b_ += 01;
        }
        x_ = x - a_;
    }
    if (x >= x_ - a_)
    {
        vx_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            b_ += 1;
        }
        x_ = x + a_;
    }

    if (y_ + b_ >= y)
    {
        vy_ *= -0.99;
        if (b_ >= r_min)
        {
            b_ -= 1;
            a_ += 1;
        }
        y_ = y - b_;
    }
    if (y >= y_ - b_)
    {
        vy_ *= -0.99;
        if (b_ >= r_min)
        {
            b_ -= 1;
            a_ += 1;
        }
        y_ = y + b_;
    }
}

void Triang::Physics ()
{
    double r_min = a_;

    x_  +=  vx_;
    y_  +=  vy_;

    vx_ *= 0.9999;
    vy_ *= 0.9999;

    if (x_ + a_ >= x)
    {
        vx_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            a_ += 01;
        }
        x_ = x - a_;
    }
    if (x >= x_ - a_)
    {
        vx_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            a_ += 1;
        }
        x_ = x + a_;
    }

    if (y_ + a_ >= y)
    {
        vy_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            a_ += 1;
        }
        y_ = y - a_;
    }
    if (y >= y_ - a_)
    {
        vy_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            a_ += 1;
        }
        y_ = y_ + a_;
    }
}

void Letter::Physics ()
{
    x_  +=  vx_;
    y_  +=  vy_;

    vx_ *= 0.9999;
    vy_ *= 0.9999;

    if (x_ + r_ >= x)
    {
        vx_ *= -0.99;
        x_ = x - r_;
    }
    if (x >= x_ - r_)
    {
        vx_ *= -0.99;
        x_ = x + r_;
    }

    if (y_ + r_ >= y)
    {
        vy_ *= -0.99;
        y_ = y - r_;
    }
    if (y >= y_ - r_)
    {
        vy_ *= -0.99;
        y_ = y + r_;
    }
}

void Box::Physics ()
{

}
double Lerp (double x1, double x2, double t)
{
    x1 = x1 + t * (x2 - x1);

    return x1;
}

MissPoint::MissPoint() :
    x_      (random (50, LENGTH - 50)),
    y_      (random (50, WIDTH  - 50)),
    vx_     (random (-15, 15)),
    vy_     (random (-15, 15)),
    colorR_ (random (50, 255)),
    colorG_ (random (50, 255)),
    colorB_ (random (50, 255))
    {}

Circle::Circle ():
    Box (),
    rx_ (rand () % 16 + 10),
    ry_ (rx_)
    {}

Rectan::Rectan ():
    Box (),
    a_  (rand () % 16 + 10),
    b_  (rand () % 16 + 10)
    {}

Triang::Triang ():
    Box (),
    a_  (rand () % 16 + 10)
    {}

Letter::Letter ():
    Box    (),
    number (rand () % 33),
    r_     (rand () % 10 + 10)
    {}

Box::Box () :
    MissPoint (),
    x (rand () % (LENGTH / 2)),
    y (rand () % (WIDTH  / 2)),
    number_   (MAX_NUMBER)
    {}
