#include "my_window.h"
#include "ui_my_window.h"
#include <QFrame>
#include <QAbstractTableModel>
#include <QFile>
#include <QGroupBox>
#include <QMouseEvent>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#define MODE_2 0
#define MODE_5 1
#define MODE_2_5 2
#define MAX_SCANS 10

const QString style_red = "QProgressBar::chunk {background: QLinearGradient( x1: 1, y1: 0, x2: 0, y2: 0,stop: 0 #F10350,stop: 0.4999 #FF3320,stop: 0.5 #FF0019,stop: 1 #FF0000 );}";
const QString style_yellow = "QProgressBar::chunk {background: QLinearGradient( x1: 1, y1: 0, x2: 0, y2: 0,stop: 0 #F10350,stop: 0.4999 #FF3320,stop: 0.5 #FF0019,stop: 1 #F0F150 );}";
const QString style_green = "QProgressBar::chunk {background: QLinearGradient( x1: 1, y1: 0, x2: 0, y2: 0,stop: 0 #F10350,stop: 0.4999 #FF3320,stop: 0.5 #FF0019,stop: 1 #05F150 );}";

my_window::my_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::my_window)
{
    ui->setupUi(this);
    mode = MODE_2_5;
    load_data();

    QGroupBox *group = new QGroupBox();
    group->setTitle("| SSID | Channel | dBm | % signal strenght");
    group_grid = new QGridLayout();
    for (int i = 0; i < ssid_list.size(); i++){
       idx_list.append(i);
       ssid_labels.append(new QPushButton(ssid_list[i]));
       connect(ssid_labels[i], SIGNAL (released()), this, SLOT (handleButton()));
       group_grid->addWidget(ssid_labels[i],i,0);
       QString ch_s = QString::number(channel_list[i]);
       channel_labels.append(new QLabel (ch_s));
       group_grid->addWidget(channel_labels[i],i,1);
       sig_labels.append(new QLabel (QString::number(sig_list[i])));
       group_grid->addWidget(sig_labels[i],i,2);
       bars.append(new QProgressBar());
       bars[i]->setRange(-90,-30);
       bars[i]->setValue(sig_list[i]);
       bars[i]->setInvertedAppearance(1);
       if (bars[i]->value()<-70){
           bars[i]->setStyleSheet(style_red);
       } else if (bars[i]->value()>=-70 && bars[i]->value()<-50) {
           bars[i]->setStyleSheet(style_yellow);
       } else if (bars[i]->value()>=-50)
           bars[i]->setStyleSheet(style_green);

       group_grid->addWidget(bars[i],i,3);
    }
     group->setLayout(group_grid);
    ui->scrollArea->setWidget(group);

    timerId = startTimer(2000);
}

my_window::~my_window()
{
    killTimer(timerId);
    delete ui;
}

void my_window::update_data()
{
    // nacitanie suboru
    static int file_num = 1;
    QChar file_num_c(file_num+'0');
    QString settings("data/scan");
    settings.append(file_num_c);
    settings.append(".json");
    QFile file;
    file.setFileName(settings);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen()){
        qWarning() << "File open error\n";
        QCoreApplication::quit();
    }
    settings = file.readAll();
    file.close();
    // parsovanie jsonu
    QJsonDocument j_doc = QJsonDocument::fromJson(settings.toUtf8());
    if (j_doc.isNull()){
        qWarning() << "File reading error\n";
        QCoreApplication::quit();
    }
    QJsonObject tmp_obj = j_doc.object();
    QJsonArray j_arr = tmp_obj["networks"].toArray();
    idx_list.clear();
    foreach (const QJsonValue & value, j_arr) {
        QJsonObject obj = value.toObject();
        QString ssid = obj["ssid"].toString();
        if (ssid_list.contains(ssid)){
            //  update siete
            int i = ssid_list.indexOf(ssid, 0);
            sig_list[i] = obj["sigstr"].toInt();
            sig_labels[i]->setText(QString::number(sig_list[i]));
            if (ui->label_4->text().compare(ssid) == 0){
                ui->label_2->setText(QString::number(sig_list[i]).append(" dBm"));
            }
            bars[i]->setValue(sig_list[i]);
            if (bars[i]->value()<-70){
                bars[i]->setStyleSheet(style_red);
            } else if (bars[i]->value()>=-70 && bars[i]->value()<-50) {
                bars[i]->setStyleSheet(style_yellow);
            } else if (bars[i]->value()>=-50)
                bars[i]->setStyleSheet(style_green);
            idx_list.append(i);
        } else {
            // nova siet
            ssid_list.append(ssid);
            channel_list.append(obj["channel"].toInt());
            freq_list.append(obj["frequency"].toDouble());
            sig_list.append(obj["sigstr"].toInt());
            int idx = ssid_list.indexOf(ssid, 0);
            idx_list.append(idx);
            ssid_labels.append(new QPushButton(ssid_list[idx]));
            connect(ssid_labels[idx], SIGNAL (released()), this, SLOT (handleButton()));
            int riadok = group_grid->rowCount();
            group_grid->addWidget(ssid_labels.last(),riadok,0);
            QString ch_s = QString::number(channel_list[idx]);
            channel_labels.append(new QLabel (ch_s));
            group_grid->addWidget(channel_labels.last(),riadok,1);
            sig_labels.append(new QLabel (QString::number(sig_list[idx])));
            group_grid->addWidget(sig_labels[idx],riadok,2);
            bars.append(new QProgressBar());
            bars[idx]->setRange(-90,-30);
            bars[idx]->setValue(sig_list[idx]);
            bars[idx]->setInvertedAppearance(1);
            if (bars[idx]->value()<-70){
                bars[idx]->setStyleSheet(style_red);
            } else if (bars[idx]->value()>=-70 && bars[idx]->value()<-50) {
                bars[idx]->setStyleSheet(style_yellow);
            } else if (bars[idx]->value()>=-50)
                bars[idx]->setStyleSheet(style_green);
            if ((mode == MODE_2 && freq_list[idx] == 5.0) ||
                (mode == MODE_5 && freq_list[idx] == 2.4)){
                bars[idx]->setVisible(false);
                ssid_labels[idx]->setVisible(false);
                channel_labels[idx]->setVisible(false);
                sig_labels[idx]->setVisible(false);
            }
            group_grid->addWidget(bars[idx],riadok,3);
        }
    }
    // uprava file_num pre nacitanie dalsich dat
    if (file_num == MAX_SCANS-1){
        // zase 0 kvoli cyklickemu citaniu vsetkych suborov
        file_num = 0;
    } else {
        file_num++;
    }
}

void my_window::load_data()
{
    // nacitanie prveho suboru s datami
    QString settings;
    QFile file;
    file.setFileName("data/scan0.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen()){
        qWarning() << "File open error\n";
        QCoreApplication::quit();
    }
    settings = file.readAll();
    file.close();
    // parsovanie jsonu
    QJsonDocument j_doc = QJsonDocument::fromJson(settings.toUtf8());
    if (j_doc.isNull()){
        qWarning() << "File reading error\n";
        QCoreApplication::quit();
    }
    QJsonObject tmp_obj = j_doc.object();
    QJsonArray j_arr = tmp_obj["networks"].toArray();
    foreach (const QJsonValue & value, j_arr) {
        QJsonObject obj = value.toObject();
        ssid_list.append(obj["ssid"].toString());
        channel_list.append(obj["channel"].toInt());
        freq_list.append(obj["frequency"].toDouble());
        sig_list.append(obj["sigstr"].toInt());
    }
}

void my_window::delete_old()
{
    int n = ssid_list.length();
    for (int i = n-1; i >= 0; i--){
        if (!idx_list.contains(i)){
            ssid_list.removeAt(i);
            channel_list.removeAt(i);
            freq_list.removeAt(i);
            sig_list.removeAt(i);
            delete bars[i];
            bars.removeAt(i);
            delete ssid_labels[i];
            ssid_labels.removeAt(i);
            delete channel_labels[i];
            channel_labels.removeAt(i);
            delete sig_labels[i];
            sig_labels.removeAt(i);
        }
    }
}

void my_window::timerEvent(QTimerEvent *event)
{
    update_data();
    delete_old();
}

void my_window::on_radioButton_clicked()
{
    mode = MODE_2;
    int n = freq_list.length();
    for (int i = 0; i < n; i++){
        if (freq_list[i] != 2.4){
            bars[i]->setVisible(false);
            ssid_labels[i]->setVisible(false);
            channel_labels[i]->setVisible(false);
            sig_labels[i]->setVisible(false);
        } else {
            bars[i]->setVisible(true);
            ssid_labels[i]->setVisible(true);
            channel_labels[i]->setVisible(true);
            sig_labels[i]->setVisible(true);
        }
    }
}

void my_window::on_radioButton_2_clicked()
{
    mode = MODE_5;
    int n = freq_list.length();
    for (int i = 0; i < n; i++){
        if (freq_list[i] != 5.0){
            bars[i]->setVisible(false);
            ssid_labels[i]->setVisible(false);
            channel_labels[i]->setVisible(false);
            sig_labels[i]->setVisible(false);
        } else {
            bars[i]->setVisible(true);
            ssid_labels[i]->setVisible(true);
            channel_labels[i]->setVisible(true);
            sig_labels[i]->setVisible(true);
        }
    }
}

void my_window::on_radioButton_3_clicked()
{
    mode = MODE_2_5;
    int q = ssid_labels.length();
    for (int i = 0; i < q; i++){
        bars[i]->setVisible(true);
        ssid_labels[i]->setVisible(true);
        channel_labels[i]->setVisible(true);
        sig_labels[i]->setVisible(true);
    }
}

void my_window::handleButton()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    int i = ssid_labels.indexOf(buttonSender);
    ui->label_4->setText(ssid_list[i]);
    ui->label_6->setText(QString::number(freq_list[i]).append(" Ghz"));
    ui->label_8->setText(QString::number(channel_list[i]));
    ui->label_2->setText(QString::number(sig_list[i]).append(" dBm"));
}
