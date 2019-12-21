#include<iostream>
#include<unistd.h>
#include<cmath>
#include<GL/glut.h>
#include<time.h>

using namespace std;

class Point{
    public:
        float x;
        float y;
        Point(){
            x = 0;
            y = 0;
        }
        Point(float x, float y){
            this->x = x;
            this->y = y;
        }
};

class Projectile{
    public:
        float velocity, angle;
        float curve_angle;
        float horizontalRange, ToF, timeslice, maxHeight;
        float g, t;
        float v_x, v_y;
        int end;
        Point point_array[100];

        float thetaToRadian(float angle){
            angle = (angle * 3.14) / 180;
            return angle;
        }

        Projectile(){
                t = 0;
                g = 9.8;
                end = 0;
                while(1){
                    velocity = rand() % 150;
                    angle = rand() % 90;
                    angle = thetaToRadian(angle);
                    horizontalRange = (velocity * velocity * sin(2*angle)) / g;
                    maxHeight = (velocity * velocity * sin(angle) * sin(angle))/ (2 * g);

                    if((50 < horizontalRange) && (horizontalRange < 1000) && (50 < maxHeight) && (maxHeight < 1000)){
                        //cout << "Angle is: " << angle << "\t" << "Velocity: " << velocity << "\n";
                        //cout << "Horizontal Range: " << horizontalRange << "Max. Height: " << maxHeight << "\n";
                        break;
                    }
                }

                ToF = (2 * velocity * sin(angle)) / g;
                timeslice = ToF / 100;
                curve_angle = angle;
                v_x = velocity * cos(angle);
                v_y = velocity * sin(angle);
                //cout << ToF << "\t" << angle << "\t" << timeslice << "\n";
        }
};

Projectile* p[2];

void rotate(float angle, Point* p1, Point* p2, Point* p3){
    p3 -> x = p1->x + (p2->x - p1->x)*cos(angle) - (p2->y - p1->y)*sin(angle);
    p3 -> y = p1->y + (p2->x - p1->x)*sin(angle) + (p2->y - p1->y)*cos(angle);
}

void myDisplayFunction(){

    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(-100, 900, -100, 900);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(5);

    glColor3f(1.0,0.0,0.0);
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2i(0,0);
        glVertex2i(900, 0);
        glVertex2i(0,0);
        glVertex2i(0, 900);
    glEnd();

    glColor3f(0.0,1.0,0.0);
    for(int i = 0; i < p[0]->end - 1; i++){
        glBegin(GL_LINES);
            glVertex2f(p[0]->point_array[i].x, p[0]->point_array[i].y);
            glVertex2f(p[0]->point_array[i+1].x, p[0]->point_array[i+1].y);
        glEnd();
    }

    glColor3f(0.0,0.0,1.0);
    for(int i = 0; i < p[1]->end - 1; i++){
        glBegin(GL_LINES);
            glVertex2f(p[1]->point_array[i].x, p[1]->point_array[i].y);
            glVertex2f(p[1]->point_array[i+1].x, p[1]->point_array[i+1].y);
        glEnd();
    }

    Point* p01 = new Point(p[0]->point_array[p[0]->end - 1].x, p[0]->point_array[p[0]->end - 1].y + 10);
    Point* p02 = new Point(p[0]->point_array[p[0]->end - 1].x, p[0]->point_array[p[0]->end - 1].y - 10);
    Point* p03 = new Point(p[0]->point_array[p[0]->end - 1].x + 10, p[0]->point_array[p[0]->end - 1].y);

    Point* t01 = new Point();
    Point* t02 = new Point();
    Point* t03 = new Point();
    rotate(p[0] -> curve_angle, &(p[0]->point_array[p[0]->end - 1]), p01, t01);
    rotate(p[0] -> curve_angle, &(p[0]->point_array[p[0]->end - 1]), p02, t02);
    rotate(p[0] -> curve_angle, &(p[0]->point_array[p[0]->end - 1]), p03, t03);

    Point* p11 = new Point(p[1]->point_array[p[1]->end - 1].x, p[1]->point_array[p[1]->end - 1].y + 10);
    Point* p12 = new Point(p[1]->point_array[p[1]->end - 1].x, p[1]->point_array[p[1]->end - 1].y - 10);
    Point* p13 = new Point(p[1]->point_array[p[1]->end - 1].x + 10, p[1]->point_array[p[1]->end - 1].y);

    Point* t11 = new Point();
    Point* t12 = new Point();
    Point* t13 = new Point();
    rotate(p[1] -> curve_angle, &(p[1]->point_array[p[1]->end - 1]), p11, t11);
    rotate(p[1] -> curve_angle, &(p[1]->point_array[p[1]->end - 1]), p12, t12);
    rotate(p[1] -> curve_angle, &(p[1]->point_array[p[1]->end - 1]), p13, t13);

    glLineWidth(2);
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINE_LOOP);
            glVertex2f(t01->x, t01->y);
            glVertex2f(t02->x, t02->y);
            glVertex2f(t03->x, t03->y);
    glEnd();

    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINE_LOOP);
            glVertex2f(t11->x, t11->y);
            glVertex2f(t12->x, t12->y);
            glVertex2f(t13->x, t13->y);
    glEnd();

 /*   glLineWidth(5);
    if(p[0]->end == 100){
        glColor3f(0.0,1.0,0.0);
        glBegin(GL_LINES);
            glVertex2f(p[0]-> point_array[p[0]->end - 1].x, p[0]->point_array[p[0]->end - 1].y);
            glVertex2f(p[0]-> horizontalRange, 0);
        glEnd();
    }

    if(p[1]->end == 100){
        glColor3f(0.0,0.0,1.0);
        glBegin(GL_LINES);
            glVertex2f(p[1]->point_array[p[1]->end - 1].x, p[1]->point_array[p[1]->end - 1].y);
            glVertex2f(p[1]-> horizontalRange, 0);
        glEnd();
    }
*/
    glFlush();
    glutSwapBuffers();
}

void animation(int index){
    p[index]->end++;
    if(p[index]->end < 101){
        p[index]->point_array[p[index]->end - 1].x = (p[index]->velocity * p[index]->t * cos(p[index]->angle));
        p[index]->point_array[p[index]->end - 1].y = (p[index]->velocity * p[index]->t * sin(p[index]->angle)) - (0.5 * p[index]->g * p[index]->t * p[index]->t);
        p[index] -> v_y = p[index]->v_y - p[index]->g * p[index]->timeslice;
        p[index]->curve_angle = atan(p[index]->v_y/p[index]->v_x);
        //cout << index << "    "  << p[index]->curve_angle << "\n";
        p[index]->t += p[index]->timeslice;
        glutTimerFunc(p[index]->timeslice*500, animation, index);
        glutPostRedisplay();
    }
    else{
        delete(p[index]);
        p[index] = new Projectile();
        glutTimerFunc(0, animation, index);
    }
}

int main(int argc, char **argv){

    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(1000,1000);
    glutInitWindowPosition(0,0);
    glutCreateWindow("My_Projectile");

    p[0] = new Projectile();
    p[1] = new Projectile();

    glutDisplayFunc(myDisplayFunction);

    glutTimerFunc(0, animation, 0);
    glutTimerFunc(0, animation, 1);

    glutMainLoop();
}
