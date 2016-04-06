#ifndef GNCALCULATE_H
#define GNCALCULATE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <stack>
#include <list>

#include "GnNode.h"

#define MAXBORDSIZE 26 // 最大棋盤規模

// for tmpbord[][]
#define TB_EMPTYPOINT 0 // 空點
#define TB_BLACKSTONE 1 // 黑子
#define TB_WHITESTONE 2 // 白子
#define TB_SAMECOLOUR 3 // 同色
#define TB_DIFFCOLOUR 4 //
#define TB_LIFTSTONES 5
#define TB_DIFFNOLIFT 6

// for lift
#define CANNOTPLACE 0 //
#define LIFTSTONES  1
#define NOLIFTS     2

struct GnCalculate { // 計算相關函數
	int siz;
	char ston[MAXBORDSIZE][MAXBORDSIZE];
    char calcbord[MAXBORDSIZE][MAXBORDSIZE];

    //struct GnSave currsave;
    //std::list<struct GnSave> bordList; // 保存棋盤
    std::list<int> moveList; // 保存move，個十位縱坐標，百千位橫坐標，萬位表示有棋盤變化

    std::list<struct GnBord> bordList; // Save movs

	int conflict; // 劫爭
	int confmove; // 劫爭熱點

	void init(); // 初始化

	int findStoneBlock(int colour, int tmpcolour, int pos, int tmpsiz, int &stonnum); // 落子判斷函數

	int configDropStone(int colour, int mov); // 著子判斷函數

	int saveCurrBord(int colour, int mov); // 保存棋盤

	int regainMove(); // 回退
};

#endif // !GNCALCULATE_H
