#pragma once
#include <graphics.h>
#include <vector>
using namespace std;

typedef enum {
	CHESS_WHITE = -1,
	CHESS_BLACK = 1
}chess_kind_t;

struct ChessPos {
	int row;
	int col;
	ChessPos(int r = 0, int c = 0) :row(r), col(c) {}
};

class Chess
{
public:
	Chess(int gradeSize,int marginX,int marginY,float chessSize);//构造函数初始化

	void init();//棋盘数据初始化，使得能够看到实际的棋盘

	bool ClickBoard(int x, int y, ChessPos* pos);//判断点击是否有效，若有效，将位置（行列）保存到pos

	void chessDown(ChessPos* pos, chess_kind_t kind);//在指定位置落子

	int getGradeSize();//获取棋盘大小

	//获取指定位置是黑棋，还是白棋，还是空白
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	bool checkover();//游戏结束处理工作

private:
	//棋盘尺寸
	int	gradeSize;
	float margin_x;//左侧边界（整个画面从左边起始到棋盘第一列的位置）
	int margin_y;
	float chessSize;//棋子大小 = 棋盘方格大小
	ChessPos lastPos; //最近落子位置

	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;

	//存储当前游戏棋盘和棋子的情况，空白0，白棋-1，黑棋1
	//定义一个名为 chessmap 的二维向量，用于存储表示棋盘及其上的棋子信息
	vector<vector<int>> chessmap;

	//指明下棋方，true：黑方，false：白方
	bool playerFlag;

	//落子后修改棋盘的棋子数据
	void updateGameMap(ChessPos* pos);

	//胜负已分，返回true，否则false
	bool checkWin();
};

