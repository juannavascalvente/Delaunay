#ifndef DELAUNAY_STOREDATAREPOSITORY_H
#define DELAUNAY_STOREDATAREPOSITORY_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "StoreData.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class StoreDataRepository
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    StoreData *data;
    static StoreDataRepository *instance;

    /*******************************************************************************************************************
     * Private methods
     ******************************************************************************************************************/
    StoreDataRepository();

public:

    /*******************************************************************************************************************
     * Public methods
     ******************************************************************************************************************/
     /**
      * @fn         getInstance
      * @brief      Gets access to instance
      *
      * @return     Pointer to repository data
      */
    static StoreDataRepository *getInstance();

    /**
     * @fn         StoreData
     * @brief      Gets access to repository data
     *
     * @return      Pointer to data
     */
    StoreData* getData() { return getInstance()->data; };

    /**
      * @fn         deleteInstance
      * @brief      Deletes repository
     */
    static void deleteInstance();

    // Delete copy constructor and assignment operator as private to avoid cloning object
    StoreDataRepository(const StoreDataRepository&) = delete;
    StoreDataRepository& operator=(const StoreDataRepository&) = delete;
};


#endif //DELAUNAY_STOREDATAREPOSITORY_H
