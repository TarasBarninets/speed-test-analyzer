#include "SpeedTestAnalyzer.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QFile>
#include <QSslConfiguration>

SpeedTestAnalyzer::SpeedTestAnalyzer(QObject *parent)
    : QObject(parent)
    , mNetworkManager(new QNetworkAccessManager(this))
    , mReplay(nullptr)
    , mFile(nullptr)
    , mDownloadTime(new QTime())
{
    connect(mNetworkManager, &QNetworkAccessManager::finished, this, &SpeedTestAnalyzer::finishDownload);
}

SpeedTestAnalyzer::~SpeedTestAnalyzer()
{
    delete  mDownloadTime;
}

void SpeedTestAnalyzer::sendRequest()
{
    const QUrl url("https://speed.hetzner.de/100MB.bin"); // https://norvig.com/big.txt
    QNetworkRequest request(url);

    QSslConfiguration configSsl = QSslConfiguration::defaultConfiguration();
    configSsl.setProtocol(QSsl::AnyProtocol);
    request.setSslConfiguration(configSsl);

    const QString downloadFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    const QString fileName = "FileDownloadedFromQt.txt";
    QString filePath = downloadFolder + '/' + fileName;

    mReplay = mNetworkManager->get(request);
    mFile = new QFile(filePath);
    mFile->open(QFile::WriteOnly);

    connect(mReplay, &QNetworkReply::downloadProgress, this, &SpeedTestAnalyzer::downloadProgress);
    connect(mReplay, &QIODevice::readyRead, this, &SpeedTestAnalyzer::readChunk);

    mDownloadTime->start();
}

void SpeedTestAnalyzer::readChunk()
{
    QByteArray data = mReplay->readAll();
    mFile->write(data);
}

void SpeedTestAnalyzer::finishDownload(QNetworkReply *reply)
{
    mFile->close();

    if(reply->error())
    {
        qDebug() << "Error " << reply->errorString();
    }
    else
    {
        qDebug() << "Downloading is completed";
    }
}

void SpeedTestAnalyzer::downloadProgress(qint64 bytesRecived, qint64 totalBytes)
{
    const double bitsDownload = bytesRecived * 8;
    const double secondsSpent = mDownloadTime->elapsed() * 1000;
    const double download = bitsDownload / secondsSpent;
    QString downloadSpeedMbps = QString::number(download, 'g' , 2) + " Mbps";

    qDebug() << " bitsDownload = " << QString::number(bitsDownload) << " / " << " secondsSpent = " << QString::number(secondsSpent);
    qDebug() << " Download Speed = " << downloadSpeedMbps;

    emit bytesReceived(bytesRecived);
    emit bytesTotal(totalBytes);
    emit downloadSpeed(downloadSpeedMbps);
}


