#include "SpeedTestJsonParser.h"

SpeedTestJsonParser::SpeedTestJsonParser()
{

}

bool SpeedTestJsonParser::createJsonDocument(QString path)
{
    QFile file;
    file.setFileName(path);

    bool result = false;
    if(file.open(QIODevice::ReadOnly|QFile::Text))
    {
        mJsonDoc = QJsonDocument::fromJson(QByteArray(file.readAll()),&mDocError);
        mJsonArray = QJsonValue(mJsonDoc.object().value("test_results")).toArray();
        result = true;
    }
    else
    {
        qDebug() << "Can not open JSON file";
    }

    file.close();
    return result;
}

void SpeedTestJsonParser::parseJsonDocument()
{
    for(int i=0; i < mJsonArray.count(); i++)
    {
        const QJsonObject& object = mJsonArray.at(i).toObject();

        qreal bytesToMbits = qPow(10,-6);
        QString download = QString::number(object.value("download").toDouble() * bytesToMbits) + " Mbps";
        QString upload = QString::number(object.value("upload").toDouble() * bytesToMbits) + " Mbps";
        QString ping = QString::number(object.value("ping").toDouble()) + " ms";
        QString timestampISO = object.value("timestamp").toString();
        QDateTime dateTime = QDateTime::fromString(timestampISO, Qt::ISODate);

        mDownloadData.push_back(object.value("download").toDouble() * bytesToMbits);
        mUploadData.push_back(object.value("upload").toDouble() * bytesToMbits);
        mPingData.push_back(object.value("ping").toDouble());
        mTimestampData.push_back(dateTime);
    }
}

const QVector<double>& SpeedTestJsonParser::getDownloadData()
{
    return mDownloadData;
}

const QVector<double>& SpeedTestJsonParser::getUploadData()
{
    return mUploadData;
}

const QVector<double>& SpeedTestJsonParser::getPingData()
{
    return mPingData;
}

const QVector<QDateTime>& SpeedTestJsonParser::getTimestampData()
{
    return mTimestampData;
}

int SpeedTestJsonParser::getCountJsonObject()
{
    return mJsonArray.size();
}
