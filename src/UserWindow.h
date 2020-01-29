#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SpeedTestJsonParser.h"
#include "SpeedTestXmlParser.h"

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

private slots:
    void chooseFile();
};

#endif // MAINWINDOW_H
