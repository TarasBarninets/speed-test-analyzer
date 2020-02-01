#include "SpeedTestXmlParser.h"
#include <QFile>
#include <QtMath>

bool SpeedTestXmlParser::createDocument(QString path)
{
    QFile file(path);
    bool result = file.open(QIODevice::ReadOnly|QFile::Text);

    if(result)
    {
        mXmlDoc.setContent(&file);
        mXmlArray = mXmlDoc.elementsByTagName("test_results");
    }
    else
    {
        assert(result && "Can not open XML file");
    }

    file.close();
    return result;
}

void SpeedTestXmlParser::parseDocument()
{
    const int countXmlObject = getCountObject();
    for(int i = 0; i < countXmlObject; i++)
    {
        QDomNode node = mXmlArray.item(i);
        qreal bytesToMbits = qPow(10,-6);

        QString timestampISO = node.firstChildElement("timestamp").text();
        QDateTime dateTime = QDateTime::fromString(timestampISO, Qt::ISODate);

        mDownloadData.push_back(node.firstChildElement("download").text().toDouble() * bytesToMbits);
        mUploadData.push_back(node.firstChildElement("upload").text().toDouble() * bytesToMbits);
        mPingData.push_back(node.firstChildElement("ping").text().toDouble());
        mTimestampData.push_back(dateTime);
    }
}

int SpeedTestXmlParser::getCountObject() const
{
    return mXmlArray.size();
}
