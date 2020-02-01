#include "SpeedTestFactoryMethod.h"
#include "SpeedTestParser.h"
#include "SpeedTestJsonParser.h"
#include "SpeedTestXmlParser.h"

SpeedTestParser* createParser(QString type)
{
    if(type == "json")
    {
        return new SpeedTestJsonParser();
    }
    else if(type == "xml")
    {
        return new SpeedTestXmlParser();
    }
    else
    {
        return nullptr;
    }
}
