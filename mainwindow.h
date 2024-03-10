#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QApplication>
#include <QFileDialog>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getFileDirFromFilePath(QString filePath);
private slots:
    void on_fpliteButton_clicked();

    void on_dirButton_clicked();

    void on_runButton_clicked();

private:
    Ui::MainWindow *ui;
    void readUserSettingsJson(QString filePath);
    const QString userSettingsFilePath = qApp->applicationDirPath()+"/usersettings.json";
    QMap<QString,QString> fileExplorerMap = {
        {"fpliteSelect",qApp->applicationDirPath()},
        {"dirSelect",qApp->applicationDirPath()}
    };
    void writeUserSettingsJson();
    void readFpliteFile(QString filePath);
    std::vector<QString> files;
    QString getSongName(QString songPath);
};
#endif // MAINWINDOW_H
