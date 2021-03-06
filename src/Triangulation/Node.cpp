/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Node.h"

#include <string>
#include <stdexcept>
using namespace std;


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
void Node::setChildren(int id1, int id2)
{
    vChildren.clear();
    vChildren.push_back(id1);
    vChildren.push_back(id2);
}


void Node::setChildren(int id1, int id2, int id3)
{
    vChildren.clear();
    vChildren.push_back(id1);
    vChildren.push_back(id2);
    vChildren.push_back(id3);
}


int Node::getiChild(int index)
{
    if (vChildren.empty() || (index > (getNChildren()-1)))
    {
        string srtMsg = &"Error accessing node child " [ (index+1)];
        throw std::runtime_error(srtMsg);
    }

    return vChildren.at(index);
}


int Node::getiPoint(int index)
{
    if (vPointsId.empty() || (index > NODE_POINTS))
    {
        string srtMsg = &"Error accessing node point " [ (index+1)];
        throw std::runtime_error(srtMsg);
    }

    return vPointsId.at(index);
}
