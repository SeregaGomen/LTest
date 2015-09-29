#include <QSqlQuery>
#include <QMessageBox>
#include "test4dialog.h"
#include "ui_test4dialog.h"

Test4Dialog::Test4Dialog(QString d,QString n,QString g,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test4Dialog)
{
    ui->setupUi(this);
    name = n;
    group = g;
    course = c;
    dt = d;
    idStudent = 0;
    setWindowTitle("Когнітивний тест з менеджменту - " + name);
    loadData();
}

Test4Dialog::~Test4Dialog()
{
    delete ui;
}

void Test4Dialog::loadData(void)
{
    QSqlQuery query(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
    QRadioButton* rb[][7] = {
                              {ui->rb11,ui->rb12,ui->rb13,ui->rb14,ui->rb15,ui->rb16,ui->rb17},
                              {ui->rb21,ui->rb22,ui->rb23,ui->rb24,ui->rb25,NULL,NULL},
                              {ui->rb31,ui->rb32,ui->rb33,ui->rb34,ui->rb35,NULL,NULL},
                              {ui->rb41,ui->rb42,ui->rb43,ui->rb44,ui->rb45,NULL,NULL},
                              {ui->rb51,ui->rb52,ui->rb53,ui->rb54,ui->rb55,NULL,NULL},
                              {ui->rb61,ui->rb62,ui->rb63,ui->rb64,ui->rb65,NULL,NULL},
                              {ui->rb71,ui->rb72,ui->rb73,ui->rb74,ui->rb75,NULL,NULL},
                              {ui->rb81,ui->rb82,ui->rb83,ui->rb84,ui->rb85,NULL,NULL}
    };
    int q;

    while (query.next())
        idStudent = query.value(0).toInt();
    if (idStudent)
    {
        // Такая анкета уже есть, загружаем ее
        query.exec(QString("SELECT * FROM tbl_test4 WHERE f_student = %1 AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            for (int i = 0; i < 8; i++)
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
                    case 5:
                        rb[i][4]->setChecked(true);
                        break;
                    case 6:
                        rb[i][5]->setChecked(true);
                        break;
                    case 7:
                        rb[i][6]->setChecked(true);
                        break;
                }
            }
    }
}

void Test4Dialog::accept(void)
{
    QSqlQuery query;
    QString resTxt,
            sql;
    int q[8],
        res,
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
    query.exec(QString("SELECT id FROM tbl_test4 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
    {
        id = query.value(0).toInt();
        isFind = true;
    }
    if (isFind)
        sql = QString("UPDATE tbl_test4 SET f_q1 = %1,f_q2 = %2,f_q3 = %3,f_q4 = %4,f_q5 = %5,f_q6 = %6,f_q7 = %7,f_q8 = %8 WHERE f_student = %9 AND f_dt = '%10'").arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_test4 (f_student,f_dt,f_q1,f_q2,f_q3,f_q4,f_q5,f_q6,f_q7,f_q8) VALUES (%1,'%2',%3,%4,%5,%6,%7,%8,%9,%10)").arg(idStudent).arg(dt).arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }
    // Определяем номер анкеты
    if (!id)
    {
        query.exec(QString("SELECT id FROM tbl_test4 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            id = query.value(0).toInt();
    }


    if (res < 5)
        resTxt = tr("недостатній");
    else if (res >= 5 && res < 8)
        resTxt = tr("достатній");
    else
        resTxt = tr("професійний");
    QMessageBox::information(this, tr("Результат"),tr("Рівень когнітивного компоненту готовності: %1").arg(resTxt), QMessageBox::Ok);

    // Сохраняем результаты
    isFind = false;
    query.exec(QString("SELECT * FROM tbl_results WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
        isFind = true;
    if (isFind)
        sql = QString("UPDATE tbl_results SET f_id4 = %1,f_res4 = %2,f_legend4 = '%3' WHERE f_student = '%4' AND f_dt = '%5'").arg(id).arg(res).arg(resTxt).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_results (f_student,f_dt,f_id4,f_res4,f_legend4) VALUES (%1,'%2',%3,%4,'%5')").arg(idStudent).arg(dt).arg(id).arg(res).arg(resTxt);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }

    QDialog::accept();
}

bool Test4Dialog::calcRes(int& res,int* q)
{
    QRadioButton* rb[][7] = {
                              {ui->rb11,ui->rb12,ui->rb13,ui->rb14,ui->rb15,ui->rb16,ui->rb17},
                              {ui->rb21,ui->rb22,ui->rb23,ui->rb24,ui->rb25,NULL,NULL},
                              {ui->rb31,ui->rb32,ui->rb33,ui->rb34,ui->rb35,NULL,NULL},
                              {ui->rb41,ui->rb42,ui->rb43,ui->rb44,ui->rb45,NULL,NULL},
                              {ui->rb51,ui->rb52,ui->rb53,ui->rb54,ui->rb55,NULL,NULL},
                              {ui->rb61,ui->rb62,ui->rb63,ui->rb64,ui->rb65,NULL,NULL},
                              {ui->rb71,ui->rb72,ui->rb73,ui->rb74,ui->rb75,NULL,NULL},
                              {ui->rb81,ui->rb82,ui->rb83,ui->rb84,ui->rb85,NULL,NULL}
    };

    res = 0;
    // Проверка на наличие ответов на все вопросы анкеты
    for (int i = 0; i < 8; i++)
    {
        q[i] = 0;
        if (rb[i][0]->isChecked())
            q[i] = 1;
        else if (rb[i][1]->isChecked())
            q[i] = 2;
        else if (rb[i][2]->isChecked())
            q[i] = 3;
        else if (rb[i][3]->isChecked())
            q[i] = 4;
        else if (rb[i][4]->isChecked())
            q[i] = 5;
        else if (rb[i][5] && rb[i][5]->isChecked())
            q[i] = 6;
        else if (rb[i][6] && rb[i][6]->isChecked())
            q[i] = 7;
        if (q[i] == 0)
        {
            QMessageBox::information(this, tr("Помилка"), tr("Не задана відповіднь на питання: %1!").arg(i + 1));
            return false;
        }
    }
    if (q[0] == 6)
        res += 1;
    if (q[1] == 1)
        res += 1;
    if (q[2] == 3)
        res += 1;
    if (q[3] == 3)
        res += 1;
    if (q[4] == 1)
        res += 1;
    if (q[5] == 2)
        res += 1;
    if (q[6] == 2)
        res += 1;
    if (q[7] == 1)
        res += 1;
    return true;
}
