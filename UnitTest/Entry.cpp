#include "../Code/Core/INamed.h"
#include "../Code/Core/Develop.h"

using namespace TS;

void main()
{
    INamed object;
    object.SetIName("who ?");

    TS_LOG(object);
    while(1)
    {
        
    }
}
