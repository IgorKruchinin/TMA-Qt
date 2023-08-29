#include "transactionstable.h"
#include "ui_transactionstable.h"
#include <vector>
#include <QDateTime>

TransactionsTable::TransactionsTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionsTable)
{
    ui->setupUi(this);
    this->defaultTotalSumString = ui->totalSum->text();
    this->table = ui->tableView;
    this->tableModel = new QStandardItemModel(this->table);

}
void TransactionsTable::getData(std::vector<MoneyData*> *data) {
    this->data = data;
    this->resetTable();
}

void TransactionsTable::resetTable() {
    this->tableModel->clear();
    this->tableModel->setColumnCount(7);
    this->table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->tableModel->setHorizontalHeaderLabels(QStringList() << "ID" << "Date" << "Name" << "Get/Waste" << "Category" << "Money before" << "Sum");
    uint32_t totalSum = 0;
    for (MoneyData *item: *data) {
        printf("%u", item->ID);
        QList<QStandardItem *> rowItems;
        rowItems.append(new QStandardItem(QString::fromStdString(std::to_string(item->ID))));
        QDateTime *date = new QDateTime();
        date->setSecsSinceEpoch(item->date);
        rowItems.append(new QStandardItem(date->date().toString(Qt::DateFormat::ISODate)));
        rowItems.append(new QStandardItem(QString::fromStdString(item->name)));
        rowItems.append(new QStandardItem(QString(item->code ? "G" : "W")));
        rowItems.append(new QStandardItem(QString::fromStdString(item->category)));
        rowItems.append(new QStandardItem(QString::fromStdString(std::to_string(item->money_before))));
        rowItems.append(new QStandardItem(QString::fromStdString(std::to_string(item->sum))));
        this->tableModel->appendRow(rowItems);
        totalSum += item->sum;
        delete date;
    }
    this->table->setModel(this->tableModel);
    ui->totalSum->setText(defaultTotalSumString + QString::fromStdString(std::to_string(totalSum)));
}

TransactionsTable::~TransactionsTable()
{
    delete data;
    delete ui;
    delete table;
    delete tableModel;
}
