//
// Created by delaunay on 21/3/20.
//

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel/DcelReader.h"
#include "defines.h"
#include "Logging.h"
#include "Point.h"

#include <fstream>
#include <Dcel/Dcel.h>


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define HEADERS 10


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool DcelReader::read(const string &fileName, bool isBinary, Dcel &dcel)
{
    bool read;		// Return value.

    // Check if read binary data.
    if (isBinary)
    {
        read = DcelReader::readBinary(fileName, dcel);
    }
    else
    {
        read = DcelReader::readDcel(fileName, dcel);
    }

    return(read);
}


/***************************************************************************
* Name: 	readDcel
* IN:		fileName		name of the file to read.
* OUT:		NONE
* RETURN:	true			if read
* 			false			i.o.c.
* GLOBAL:	NONE
* Description:	reads the dcel data from a file.
***************************************************************************/
bool DcelReader::readDcel(const string &fileName, Dcel &dcel)
{
    bool read=true;		// Return value.
//    ifstream ifs;		// Input file.
//    Edge 	edge;		// Edge to read.
//    Face 	face;		// Face to read.
//
//    // Open file.
//    ifs.open(fileName.c_str(), ios::in);
//    if (ifs.is_open())
//    {
//        // Read points.
//        if (DcelReader::readVertexSet(ifs, dcel))
//        {
//            // Get # edges to read.
//            int size=0;
//            ifs >> size;
//            dcel.setSizeEdges(size);
//
//            if (dcel.getSizeEdges() > 0)
//            {
//                for (int i=0; i<dcel.getSizeEdges() ;i++)
//                {
//                    ifs >> edge;
//                    dcel.addEdge(&edge);
//                }
//
//                // Get # faces to read.
//                ifs >> size;
//                dcel.setSizeFaces(size);
//                if (dcel.getSizeFaces() > 0)
//                {
//                    for (int i=0; i<dcel.getSizeFaces() ;i++)
//                    {
//                        ifs >> face;
//                        dcel.addFace(&face);
//                    }
//
//                    // Close file.
//                    ifs.close();
//                }
//                else
//                {
//                    Logging::buildText(__FUNCTION__, __FILE__, "Number of faces is NOT positive: ");
//                    Logging::buildText(__FUNCTION__, __FILE__, dcel.getSizeFaces());
//                    Logging::write(true, Error);
//                    read = false;
//                }
//            }
//            else
//            {
//                Logging::buildText(__FUNCTION__, __FILE__, "Number of edges is NOT positive: ");
//                Logging::buildText(__FUNCTION__, __FILE__, dcel.getSizeEdges());
//                Logging::write(true, Error);
//                read = false;
//            }
//        }
//    }
//    // Error opening file.
//    else
//    {
//        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
//        Logging::buildText(__FUNCTION__, __FILE__, fileName);
//        Logging::write(true, Error);
//        read = false;
//    }

    return(read);
}


bool DcelReader::readPoints(const string &fileName, bool fromFlatFile, Dcel &dcel)
{
    bool success=true;		// Return value.

    // Read points from flat points file.
    if (fromFlatFile)
    {
        success = DcelReader::readFlatPoints(fileName, dcel);
    }
        // Read points from DCEL points file.
    else
    {
        success = DcelReader::readDcelPoints(fileName, dcel);
    }

    return(success);
}


/***********************************************************************************************************************
* Private methods definitions
***********************************************************************************************************************/
/***************************************************************************
* Name: 	readVertexSet
* IN:		ifs				file stream
* OUT:		NONE
* RETURN:	true			if read
* 			false			i.o.c.
* GLOBAL:	NONE
* Description:	reads the set of points from the ifs file stream and resizes
* 				the DCEL allocating memory for vertex, edges and faces.
***************************************************************************/
bool DcelReader::readVertexSet(ifstream &ifs, Dcel &dcel)
{
    int	 i=0;			// Loop counter.
    int  nElements=0;	// # elements to read.
    bool success=true;	// Return value.
//    Vertex 	vertex;		// Vertex to read.
//
//    // Get # points to read.
//    ifs >> nElements;
//    if (nElements > 0)
//    {
//        dcel.resize(nElements);
//
//        // Points loop.
//        for (i=0; i<dcel.getSizeVertex() ;i++)
//        {
//            ifs >> vertex;
//            dcel.addVertex(&vertex);
//        }
//    }
//    else
//    {
//        Logging::buildText(__FUNCTION__, __FILE__, "Number of points is NOT positive: ");
//        Logging::buildText(__FUNCTION__, __FILE__, nElements);
//        Logging::write(true, Error);
//        success = false;
//    }

    return(success);
}


/***************************************************************************
* Name: 	readFlatPoints
* IN:		fileName	file name to write data into.
* OUT:		NONE
* RETURN:	true 		if file read.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	read the set of points formatted from input file. The format
* 				is NPOINTS x1 y1 x2 y2 x3 y3........xN yN
***************************************************************************/
bool DcelReader::readFlatPoints(const string &fileName, Dcel &dcel)
{
    bool success=true;	// Return value.
    ifstream ifs;		// Output file.
    int	i=0;			// Loop counter.
    int	nVertex=0;		// # vertex read from file.
    Point<TYPE>	p;		// Point read.

//    // Open file.
//    ifs.open(fileName.c_str(), ios::in);
//
//    // Check file is opened.
//    if (ifs.is_open())
//    {
//        // Get # points to read.
//        ifs >> nVertex;
//        if (nVertex > 0)
//        {
//            dcel.resize(nVertex);
//
//            // Points loop.
//            for (i=0; i<dcel.getSizeVertex() ;i++)
//            {
//                ifs >> p;
//                dcel.addVertex(&p, INVALID);
//            }
//        }
//        else
//        {
//            Logging::buildText(__FUNCTION__, __FILE__, "Number of vertex must be positive and is: ");
//            Logging::buildText(__FUNCTION__, __FILE__, nVertex);
//            Logging::write(true, Error);
//            success = false;
//        }
//        // Close file.
//        ifs.close();
//    }
//        // Error opening points file.
//    else
//    {
//        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
//        Logging::buildText(__FUNCTION__, __FILE__, fileName);
//        Logging::write(true, Error);
//        success = false;
//    }

    return(success);
}


/***************************************************************************
* Name: 	readDcelPoints
* IN:		fileName	file name to write data into.
* OUT:		NONE
* RETURN:	true 		if file read.
* 			false		i.o.c.
* GLOBAL:	NONE
* Description: 	reads the set of points from a DCEL format file.
***************************************************************************/
bool DcelReader::readDcelPoints(const string &fileName, Dcel &dcel)
{
    bool success=true;	// Return value.
    ifstream ifs;		// Output file.

    // Open file.
    ifs.open(fileName.c_str(), ios::in);

    // Check file is opened.
    if (ifs.is_open())
    {
        // Read points.
        if (!DcelReader::readVertexSet(ifs, dcel))
        {
            Logging::buildText(__FUNCTION__, __FILE__, "Error reading set of points from ");
            Logging::buildText(__FUNCTION__, __FILE__, fileName);
            Logging::write(true, Error);
        }

        // Close file.
        ifs.close();
    }

    return(success);
}


/***************************************************************************
* Name: 	readBinary
* IN:		fileName		input file name.
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	reads DCEL data from input stream in binary format.
***************************************************************************/
bool DcelReader::readBinary(const string &fileName, Dcel &dcel)
{
    bool read=true;				// Return value.
    ifstream ifs;				// Input file.
    unsigned long size=0;					// Buffer size.
    char vectorsData[HEADERS];	// Vectors header data.
    char *buffer, *ptr;			// Buffer data.

    // Open file.
    ifs.open(fileName.c_str(), std::ifstream::binary);
    if (ifs.is_open())
    {
        // Read common fields and vertex vector size and nElements.
        ifs.read(vectorsData, HEADERS);

        // Extract data.
//        ptr = vectorsData;
//        memcpy(&dcel.created, ptr++, sizeof(bool));
//        memcpy(&dcel.incremental, ptr++, sizeof(bool));
//        memcpy(&dcel.nVertex, ptr, sizeof(int));
//        ptr = ptr + sizeof(int);
//        memcpy(&dcel.sizeVertex, ptr, sizeof(int));
//
//        // Allocate DCEL data and buffer.
//        dcel.resize(dcel.sizeVertex, false);
//        size = sizeof(int)*4 + sizeof(Vertex)*dcel.sizeVertex + sizeof(Edge)*dcel.sizeEdges + sizeof(Face)*dcel.sizeFaces;
//        buffer = new char[size];
//
//        // Read data.
//        ifs.read(buffer, size);
//
//        // Copy vertex data.
//        ptr = buffer;
//        memcpy((char *) dcel.vertex, ptr, sizeof(Vertex)*dcel.sizeVertex);
//        ptr = ptr + sizeof(Vertex)*dcel.sizeVertex;
//
//        // Copy edge data.
//        memcpy(&dcel.nEdges, ptr, sizeof(int));
//        ptr = ptr + sizeof(int);
//        memcpy(&dcel.sizeEdges, ptr, sizeof(int));
//        ptr = ptr + sizeof(int);
//        memcpy((char *) dcel.edges, ptr, sizeof(Edge)*dcel.sizeEdges);
//        ptr = ptr + sizeof(Edge)*dcel.sizeEdges;
//
//        // Copy faces data.
//        memcpy(&dcel.nFaces, ptr, sizeof(int));
//        ptr = ptr + sizeof(int);
//        memcpy(&dcel.sizeFaces, ptr, sizeof(int));
//        ptr = ptr + sizeof(int);
//        memcpy((char *) dcel.faces, ptr, sizeof(Face)*dcel.sizeFaces);

        // Close file.
        ifs.close();

        // Deallocate data.
        delete[] buffer;
    }
        // Error opening file.
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
        read = false;
    }

    return(read);
}
