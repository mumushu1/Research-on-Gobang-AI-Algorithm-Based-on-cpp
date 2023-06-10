#pragma once
#include "Chess.h"

class AI
{
public:
	void init(Chess *chess);
	
	void go();

private:
	Chess* chess;//��manһ������ɶ������������
	vector<vector<int>>scoreMap;//AI�ڸ�����λ�Ĵ�֣���ΪAI���ӵ�����

private:
	void calculateScore();
	ChessPos think();//AI˼��
};

