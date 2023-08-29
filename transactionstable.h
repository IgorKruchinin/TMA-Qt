#ifndef TRANSACTIONSTABLE_H
#define TRANSACTIONSTABLE_H

#include <QDialog>
#include <vector>
#include <QStandardItem>
#include <QStandardItemModel>
#include "tmaAPI/money_data.h"
#include <QTableView>

namespace Ui {
class TransactionsTable;
}

class TransactionsTable : public QDialog
{
    Q_OBJECT


public:
    explicit TransactionsTable(QWidget *parent = nullptr);
    ~TransactionsTable();
    void getData(std::vector<MoneyData*> *data);
    void resetTable();

private slots:
    void on_exportToCSVBtn_clicked();

private:
    Ui::TransactionsTable *ui;
    std::vector<MoneyData*> *data;
    QTableView *table;
    QStandardItemModel *tableModel;
    QString defaultTotalSumString;
};

#endif // TRANSACTIONSTABLE_H
