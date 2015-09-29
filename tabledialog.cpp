#include "tabledialog.h"
#include "ui_tabledialog.h"

TableDialog::TableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    setupDialog();
}

TableDialog::~TableDialog()
{
    delete ui;
}

void TableDialog::setupDialog(void)
{
    QString sql = "SELECT student.id AS sid, student.fio AS fio, student.gr_id AS gr_id, testing.id AS tid, testing.a_date AS a_date, ank2.res AS res1, ank3.res AS res2, ank1.res AS res3  FROM student, testing, ank1, ank2, ank3 WHERE student.id = testing.student_id AND ank1.testing_id = testing.id AND ank2.testing_id = testing.id AND ank3.testing_id = testing.id";

    if (!beginQuery.exec(QString("SELECT tbl_student.f_name,tbl_student.f_group,tbl_student.f_class,f_res%1,f_legend%1 FROM tbl_results,tbl_student,tbl_test%1 WHERE tbl_test%1.id = tbl_results.f_id%1 AND tbl_student.id = tbl_results.f_student AND tbl_results.f_dt='%2' AND tbl_student.f_group IN (%3)").arg(currentTest + 1).arg(beginDate).arg(beginPredicate)))
     {
         QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
         qDebug() << beginQuery.lastError();
         return;
     }
}
