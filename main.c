#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "vector.h"

#define WIDTH 400
#define HEIGHT 400
#define STEP 0.0001
#define NUMCURVES 12

// these are the vector graphics: the channel logo and a bubbling flask with fumes
int curves[2][NUMCURVES][4][2] = {
   {
        {{50, 100},{50, 100},{50, 300},{50, 300}},
        {{50, 100},{50, 100},{200, 25},{200, 25}},
        {{200, 25},{200, 25},{350, 100},{350, 100}},
        {{50, 300},{50, 300},{200, 375},{200, 375}},
        {{200, 375},{200, 375},{350, 300},{350, 300}},
        {{350, 100},{350, 100},{350, 300},{350, 300}},
        {{75, 105},{75, 105},{75, 295},{75, 295}},
        {{200, 55},{200, 55},{325, 120},{325, 120}},
        {{200, 340},{200, 340},{325, 285},{325, 285}},
        {{150, 125},{150, 125},{150, 125},{250, 125}},
        {{250, 125},{250, 125},{150, 275},{150, 275}},
        {{150, 275},{150, 275},{150, 275},{250, 275}}
   },
   {
        {{210, 100},{210, 100},{210, 100},{300, 304}},
        {{100, 300},{100, 300},{300, 300},{300, 300}},
        {{125, 250},{125, 250},{275, 250},{275, 250}},
        {{189, 107},{189, 107},{189, 75},{189, 75}},
        {{212, 107},{212, 107},{212, 75},{212, 75}},
        {{189, 75},{150, 65},{250, 65},{212, 75}},
        {{140, 250},{140, 220},{170, 220},{170, 250}},
        {{200, 250},{200, 240},{210, 240},{210, 250}},
        {{100, 300},{100, 300},{100, 300},{190, 100}},
        {{180, 60},{200, 60},{170, 10},{200, 10}},
        {{190, 60},{210, 60},{180, 10},{210, 10}},
        {{200, 60},{230, 60},{200, 10},{220, 10}}
   }
};
Vector vectors[2][NUMCURVES][4];

double lt = 0;

void lerp(Vector *out, double t, Vector a, Vector b) {
    // linearly interpolate between 2 vectors a and b with interpolation parameter t
    out->x = (1 - t) * a.x + (t * b.x);
    out->y = (1 - t) * a.y + (t * b.y);
}

void paintBezier(Vector v0, Vector v1, Vector v2, Vector v3) {
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    // draw a Bezier curve between vectors using de Casteljau's algorithm
    Vector cv, p0, p1, p2, p3, p4;
    double t = 0;
    while (t < 1) {
        // these give the first set of lines
        lerp(&p0, t, v0, v1);
        lerp(&p1, t, v1, v2);
        lerp(&p2, t, v2, v3);
        // now those are interpolated into a quadratic Bezier curve
        lerp(&p3, t, p0, p1);
        lerp(&p4, t, p1, p2);
        // and the quadratics into cubics
        lerp(&cv, t, p3, p4);
        glVertex2d(cv.x, cv.y);
        t += 0.01;
    }
    glEnd();
}

void copyVectors() {
    // copy the image from coordinate pairs to vectors
    for (int i = 0; i < 2; i ++) {
        for (int j = 0; j < NUMCURVES; j ++) {
            for (int k = 0; k < 4; k ++) {
                initVector(&vectors[i][j][k], curves[i][j][k][0], curves[i][j][k][1]);
            }
        }
    }
}

void loop(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    Vector v1, v2, v3, v4;
    double lval = sin(lt) * sin(lt);
    for (int j = 0; j < NUMCURVES; j ++) {
        // interpolate between vectors and draw the curve
        lerp(&v1, lval, vectors[0][j][0], vectors[1][j][0]);
        lerp(&v2, lval, vectors[0][j][1], vectors[1][j][1]);
        lerp(&v3, lval, vectors[0][j][2], vectors[1][j][2]);
        lerp(&v4, lval, vectors[0][j][3], vectors[1][j][3]);
        paintBezier(v1, v2, v3, v4);
    }
    lt += STEP; // add or subtract from the counter based on the indicator
    glFlush();
}

void idle() { glutPostRedisplay(); }

int main (int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Bezier Curve");

    copyVectors();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glClearColor(0.87058, 0.75686, 1.0, 1.0);
    glColor3f(0.49019, 0.43921, 0.72941);

    glutDisplayFunc(loop);
    glutIdleFunc(&idle);
    glutMainLoop();
}
