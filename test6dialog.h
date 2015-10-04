#ifndef TEST6DIALOG_H
#define TEST6DIALOG_H

#include <QDialog>

namespace Ui {
class Test6Dialog;
}

class Test6Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Test6Dialog(QString,QString,QString,int,QWidget *parent = 0);
    ~Test6Dialog();
    QString getLegend(void);
    int getResults(void);

public slots:
    void accept(void);

private:
    Ui::Test6Dialog *ui;
    QString name;
    QString group;
    QString dt;
    int course;
    int idStudent;
    int res;
    void loadData(void);
    bool calcRes(int&,int*);
};

#endif // TEST6DIALOG_H
