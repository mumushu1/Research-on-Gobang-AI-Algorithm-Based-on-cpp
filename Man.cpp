#include "Man.h"

void Man::init(Chess* chess)
{
	this->chess = chess;
}

//��������
void Man::go()
{
	MOUSEMSG msg;//�����Ϣ
	ChessPos pos;//����λ��
	while (1) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && chess->ClickBoard(msg.x, msg.y, &pos)) {//���������������λ����ȷ
			break;
		}
	}
	//����
	chess->chessDown(&pos, CHESS_BLACK);
}
