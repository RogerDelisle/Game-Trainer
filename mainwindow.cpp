#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mSymbol = ui->radio_X->isChecked() ? make_tuple("", "X", "O") : make_tuple("", "O", "X");
    game.SetSquare(new Square<QPushButton, MainWindow>(0, this, ui->pushButton_1));
    game.SetSquare(new Square<QPushButton, MainWindow>(1, this, ui->pushButton_2));
    game.SetSquare(new Square<QPushButton, MainWindow>(2, this, ui->pushButton_3));
    game.SetSquare(new Square<QPushButton, MainWindow>(3, this, ui->pushButton_4));
    game.SetSquare(new Square<QPushButton, MainWindow>(4, this, ui->pushButton_5));
    game.SetSquare(new Square<QPushButton, MainWindow>(5, this, ui->pushButton_6));
    game.SetSquare(new Square<QPushButton, MainWindow>(6, this, ui->pushButton_7));
    game.SetSquare(new Square<QPushButton, MainWindow>(7, this, ui->pushButton_8));
    game.SetSquare(new Square<QPushButton, MainWindow>(8, this, ui->pushButton_9));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_1_clicked() { ProcessButtonClick(0); }
void MainWindow::on_pushButton_2_clicked() { ProcessButtonClick(1); }
void MainWindow::on_pushButton_3_clicked() { ProcessButtonClick(2); }
void MainWindow::on_pushButton_4_clicked() { ProcessButtonClick(3); }
void MainWindow::on_pushButton_5_clicked() { ProcessButtonClick(4); }
void MainWindow::on_pushButton_6_clicked() { ProcessButtonClick(5); }
void MainWindow::on_pushButton_7_clicked() { ProcessButtonClick(6); }
void MainWindow::on_pushButton_8_clicked() { ProcessButtonClick(7); }
void MainWindow::on_pushButton_9_clicked() { ProcessButtonClick(8); }

void MainWindow::ProcessButtonClick(GT_SQUARE_NO sqno)
{
    char s[1024];

    GT_GAME_STATE_T gs = game.sigSquareClicked(sqno); // GameLogic object gets triggered to play from this call.
    switch (gs) {
        case GT_GAME_STATE_PLAY:
            sprintf(s, "Your Move");
            break;
        case GT_GAME_STATE_PLAYER_WINS:
            sprintf(s, "You WIN!! :-O");
            break;
        case GT_GAME_STATE_LOGIC_WINS :
            sprintf(s, "Computer WINS!! >:-)");
            break;
        case GT_GAME_STATE_TIE:
            sprintf(s, "Tie Game... :-/");
            break;
    }

    ui->lineGameStatus->setText(s);
}

void MainWindow::on_checkPlayerFirst_stateChanged(int arg1)
{
    game.SetPlayerFirstFlag(arg1 == 2);
}

void MainWindow::on_pushNewGame_clicked()
{
    ui->lineGameStatus->setText("New Game.");
    game.NewGame();
}

void MainWindow::on_radio_X_clicked()
{
    std::get<1>(mSymbol) = "X";
    std::get<2>(mSymbol) = "O";
    game.redrawRequest();
}

void MainWindow::on_radio_O_clicked()
{
    std::get<1>(mSymbol) = "O";
    std::get<2>(mSymbol) = "X";
    game.redrawRequest();
}

void MainWindow::SetUIState(GT_SQ_STATE_T st, QPushButton * pb)
{
    const char* c = (st == GT_SQ_STATE_EMPTY) ? std::get<0>(mSymbol) :
                                             (st == GT_SQ_STATE_PLAYER) ? std::get<1>(mSymbol) : std::get<2>(mSymbol);
    pb->setText(c);
}
