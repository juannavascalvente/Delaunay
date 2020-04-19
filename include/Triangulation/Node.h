#ifndef INCLUDE_NODE_H_
#define INCLUDE_NODE_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "defines.h"

#include <vector>
using namespace std;


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define TWO 			2
#define MAX_CHILDREN	3
#define NODE_POINTS		3


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Node
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    vector<int> vChildren;          // Children nodes IDs
    vector<int> vPointsId;          // Node points IDs
	int	face;						// Face id.

	friend class NodeIO;
public:

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
	Node() : face(INVALID) {};
	Node(int p1, int p2, int p3, int face);
    Node(int p1, int p2, int p3, int iChild1, int iChild2, int iChild3, int faceIn) : face(faceIn)
    {
        // Set children
        vChildren.push_back(iChild1);
        vChildren.push_back(iChild2);
        vChildren.push_back(iChild3);

        // Set points
        vPointsId.push_back(p1);
        vPointsId.push_back(p2);
        vPointsId.push_back(p3);
    };
    Node(int p1, int p2, int p3, int iChild1, int iChild2, int faceIn) : face(faceIn)
    {
        // Set children
        vChildren.push_back(iChild1);
        vChildren.push_back(iChild2);

        // Set points
        vPointsId.push_back(p1);
        vPointsId.push_back(p2);
        vPointsId.push_back(p3);
    };

    /*******************************************************************************************************************
    * Getters/Setters
    *******************************************************************************************************************/
	inline int getNChildren() { return vChildren.size(); };
	inline int getFace() { return(this->face); };
	inline bool isLeaf() { return vChildren.empty(); };
	inline bool isSplitted() { return (getNChildren() == TWO); };
    int getiChild(int index);
    int getiPoint(int index);
    void setChildren(int id1, int id2);
    void setChildren(int id1, int id2, int id3);
};

#endif /* INCLUDE_NODE_H_ */
