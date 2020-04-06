/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Config.h"
#include "Visual/Display/DisplayService.h"


/***********************************************************************************************************************
* Static functions
***********************************************************************************************************************/
static void refresh();


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
void DisplayService::init(int argc, char **argv)
{
    int minX, minY, maxX, maxY;		// Window dimension.

    // Initialize openGL.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1100, 750);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("GEOMETRICAL");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set background color.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    glutIdleFunc(refresh);

    // Set x, y and z axis size.
    Config::getScreenCoordinates(minX, minY, maxX, maxY);
    glOrtho(minX-100, maxX+100, minY-100, maxY+100, -1, 3);
}

void refresh()
{
    glutPostRedisplay();
}
