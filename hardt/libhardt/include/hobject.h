#ifndef __HOBJECT_H
#define __HOBJECT_H

#include <string>
#include <utility>

/**
    Base class for (almost) all Hardt types.

    If you extend the Hardt framework by adding your own components,
    then either inherit an existing Hardt component, or at the very least
    this type.
*/
class HObject
{
    private:

        std::string _id;

    public:

        /**
         * Construct a new HObject
         *
         * @param id Element identifier
         */
        explicit HObject(std::string id) {
            _id = std::move(id);
        }

        /** Get the object id */
        std::string GetId() { return _id; }
};

#endif