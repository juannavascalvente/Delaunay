//
// Created by delaunay on 25/3/20.
//

#ifndef DELAUNAY_DISPLAYABLE_H
#define DELAUNAY_DISPLAYABLE_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DisplayService.h"
#include "Point.h"
#include "Dcel.h"

#include <vector>
using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Displayable
{
    void (* f)();

    virtual void runConfig()
    {
        cout << "RUNNING CONFIG " << endl;
        if ((*f))
        {
            (*f)();
        }
    }
public:
    Displayable() : f(nullptr) {};
    virtual ~Displayable() = default;
    virtual void display() { runConfig(); };

    virtual void setColor(void (* fIn)()) { f = fIn; };
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DispPoint : public Displayable
{
    Point<TYPE> point;

public:
    explicit DispPoint(Point<TYPE> &inPoint) : point(inPoint) {};

    void display() override
    {
        Displayable::display();

        // Draw point.
        DisplayService::startPoints();
        glVertex2f(point.getX(), point.getY());
        DisplayService::finish();
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DispPointsSet : public Displayable
{
    vector<Point<TYPE>> vPoints;
public:
    explicit DispPointsSet(vector<Point<TYPE>> &vPointsIn) : vPoints(vPointsIn) {};

    void display() override
    {
        Displayable::display();

        // Loop to draw points.
        for (auto point : vPoints)
        {
            DisplayService::startPoints();
            DisplayService::display(point.getX(), point.getY());
            DisplayService::finish();
        }
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DispPolygon : public Displayable
{
    vector<Point<TYPE>> vPoints;
public:
    explicit DispPolygon(vector<Point<TYPE>> &vPointsIn) : vPoints(vPointsIn) {};

    void display() override
    {
        Displayable::display();

        // Computes distance between every pair of points.
        for (size_t i=0; i<vPoints.size()-1 ;i++)
        {
            // Draw line
            DisplayService::startLine();
            DisplayService::display(vPoints.at(i).getX(), vPoints.at(i).getY());
            DisplayService::display(vPoints.at(i+1).getX(), vPoints.at(i+1).getY());
            DisplayService::finish();
        }

        // Draw line
        DisplayService::startLine();
        DisplayService::display(vPoints.at(vPoints.size()-1).getX(), vPoints.at(vPoints.size()-1).getY());
        DisplayService::display(vPoints.at(0).getX(), vPoints.at(0).getY());
        DisplayService::finish();
    }
};


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DispDcel : public Displayable
{
    Dcel *dcel;
public:
    explicit DispDcel(Dcel *dcelIn) : dcel(dcelIn) {};

    void display() override
    {
        Displayable::display();

        // Start
        DisplayService::startPoints();

        // Draw all edges.
        for (size_t edgeIndex=0; edgeIndex<dcel->getNEdges() ;edgeIndex++)
        {
            // Skip triangle if any vertex is P_MINUS_1 or P_MINUS_2.
            if (!dcel->hasNegativeVertex((int) edgeIndex+1))
            {
                // Not draw edges whose twin has been already drawn.
                if (edgeIndex < dcel->getTwin(edgeIndex)-1)
                {
                    // Get origin vertex of edge.
                    Point<TYPE> *vertex1 = dcel->getRefPoint(dcel->getOrigin(edgeIndex)-1);

                    // Get destination vertex of edge.
                    Point<TYPE> *vertex2 = dcel->getRefPoint(dcel->getOrigin(dcel->getTwin(edgeIndex)-1)-1);

                    // Start line
                    DisplayService::startLine();

                    // Draw first point
                    DisplayService::display(vertex1->getX(), vertex1->getY());

                    // Draw second point
                    DisplayService::display(vertex2->getX(), vertex2->getY());

                    // End line
                    DisplayService::finish();
                }
            }
        }
    }
};

#endif //DELAUNAY_DISPLAYABLE_H
