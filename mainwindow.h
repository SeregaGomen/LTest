#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotTest1(void);
    void slotTest2(void);
    void slotTest3(void);
    void slotTest4(void);
    void slotTest5(void);
    void slotTest6(void);
    void slotStudent(void);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    void readSettings(void);
    void writeSettings(void);
    void openDB(void);
    void closeDB(void);
    void setupLanguage(void);
    void calcStudent(int,QString,QString,QStringList,QStringList);
    bool checkDB(QString);
    bool createDB(QString);
};

#endif // MAINWINDOW_H
