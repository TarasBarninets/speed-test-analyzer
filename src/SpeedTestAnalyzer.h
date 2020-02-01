#ifndef SPEED_TEST_ANALYZER_H
#define SPEED_TEST_ANALYZER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QSslConfiguration>
#include <QSslSocket>

class SpeedTestAnalyzer : public QObject
{
    Q_OBJECT

public:
    explicit SpeedTestAnalyzer(QObject *parent);

signals:
    void bytesReceived(qint64 bytesRecieved);
    void bytesTotal( qint64 totalBytes);
    void downloadSpeed(QString download);

public slots:
    void sendRequest();
    void getResponse(QNetworkReply *reply);
    void progress(qint64 bytesRecieved, qint64 totalBytes);
    void readChunk();

private:
    QNetworkAccessManager *mNetworkManager;
    QNetworkReply *mReplay;
    QFile *mFile;
    QTime *mDownloadTime;
};

#endif // SPEEDTESTANALYZER_H
