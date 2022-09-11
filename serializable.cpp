#include  "serializable.h" 
 
string Serializable::serialize()
{
    JSON js;
    js.setString("class", this->classNameS);
    js = serializeFields(js, this->getFildsForSerialization());
    js = serializeTags(js, this->tags);
    return js.ToString();
}

JSON Serializable::serializeFields(JSON js, FieldsMapHelper _fields)
{
    for (auto &currField: _fields.theMap)
        js.setString(currField.first, currField.second.getString());

    return js;
}

JSON Serializable::serializeTags(JSON js, map<string, string> _tags)
{
    for (auto &currField: _tags)
        js.setString("tags."+currField.first, currField.second);
    
    return js;
}


bool Serializable::deserialize(string data, bool throwExceptions = true)
{
    JSON js(data);
    auto _class = js.getString("class", "");
    if (_class == this->classNameS)
    {
        deserializeTags(js);
        deserializeFields(js);
        return true;
    }
    else 
    {
        dim.get<ILogger>()->critical(classNameS, "Invalid desserialization class. Expected '"+classNameS+"' but received '"+_class+"'", false);

        if (throwExceptions)
            throw CapitalManager::Exceptions::DeserializingInvalidClassException();
    }
    return false;
}

void Serializable::deserializeTags(JSON js)
{
    auto tagsNames = js.getChildsNames("tags");
    for (auto &currTag: tagsNames)
        this->setTag("tags."+currTag, js.getString(currTag));
}

void Serializable::deserializeFields(JSON js)
{
    FieldsMapHelper fields;
    auto fieldsNames = js.getChildsNames("");
    for (auto &currField: fieldsNames)
    {
        if (currField != "class" && currField != "tags")
            fields.set(currField, js.getString(currField));
    }

    this->setFieldsFromDesearilization(fields);
}

DynamicVar FieldsMapHelper::get(string fieldName, DynamicVar defaultValue)
{
    if (theMap.count(fieldName) > 0)
        return theMap[fieldName];

    return defaultValue;
}

void FieldsMapHelper::set(string fieldName, DynamicVar value)
{
    theMap[fieldName] = value;
}
