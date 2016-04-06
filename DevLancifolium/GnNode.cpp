#include "GnNode.h"

void GnNode::init(struct GnNode *par) { // 初始化（此處的默認參數不能加）
	parent = par; // 父節點

	next = NULL;
	nxt.clear();
	nxt.reserve(0); // 初始分配空間爲0

	stoneProp = EMPTYPOINT; // 空節點，脫先
	mov = NONE_MOV; // 走子和走子自然手數，沒有走子記爲-1

	addblacks.clear();
	addblacks.reserve(0);
	addwhites.clear();
	addwhites.reserve(0);

	labels.clear();
	labels.reserve(0);
} // finished init

GnNode::GnNode() {
	init();
}

int GnNode::insertNextNode(struct GnNode *tmpnxt) { // 挿入後續節點
	tmpnxt->parent = this;
	nxt.push_back(tmpnxt);
} // finished insertNextNode

int GnNode::insertAddStones(int tmpmov, int colour) {
	if (colour == 1) { // black stones
		addblacks.push_back(tmpmov);
	}
	else { // white stones
		addwhites.push_back(tmpmov);
	}
}

