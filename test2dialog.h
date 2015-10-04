#ifndef TEST2DIALOG_H
#define TEST2DIALOG_H

#include <QDialog>

namespace Ui {
class Test2Dialog;
}

class Test2Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Test2Dialog(QString,QString,QString,int,QWidget *parent = 0);
    ~Test2Dialog();
    QString getLegend(void);
    int getResults(void);

public slots:
    void accept(void);

private:
    Ui::Test2Dialog *ui;
    QString name;
    QString group;
    QString dt;
    int course;
    int idStudent;
    int res;
    void loadData(void);
    bool calcRes(int&,int*);
};

#endif // TEST2DIALOG_H
