#include "logic.h"

int board[15][15]={{0}};
int chessNum=0;//落子数目

static BestPoint bestPoint;
//static SelectNode* head=nullptr;

//根据棋盘分布问题，越中心点的分值应越高
static const int positon[15][15]={
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
    {0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
    {0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
    {0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
    {0,1,2,3,4,5,6,7,6,5,4,3,2,1,0},
    {0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
    {0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
    {0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
    {0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
    {0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};




static int searchNodeNum=0;//搜索结点数
GameTree::GameTree(){

}
GameTree::~GameTree(){
    if(this->firstChild)
        delete this->firstChild;
    if(this->nextSibling)
        delete this->nextSibling;
}
void GameTree::InitCSNode(GameTree *root)
{
    //拷贝棋盘
    if(root==nullptr){
        for(int i=0;i<15;i++)
            for(int j=0;j<15;j++)
                this->nBoard[i][j]=board[i][j];
        this->nextSibling=nullptr;
        this->firstChild=nullptr;
    }
    else{
        for(int i=0;i<15;i++)
            for(int j=0;j<15;j++)
                this->nBoard[i][j]=root->nBoard[i][j];
        this->firstChild=nullptr;
        this->nextSibling=root->firstChild;
        root->firstChild=this;
    }
}
//寻找可落子点
bool GameTree::FindPossiblePos(NewPoint pos[],int &length)
{
    length=0;
    int regionOfPossible[15][15]={{0}};

    bool flag=false;//标志有无活四、长连、活三、冲四出现
    /*
    //如果当前下棋点this->point造成了活三、活四、冲四、长连，必堵
    vector<string> n;
    this->IntArraytoString(n,this->point);
    int i;//i指向当前位置
    int blackNum;//连续的黑子、白子数
    int whiteNum;


    blackNum=0;
    whiteNum=0;
    for(i=0;i<static_cast<int>(n[0].length());i++){
        if(n[0][i]==WHITE+48){
            ++whiteNum;
            blackNum=0;

            //长连（11111型）
            //如果这个11111型的两端都有棋子该怎么办
            if(whiteNum>=5){
                if(i+1<15)
                    regionOfPossible[this->point.x][i+1]=1;
                if(i-5>=0)
                    regionOfPossible[this->point.x][i-5]=1;
            }

            //冲四（011112型或211110型）或活四（011110型）时
            else if(whiteNum==4){
                if(i-4>=0)
                    regionOfPossible[this->point.x][i-4]=1;
                if(i+1<15)
                    regionOfPossible[this->point.x][i+1]=1;
            }

            //冲四（10111型或11101型）或活三（01110型）
            else if(whiteNum==3){
                if(i-4>=0&&n[0][i-4]==WHITE+48&&n[0][i-3]=='0')
                    regionOfPossible[this->point.x][i-3]=1;
                else if(i+2<15&&n[0][i+2]==WHITE+48&&n[0][i+1]=='0')
                    regionOfPossible[this->point.x][i+1]=0;
                else if(i-3>=0&&i+1<15&&n[0][i-3]=='0'&&n[0][i+1]=='0'){
                    regionOfPossible[this->point.x][i-3]=1;
                    regionOfPossible[this->point.x][i+1]=1;
                }
            }

            //冲四（11011型）或活三（010110型或011010型）
            else if(whiteNum==2){
                if(i+3<15&&n[0][i+1]=='0'&&n[0][i+2]==WHITE+48&&n[0][i+3]==WHITE+48)
                    regionOfPossible[this->point.x][i+1]=1;
                else if(i-4>=0&&i+1<15&&n[0][i-4]=='0'&&n[0][i-3]==WHITE+48&&n[0][i-2]=='0'&&n[0][i+1]=='0')
                    regionOfPossible[this->point.x][i-2]=1;
                else if(i-2>=0&&i+3<15&&n[0][i-2]=='0'&&n[0][i+1]=='0'&&n[0][i+2]==WHITE+48&&n[0][i+3]=='0')
                    regionOfPossible[this->point.x][i+1]=1;
            }
        }

        else if(n[0][i]==BLACK+48){
            whiteNum=0;
            ++blackNum;

            //长连（11111型）
            //如果这个11111型的两端都有棋子该怎么办
            if(blackNum>=5){
                if(i+1<15)
                    regionOfPossible[this->point.x][i+1]=1;
                if(i-5>=0)
                    regionOfPossible[this->point.x][i-5]=1;
            }

            //冲四（011112型或211110型）或活四（011110型）时
            else if(blackNum==4){
                if(i-4>=0)
                    regionOfPossible[this->point.x][i-4]=1;
                if(i+1<15)
                    regionOfPossible[this->point.x][i+1]=1;
            }

            //冲四（10111型或11101型）或活三（01110型）
            else if(blackNum==3){
                if(i-4>=0&&n[0][i-4]==BLACK+48&&n[0][i-3]=='0')
                    regionOfPossible[this->point.x][i-3]=1;
                else if(i+2<15&&n[0][i+2]==BLACK+48&&n[0][i+1]=='0')
                    regionOfPossible[this->point.x][i+1]=0;
                else if(i-3>=0&&i+1<15&&n[0][i-3]=='0'&&n[0][i+1]=='0'){
                    regionOfPossible[this->point.x][i-3]=1;
                    regionOfPossible[this->point.x][i+1]=1;
                }
            }

            //冲四（11011型）或活三（010110型或011010型）
            else if(blackNum==2){
                if(i+3<15&&n[0][i+1]=='0'&&n[0][i+2]==BLACK+48&&n[0][i+3]==BLACK+48)
                    regionOfPossible[this->point.x][i+1]=1;
                else if(i-4>=0&&i+1<15&&n[0][i-4]=='0'&&n[0][i-3]==BLACK+48&&n[0][i-2]=='0'&&n[0][i+1]=='0')
                    regionOfPossible[this->point.x][i-2]=1;
                else if(i-2>=0&&i+3<15&&n[0][i-2]=='0'&&n[0][i+1]=='0'&&n[0][i+2]==BLACK+48&&n[0][i+3]=='0')
                    regionOfPossible[this->point.x][i+1]=1;
            }

        }
        else//n[0][i]=='0'
        {
            whiteNum=0;
            blackNum=0;
        }

    }


    blackNum=0;
    whiteNum=0;
    for(i=0;i<static_cast<int>(n[1].length());i++){
        if(n[1][i]==WHITE+48){
            ++whiteNum;
            blackNum=0;

            //长连（11111型）
            //如果这个11111型的两端都有棋子该怎么办
            if(whiteNum>=5){
                if(i+1<15)
                    regionOfPossible[i+1][this->point.y]=1;
                if(i-5>=0)
                    regionOfPossible[i-5][this->point.y]=1;
            }

            //冲四（011112型或211110型）或活四（011110型）时
            else if(whiteNum==4){
                if(i-4>=0)
                    regionOfPossible[i-4][this->point.y]=1;
                if(i+1<15)
                    regionOfPossible[i+1][this->point.y]=1;
            }

            //冲四（10111型或11101型）或活三（01110型）
            else if(whiteNum==3){
                if(i-4>=0&&n[1][i-4]==WHITE+48&&n[1][i-3]=='0')
                    regionOfPossible[i-3][this->point.y]=1;
                else if(i+2<15&&n[1][i+2]==WHITE+48&&n[1][i+1]=='0')
                    regionOfPossible[i+1][this->point.y]=0;
                else if(i-3>=0&&i+1<15&&n[1][i-3]=='0'&&n[1][i+1]=='0'){
                    regionOfPossible[i-3][this->point.y]=1;
                    regionOfPossible[i+1][this->point.y]=1;
                }
            }

            //冲四（11011型）或活三（010110型或011010型）
            else if(whiteNum==2){
                if(i+3<15&&n[1][i+1]=='0'&&n[1][i+2]==WHITE+48&&n[1][i+3]==WHITE+48)
                    regionOfPossible[i+1][this->point.y]=1;
                else if(i-4>=0&&i+1<15&&n[1][i-4]=='0'&&n[1][i-3]==WHITE+48&&n[1][i-2]=='0'&&n[1][i+1]=='0')
                    regionOfPossible[i-2][this->point.y]=1;
                else if(i-2>=0&&i+3<15&&n[1][i-2]=='0'&&n[1][i+1]=='0'&&n[1][i+2]==WHITE+48&&n[1][i+3]=='0')
                    regionOfPossible[i+1][this->point.y]=1;
            }
        }

        else if(n[1][i]==BLACK+48){
            whiteNum=0;
            ++blackNum;

            //长连（11111型）
            //如果这个11111型的两端都有棋子该怎么办
            if(blackNum>=5){
                if(i+1<15)
                    regionOfPossible[i+1][this->point.y]=1;
                if(i-5>=0)
                    regionOfPossible[i-5][this->point.y]=1;
            }

            //冲四（011112型或211110型）或活四（011110型）时
            else if(blackNum==4){
                if(i-4>=0)
                    regionOfPossible[i-4][this->point.y]=1;
                if(i+1<15)
                    regionOfPossible[i+1][this->point.y]=1;
            }

            //冲四（10111型或11101型）或活三（01110型）
            else if(blackNum==3){
                if(i-4>=0&&n[1][i-4]==BLACK+48&&n[1][i-3]=='0')
                    regionOfPossible[i-3][this->point.y]=1;
                else if(i+2<15&&n[1][i+2]==BLACK+48&&n[1][i+1]=='0')
                    regionOfPossible[i+1][this->point.y]=0;
                else if(i-3>=0&&i+1<15&&n[1][i-3]=='0'&&n[1][i+1]=='0'){
                    regionOfPossible[i-3][this->point.y]=1;
                    regionOfPossible[i+1][this->point.y]=1;
                }
            }

            //冲四（11011型）或活三（010110型或011010型）
            else if(blackNum==2){
                if(i+3<15&&n[1][i+1]=='0'&&n[1][i+2]==BLACK+48&&n[1][i+3]==BLACK+48)
                    regionOfPossible[i+1][this->point.y]=1;
                else if(i-4>=0&&i+1<15&&n[1][i-4]=='0'&&n[1][i-3]==BLACK+48&&n[1][i-2]=='0'&&n[1][i+1]=='0')
                    regionOfPossible[i-2][this->point.y]=1;
                else if(i-2>=0&&i+3<15&&n[1][i-2]=='0'&&n[1][i+1]=='0'&&n[1][i+2]==BLACK+48&&n[1][i+3]=='0')
                    regionOfPossible[i+1][this->point.y]=1;
            }

        }
        else//n[1][i]=='0'
        {
            whiteNum=0;
            blackNum=0;
        }

    }


    blackNum=0;
    whiteNum=0;
    NewPoint temp;   //表示在n[2]中n[2][0]的位置
    if(this->point.y-this->point.x>=0){
        temp.x=0;
        temp.y=this->point.y-this->point.x;
    }
    else{
        temp.x=this->point.x-this->point.y;
        temp.y=0;
    }
    for(i=0;i<static_cast<int>(n[2].length());i++){
        if(n[2][i]==WHITE+48){
            ++whiteNum;
            blackNum=0;

            //长连（11111型）
            //如果这个11111型的两端都有棋子该怎么办
            if(whiteNum>=5){
                if(i+1<static_cast<int>(n[2].length()))
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=1;
                if(i-5>=0)
                    regionOfPossible[temp.x+i-5][temp.y+i-5]=1;
            }

            //冲四（011112型或211110型）或活四（011110型）时
            else if(whiteNum==4){
                if(i-4>=0)
                    regionOfPossible[temp.x+i-4][temp.y+i-4]=1;
                if(i+1<static_cast<int>(n[2].length()))
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=1;
            }

            //冲四（10111型或11101型）或活三（01110型）
            else if(whiteNum==3){
                if(i-4>=0&&n[2][i-4]==WHITE+48&&n[2][i-3]=='0')
                    regionOfPossible[temp.x+i-3][temp.y+i-3]=1;
                else if(i+2<static_cast<int>(n[2].length())&&n[2][i+2]==WHITE+48&&n[2][i+1]=='0')
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=0;
                else if(i-3>=0&&i+1<static_cast<int>(n[2].length())&&n[2][i-3]=='0'&&n[2][i+1]=='0'){
                    regionOfPossible[temp.x+i-3][temp.y+i-3]=1;
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=1;
                }
            }

            //冲四（11011型）或活三（010110型或011010型）
            else if(whiteNum==2){
                if(i+3<static_cast<int>(n[2].length())&&n[2][i+1]=='0'&&n[2][i+2]==WHITE+48&&n[2][i+3]==WHITE+48)
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=1;
                else if(i-4>=0&&i+1<static_cast<int>(n[2].length())&&n[2][i-4]=='0'&&n[2][i-3]==WHITE+48&&n[2][i-2]=='0'&&n[2][i+1]=='0')
                    regionOfPossible[temp.x+i-2][temp.y+i-2]=1;
                else if(i-2>=0&&i+3<static_cast<int>(n[2].length())&&n[2][i-2]=='0'&&n[2][i+1]=='0'&&n[2][i+2]==WHITE+48&&n[2][i+3]=='0')
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=1;
            }
        }

        else if(n[2][i]==BLACK+48){
            whiteNum=0;
            ++blackNum;

            //长连（11111型）
            //如果这个11111型的两端都有棋子该怎么办
            if(blackNum>=5){
                if(i+1<static_cast<int>(n[2].length()))
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=1;
                if(i-5>=0)
                    regionOfPossible[temp.x+i-5][temp.y+i-5]=1;
            }

            //冲四（011112型或211110型）或活四（011110型）时
            else if(blackNum==4){
                if(i-4>=0)
                    regionOfPossible[temp.x+i-4][temp.y+i-4]=1;
                if(i+1<static_cast<int>(n[2].length()))
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=1;
            }

            //冲四（10111型或11101型）或活三（01110型）
            else if(blackNum==3){
                if(i-4>=0&&n[2][i-4]==BLACK+48&&n[2][i-3]=='0')
                    regionOfPossible[temp.x+i-3][temp.y+i-3]=1;
                else if(i+2<static_cast<int>(n[2].length())&&n[2][i+2]==BLACK+48&&n[2][i+1]=='0')
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=0;
                else if(i-3>=0&&i+1<static_cast<int>(n[2].length())&&n[2][i-3]=='0'&&n[2][i+1]=='0'){
                    regionOfPossible[temp.x+i-3][temp.y+i-3]=1;
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=1;
                }
            }

            //冲四（11011型）或活三（010110型或011010型）
            else if(blackNum==2){
                if(i+3<static_cast<int>(n[2].length())&&n[2][i+1]=='0'&&n[2][i+2]==BLACK+48&&n[2][i+3]==BLACK+48)
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=1;
                else if(i-4>=0&&i+1<static_cast<int>(n[2].length())&&n[2][i-4]=='0'&&n[2][i-3]==BLACK+48&&n[2][i-2]=='0'&&n[2][i+1]=='0')
                    regionOfPossible[temp.x+i-2][temp.y+i-2]=1;
                else if(i-2>=0&&i+3<static_cast<int>(n[2].length())&&n[2][i-2]=='0'&&n[2][i+1]=='0'&&n[2][i+2]==BLACK+48&&n[2][i+3]=='0')
                    regionOfPossible[temp.x+i+1][temp.y+i+1]=1;
            }

        }
        else//n[2][i]=='0'
        {
            whiteNum=0;
            blackNum=0;
        }
    }


    blackNum=0;
    whiteNum=0;
    //NewPoint temp;   //表示在n[3]中n[3][0]的位置
    if(14-this->point.y-this->point.x>=0){
        temp.x=0;
        temp.y=this->point.x+this->point.y;
    }
    else{
        temp.x=this->point.x+this->point.y-14;
        temp.y=14;
    }
    for(i=0;i<static_cast<int>(n[3].length());i++){
        if(n[3][i]==WHITE+48){
            ++whiteNum;
            blackNum=0;

            //长连（11111型）
            //如果这个11111型的两端都有棋子该怎么办
            if(whiteNum>=5){
                if(i+1<static_cast<int>(n[3].length()))
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=1;
                if(i-5>=0)
                    regionOfPossible[temp.x+i-5][temp.y-i+5]=1;
            }

            //
            //
            ///一下的未修改
            ///
            ///
            //冲四（011112型或211110型）或活四（011110型）时
            else if(whiteNum==4){
                if(i-4>=0)
                    regionOfPossible[temp.x+i-4][temp.y-i+4]=1;
                if(i+1<static_cast<int>(n[3].length()))
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=1;
            }

            //冲四（10111型或11101型）或活三（01110型）
            else if(whiteNum==3){
                if(i-4>=0&&n[3][i-4]==WHITE+48&&n[3][i-3]=='0')
                    regionOfPossible[temp.x+i-3][temp.y-i+3]=1;
                else if(i+2<static_cast<int>(n[3].length())&&n[3][i+2]==WHITE+48&&n[3][i+1]=='0')
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=0;
                else if(i-3>=0&&i+1<static_cast<int>(n[3].length())&&n[3][i-3]=='0'&&n[3][i+1]=='0'){
                    regionOfPossible[temp.x+i-3][temp.y-i+3]=1;
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=1;
                }
            }

            //冲四（11011型）或活三（010110型或011010型）
            else if(whiteNum==2){
                if(i+3<static_cast<int>(n[3].length())&&n[3][i+1]=='0'&&n[3][i+2]==WHITE+48&&n[3][i+3]==WHITE+48)
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=1;
                else if(i-4>=0&&i+1<static_cast<int>(n[3].length())&&n[3][i-4]=='0'&&n[3][i-3]==WHITE+48&&n[3][i-2]=='0'&&n[3][i+1]=='0')
                    regionOfPossible[temp.x+i-2][temp.y-i+2]=1;
                else if(i-2>=0&&i+3<static_cast<int>(n[3].length())&&n[3][i-2]=='0'&&n[3][i+1]=='0'&&n[3][i+2]==WHITE+48&&n[3][i+3]=='0')
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=1;
            }
        }

        else if(n[3][i]==BLACK+48){
            whiteNum=0;
            ++blackNum;

            //长连（11111型）
            //如果这个11111型的两端都有棋子该怎么办
            if(blackNum>=5){
                if(i+1<static_cast<int>(n[3].length()))
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=1;
                if(i-5>=0)
                    regionOfPossible[temp.x+i-5][temp.y-i+5]=1;
            }

            //冲四（011112型或211110型）或活四（011110型）时
            else if(blackNum==4){
                if(i-4>=0)
                    regionOfPossible[temp.x+i-4][temp.y-i+4]=1;
                if(i+1<static_cast<int>(n[3].length()))
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=1;
            }

            //冲四（10111型或11101型）或活三（01110型）
            else if(blackNum==3){
                if(i-4>=0&&n[3][i-4]==BLACK+48&&n[3][i-3]=='0')
                    regionOfPossible[temp.x+i-3][temp.y-i+3]=1;
                else if(i+2<static_cast<int>(n[3].length())&&n[3][i+2]==BLACK+48&&n[3][i+1]=='0')
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=0;
                else if(i-3>=0&&i+1<static_cast<int>(n[3].length())&&n[3][i-3]=='0'&&n[3][i+1]=='0'){
                    regionOfPossible[temp.x+i-3][temp.y-i+3]=1;
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=1;
                }
            }

            //冲四（11011型）或活三（010110型或011010型）
            else if(blackNum==2){
                if(i+3<static_cast<int>(n[3].length())&&n[3][i+1]=='0'&&n[3][i+2]==BLACK+48&&n[3][i+3]==BLACK+48)
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=1;
                else if(i-4>=0&&i+1<static_cast<int>(n[3].length())&&n[3][i-4]=='0'&&n[3][i-3]==BLACK+48&&n[3][i-2]=='0'&&n[3][i+1]=='0')
                    regionOfPossible[temp.x+i-2][temp.y-i+2]=1;
                else if(i-2>=0&&i+3<static_cast<int>(n[3].length())&&n[3][i-2]=='0'&&n[3][i+1]=='0'&&n[3][i+2]==BLACK+48&&n[3][i+3]=='0')
                    regionOfPossible[temp.x+i+1][temp.y-i-1]=1;
            }

        }
        else//n[3][i]=='0'
        {
            whiteNum=0;
            blackNum=0;
        }
    }




    //剔除已经有棋子的点
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
            if(regionOfPossible[i][j]==1&&this->nBoard[i][j]==0){
                pos[length].x=i;
                pos[length].y=j;
                length++;
                flag=true;
            }
*/

    //否则
    if(!flag){
        for(int i=0;i<15;i++)
            for(int j=0;j<15;j++)
                if(this->nBoard[i][j]!=0)
                    //上下左右各扩充一格来进行下棋
                    for(int m=max(0,i-1);m<=min(i+1,14);m++)
                        for(int n=max(0,j-1);n<=min(j+1,14);n++)
                            regionOfPossible[m][n]=1;
        /*

        //优先把落子点附近的点填入
        for(int m=max(0,this->point.x-1);m<=min(this->point.x+1,14);m++)
            for(int n=max(0,this->point.y-1);n<=min(this->point.y+1,14);n++)
                if(this->nBoard[m][n]==0&&regionOfPossible[m][n]==1){
                    pos[length].x=m;
                    pos[length].y=n;
                    length++;
                    regionOfPossible[m][n]=0;
                }

            */


        //剔除已经有棋子的点
        for(int i=0;i<15;i++)
            for(int j=0;j<15;j++)
                if(this->nBoard[i][j]==0&&regionOfPossible[i][j]==1){
                    pos[length].x=i;
                    pos[length].y=j;
                    length++;
                }
    }

    //GetLocalTime(&a);
    //qDebug()<<"end findpos:"<<a.wMinute<<":"<<a.wSecond<<":"<<a.wMilliseconds<<endl;

    //qDebug()<<"结束第"<<++searchNodeNum<<"次搜索";
    return flag;
}

void GameTree::OptimezePos(NewPoint pos[],int length,bool isMaxNode)
{
    struct temp{
        NewPoint point;
        int score;
        static bool cmp1(temp a,temp b){
            return a.score>b.score;
        }
        static bool cmp2(temp a,temp b){
            return a.score<b.score;
        }
    };
    vector<temp> array;

    bool rush4white;
    bool live3white;
    bool rush4black;
    bool live3black;
    temp t;
    for(int i=0;i<length;i++){
        vector<string> n;
        this->IntArraytoString(n,pos[i]);
        rush4white=false;
        live3white=false;
        rush4black=false;
        live3black=false;
        t.point=pos[i];
        if(isMaxNode)
            t.score=EvaluateByArray(n,rush4white,live3white,rush4black,live3black,!isMaxNode)+positon[pos[i].x][pos[i].y];
        else
            t.score=EvaluateByArray(n,rush4white,live3white,rush4black,live3black,!isMaxNode)-positon[pos[i].x][pos[i].y];
        array.push_back(t);
    }


    if(isMaxNode==true)
        std::sort(array.begin(),array.end(),t.cmp1);        //对array按照score进行降序排序
    else
        std::sort(array.begin(),array.end(),t.cmp2);        //对array按照score进行升序排序

    for(int i=0;i<length;i++)
        pos[i]=array[i].point;
    return;
}

//棋局估值函数，对当前棋盘形势进行判断，数字越大表示己方获胜几率越大
int GameTree::Evaluation()
{
    //SYSTEMTIME a;
    //GetLocalTime(&a);
    //qDebug()<<"begin evaluation:"<<a.wMinute<<":"<<a.wSecond<<":"<<a.wMilliseconds;

    int score=0;

    vector<string> row,col,mainDiagonal,counterDiagonal;
    this->IntArraytoString(row,col,mainDiagonal,counterDiagonal);


    if(FiveKill_Black(row)||FiveKill_Black(col)||FiveKill_Black(mainDiagonal)||FiveKill_Black(counterDiagonal))
        score+=-100000;
    if(FiveKill_White(row)||FiveKill_White(col)||FiveKill_White(mainDiagonal)||FiveKill_White(counterDiagonal))
        score+= 100000;
    if(LiveFour_Black(row)||LiveFour_Black(col)||LiveFour_Black(mainDiagonal)||LiveFour_Black(counterDiagonal)
            ||DoubleRushFour_Black(row,col,mainDiagonal,counterDiagonal))
        score+= -50000;
    //代替RushFourAndLiveThree_Black(row,col,mainDiagonal,counterDiagonal)，减轻计算量
    bool rush4black=false;
    bool live3black=false;
    if(RushFour_Black(row)||RushFour_Black(col)||RushFour_Black(mainDiagonal)||RushFour_Black(counterDiagonal)){
        score+=-4950;
        rush4black=true;
    }
    if(LiveThree_Black(row)||LiveThree_Black(col)||LiveThree_Black(mainDiagonal)||LiveThree_Black(counterDiagonal)){
        score+=-5000;
        live3black=true;
    }
    if(rush4black&&live3black)
        score+= -50000;

    if(LiveFour_White(row)||LiveFour_White(col)||LiveFour_White(mainDiagonal)||LiveFour_White(counterDiagonal)
            ||DoubleRushFour_White(row,col,mainDiagonal,counterDiagonal))
        score+= 50000;
    //代替NewRushFourAndLiveThree_White(white)，减轻计算量
    bool rush4white=false;
    bool live3white=false;
    if(RushFour_White(row)||RushFour_White(col)||RushFour_White(mainDiagonal)||RushFour_White(counterDiagonal)){
        score+=4950;
        rush4white=true;
    }
    if(LiveThree_White(row)||LiveThree_White(col)||LiveThree_White(mainDiagonal)||LiveThree_White(counterDiagonal)){
        score+=5000;
        live3white=true;
    }
    if(rush4white&&live3white)
        score+= 50000;

    if(DoubleLiveThree_Black(row,col,mainDiagonal,counterDiagonal))
        score+= -30000;
    if(DoubleLiveThree_White(row,col,mainDiagonal,counterDiagonal))
        score+= 30000;

    /********************
     * 没有绝杀就累计分数
    *********************/
    int live2num=LiveTwo_Black(row)+LiveTwo_Black(col)+LiveTwo_Black(mainDiagonal)+LiveTwo_Black(counterDiagonal);
    score+=-10*live2num;

    //计算白棋活2、眠三...是否要对整个棋局进行计算...？
    live2num=LiveTwo_White(row)+LiveTwo_White(col)+LiveTwo_White(mainDiagonal)+LiveTwo_White(counterDiagonal);
    score+=10*live2num;


    int sleep3num=SleepThree_Black(row)+SleepThree_Black(col)+SleepThree_Black(mainDiagonal)+SleepThree_Black(counterDiagonal);
    score+=-20*sleep3num;
    sleep3num=SleepThree_White(row)+SleepThree_White(col)+SleepThree_White(mainDiagonal)+SleepThree_White(counterDiagonal);
    score+=20*sleep3num;

    int sleep2num=SleepTwo_Black(row)+SleepTwo_Black(col)+SleepTwo_Black(mainDiagonal)+SleepTwo_Black(counterDiagonal);
    score+=-5*sleep2num;
    sleep2num=SleepTwo_White(row)+SleepTwo_White(col)+SleepTwo_White(mainDiagonal)+SleepTwo_White(counterDiagonal);
    score+=5*sleep2num;


    //GetLocalTime(&a);
    //qDebug()<<"end evaluation:"<<a.wMinute<<":"<<a.wSecond<<":"<<a.wMilliseconds<<endl;

    /********************
     * 死四、死三等暂不考虑
     ********************/
    return score;
}
//新估值函数,最大搜索层数为奇数时使用。白棋用所下点行、列、主对角线方向、副对角线方向的落子情况进行估值，黑棋对整个棋局进行估值
int GameTree::AnotherEvaluation(bool isBlackChess)
{
    //SYSTEMTIME a;
    //GetLocalTime(&a);
    //qDebug()<<"begin evaluation:"<<a.wMinute<<":"<<a.wSecond<<":"<<a.wMilliseconds;

    int score=0;

    vector<string> row,col,mainDiagonal,counterDiagonal;
    this->IntArraytoString(row,col,mainDiagonal,counterDiagonal);

    //用于判断双冲四、双活三
    bool rush4white=false;
    bool live3white=false;
    bool rush4black=false;
    bool live3black=false;

    score=EvaluateByArray(row,rush4white,live3white,rush4black,live3black,isBlackChess)
            +EvaluateByArray(col,rush4white,live3white,rush4black,live3black,isBlackChess)
            +EvaluateByArray(mainDiagonal,rush4white,live3white,rush4black,live3black,isBlackChess)
            +EvaluateByArray(counterDiagonal,rush4white,live3white,rush4black,live3black,isBlackChess);

    if(live3black&&rush4black)
        score-=50000;
    else if(live3white&&rush4white)
        score+=50000;

    //这一步是黑棋，下一步就是白棋，此时白方有冲四白必赢，或黑方无冲四，白方有活三，白必赢。否则，反之
    if(isBlackChess==true&&(rush4white||(live3white&&!rush4black)))
        score+=50000;
    else if(isBlackChess==false&&(rush4black||(live3black&&!rush4white)))
        score-=50000;


    /********************
     * 死四、死三等暂不考虑
     ********************/
    //qDebug()<<"估值执行正常";


    return score;
}


int GameTree::EvaluateByArray(vector<string> n,bool &rush4white,bool &live3white,bool &rush4black,bool &live3black,bool isBlackChess)
{
    int score=0;
    int i;//i指向当前位置
    int blackNum;//连续的黑子、白子数
    int whiteNum;

    blackNum=0;
    whiteNum=0;
    for(auto &temp:n){
        for(i=0;i<static_cast<int>(temp.length());i++){
            if(temp[i]==WHITE+48){
                ++whiteNum;
                blackNum=0;

                //长连（11111型）
                //如果这个11111型的两端都有棋子该怎么办
                if(whiteNum>=5)
                    score+=9999999;

                //活四（011110型）或冲四（011112型或211110型）时
                else if(whiteNum==4){
                    if((i-4>=0&&temp[i-4]=='0')&&(i+1<static_cast<int>(temp.length())&&temp[i+1]=='0'))
                        score+=100000;
                    else if((i-4>=0&&temp[i-4]=='0')||(i+1<static_cast<int>(temp.length())&&temp[i+1]=='0')){
                        score+=4950;
                        if(rush4white)
                            score+=50000;
                        else
                            rush4white=true;
                    }
                }

                //冲四（10111型或11101型）或活三（01110型）或眠三（001112型或211100型或2011102型）
                else if(whiteNum==3){
                    if(i-4>=0&&temp[i-4]==WHITE+48&&temp[i-3]=='0'){
                        score+=4950;
                        if(rush4white)
                            score+=50000;
                        else
                            rush4white=true;
                    }
                    else if(i+2<static_cast<int>(temp.length())&&temp[i+2]==WHITE+48&&temp[i+1]=='0'){
                        score+=4950;
                        if(rush4white)
                            score+=50000;
                        else
                            rush4white=true;
                    }
                    else if(i-3>=0&&i+1<static_cast<int>(temp.length())&&temp[i-3]=='0'&&temp[i+1]=='0'){
                        score+=5000;
                        if(live3white)
                            score+=40000;
                        else
                            live3white=true;
                    }
                    else if((i-4>=0&&i+1<static_cast<int>(temp.length())&&temp[i-4]=='0'&&temp[i-3]=='0'&&temp[i+1]==BLACK+48)
                            ||(i-3>=0&&i+2<static_cast<int>(temp.length())&&temp[i-3]==BLACK+48&&temp[i+1]=='0'&&temp[i+2]=='0')
                            ||(i-4>=0&&i+2<static_cast<int>(temp.length())&&temp[i-4]==BLACK+48&&temp[i-3]=='0'&&temp[i+1]=='0'&&temp[i+2]==BLACK+48))
                        score+=20;
                }

                //冲四（11011型）或活三（010110型或011010型）或活二（00110型或01100型）或眠三（010112|210110或|211010|011012或10011或11001）
                //或眠二（000112或211000或2011002或2001102）
                else if(whiteNum==2){
                    if(i+3<static_cast<int>(temp.length())&&temp[i+1]=='0'&&temp[i+2]==WHITE+48&&temp[i+3]==WHITE+48){
                        score+=4950;
                        if(rush4white)
                            score+=50000;
                        else
                            rush4white=true;
                    }
                    else if(i-4>=0&&i+1<static_cast<int>(temp.length())&&temp[i-4]=='0'&&temp[i-3]==WHITE+48&&temp[i-2]=='0'&&temp[i+1]=='0'){
                        score+=5000;
                        if(live3white)
                            score+=40000;
                        else
                            live3white=true;
                    }
                    else if(i-2>=0&&i+3<static_cast<int>(temp.length())&&temp[i-2]=='0'&&temp[i+1]=='0'&&temp[i+2]==WHITE+48&&temp[i+3]=='0'){
                        score+=5000;
                        if(live3white)
                            score+=40000;
                        else
                            live3white=true;
                    }
                    else if((i-3>=0&&i+1<static_cast<int>(temp.length())&&temp[i-3]=='0'&&temp[i-2]=='0'&&temp[i+1]=='0')
                            ||(i-2>=0&&i+2<static_cast<int>(temp.length())&&temp[i-2]=='0'&&temp[i+1]=='0'&&temp[i+2]=='0'))
                        score+=30;
                    else if((i-4>=0&&i+1<static_cast<int>(temp.length())&&((temp[i-4]=='0'&&temp[i-3]==WHITE+48&&temp[i-2]=='0'&&temp[i+1]==BLACK+48)
                                                                           ||(temp[i-4]==BLACK+48&&temp[i-3]==WHITE+48&&temp[i-2]=='0'&&temp[i+1]=='0')))
                            ||(i-2>=0&&i+3<static_cast<int>(temp.length())&&((temp[i-2]==BLACK+48&&temp[i+1]=='0'&&temp[i+2]==WHITE+48&&temp[i+3]=='0')
                                                                             ||(temp[i-2]=='0'&&temp[i+1]=='0'&&temp[i+2]==WHITE+48&&temp[i+3]==BLACK+48)))
                            ||(i-4>=0&&temp[i-4]==WHITE+48&&temp[i-3]=='0'&&temp[i-2]=='0')
                            ||(i+3<static_cast<int>(temp.length())&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]==WHITE+48))
                        score+=20;
                    else if((i-4>=0&&i+1<static_cast<int>(temp.length())&&temp[i-4]=='0'&&temp[i-3]=='0'&&temp[i-2]=='0'&&temp[i+1]==BLACK+48)
                            ||(i-2>=0&&i+3<static_cast<int>(temp.length())&&temp[i-2]==BLACK+48&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]=='0')
                            ||(i-3>=0&&i+3<static_cast<int>(temp.length())&&temp[i-3]==BLACK+48&&temp[i-2]=='0'&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]==BLACK+48)
                            ||(i-4>=0&&i+2<static_cast<int>(temp.length())&&temp[i-4]==BLACK+48&&temp[i-3]=='0'&&temp[i-2]=='0'&&temp[i+1]=='0'&&temp[i+2]==BLACK+48))
                        score+=5;
                }

                //活二（01010或010010）或眠三（10101）或眠二（001012或210100|010012|210010或10001或2010102）
                else//whiteNum==1
                {
                    if((i-1>=0&&i+3<static_cast<int>(temp.length())&&temp[i-1]=='0'&&temp[i+1]=='0'&&temp[i+2]==WHITE+48&&temp[i+3]=='0')
                            ||(i-1>=0&&i+4<static_cast<int>(temp.length())&&temp[i-1]=='0'&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]==WHITE+48&&temp[i+4]=='0'))
                        score+=30;
                    else if(i+4<static_cast<int>(temp.length())&&temp[i+1]=='0'&&temp[i+2]==WHITE+48&&temp[i+3]=='0'&&temp[i+4]==WHITE+48)
                        score+=20;
                    else if((i-2>=0&&i+3<static_cast<int>(temp.length())&&temp[i-2]=='0'&&temp[i-1]=='0'&&temp[i+1]=='0'&&temp[i+2]==WHITE+48&&temp[i+3]==BLACK+48)
                            ||(i-1>=0&&i+4<static_cast<int>(temp.length())&&((temp[i-1]==BLACK+48&&temp[i+1]=='0'&&temp[i+2]==WHITE+48&&temp[i+3]=='0'&&temp[i+4]=='0')
                                                                             ||(temp[i-1]=='0'&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]==WHITE+48&&temp[i+4]==BLACK+48)
                                                                             ||(temp[i-1]==BLACK+48&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]==WHITE+48&&temp[i+4]=='0')))
                            ||(i+4<static_cast<int>(temp.length())&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]=='0'&&temp[i+4]==WHITE+48)
                            ||(i-2>=0&&i+4<static_cast<int>(temp.length())&&temp[i-2]==BLACK+48&&temp[i-1]=='0'&&temp[i+1]=='0'&&temp[i+2]==WHITE+48&&temp[i+3]=='0'&&temp[i+4]==BLACK+48))
                        score+=5;
                }
            }

            else if(temp[i]==BLACK+48){
                whiteNum=0;
                ++blackNum;
                int length=static_cast<int>(temp.length());
                //长连（11111型）
                //如果这个11111型的两端都有棋子该怎么办
                if(blackNum>=5)
                    score-=9999999;

                //活四（011110型）或冲四（011112型或211110型）时
                else if(blackNum==4){
                    if((i-4>=0&&temp[i-4]=='0')&&(i+1<length&&temp[i+1]=='0'))
                        score-=100000;
                    else if((i-4>=0&&temp[i-4]=='0')||(i+1<length&&temp[i+1]=='0')){
                        score-=4950;
                        if(rush4black)
                            score-=50000;
                        else
                            rush4black=true;
                    }
                }

                //冲四（10111型或11101型）或活三（01110型）或眠三（001112型或211100型或2011102型）
                else if(blackNum==3){
                    if(i-4>=0&&temp[i-4]==BLACK+48&&temp[i-3]=='0'){
                        score-=4950;
                        if(rush4black)
                            score-=50000;
                        else
                            rush4black=true;
                    }
                    else if(i+2<length&&temp[i+2]==BLACK+48&&temp[i+1]=='0'){
                        score-=4950;
                        if(rush4black)
                            score-=50000;
                        else
                            rush4black=true;
                    }
                    else if(i-3>=0&&i+1<length&&temp[i-3]=='0'&&temp[i+1]=='0'){
                        score-=5000;
                        if(live3black)
                            score-=40000;
                        else
                            live3black=true;
                    }
                    else if((i-4>=0&&i+1<length&&temp[i-4]=='0'&&temp[i-3]=='0'&&temp[i+1]==WHITE+48)
                            ||(i-3>=0&&i+2<length&&temp[i-3]==WHITE+48&&temp[i+1]=='0'&&temp[i+2]=='0')
                            ||(i-4>=0&&i+2<length&&temp[i-4]==WHITE+48&&temp[i-3]=='0'&&temp[i+1]=='0'&&temp[i+2]==WHITE+48))
                        score-=20;
                }

                //冲四（11011型）或活三（010110型或011010型）或活二（00110型或01100型）或眠三（010112|210110或|211010|011012或10011或11001）
                //或眠二（000112或211000或2011002或2001102）
                else if(blackNum==2){
                    if(i+3<length&&temp[i+1]=='0'&&temp[i+2]==BLACK+48&&temp[i+3]==BLACK+48){
                        score-=4950;
                        if(rush4black)
                            score-=50000;
                        else
                            rush4black=true;
                    }
                    else if(i-4>=0&&i+1<length&&temp[i-4]=='0'&&temp[i-3]==BLACK+48&&temp[i-2]=='0'&&temp[i+1]=='0'){
                        score-=5000;
                        if(live3black)
                            score-=40000;
                        else
                            live3black=true;
                    }
                    else if(i-2>=0&&i+3<length&&temp[i-2]=='0'&&temp[i+1]=='0'&&temp[i+2]==BLACK+48&&temp[i+3]=='0'){
                        score-=5000;
                        if(live3black)
                            score-=40000;
                        else
                            live3black=true;
                    }
                    else if((i-3>=0&&i+1<length&&temp[i-3]=='0'&&temp[i-2]=='0'&&temp[i+1]=='0')
                            ||(i-2>=0&&i+2<length&&temp[i-2]=='0'&&temp[i+1]=='0'&&temp[i+2]=='0'))
                        score-=30;
                    else if((i-4>=0&&i+1<length&&((temp[i-4]=='0'&&temp[i-3]==BLACK+48&&temp[i-2]=='0'&&temp[i+1]==WHITE+48)
                                                  ||(temp[i-4]==WHITE+48&&temp[i-3]==BLACK+48&&temp[i-2]=='0'&&temp[i+1]=='0')))
                            ||(i-2>=0&&i+3<length&&((temp[i-2]==WHITE+48&&temp[i+1]=='0'&&temp[i+2]==BLACK+48&&temp[i+3]=='0')
                                                    ||(temp[i-2]=='0'&&temp[i+1]=='0'&&temp[i+2]==BLACK+48&&temp[i+3]==WHITE+48)))
                            ||(i-4>=0&&temp[i-4]==BLACK+48&&temp[i-3]=='0'&&temp[i-2]=='0')
                            ||(i+3<length&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]==BLACK+48))
                        score-=20;
                    else if((i-4>=0&&i+1<length&&temp[i-4]=='0'&&temp[i-3]=='0'&&temp[i-2]=='0'&&temp[i+1]==WHITE+48)
                            ||(i-2>=0&&i+3<length&&temp[i-2]==WHITE+48&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]=='0')
                            ||(i-3>=0&&i+3<length&&temp[i-3]==WHITE+48&&temp[i-2]=='0'&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]==WHITE+48)
                            ||(i-4>=0&&i+2<length&&temp[i-4]==WHITE+48&&temp[i-3]=='0'&&temp[i-2]=='0'&&temp[i+1]=='0'&&temp[i+2]==WHITE+48))
                        score-=5;
                }

                //活二（01010或010010）或眠三（10101）或眠二（001012或210100|010012|210010或10001或2010102）
                else//blackNum==1
                {
                    if((i-1>=0&&i+3<length&&temp[i-1]=='0'&&temp[i+1]=='0'&&temp[i+2]==BLACK+48&&temp[i+3]=='0')
                            ||(i-1>=0&&i+4<length&&temp[i-1]=='0'&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]==BLACK+48&&temp[i+4]=='0'))
                        score-=30;
                    else if(i+4<length&&temp[i+1]=='0'&&temp[i+2]==BLACK+48&&temp[i+3]=='0'&&temp[i+4]==BLACK+48)
                        score-=20;
                    else if((i-2>=0&&i+3<length&&temp[i-2]=='0'&&temp[i-1]=='0'&&temp[i+1]=='0'&&temp[i+2]==BLACK+48&&temp[i+3]==WHITE+48)
                            ||(i-1>=0&&i+4<length&&((temp[i-1]==WHITE+48&&temp[i+1]=='0'&&temp[i+2]==BLACK+48&&temp[i+3]=='0'&&temp[i+4]=='0')
                                                    ||(temp[i-1]=='0'&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]==BLACK+48&&temp[i+4]==WHITE+48)
                                                    ||(temp[i-1]==WHITE+48&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]==BLACK+48&&temp[i+4]=='0')))
                            ||(i+4<length&&temp[i+1]=='0'&&temp[i+2]=='0'&&temp[i+3]=='0'&&temp[i+4]==BLACK+48)
                            ||(i-2>=0&&i+4<length&&temp[i-2]==WHITE+48&&temp[i-1]=='0'&&temp[i+1]=='0'&&temp[i+2]==BLACK+48&&temp[i+3]=='0'&&temp[i+4]==WHITE+48))
                        score-=5;
                }
            }
            else//temp[i]=='0'
            {
                whiteNum=0;
                blackNum=0;
            }
        }
    }



    return score;
}


//剪枝
int GameTree::Alpha_BetaCut(int depth,int alpha,int beta,int searchDepth)
{
    if(depth==searchDepth){
        if(depth%2==1)
            return this->AnotherEvaluation(true);
        else
            return this->AnotherEvaluation(false);
    }


    NewPoint *pos=new NewPoint[255];
    int n;

    this->FindPossiblePos(pos,n);
    if(this->FindPossiblePos(pos,n)==false){
        if(depth%2==1)//MAX结点
            this->OptimezePos(pos,n,true);
        else
            this->OptimezePos(pos,n,false);
        //if(n>25)
        //    n=25;
    }


    if(n==0)
        qDebug()<<"！！！！！！\n可行数组长度为0\n！！！！！！";



    for(int i=0;i<n;i++)
    {
        ++searchNodeNum;


        GameTree *node=new GameTree;
        node->InitCSNode(this);
        if(depth%2==1)
            node->nBoard[pos[i].x][pos[i].y]=WHITE;
        else
            node->nBoard[pos[i].x][pos[i].y]=BLACK;
        node->point.x=pos[i].x;
        node->point.y=pos[i].y;

        //递归
        int score=node->Alpha_BetaCut(depth+1,alpha,beta,searchDepth);

        //剪枝
        if(depth%2==1){
            //max结点
            //存入最佳点
            if(depth==1&&score>=bestPoint.score){
                if(score>bestPoint.score){
                    bestPoint.length=0;
                    //qDebug()<<"执行清零！！！";
                }
                bestPoint.pos[bestPoint.length].x=pos[i].x;
                bestPoint.pos[bestPoint.length].y=pos[i].y;
                bestPoint.length++;
                bestPoint.score=score;
                //qDebug()<<bestPoint.length<<"个 score:"<<bestPoint.score<<"     pos"<<
                          //bestPoint.pos[bestPoint.length-1].x<<"  "<<bestPoint.pos[bestPoint.length-1].y<<"  alpha:"<<alpha<<"  beta:"<<beta;
            }

            if(score>alpha){
               // qDebug()<<"白棋alpha:"<<alpha<<" 更新alpha:"<<score<<"   行："<<pos[i].x<<"列："<<pos[i].y;
                alpha=score;

            }
            //剪枝
            //这里的>=改成>是为了防止最佳点录入(bestpoint部分)出现bug
            if(alpha>beta){
               // qDebug()<<"max剪枝 alpha:"<<alpha<<" beta:"<<beta<<"  pos:"<<pos[i].x<<"  "<<pos[i].y;
                delete this->firstChild;
                this->firstChild=nullptr;
                break;
            }
        }

        //min结点
        else{
            if(score<beta){
                //qDebug()<<"黑棋beta:"<<beta<<" 更新beta:"<<score<<"   行："<<pos[i].x<<"列："<<pos[i].y;
                beta=score;

            }
            if(alpha>beta){
               // qDebug()<<"min剪枝 alpha:"<<alpha<<" beta:"<<beta<<"  pos:"<<pos[i].x<<"  "<<pos[i].y;
                delete this->firstChild;
                this->firstChild=nullptr;
                break;
            }
        }



            //qDebug()<<"bestpointscore "<<alpha<<"     length:"<<bestPoint.length;
            //qDebug()<<pos[i].x<<" "<<pos[i].y<<endl;

    }





    delete []pos;

    if(depth%2==1)
        return alpha;
    //min结点
    else
        return beta;

}
//将整个棋盘行、列、主对角线方向、副对角线方向的落子情况写入vector中
void GameTree::IntArraytoString(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal)
{
    for(int i=0;i<15;i++){
        std::ostringstream temp("");
        for(int j=0;j<15;j++)
            temp<<this->nBoard[i][j];
        row.push_back(temp.str());
    }

    for(int i=0;i<15;i++){
        std::ostringstream temp("");
        for(int j=0;j<15;j++)
            temp<<this->nBoard[j][i];
        col.push_back(temp.str());
    }

    //对角线方向有29条
    for(int i=14;i>=0;i--){
        std::ostringstream temp("");
        for(int j=0;j<=14-i;j++)
            temp<<this->nBoard[j][i+j];
        mainDiagonal.push_back(temp.str());
    }
    for(int i=1;i<=14;i++){
        std::ostringstream temp("");
        for(int j=0;j<=14-i;j++)
            temp<<this->nBoard[i+j][j];
        mainDiagonal.push_back(temp.str());
    }

    for(int i=14;i>=0;i--){
        std::ostringstream temp("");
        for(int j=0;j<=14-i;j++)
            temp<<this->nBoard[j][14-i-j];
        counterDiagonal.push_back(temp.str());
    }
    for(int i=1;i<=14;i++){
        std::ostringstream temp("");
        for(int j=0;j<=14-i;j++)
            temp<<this->nBoard[i+j][14-j];
        counterDiagonal.push_back(temp.str());
    }
}
//将落子点行、列、主对角线方向、副对角线方向的落子情况写入n中
void GameTree::IntArraytoString(vector<string> &n,NewPoint pos)
{
    //本来应该考虑的是落子点各个方向各扩充5~6格情形，但考虑15格也差不多
    std::ostringstream row(""),col(""),mainDiagonal(""),counterDiagonal("");
    for(int i=0;i<15;i++)
        row<<this->nBoard[pos.x][i];
    n.push_back(row.str());

    for(int i=0;i<15;i++)
        col<<this->nBoard[i][pos.y];
    n.push_back((col.str()));

    if(pos.y>=pos.x){
        for(int i=0;i<15-pos.y+pos.x;i++)
            mainDiagonal<<this->nBoard[i][i+pos.y-pos.x];
    }
    else{
        for(int i=0;i<15-pos.x+pos.y;i++)
            mainDiagonal<<this->nBoard[i+pos.x-pos.y][i];
    }
    n.push_back(mainDiagonal.str());

    if(14-pos.x-pos.y>=0){
        for(int i=0;i<=pos.x+pos.y;i++)
            counterDiagonal<<this->nBoard[i][pos.x+pos.y-i];
    }
    else{
        for(int i=14;i>=pos.x+pos.y-14;i--)
            counterDiagonal<<this->nBoard[pos.x+pos.y-i][i];
    }
    n.push_back(counterDiagonal.str());
}
//判断是否胜利
bool GameTree::IsHumanWin(NewPoint pos)
{
    vector<string> n;
    this->IntArraytoString(n,pos);
    if(FiveKill_Black(n))
        return true;
    else
        return false;
}
bool GameTree::IsComputerWin(NewPoint pos)
{
    vector<string> n;
    this->IntArraytoString(n,pos);
    if(FiveKill_White(n))
        return true;
    else
        return false;
}
//整合逻辑，以便在game.cpp中调用
WhoWin GameTree::Play(NewPoint humanPos,NewPoint &computerPos)
{
    SYSTEMTIME a;
    GetLocalTime(&a);
    qDebug()<<"begin play:"<<a.wMinute<<":"<<a.wSecond<<":"<<a.wMilliseconds<<endl;

    //初始化
    this->InitCSNode(nullptr);
    this->point.x=humanPos.x;
    this->point.y=humanPos.y;
    //if(head)
     //   delete head;
    //head=new SelectNode;
    //SelectNode::length=0;


    if(this->IsHumanWin(humanPos))
        return BlackWin;

    chessNum++;
    if(chessNum==255)
        return Stalemate;

    bestPoint.score=-80000000;
    bestPoint.length=0;


    if(chessNum<15)
        this->Alpha_BetaCut(1,-80000000,80000000,3);
    else
        this->Alpha_BetaCut(1,-80000000,80000000,SEARCHDEPTH);


    qDebug()<<"搜索结点数："<<searchNodeNum;
    searchNodeNum=0;


    int temp;
    if(bestPoint.length!=0){
        srand(static_cast<unsigned int>(time(nullptr)));
        temp=rand()%bestPoint.length;
    }
    else{
        //qDebug()<<"\n！！！发生不该发生的错误！！！\nbestpoint可行长度为0，但当前棋盘有空位";
        exit(-1);
    }


    computerPos.x=bestPoint.pos[temp].x;
    computerPos.y=bestPoint.pos[temp].y;

    chessNum++;
    //将白棋的点填入并进行判断
    this->nBoard[computerPos.x][computerPos.y]=WHITE;
    if(this->IsComputerWin(computerPos))
        return WhiteWin;
    if(chessNum==255)
        return Stalemate;


    /*qDebug()<<"length:"<<bestPoint.length<<"  computer pos:"<<computerPos.x<<"   "<<computerPos.y;
    for(int i=0;i<bestPoint.length;i++)
        qDebug()<<i+1<<"个 score:"<<bestPoint.score<<"     pos"<<bestPoint.pos[i].x<<"  "<<bestPoint.pos[i].y;
        */
    GetLocalTime(&a);
    qDebug()<<"end play:"<<a.wMinute<<":"<<a.wSecond<<":"<<a.wMilliseconds<<endl;
    return StillPlay;
}


/*int SelectNode::length=0;
SelectNode::SelectNode()
{
    this->next=nullptr;
}
SelectNode::SelectNode(GameTree *p,int s)
{
    this->pnode=p;
    this->score=s;
    this->next=nullptr;
}
SelectNode::~SelectNode()
{
    if(this->next)
        delete this->next;
}
void SelectNode::Add(SelectNode *p)
{
    //按照score从大到小排序
    SelectNode *temp=this;
    while (temp->next) {
        if(p->score>temp->next->score)
            break;
        temp=temp->next;
    }
    p->next=temp->next;
    temp->next=p;
    SelectNode *t=this;
    if(length==SELECTNODENUM){
        //删掉最后一个
        while (t->next!=nullptr) {
            t=t->next;
        }
        delete t;
    }
    else
        length++;
}
*/











static const regex white_fivekill("1{5,}");
static const regex black_fivekill("2{5,}");
static const regex white_livefour("011110");
static const regex black_livefour("022220");
static const regex white_rushfour("011112|211110|0101110|0111010|0110110");
static const regex black_rushfour("022221|122220|0202220|0222020|0220220");
static const regex white_livethree("01110|010110|011010");
static const regex black_livethree("02220|020220|022020");
static const regex white_livetwo("00110|01100|01010|010010");
static const regex black_livetwo("00220|02200|02020|020020");
static const regex white_sleepthree("001112|211100|010112|211010|011012|210110|10011|11001|10101|2011102");
static const regex black_sleepthree("002221|122200|020221|122020|022021|120220|20022|22002|20202|1022201");
static const regex white_sleeptwo("000112|211000|001012|210100|010012|210010|10001|2010102|2011002|2001102");
static const regex black_sleeptwo("000221|122000|002021|120200|020021|120020|20002|1020201|1022001|1002201");

//长连
bool FiveKill_White(vector<string> &n){
    for(auto& temp:n)
        if(regex_search(temp,white_fivekill))
            return true;
    return false;
}
bool FiveKill_Black(vector<string> &n){
    for(auto& temp:n)
        if(regex_search(temp,black_fivekill))
            return true;
    return false;
}
//活四
bool LiveFour_White(vector<string> &n){
    for(auto& temp:n)
        if(regex_search(temp,white_livefour))
            return true;
    return false;
}
bool LiveFour_Black(vector<string> &n){
    for(auto& temp:n)
        if(regex_search(temp,black_livefour))
            return true;
    return false;
}
//双冲四
bool DoubleRushFour_White(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal){
    bool flag=false;
    for(auto& temp:row)
        if(regex_search(temp,white_rushfour)){
            if(flag==true)
                return true;
            else
                flag=true;
        }
    for(auto& temp:col)
        if(regex_search(temp,white_rushfour)){
            if(flag==true)
                return true;
            else
                flag=true;
        }
    for(auto& temp:mainDiagonal)
        if(regex_search(temp,white_rushfour)){
            if(flag==true)
                return true;
            else
                flag=true;
        }
    for(auto& temp:counterDiagonal)
        if(regex_search(temp,white_rushfour)){
            if(flag==true)
                return true;
            else
                flag=true;
        }
    return false;
}
bool DoubleRushFour_Black(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal){
    bool flag=false;
    for(auto& temp:row)
        if(regex_search(temp,black_rushfour)){
            if(flag==true)
                return true;
            else
                flag=true;
        }
    for(auto& temp:col)
        if(regex_search(temp,black_rushfour)){
            if(flag==true)
                return true;
            else
                flag=true;
        }
    for(auto& temp:mainDiagonal)
        if(regex_search(temp,black_rushfour)){
            if(flag==true)
                return true;
            else
                flag=true;
        }
    for(auto& temp:counterDiagonal)
        if(regex_search(temp,black_rushfour)){
            if(flag==true)
                return true;
            else
                flag=true;
        }
    return false;
}
bool NewDoubleRushFour_White(vector<string> &n){
    bool flag=false;
    for(auto& temp:n)
        if(regex_search(temp,white_rushfour)){
            if(flag)
                return true;
            else
                flag=true;
        }
    return false;
}
bool NewDoubleRushFour_Black(vector<string> &n){
    bool flag=false;
    for(auto& temp:n)
        if(regex_search(temp,black_rushfour)){
            if(flag)
                return true;
            else
                flag=true;
        }
    return false;
}
//冲四活三
bool RushFourAndLiveThree_White(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal){
    bool rush4flag=false;
    bool live3flag=false;
    for(auto& temp:row)
        if(regex_search(temp,white_rushfour)){
            rush4flag=true;
            goto live3;
        }
    for(auto& temp:col)
        if(regex_search(temp,white_rushfour)){
            rush4flag=true;
            goto live3;
        }
    for(auto& temp:mainDiagonal)
        if(regex_search(temp,white_rushfour)){
            rush4flag=true;
            goto live3;
        }
    for(auto& temp:counterDiagonal)
        if(regex_search(temp,white_rushfour)){
            rush4flag=true;
            goto live3;
        }

live3:
    for(auto& temp:row)
        if(regex_search(temp,white_livethree)){
            live3flag=true;
            goto judge;
        }
    for(auto& temp:col)
        if(regex_search(temp,white_livethree)){
            live3flag=true;
            goto judge;
        }
    for(auto& temp:mainDiagonal)
        if(regex_search(temp,white_livethree)){
            live3flag=true;
            goto judge;
        }
    for(auto& temp:counterDiagonal)
        if(regex_search(temp,white_livethree)){
            live3flag=true;
            goto judge;
        }

judge:
    if(live3flag&&rush4flag)
        return true;
    else
        return false;
}
bool RushFourAndLiveThree_Black(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal){
    bool rush4flag=false;
    bool live3flag=false;
    for(auto& temp:row)
        if(regex_search(temp,black_rushfour)){
            rush4flag=true;
            goto live3;
        }
    for(auto& temp:col)
        if(regex_search(temp,black_rushfour)){
            rush4flag=true;
            goto live3;
        }
    for(auto& temp:mainDiagonal)
        if(regex_search(temp,black_rushfour)){
            rush4flag=true;
            goto live3;
        }
    for(auto& temp:counterDiagonal)
        if(regex_search(temp,black_rushfour)){
            rush4flag=true;
            goto live3;
        }

live3:
    for(auto& temp:row)
        if(regex_search(temp,black_livethree)){
            live3flag=true;
            goto judge;
        }
    for(auto& temp:col)
        if(regex_search(temp,black_livethree)){
            live3flag=true;
            goto judge;
        }
    for(auto& temp:mainDiagonal)
        if(regex_search(temp,black_livethree)){
            live3flag=true;
            goto judge;
        }
    for(auto& temp:counterDiagonal)
        if(regex_search(temp,black_livethree)){
            live3flag=true;
            goto judge;
        }

judge:
    if(live3flag&&rush4flag)
        return true;
    else
        return false;
}
bool NewRushFourAndLiveThree_White(vector<string> &n){
    bool rush4flag=false;
    bool live3flag=false;
    for(auto& temp:n)
        if(regex_search(temp,white_rushfour)){
            rush4flag=true;
            break;
        }
    for(auto& temp:n)
        if(regex_search(temp,white_livethree)){
            live3flag=true;
            break;
        }
    if(rush4flag&&live3flag)
        return true;
    else
        return false;
}
bool NewRushFourAndLiveThree_Black(vector<string> &n){
    bool rush4flag=false;
    bool live3flag=false;
    for(auto& temp:n)
        if(regex_search(temp,black_rushfour)){
            rush4flag=true;
            break;
        }
    for(auto& temp:n)
        if(regex_search(temp,black_livethree)){
            live3flag=true;
            break;
        }
    if(rush4flag&&live3flag)
        return true;
    else
        return false;
}
//双活三

bool DoubleLiveThree_White(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal){
    bool flag=false;
    for(auto& temp:row)
        if(regex_search(temp,white_livethree)){
            if(flag)
                return true;
            else
                flag=true;
        }
    for(auto& temp:col)
        if(regex_search(temp,white_livethree)){
            if(flag)
                return true;
            else
                flag=true;
        }
    for(auto& temp:mainDiagonal)
        if(regex_search(temp,white_livethree)){
            if(flag)
                return true;
            else
                flag=true;
        }
    for(auto& temp:counterDiagonal)
        if(regex_search(temp,white_livethree)){
            if(flag)
                return true;
            else
                flag=true;
        }
    return false;
}

bool DoubleLiveThree_Black(vector<string> &row,vector<string> &col,vector<string> &mainDiagonal,vector<string> &counterDiagonal){
    bool flag=false;
    for(auto& temp:row)
        if(regex_search(temp,black_livethree)){
            if(flag)
                return true;
            else
                flag=true;
        }
    for(auto& temp:col)
        if(regex_search(temp,black_livethree)){
            if(flag)
                return true;
            else
                flag=true;
        }
    for(auto& temp:mainDiagonal)
        if(regex_search(temp,black_livethree)){
            if(flag)
                return true;
            else
                flag=true;
        }
    for(auto& temp:counterDiagonal)
        if(regex_search(temp,black_livethree)){
            if(flag)
                return true;
            else
                flag=true;
        }
    return false;
}
bool NewDoubleLiveThree_White(vector<string> &n){
    bool flag=false;
    for(auto& temp:n)
        if(regex_search(temp,white_livethree)){
            if(flag)
                return true;
            else
                flag=true;
        }
    return false;
}
bool NewDoubleLiveThree_Black(vector<string> &n){
    bool flag=false;
    for(auto& temp:n)
        if(regex_search(temp,black_livethree)){
            if(flag)
                return true;
            else
                flag=true;
        }
    return false;
}

//冲四
bool RushFour_White(vector<string> &n)
{
    for(auto& temp:n)
        if(regex_search(temp,white_rushfour))
            return true;
    return false;
}
bool RushFour_Black(vector<string> &n)
{
    for(auto& temp:n)
        if(regex_search(temp,black_rushfour))
            return true;
    return false;
}
//活三
bool LiveThree_White(vector<string> &n){
    for(auto& temp:n)
        if(regex_search(temp,white_livethree))
            return true;
    return false;
}
bool LiveThree_Black(vector<string> &n){
    for(auto& temp:n)
        if(regex_search(temp,black_livethree))
            return true;
    return false;
}
//活二
int LiveTwo_White(vector<string> &n){
    int num=0;
    for(auto& temp:n)
        if(regex_search(temp,white_livetwo))
            num++;
    return num;
}
int LiveTwo_Black(vector<string> &n){
    int num=0;
    for(auto& temp:n)
        if(regex_search(temp,black_livetwo))
            num++;
    return num;
}
//眠三
int SleepThree_White(vector<string> &n){
    int num=0;
    for(auto& temp:n)
        if(regex_search(temp,white_sleepthree))
            num++;
    return num;
}
int SleepThree_Black(vector<string> &n){
    int num=0;
    for(auto& temp:n)
        if(regex_search(temp,black_sleepthree))
            num++;
    return num;
}
//眠二
int SleepTwo_White(vector<string> &n){
    int num=0;
    for(auto& temp:n)
        if(regex_search(temp,white_sleeptwo))
            num++;
    return num;
}
int SleepTwo_Black(vector<string> &n){
    int num=0;
    for(auto& temp:n)
        if(regex_search(temp,black_sleeptwo))
            num++;
    return num;
}











