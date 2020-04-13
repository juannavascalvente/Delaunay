#ifndef DELAUNAY_DISPLAYSERVICE_H
#define DELAUNAY_DISPLAYSERVICE_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <GL/glut.h>
#include "defines.h"
#include "DisplayableColor.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DisplayService
{
public:
    static void init(int argc, char **argv);

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
     * @fn      startLoop
     * @brief   Starts glu main loop
     */
    static void startLoop()
    {
        glutMainLoop();
    }

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
     * @fn      setDashedLines
     * @brief   Display dashed line depending on input parameter
     *
     * @param isDashed
     */
    static void setDashedLines(bool isDashed)
    {
        if (isDashed)
        {
            glLineStipple(5, 0xAAAA);
            glEnable(GL_LINE_STIPPLE);
        }
        else
        {
            glDisable(GL_LINE_STIPPLE);
        }
    };

    /**
     * Set color functions.
     */
    static void setColor(const DisplayableColor &color) {glColor3f(color.getRed(), color.getGreen(),color.getBlue()); };

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
