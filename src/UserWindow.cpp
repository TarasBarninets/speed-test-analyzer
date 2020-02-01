#include "UserWindow.h"
#include "ui_mainwindow.h"
#include "SpeedTestParser.h"
#include "SpeedTestFactoryMethod.h"

#include <QFile>
#include <QFileDialog>
#include <QStandardItem>
#include <QtMath>
#include <QDateTime>
#include <QDebug>
#include <QLineSeries>
#include <QStandardItemModel>
#include <algorithm>

UserWindow::UserWindow(QWidget *parent)
    : QMainWindow(parent) // pass constructor parameter into construction of base class -> QMainWindow
    , mUi(new Ui::MainWindow)
    , mModel(new QStandardItemModel)
    , mChartView(nullptr)
    , mDownloader(new SpeedTestAnalyzer(this))
{
    mUi->setupUi(this);
    mUi->progressBar->reset();
    connect(mUi->pushButton, &QPushButton::clicked, this, &UserWindow::chooseFile);
    connect(mUi->pushButton_2, &QPushButton::clicked, mDownloader, &SpeedTestAnalyzer::sendRequest);
    connect(mDownloader, &SpeedTestAnalyzer::bytesTotal, mUi->progressBar, &QProgressBar::setMaximum);
    connect(mDownloader, &SpeedTestAnalyzer::bytesReceived, mUi->progressBar, &QProgressBar::setValue);
    connect(mDownloader, &SpeedTestAnalyzer::downloadSpeed, mUi->label, &QLabel::setText);

    QStringList headerLabels = { "Download", "Upload", "Ping", "Timestamp" };
    mModel->setHorizontalHeaderLabels(headerLabels);
    mUi->tableView->setModel(mModel);
    mUi->tabWidget->setTabText(0,"Data");
}

UserWindow::~UserWindow()
{
    delete mUi;
}

void UserWindow::createSeries(QChart* chart)
{
    // prepare data
    QLineSeries *downloadSeries = new QLineSeries();
    QLineSeries *uploadSeries = new QLineSeries();
    QLineSeries *pingSeries = new QLineSeries();

    const int countJsonObject = mParser->getCountObject();
    for (int i = 0; i < countJsonObject; i++)
    {
        const qint64 timestamp = mParser->getTimestampData().at(i).toMSecsSinceEpoch();

        downloadSeries->append(timestamp, mParser->getDownloadData().at(i));
        uploadSeries->append(timestamp, mParser->getUploadData().at(i));
        pingSeries->append(timestamp, mParser->getPingData().at(i));
    }

    // name it
    downloadSeries->setName("Download speed(Mbps)");
    uploadSeries->setName("Upload speed(Mbps)");
    pingSeries->setName("Ping speed(ms)");

    // add data series to the chart
    chart->addSeries(downloadSeries);
    chart->addSeries(uploadSeries);
    chart->addSeries(pingSeries);

    // specify legend position
    chart->legend()->setAlignment(Qt::AlignBottom);

    // set chart title
    chart->setTitle("Chart based on parsed file");
    chart->setAnimationOptions(QChart::AllAnimations);

    // find values for set Range and TickCount
    const double maxDownload = mParser->getMaxDownload();
    const double maxUpload = mParser->getMaxUpload();
    const double maxPing = mParser->getMaxPing();

    const double maxRange = std::max(std::max(maxDownload, maxUpload), maxPing);
    const int tickCount = static_cast<int>(maxRange/3);

    // add axis to the chart
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("MM-dd hh:mm:ss");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    downloadSeries->attachAxis(axisX);
    uploadSeries->attachAxis(axisX);
    pingSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTickCount(tickCount);
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Speed");
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->axes(Qt::Vertical).back()->setRange(0 , maxRange);
    downloadSeries->attachAxis(axisY);
    uploadSeries->attachAxis(axisY);
    pingSeries->attachAxis(axisY);
}

void UserWindow::drawChart()
{
    // if chart exist - clear series and axis
    if(mChartView != nullptr)
    {
         QChart* chart = mChartView->chart();
         chart->removeAllSeries();
         chart->removeAxis(chart->axes(Qt::Horizontal).back());
         chart->removeAxis(chart->axes(Qt::Vertical).back());
         createSeries(chart);
    }
    else
    {
        QChart *chart = new QChart();
        createSeries(chart);

        // create new view
        mChartView = new QChartView(chart, this);
        mChartView->setRenderHint(QPainter::Antialiasing);

        // add new tab that contain chart
        mUi->tabWidget->addTab(mChartView,"Chart");
    }
}

void UserWindow::fillTableView(QStandardItemModel* model)
{
    // if tableview not empty - clear data
    int rowCount = mModel->rowCount();
    if(rowCount > 0)
    {
        mModel->setRowCount(0);
    }

    const QVector<double>& downloadData = mParser->getDownloadData();
    const QVector<double>& uploadData = mParser->getUploadData();
    const QVector<double>& pingData = mParser->getPingData();
    const QVector<QDateTime>& timestampData = mParser->getTimestampData();
    const int countJsonObject = mParser->getCountObject();

    for(int i = 0; i < countJsonObject; i++)
    {
        QString download = QString::number(downloadData.at(i)) + " Mbps";
        QString upload = QString::number(uploadData.at(i)) + " Mbps";
        QString ping = QString::number(pingData.at(i)) + " ms";
        QDateTime dateTime = timestampData.at(i);

        QStandardItem* downloadColumn = new QStandardItem(download);
        QStandardItem* uploadColumn = new QStandardItem(upload);
        QStandardItem* pingColumn = new QStandardItem(ping);
        QStandardItem* timestampColumn = new QStandardItem();

        timestampColumn->setData(dateTime.toString(),Qt::DisplayRole);
        QList<QStandardItem*> dataColumns = { downloadColumn, uploadColumn, pingColumn, timestampColumn };
        model->appendRow(dataColumns);
    }
}


void UserWindow::chooseFile()
{
    const QString documentsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    const QString path = QFileDialog::getOpenFileName(nullptr, "", documentsFolder, "Files (*.json *.xml)");

    QFileInfo info(path);
    QString ext = info.suffix();

    mParser.reset(createParser(ext));

    if(mParser)
    {
        mParser->createDocument(path);
        mParser->parseDocument();
        fillTableView(mModel);
        mUi->tableView->resizeColumnsToContents();
        drawChart();
    }
    else
    {
       qDebug() << "mParser invalid";
    }
}
