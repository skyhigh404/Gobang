#ifndef LOGIC_H
#define LOGIC_H
#include "basedatastruct.h"
#include <algorithm>
#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdlib>

#include <QDebug>
#include <windows.h>
using std::max;
using std::min;
using std::string;
using std::vector;
using std::regex;
using std::regex_search;



/*** 电脑执白棋***/

#define SEARCHDEPTH 3
#define WHITE 1
#define BLACK 2
#define SELECTNODENUM 5 //SelectNodele类留下的结点数目

extern int board[15][15];
extern int chessNum;//落子数目

//博弈树
class GameTree
{
public:
    GameTree();
    ~GameTree();
    void InitCSNode(GameTree *root);
    //寻找可供下棋的位置，返回的bool值表示有无必下落点
    bool FindPossiblePos(NewPoint pos[],int &length);
    //对下棋的棋子进行排序优化
    void OptimezePos(NewPoint pos[],int length,bool isMaxNode);
    //剪枝
    //极大结点下界为alpha，极小结点上界为beta,初始时alpha为-80000000，beta为80000000，第1层为max结点
    int Alpha_BetaCut(int depth,int alpha,int beta,int searchDepth);
    //

    //棋局估值函数，对当前棋盘形势进行判断，数字越大表示己方获胜几率越大
    int Evaluation();
    //新估值函数
    int AnotherEvaluation(bool isBlackChess);
    int EvaluateByArray(vector<string> n,bool &rush4white,bool &live3white,bool &rush4black,bool &live3black,bool isBlackChess);
    void IntArraytoString(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal);
    //将落子点行、列、主对角线方向、副对角线方向的落子情况写入n中
    void IntArraytoString(vector<string> &n,NewPoint pos);
    //判断是否胜利
    bool IsHumanWin(NewPoint pos);
    bool IsComputerWin(NewPoint pos);
    //整合逻辑，以便在game.cpp中调用
    WhoWin Play(NewPoint humanPos,NewPoint &computerPos);
private:
    int nBoard[15][15]; //棋盘
    NewPoint point;     //当前结点下棋的位置
    GameTree* firstChild;
    GameTree* nextSibling;
    //CSNode *data;
};

//选择较好的结点来进一步搜索，减少计算量
/*class SelectNode
{
public:
    friend class GameTree;
    SelectNode();
    SelectNode(GameTree *p,int s);
    ~SelectNode();
    void Add(SelectNode *p);
    static int length;
private:
    GameTree *pnode;
    int score;
    SelectNode *next;

};
*/








//棋型判断
bool FiveKill_White(vector<string> &n);//长连
bool FiveKill_Black(vector<string> &n);
bool LiveFour_White(vector<string> &n);//活四
bool LiveFour_Black(vector<string> &n);
bool DoubleRushFour_White(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal);//双冲四
bool DoubleRushFour_Black(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal);
bool RushFourAndLiveThree_White(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal);//冲四活三
bool RushFourAndLiveThree_Black(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal);
bool DoubleLiveThree_White(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal);//双活三
bool DoubleLiveThree_Black(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal);
bool RushFour_White(vector<string> &n);//冲四
bool RushFour_Black(vector<string> &n);
bool LiveThree_White(vector<string> &n);//活三
bool LiveThree_Black(vector<string> &n);
int LiveTwo_White(vector<string> &n);//活二
int LiveTwo_Black(vector<string> &n);
int SleepThree_White(vector<string> &n);//眠三
int SleepThree_Black(vector<string> &n);
int SleepTwo_White(vector<string> &n);//眠二
int SleepTwo_Black(vector<string> &n);

bool NewDoubleRushFour_White(vector<string> &n);
bool NewRushFourAndLiveThree_White(vector<string> &n);
bool NewDoubleLiveThree_White(vector<string> &n);
bool NewDoubleRushFour_Black(vector<string> &n);
bool NewRushFourAndLiveThree_Black(vector<string> &n);
bool NewDoubleLiveThree_Black(vector<string> &n);


bool DeadFour_White(vector<string> &n);//死四
bool DeadFour_Black(vector<string> &n);
bool DeadThree_White(vector<string> &n);//死三
bool DeadThree_Black(vector<string> &n);
bool DeadTwo_White(vector<string> &n);//死二
bool DeadTwo_Black(vector<string> &n);

#endif // LOGIC_H
