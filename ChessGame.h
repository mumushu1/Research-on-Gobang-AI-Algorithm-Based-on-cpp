#pragma once
#include "Man.h"
#include"AI.h"
#include "Chess.h"

class ChessGame
{
public:
	ChessGame(Man* man, AI* ai, Chess* chess);//添加3各数据成员，构造函数初始化这三个数据成员
	void play();

private:
	Man* man;
	AI* ai;
	Chess* chess;
};



