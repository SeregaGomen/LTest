#include "resultdialog.h"
#include "ui_resultdialog.h"

ResultDialog::ResultDialog(int pi, QString pdt1, QString pdt2, int n1,int n2,int index,float m1, float m2, float s1, float s2, float sx1, float sx2, float t, float v,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    QString title[] = {
                        "Діяльнісно-операційний тест",
                        "Інформаційно-технологічний тест",
                        "Когнітивний тест з маркетингу туризму",
                        "Когнітивний тест з менеджменту",
                        "Особистісно-творчий тест",
                        "Мотиваційний тест"
                      };
    float ts[][4] =  {
                         { 1, 12.71, 63.66, 0 },
                         { 2, 4.30, 9.92, 31.60 },
                         { 3, 3.18, 5.84, 12.92 },
                         { 4, 2.78, 4.60, 8.61 },
                         { 5, 2.57, 4.03, 6.87 },
                         { 6, 2.45, 3.71, 5.96 },
                         { 7, 2.37, 3.50, 5.41 },
                         { 8, 2.31, 3.36, 5.04 },
                         { 9, 2.26, 3.25, 4.78 },
                         {10, 2.23, 3.17, 4.59 },
                         {20, 2.09, 2.85, 3.85 },
                         {30, 2.04, 2.75, 3.65 },
                         {60, 2.00, 2.66, 3.46 }
                     };


    ui->setupUi(this);

    setWindowTitle(title[pi]);
    ui->lbBeginDate->setText(pdt1);
    ui->lbCurrentDate->setText(pdt2);
    ui->lbBeginN->setText(QString("%1").arg(n1));
    ui->lbCurrentN->setText(QString("%1").arg(n2));
    ui->lbBeginExpect->setText(QString("%1").arg(m1,6,'f',2));
    ui->lbCurrentExpect->setText(QString("%1").arg(m2,6,'f',2));
    ui->lbBeginDeviation->setText(QString("%1").arg(s1,6,'f',2));
    ui->lbCurrentDeviation->setText(QString("%1").arg(s2,6,'f',2));
    ui->lbBeginError->setText(QString("%1").arg(sx1,6,'f',2));
    ui->lbCurrentError->setText(QString("%1").arg(sx2,6,'f',2));
    ui->lbT->setText(QString("%1").arg(t,6,'f',2));
    ui->lbFreedom->setText(QString("%1").arg(v,6,'f',2));


    if (t < ts[index][1])
        ui->lbSummary->setText(tr("Групи статистично не відрізняються! (%1 < %2)").arg(t,6,'f',2).arg(ts[index][1],6,'f',2));
    else
        ui->lbSummary->setText(tr("Групи статистично відрізняються! (%1 > %2)").arg(t,6,'f',2).arg(ts[index][1],6,'f',2));

    for (int i = 0; i < 13; i++)
    {
        ui->twTable->insertRow(i);
        ui->twTable->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(ts[i][0])));
        ui->twTable->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(ts[i][1],6,'f',2)));
    }
    ui->twTable->setCurrentCell(index,0);
}

ResultDialog::~ResultDialog()
{
    delete ui;
}
