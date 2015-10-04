#include <QSqlQuery>
#include <QMessageBox>
#include "test2dialog.h"
#include "ui_test2dialog.h"

Test2Dialog::Test2Dialog(QString d,QString n,QString g,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test2Dialog)
{
    ui->setupUi(this);
    name = n;
    group = g;
    course = c;
    dt = d;
    idStudent = 0;
    setWindowTitle("Інформаційно-технологічний тест - " + name);
    loadData();
}

Test2Dialog::~Test2Dialog()
{
    delete ui;
}

void Test2Dialog::loadData(void)
{
    QSqlQuery query(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
    QRadioButton* rb[][4] = {
                              {ui->rb11,ui->rb12,ui->rb13,ui->rb14},
                              {ui->rb21,ui->rb22,ui->rb23,ui->rb24},
                              {ui->rb31,ui->rb32,ui->rb33,ui->rb34},
                              {ui->rb41,ui->rb42,ui->rb43,ui->rb44},
                              {ui->rb51,ui->rb52,ui->rb53,ui->rb54},
                              {ui->rb61,ui->rb62,ui->rb63,ui->rb64},
                              {ui->rb71,ui->rb72,ui->rb73,ui->rb74}
    };
    int q;

    while (query.next())
        idStudent = query.value(0).toInt();
    if (idStudent)
    {
        // Такая анкета уже есть, загружаем ее
        query.exec(QString("SELECT * FROM tbl_test2 WHERE f_student = %1 AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            for (int i = 0; i < 7; i++)
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
                    case 4:
                        rb[i][3]->setChecked(true);
                        break;
                }
            }
    }
}

void Test2Dialog::accept(void)
{
    QSqlQuery query;
    QString resTxt,
            sql;
    int q[9],
        id = 0;
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
    query.exec(QString("SELECT id FROM tbl_test2 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
    {
        id = query.value(0).toInt();
        isFind = true;
    }
    if (isFind)
        sql = QString("UPDATE tbl_test2 SET f_q1 = %1,f_q2 = %2,f_q3 = %3,f_q4 = %4,f_q5 = %5,f_q6 = %6,f_q7 = %7 WHERE f_student = %8 AND f_dt = '%9'").arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_test2 (f_student,f_dt,f_q1,f_q2,f_q3,f_q4,f_q5,f_q6,f_q7) VALUES (%1,'%2',%3,%4,%5,%6,%7,%8,%9)").arg(idStudent).arg(dt).arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }
    // Определяем номер анкеты
    if (!id)
    {
        query.exec(QString("SELECT id FROM tbl_test2 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            id = query.value(0).toInt();
    }

    if (res < 7)
        resTxt = tr("недостатній");
    else if (res == 7)
        resTxt = tr("достатній");
    else
        resTxt = tr("професійний");
    QMessageBox::information(this, tr("Результат"),tr("Рівень інформаційно-технологічного компоненту готовності: %1").arg(resTxt), QMessageBox::Ok);


    // Сохраняем результаты
    isFind = false;
    query.exec(QString("SELECT * FROM tbl_results WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
        isFind = true;
    if (isFind)
        sql = QString("UPDATE tbl_results SET f_id2 = %1,f_res2 = %2,f_legend2 = '%3' WHERE f_student = '%4' AND f_dt = '%5'").arg(id).arg(res).arg(resTxt).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_results (f_student,f_dt,f_id2,f_res2,f_legend2) VALUES (%1,'%2',%3,%4,'%5')").arg(idStudent).arg(dt).arg(id).arg(res).arg(resTxt);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }

    QDialog::accept();
}

bool Test2Dialog::calcRes(int& res,int* q)
{
    QRadioButton* rb[][4] = {
                              {ui->rb11,ui->rb12,ui->rb13,ui->rb14},
                              {ui->rb21,ui->rb22,ui->rb23,ui->rb24},
                              {ui->rb31,ui->rb32,ui->rb33,ui->rb34},
                              {ui->rb41,ui->rb42,ui->rb43,ui->rb44},
                              {ui->rb51,ui->rb52,ui->rb53,ui->rb54},
                              {ui->rb61,ui->rb62,ui->rb63,ui->rb64},
                              {ui->rb71,ui->rb72,ui->rb73,ui->rb74}
    };

    res = 0;
    // Проверка на наличие ответов на все вопросы анкеты
    for (int i = 0; i < 7; i++)
    {
        q[i] = 0;
        if (rb[i][0]->isChecked())
        {
            q[i] = 1;
            res += 0;
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
        else if (rb[i][3]->isChecked())
        {
            q[i] = 4;
            res += 3;
        }
        if (q[i] == 0)
        {
            QMessageBox::information(this, tr("Помилка"), tr("Не задана відповіднь на питання: %1!").arg(i + 1));
            return false;
        }
    }
    return true;
}

QString Test2Dialog::getLegend(void)
{
    QString resTxt;

    if (res < 7)
        resTxt = tr("недостатній");
    else if (res == 7)
        resTxt = tr("достатній");
    else
        resTxt = tr("професійний");

    return resTxt;
}

int Test2Dialog::getResults(void)
{
    return res;
}
