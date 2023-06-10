#include "Man.h"

void Man::init(Chess* chess)
{
	this->chess = chess;
}

//棋手走棋
void Man::go()
{
	MOUSEMSG msg;//鼠标信息
	ChessPos pos;//棋子位置
	while (1) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && chess->ClickBoard(msg.x, msg.y, &pos)) {//点击鼠标左键且落子位置正确
			break;
		}
	}
	//落子
	chess->chessDown(&pos, CHESS_BLACK);
}
