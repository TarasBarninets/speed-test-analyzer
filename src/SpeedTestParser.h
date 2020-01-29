#ifndef SPEED_TEST_PARSER_H
#define SPEED_TEST_PARSER_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <algorithm>

class SpeedTestParser : public QObject
{
public:
    virtual bool createDocument(QString path) = 0;
    virtual void parseDocument() = 0;
    virtual int getCountObject() const = 0;

    const QVector<double>& getDownloadData() const noexcept { return mDownloadData; }
    const QVector<double>& getUploadData()  const noexcept { return mUploadData; }
    const QVector<double>& getPingData()  const noexcept { return mPingData; }
    const QVector<QDateTime>& getTimestampData()  const noexcept { return mTimestampData; }
    double getMaxDownload() { return *std::max_element(mDownloadData.begin(), mDownloadData.end());}
    double getMaxUpload() { return *std::max_element(mUploadData.begin(), mUploadData.end());}
    double getMaxPing() { return *std::max_element(mPingData.begin(), mPingData.end());}

protected:
    QVector<double> mDownloadData;
    QVector<double> mUploadData;
    QVector<double> mPingData;
    QVector<QDateTime> mTimestampData;
};

#endif // SPEEDTESTPARSER_H
