#ifndef TEST5DIALOG_H
#define TEST5DIALOG_H

#include <QDialog>

namespace Ui {
class Test5Dialog;
}

class Test5Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Test5Dialog(QString,QString,QString,int,QWidget *parent = 0);
    ~Test5Dialog();
    QString getLegend(void);
    int getResults(void);

public slots:
    void accept(void);

private:
    Ui::Test5Dialog *ui;
    QString name;
    QString group;
    QString dt;
    int course;
    int idStudent;
    int res;
    void loadData(void);
    bool calcRes(int&,int*);
};

#endif // TEST5DIALOG_H
