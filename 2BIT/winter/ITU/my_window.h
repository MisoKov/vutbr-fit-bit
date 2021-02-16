#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QProgressBar>
#include <QGridLayout>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class my_window; }
QT_END_NAMESPACE

class my_window : public QMainWindow
{
    Q_OBJECT

public:
    my_window(QWidget *parent = nullptr);
    ~my_window();

private:
    Ui::my_window *ui;
    void load_data();
    void update_data();
    void delete_old();
    int timerId;
    int mode;
    QGridLayout *group_grid;
    QList<QProgressBar *> bars;
    QList<int> idx_list;
    QStringList ssid_list;
    QList<int> channel_list;
    QList<double> freq_list;
    QList<int> sig_list;
    QList<QPushButton*> ssid_labels;
    QList<QLabel*> channel_labels;
    QList<QLabel*> sig_labels;

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void handleButton();
};
#endif // MY_WINDOW_H
