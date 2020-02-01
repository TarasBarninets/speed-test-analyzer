#ifndef SPEED_TEST_JSON_PARSER_H
#define SPEED_TEST_JSON_PARSER_H

#include "SpeedTestParser.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

class SpeedTestJsonParser : public SpeedTestParser
{
public:
    bool createDocument(QString path) override;
    void parseDocument() override;
    int getCountObject() const override;

private:
    QJsonDocument mJsonDoc;
    QJsonArray mJsonArray;
    QJsonParseError mDocError;
};

#endif // SPEED_TEST_JSON_PARSER_H
