#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>
#include <vector>
#include <QPushButton>
#include "logic.h"
#include "basedatastruct.h"

extern int board[15][15];
extern int chessNum;


namespace Ui {
class game;
}
class game : public QMainWindow
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = nullptr);
    ~game();
    //放置棋子，1是白色，2是黑色
    bool SetChessPhoto(int x,int y,int color);


    void Win();
    void Fail();
    void NobodyWin();
    void SetChooseButton();
private:
    Ui::game *ui;
    std::vector<QLabel*> chessFig;
    QLabel *chessBoard;
    QLabel *gameResult;
    QPushButton *quit;
    QPushButton *tryAgain;
    bool isGameContinue;
    bool mouseStatus=true;
protected:
    //实现虚函数
    void mousePressEvent(QMouseEvent *event);
private slots:
    void Quit();
    void TryAgain();
    //当自己点击鼠标的时候
    //void ClickChessBoard();
};

#endif // GAME_H




