#ifndef GNNODE_H
#define GNNODE_H


#include "FunLancifolium.h"

using std::string;
using std::vector;

#define EMPTYPOINT 0 // 沒有落子
#define BLACKSTONE 1 // 黑子
#define WHITESTONE 2 // 白子
#define NONE_MOV -1 // 沒有走子記爲-1

typedef struct GnNode { // 讀取棋譜文件所用節點，亦當可用於戰鬥
	struct GnNode *parent; // 父節點

	vector<struct GnNode *> nxt; // 分支節點線性表

	unsigned char stoneProp; // 0空節點，1黑走子，2白走子
	int mov; // 走子：百位以上爲橫坐標，百位以下爲縱坐標，例如909即坐標天元，最大爲2626

	vector<int> addblacks; // 添加黑子
	vector<int> addwhites; // 添加白子

	vector<int> labels; // 標籤(LB) 長度 labanum，萬位以上爲標籤對應數字值，千百爲縱坐標，個十爲橫坐標

    string comment; // 評論
    string nodename; // 節點名稱

	vector<int> liftsave; // 提子保存
	unsigned char liftcolour; // 提子顏色

	void init(struct GnNode *par = NULL); // 此處默認參數在後面不能加
	GnNode();
    GnNode(struct GnNode *par);

	int insertNextNode(struct GnNode *tmpnxt);
	int insertAddStones(int tmpmov, int colour);

	/* 判斷 */
	inline bool operator==(const GnNode &tmpnode) const;

	int joinsame(const GnNode &tmpnode);

    int jud_nextmov(int tmpmov); // 判斷下一個落子是否在nxt中，

	void printing() {
		printf("\n|%p|%p|[%d](%d)", parent, this, mov, stoneProp);
	}
	void printbase() {
		//printf("|%d|%d|%d|", stoneProp, mov, nxtnum);
		printf("[%d]", mov);
	}
} GnNode;

struct GnLift { // 保存提子信息，GnNode中已經包含了這個功能
    vector<int> bordsave; // 提子列表
    int colour; // 提子顏色
    void init() {
        bordsave.reserve(0);
    }
    GnLift() {
        init();
    }
}; // only for GnCalculate

#endif // !GNNODE_H
