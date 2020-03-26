//
// Created by delaunay on 25/3/20.
//

#ifndef DELAUNAY_DISPLAYSERVICE_H
#define DELAUNAY_DISPLAYSERVICE_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <GL/glut.h>
#include "defines.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DisplayService
{
public:
    /**
     *
     */
    static void clear()  { glClear(GL_COLOR_BUFFER_BIT);} ;

    /**
     *
     */
    static void flush()  { glutSwapBuffers(); };

    /**
     *
     */
    static void startPoints() { glBegin(GL_POINTS); };

    /**
     *
     */
    static void startCircle() { glBegin(GL_LINE_LOOP); };

    /**
     *
     */
    static void startLine()   { glBegin(GL_LINE_STRIP); };

    /**
     *
     */
    static void finish() { glEnd(); };

    /**
     *
     * @param size
     */
    static void setPointSize(float size) { glPointSize(size);} ;

    /**
     *
     * @param size
     */
    static void setLineSize(float size) { glLineWidth(size);} ;

    /**
     * Set color functions.
     */
    static void setRed()        { glColor3f(1.0, 0.0, 0.0); };
    static void setGreen()      { glColor3f(0.0, 1.0, 0.0); };
    static void setBlue()       { glColor3f(0.0, 0.0, 1.0); };
    static void setLightBlue()  { glColor3f(0.0, 1.0, 1.0); };
    static void setYellow()     { glColor3f(1.0, 1.0, 0.0); };
    static void setWhite()      { glColor3f(1.0, 1.0, 1.0); };

    /**
     * @fn          display
     * @brief       Display point
     *
     * @param x     x coordinate
     * @param y     y coordinate
     */
    static void display(TYPE x, TYPE y)
    {
        glVertex2f(x, y);
    }
};


#endif //DELAUNAY_DISPLAYSERVICE_H
