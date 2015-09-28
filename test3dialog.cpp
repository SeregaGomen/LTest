#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include "test3dialog.h"
#include "ui_test3dialog.h"

// Коды вариантов ответов
#define _1_ 1 // 0000000000000001
#define _2_ 2 // 0000000000000010
#define _3_ 4 // 0000000000000100
#define _4_ 8 // 0000000000001000

Test3Dialog::Test3Dialog(QString d,QString n,QString g,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test3Dialog)
{
    ui->setupUi(this);
    name = n;
    group = g;
    course = c;
    dt = d;
    idStudent = 0;
    setWindowTitle("Когнітивний тест з маркетингу туризму - " + name);
    loadData();
}

Test3Dialog::~Test3Dialog()
{
    delete ui;
}

void Test3Dialog::loadData(void)
{
    QSqlQuery query(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
    QCheckBox* cb[][4] = {
                              {ui->cb11,ui->cb12,ui->cb13,NULL},
                              {ui->cb21,ui->cb22,ui->cb23,NULL},
                              {ui->cb31,ui->cb32,ui->cb33,NULL},
                              {ui->cb41,ui->cb42,ui->cb43,ui->cb44},
                              {ui->cb51,ui->cb52,ui->cb53,NULL},
                              {ui->cb61,ui->cb62,ui->cb63,ui->cb64},
                              {ui->cb71,ui->cb72,ui->cb73,ui->cb74},
                              {ui->cb81,ui->cb82,NULL,NULL}
    };
    int q;

    while (query.next())
        idStudent = query.value(0).toInt();
    if (idStudent)
    {
        // Такая анкета уже есть, загружаем ее
        query.exec(QString("SELECT * FROM tbl_test3 WHERE f_student = %1 AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            for (int i = 0; i < 8; i++)
            {
                q = query.value(3 + i).toInt();
                if ((q & _1_) == _1_)
                    cb[i][0]->setChecked(true);
                if ((q & _2_) == _2_)
                    cb[i][1]->setChecked(true);
                if ((q & _3_) == _3_)
                    cb[i][2]->setChecked(true);
                if ((q & _4_) == _4_)
                    cb[i][3]->setChecked(true);
            }
    }
}

void Test3Dialog::accept(void)
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
    query.exec(QString("SELECT id FROM tbl_test3 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
    {
        id = query.value(0).toInt();
        isFind = true;
    }
    if (isFind)
        sql = QString("UPDATE tbl_test3 SET f_q1 = %1,f_q2 = %2,f_q3 = %3,f_q4 = %4,f_q5 = %5,f_q6 = %6,f_q7 = %7,f_q8 = %8 WHERE f_student = %9 AND f_dt = '%10'").arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_test3 (f_student,f_dt,f_q1,f_q2,f_q3,f_q4,f_q5,f_q6,f_q7,f_q8) VALUES (%1,'%2',%3,%4,%5,%6,%7,%8,%9,%10)").arg(idStudent).arg(dt).arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }
    // Определяем номер анкеты
    if (!id)
    {
        query.exec(QString("SELECT id FROM tbl_test3 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            id = query.value(0).toInt();
    }



    if (res < 10)
        resTxt = tr("недостатній");
    else if (res >= 10 && res < 16)
        resTxt = tr("достатній");
    else
        resTxt = tr("професійний");
    QMessageBox::information(this, tr("Результат"),tr("Рівень когнітивного компоненту компоненту готовності: %1").arg(resTxt), QMessageBox::Ok);

    // Сохраняем результаты
    isFind = false;
    query.exec(QString("SELECT * FROM tbl_results WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
        isFind = true;
    if (isFind)
        sql = QString("UPDATE tbl_results SET f_id3 = %1,f_res3 = %2,f_legend3 = '%3' WHERE f_student = '%4' AND f_dt = '%5'").arg(id).arg(res).arg(resTxt).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_results (f_student,f_dt,f_id3,f_res3,f_legend3) VALUES (%1,'%2',%3,%4,'%5')").arg(idStudent).arg(dt).arg(id).arg(res).arg(resTxt);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }

    QDialog::accept();
}

bool Test3Dialog::calcRes(int& res,int* q)
{
    QCheckBox* cb[][4] = {
                              {ui->cb11,ui->cb12,ui->cb13,NULL},
                              {ui->cb21,ui->cb22,ui->cb23,NULL},
                              {ui->cb31,ui->cb32,ui->cb33,NULL},
                              {ui->cb41,ui->cb42,ui->cb43,ui->cb44},
                              {ui->cb51,ui->cb52,ui->cb53,NULL},
                              {ui->cb61,ui->cb62,ui->cb63,ui->cb64},
                              {ui->cb71,ui->cb72,ui->cb73,ui->cb74},
                              {ui->cb81,ui->cb82,NULL,NULL}
    };
    res = 0;
    for (int i = 0; i < 8; i++)
    {
        q[i] = 0;
        if (cb[i][0]->isChecked())
            q[i] |= _1_;
        if (cb[i][1]->isChecked())
            q[i] |= _2_;
        if (cb[i][2] && cb[i][2]->isChecked())
            q[i] |= _3_;
        if (cb[i][3] && cb[i][3]->isChecked())
            q[i] |= _4_;
        if (q[i] == 0)
        {
            QMessageBox::information(this, tr("Помилка"), tr("Не задана відповіднь на питання: %1!").arg(i + 1));
            return false;
        }
    }
    if (q[0] == (_1_ | _2_))
        res += 2;
    if (q[1] == (_2_ | _3_))
        res += 2;
    if (q[2] == (_1_ | _2_))
        res += 2;
    if (q[3] == (_1_ | _2_ | _3_))
        res += 3;
    if (q[4] == (_1_ | _2_))
        res += 2;
    if (q[5] == (_1_ | _2_))
        res += 2;
    if (q[6] == (_1_ | _2_))
        res += 2;
    if (q[7] == _1_)
        res += 1;
    return true;
}
