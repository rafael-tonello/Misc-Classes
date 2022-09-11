#ifndef __SHARED__STREAM__
#define __SHARED__STREAM__

#include <functional>
#include <vector>
#include <mutex>
#include <map>

using namespace std;
namespace Shared{
    #define ID uint

    template <typename T>
    /*A sincronous stream*/
    class Stream{
    private:
        ID currId = 0;
        map<ID, function<int(T data)>> listeners;
        mutex listenerMutex;
        T last;
    public:
        /* this function add an obsever tot he Stream. This observer will receive all data that is added via 'add' function
         * @param observer an function to be called whena  new data arrives in the stream. This function is am lambda with the signature [](T data){ }
         * @return this function returns an observer id, that can be used to stop observatin the stream
         */
        uint listen(function<void(T data)> observer);

        /* This function adds data to the stream, i.e., all observer will receive this data
         * @param data is the data to be sent to all observers
         */
        void add(T data);

        /* Return the last received data */
        T get();

        /* Stops observating the stream
         * @param the id of the observer, previusly returned by 'listen' function
         */
        void stopListen(ID id);
    };
}

#endif