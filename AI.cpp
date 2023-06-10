#include "AI.h"

//AI��ʼ��
void AI::init(Chess* chess)
{
	this->chess = chess;
	int size = chess->getGradeSize();

	for (int i = 0; i < size; i++) {
		vector<int>row;
		for (int j = 0; j < size; j++) {
			row.push_back(0);
		}
		scoreMap.push_back(row);
	}
}
//AI����
void AI::go()
{
	ChessPos pos = think();
	Sleep(1000);//����AI˼������Ӱ����Ϸ����
	chess->chessDown(&pos, CHESS_WHITE);
}

//ÿ��������
void AI::calculateScore()
{
	int personNum = 0; //�����������
	int aiNum = 0; //�����������
	int emptyNum = 0; //�հ�λ����

	// ����������������
	for (int i = 0; i < scoreMap.size(); i++) {
		for (int j = 0; j < scoreMap[i].size(); j++) {
			scoreMap[i][j] = 0;
		}
	}

	int size = chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			//��ÿ������м���
			if (chess->getChessData(row, col)) continue;

			for (int y = -1; y <= 0; y++) {        //Y�ķ�Χ����-1�� 0
				for (int x = -1; x <= 1; x++) {    //X�ķ�Χ�� -1,0,1
					if (y == 0 && x == 0) continue;
					if (y == 0 && x != 1) continue; //��y=0ʱ��������x=1

					personNum = 0;
					aiNum = 0;
					emptyNum = 0;

					// ����������������-������
					for (int i = 1; i <= 4; i++) {
						int curRow = row + i * y;
						int curCol = col + i * x;

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							personNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}

					// �����������
					for (int i = 1; i <= 4; i++) {
						int curRow = row - i * y;
						int curCol = col - i * x;

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							personNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}

					//���������������������ʲô���
					if (personNum == 1) { //����
						scoreMap[row][col] += 10;
					}
					else if (personNum == 2) {
						if (emptyNum == 1) {//����
							scoreMap[row][col] += 30;
						}
						else if (emptyNum == 2) {//����
							scoreMap[row][col] += 40;
						}
					}
					else if (personNum == 3) {
						if (emptyNum == 1) {//����
							scoreMap[row][col] = 60;
						}
						else if (emptyNum == 2) {//����
							scoreMap[row][col] = 5000; //200
						}
					}
					else if (personNum == 4) {//����
						scoreMap[row][col] = 20000;
					}

					// ����������������-������
					emptyNum = 0;

					for (int i = 1; i <= 4; i++) {
						int curRow = row + i * y;
						int curCol = col + i * x;		

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aiNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
					//�������������
					for (int i = 1; i <= 4; i++) {
						int curRow = row - i * y;
						int curCol = col - i * x;

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aiNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
					
					//������������ڴˣ������ʲô������߼�ͬ����
					if (aiNum == 0) {
						scoreMap[row][col] += 5;
					}
					else if (aiNum == 1) {
						scoreMap[row][col] += 10;
					}
					else if (aiNum == 2) {
						if (emptyNum == 1) {
							scoreMap[row][col] += 25;
						}
						else if (emptyNum == 2) {
							scoreMap[row][col] += 50;
						}
					}
					else if (aiNum == 3) {
						if (emptyNum == 1) {
							scoreMap[row][col] += 55;
						}
						else if (emptyNum == 2) {
							scoreMap[row][col] += 10000;
						}
					}
					else if (aiNum >= 4) {
						scoreMap[row][col] += 30000;
					}
				}
			}
		}
	}
}
	



//AI˼���������ҳ�������ߵĵ�
ChessPos AI::think()
{
	calculateScore();

	//���������ҳ�������
	int maxscore = 0;
	vector<ChessPos>maxPoints;
	int k = 0;

	int size = chess->getGradeSize();//���̴�С

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (chess->getChessData(row, col) == 0) {//ֻ���ǿ�����
				if (scoreMap[row][col] > maxscore) {
					maxscore = scoreMap[row][col];
					maxPoints.clear();
					maxPoints.push_back(ChessPos(row, col));
				}
				else if(scoreMap[row][col] == maxscore){//�ж�������������ȵģ���������
					maxPoints.push_back(ChessPos(row,col));
				}
			}
		}
	}
	//���ж����ͬ��С�ĵ㣬������
	int index = rand() % maxPoints.size();
	return maxPoints[index];
}
