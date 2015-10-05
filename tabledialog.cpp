#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "tabledialog.h"
#include "ui_tabledialog.h"
#include "test1dialog.h"
#include "test2dialog.h"
#include "test3dialog.h"
#include "test4dialog.h"
#include "test5dialog.h"
#include "test6dialog.h"


TableDialog::TableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    createMenu();
    setupDialog();
}

TableDialog::~TableDialog()
{
    delete ui;
}

void TableDialog::createMenu(void)
{
    delMenu = menu.addMenu("&Вилучити");
    delMenu->addAction("&Всі поточні тести...");
    delMenu->addAction("&Діяльнісно-операційний тест...");
    delMenu->addAction("&Інформаційно-технологічний тест...");
    delMenu->addAction("Когнітивний тест з маркетингу &туризму");
    delMenu->addAction("Когнітивний тест з &менеджменту...");
    delMenu->addAction("&Особистісно-творчий тест...");
    delMenu->addAction("Моти&ваційний тест...");
}

void TableDialog::removeTest(int no)
{
    QSqlQuery query;
    QString dt = ui->tableWidget->model()->index(ui->tableWidget->currentRow(),3).data().toString();
    int idStudent = ui->tableWidget->model()->index(ui->tableWidget->currentRow(),10).data().toInt(),
        id = ui->tableWidget->model()->index(ui->tableWidget->currentRow(),11).data().toInt();

    if (QMessageBox::question(this,tr("Підтвердження"),tr("Ви впевнені, що бажаєте вилучити інформацію?"),QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;

    if (no == 0)
    {
        // Удалить все анкеты
        for (int i = 1; i < 7; i++)
            if (!query.exec(QString("DELETE FROM tbl_test%1 WHERE f_student = %2 AND f_dt = '%3'").arg(i).arg(idStudent).arg(dt)))
            {
                QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
                qDebug() << query.lastError();
                return;
            }
        if (!query.exec(QString("DELETE FROM tbl_results WHERE id = %1").arg(id)))
        {
            QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
            qDebug() << query.lastError();
            return;
        }
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    }
    else
    {
        if (!query.exec(QString("DELETE FROM tbl_test%1 WHERE f_student = %2 AND f_dt = '%3'").arg(no).arg(idStudent).arg(dt)))
        {
            QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
            qDebug() << query.lastError();
            return;
        }
        if (!query.exec(QString("UPDATE tbl_results SET f_id%1 = NULL,f_res%1 = NULL,f_legend%1 = NULL WHERE id = %2").arg(no).arg(id)))
        {
            QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
            qDebug() << query.lastError();
            return;
        }
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 3 + no, new QTableWidgetItem(QString("")));
    }
}


void TableDialog::showContextMenu(const QPoint &pos)
{
    QPoint globalPos;
    QAction* selectedItem;


    if (sender()->inherits("QAbstractScrollArea"))
        globalPos = ((QAbstractScrollArea*)sender())->viewport()->mapToGlobal(pos);
    else
        globalPos = ((QWidget*)sender())->mapToGlobal(pos);

    if ((selectedItem = menu.exec(globalPos)))
    {
        if (selectedItem == delMenu->actions().at(0))
            removeTest(0);
        else if (selectedItem == delMenu->actions().at(1))
            removeTest(1);
        else if (selectedItem == delMenu->actions().at(2))
            removeTest(2);
        else if (selectedItem == delMenu->actions().at(3))
            removeTest(3);
        else if (selectedItem == delMenu->actions().at(4))
            removeTest(4);
        else if (selectedItem == delMenu->actions().at(5))
            removeTest(5);
        else if (selectedItem == delMenu->actions().at(6))
            removeTest(6);
    }
}

void TableDialog::setupDialog(void)
{
    QSqlQuery query;
    int i = 0;


    ui->tbFirst->setEnabled(false);
    ui->tbPrior->setEnabled(false);
    ui->tbNext->setEnabled(false);
    ui->tbLast->setEnabled(false);
    ui->tb1->setEnabled(false);
    ui->tb2->setEnabled(false);
    ui->tb3->setEnabled(false);
    ui->tb4->setEnabled(false);
    ui->tb5->setEnabled(false);
    ui->tb6->setEnabled(false);

    if (!query.exec(QString("SELECT f_name,f_group,f_class,f_dt,f_res1,f_legend1,f_res2,f_legend2,f_res3,f_legend3,f_res4,f_legend4,f_res5,f_legend5,f_res6,f_legend6,f_student,tbl_results.id \
                             FROM tbl_results,tbl_student \
                             WHERE  tbl_student.id = tbl_results.f_student ")))
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
        if (!query.value(5).isNull())
            ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(5).toString()).arg(query.value(4).toString()))); // Тест1
        if (!query.value(7).isNull())
            ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(7).toString()).arg(query.value(6).toString()))); // Тест2
        if (!query.value(9).isNull())
            ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(9).toString()).arg(query.value(8).toString()))); // Тест3
        if (!query.value(11).isNull())
            ui->tableWidget->setItem(i, 7, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(11).toString()).arg(query.value(10).toString()))); // Тест4
        if (!query.value(13).isNull())
            ui->tableWidget->setItem(i, 8, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(13).toString()).arg(query.value(12).toString()))); // Тест5
        if (!query.value(15).isNull())
            ui->tableWidget->setItem(i, 9, new QTableWidgetItem(QString("%1 (%2)").arg(query.value(15).toString()).arg(query.value(14).toString()))); // Тест6
        ui->tableWidget->setItem(i, 10, new QTableWidgetItem(query.value(16).toString()));  // Id студента
        ui->tableWidget->setItem(i, 11, new QTableWidgetItem(query.value(17).toString()));  // Id
        i++;
    }
    ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnHidden(10,true);
    ui->tableWidget->setColumnHidden(11,true);


    connect(ui->tableWidget->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(setEnabledBtn(QItemSelection, QItemSelection)));
    connect(ui->tbFirst, SIGNAL(clicked(bool)), this, SLOT(slotFirst()));
    connect(ui->tbPrior, SIGNAL(clicked(bool)), this, SLOT(slotPrior()));
    connect(ui->tbNext, SIGNAL(clicked(bool)), this, SLOT(slotNext()));
    connect(ui->tbLast, SIGNAL(clicked(bool)), this, SLOT(slotLast()));

    connect(ui->tb1, SIGNAL(clicked(bool)), this, SLOT(slotTest1()));
    connect(ui->tb2, SIGNAL(clicked(bool)), this, SLOT(slotTest2()));
    connect(ui->tb3, SIGNAL(clicked(bool)), this, SLOT(slotTest3()));
    connect(ui->tb4, SIGNAL(clicked(bool)), this, SLOT(slotTest4()));
    connect(ui->tb5, SIGNAL(clicked(bool)), this, SLOT(slotTest5()));
    connect(ui->tb6, SIGNAL(clicked(bool)), this, SLOT(slotTest6()));

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(showContextMenu(QPoint)));
}

void TableDialog::slotTest4(void)
{
    Test3Dialog *dlg;
    QString dt = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 3).data().toString(),
            name = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 0).data().toString(),
            group = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 1).data().toString();
    int course = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 2).data().toInt();

    dlg = new Test3Dialog(dt,name,group,course,this);
    if (dlg->exec() != QDialog::Rejected)
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 7, new QTableWidgetItem(QString("%1 (%2)").arg(dlg->getLegend()).arg(dlg->getResults())));

    delete dlg;
}
void TableDialog::slotTest5(void)
{
    Test5Dialog *dlg;
    QString dt = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 3).data().toString(),
            name = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 0).data().toString(),
            group = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 1).data().toString();
    int course = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 2).data().toInt();

    dlg = new Test5Dialog(dt,name,group,course,this);
    if (dlg->exec() != QDialog::Rejected)
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 8, new QTableWidgetItem(QString("%1 (%2)").arg(dlg->getLegend()).arg(dlg->getResults())));

    delete dlg;
}
void TableDialog::slotTest6(void)
{
    Test6Dialog *dlg;
    QString dt = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 3).data().toString(),
            name = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 0).data().toString(),
            group = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 1).data().toString();
    int course = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 2).data().toInt();

    dlg = new Test6Dialog(dt,name,group,course,this);
    if (dlg->exec() != QDialog::Rejected)
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 9, new QTableWidgetItem(QString("%1 (%2)").arg(dlg->getLegend()).arg(dlg->getResults())));

    delete dlg;
}

void TableDialog::setEnabledBtn(QItemSelection,QItemSelection)
{
    bool isSel = ui->tableWidget->selectionModel()->hasSelection();

    ui->tbFirst->setEnabled(isSel && ui->tableWidget->currentRow() != 0);
    ui->tbPrior->setEnabled(isSel && ui->tableWidget->currentRow() != 0);
    ui->tbNext->setEnabled(isSel && ui->tableWidget->currentRow() != ui->tableWidget->rowCount() - 1);
    ui->tbLast->setEnabled(isSel && ui->tableWidget->currentRow() != ui->tableWidget->rowCount() - 1);

    ui->tb1->setEnabled(isSel);
    ui->tb2->setEnabled(isSel);
    ui->tb3->setEnabled(isSel);
    ui->tb4->setEnabled(isSel);
    ui->tb5->setEnabled(isSel);
    ui->tb6->setEnabled(isSel);
}

void TableDialog::slotFirst(void)
{
    ui->tableWidget->setCurrentIndex(ui->tableWidget->model()->index(0, ui->tableWidget->currentColumn()));
}

void TableDialog::slotPrior(void)
{
    ui->tableWidget->setCurrentIndex(ui->tableWidget->model()->index(ui->tableWidget->currentRow() - 1, ui->tableWidget->currentColumn()));
}
void TableDialog::slotNext(void)
{
    ui->tableWidget->setCurrentIndex(ui->tableWidget->model()->index(ui->tableWidget->currentRow() + 1, ui->tableWidget->currentColumn()));
}
void TableDialog::slotLast(void)
{
    ui->tableWidget->setCurrentIndex(ui->tableWidget->model()->index(ui->tableWidget->rowCount() - 1, ui->tableWidget->currentColumn()));
}

void TableDialog::slotTest1(void)
{
    Test1Dialog *dlg;
    QString dt = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 3).data().toString(),
            name = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 0).data().toString(),
            group = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 1).data().toString();
    int course = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 2).data().toInt();

    dlg = new Test1Dialog(dt,name,group,course,this);
    if (dlg->exec() != QDialog::Rejected)
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 4, new QTableWidgetItem(QString("%1 (%2)").arg(dlg->getLegend()).arg(dlg->getResults())));

    delete dlg;
    return;
}
void TableDialog::slotTest2(void)
{
    Test2Dialog *dlg;
    QString dt = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 3).data().toString(),
            name = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 0).data().toString(),
            group = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 1).data().toString();
    int course = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 2).data().toInt();

    dlg = new Test2Dialog(dt,name,group,course,this);
    if (dlg->exec() != QDialog::Rejected)
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 5, new QTableWidgetItem(QString("%1 (%2)").arg(dlg->getLegend()).arg(dlg->getResults())));

    delete dlg;
}
void TableDialog::slotTest3(void)
{
    Test3Dialog *dlg;
    QString dt = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 3).data().toString(),
            name = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 0).data().toString(),
            group = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 1).data().toString();
    int course = ui->tableWidget->model()->index(ui->tableWidget->currentRow(), 2).data().toInt();

    dlg = new Test3Dialog(dt,name,group,course,this);
    if (dlg->exec() != QDialog::Rejected)
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 6, new QTableWidgetItem(QString("%1 (%2)").arg(dlg->getLegend()).arg(dlg->getResults())));

    delete dlg;
}

