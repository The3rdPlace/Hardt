#ifndef __HSWITCH_H
#define __HSWITCH_H

#include "hreader.h"
#include "hwriter.h"

#include <vector>

/**
    Implements a switch that can hold a number of object that can be switched
    between, say 3 filters where the switch position determines which filter
    gets the reads or writes passed to/from this component

    The switch also has a zero-position called "bypass" (index 0), in
    which all writes or reads are passed through directly

    Allthough you can warp the intended usage pattern by supplying
    output- or nullwriters , this component is really meant to be used as a
    pass-through device with filters or other types of sample-manipulating
    objects.

    If you are looking for an on-off switch, use the HMute class.
*/
template <class T>
class HSwitch: public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    public:

        union ComponentPtr
        {
            HReader<T>* Reader;
            HWriter<T>* Writer;

            ComponentPtr(HReader<T>* reader)
            {
                Reader = reader;
            }

            ComponentPtr(HWriter<T>* writer)
            {
                Writer = writer;
            }
        };

    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;
        size_t _blocksize;

        T* _buffer;

        bool _isWriter;
        bool _isWriterConsumer;
        bool _isReader;

        int _position;

        std::vector<ComponentPtr> _components;

    public:

        /** Construct a new HSwitch that handle writers */
        HSwitch(HWriter<T>* writer, size_t blocksize);

        /** Construct a new HSwitch that handle writer consumers */
        HSwitch(HWriterConsumer<T>* consumer, size_t blocksize);

        /** Construct a new HSwitch that handle readers */
        HSwitch(HReader<T>* reader, size_t blocksize);

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer);

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

        /** Call Start() on all known components */
        bool Start();

        /** Call Stop() on all known components */
        bool Stop();
};

#endif