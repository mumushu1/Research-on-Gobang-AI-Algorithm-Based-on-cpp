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
	Chess(int gradeSize,int marginX,int marginY,float chessSize);//���캯����ʼ��

	void init();//�������ݳ�ʼ����ʹ���ܹ�����ʵ�ʵ�����

	bool ClickBoard(int x, int y, ChessPos* pos);//�жϵ���Ƿ���Ч������Ч����λ�ã����У����浽pos

	void chessDown(ChessPos* pos, chess_kind_t kind);//��ָ��λ������

	int getGradeSize();//��ȡ���̴�С

	//��ȡָ��λ���Ǻ��壬���ǰ��壬���ǿհ�
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	bool checkover();//��Ϸ����������

private:
	//���̳ߴ�
	int	gradeSize;
	float margin_x;//���߽磨��������������ʼ�����̵�һ�е�λ�ã�
	int margin_y;
	float chessSize;//���Ӵ�С = ���̷����С
	ChessPos lastPos; //�������λ��

	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;

	//�洢��ǰ��Ϸ���̺����ӵ�������հ�0������-1������1
	//����һ����Ϊ chessmap �Ķ�ά���������ڴ洢��ʾ���̼����ϵ�������Ϣ
	vector<vector<int>> chessmap;

	//ָ�����巽��true���ڷ���false���׷�
	bool playerFlag;

	//���Ӻ��޸����̵���������
	void updateGameMap(ChessPos* pos);

	//ʤ���ѷ֣�����true������false
	bool checkWin();
};

