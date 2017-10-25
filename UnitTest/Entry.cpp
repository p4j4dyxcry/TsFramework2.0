#include "../Code/Core/Object.h"
#include "../Code/Core/NamedObject.h"
#include <iostream>

using namespace TS;

void main()
{
    NamedObject object;
    object.SetName("who ?");
    std::cout << object.GetClassName() << std::endl;
    std::cout << object.GetName() << std::endl;
    while(1)
    {
        
    }
}
