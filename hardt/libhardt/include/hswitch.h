#ifndef __HSWITCH_H
#define __HSWITCH_H

#include "hreader.h"
#include "hwriter.h"

/**
    Implements a switch that can hold a number of object that can be switched
    between, say 3 filters where the switch position determines which filter
    gets the reads or writes passed to/from this component

    The switch also has a zero-position called "bypass" (index 0), in
    which all writes or reads are passed through directly
*/
template <class T>
class HSwitch: public HReader<T>, public HWriter<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;

        int _position;

        std::vector<HObject*> _components;

    public:

        /** Construct a new HSwitch that handle writers */
        HSwitch(HWriter<T>* writer);

        /** Construct a new HSwitch that handle writers and registers with an upstream writer */
        HSwitch(HWriterConsumer<T>* consumer);

        /** Construct a new HFilter that handle readers */
        HSwitch(HReader<T>* reader);

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer)
        {
            _writer = writer;
        }

    public:

        /** Default destructor */
        ~HSwitch();

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Read a block of samples */
        int Read(T* dest, size_t blocksize);

        /** Set switch position */
        void SetPosition(int position);

        /** Get switch position */
        int GetPosition();

        /** Add reader */
        void Add(HReader<T>* reader);

        /** Add writer */
        void Add(HWriter<T>* writer);
};

#endif