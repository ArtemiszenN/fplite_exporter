#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readUserSettingsJson(userSettingsFilePath);
}

MainWindow::~MainWindow()
{
    writeUserSettingsJson();
    delete ui;
}

QString MainWindow::getFileDirFromFilePath(QString filePath)
{
    QDir dir = QFileInfo(filePath).absoluteDir();
    return dir.absolutePath();
}

void MainWindow::readFpliteFile(QString filePath)
{
    std::vector<QString> strings;
    QFile inputFile(filePath);
    if (inputFile.open(QIODevice::ReadOnly)){
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          strings.push_back(line);
       }
       inputFile.close();
    }else{
        QMessageBox::warning(this,"Read failed","Could not open file at " + filePath + " for reading.");
        return;
    }
    files.clear();
    ui->foundFilesEdit->clear();
    for(QString line: strings){
        QString currentFilePath = line.right(line.length() - 7);
        ui->foundFilesEdit->appendPlainText(currentFilePath);
        files.push_back(currentFilePath);
    }
}

void MainWindow::on_fpliteButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select fplite playlist",  fileExplorerMap["fpliteSelect"], tr("fplite files(*.fplite)"));
    if(fileName.trimmed().length() == 0){
        return;
    }
    ui->fpliteFilePath->setText(fileName);
    fileExplorerMap["fpliteSelect"] = getFileDirFromFilePath(fileName);
    readFpliteFile(fileName);
}

void MainWindow::on_dirButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Select export directory",  fileExplorerMap["dirSelect"]);
    if(dirPath.trimmed().length() == 0){
        return;
    }
    ui->outputDirPath->setText(dirPath);
    fileExplorerMap["dirSelect"] = dirPath;
}

QString MainWindow::getSongName(QString songPath){
    QFileInfo fileInfo(songPath);
    return fileInfo.fileName();
}

void MainWindow::on_runButton_clicked()
{
    for(int i=0;i<(int)files.size();i++){
        QString song = files[i];
        QString copyTo = ui->outputDirPath->text() + QDir::separator() + getSongName(song);
        ui->foundFilesEdit->appendPlainText("Transferring " + song + " to " + copyTo);
        if(QFile::exists(copyTo)){
            QFile::remove(copyTo);
        }
        QFile::copy(song, copyTo);
    }
    ui->foundFilesEdit->appendPlainText("Done.");
}

void MainWindow::readUserSettingsJson(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadWrite)){
        QMessageBox::warning(this,"Read failed","Could not open file at "+filePath+ " to read fplite exporter user settings.");
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    if(data.isEmpty()){
        return;
    }
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject obj = document.object();
    foreach(QString key, fileExplorerMap.keys()){
        QString val = obj[key].toString();
        if(val.trimmed().length()==0)continue;
        fileExplorerMap[key]=val;
    }
}

void MainWindow::writeUserSettingsJson()
{
    QVariantMap map;
    QMapIterator<QString,QString>i(fileExplorerMap);
    while(i.hasNext()){
        i.next();
        map.insert(i.key(),i.value());
    }
    QJsonDocument document = QJsonDocument::fromVariant(map);
    QFile file(userSettingsFilePath);
    if(!file.open(QIODevice::ReadWrite)){
        QMessageBox::warning(this,"Write failed","Could not open file at " + userSettingsFilePath+ " to save fplite exporter user settings.");
        return;
    }
    file.resize(0);
    file.write(document.toJson());
    file.close();
}
