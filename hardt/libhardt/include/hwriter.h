#ifndef __HWRITER_H
#define __HWRITER_H

#include <utility>

#include "hcommand.h"
#include "hobject.h"

/**
    Base class for all writer types
*/
template <class T>
class HWriter : public HObject
{
    public:

        /** Constructor */
        explicit HWriter(std::string id):
            HObject(id) {}

        /** Virtual destructor enabling delete on derived objects */
        virtual ~HWriter() = default;

        /** Write a block of samples */
        virtual int WriteImpl(T* src, size_t blocksize) = 0;

        /** Initialize before first write */
        virtual bool Start()
        {
            return true;
        }

        /** Clean after last write */
        virtual bool Stop()
        {
            return true;
        }

        /** Return a pointer to this instance as a HWriter* */
        HWriter<T>* Writer()
        {
            return (HWriter<T>*) this;
        }

	    /** Receive, handle and propagate command

	        Returns:
		    True if the command was executed correctly
		    False otherwise. Errors is written to the error log
	    */
	    virtual bool Command(HCommand* command) = 0;
};

#endif
