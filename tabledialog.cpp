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

    "select tbl_student.id,tbl_student.f_group,tbl_student.f_name,tbl_test1.f_res,tbl_test2.f_res,tbl_test1.f_dt FROM tbl_student,tbl_test1,tbl_test2 WHERE tbl_student.id = tbl_test1.f_student AND tbl_student.id = tbl_test2.f_student AND tbl_test1.f_dt = tbl_test2.f_dt";
}
