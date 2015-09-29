#include <QSqlQuery>
#include <QMessageBox>
#include "test6dialog.h"
#include "ui_test6dialog.h"

Test6Dialog::Test6Dialog(QString d,QString n,QString g,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test6Dialog)
{
    ui->setupUi(this);
    name = n;
    group = g;
    course = c;
    dt = d;
    idStudent = 0;
    setWindowTitle("Мотиваційний тест - " + name);
    loadData();
}

Test6Dialog::~Test6Dialog()
{
    delete ui;
}

void Test6Dialog::loadData(void)
{
    QSqlQuery query(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
    QRadioButton* rb[][2] = {
                              {ui->rb11,ui->rb12},
                              {ui->rb21,ui->rb22},
                              {ui->rb31,ui->rb32},
                              {ui->rb41,ui->rb42},
                              {ui->rb51,ui->rb52},
                              {ui->rb61,ui->rb62},
                              {ui->rb71,ui->rb72},
                              {ui->rb81,ui->rb82},
                              {ui->rb91,ui->rb92},
                              {ui->rb101,ui->rb102},
                              {ui->rb111,ui->rb112},
                              {ui->rb121,ui->rb122},
                              {ui->rb131,ui->rb132},
                              {ui->rb141,ui->rb142},
                              {ui->rb151,ui->rb152},
                              {ui->rb161,ui->rb162},
                              {ui->rb171,ui->rb172},
                              {ui->rb181,ui->rb182},
                              {ui->rb191,ui->rb192},
                              {ui->rb201,ui->rb202},
                              {ui->rb211,ui->rb212},
                              {ui->rb221,ui->rb222},
                              {ui->rb231,ui->rb232},
    };
    int q;

    while (query.next())
        idStudent = query.value(0).toInt();
    if (idStudent)
    {
        // Такая анкета уже есть, загружаем ее
        query.exec(QString("SELECT * FROM tbl_test6 WHERE f_student = %1 AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            for (int i = 0; i < 23; i++)
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
                }
            }
    }
}

void Test6Dialog::accept(void)
{
    QSqlQuery query;
    QString resTxt,
            sql;
    int q[23],
        id = 0,
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
    query.exec(QString("SELECT id FROM tbl_test6 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
    {
        id = query.value(0).toInt();
        isFind = true;
    }
    if (isFind)
        sql = QString("UPDATE tbl_test6 SET f_q1 = %1,f_q2 = %2,f_q3 = %3,f_q4 = %4,f_q5 = %5,f_q6 = %6,f_q7 = %7,f_q8 = %8,f_q9 = %9,f_q10 = %10,f_q11 = %11,f_q12 = %12,f_q13 = %13,f_q14 = %14,f_q15 = %15,f_q16 = %16,f_q17 = %17,f_q18 = %18,f_q19 = %19,f_q20 = %20,f_q21 = %21,f_q22 = %22,f_q23 = %23 WHERE f_student = %24 AND f_dt = '%25'").arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(q[12]).arg(q[13]).arg(q[14]).arg(q[15]).arg(q[16]).arg(q[17]).arg(q[18]).arg(q[19]).arg(q[20]).arg(q[21]).arg(q[22]).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_test6 (f_student,f_dt,f_q1,f_q2,f_q3,f_q4,f_q5,f_q6,f_q7,f_q8,f_q9,f_q10,f_q11,f_q12,f_q13,f_q14,f_q15,f_q16,f_q17,f_q18,f_q19,f_q20,f_q21,f_q22,f_q23) VALUES (%1,'%2',%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20,%21,%22,%23,%24,%25)").arg(idStudent).arg(dt).arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(q[12]).arg(q[13]).arg(q[14]).arg(q[15]).arg(q[16]).arg(q[17]).arg(q[18]).arg(q[19]).arg(q[20]).arg(q[21]).arg(q[22]);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }
    // Определяем номер анкеты
    if (!id)
    {
        query.exec(QString("SELECT id FROM tbl_test6 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            id = query.value(0).toInt();
    }

    if (res < 12)
        resTxt = tr("недостатній");
    else if (res >= 12 && res < 16)
        resTxt = tr("достатній");
    else
        resTxt = tr("професійний");
    QMessageBox::information(this, tr("Результат"),tr("Рівень мотиваційного компоненту готовності: %1").arg(resTxt), QMessageBox::Ok);

    // Сохраняем результаты
    isFind = false;
    query.exec(QString("SELECT * FROM tbl_results WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
        isFind = true;
    if (isFind)
        sql = QString("UPDATE tbl_results SET f_id6 = %1,f_res6 = %2,f_legend6 = '%3' WHERE f_student = '%4' AND f_dt = '%5'").arg(id).arg(res).arg(resTxt).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_results (f_student,f_dt,f_id6,f_res6,f_legend6) VALUES (%1,'%2',%3,%4,'%5')").arg(idStudent).arg(dt).arg(id).arg(res).arg(resTxt);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }

    QDialog::accept();
}

bool Test6Dialog::calcRes(int& res,int* q)
{
    QRadioButton* rb[][2] = {
                              {ui->rb11,ui->rb12},
                              {ui->rb21,ui->rb22},
                              {ui->rb31,ui->rb32},
                              {ui->rb41,ui->rb42},
                              {ui->rb51,ui->rb52},
                              {ui->rb61,ui->rb62},
                              {ui->rb71,ui->rb72},
                              {ui->rb81,ui->rb82},
                              {ui->rb91,ui->rb92},
                              {ui->rb101,ui->rb102},
                              {ui->rb111,ui->rb112},
                              {ui->rb121,ui->rb122},
                              {ui->rb131,ui->rb132},
                              {ui->rb141,ui->rb142},
                              {ui->rb151,ui->rb152},
                              {ui->rb161,ui->rb162},
                              {ui->rb171,ui->rb172},
                              {ui->rb181,ui->rb182},
                              {ui->rb191,ui->rb192},
                              {ui->rb201,ui->rb202},
                              {ui->rb211,ui->rb212},
                              {ui->rb221,ui->rb222},
                              {ui->rb231,ui->rb232},
    };

    res = 0;
    // Проверка на наличие ответов на все вопросы анкеты
    for (int i = 0; i < 23; i++)
    {
        q[i] = 0;
        if (rb[i][0]->isChecked())
            q[i] = 1;
        else if (rb[i][1]->isChecked())
            q[i] = 2;
        if (q[i] == 0)
        {
            QMessageBox::information(this, tr("Помилка"), tr("Не задана відповіднь на питання: %1!").arg(i + 1));
            return false;
        }
        if ((i == 1 || i == 5 || i == 6 || i == 7 || i == 13 || i == 15 || i == 17 || i == 18 || i == 20 || i == 21 || i == 22) && q[i] == 1)
            res++;

        if ((i == 0 || i == 2 || i == 3 || i == 4 || i == 8 || i == 9 || i == 10 || i == 11 || i == 12 || i == 14 || i == 16 || i == 19) && q[i] == 2)
            res++;
    }
    return true;
}
