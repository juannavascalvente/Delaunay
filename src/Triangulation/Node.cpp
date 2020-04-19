/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Node.h"

#include <sstream>
#include <iostream>
using namespace std;


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
Node::Node(int p1, int p2, int p3, int face)
{
	// Initialize fields.
	vPointsId.clear();
    vPointsId.push_back(p1);
    vPointsId.push_back(p2);
    vPointsId.push_back(p3);
	this->face = face;
}


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


/***************************************************************************
* Name: 	print
* IN:		out			output stream
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	prints node information
***************************************************************************/
void Node::print(std::ostream& out)
{
	// Check if node is a leaf.
	if (this->isLeaf())
	{
		out << "Node is a leaf";
	}
	// Print node data.
	else
	{
        out << "Node has " << this->vChildren.size() << " children: ";
        for (auto value : vChildren)
        {
            out << value << " ";
        }
	}
    out << ". Node points: ";
    for (auto value : vPointsId)
    {
        out << value << " ";
    }
    out << ". Assigned face " << this->face;
}

/***************************************************************************
* Name: 	read
* IN:		in			in stream to read form.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	reads the node information from "in" stream.
***************************************************************************/
void Node::read(std::istream& in)
{
    int iValue;

	// Write node points.
	for (size_t i=0; i<NODE_POINTS; i++)
	{
		in >> iValue;
        vPointsId.push_back(iValue);
	}

	// Write node children.
	in >> iValue;
	for (size_t  i=0; i<iValue; i++)
	{
	    int iChild;
		in >> iChild;
        vChildren.push_back(iChild);
	}

	// Write node face.
	in >> this->face;
}

/***************************************************************************
* Name: 	write
* IN:		out			output stream to write to.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	writes the node information to "out" stream.
***************************************************************************/
void Node::write(std::ostream& out)
{
	int	i=0;		// Loop counter.

	// Write node points.
	for (i=0; i<NODE_POINTS; i++)
	{
		out << this->vPointsId.at(i) << " ";
	}

	// Write node children.
	out << this->vChildren.size() << " ";
	for (i=0; i<MAX_CHILDREN; i++)
	{
		out << this->vChildren.at(i) << " ";
	}

	// Write node face.
	out << this->face;
}

/***************************************************************************
* Name: 	toStr
* IN:		NONE
* OUT:		NONE
* RETURN:	node information as a string
* GLOBAL:	NONE
* Description: 	concat the node information as a string
***************************************************************************/
string Node::toStr()
{
	ostringstream oss;
	string text;

	// Build file name.
	this->print( oss);
	text = oss.str();

	return(text);
}
