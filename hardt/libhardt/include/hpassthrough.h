#ifndef __HPASSTHROUGH_H
#define __HPASSTHROUGH_H

/**
    A filter providing strict passthrough feeding of the signal.
    Can be used if you want to define a component in a chain that is
    to be replaced by a more usable component later on, but mostly
    used if you want to attach a probe to a signal somewhere in the
    chain where there is no other component that accepts a probe.
*/
template <class T>
class HPassThrough : public HFilter<T>
{
    public:

        /** Construct a new HPassThrough object that writes to a writer */
        HPassThrough(std::string id, HWriter<T>* writer, size_t blocksize);

        /** Construct a new HPassThrough object that registers with an upstream writer */
        HPassThrough(std::string id, HWriterConsumer<T>* consumer, size_t blocksize);

        /** Construct a new HPassThrough object that reads from a reader */
        HPassThrough(std::string id, HReader<T>* reader, size_t blocksize);

        /** Default destructor */
        ~HPassThrough();

        /** Run a block of samples through the passthrough filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);
};

#endif