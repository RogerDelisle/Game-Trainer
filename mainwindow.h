#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QPushButton>

#include <QMainWindow>

#include <utility>
#include <tuple>

#include "gamelogic.h"


using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void    SetUIState(GT_SQ_STATE_T, QPushButton*);

private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();

    void on_checkPlayerFirst_stateChanged(int arg1);
    void on_pushNewGame_clicked();

    void on_radio_X_clicked();

    void on_radio_O_clicked();

private:
    Ui::MainWindow*                     ui;

    tuple<const char*, const char*, const char*>     mSymbol;
    GameLogic<QPushButton, MainWindow>  game;
    void                                ProcessButtonClick(GT_SQUARE_NO sqno);
};

#endif // MAINWINDOW_H
