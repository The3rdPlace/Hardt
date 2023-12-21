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

	/** Contains either a reader or a writer */
        struct ComponentPtr
        {
            /** A reader */
            HReader<T>* Reader;

            /** A writer */
            HWriter<T>* Writer;

            /** Create a new ComponentPtr containing a reader */
            ComponentPtr(HReader<T>* reader)
            {
                Reader = reader;
                Writer = nullptr;
            }

            /** Create a new ComponentPtr containing a writer */
            ComponentPtr(HWriter<T>* writer)
            {
                Reader = nullptr;
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
        HSwitch(std::string id, HWriter<T>* writer, size_t blocksize);

        /** Construct a new HSwitch that handle writer consumers */
        HSwitch(std::string id, HWriterConsumer<T>* consumer, size_t blocksize);

        /** Construct a new HSwitch that handle readers */
        HSwitch(std::string id, HReader<T>* reader, size_t blocksize);

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer);

    public:

        /** Default destructor */
        ~HSwitch();

        /** Write a block of samples */
        int WriteImpl(T* src, size_t blocksize);

        /** Read a block of samples */
        int ReadImpl(T* dest, size_t blocksize);

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

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            if( _writer != nullptr && _position == 0 )
            {
                if( !_writer->Command(command) )
                {
                    return false;
                }
            }
            if( _reader != nullptr && _position == 0 )
            {
                if( !_reader->Command(command) )
                {
                    return false;
                }
            }
            if( _position > 0 )
            {
                if( _components.at(_position - 1).Reader != nullptr )
                {
                    if( !_components.at(_position - 1).Reader->Command(command) )
                    {
                        return false;
                    }
                }
                if( _components.at(_position - 1).Writer != nullptr )
                {
                    if( !_components.at(_position - 1).Writer->Command(command) )
                    {
                        return false;
                    }
                }
            }
            return true;
        }
};

#endif
