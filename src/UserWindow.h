#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SpeedTestJsonParser.h"
#include "SpeedTestXmlParser.h"
#include "SpeedTestAnalyzer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    UserWindow(QWidget *parent = nullptr);
    virtual ~UserWindow();

private:
    void fillTableView(QStandardItemModel* model);
    void drawChart();
    void createSeries(QChart* chart);

    Ui::MainWindow* mUi;
    QStandardItemModel* mModel;
    std::unique_ptr<SpeedTestParser> mParser;
    QChartView* mChartView;
    SpeedTestAnalyzer* mDownloader;

private slots:
    void chooseFile();
};

class Factory
{
  public:
    virtual SpeedTestParser* createParser() = 0;
    virtual ~Factory() {}
};

class JsonFactory: public Factory
{
  public:
    SpeedTestParser* createParser() {
      return new SpeedTestJsonParser;
    }
};

class XmlFactory: public Factory
{
  public:
    SpeedTestParser* createParser() {
      return new SpeedTestXmlParser;
    }
};

#endif // MAINWINDOW_H
