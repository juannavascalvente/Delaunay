#ifndef DELAUNAY_COMMANDRESULT_H
#define DELAUNAY_COMMANDRESULT_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Config.h"
#include "Dcel.h"
#include "defines.h"
#include "Displayable/DisplayableFactory.h"
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
    vector<Displayable*> vDisplayable;

public:
    CommandResult(bool isSuccessIn, vector<Displayable*> &vDisplayableIn) : isSuccess(isSuccessIn), vDisplayable(vDisplayableIn) {};

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
