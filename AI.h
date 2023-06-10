#pragma once
#include "Chess.h"

class AI
{
public:
	void init(Chess *chess);
	
	void go();

private:
	Chess* chess;//和man一样，用啥规格的棋盘下棋
	vector<vector<int>>scoreMap;//AI在各个点位的打分，作为AI落子的依据

private:
	void calculateScore();
	ChessPos think();//AI思考
};

