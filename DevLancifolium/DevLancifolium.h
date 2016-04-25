#ifndef DEVLANCIFOLIUM_H
#define DEVLANCIFOLIUM_H

#include <algorithm>
#include "GnCalculate.h"
#include "GnNode.h"
#include "FunLancifolium.h"

#define TRIANGLE 1 /* 三角形 */
#define DIAMOND 2 /* 方塊 */
#define FORK 3 /* 叉 */
#define CIRCLE 4 /* 圓 */

struct DevLancifolium { // 全局結構
	int siz; // 棋盤規格

	struct GnNode *root; // 根節點

	FILE *filebuff;
	char reader;

	struct GnNode *curNode; // 當前節點指針
	struct GnNode *tmpNode; // 臨時指針
	std::stack<struct GnNode *> branchStack; // 分支棧

    /* 初始 */
	void init();
	void deleteroot(struct GnNode *tmproot);
	void clearall();
	DevLancifolium();
	~DevLancifolium();

    /* 棋譜信息函數 */
	int dealSize();


    /* 讀取棋譜函數 */
	int openfile(char *filename) { // 讀入文件
		filebuff = fopen(filename, "r");
		if (filebuff == NULL) return 0;
		else return 1;
	} // finished openfile

    /* 讀取處理 */
	int dealAddStones(struct GnNode *tmpnode, int colour);

	int dealMove(struct GnNode *tmpnode, int colour);

	int dealCommentNodename(struct GnNode *tmpnode, int tmpkind);

	int dealLabels(struct GnNode *tmpnode, int form);

    /* 節點處理函數 */
	int configNode();

    /* 讀取棋譜主函數 */
	int configManual(char *filename);

    /* 棋譜內部調整函數 */
    int adjustnxtlist(vector<struct GnNode *> &tmpnxt);
    int adjustManual(GnNode *tempnode);
	int joinManuals(DevLancifolium &sig); /* 合併兩個棋譜 */

    /* 棋譜寫出函數 */
    int saveManual(char *filename);

    /* 測試函數 */
	int reverse(int deep, struct GnNode *cur);

	int printfManual(); // 測試函數，用來輸出讀取棋譜
};

#endif // DEVLANCIFOLIUM_H
