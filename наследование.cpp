#include "TXLib.h"

double Lerp (double x1, double x2, double t);

const int LENGTH = GetSystemMetrics (SM_CXSCREEN) - 5;
const int WIDTH  = GetSystemMetrics (SM_CYSCREEN) - 68;

const int MAX_NUMBER = 50;
const int MIN_NUMBER = 10;

struct MissPoint
{
    int min_X, min_Y;
    int max_X, max_Y;

    double x_,  y_;
    double vx_, vy_;
    double colorR_, colorG_, colorB_;

    public : MissPoint();

    virtual void Draw    () = 0;
    virtual void Physics () = 0;
};

struct Circle : public MissPoint
{
    double rx_, ry_;

    Circle ();

    virtual void Draw    ();
    virtual void Physics ();
};

struct Rectan : public MissPoint
{
    double a_, b_;

    Rectan ();

    virtual void Draw    ();
    virtual void Physics ();
};

struct Triang : public MissPoint
{
    double a_;

    Triang ();

    virtual void Draw    ();
    virtual void Physics ();
};

struct Letter : public MissPoint
{
    int number_;
    int r_;

    Letter ();

    virtual void Draw    ();
    virtual void Physics ();
};

void Control (int* box_N, MissPoint* boxes[]);

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

    int box_N = 15;

    MissPoint* boxes[MAX_NUMBER];

    for (int i = 0; i < box_N; i ++)
    {
        int type = rand() % 4;

        if (type == 0) boxes[i] = new Circle;
        if (type == 1) boxes[i] = new Rectan;
        if (type == 2) boxes[i] = new Triang;
        if (type == 3) boxes[i] = new Letter;
    }

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
            for (int j = 0; j < Func_N; j ++)
            {
                Mechanics[j] (boxes[i]);
            }
        }

        Control (&box_N, boxes);

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

void Control (int* box_N, MissPoint* boxes[])
{
    printf ("%d ", *box_N);

    if (GetAsyncKeyState (VK_RETURN))
    {
        txSleep (100);
        while (!GetAsyncKeyState (VK_RETURN)) {}
    }
    if (GetAsyncKeyState ('C')) boxes[rand() % *box_N] = new Circle;
    if (GetAsyncKeyState ('R')) boxes[rand() % *box_N] = new Rectan;
    if (GetAsyncKeyState ('T')) boxes[rand() % *box_N] = new Triang;
    if (GetAsyncKeyState ('L')) boxes[rand() % *box_N] = new Letter;
    if (GetAsyncKeyState ('N') && *box_N < MAX_NUMBER)
    {
        *box_N += 1;
        int type = rand() % 4;

        if (type == 0) boxes[*box_N - 1] = new Circle;
        if (type == 1) boxes[*box_N - 1] = new Rectan;
        if (type == 2) boxes[*box_N - 1] = new Triang;
        if (type == 3) boxes[*box_N - 1] = new Letter;

    }
    if (GetAsyncKeyState ('D') && *box_N > MIN_NUMBER) *box_N -= 1;
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

    if (x_ + rx_ >= max_X)
    {
        vx_ *= -0.99;
        if (rx_ >= r_min)
        {
            rx_ -= 0.1;
            ry_ += 0.01;
        }
        x_ = max_X - rx_;
    }
    if (min_X >= x_ - rx_)
    {
        vx_ *= -0.99;
        if (rx_ >= r_min)
        {
            rx_ -= 0.1;
            ry_ += 0.01;
        }
        x_ = min_X + rx_;
    }

    if (y_ + ry_ >= max_Y)
    {
        vy_ *= -0.99;
        if (ry_ >= r_min)
        {
            ry_ -= 0.1;
            rx_ += 0.01;
        }
        y_ = max_Y - ry_;
    }
    if (min_Y >= y_ - ry_)
    {
        vy_ *= -0.99;
        if (ry_ >= r_min)
        {
            ry_ -= 0.1;
            rx_ += 0.01;
        }
        y_ = min_Y + ry_;
    }
}

void Rectan::Physics ()
{
    double r_min = a_;

    x_  +=  vx_;
    y_  +=  vy_;

    vx_ *= 0.9999;
    vy_ *= 0.9999;

    if (x_ + a_ >= max_X)
    {
        vx_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            b_ += 01;
        }
        x_ = max_X - a_;
    }
    if (min_X >= x_ - a_)
    {
        vx_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            b_ += 1;
        }
        x_ = min_X + a_;
    }

    if (y_ + b_ >= max_Y)
    {
        vy_ *= -0.99;
        if (b_ >= r_min)
        {
            b_ -= 1;
            a_ += 1;
        }
        y_ = max_Y - b_;
    }
    if (min_Y >= y_ - b_)
    {
        vy_ *= -0.99;
        if (b_ >= r_min)
        {
            b_ -= 1;
            a_ += 1;
        }
        y_ = min_Y + b_;
    }
}

void Triang::Physics ()
{
    double r_min = a_;

    x_  +=  vx_;
    y_  +=  vy_;

    vx_ *= 0.9999;
    vy_ *= 0.9999;

    if (x_ + a_ >= max_X)
    {
        vx_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            a_ += 01;
        }
        x_ = max_X - a_;
    }
    if (min_X >= x_ - a_)
    {
        vx_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            a_ += 1;
        }
        x_ = min_X + a_;
    }

    if (y_ + a_ >= max_Y)
    {
        vy_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            a_ += 1;
        }
        y_ = max_Y - a_;
    }
    if (min_Y >= y_ - a_)
    {
        vy_ *= -0.99;
        if (a_ >= r_min)
        {
            a_ -= 1;
            a_ += 1;
        }
        y_ = min_Y + a_;
    }
}

void Letter::Physics ()
{
    x_  +=  vx_;
    y_  +=  vy_;

    vx_ *= 0.9999;
    vy_ *= 0.9999;

    if (x_ + r_ >= max_X)
    {
        vx_ *= -0.99;
        x_ = max_X - r_;
    }
    if (min_X >= x_ - r_)
    {
        vx_ *= -0.99;
        x_ = min_X + r_;
    }

    if (y_ + r_ >= max_Y)
    {
        vy_ *= -0.99;
        y_ = max_Y - r_;
    }
    if (min_Y >= y_ - r_)
    {
        vy_ *= -0.99;
        y_ = min_Y + r_;
    }
}

double Lerp (double x1, double x2, double t)
{
    x1 = x1 + t * (x2 - x1);

    return x1;
}

MissPoint::MissPoint() :
    min_X  (100),
    min_Y  (100),
    max_X  (LENGTH-100),
    max_Y  (WIDTH-100),
    x_      (random (min_X + 50, max_X - 50)),
    y_      (random (min_Y + 50, max_Y - 50)),
    vx_     (random (-15, 15)),
    vy_     (random (-15, 15)),
    colorR_ (random (50, 255)),
    colorG_ (random (50, 255)),
    colorB_ (random (50, 255))
    {}

Circle::Circle ():
    MissPoint (),
    rx_  (rand () % 16 + 10),
    ry_  (rx_)
    {}

Rectan::Rectan ():
    MissPoint (),
    a_   (rand () % 16 + 10),
    b_   (rand () % 16 + 10)
    {}

Triang::Triang ():
    MissPoint (),
    a_   (rand () % 16 + 10)
    {}

Letter::Letter ():
    MissPoint (),
    number_ (rand () % 33),
    r_      (rand () % 10 + 10)
    {}
