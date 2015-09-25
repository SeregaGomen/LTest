#ifndef GETSTUDENTDIALOG_H
#define GETSTUDENTDIALOG_H

#include <QDialog>

namespace Ui {
class GetStudentDialog;
}

class GetStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetStudentDialog(QWidget *parent = 0);
    ~GetStudentDialog();
    QString getStudentName(void);
    QString getStudentGroup(void);
    QString getDate(void);
    int getStudentClass(void);

private slots:
    void slotChangeText(const QString&);

private:
    Ui::GetStudentDialog *ui;
    void initDialog(void);
};

#endif // GETSTUDENTDIALOG_H
