#ifndef DELAUNAY_COMMANDRESULT_H
#define DELAUNAY_COMMANDRESULT_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Config.h"
#include "Dcel.h"
#include "defines.h"
#include "DisplayableFactory.h"
#include "Point.h"
#include "StoreService.h"

using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class CommandResult
{
protected:
    bool isSuccess;
    Status status;
    StoreService *storeService;
    vector<Displayable*> vDisplayable;

public:
    CommandResult(bool isSuccessIn, Status &statusIn, StoreService *storeServiceIn, vector<Displayable*> &vDisplayableIn) :
                                                                                        isSuccess(isSuccessIn),
                                                                                        status(statusIn),
                                                                                        storeService(storeServiceIn),
                                                                                        vDisplayable(vDisplayableIn) {};

    /**
     * @fn      updateStatus
     * @brief   Updates status based on result execution
     */
    void updateStatus()
    {
        if (wasSuccess())
        {
            storeService->getStatus()->set(status);
        }
    };

    /**
     * @fn      getItems
     * @brief   Get items to display
     *
     * @param   vDisplayableOut    (OUT) Vector that contains elements to display
     */
    virtual void getItems(vector<Displayable*> &vDisplayableOut)
    {
        if (wasSuccess())
        {
            vDisplayableOut = vDisplayable;
        }
    };


    /*******************************************************************************************************************
    * Getter/Setter
    *******************************************************************************************************************/
    bool wasSuccess() const { return isSuccess; }
};


#endif //DELAUNAY_COMMANDRESULT_H
