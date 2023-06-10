#include "Chess.h"
#include<math.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

//自定义的图形渲染接口,因为easyx图形库默认不支持背景透明的png格式图片，棋子图片会有背景色，这段代码纯引用
void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
    // 变量初始化
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
    int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
    int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
    int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
    int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
    int dstX = 0;    //在显存里像素的角标

    // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //在显存里像素的角标
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
            int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
            }
        }
    }
}

Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
{
    this->gradeSize = gradeSize;
    this->margin_x = marginX;
    this->margin_y = marginY;
    this->chessSize = chessSize;
    playerFlag = CHESS_BLACK;

    //初始化 chessmap
    for (int i = 0; i < gradeSize; i++) {
        vector<int>row;
        for (int j = 0; j < gradeSize; j++) {
            row.push_back(0);
        }
        chessmap.push_back(row);
    }


}

void Chess::init()
{
    initgraph(897, 895);
    loadimage(0, "res/棋盘2.jpg");

    mciSendString("play res/start.wav", 0, 0, 0);//音乐

    loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);//chessSize:棋子宽&高
    loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, false);

    for (int i = 0; i < chessmap.size(); i++) {
        for (int j = 0; j < chessmap.size(); j++) {
            chessmap[i][j] = 0;
        }
    }
    playerFlag = true;
}

//判断落子位置是否有效
bool Chess::ClickBoard(int x, int y, ChessPos* pos)
{
    int col = (x - margin_x) / chessSize;//点击位置左上角列
    int row = (y - margin_y) / chessSize;//点击位置左上角行
    int leftTopPosX = margin_x + chessSize * col;//左上角行坐标
    int leftTopPosY = margin_y + chessSize * row;//左上角列坐标
    int offset = chessSize * 0.4;//设置阈值

    int len;//点击位置距离边界点的距离
    int selectPos = false;

    do {
        //距离左上角的距离
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));//勾股定理
        if (len < offset) {
            pos->row = row;
            pos->col = col;
            if (chessmap[pos->row][pos->col] == 0) {//判断棋盘(chessMap)中该行列位置是否为空，空就可以落子
                selectPos = true;
            }
            break;
        }

        //距离右上角的距离
        len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY) * (y - leftTopPosY ));
        if (len < offset) {
            pos->row = row;
            pos->col = col + 1;//row 和 col 均是左上角的坐标，所以col要+1，表示右上角坐标
            if (chessmap[pos->row][pos->col] == 0) {
                selectPos = true;
            }
            break;
        }

        //距离左下角距离
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
        if (len < offset) {
            pos->row = row + 1;
            pos->col = col;
            if (chessmap[pos->row][pos->col] == 0) {
                selectPos = true;
            }
            break;
        }

        //距离右下角距离
        len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
        if (len < offset) {
            pos->row = row + 1;
            pos->col = col + 1;
            if (chessmap[pos->row][pos->col] == 0) {
                selectPos = true;
            }
            break;
        }
    } while (0);
    
    return selectPos;
}

//落子
void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
    mciSendString("play res/down7.WAV", 0, 0, 0);//Windows系统调用MCI，播放落子音效
    int x = margin_x + pos->col * chessSize - 0.5 * chessSize;
    int y = margin_y + pos->row * chessSize - 0.5 * chessSize;

    if (kind == CHESS_WHITE) {
        putimagePNG(x, y, &chessWhiteImg);
    }
    else {
        putimagePNG(x, y, &chessBlackImg);
    }

    updateGameMap(pos);
}

int Chess::getGradeSize()
{
    return gradeSize;
}

int Chess::getChessData(ChessPos* pos)
{
    return chessmap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
    return chessmap[row][col];
}

//对胜负进行处理
bool Chess::checkover()
{
    if (checkWin()) {
        Sleep(1500);
        if (playerFlag == false) {//即下一步该白棋下（黑棋赢了）;
            loadimage(0, "res/胜利.png");
        }
        else {//（白棋赢，man输了）
            loadimage(0, "res/失败.png");
        }
        Sleep(5000);
        return true;
    }
    return false;
}

void Chess::updateGameMap(ChessPos* pos)
{
    chessmap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;//设置当前玩家落子后对应的棋盘格子颜色
    playerFlag = !playerFlag;//交换下棋方
    lastPos = *pos;
}

//判断是否胜负已分，原理就是对刚才的落子位置进行判断，判断该位置在4个方向上是否有5颗连续的同类棋子。
bool Chess::checkWin()
{
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    int row = lastPos.row;
    int col = lastPos.col;

    for (int i = 0; i < 5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if (col - i >= 0 &&
            col - i + 4 < gradeSize &&
            chessmap[row][col - i] == chessmap[row][col - i + 1] &&
            chessmap[row][col - i] == chessmap[row][col - i + 2] &&
            chessmap[row][col - i] == chessmap[row][col - i + 3] &&
            chessmap[row][col - i] == chessmap[row][col - i + 4])
            return true;
    }

    // 竖直方向(上下延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (row - i >= 0 &&
            row - i + 4 < gradeSize &&
            chessmap[row - i][col] == chessmap[row - i + 1][col] &&
            chessmap[row - i][col] == chessmap[row - i + 2][col] &&
            chessmap[row - i][col] == chessmap[row - i + 3][col] &&
            chessmap[row - i][col] == chessmap[row - i + 4][col])
            return true;
    }

    // “/"方向
    for (int i = 0; i < 5; i++)
    {
        if (row + i < gradeSize &&
            row + i - 4 >= 0 &&
            col - i >= 0 &&
            col - i + 4 < gradeSize &&
            // 第[row+i]行，第[col-i]的棋子，与右上方连续4个棋子都相同
            chessmap[row + i][col - i] == chessmap[row + i - 1][col - i + 1] &&
            chessmap[row + i][col - i] == chessmap[row + i - 2][col - i + 2] &&
            chessmap[row + i][col - i] == chessmap[row + i - 3][col - i + 3] &&
            chessmap[row + i][col - i] == chessmap[row + i - 4][col - i + 4])
            return true;
    }

    // “\“ 方向
    for (int i = 0; i < 5; i++)
    {
        // 第[row+i]行，第[col-i]的棋子，与右下方连续4个棋子都相同
        if (row - i >= 0 &&
            row - i + 4 < gradeSize &&
            col - i >= 0 &&
            col - i + 4 < gradeSize &&
            chessmap[row - i][col - i] == chessmap[row - i + 1][col - i + 1] &&
            chessmap[row - i][col - i] == chessmap[row - i + 2][col - i + 2] &&
            chessmap[row - i][col - i] == chessmap[row - i + 3][col - i + 3] &&
            chessmap[row - i][col - i] == chessmap[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}
