#include "ChessGame.h"

//���캯����ʼ������
ChessGame::ChessGame(Man* man, AI* ai, Chess* chess) {
	this->man = man;
	this->ai = ai;
	this->chess = chess;

	man->init(chess);//��ʼ������
	ai->init(chess);//��ʼ��AI
}

void ChessGame::play()
{
	chess->init();
	while (1) {
		//ÿ��һ���ж��Ƿ���Ϸ����
		man->go();//������
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
