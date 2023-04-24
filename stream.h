#ifndef __SHARED__STREAM__
#define __SHARED__STREAM__

#include <functional>
#include <vector>
#include <mutex>
#include <map>
#include <future>

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
        ID listen(function<void(T data)> observer);
        /* Another way to listen data comming from the stream (redirects to 'listen' function) */
        ID subscribe(function<void(T data)> f){ return listen(f); }

        /* This function adds data to the stream, i.e., all observer will receive this data
         * @param data is the data to be sent to all observers
         */
        void add(T data);

        /* Another way to add data to the stream (redirects to 'add' function) */
        void stream(T data){ add(data); }

        future<T> getNext();

        /* Return the last received data */
        T get();

        /* Stops observating the stream
         * @param the id of the observer, previusly returned by 'listen' function
         */
        void stopListen(ID id);

        /* Another way to stop lisening data comming from the stream (redirects to 'stopListen' function) */
        ID unsubscribe(ID subscribeId){ return stopListen(subscribeId); }
    };
}

#endif