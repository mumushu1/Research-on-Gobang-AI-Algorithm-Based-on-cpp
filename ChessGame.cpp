#include "ChessGame.h"

//构造函数初始化棋盘
ChessGame::ChessGame(Man* man, AI* ai, Chess* chess) {
	this->man = man;
	this->ai = ai;
	this->chess = chess;

	man->init(chess);//初始化棋手
	ai->init(chess);//初始化AI
}

void ChessGame::play()
{
	chess->init();
	while (1) {
		//每走一步判断是否游戏结束
		man->go();//人先手
		if (chess->checkover()) {
			chess->init();
			continue;
		}

		ai->go();
		if (chess->checkover()) {
			chess->init();
			continue;
		}
	}
}
