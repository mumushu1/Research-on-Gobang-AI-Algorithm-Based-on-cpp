#pragma once
#include "Man.h"
#include"AI.h"
#include "Chess.h"

class ChessGame
{
public:
	ChessGame(Man* man, AI* ai, Chess* chess);//���3�����ݳ�Ա�����캯����ʼ�����������ݳ�Ա
	void play();

private:
	Man* man;
	AI* ai;
	Chess* chess;
};



