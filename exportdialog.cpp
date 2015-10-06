#include <QPushButton>
#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    connect(ui->cbTest1, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbTest2, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbTest3, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbTest4, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbTest5, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbTest6, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbName, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbGroup, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbClass, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbDate, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbLegend, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
    connect(ui->cbResult, SIGNAL(clicked(bool)), this, SLOT(slotCBClicked(bool)));
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::slotCBClicked(bool)
{
    bool isTest = ui->cbTest1->isChecked() || ui->cbTest2->isChecked() || ui->cbTest3->isChecked() || ui->cbTest4->isChecked() || ui->cbTest5->isChecked() || ui->cbTest6->isChecked(),
         isFields = ui->cbName->isChecked() || ui->cbGroup->isChecked() || ui->cbDate->isChecked() || ui->cbClass->isChecked() || ui->cbResult->isChecked() || ui->cbLegend->isChecked();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isTest && isFields);
}
