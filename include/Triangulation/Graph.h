#ifndef INCLUDE_GRAPH_H_
#define INCLUDE_GRAPH_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Node.h"

#include <cstring>
#include <vector>


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Graph
{
    /*******************************************************************************************************************
    * Private class members
    *******************************************************************************************************************/
    vector<int> vFaceNode;
    vector<Node> vNodes;

    friend class GraphWriter;

public:
    /*******************************************************************************************************************
    * Public methods declarations
    *******************************************************************************************************************/
	Graph() = default;
	~Graph() = default;

    Graph& operator=(const Graph &g)
    {
        if (this != &g)
        {
            this->vFaceNode = g.vFaceNode;
            this->vNodes = g.vNodes;
        }

        return *this;
    }

	int getSize() {return this->vNodes.size(); };
    Node* getRefNode(int nodeId) { return &this->vNodes.at(nodeId); };
    int getNodeAssigned(int face);
    int getNChildren(int nodeId) { return this->vNodes.at(nodeId).getNChildren(); };
    int getiChild(int nodeId, int iChild) { return this->vNodes.at(nodeId).getiChild(iChild); };
    void getVertices(int nodeIndex, int &index1, int &index2, int &index3);

	// Graph manipulation
	void insert(Node &node);
	void update(int index, Node *node);
    int isLeaf(int nodeIndex) { return this->vNodes.at(nodeIndex).isLeaf(); };

	// Graph size functions.
	void reset();
};

#endif /* INCLUDE_GRAPH_H_ */
