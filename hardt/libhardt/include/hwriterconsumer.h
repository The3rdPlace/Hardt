#ifndef __HWRITERCONSUMER_H
#define __HWRITERCONSUMER_H

#include <utility>

#include "hobject.h"

/**
    Consume a writer further down the chain
*/
template <class T>
class HWriterConsumer: public HObject
{
    private:

        std::string _id;

    public:

        /** Construct a new HObject */
        explicit HWriterConsumer<T>(std::string id):
            _id(std::move(id)) {};

        std::string GetId() { return _id; }

        /** This function must be implemented if you inherit this interface */
        virtual void SetWriter(HWriter<T>* writer) = 0;

        /** Return a pointer to this consumer */
        HWriterConsumer<T>* Consumer()
        {
            return (HWriterConsumer<T>*) this;
        }
};

#endif