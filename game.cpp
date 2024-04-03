#include "game.h"
#include "ui_game.h"
#include <QDebug>
game::game(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::game)
{
    ui->setupUi(this);
    isGameContinue=true;
    chessBoard=new QLabel(this);
    chessBoard->setFixedSize(720,720);
    QPixmap pixmap(":/images/resource/chessboard.png");
    chessBoard->setPixmap(pixmap);
    chessBoard->setScaledContents(true);
    chessBoard->show();

    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
            board[i][j]=0;
    chessNum=0;
}

game::~game()
{
    delete ui;
}

void game::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton&&isGameContinue&&mouseStatus)
    {
        //this->NobodyWin();
        mouseStatus=false;

        //下棋后有延迟显示，因为调用show只是发送一个事件，在消息队列中排序。可以发送一个异步事件
        //https://bbs.csdn.net/topics/390936663
        QPoint position=event->pos();
        //调整坐标系，x是行y是列
        NewPoint humanPos={(position.y()-6)/48,(position.x()-6)/48};

        qDebug()<<"black pos x:"<<(position.y()-6)/48<<" y:"<<(position.x()-6)/48<<endl;

        if(SetChessPhoto(humanPos.x,humanPos.y,BLACK)){
            //调用电脑进行下棋
            NewPoint computerPos={-1,-1};
            WhoWin flag;
            GameTree *root=new GameTree;
            flag=root->Play(humanPos,computerPos);
            /***************
             * 导出棋局状态
             **************/
            if(flag==StillPlay)
                SetChessPhoto(computerPos.x,computerPos.y,WHITE);
            if(flag==BlackWin)
                this->Win();//黑方胜利执行
            if(flag==WhiteWin){
                SetChessPhoto(computerPos.x,computerPos.y,WHITE);
                this->Fail();//白方胜利执行
            }
            if(flag==Stalemate){
                if(computerPos.x!=-1)
                    SetChessPhoto(computerPos.x,computerPos.y,WHITE);
                this->NobodyWin();//和局动作执行
            }

            delete root;
        }
    }
    //else
        //QMainWindow::mousePressEvent(event);
}

bool game::SetChessPhoto(int x,int y,int color)
{
    //判断(x,y)是否已有棋子和超出棋盘
    //qDebug()<<x<<" "<<y;
    mouseStatus=true;

    if(x>=0&&x<15&&y>=0&&y<15&&isGameContinue){
        if(color==WHITE&&(board[x][y]==0))
        {
            QLabel *plabel=new QLabel(this);
            //之前坐标系使用错误，这里将x和y调换改正
            plabel->move(int(y*47.5+5),int(x*47.5)+3);
            plabel->setFixedSize(44,44);
            plabel->setStyleSheet("background-image: url(:/images/resource/whitechess.png)");
            plabel->show();
            chessFig.push_back(plabel);
            board[x][y]=WHITE;
            return true;
        }
        else if(color==BLACK&&(board[x][y]==0))
        {
            QLabel *plabel=new QLabel(this);
            plabel->move(int(y*47.5+5),int(x*47.5)+3);
            plabel->setFixedSize(46,47);
            plabel->setStyleSheet("background-image: url(:/images/resource/blackchess.png)");
            plabel->show();
            chessFig.push_back(plabel);
            board[x][y]=BLACK;
            return true;
        }
    }
    return false;
}

void game::SetChooseButton(){
    quit=new QPushButton(this);
    QIcon icon1(":/images/resource/quit.png");
    quit->setIcon(icon1);
    quit->setAutoDefault(true);
    quit->setFlat(true);
    quit->setIconSize(QSize(150,150));
    quit->setGeometry(380,320,150,150);
    quit->setStyleSheet("background-color: rgb(0,0,0,0);");
    quit->show();
    connect(quit,&QPushButton::clicked,this,&game::Quit);

    tryAgain=new QPushButton(this);
    QIcon icon2(":/images/resource/tryagain.png");
    tryAgain->setIcon(icon2);
    tryAgain->setAutoDefault(true);
    tryAgain->setFlat(true);
    tryAgain->setIconSize(QSize(200,200));
    tryAgain->setGeometry(190,294,200,200);
    tryAgain->setStyleSheet("background-color: rgb(0,0,0,0);");
    tryAgain->show();
    connect(tryAgain,&QPushButton::clicked,this,&game::TryAgain);
}

void game::Win(){ 
    isGameContinue=false;

    gameResult=new QLabel(this);
    gameResult->move(185,40);
    gameResult->setFixedSize(350,350);
    QPixmap pixmap(":/images/resource/win.png");
    gameResult->setScaledContents(true);
    gameResult->setPixmap(pixmap);
    gameResult->show();

    SetChooseButton();
}

void game::Fail(){


    isGameContinue=false;

    gameResult=new QLabel(this);
    gameResult->move(180,140);
    gameResult->setFixedSize(350,230);
    QPixmap pixmap(":/images/resource/fail.png");
    gameResult->setScaledContents(true);
    gameResult->setPixmap(pixmap);
    gameResult->show();

    SetChooseButton();
}

void game::NobodyWin(){
    isGameContinue=false;
    gameResult=new QLabel(this);
    gameResult->move(190,150);
    gameResult->setFixedSize(320,150);
    QPixmap pixmap(":/images/resource/stalemate.png");
    gameResult->setScaledContents(true);
    gameResult->setPixmap(pixmap);
    gameResult->show();

    SetChooseButton();
}

void game::Quit()
{
    delete quit;
    delete tryAgain;
    delete gameResult;
}

void game::TryAgain()
{
    delete quit;
    delete tryAgain;
    delete gameResult;

    game *a=new game;
    a->show();
    this->close();
}




