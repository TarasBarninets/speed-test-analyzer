#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SpeedTestJsonParser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//class SpeedTestJsonParser;

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

private:
    Ui::MainWindow *mUi;
    QStandardItemModel* mModel;
    SpeedTestJsonParser mJsonParser;
    void fillTableView(QStandardItemModel* model);
    void drawChart();

private slots:
    void chooseJsonFile();

};

#endif // MAINWINDOW_H
