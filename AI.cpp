#include "AI.h"

//AI初始化
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
//AI走棋
void AI::go()
{
	ChessPos pos = think();
	Sleep(1000);//避免AI思考过快影响游戏体验
	chess->chessDown(&pos, CHESS_WHITE);
}

//每个点评分
void AI::calculateScore()
{
	int personNum = 0; //连续黑棋个数
	int aiNum = 0; //连续白棋个数
	int emptyNum = 0; //空白位个数

	// 评分向量数组清零
	for (int i = 0; i < scoreMap.size(); i++) {
		for (int j = 0; j < scoreMap[i].size(); j++) {
			scoreMap[i][j] = 0;
		}
	}

	int size = chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			//对每个点进行计算
			if (chess->getChessData(row, col)) continue;

			for (int y = -1; y <= 0; y++) {        //Y的范围还是-1， 0
				for (int x = -1; x <= 1; x++) {    //X的范围是 -1,0,1
					if (y == 0 && x == 0) continue;
					if (y == 0 && x != 1) continue; //当y=0时，仅允许x=1

					personNum = 0;
					aiNum = 0;
					emptyNum = 0;

					// 计算连续黑棋数量-正方向
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

					// 反向继续计算
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

					//假设黑棋落子在这里会出现什么情况
					if (personNum == 1) { //连二
						scoreMap[row][col] += 10;
					}
					else if (personNum == 2) {
						if (emptyNum == 1) {//活三
							scoreMap[row][col] += 30;
						}
						else if (emptyNum == 2) {//死三
							scoreMap[row][col] += 40;
						}
					}
					else if (personNum == 3) {
						if (emptyNum == 1) {//活四
							scoreMap[row][col] = 60;
						}
						else if (emptyNum == 2) {//死四
							scoreMap[row][col] = 5000; //200
						}
					}
					else if (personNum == 4) {//连五
						scoreMap[row][col] = 20000;
					}

					// 计算连续白棋数量-正方向
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
					//反方向继续计算
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
					
					//假设白棋落在在此，会出现什么情况，逻辑同黑棋
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
	



//AI思考，遍历找出评分最高的点
ChessPos AI::think()
{
	calculateScore();

	//从评分中找出最大分数
	int maxscore = 0;
	vector<ChessPos>maxPoints;
	int k = 0;

	int size = chess->getGradeSize();//棋盘大小

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (chess->getChessData(row, col) == 0) {//只考虑空坐标
				if (scoreMap[row][col] > maxscore) {
					maxscore = scoreMap[row][col];
					maxPoints.clear();
					maxPoints.push_back(ChessPos(row, col));
				}
				else if(scoreMap[row][col] == maxscore){//有多个分数最大且相等的，都存起来
					maxPoints.push_back(ChessPos(row,col));
				}
			}
		}
	}
	//若有多个相同大小的点，随机落点
	int index = rand() % maxPoints.size();
	return maxPoints[index];
}
