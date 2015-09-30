#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>
#include <QItemSelection>

namespace Ui {
class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableDialog(QWidget *parent = 0);
    ~TableDialog();

private slots:
    void setEnabledBtn(QItemSelection,QItemSelection);

private:
    Ui::TableDialog *ui;
    void setupDialog(void);
    void checkButtons(void);
};

#endif // TABLEDIALOG_H
