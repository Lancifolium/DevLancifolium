#include "GnNode.h"

void GnNode::init(struct GnNode *par) { // 初始化（此處的默認參數不能加）
	parent = par; // 父節點

	next = NULL; // 子節點
	nxt = NULL;
	nxtnum = 0;

	stoneProp = EMPTYPOINT; // 空節點，脫先
	mov = NONE_MOV; // 走子和走子自然手數，沒有走子記爲-1

	addblacks = addwhites = NULL;
	numblacks = numwhites = 0;

	labels = NULL; labanum = 0; // 標籤和標籤數目
	comment = NULL; nodename = NULL; // 評論、節點名稱
} // finished init

GnNode::GnNode() {
	init();
}

int GnNode::remove() {
	if (labels != NULL) {
		free(labels); labels = NULL;
	}
	if (comment != NULL) {
		free(comment); comment = NULL;
	}
	if (nodename != NULL) {
		free(nodename); nodename = NULL;
	}
	int tmpi;
	for (tmpi = 0; tmpi < nxtnum; tmpi++) {
		if (nxt[tmpi] != NULL) break;
	}
	if (tmpi == nxtnum) {
		free(nxt); nxt = NULL;
	}
}


int GnNode::insertNextNode(struct GnNode *tmpnxt) {
	tmpnxt->parent = this;
	if (nxtnum == 0) {
		nxt = new GnNode *; *nxt = tmpnxt; nxtnum++;
	}
	else {
		GnNode **tmpval;
		int tmpi;
		tmpval = nxt;
		nxt = new GnNode *[nxtnum + 1];
		for (tmpi = 0; tmpi < nxtnum; tmpi++) nxt[tmpi] = tmpval[tmpi];
		nxt[tmpi] = tmpnxt; nxtnum++;
	}
} // finished insertNextNode

int GnNode::insertAddStones(int *tmpcoors, int tmpnum, int colour) {
	if (colour == 1) { // black stones
		if (addblacks == NULL) {
			addblacks = new int[tmpnum];
			for (int tmpi = 0; tmpi < tmpnum; tmpi++) {
				addblacks[tmpi] = tmpcoors[tmpi];
			}
			numblacks = tmpnum;
		}
		else {
			int *tmpaddblacks = addblacks;
			addblacks = new int[tmpnum + numblacks];
			for (int tmpi = 0; tmpi < numblacks; tmpi++) {
				addblacks[tmpi] = tmpaddblacks[tmpi];
			}
			for (int tmpi = numblacks; tmpi < numblacks + tmpnum; tmpi++) {
				addblacks[tmpi] = tmpcoors[tmpi];
			}
			numblacks += tmpnum;
		}
	}
	else { // white stones
		if (addwhites == NULL) {
			addwhites = new int[tmpnum];
			for (int tmpi = 0; tmpi < tmpnum; tmpi++) {
				addwhites[tmpi] = tmpcoors[tmpi];
			}
			numwhites = tmpnum;
		}
		else {
			int *tmpaddwhites = addwhites;
			addwhites = new int[tmpnum + numwhites];
			for (int tmpi = 0; tmpi < numwhites; tmpi++) {
				addwhites[tmpi] = tmpaddwhites[tmpi];
			}
			for (int tmpi = numwhites; tmpi < numwhites + tmpnum; tmpi++) {
				addwhites[tmpi] = tmpcoors[tmpi];
			}
			numwhites += tmpnum;
		}
	}
}

int GnNode::insertLabels(int *tmplab, int tmplength) { //
	int tmpi;
	if (labels == NULL) {
		labanum = tmplength;
		labels = new int[labanum];
		for (tmpi = 0; tmpi < labanum; tmpi++) labels[tmpi] = tmplab[tmpi];
	}
	else {
		int *tmplist;
		tmplist = labels;
		labels = new int[labanum + tmplength];
		for (tmpi = 0; tmpi < labanum; tmpi++) labels[tmpi] = tmplist[tmpi];
		for (tmpi = 0; tmpi < tmplength; tmpi++) labels[labanum + tmpi] = tmplab[tmpi];
		labanum += tmplength;
		free(tmplist);
	}
}

int GnNode::insertComment(char *tmpcomment, int tmplen) {
	if (comment != NULL) free(comment);
	comment = new char[tmplen];
	strcpy(comment, tmpcomment);
}

int GnNode::insertNodename(char *tmpnodename, int tmplen) {
	if (nodename != NULL) free(nodename);
	nodename = new char[tmplen];
	strcpy(nodename, tmpnodename);
}
