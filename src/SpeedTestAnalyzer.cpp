#include "SpeedTestAnalyzer.h"

SpeedTestAnalyzer::SpeedTestAnalyzer(QObject *parent)
    : QObject(parent)
    , mNetworkManager(new QNetworkAccessManager())
    , mReplay(nullptr)
    , mFile(nullptr)
    , mDownloadTime(new QTime())
{
    connect(mNetworkManager, &QNetworkAccessManager::finished, this, &SpeedTestAnalyzer::getResponse);
}

void SpeedTestAnalyzer::sendRequest()
{
    QUrl url("https://speed.hetzner.de/100MB.bin"); //https://norvig.com/big.txt
    QNetworkRequest request;
    request.setUrl(url);

    QSslConfiguration configSsl = QSslConfiguration::defaultConfiguration();
    configSsl.setProtocol(QSsl::AnyProtocol);
    request.setSslConfiguration(configSsl);

    mReplay = mNetworkManager->get(request);
    mFile = new QFile("C:\\Users\\Taras\\Downloads\\FromQT.txt");
    mFile->open(QFile::WriteOnly);

    connect(mReplay, &QNetworkReply::downloadProgress, this, &SpeedTestAnalyzer::progress);
    connect(mReplay, &QIODevice::readyRead, this, &SpeedTestAnalyzer::readChunk);

    mDownloadTime->start();
}

void SpeedTestAnalyzer::readChunk()
{
    const qint64 dataBytes = mReplay->size();
    const qint64 milliseconds = mDownloadTime->elapsed();

    qDebug() << "Chunk size = " << dataBytes;
    qDebug() << "Per milliseconds = " << milliseconds;

    QByteArray data= mReplay->readAll();
    mFile->write(data);

    const qint64 bytesPerMilliseconds = dataBytes / milliseconds;
    const qint64 speed = ( bytesPerMilliseconds * 1000 );
    const QString download = QString::number(speed) + "bytes/sec";

    qDebug() << "speed = " << speed << "bytes/sec";
    emit downloadSpeed(download);
}

void SpeedTestAnalyzer::getResponse(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << " Error " << reply->errorString();
    }
    else
    {
        /*
        mFile = new QFile("C:\\Users\\Taras\\Downloads\\FromQT.txt");
        if(mFile->open(QFile::WriteOnly)) //instead file - mFile
        {
            mFile->write(reply->readAll());
            mFile->close();
        }
        */

        qDebug() << "Downloading is completed";
        mFile->close();
    }
}

void SpeedTestAnalyzer::progress(qint64 bytesRecived, qint64 totalBytes)
{
    qDebug() << bytesRecived << "/" << totalBytes;
    emit bytesReceived(bytesRecived);
    emit bytesTotal(totalBytes);
}


