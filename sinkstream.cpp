// Example program
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <vector>
#include <map>
#include <functional>

using namespace std;
namespace SinkStream{

    template<typename T1>
    class Stream{
    protected:
        uint id = 0;
        function<T1()> getLastData_f = NULL;
        map<uint, function<void(T1 data)>> observers_f;
    public:
        /* Observate the stream
         * @param onData the event to be called when new data arrive in the stram 
         * @return an id that can be used to stop observing the stream
        */
        uint listen(function<void(T1 data)> onData, bool callOnDataWithLastData = true) 
        {
            auto newId = id++;
            observers_f[newId] = onData;
            if (callOnDataWithLastData)
                onData(getLastData())
            return newId;
        };

        /* Remove an observation event fro the observers list
         * @param id is an id returned by 'listen' method
         */
        void stopListen(uint id)
        {
            if (observers_f.count(id) > 0)
                observers_f.erase(id);
        }

        /*Return the last data received by the sink/stream*/
        T1 getLastData()
        {
            return getLastData_f();
        }

        Stream(function<T1()> _getLastDataFunc){ getLastData_f = _getLastDataFunc;}
    };

    template<typename T2>
    class StreamCaller: public Stream<T2>{
    private:
        function<void(StreamCaller*)> onDelete_f = NULL;
    public:
        void onNewData(T2 data)
        { 
            for (auto &c: this->observers_f)
                c.second(data);
        }
        void setOnDelete(function<void(StreamCaller*)> _onDelete_f){onDelete_f = _onDelete_f;};

        StreamCaller(function<T2()> _getLastDataFunc): Stream<T2>(_getLastDataFunc){};
        ~StreamCaller(){if (onDelete_f != NULL) onDelete_f(this);}
    };

    template <typename T>
    class Sink{
    private:
        T lastData;

        mutex streamsLocker;
        vector<StreamCaller<T>*> streams;

        void runLocked(function<void()> f)
        {
            streamsLocker.lock();
            f();
            streamsLocker.unlock();
        }

        void streamCallerDestroyed(StreamCaller<T>* sc)
        {
            auto itemPos = std::find(streams.begin(), streams.end(), sc);
            if (itemPos != streams.end())
                runLocked([&](){
                    this->streams.erase(itemPos);
                });
            else
                cerr << "Try do deleting a not existing StreamCaller" << endl;
        }

        void sendToObservers(T data)
        {
            runLocked([&](){
                for (size_t c = 0; c < streams.size(); c++)
                    streams[c]->onNewData(data);
            });
        }

    public:

        Sink(T initialOrInvalidDataSample_forInitialization){ lastData = initialOrInvalidDataSample_forInitialization; }
        /*
            return a stream that observers the sink
        */
        shared_ptr<Stream<T>> getStream()
        {
            auto s = new StreamCaller<T>([&](){ return lastData; });
            
            runLocked([&](){
                this->streams.push_back(s);
            });

            s->setOnDelete([&](StreamCaller<T>* sc){ 
                streamCallerDestroyed(sc); 
            });
            return shared_ptr<Stream<T>>(s);
        }

        /*
            add new data to the sink. All observers will receive a notification with the new data.
            @param data is the new data to be sent to the streams
        */
        void addData(T data)
        {
            lastData = data;
            sendToObservers(data);
        }
    };
}

int main()
{
    SinkStream::Sink<string> sink("");
    auto stream = sink.getStream();
    stream->listen([](string dt){
        cout << "Received new data: " << dt << endl;
    });

    sink.addData("legal");
    sink.addData("outra mensagem");

    cout << "last data: " << stream->getLastData() << endl;


}