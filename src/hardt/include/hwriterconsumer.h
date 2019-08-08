#ifndef __HWRITERCONSUMER_H
#define __HWRITERCONSUMER_H

/**
    Consume a writer further down the chain
*/
template <class T>
class HWriterConsumer : public HObject
{
    public:

        /** This function must be implemented if you inherit this interface */
        virtual void SetWriter(HWriter<T>* writer) = 0;

        /** Return a pointer to this consumer */
        HWriterConsumer<T>* Consumer()
        {
            return (HWriterConsumer<T>*) this;
        }
};

#endif