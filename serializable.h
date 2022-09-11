#ifndef __SERIALIZABLE__H__ 
#define __SERIALIZABLE__H__ 
#include <string>
#include <TaggedObject.h>
#include <json_maker/sources/JSON.h>
#include <DynamicVar.h>


//specif project logic
#include <main.h>
#include <deserializingInvalidClassException.h>
#include <logger.h>

using namespace std;
using namespace JsonMaker;

class FieldsMapHelper
{
public:
    map<string, DynamicVar> theMap;

    FieldsMapHelper();
    FieldsMapHelper(map<string, DynamicVar> _fields): theMap(_fields){}
    DynamicVar get(string fieldName, DynamicVar defaultValue);
    void set(string fieldName, DynamicVar value);
};

class Serializable: public TaggedObject { 
private:
    void deserializeTags(JSON js);
    void deserializeFields(JSON js);

    //tw two function bellow are pure functions and can be converted to static
    JSON serializeTags(JSON js, map<string, string> _tags);
    JSON serializeFields(JSON js, FieldsMapHelper _fields);
protected:
    std::string classNameS;
    virtual FieldsMapHelper getFildsForSerialization() = 0;
    virtual void setFieldsFromDesearilization(FieldsMapHelper fields) = 0;
public:
    Serializable(std::string className):classNameS(className){}

    string serialize();
    bool deserialize(string data, bool throwExceptions = true);
}; 
 
#endif 
