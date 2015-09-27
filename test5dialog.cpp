#include <QSqlQuery>
#include <QMessageBox>
#include "test5dialog.h"
#include "ui_test5dialog.h"

Test5Dialog::Test5Dialog(QString d,QString n,QString g,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test5Dialog)
{
    ui->setupUi(this);
    name = n;
    group = g;
    course = c;
    dt = d;
    idStudent = 0;
    setWindowTitle("Особистісно-творчий тест - " + name);
    loadData();
}

Test5Dialog::~Test5Dialog()
{
    delete ui;
}

void Test5Dialog::loadData(void)
{
    QSqlQuery query(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
    QRadioButton* rb[][3] = {
                              {ui->rb11,ui->rb12,ui->rb13},
                              {ui->rb21,ui->rb22,ui->rb23},
                              {ui->rb31,ui->rb32,ui->rb33},
                              {ui->rb41,ui->rb42,ui->rb43},
                              {ui->rb51,ui->rb52,ui->rb53},
                              {ui->rb61,ui->rb62,ui->rb63},
                              {ui->rb71,ui->rb72,ui->rb73},
                              {ui->rb81,ui->rb82,ui->rb83},
                              {ui->rb91,ui->rb92,ui->rb93},
                              {ui->rb101,ui->rb102,ui->rb103},
                              {ui->rb111,ui->rb112,ui->rb113},
                              {ui->rb121,ui->rb122,ui->rb123},
                              {ui->rb131,ui->rb132,ui->rb133},
                              {ui->rb141,ui->rb142,ui->rb143},
                              {ui->rb151,ui->rb152,ui->rb153},
                              {ui->rb161,ui->rb162,ui->rb163},
                              {ui->rb171,ui->rb172,ui->rb173},
                              {ui->rb181,ui->rb182,ui->rb183},
    };
    int q;

    while (query.next())
        idStudent = query.value(0).toInt();
    if (idStudent)
    {
        // Такая анкета уже есть, загружаем ее
        query.exec(QString("SELECT * FROM tbl_test5 WHERE f_student = %1 AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            for (int i = 0; i < 18; i++)
            {
                q = query.value(3 + i).toInt();
                switch (q)
                {
                    case 1:
                        rb[i][0]->setChecked(true);
                        break;
                    case 2:
                        rb[i][1]->setChecked(true);
                        break;
                    case 3:
                        rb[i][2]->setChecked(true);
                        break;
                }
            }
    }
}

void Test5Dialog::accept(void)
{
    QSqlQuery query;
    QString resTxt,
            sql;
    int q[18],
        res;
    bool isFind = false;

    if (!calcRes(res,q))
        return;
    if (!idStudent)
    {
        // Сохраняем информацию о тестируемом
        if (!query.exec(QString("INSERT INTO tbl_student (f_name,f_group,f_class) VALUES ('%1','%2',%3)").arg(name.toUpper()).arg(group.toUpper()).arg(course)))
        {
            QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
            return;
        }

        query.exec(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
        while (query.next())
            idStudent = query.value(0).toInt();
    }
    // Сохраняем анекту
    query.exec(QString("SELECT * FROM tbl_test5 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
        isFind = true;
    if (isFind)
        sql = QString("UPDATE tbl_test5 SET f_q1 = %1,f_q2 = %2,f_q3 = %3,f_q4 = %4,f_q5 = %5,f_q6 = %6,f_q7 = %7,f_q8 = %8,f_q9 = %9,f_q10 = %10,f_q11 = %11,f_q12 = %12,f_q13 = %13,f_q14 = %14,f_q15 = %15,f_q16 = %16,f_q17 = %17,f_q18 = %18,f_res = %19 WHERE f_student = %20 AND f_dt = '%21'").arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(q[12]).arg(q[13]).arg(q[14]).arg(q[15]).arg(q[16]).arg(q[17]).arg(res).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_test5 (f_student,f_dt,f_q1,f_q2,f_q3,f_q4,f_q5,f_q6,f_q7,f_q8,f_q9,f_q10,f_q11,f_q12,f_q13,f_q14,f_q15,f_q16,f_q17,f_q18,f_res) VALUES (%1,'%2',%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20,%21)").arg(idStudent).arg(dt).arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(q[12]).arg(q[13]).arg(q[14]).arg(q[15]).arg(q[16]).arg(q[17]).arg(res);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }

    if (res < 24)
        resTxt = tr("Рівень особистісно-творчого компоненту готовності: %1").arg("недостатній");
    else if (res >= 24 && res < 36)
        resTxt = tr("Рівень особистісно-творчого компоненту готовності: %1").arg("достатній");
    else
        resTxt = tr("Рівень особистісно-творчого компоненту готовності: %1").arg("професійний");
    QMessageBox::information(this, tr("Результат"),resTxt, QMessageBox::Ok);
    QDialog::accept();
}

bool Test5Dialog::calcRes(int& res,int* q)
{
    QRadioButton* rb[][3] = {
                              {ui->rb11,ui->rb12,ui->rb13},
                              {ui->rb21,ui->rb22,ui->rb23},
                              {ui->rb31,ui->rb32,ui->rb33},
                              {ui->rb41,ui->rb42,ui->rb43},
                              {ui->rb51,ui->rb52,ui->rb53},
                              {ui->rb61,ui->rb62,ui->rb63},
                              {ui->rb71,ui->rb72,ui->rb73},
                              {ui->rb81,ui->rb82,ui->rb83},
                              {ui->rb91,ui->rb92,ui->rb93},
                              {ui->rb101,ui->rb102,ui->rb103},
                              {ui->rb111,ui->rb112,ui->rb113},
                              {ui->rb121,ui->rb122,ui->rb123},
                              {ui->rb131,ui->rb132,ui->rb133},
                              {ui->rb141,ui->rb142,ui->rb143},
                              {ui->rb151,ui->rb152,ui->rb153},
                              {ui->rb161,ui->rb162,ui->rb163},
                              {ui->rb171,ui->rb172,ui->rb173},
                              {ui->rb181,ui->rb182,ui->rb183},
    };

    res = 0;
    for (int i = 0; i < 18; i++)
    {
        q[i] = 0;
        if (rb[i][0]->isChecked())
        {
            q[i] = 1;
            res += 3;
        }
        else if (rb[i][1]->isChecked())
        {
            q[i] = 2;
            res += 1;
        }
        else if (rb[i][2]->isChecked())
        {
            q[i] = 3;
            res += 2;
        }
        if (q[i] == 0)
        {
            QMessageBox::information(this, tr("Помилка"), tr("Не задана відповіднь на питання: %1!").arg(i + 1));
            return false;
        }
    }
    return true;
}
