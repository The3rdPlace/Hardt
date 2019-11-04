#ifndef __HREADER_H
#define __HREADER_H

/**
    The base class for any reader type. It provides a pure virtual Read() function which must
    be implemented by the specific reader.

    It further provides two virtual methods Start() and Stop() which is used by
    specifik readers to do any initialization/clean-up when reading starts and stops.

    If you implement types that uses a HReader, you must implement Start() and Stop() so that
    they call Start() and Stop() on the reader the use.

    Ex: You implement a mp3 file decoder based on the HFileReader class.  The processor will
    read from your decoder class by calling Start() and then Read() untill you return 0 (zero).
    Then Stop() will be called. If your decoder class do not call Start() and Stop() on the base
    HFilerReader, the file will never be opened and the first Read() will return 0 (zero).

    Always cascade Start() and Stop() to any HReader you implement or use.!
*/
template <class T>
class HReader : public HObject
{
    public:

        /** Virtual destructor enabling delete on derived objects */
        virtual ~HReader() = default;

        /** Read a block of samples */
        virtual int Read(T* dest, size_t blocksize) = 0;

        /** Initialize before first read */
        virtual bool Start()
        {
            return true;
        }

        /** Cleanup after last read */
        virtual bool Stop()
        {
            return true;
        }

        /** Returns a HReader* to this reader */
        HReader<T>* Reader()
        {
            return (HReader<T>*) this;
        }
};

#define READER (HReader<T>*)

#endif