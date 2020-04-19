/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "NodeIO.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
/***************************************************************************
* Name: 	print
* IN:		out			output stream
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	prints node information
***************************************************************************/
void NodeIO::print(Node &node, std::ostream& out)
{
    // Check if node is a leaf.
    if (node.isLeaf())
    {
        out << "Node is a leaf";
    }
        // Print node data.
    else
    {
        out << "Node has " << node.vChildren.size() << " children: ";
        for (auto value : node.vChildren)
        {
            out << value << " ";
        }
    }
    out << ". Node points: ";
    for (auto value : node.vPointsId)
    {
        out << value << " ";
    }
    out << ". Assigned face " << node.face;
}

/***************************************************************************
* Name: 	read
* IN:		in			in stream to read form.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	reads the node information from "in" stream.
***************************************************************************/
void NodeIO::read(Node &node, std::istream& in)
{
    int iValue;

    // Write node points.
    for (size_t i=0; i<NODE_POINTS; i++)
    {
        in >> iValue;
        node.vPointsId.push_back(iValue);
    }

    // Write node children.
    in >> iValue;
    for (size_t  i=0; i<iValue; i++)
    {
        int iChild;
        in >> iChild;
        node.vChildren.push_back(iChild);
    }

    // Write node face.
    in >> node.face;
}

/***************************************************************************
* Name: 	write
* IN:		out			output stream to write to.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	writes the node information to "out" stream.
***************************************************************************/
void NodeIO::write(Node &node, std::ostream& out)
{
    int	i=0;		// Loop counter.

    // Write node points.
    for (i=0; i<NODE_POINTS; i++)
    {
        out << node.vPointsId.at(i) << " ";
    }

    // Write node children.
    out << node.vChildren.size() << " ";
    for (i=0; i<MAX_CHILDREN; i++)
    {
        out << node.vChildren.at(i) << " ";
    }

    // Write node face.
    out << node.face;
}

/***************************************************************************
* Name: 	toStr
* IN:		NONE
* OUT:		NONE
* RETURN:	node information as a string
* GLOBAL:	NONE
* Description: 	concat the node information as a string
***************************************************************************/
string NodeIO::toStr(Node &node)
{
    ostringstream oss;
    string text;

    // Build file name.
    NodeIO::print(node, oss);
    text = oss.str();

    return text;
}