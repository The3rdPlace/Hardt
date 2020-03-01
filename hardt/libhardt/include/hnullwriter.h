#ifndef __HNULLWRITER_H
#define __HNULLWRITER_H

/**
    /dev/null

    Accepts any number of writes of any blocksize
    and throws the data away.

    Most usefull while your are building or testing an application and
    dont want to spend time finishing a branch that has no relation to your
    current focus area.
*/
template <class T>
class HNullWriter : public HWriter<T>, public HWriterConsumer<T>
{
    public:

        /** Construct a new HNullWriter */
        HNullWriter()
        {}

        /** Construct a new HNullWriter that registers with the upstream writer */
        HNullWriter(HWriterConsumer<T>* consumer)
        {
            consumer->SetWriter(this);
        }

        /** Write a block of samples */
        int Write(T* src, size_t blocksize)
        {
            return blocksize;
        }

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer)
        {
            // Writing into empty space
        }

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No ruther propagation of commands
            return true;
        }
};

#endif