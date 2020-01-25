#include "SpeedTestJsonParser.h"

bool SpeedTestJsonParser::createDocument(QString path)
{
    QFile file(path);


    bool result = file.open(QIODevice::ReadOnly|QFile::Text);
    if(result)
    {
        mJsonDoc = QJsonDocument::fromJson(QByteArray(file.readAll()),&mDocError);
        mJsonArray = mJsonDoc.object().value("test_results").toArray();
    }
    else
    {
        assert(result && "Can not open JSON file");
    }

    file.close();
    return result;
}

void SpeedTestJsonParser::parseDocument()
{
    const int countJsonObject = getCountObject();
    for(int i = 0; i < countJsonObject; i++)
    {
        const QJsonObject& object = mJsonArray.at(i).toObject();

        qreal bytesToMbits = qPow(10,-6);
        QString timestampISO = object.value("timestamp").toString();
        QDateTime dateTime = QDateTime::fromString(timestampISO, Qt::ISODate);

        mDownloadData.push_back(object.value("download").toDouble() * bytesToMbits);
        mUploadData.push_back(object.value("upload").toDouble() * bytesToMbits);
        mPingData.push_back(object.value("ping").toDouble());
        mTimestampData.push_back(dateTime);
    }
}

int SpeedTestJsonParser::getCountObject() const
{
    return mJsonArray.size();
}
