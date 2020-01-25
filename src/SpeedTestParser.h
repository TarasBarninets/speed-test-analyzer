#ifndef SPEED_TEST_PARSER_H
#define SPEED_TEST_PARSER_H

#include <QObject>
#include <QVector>
#include <QDateTime>

class SpeedTestParser : public QObject
{
     Q_OBJECT

public:
    virtual bool createDocument(QString path) = 0;
    virtual void parseDocument() = 0;
    virtual int getCountObject() const = 0;

    const QVector<double>& getDownloadData() const noexcept { return mDownloadData; }
    const QVector<double>& getUploadData()  const noexcept { return mUploadData; }
    const QVector<double>& getPingData()  const noexcept { return mPingData; }
    const QVector<QDateTime>& getTimestampData()  const noexcept { return mTimestampData; }

protected:
    QVector<double> mDownloadData;
    QVector<double> mUploadData;
    QVector<double> mPingData;
    QVector<QDateTime> mTimestampData;
};

#endif // SPEEDTESTPARSER_H
