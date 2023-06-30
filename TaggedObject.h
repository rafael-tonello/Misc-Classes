#ifndef __TAGGEDOBJECT
#define __TAGGEDOBJECT

#include<string>
#include<map>

using namespace std;
class TaggedObject
{
public:
    map<string, string> tags;
    void setTag(string tag, string value);
    string getTag(string tag, string defaultValue = "");

};

class ObjectWithMetadata: public TaggedObject{
public:
    template<typename T>
    void setMeta(string name, T value)
    {
        this->setTag(name, to_string(value));
    }

    template<typename T>
    T getMeta(string name, T defaultValue)
    {
        string val = getTag(name, to_string(defaultValue));
        if (typeid(T) == typeid(int))               return (T)stoi(val);
        else if (typeid(T) == typeid(uint))         return (T)(uint)stoul(val);
        else if (typeid(T) == typeid(int64_t))      return (T)stoll(val);
        else if (typeid(T) == typeid(uint64_t))     return (T)stoull(val);
        else if (typeid(T) == typeid(double))       return (T)stold(val);
        else if (typeid(T) == typeid(bool))         return (T)string("trueTRUE1Yes").find(val) != string::npos;
        else if (typeid(T) == typeid(char))         return (T)((val.size() > 0 ? val[0] : 0));
        
        return defaultValue;
    }

    template<typename T>
    void setMetaP(string name, T *p)
    {
        this->setMeta<uint64_t>(name, (uint64_t)p);
    }

    template<typename T>
    T* getMetaP(string name)
    {
        uint64_t v = getMeta<uint64_t>(name, 0);
        return (T*)(v);
    }

};

#endif