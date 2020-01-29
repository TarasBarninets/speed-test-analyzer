#ifndef SPEED_TEST_XML_PARSER_H
#define SPEED_TEST_XML_PARSER_H

#include "SpeedTestParser.h"

#include <QDomDocument>
#include <QtCharts>

class SpeedTestXmlParser : public SpeedTestParser
{
public:
    bool createDocument(QString path) override;
    void parseDocument() override;
    int getCountObject() const override;

private:
    QDomDocument mXmlDoc;
    QDomNodeList mXmlArray;
};

#endif // SPEED_TEST_XML_PARSER_H
