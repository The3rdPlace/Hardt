#ifndef __HOBJECT_H
#define __HOBJECT_H

#include <string>

/**
    Base class for (almost) all Hardt types.

    If you extend the Hardt framework by adding your own components,
    then either inherit an existing Hardt component, or at the very least
    this type.
*/
class HObject
{
    private:

        //std::string _id;

    public:

        /** Construct a new HObject */
        //explicit HObject(std::string id);

        /** Destruct a HObject */
        //~HObject();

        /** Get the object id */
        virtual std::string GetId() = 0;
};

#endif