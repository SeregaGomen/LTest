#ifndef TEST1DIALOG_H
#define TEST1DIALOG_H

#include <QDialog>

namespace Ui {
class Test1Dialog;
}

class Test1Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Test1Dialog(QString,QString,QString,int,QWidget *parent = 0);
    ~Test1Dialog();

public slots:
    void accept(void);

private:
    Ui::Test1Dialog *ui;
    QString name;
    QString group;
    QString dt;
    int course;
    int idStudent;
    int res;
    void loadData(void);
    bool calcRes(int&,int*);
};

#endif // TEST1DIALOG_H
