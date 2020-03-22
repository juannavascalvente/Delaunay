//
// Created by delaunay on 21/3/20.
//

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel/DcelWriter.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool DcelWriter::write(const string &fileName, bool isBinary, const Dcel &dcel)
{
    bool write=true;		// Return value.

    // Check if write binary data.
    if (isBinary)
    {
        write = DcelWriter::writeBinary(fileName, dcel);
    }
    else
    {
        write = DcelWriter::write(fileName, dcel);
    }

    return(write);
}


bool DcelWriter::write(const string &fileName, const Dcel &dcel)
{
    bool written=true;	// Return value.
    ofstream ofs;		// Output file.

    // Open file.
    ofs.open(fileName.c_str(), ios::out);
    if (ofs.is_open())
    {
        // Get # points to read and points.
        ofs << dcel.nVertex << std::endl;
        for (size_t i=0; i<dcel.nVertex ;i++)
        {
            ofs << dcel.vertex[i];
        }

        // Get # edges to read and edges.
        ofs << dcel.nEdges << std::endl;
        for (size_t i=0; i<dcel.nEdges ;i++)
        {
            ofs << dcel.edges[i];
        }

        // Get # faces to read and faces.
        ofs << dcel.nFaces << std::endl;
        for (size_t i=0; i<dcel.nFaces ;i++)
        {
            ofs << dcel.faces[i] << std::endl;
        }

        // Close file.
        ofs.close();
    }
    // Error opening file.
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
        written = false;
    }

    return(written);
}


bool DcelWriter::writeBinary(const string &fileName, const Dcel &dcel)
{
    bool written=true;	    // Return value.
    ofstream ofs;		    // Output file.

    // Open file.
    ofs.open(fileName.c_str(), ios::out);
    if (ofs.is_open())
    {
        // Computing amount of data to write.
        unsigned long size = sizeof(Dcel) + sizeof(Vertex)*dcel.sizeVertex + sizeof(Edge)*dcel.sizeEdges + sizeof(Face)*dcel.sizeFaces;

        // Allocate buffer.
        char *buffer = new char[size];

        // Common fields.
        char *ptr = buffer;
        memcpy(ptr++, &dcel.created, sizeof(bool));
        memcpy(ptr++, &dcel.incremental, sizeof(bool));

        // Copy vertex data.
        memcpy(ptr, &dcel.nVertex, sizeof(int));
        ptr = ptr + sizeof(int);
        memcpy(ptr, &dcel.sizeVertex, sizeof(int));
        ptr = ptr + sizeof(int);

        unsigned long n = sizeof(Vertex)*dcel.sizeVertex;
        memcpy(ptr, (char *) dcel.vertex, n);
        ptr = ptr + n;

        // Copy edges data.
        memcpy(ptr, &dcel.nEdges, sizeof(int));
        ptr = ptr + sizeof(int);
        memcpy(ptr, &dcel.sizeEdges, sizeof(int));
        ptr = ptr + sizeof(int);
        n = sizeof(Edge)*dcel.sizeEdges;
        memcpy(ptr, (char *) dcel.edges, n);
        ptr = ptr + n;

        // Copy faces data.
        memcpy(ptr, &dcel.nFaces, sizeof(int));
        ptr = ptr + sizeof(int);
        memcpy(ptr, &dcel.sizeFaces, sizeof(int));
        ptr = ptr + sizeof(int);
        n = sizeof(Face)*dcel.sizeFaces;
        memcpy(ptr, (char *) dcel.faces, n);

        // Write data.
        ofs.write(buffer, sizeof(char)*size);

        // Close file.
        ofs.close();

        // Deallocate data.
        delete[] buffer;
    }
    // Error opening file.
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
        written = false;
    }

    return(written);
}


bool DcelWriter::writePoints(const string &fileName, int nPoints, const Dcel &dcel)
{
    bool written=true;	// Return value.
    ofstream ofs;		// Output file.
    int	i=0;			// Loop counter.
    int	length=0;		// # points to write.

    // Open file.
    ofs.open(fileName.c_str(), ios::out);

    // Check file is opened.
    if (ofs.is_open())
    {
        // If nPoints INVALID then write all DCEL points.
        if (nPoints == INVALID)
        {
            length = dcel.nVertex;
        }
            // If not then write only nPoints points.
        else
        {
            length = nPoints;
        }

        // Points loop.
        ofs << length << " ";
        for (i=0; i<length ;i++)
        {
            dcel.vertex[i].printPoint(ofs);
        }

        // Close file.
        ofs.close();
    }
        // Error opening points file.
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, fileName);
        Logging::write(true, Error);
        written = false;
    }

    return(written);
}


void DcelWriter::print(std::ostream& out, const Dcel &dcel)
{
    // Points loop.
    std::cout << dcel.nVertex << std::endl;
    for (size_t i=0; i<dcel.nVertex ;i++)
    {
        dcel.vertex[i].print(std::cout);
    }

    // Edges loop.
    std::cout << dcel.nEdges << std::endl;
    for (size_t i=0; i<dcel.nEdges ;i++)
    {
        dcel.edges[i].print(std::cout);
    }

    // Faces loop.
    std::cout << dcel.nFaces << std::endl;
    for (size_t i=0; i<dcel.nFaces ;i++)
    {
        dcel.faces[i].print(std::cout);
    }
}
