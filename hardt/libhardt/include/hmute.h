#ifndef __HMUTE_H
#define __HMUTE_H

/**
    A filter that can be muted. When muted, reads will return
    zero samples and writes will write zero samples.

    When the component is muted, Stop() will be called on the
    up- or downstream writer or reader. And when unmuted, Start()
    will be called.

    A call to Start() or Stop() will only be propagated if the muted state
    is 'not muted' (false).
*/
template <class T>
class HMute : public HFilter<T>
{
    private:

        bool _muted;

    public:

        /** Construct a new HMute object that writes to a writer */
        HMute(std::string id, HWriter<T>* writer, bool muted, size_t blocksize);

        /** Construct a new HMute object that registers with an upstream writer */
        HMute(std::string id, HWriterConsumer<T>* consumer, bool muted, size_t blocksize);

        /** Construct a new HMute object that reads from a reader */
        HMute(std::string id, HReader<T>* reader, bool muted, size_t blocksize);

        /** Default destructor */
        ~HMute();

        /** Run a block of samples through the mute filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Set state */
        void SetMuted(bool muted);

        /** Get state */
        bool GetMuted() {
            return _muted;
        }

        /** Initialize before first write */
        bool Start();

        /** Cleanup after last write */
        bool Stop();
};

#endif
