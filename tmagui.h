#ifndef TMAGUI_H
#define TMAGUI_H
#include "tmaAPI/tma.h"


#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TMAGUI; }
QT_END_NAMESPACE

class TMAGUI : public QMainWindow
{
    Q_OBJECT
    TMA *tma;
    QString defaultBalanceString;
    QString noEnoughMoneyString;

public:
    TMAGUI(QWidget *parent = nullptr);
    void resetBalance();
    void resetHints();
    void showTableBtnFunc();
    ~TMAGUI();

public slots:
    void registerBtnFunc();

private slots:
    void on_registerBtn_clicked();

    void on_viewTransactBtn_clicked();

    void on_catSelectBx_stateChanged(int arg1);

    void on_nameSelectBx_stateChanged(int arg1);

    void on_dateSelectBx_stateChanged(int arg1);

private:
    Ui::TMAGUI *ui;
};
#endif // TMAGUI_H
