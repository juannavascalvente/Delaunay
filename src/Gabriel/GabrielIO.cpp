//
// Created by delaunay on 23/3/20.
//
/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "GabrielIO.h"


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
void GabrielIO::print(std::ostream &out, const Gabriel &gabriel)
{
    // Write # elements.
    out << gabriel.getSize() << endl;

    for (size_t i=0; i<gabriel.getSize() ;i++)
    {
        out << gabriel.at(i) << " ";
    }
}


bool GabrielIO::read(const string &strFileName, Gabriel &gabriel)
{
    bool 		read=false;		// Return value.
    ifstream 	ifs;			// Input file.
    int 		size=0;			// # elements to read.
    int			i=0;			// Loop counter.
    bool		value=false;	// Value read.

    // Open file.
    ifs.open(strFileName.c_str(), ios::in);

    // Check file is opened.
    if (ifs.is_open())
    {
        // Read # nodes.
        ifs >> size;

        // Initialize graph.
        gabriel.resize( size, false);

        // Nodes main loop.
        for (i=0; i<gabriel.getSize() ;i++)
        {
            ifs >> value;
            if (value)
            {
                gabriel.set(i);
            }
        }
    }
#ifdef DEBUG_GABRIEL_READ
    gabriel.print(std::cout);
#endif

    return(read);
}


bool GabrielIO::write(const string &strFileName, const Gabriel &gabriel)
{
    bool written=false;		// Return value.
    ofstream ofs;			// Output file.

    // Check Gabriel graph allocated.
    if (gabriel.valid)
    {
        // Open file.
        ofs.open(strFileName.c_str(), ios::out);

        // Check file is opened.
        if (ofs.is_open())
        {
            // Write elements.
            GabrielIO::print(ofs, gabriel);
            written = true;
        }
    }

    return(written);
}


bool GabrielIO::readBinary(const string &strFileName, Gabriel &gabriel)
{
    bool read=false;		// Return value.
    ifstream ifs;			// Input file.
    int	size=0;				// Buffer size.
    char *buffer;			// Buffer data.

    // Check Gabriel graph allocated.
    if (gabriel.valid)
    {
        // Open file.
        ifs.open(strFileName.c_str(), std::ifstream::binary);

        // Check file is opened.
        if (ifs.is_open())
        {
            buffer = new char[sizeof(int)];
            ifs.read( buffer, sizeof(int));
            memcpy( &size, buffer, sizeof(int));
            delete[] buffer;

            // Allocate Gabriel graph data.
            gabriel.resize( size, false);

            // Read data.
            buffer = new char[sizeof(bool)*size];
            ifs.read( buffer, sizeof(bool)*size);
            memcpy( gabriel.validEdge, buffer, sizeof(bool)*size);
            delete[] buffer;
            read = gabriel.valid;
        }
    }

#ifdef DEBUG_GABRIEL_READ
    gabriel.print(std::cout);
#endif

    return(read);
}


bool GabrielIO::writeBinary(const string &strFileName, const Gabriel &gabriel)
{
    bool written=false;		// Return value.
    ofstream ofs;			// Output file.
    unsigned long size=0;	// Buffer size.
    int	nElements=0;		// # elements in Gabriel graph.
    char* buffer=nullptr;	// Buffer data.

    // Check Gabriel graph allocated.
    if (gabriel.valid)
    {
        // Open file.
        ofs.open(strFileName.c_str(), std::ofstream::binary);

        // Check file is opened.
        if (ofs.is_open())
        {
            size = (gabriel.getSize()*sizeof(bool)) + sizeof(int);
            buffer = new char[size];
            nElements = gabriel.getSize();
            memcpy( buffer, &nElements, sizeof(int));
            memcpy( &buffer[4], gabriel.validEdge, sizeof(bool)*gabriel.getSize());
            ofs.write( buffer, sizeof(bool)*size);

            written = true;
            delete[] buffer;
        }
    }

    return(written);
}
