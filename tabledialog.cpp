#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
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
    QSqlQuery query;
    int i = 0;



    if (!query.exec(QString("SELECT tbl_student.f_name,tbl_student.f_group,tbl_student.f_class,tbl_results.f_dt,f_res1,f_legend1,f_res2,f_legend2,f_res3,f_legend3,f_res4,f_legend4,f_res5,f_legend5,f_res6,f_legend6,tbl_results.id,f_id1,f_id2,f_id3,f_id4,f_id5,f_id6  \
                             FROM tbl_results,tbl_student,tbl_test1,tbl_test2,tbl_test3,tbl_test4,tbl_test5,tbl_test6 \
                             WHERE tbl_test1.id = tbl_results.f_id1 AND tbl_test2.id = tbl_results.f_id2 AND tbl_test3.id = tbl_results.f_id3 AND tbl_test4.id = tbl_results.f_id4 AND tbl_test5.id = tbl_results.f_id5 AND tbl_test6.id = tbl_results.f_id6 AND tbl_student.id = tbl_results.f_student \
                             ORDER BY tbl_results.f_dt,tbl_student.f_group,tbl_student.f_name")))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
        qDebug() << query.lastError();
        return;
    }

    while (query.next())
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));  // ПІБ
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));  // Група
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));  // Курс
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));  // Дата
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(5).toString()).arg(query.value(4).toString()))); // Тест1
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(7).toString()).arg(query.value(6).toString()))); // Тест2
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(9).toString()).arg(query.value(8).toString()))); // Тест3
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(11).toString()).arg(query.value(10).toString()))); // Тест4
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(13).toString()).arg(query.value(12).toString()))); // Тест5
        ui->tableWidget->setItem(i, 9, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(15).toString()).arg(query.value(14).toString()))); // Тест6

        ui->tableWidget->setItem(i, 10, new QTableWidgetItem(query.value(16).toString()));  // Id
        ui->tableWidget->setItem(i, 11, new QTableWidgetItem(query.value(17).toString()));  // Id1
        ui->tableWidget->setItem(i, 12, new QTableWidgetItem(query.value(18).toString()));  // Id2
        ui->tableWidget->setItem(i, 13, new QTableWidgetItem(query.value(19).toString()));  // Id3
        ui->tableWidget->setItem(i, 14, new QTableWidgetItem(query.value(20).toString()));  // Id4
        ui->tableWidget->setItem(i, 15, new QTableWidgetItem(query.value(21).toString()));  // Id5
        ui->tableWidget->setItem(i, 16, new QTableWidgetItem(query.value(22).toString()));  // Id6

        i++;
    }
    ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 10; i < 17; i++)
        ui->tableWidget->setColumnHidden(i,true);

    connect(ui->tableWidget->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(setEnabledBtn(QItemSelection, QItemSelection)));
}

void TableDialog::setEnabledBtn(QItemSelection,QItemSelection)
{
    ui->tbFirst->setEnabled(ui->tableWidget->selectionModel()->hasSelection());
    ui->tbPrev->setEnabled(ui->tableWidget->selectionModel()->hasSelection());
    ui->tbNext->setEnabled(ui->tableWidget->selectionModel()->hasSelection());
    ui->tbLast->setEnabled(ui->tableWidget->selectionModel()->hasSelection());
    ui->tbDel->setEnabled(ui->tableWidget->selectionModel()->hasSelection());
}

void TableDialog::checkButtons(void)
{
    if (ui->tableWidget->rowCount())
    {
    }
}
