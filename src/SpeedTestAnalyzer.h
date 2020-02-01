#ifndef SPEED_TEST_ANALYZER_H
#define SPEED_TEST_ANALYZER_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;
class QFile;

class SpeedTestAnalyzer : public QObject
{
    Q_OBJECT

public:
    SpeedTestAnalyzer(QObject *parent);
    ~SpeedTestAnalyzer();

signals:
    void bytesReceived(qint64 bytesRecieved);
    void bytesTotal( qint64 totalBytes);
    void downloadSpeed(QString download);

public slots:
    void sendRequest();
    void finishDownload(QNetworkReply *reply);
    void downloadProgress(qint64 bytesRecieved, qint64 totalBytes);
    void readChunk();

private:
    QNetworkAccessManager *mNetworkManager;
    QNetworkReply *mReplay;
    QFile *mFile;
    QTime *mDownloadTime;
};

#endif // SPEED_TEST_ANALYZER_H
