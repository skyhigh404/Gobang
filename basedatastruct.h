#ifndef BASE_H
#define BASE_H

extern int board[15][15];
extern int chessNum;//落子数目

enum WhoWin{BlackWin,WhiteWin,Stalemate,StillPlay};
//数据域
typedef struct NewPoint{
    int x,y;
}NewPoint;

typedef struct CSNode
{
    //int evalue;         //新下一步棋后当前情况的估值
    int nBoard[15][15]; //棋盘
    NewPoint point;     //新下的一步棋的位置
    struct CSNode* firstChild;
    struct CSNode* nextSibling;
    //struct CSNode* parent;
}CSNode,*CSTree;


//存储最优点
typedef struct BestPoint
{
    NewPoint pos[255];
    int score;
    int length;
}BestPoint;



#endif // BASE_H
