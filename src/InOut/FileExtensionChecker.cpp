/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "FileExtensionChecker.h"


/***********************************************************************************************************************
* Static members
***********************************************************************************************************************/
vector<string> FileExtensionChecker::vBinaryExtensions={".bin",".dat",".raw"};


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
/**
 * @fn              isBinary
 * @brief           Checks if input string ends with binary extension suffix
 *
 * @param str       (IN) String to check
 * @return          true if input string ends with binary suffix
 *                  false otherwise
 */
bool FileExtensionChecker::isBinary(const string &str)
{
    bool isBinaryExtension=false;

    // Check all extensions
    for (const auto& suffix : vBinaryExtensions)
    {
        if (str.size() >= suffix.size())
        {
            isBinaryExtension = (str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0);
            break;
        }
    }

    return isBinaryExtension;
}