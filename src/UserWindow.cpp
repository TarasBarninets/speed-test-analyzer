#include "UserWindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QStandardItem>
#include <QtMath>
#include <QDateTime>
#include <QDebug>
#include <QLineSeries>

UserWindow::UserWindow(QWidget *parent)
    : QMainWindow(parent) // pass constructor parameter into construction of base class -> QMainWindow
    , mUi(new Ui::MainWindow)
    , mParser(new SpeedTestJsonParser)
{
    mUi->setupUi(this);

    connect(mUi->pushButton, &QPushButton::clicked, this, &UserWindow::chooseFile);

    mModel = new QStandardItemModel(nullptr);
    QStringList headerLabels = { "Download", "Upload", "Ping", "Timestamp" };
    mModel->setHorizontalHeaderLabels(headerLabels);
    mUi->tableView->setModel(mModel);
    mUi->tabWidget->setTabText(0,"Data");
}

UserWindow::~UserWindow()
{
    delete mUi;
    delete mParser;
}

void UserWindow::drawChart()
{
    QChart *chart = new QChart();

    // prepare data
    QLineSeries *downloadSeries = new QLineSeries();
    QLineSeries *uploadSeries = new QLineSeries();
    QLineSeries *pingSeries = new QLineSeries();

    int countJsonObject = mParser->getCountObject();
    for (int i=0; i<countJsonObject; i++)
    {
        qint64 timestamp = mParser->getTimestampData().at(i).toMSecsSinceEpoch();

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
    chart->setTitle("Chart build based on parsed JSON file");
    chart->setAnimationOptions(QChart::AllAnimations);

    // add axis to the chart
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(7);
    axisX->setFormat("MM-dd hh:mm:ss");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    downloadSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Speed");
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->axes(Qt::Vertical).back()->setRange(0, 25);
    downloadSeries->attachAxis(axisY);

    // create new view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    mUi->tabWidget->addTab(chartView,"Chart");
}

void UserWindow::fillTableView(QStandardItemModel* model)
{
     const QVector<double>& downloadData = mParser->getDownloadData();
     const QVector<double>& uploadData = mParser->getUploadData();
     const QVector<double>& pingData = mParser->getPingData();
     const QVector<QDateTime>& timestampData = mParser->getTimestampData();
     int countJsonObject = mParser->getCountObject();

    for(int i=0; i < countJsonObject; i++)
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
    const QString path = QFileDialog::getOpenFileName(nullptr, "", documentsFolder, "*.json");

    // create valid instance of parser based on the type of file that has been chosen - xml, json , etc.
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
