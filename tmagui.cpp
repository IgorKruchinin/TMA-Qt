#include "tmagui.h"
#include "ui_tmagui.h"
#include "tmaAPI/tma.cpp"
#include "transactionstable.h"


TMAGUI::TMAGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TMAGUI)
{
    ui->setupUi(this);
    defaultBalanceString = ui->currBalanceLbl->text();
    noEnoughMoneyString = ui->errFld->text();
    ui->setDate->setDate(QDate::currentDate());
    ui->selectBegDate->setDate(QDate::currentDate());
    ui->selectEndDate->setDate(QDate::currentDate());
    this->tma = new TMA();
    resetBalance();
    resetHints();
}
void TMAGUI::registerBtnFunc() {
    bool code = 0;
    if (ui->regGet->isChecked()) {
        code = 1;
    } else if (ui->regWaste->isChecked()) {
        code = 0;
    }
    long long unsigned date = ui->setDate->dateTime().toSecsSinceEpoch();
    std::string name = ui->setName->currentText().toStdString();
    std::string category = ui->setCat->currentText().toStdString();
    uint32_t sum = ui->setSum->value();
    try {
        this->tma->append(date, code, name, sum, category);
        resetBalance();
        resetHints();
    } catch (NoEnoughMoneyException) {
        ui->errFld->setText(noEnoughMoneyString);
    }

}

void TMAGUI::resetHints() {
    QComboBox *names = ui->setName;
    QComboBox *selectNames = ui->selectName;

    names->clear();
    selectNames->clear();

    names->addItem(QString::fromStdString(""));
    selectNames->addItem(QString::fromStdString(""));

    for (std::string name: tma->getNamesCached()) {
        names->addItem(QString::fromStdString(name));
        selectNames->addItem(QString::fromStdString(name));
    }

    QComboBox *cats = ui->setCat;
    QComboBox *selectCats = ui->selectCat;

    cats->clear();
    selectCats->clear();

    cats->addItem(QString::fromStdString(""));
    selectCats->addItem(QString::fromStdString(""));

    for (std::string cat: tma->getCategoriesCached()) {
        cats->addItem(QString::fromStdString(cat));
        selectCats->addItem(QString::fromStdString(cat));
    }
}

void TMAGUI::resetBalance() {
    ui->currBalanceLbl->setText(defaultBalanceString + QString::fromStdString(std::to_string(tma->getBalance())));
    ui->errFld->setText("");
}

void TMAGUI::showTableBtnFunc() {
    std::vector<MoneyData*> *selectedData = new std::vector<MoneyData*>();
    std::string name = ui->selectName->currentText().toStdString();
    std::string category = ui->selectCat->currentText().toStdString();
    if (!(ui->showGettingsBx->isChecked() && ui->showWastesBx->isChecked())) {
        bool code = 0;
        if (ui->showGettingsBx->isChecked()) {
            code = 1;
        } else if (ui->showWastesBx->isChecked()) {
            code = 0;
        }
        if (ui->dateSelectBx->isChecked()) {
            unsigned long long dateBeg = ui->selectBegDate->dateTime().toSecsSinceEpoch();
            unsigned long long dateEnd = ui->selectEndDate->dateTime().toSecsSinceEpoch();
            tma->get(selectedData, code, category, name, dateBeg, dateEnd);
        } else {
            tma->get(selectedData, code, category, name);
        }
    } else {
        if (ui->dateSelectBx->isChecked()) {
            unsigned long long dateBeg = ui->selectBegDate->dateTime().toSecsSinceEpoch();
            unsigned long long dateEnd = ui->selectEndDate->dateTime().toSecsSinceEpoch();
            tma->get(selectedData, category, name, dateBeg, dateEnd);
        } else {
            tma->get(selectedData, category, name);
        }
    }
    TransactionsTable *tt = new TransactionsTable();
    tt->getData(selectedData);
    tt->show();
}

TMAGUI::~TMAGUI()
{
    delete tma;
    delete ui;
}


void TMAGUI::on_registerBtn_clicked()
{
    registerBtnFunc();
}


void TMAGUI::on_viewTransactBtn_clicked()
{
    showTableBtnFunc();
}


void TMAGUI::on_catSelectBx_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->selectCat->clear();
        ui->selectCat->setEnabled(0);
        ui->selectCatLbl->setEnabled(0);
    } else {
        resetHints();
        ui->selectCat->setEnabled(1);
        ui->selectCatLbl->setEnabled(1);
    }
}


void TMAGUI::on_nameSelectBx_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->selectName->clear();
        ui->selectName->setEnabled(0);
        ui->selectNameLbl->setEnabled(0);
    } else {
        resetHints();
        ui->selectName->setEnabled(1);
        ui->selectNameLbl->setEnabled(1);
    }
}


void TMAGUI::on_dateSelectBx_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->selectBegDate->setEnabled(0);
        ui->selectBegDateLbl->setEnabled(0);
        ui->selectEndDate->setEnabled(0);
        ui->selectEndDateLbl->setEnabled(0);
    } else {
        ui->selectBegDate->setEnabled(1);
        ui->selectBegDateLbl->setEnabled(1);
        ui->selectEndDate->setEnabled(1);
        ui->selectEndDateLbl->setEnabled(1);
    }
}

