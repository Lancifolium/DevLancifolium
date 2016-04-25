#include "GnNode.h"

void GnNode::init(struct GnNode *par) { // 初始化（此處的默認參數不能加）
	parent = par; // 父節點

	nxt.clear();
	nxt.reserve(0); // 初始分配空間爲0

	stoneProp = EMPTYPOINT; // 空節點，脫先
	mov = NONE_MOV; // 走子和走子自然手數，沒有走子記爲-1
	liftcolour = 0;

	addblacks.clear(); // 這些以後試著刪除
	addblacks.reserve(0);
	addwhites.clear();
	addwhites.reserve(0);

	labels.clear();
	labels.reserve(0);
	liftsave.clear();
	liftsave.reserve(0);
} // finished init

GnNode::GnNode() {
	init();
}

GnNode::GnNode(struct GnNode *par) {
    init(par);
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

inline bool GnNode::operator==(const GnNode &tmpnode) const {
	if (this->stoneProp != tmpnode.stoneProp) return false;
	if (this->mov != tmpnode.mov) return false;
	/* add black stones */
	if (this->addblacks.size() != tmpnode.addblacks.size()) return false;
	else {
		for (int tmpi = 0; tmpi < this->addblacks.size(); tmpi++) {
			if (this->addblacks[tmpi] != tmpnode.addblacks[tmpi]) return false;
		}
	}
	/* add white stones */
	if (this->addwhites.size() != tmpnode.addwhites.size()) return false;
	else {
		for (int tmpi = 0; tmpi < this->addwhites.size(); tmpi++) {
			if (this->addwhites[tmpi] != tmpnode.addwhites[tmpi]) return false;
		}
	}
	printf("mov|%d|%d, stp|%d|%d, adb|%d|%d, adw|%d|%d\n",
		this->mov, tmpnode.mov, this->stoneProp, tmpnode.stoneProp,
		this->addblacks.size(), tmpnode.addblacks.size(),
		this->addwhites.size(), tmpnode.addwhites.size()); /* --------------------- */
	return true;
}

int GnNode::joinsame(const GnNode &tmpnode) { /* 將一個節點合併進來 */
	/* 評論和節點名*/
	if (this->comment != tmpnode.comment) this->comment += tmpnode.comment;
	if (this->nodename != tmpnode.nodename) this->nodename += tmpnode.nodename;
	/* 標籤 */
	this->labels.insert(this->labels.end(), tmpnode.labels.begin(), tmpnode.labels.end());
	std::sort(this->labels.begin(), this->labels.end(), sortcmp);
	vector<int>::iterator iter = std::unique(this->labels.begin(), this->labels.end());
	this->labels.erase(iter, this->labels.end());
	/* 後續節點 */
	printf("$ p(%p, %p), mov(%d, %d), nxtlist(%d, %d), ", this, &tmpnode, this->mov, tmpnode.mov, this->nxt.size(), tmpnode.nxt.size());
	this->nxt.insert(this->nxt.end(), tmpnode.nxt.begin(), tmpnode.nxt.end());
	printf("finally nxt: %d; \n", this->nxt.size());
}

int GnNode::jud_nextmov(int tmpmov) { /* 判斷下一手是否在子節點中 */
	for (int tmpi = 0; tmpi < nxt.size(); tmpi++) {
		if (nxt[tmpi]->mov == tmpmov) return 1;
	} /* 先實現再說，這裡用不著高明算法 */
	return 0;
}