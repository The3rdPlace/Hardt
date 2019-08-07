#ifndef __HWRITERCONSUMER_H
#define __HWRITERCONSUMER_H

/**
    Consume a writer further down the chain
*/
template <class T>
class HWriterConsumer : public HObject
{
    public:

        virtual void SetWriter(HWriter<T>* writer) = 0;

        HWriterConsumer<T>* Consumer()
        {
            return (HWriterConsumer<T>*) this;
        }
};

#endif