#ifndef DELAUNAY_GRAPHWRITER_H
#define DELAUNAY_GRAPHWRITER_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Graph.h"

#include <string>
using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class GraphWriter
{
    /*******************************************************************************************************************
    * Private methods declarations
    *******************************************************************************************************************/
    static bool writeFlat(const string &strFileName, Graph &graph);
    static bool writeBinary(const string &strFileName, Graph &graph);

public:
    /*******************************************************************************************************************
    * Public methods declarations
    *******************************************************************************************************************/
    /**
     * @fn                  write
     * @brief               Writes graph to output file in flat format or binary depending on isBinary value
     *
     * @param strFileName   (IN)    File name to write to
     * @param isBinary      (IN)    Binary format flag
     * @param graph         (IN)    Graph to write to file
     * @return              true if written successfully
     *                      false otherwise
     */
    static bool write(const string &strFileName, bool isBinary, Graph &graph);


    /**
     * @fn          print
     * @brief       Print graph data
     *
     * @param out       (IN)    Stream where data is printed
     * @param graph     (IN)    Graph to print
     */
    static void print(std::ostream &out, Graph &graph);
};


#endif //DELAUNAY_GRAPHWRITER_H