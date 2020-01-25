#ifndef SPEEDTESTJSONPARSER_H
#define SPEEDTESTJSONPARSER_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QtCharts>

class SpeedTestJsonParser : public QObject
{
     Q_OBJECT

public:
    SpeedTestJsonParser();

    bool createJsonDocument(QString path);
    void parseJsonDocument();

    const QVector<double>& getDownloadData();
    const QVector<double>& getUploadData();
    const QVector<double>& getPingData();
    const QVector<QDateTime>& getTimestampData();
    int getCountJsonObject();

private:
    QJsonDocument mJsonDoc;
    QJsonArray mJsonArray;
    QJsonParseError mDocError;
    QVector<double> mDownloadData;
    QVector<double> mUploadData;
    QVector<double> mPingData;
    QVector<QDateTime> mTimestampData;
};

#endif // SPEEDTESTJSONPARSER_H
