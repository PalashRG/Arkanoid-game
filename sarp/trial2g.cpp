
#include <GL/glut.h>
#include <stdarg.h>
#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;

#define RADIUS 0.02

#define RATIO (4./3.)

int show[5][10];
float x_brick[4][9];
float y_brick[4][9];

float paddle_x = 0;
float paddle_y = -.76;
float paddle_speed = 0;
const float PaddleSpeedFactor = 2.;

bool phit_center = false , phit_corner = false;
bool game_over = false;
bool lvlprogress;
float speed_x = 0.;
float speed_y = 0.;

float x_pos = 0,y_pos = -0.74;                                              //ball coordinates
int lives  = 3;
int level = 0;

float T_last_frame = 0.;
int gamestate=0;
int blackOrRed=0;                                                     //menu button black/red

int powbhit = 0;                                                      //For movement of power block
float pow_x;
float pow_y;
float pow_speed_y= -0.2;
int showpow=1;
int pow_type=0;

float score =0;
int flag=1;
int hisccheck=0;



void initialshow()
{
for(int a = 0; a < 9; a++)
    {
        for(int b = 0; b < 4; b++)
        {
            show[b][a]=0;
        }
    }

}

void lvlprog()
{
     lvlprogress = false;
     int dummy = 0;
     for(int a = 0; a < 9; a++)
       {
          for(int b = 0; b < 4; b++)
            {
              if(show[b][a] != 0 )
                 dummy =1;
            }
       }
    if(dummy == 0)
        {
            level++;
            lvlprogress = true;
        }
}

void draw_bricks()                     //1
{

    glBegin(GL_QUADS);
    for(int a = 0; a < 9; a++)
    {
        for(int b = 0; b < 4; b++)
        {
            if(show[b][a] == 1)
            {
                glColor3f(1,0.95,.0);
                glVertex2f(x_brick[b][a],y_brick[b][a]);
                glVertex2f(x_brick[b][a],y_brick[b][a] - 0.10);
                glVertex2f(x_brick[b][a]+0.2,y_brick[b][a] - 0.10);
                glVertex2f(x_brick[b][a]+0.2,y_brick[b][a]);
            }
            else if(show[b][a] == 2)
            {
                glColor3f(1.0,0.0,0.0);
                glVertex2f(x_brick[b][a],y_brick[b][a]);
                glVertex2f(x_brick[b][a],y_brick[b][a] - 0.10);
                glVertex2f(x_brick[b][a]+0.2,y_brick[b][a] - 0.10);
                glVertex2f(x_brick[b][a]+0.2,y_brick[b][a]);
            }
            else if(show[b][a] == 3)
            {
                glColor3f(0.0,.5,1.0);
                glVertex2f(x_brick[b][a],y_brick[b][a]);
                glVertex2f(x_brick[b][a],y_brick[b][a] - 0.10);
                glVertex2f(x_brick[b][a]+0.2,y_brick[b][a] - 0.10);
                glVertex2f(x_brick[b][a]+0.2,y_brick[b][a]);
            }
        }
    }



    glEnd();
}

void ball_draw()                //2
{
    if(pow_type==1)
      glColor3f(1.0,0.0,0.0);
    else
      glColor3f(0.0,0.0,0.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x_pos,y_pos);
    for (float angle = 0; angle < (10); angle+=0.01)
    {
        glVertex2f((x_pos + sin(angle) * RADIUS), (y_pos + (cos(angle)) * RADIUS));
    }
    glColor3f(0.0,0.0,0.0);
    glEnd();
}

int brick_hit()              //3
{
    int hit = 0;
    int flag = 1;

    for(int a = 0; a < 9; a++)
    {
        for(int b =0; b < 4; b++)
        {

            if((x_pos >= x_brick[b][a] && x_pos  <= x_brick[b][a] + 0.02)  || (x_pos  >= x_brick[b][a] + 0.18 && x_pos <= x_brick[b][a] + 0.2))
            {
                if((y_pos - RADIUS <= y_brick[b][a] && y_pos - RADIUS >= y_brick[b][a] - 0.02) || (y_pos + RADIUS >= y_brick[b][a] - 0.1 && y_pos + RADIUS <= y_brick[b][a] - 0.08))
                {
                    if(show[b][a] >= 1 )
                    {
                        show[b][a]--;
                        flag = 0;
                        hit = 1;
                        score+=10;
                        if(a==5&&b==0)
                        {
                            pow_x=x_brick[0][5];
                            pow_y=y_brick[0][5];
                            powbhit=1;
                            score+=20;
                        }
                        else if(a==8&&b==3)
                        {
                            pow_x=x_brick[3][8];
                            pow_y=y_brick[3][8];
                            powbhit=2;
                            score+=20;
                        }
                        break;
                    }
                }
            }
            else  if(x_pos > x_brick[b][a] && x_pos < x_brick[b][a] + 0.2)
            {
                if((y_pos - RADIUS < y_brick[b][a] && y_pos - RADIUS > y_brick[b][a] - 0.02) || (y_pos + RADIUS > y_brick[b][a] - 0.1 && y_pos + RADIUS < y_brick[b][a] - 0.08))
                {
                    if(show[b][a] >= 1)
                    {
                        show[b][a]--;
                        flag = 0;
                        hit = 2;
                        score+=10;
                        if(a==5&&b==0)
                        {
                            pow_x=x_brick[0][5];
                            pow_y=y_brick[0][5];
                            powbhit=1;
                            score+=20;
                        }
                         else if(a==8&&b==3)
                        {
                            pow_x=x_brick[3][8];
                            pow_y=y_brick[3][8];
                            powbhit=2;
                            score+=20;
                        }
                        break;
                    }
                }
            }
            else  if(y_pos > y_brick[b][a] - 0.1 && y_pos < y_brick[b][a] )
            {
                if((x_pos - RADIUS < x_brick[b][a] + 0.2 && x_pos - RADIUS > x_brick[b][a] + 0.18) || (x_pos + RADIUS > x_brick[b][a] && x_pos + RADIUS < x_brick[b][a] + 0.02))
                {
                    if(show[b][a] >= 1)
                    {
                        show[b][a]--;
                        flag = 0;
                        score+=10;
                        if(a==5&&b==0)
                        {
                            pow_x=x_brick[0][5];
                            pow_y=y_brick[0][5];
                            powbhit=1;
                            score+=20;

                        }
                         else if(a==8&&b==3)
                        {
                            pow_x=x_brick[3][8];
                            pow_y=y_brick[3][8];
                            powbhit=2;
                            score+=20;
                        }
                        hit = 3;
                        break;
                    }
                }
            }


        }
        if(flag == 0)
            break;
    }
    return hit;
}

void draw_power()
{
     if(showpow==1)
     {

       if(powbhit==1)
        {
         glColor3f(1,0,0);
         glBegin(GL_QUADS);
         glVertex2f(pow_x + 0.05,pow_y);
         glVertex2f(pow_x + 0.15,pow_y);
         glVertex2f(pow_x +0.15,pow_y-0.04);
         glVertex2f(pow_x +0.05,pow_y-0.04);
         glEnd();
       }

       if(powbhit==2)
       {
         glColor3f(0.66,0.66,0.66);
         glBegin(GL_QUADS);
         glVertex2f(pow_x + 0.05,pow_y);
         glVertex2f(pow_x + 0.15,pow_y);
         glVertex2f(pow_x +0.15,pow_y-0.04);
         glVertex2f(pow_x +0.05,pow_y-0.04);
         glEnd();
       }
     }
}

bool crashed()                       //4
{
    if(y_pos < paddle_y - 0.1)
        {
         lives--;
         pow_type=0;
         showpow=0;
         return true;
        }
    return false;
}

void paddle_hit()                  //5
{
    phit_corner = false;
    phit_center = false;
    if(x_pos <= paddle_x + 0.13 && x_pos >= paddle_x - 0.13)
    {
        if(y_pos <= paddle_y)
        {
            phit_center = true;
        }
    }
    else if( (x_pos >= paddle_x + 0.13 && x_pos <= paddle_x + 0.2) ||
             (x_pos <= paddle_x - 0.13 && x_pos >= paddle_x - 0.2))
    {
        if(y_pos <= paddle_y)
        {
            phit_corner = true;
        }
    }
}

void paddle_move(float dT)         //6
{
    dT=dT/8;
    if(paddle_x < RATIO && paddle_x > -RATIO)
        paddle_x += paddle_speed * PaddleSpeedFactor * dT;

    if( paddle_x > 1.2)
    {
        paddle_x = 1.2;
        paddle_speed *= 0.;
    }

    if( paddle_x < -1.2)
    {
        paddle_x = -1.2;
        paddle_speed = 0.;
    }

    paddle_speed *= (1. - 0.01);

}

void ball_move(float dT)           //7
{
    if(pow_type==1)
      dT= 2.5*dT;
    x_pos += speed_x* dT/2;
    y_pos += speed_y * dT/2;
    int bh = brick_hit();

    if(pow_type!=1)
    {
       if( bh == 1 )
        {
          speed_x *= -1;
          speed_y *= -1;
        }
        if( bh == 2)
        {
          speed_y *= -1;
        }
        if( bh == 3)
        {
          speed_x *= -1;
        }
    }

    if( x_pos >= (RATIO-RADIUS) || x_pos <= (-RATIO+RADIUS ) )
    {
        speed_x *= -1;
    }

    if( y_pos >= (0.8 -RADIUS) )
    {
        speed_y *= -1;
    }

    paddle_hit();
    if(phit_center)
    {
        speed_y = 1;
        if(abs(speed_x + paddle_speed)<=2.0)
           speed_x += paddle_speed ;
        else
           speed_x *= 2.0/abs(speed_x);
    }
    if(phit_corner)
    {
        speed_x += -1*paddle_speed ;
        if(abs(speed_x)>2.5)
           speed_x *=2.5/abs(speed_x);
        speed_y = 1;
    }
}

void power_move(float dT)
{
   pow_y+=pow_speed_y*dT;
   if(pow_x+0.05>=paddle_x-0.2 && pow_x+0.15<=paddle_x+0.2)
      if(pow_y<=paddle_y)
        {
            pow_speed_y=0;
            showpow=0;
            if(powbhit==1)
              {
                  pow_type=1;
                  score+=50;
              }

            if(powbhit==2)
              {
                  lives++;
                  score+=50;
              }
            powbhit=0;
        }
}

void paddle_draw()                  //8
{
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_QUADS);
    glVertex2f(paddle_x - 0.2, paddle_y);
    glVertex2f(paddle_x + 0.2, paddle_y);
    glVertex2f(paddle_x + 0.2, paddle_y - 0.05);
    glVertex2f(paddle_x - 0.2, paddle_y - 0.05);
    glEnd();
}

void step_game()                  //9
{
    if(powbhit==1||powbhit==2)
    power_move(T_last_frame);
    paddle_move(T_last_frame*2);
    ball_move(T_last_frame);

    if(crashed())
    {
        speed_x = 0;
        speed_y = 0;
        x_pos = 0;
        y_pos = -0.74 ;
        paddle_speed = 0;
        paddle_x = 0;
    }

    glutPostRedisplay();
}

void launch_ball()                   //10
{
   speed_y = 1.5;
   speed_x = 1.5;
}


void ArrowKeys(int key, int x, int y)                  //12
{
    if(key==GLUT_KEY_LEFT)
        paddle_speed = -3.;

    if(key==GLUT_KEY_RIGHT)
        paddle_speed = +3.;


}


void DrawTex(const char *text, int leng , float x, float y,int fsize)
{
    const int win_width  = glutGet(GLUT_WINDOW_WIDTH);
    const int win_height = glutGet(GLUT_WINDOW_HEIGHT);
    const float win_aspect = (float)win_width / (float)win_height;
    glMatrixMode(GL_PROJECTION);
    double *matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX,matrix);
    glLoadIdentity();
    if(win_aspect > RATIO)
    {
        glOrtho(-win_aspect, win_aspect , -1., 1., 0.0f, 1.);
    }
    else
    {
        glOrtho(-RATIO, RATIO, -RATIO/win_aspect, RATIO/win_aspect, 0.0f, 1.);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(x,y);
    if(fsize==1)
       for(int i=0;i<leng;i++)
       {
          glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)text[i]);
       }
    else if(fsize==2)
       for(int i=0;i<leng;i++)
       {
          glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,(int)text[i]);
       }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
    delete[] matrix;
}

void set_xy()                        //13
{
    int lvl;
    ifstream fi;
    if(level==1)
        fi.open("level1.txt");
    if(level==2)
        fi.open("level2.txt");
    if(level==3)
        fi.open("level3.txt");

    for(int a = 0; a < 4; a++)
    {
        for(int b = 0; b < 9; b++)
        {
            fi>>lvl;
            if(lvl==1)
               show[a][b] = 1;
            else if(lvl==2)
               show[a][b] = 2;
            else if(lvl==3)
                show[a][b] = 3;
            else if(lvl==0)
                show[a][b] = 0;

        }
    }
    fi.close();
    int c = 0;
    for(float a = -0.94; c <= 8; a+=0.21)
    {

        for(int b = 0; b <= 5; b++)
        {
            x_brick[b][c] = a;

        }
        c++;
    }
    int d = 0;
    for(float s = 0.69; d <= 3; s-=0.11)
    {
        for(int  r = 0; r < 9; r++)
        {
            y_brick[d][r] = s;
        }
        d++;
    }
}

void user_input(unsigned char key, int x, int y)    //11
{
    if(key == 13 && (crashed() || (speed_x==0 && speed_y==0)) && lives!=0)
        launch_ball();
    else if(key==13 && lives==0)
       {
        gamestate=0;
        lives=3;
        set_xy();
          powbhit=0;
          pow_type=0;
          showpow=1;
          score=0;
       }
     if(key==27 && gamestate!=1)
        gamestate=0;
}


void credits()
{
   const int win_width  = glutGet(GLUT_WINDOW_WIDTH);
   const int win_height = glutGet(GLUT_WINDOW_HEIGHT);
   const float win_aspect = (float)win_width / (float)win_height;
    glViewport(0,0,win_width,win_height);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0., 0.1, 1., 0.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-RATIO, RATIO, -RATIO/win_aspect, RATIO/win_aspect, 0., 1.);
    glBegin(GL_QUADS);
       glColor3f(0.95,1,1.0);
       glVertex2f(-RATIO,-1.0f);
       glVertex2f(RATIO,-1.0f);
       glColor3f(1,0.75,0.5);
       glVertex2f(RATIO,1.0f);
       glVertex2f(-RATIO,1.0f);
       glEnd();
       string cred,ad,s,r,gd,a,p;
       cred="CREDITS";
       ad="Algorithm developers :";
       s="Suraj Naik";
       r="Raj Verma";
       gd="Graphics designers :";
       a="Ashwin Munjewar";
       p="Palash Gajjar";
       glColor3f(0,0,0);
       DrawTex(cred.data(),cred.size(),-0.2,0.7,1);
       glColor3f(0,0,1);
       DrawTex(ad.data(),ad.size(),-0.7,0.5,1);
       DrawTex(gd.data(),gd.size(),-0.7,0.0,1);
       glColor3f(1,0,0.2);
       DrawTex(s.data(),s.size(),-0.6,0.3,1);
       DrawTex(r.data(),r.size(),-0.6,0.2,1);
       DrawTex(a.data(),a.size(),-0.6,-0.2,1);
       DrawTex(p.data(),p.size(),-0.6,-0.3,1);
       glutSwapBuffers();

}

void hiScore()
{
   const int win_width  = glutGet(GLUT_WINDOW_WIDTH);
   const int win_height = glutGet(GLUT_WINDOW_HEIGHT);
   const float win_aspect = (float)win_width / (float)win_height;
    glViewport(0,0,win_width,win_height);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0., 0.1, 1., 0.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-RATIO, RATIO, -RATIO/win_aspect, RATIO/win_aspect, 0., 1.);
     glBegin(GL_QUADS);
       glColor3f(1,0.5,0);
       glVertex2f(-RATIO,-1.0f);
       glVertex2f(RATIO,-1.0f);
       glColor3f(0,1,0.5);
       glVertex2f(RATIO,1.0f);
       glVertex2f(-RATIO,1.0f);
       glEnd();

       glColor3f(0,0,0);
       float sc;
       string hisc;
       hisc="-- HI - SCORE --";

       DrawTex(hisc.data(),hisc.size(),-0.3,0.7,1);
       ifstream fi;
       fi.open("hi-score.txt");
       int i=0;
       while(fi>>sc && i<10)
       {
           ostringstream oss;
           oss<<sc;
           string hi=oss.str();
           DrawTex(hi.data(),hi.size(),-0.3,0.6-i*0.1,2);
           i++;

       }
       fi.close();
       glutSwapBuffers();
}

void help()
{
   const int win_width  = glutGet(GLUT_WINDOW_WIDTH);
   const int win_height = glutGet(GLUT_WINDOW_HEIGHT);
   const float win_aspect = (float)win_width / (float)win_height;
    glViewport(0,0,win_width,win_height);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0., 0.1, 1., 0.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-RATIO, RATIO, -RATIO/win_aspect, RATIO/win_aspect, 0., 1.);
     glBegin(GL_QUADS);
       glColor3f(1,1,0);
       glVertex2f(-RATIO,-1.0f);
       glVertex2f(RATIO,-1.0f);
       glColor3f(1,0,0);
       glVertex2f(RATIO,1.0f);
       glVertex2f(-RATIO,1.0f);
       glEnd();
       string hp,lp,rp,bu,ac,es,lb,ai1,ai2,pow0,pow1,pow2;
       glColor3f(0,0,0);
       ai1="AIM : Player has to control the paddle to prevent the ball from";
       ai2="falling from the playing field";
       lb="Launch Ball :";
       bu="Button";
       ac="Action";
       hp="HELP";
       lp="Paddle  move left :";
       rp="Paddle move right :";
       es="To escape from sub-menu :";
       pow0="There are 2 power-ups : ";
       pow1="Fireball (Red powerblock)";
       pow2="Extra life (Grey powerblock)";
       DrawTex(ai1.data(),ai1.size(),-0.8*win_width/800,0.8*win_height/600,2);
       DrawTex(ai2.data(),ai2.size(),-0.63*win_width/800,0.7*win_height/600,2);
       DrawTex(hp.data(),hp.size(),-0.1*win_width/800,0.9*win_height/600,1);
       DrawTex(bu.data(),bu.size(),0.16*win_width/800,0.4*win_height/600,1);
       DrawTex(ac.data(),ac.size(),-0.7*win_width/800,0.4*win_height/600,1);
       DrawTex(es.data(),es.size(),-0.8*win_width/800,-0.2*win_height/600,2);
       DrawTex(lp.data(),lp.size(),-0.8*win_width/800,0.1*win_height/600,2);
       DrawTex(rp.data(),rp.size(),-0.8*win_width/800,-0.05*win_height/600,2);
       DrawTex(lb.data(),lb.size(),-0.8*win_width/800,0.25*win_height/600,2);
       DrawTex(pow0.data(),pow0.size(),-0.8*win_width/800,-0.4*win_height/600,2);
       DrawTex(pow1.data(),pow1.size(),-0.8*win_width/800,-0.5*win_height/600,2);
       DrawTex(pow2.data(),pow2.size(),-0.8*win_width/800,-0.6*win_height/600,2);

       glBegin(GL_QUADS);
       glColor3f(1,1,1);
       glVertex2f(0.17f*win_width/800,-0.22f*win_height/600);
       glVertex2f(0.38f*win_width/800,-0.22f*win_height/600);
       glVertex2f(0.38f*win_width/800,-0.13f*win_height/600);
       glVertex2f(0.17f*win_width/800,-0.13f*win_height/600);
       glEnd();
       glColor3f(0,0,0);
       string esc="Esc";
       DrawTex(esc.data(),esc.size(),0.22*win_width/800,-0.2*win_height/600,2);

       glBegin(GL_QUADS);
       glColor3f(1,1,1);
       glVertex2f(0.17f*win_width/800,0.23f*win_height/600);
       glVertex2f(0.38f*win_width/800,0.23f*win_height/600);
       glVertex2f(0.38f*win_width/800,0.32f*win_height/600);
       glVertex2f(0.17f*win_width/800,0.32f*win_height/600);
       glEnd();
       glColor3f(0,0,0);
       string ent="Enter";
       DrawTex(ent.data(),ent.size(),0.2*win_width/800,0.25*win_height/600,2);

       glBegin(GL_QUADS);
       glColor3f(1,1,1);
       glVertex2f(0.17f*win_width/800,0.08f*win_height/600);
       glVertex2f(0.38f*win_width/800,0.08f*win_height/600);
       glVertex2f(0.38f*win_width/800,0.17f*win_height/600);
       glVertex2f(0.17f*win_width/800,0.17f*win_height/600);
       glColor3f(0,0,0);
       glVertex2f(0.2f*win_width/800,0.13f*win_height/600);
       glVertex2f(0.3f*win_width/800,0.13f*win_height/600);
       glVertex2f(0.3f*win_width/800,0.12f*win_height/600);
       glVertex2f(0.2f*win_width/800,0.12f*win_height/600);
       glEnd();
       glBegin(GL_TRIANGLES);
       glVertex2f(0.35f*win_width/800,0.125f*win_height/600);
       glVertex2f(0.3f*win_width/800,0.140f*win_height/600);
       glVertex2f(0.3f*win_width/800,0.110f*win_height/600);
       glEnd();

       glBegin(GL_QUADS);
       glColor3f(1,1,1);
       glVertex2f(0.17f*win_width/800,-0.07f*win_height/600);
       glVertex2f(0.38f*win_width/800,-0.07f*win_height/600);
       glVertex2f(0.38f*win_width/800,0.02f*win_height/600);
       glVertex2f(0.17f*win_width/800,0.02f*win_height/600);
       glColor3f(0,0,0);
       glVertex2f(0.25f*win_width/800,-0.02f*win_height/600);
       glVertex2f(0.35f*win_width/800,-0.02f*win_height/600);
       glVertex2f(0.35f*win_width/800,-0.03f*win_height/600);
       glVertex2f(0.25f*win_width/800,-0.03f*win_height/600);
       glEnd();
       glBegin(GL_TRIANGLES);
       glVertex2f(0.20f*win_width/800,0.-0.025f*win_height/600);
       glVertex2f(0.25f*win_width/800,-0.010f*win_height/600);
       glVertex2f(0.25f*win_width/800,-0.040f*win_height/600);
       glEnd();


       glutSwapBuffers();
}

void displayMenu()
{
  const int win_width  = glutGet(GLUT_WINDOW_WIDTH);
  const int win_height = glutGet(GLUT_WINDOW_HEIGHT);
  glViewport(0,0,win_width,win_height);
     glMatrixMode(GL_MODELVIEW);
     glClearColor(0., 0.75, 1., 0.);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glBegin(GL_QUADS);
        glColor3f(1,1,1);
        glVertex2f(-RATIO,-1.0f);
        glVertex2f(RATIO,-1.0f);
        glColor3f(0,0,0.5);
        glVertex2f(RATIO,1.0f);
        glVertex2f(-RATIO,1.0f);
        glEnd();
  glColor3f(0.5,1,1);
  glBegin(GL_QUADS);
  for(int i=0;i<15;i++)
     {  if(i==3||i==5)
        continue;
        glVertex2f(-0.65+0.1*i,0.4);
        glVertex2f(-0.65+0.1*i,0.7);
        glVertex2f(-0.67+0.1*i,0.7);
        glVertex2f(-0.67+0.1*i,0.4);

     }
  glEnd();

  glBegin(GL_QUADS);
       glVertex2f(-0.55,0.7);
       glVertex2f(-0.67,0.7);
       glVertex2f(-0.67,0.72);
       glVertex2f(-0.55,0.72);

       glVertex2f(-0.55,0.55);
       glVertex2f(-0.67,0.55);
       glVertex2f(-0.67,0.57);
       glVertex2f(-0.55,0.57);

       glVertex2f(-0.35,0.7);
       glVertex2f(-0.47,0.7);
       glVertex2f(-0.47,0.72);
       glVertex2f(-0.35,0.72);

       glVertex2f(-0.35,0.72);
       glVertex2f(-0.37,0.72);
       glVertex2f(-0.35,0.55);
       glVertex2f(-0.37,0.55);

       glVertex2f(-0.36,0.55);
       glVertex2f(-0.36,0.57);
       glVertex2f(-0.47,0.57);
       glVertex2f(-0.47,0.55);

       glVertex2f(-0.36,0.4);
       glVertex2f(-0.36,0.44);
       glVertex2f(-0.47,0.57);
       glVertex2f(-0.47,0.55);

       glVertex2f(-0.26,0.55);
       glVertex2f(-0.26,0.59);
       glVertex2f(-0.17,0.72);
       glVertex2f(-0.17,0.70);

       glVertex2f(-0.26,0.55);
       glVertex2f(-0.26,0.59);
       glVertex2f(-0.17,0.42);
       glVertex2f(-0.17,0.4);

       glVertex2f(-0.05,0.68);
       glVertex2f(-0.05,0.7);
       glVertex2f(0.05,.7);
       glVertex2f(0.05,0.68);

       glVertex2f(-0.05,0.57);
       glVertex2f(-0.05,0.59);
       glVertex2f(0.05,.59);
       glVertex2f(0.05,0.57);

       glVertex2f(0.15,0.67);
       glVertex2f(0.15,0.7);
       glVertex2f(0.25,.42);
       glVertex2f(0.25,0.39);

       glVertex2f(0.33,0.7);
       glVertex2f(0.33,0.72);
       glVertex2f(0.45,.72);
       glVertex2f(0.45,0.7);

       glVertex2f(0.33,0.4);
       glVertex2f(0.33,0.42);
       glVertex2f(0.45,.42);
       glVertex2f(0.45,0.4);

       glVertex2f(0.75,0.7);
       glVertex2f(0.75,0.72);
       glVertex2f(0.60,.72);
       glVertex2f(0.60,0.7);

       glVertex2f(0.73,0.4);
       glVertex2f(0.73,0.42);
       glVertex2f(0.60,.42);
       glVertex2f(0.60,0.4);


  glEnd();

  glColor3f(0.1,0.1,0);
  string start,help,hiscore,credits,exitg;

  start="Start";
  help="Help";
  hiscore="Hi-Score";
  credits="Credits";
  exitg="Exit";
  if(blackOrRed==0)
  {

    float dis= 0.15;
    glBegin(GL_QUADS);
     for(int i=0;i<5;i++)
     {
        glVertex2f(-0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,0.0f-dis*i);
        glVertex2f(-0.2f,0.0f-dis*i);

     }
  glEnd();
  glColor3f(0.7,0.7,0);

  DrawTex(start.data(),start.size(),-0.08f,-0.067f,1);
  DrawTex(help.data(),help.size(),-0.08f,-0.217f,1);
  DrawTex(hiscore.data(),hiscore.size(),-0.146f,-0.367f,1);
  DrawTex(credits.data(),credits.size(),-0.116f,-0.517f,1);
  DrawTex(exitg.data(),exitg.size(),-0.086f,-0.667f,1);
  }
  else if(blackOrRed==1)
  {
    float dis= 0.15;
    glBegin(GL_QUADS);
     for(int i=0;i<5;i++)
     {
         if(i==blackOrRed-1)
         glColor3f(1,0,0);
        else
         glColor3f(0.1,0.1,0);
        glVertex2f(-0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,0.0f-dis*i);
        glVertex2f(-0.2f,0.0f-dis*i);

     }
  glEnd();

  glColor3f(0,0,0);
  DrawTex(start.data(),start.size(),-0.08f,-0.067f,1);
  glColor3f(0.7,0.7,0);
  DrawTex(help.data(),help.size(),-0.08f,-0.217f,1);
  DrawTex(hiscore.data(),hiscore.size(),-0.146f,-0.367f,1);
  DrawTex(credits.data(),credits.size(),-0.116f,-0.517f,1);
  DrawTex(exitg.data(),exitg.size(),-0.086f,-0.667f,1);
  }

  else if(blackOrRed==2)
  {
    float dis= 0.15;
    glBegin(GL_QUADS);
     for(int i=0;i<5;i++)
     {
         if(i==blackOrRed-1)
         glColor3f(1,0,0);
        else
         glColor3f(0.1,0.1,0);
        glVertex2f(-0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,0.0f-dis*i);
        glVertex2f(-0.2f,0.0f-dis*i);

     }
  glEnd();

  glColor3f(0.7,0.7,0);
  DrawTex(start.data(),start.size(),-0.08f,-0.067f,1);
  glColor3f(0,0,0);
  DrawTex(help.data(),help.size(),-0.08f,-0.217f,1);
  glColor3f(0.7,0.7,0);
  DrawTex(hiscore.data(),hiscore.size(),-0.146f,-0.367f,1);
  DrawTex(credits.data(),credits.size(),-0.116f,-0.517f,1);
  DrawTex(exitg.data(),exitg.size(),-0.086f,-0.667f,1);
  }

  else if(blackOrRed==3)
  {
    float dis= 0.15;
    glBegin(GL_QUADS);
     for(int i=0;i<5;i++)
     {
         if(i==blackOrRed-1)
         glColor3f(1,0,0);
        else
         glColor3f(0.1,0.1,0);
        glVertex2f(-0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,0.0f-dis*i);
        glVertex2f(-0.2f,0.0f-dis*i);

     }
  glEnd();

  glColor3f(0.7,0.7,0);
  DrawTex(start.data(),start.size(),-0.08f,-0.067f,1);
  DrawTex(help.data(),help.size(),-0.08f,-0.217f,1);
  glColor3f(0,0,0);
  DrawTex(hiscore.data(),hiscore.size(),-0.146f,-0.367f,1);
  glColor3f(0.7,0.7,0);
  DrawTex(credits.data(),credits.size(),-0.116f,-0.517f,1);
  DrawTex(exitg.data(),exitg.size(),-0.086f,-0.667f,1);
  }

  else if(blackOrRed==4)
  {
    float dis= 0.15;
    glBegin(GL_QUADS);
     for(int i=0;i<5;i++)
     {
         if(i==blackOrRed-1)
         glColor3f(1,0,0);
        else
         glColor3f(0.1,0.1,0);
        glVertex2f(-0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,0.0f-dis*i);
        glVertex2f(-0.2f,0.0f-dis*i);

     }
  glEnd();

  glColor3f(0.7,0.7,0);
  DrawTex(start.data(),start.size(),-0.08f,-0.067f,1);
  DrawTex(help.data(),help.size(),-0.08f,-0.217f,1);
  DrawTex(hiscore.data(),hiscore.size(),-0.146f,-0.367f,1);
  glColor3f(0,0,0);
  DrawTex(credits.data(),credits.size(),-0.116f,-0.517f,1);
  glColor3f(0.7,0.7,0);
  DrawTex(exitg.data(),exitg.size(),-0.086f,-0.667f,1);
  }

  else if(blackOrRed==5)
  {
    float dis= 0.15;
    glBegin(GL_QUADS);
     for(int i=0;i<5;i++)
     {
         if(i==blackOrRed-1)
         glColor3f(1,0,0);
        else
         glColor3f(0.1,0.1,0);
        glVertex2f(-0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,-0.1f-dis*i);
        glVertex2f(0.2f,0.0f-dis*i);
        glVertex2f(-0.2f,0.0f-dis*i);

     }
  glEnd();

  glColor3f(0.7,0.7,0);
  DrawTex(start.data(),start.size(),-0.08f,-0.067f,1);
  DrawTex(help.data(),help.size(),-0.08f,-0.217f,1);
  DrawTex(hiscore.data(),hiscore.size(),-0.146f,-0.367f,1);
  DrawTex(credits.data(),credits.size(),-0.116f,-0.517f,1);
  glColor3f(0,0,0);
  DrawTex(exitg.data(),exitg.size(),-0.086f,-0.667f,1);
  }

  glFlush();
  glutSwapBuffers();

}

void gamePlayScreen()
{

     const int win_width  = glutGet(GLUT_WINDOW_WIDTH);
     const int win_height = glutGet(GLUT_WINDOW_HEIGHT);
     const float win_aspect = (float)win_width / (float)win_height;
     glViewport(0,0,win_width,win_height);
     glMatrixMode(GL_MODELVIEW);
     glClearColor(0, 0, 0, 0);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     if(win_aspect > RATIO)
     {
          glOrtho(-win_aspect, win_aspect , -1., 1., 0., 1.);
     }
      else
     {
        glOrtho(-RATIO, RATIO, -RATIO/win_aspect, RATIO/win_aspect, 0., 1.);
     }

        glBegin(GL_QUADS);
        glColor3f(1.0,0.8,0);
        glVertex2f(-RATIO, -1);
        glVertex2f(RATIO, -1);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(RATIO, 1);
        glVertex2f(-RATIO, 1);
        glEnd();

        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glVertex2f(-RATIO,0.8);
        glVertex2f(RATIO,0.8);
        glVertex2f(RATIO,0.81);
        glVertex2f(-RATIO,0.81);
        glEnd();


        lvlprog();
        if(lvlprogress)
        {
          if(level>1)
          lives++;
          speed_x = 0;
          speed_y = 0;
          x_pos = 0;
          y_pos = -0.74 ;
          paddle_speed = 0;
          paddle_x = 0;
          set_xy();
          lvlprogress=false;
          powbhit=0;
          pow_type=0;
          showpow=1;
          pow_speed_y= -0.2;
       }
       draw_bricks();
       paddle_draw();
       ball_draw();
       if(powbhit==1||powbhit==2)
       draw_power();
       glColor3f(0,0,0);
       ostringstream oss1,oss2;
       oss1<<"Score : "<<score;
       string sc = oss1.str();
       DrawTex(sc.data(),sc.size(),-0.8,0.85,1);
       oss2<<"Lives : "<<lives;
       string li = oss2.str();
       DrawTex(li.data(),li.size(),0.3,0.85,1);
       if ( lives==0)
       {
           string go;
           go = " Game Over !!";
           DrawTex(go.data(),go.size(),-0.2,0.0,1);
           string ent;
           ent ="(Press enter to return to main menu)";
           DrawTex(ent.data(),ent.size(),-0.45,-0.2,2);
           if(hisccheck==0)
           {
                  ofstream fo;


              fo.open("hi-score.txt",ios::app);

                 fo<<score<<"  ";
               fo.close();
               hisccheck=1;
           }
       }
       glutSwapBuffers();

       T_last_frame = 1./800.;
}

void display()                     //14
{

     if(gamestate==0)

          displayMenu();

     else if(gamestate==1)

          gamePlayScreen();

     else if(gamestate==2)

          help();

     else if(gamestate==3)

          hiScore();

     else if(gamestate==4)

          credits();

}

void mouseClick(int button, int state, int x, int y)
{
    const int win_width  = glutGet(GLUT_WINDOW_WIDTH);
    const int win_height = glutGet(GLUT_WINDOW_HEIGHT);

    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
         if(x>=0.4*win_width && x<=0.6*win_width)
         {
           if(y>=0.5*win_height && y<=0.55*win_height)

               gamestate=1;

           else if(y>=0.575*win_height && y<=0.625*win_height)

               gamestate=2;

           else if(y>=0.65*win_height && y<=0.7*win_height)

                gamestate=3;

           else if(y>=0.725*win_height && y<=0.775*win_height)

                gamestate=4;

           else if(y>=0.8*win_height && y<=0.85*win_height)

                exit(0);

         }

    }
}

void mouseMove(int x, int y)
{
    const int win_width  = glutGet(GLUT_WINDOW_WIDTH);
    const int win_height = glutGet(GLUT_WINDOW_HEIGHT);
    string start,help,hiscore,credits,exitg;

    start="Start";
    help="Help";
    hiscore="Hi-Score";
    credits="Credits";
    exitg="Exit";


    if(gamestate==0)
    {


         if(x>=0.4*win_width && x<=0.6*win_width)
         {
           if(y>=0.5*win_height && y<=0.55*win_height)

                   blackOrRed=1;

           else if(y>=0.575*win_height && y<=0.625*win_height)

                   blackOrRed=2;

           else if(y>=0.65*win_height && y<=0.7*win_height)

                   blackOrRed=3;

           else if(y>=0.725*win_height && y<=0.775*win_height)

                   blackOrRed=4;

           else if(y>=0.8*win_height && y<=0.85*win_height)

                   blackOrRed=5;
          else
            blackOrRed=0;
         }
       else
            blackOrRed=0;

    }
    glutSwapBuffers();
}

int main(int argc, char **argv)
{

    initialshow();
    set_xy();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowPosition(0,0);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Arkanoid - SARP");
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseMove);
    glutDisplayFunc(display);

    glutKeyboardFunc(user_input);
    glutSpecialFunc(ArrowKeys);

    glutIdleFunc(step_game);

    glutMainLoop();

    return 0;

}
