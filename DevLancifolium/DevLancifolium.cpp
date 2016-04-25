#include "DevLancifolium.h"

void DevLancifolium::init() {
	siz = 19; root = NULL;
}

void DevLancifolium::deleteroot(GnNode *tmproot) {
	if (tmproot != NULL) {
		for (int tmpi = 0; tmpi < tmproot->nxt.size(); tmpi++) {
			deleteroot(tmproot->nxt[tmpi]);
		}
		delete tmproot;
	}
}

void DevLancifolium::clearall() {
	//while (!branchStack.empty()) branchStack.pop();
	deleteroot(root);
	init();
}

DevLancifolium::DevLancifolium() {
	init();
}

DevLancifolium::~DevLancifolium() {
	printf("\nRunning Destructor. \n");
	//clearall();
}

int DevLancifolium::dealSize() {
	char tmpnum[3];//, reader;
	int tmpi;
	reader = fgetc(filebuff); // 棄了'['
	tmpi = 0;
	while (isdigit(reader)) {
		tmpnum[tmpi++] = reader; reader = fgetc(filebuff);
		if (tmpi > 2) break;
	}
	tmpnum[tmpi] = '\0';
	siz = atoi(tmpnum);
	if (siz < 4) siz = 4; else if (siz > 26) siz = 26;
	reader = fgetc(filebuff); //
	return 1;
}

int DevLancifolium::dealAddStones(struct GnNode *tmpnode, int colour) {
	int tmpx, tmpy;

	if (colour == 0) return 0;
	else if (colour == 1) { // 添加黑子
		while (reader == '[') {
			tmpx = toupper(fgetc(filebuff)) - 'A';
			tmpy = toupper(fgetc(filebuff)) - 'A';
			if ((tmpx >= 0) && (tmpx < siz) && (tmpy >= 0) && (tmpy < siz)) {
				tmpnode->addblacks.push_back(tmpx * 100 + tmpy);
			}
			reader = fgetc(filebuff); // 棄了']'
			reader = fgetc(filebuff); // '['
		}
	}
	else { // 添加白子
		while (reader == '[') {
			tmpx = toupper(fgetc(filebuff)) - 'A';
			tmpy = toupper(fgetc(filebuff)) - 'A';
			if ((tmpx >= 0) && (tmpx < siz) && (tmpy >= 0) && (tmpy < siz)) {
				tmpnode->addwhites.push_back(tmpx * 100 + tmpy);
			}
			reader = fgetc(filebuff); // 棄了']'
			reader = fgetc(filebuff); // '['
		}
	}
} // finished dealAddStones

int DevLancifolium::dealMove(struct GnNode *tmpnode, int colour) {
	int tmpx, tmpy;
	reader = toupper(fgetc(filebuff));
	while (!isalpha(reader) && reader != EOF) reader = fgetc(filebuff);
	tmpx = reader - 'A';
	reader = toupper(fgetc(filebuff));
	tmpy = reader - 'A';
	if (reader == EOF) return 0;
	tmpnode->mov = tmpx * 100 + tmpy;
	tmpnode->stoneProp = colour;

	reader = fgetc(filebuff); // ']'
	reader = fgetc(filebuff); // 棄了']'
	return 1;
}

int DevLancifolium::dealCommentNodename(struct GnNode *tmpnode, int tmpkind) {
	char tmpsave, buff[9999999];
	int tmpi = 0;
	reader = fgetc(filebuff); // 棄了'['
	tmpsave = '\0';
	while ((reader != ']') || (tmpsave == '\\')) {
		if (tmpsave == '\\') { // 此處要改
			if (reader == 'n') buff[tmpi - 1] = '\n';
			else if (reader == 't') buff[tmpi - 1] = '\t';
			else if ((reader == '[') || (reader == ']')
				|| (reader == '\\')) buff[tmpi - 1] = reader;
			else buff[tmpi++] = reader;
			tmpsave = reader;
		}
		else buff[tmpi++] = tmpsave = reader;
		reader = fgetc(filebuff); // 下一個
	}
	buff[tmpi] = '\0';
	if (tmpkind == 1) tmpnode->comment = buff; // 評論
	else tmpnode->nodename = buff; // 節點名稱
	reader = fgetc(filebuff); // 棄了']'
} // finished dealCommentNodename

int DevLancifolium::dealLabels(struct GnNode *tmpnode, int form) {
	int tmpform;
	int tmplab;
	while (reader == '[') {
		tmplab = toupper(fgetc(filebuff)) - 'A';
		tmplab *= 100;
		tmplab += toupper(fgetc(filebuff)) - 'A';
		reader = fgetc(filebuff); // ']'
		reader = fgetc(filebuff); // '[' or not
		tmpnode->labels.push_back(tmplab);
	}

	switch (form) {
	case 0: // 字母標籤
		for (int tmpi = 0; tmpi < tmpnode->labels.size(); tmpi++) {
			tmpnode->labels[tmpi] += ('A' + tmpi) * 10000;
			printf("----alphabet labels: %d \n", tmpnode->labels[tmpi]); /*----------------*/
		}
		break;
	case TRIANGLE: tmpform = TRIANGLE;
	case DIAMOND: tmpform = DIAMOND;
	case FORK: tmpform = FORK;
	case CIRCLE: tmpform = CIRCLE;
		for (int tmpi = 0; tmpi < tmpnode->labels.size(); tmpi++)
			tmpnode->labels[tmpi] += tmpform * 10000;
		break;
	default: break;
	}

	// std::sort(tmpnode->labels.begin(), tmpnode->labels.end(), sortcmp); /* 升序排序 */
	return 1;
}


int DevLancifolium::configNode() { // 處理一個非根節點，curNode指之
	char operate[10]; int tmpi;
	reader = fgetc(filebuff); // 棄掉';' 始以';'，終以';', '(', ')'

	while ((reader != ';') && (reader != '(') && (reader != ')')) { // 內容
		while (iswhite(reader)) reader = fgetc(filebuff); // 空白
		tmpi = 0; // 獲取標誌
		while (isalpha(reader)) { // 當是字母的時候
			operate[tmpi++] = toupper(reader); reader = fgetc(filebuff);
		}
		operate[tmpi] = '\0'; // LB, C, N, AB, AW, SZ,
		while (reader != '[' && reader != EOF) reader = fgetc(filebuff); // 找到'['
		if (reader == EOF) return 0; // EOF
		printf("[%s]", operate); //////////////////////

		if (strcmp(operate, "LB") == 0) dealLabels(curNode, 0); // 字母 0
		else if (strcmp(operate, "TR") == 0) dealLabels(curNode, TRIANGLE); // 三角 1
		else if (strcmp(operate, "SQ") == 0) dealLabels(curNode, DIAMOND); // 方塊 2
		else if (strcmp(operate, "MA") == 0) dealLabels(curNode, FORK); // 叉 3
		else if (strcmp(operate, "CR") == 0) dealLabels(curNode, CIRCLE); // 圓 4
		else if (strcmp(operate, "C") == 0) dealCommentNodename(curNode, 1); /* comment */
		else if (strcmp(operate, "N") == 0) dealCommentNodename(curNode, 2); /* nodename */
		else if (strcmp(operate, "AB") == 0) dealAddStones(curNode, BLACKSTONE);
		else if (strcmp(operate, "AW") == 0) dealAddStones(curNode, WHITESTONE);
		else if (strcmp(operate, "SZ") == 0) dealSize();
		else if (strcmp(operate, "B") == 0) dealMove(curNode, BLACKSTONE); // 黑走子
		else if (strcmp(operate, "W") == 0) dealMove(curNode, WHITESTONE); // 白走子
		else { // 忽略的項
			while (reader != ']' && reader != EOF) reader = fgetc(filebuff);
			reader = fgetc(filebuff); // 棄了']'
		} // finished if-else

		while (iswhite(reader)) reader = fgetc(filebuff); // 去掉空白，必不可少
	} // finished while ';' '('
	std::sort(curNode->addblacks.begin(), curNode->addblacks.end(), sortcmp);
	std::sort(curNode->addwhites.begin(), curNode->addwhites.end(), sortcmp);
	std::sort(curNode->labels.begin(), curNode->labels.end(), sortcmp);
} // finished configNode

int DevLancifolium::configManual(char *filename) {
	if (!openfile(filename)) return 0; // 文件讀取失敗
	if (root != NULL) return 0; // 棋譜樹不爲空
	init(); // 初始化
	root = new struct GnNode;
	root->init(NULL);
	while (!branchStack.empty()) branchStack.pop(); // 清空棧

	reader = fgetc(filebuff);
	while (reader != '(' && reader != EOF) reader = fgetc(filebuff); // 找到第一個左括號
	while (reader != ';' && reader != EOF) reader = fgetc(filebuff); // 找到分號
	curNode = root; /**/
	configNode();

	while (true) { // 處理棋譜
		if (reader == ';') { // ';'
			tmpNode = curNode;
			curNode = new struct GnNode;
			tmpNode->insertNextNode(curNode);
			configNode(); /**/
		}
		else if (reader == '(') { // '('
			tmpNode = curNode;
			curNode = new struct GnNode;
			tmpNode->insertNextNode(curNode); /* 挿入分支 */
			branchStack.push(tmpNode); /* 上一分支節點入棧 */
			reader = fgetc(filebuff);
			while ((reader != ';') && (reader != '(') && (reader != ')')
				&& (reader != EOF)) reader = fgetc(filebuff);
			if (reader == ';') configNode(); /* 當是節點的時候直接設置 */
		}
		else if (reader == ')') { // ')'
			if (branchStack.empty()) { // 根節點的')'，讀譜結束
                printf("\nFinished Reading Manual. \n");
				break;
			}
			else {
				curNode = branchStack.top();
				branchStack.pop();
				reader = fgetc(filebuff); // 棄掉')'
				while ((reader != ';') && (reader != '(') && (reader != ')')
					&& (reader != EOF)) reader = fgetc(filebuff);
			}
		} // finished if-else ';', '(', ')'.
		if (reader == EOF) break; // EOF
	} // finished while true
} // finished configManual


int DevLancifolium::adjustnxtlist(vector<GnNode *> &nxt) {
	vector<GnNode *>::iterator itoi, itoj;
	for (itoi = nxt.begin(); itoi != nxt.end(); itoi++) {
		for (itoj = itoi + 1; itoj != nxt.end();) {
			if (*itoi == *itoj) nxt.erase(itoj); /* 指針地址相同 */
			else if (**itoi == **itoj) { /* 兩個節點配置相同 */
				printf("mov(%d, %d)\n", (*itoi)->mov, (*itoj)->mov); /* ------------- */
				printf("nxtlist(%d, %d)\n", (*itoi)->nxt.size(), (*itoj)->nxt.size()); /* ------------- */
				(*itoi)->joinsame(**itoj); /* 將itoj複製到itoi */
				/* 之前這裡出了問題，joinsame裡面是兩個*，不能是一個星 */
				nxt.erase(itoj); /* 理解，這裡不要++ */
			}
			else itoj++; /* 理解 */
		}
	}
}

int DevLancifolium::adjustManual(GnNode *tmpnode) {
    if (tmpnode == NULL) return 0;
	adjustnxtlist(tmpnode->nxt); /* 調整當前節點的子節點列表 */
	for (vector<GnNode *>::iterator tmpi = tmpnode->nxt.begin();
			tmpi != tmpnode->nxt.end(); tmpi++) {
		adjustManual(*tmpi);
	}
	return 1;
}


int DevLancifolium::joinManuals(DevLancifolium &sig) { /* 將另一個棋譜歸併進來 */
	if (sig.siz > this->siz) this->siz = sig.siz; /* 棋盤取大 */
	for (int tmpi = 0; tmpi < sig.root->nxt.size(); tmpi++) { /* 更改sig分支的父節點 */
		sig.root->nxt[tmpi]->parent = this->root;
	}
	this->root->nxt.insert(this->root->nxt.begin(), sig.root->nxt.begin(), sig.root->nxt.end());
	delete sig.root; sig.root = NULL;
	adjustManual(this->root); /* 調整棋譜即可 */
}

int DevLancifolium::reverse(int deep, struct GnNode *cur) {
	if (cur == NULL) return 0;
	cur->printbase();

	if (cur->nxt.size() > 1) {
		deep++;
		for (int tmpi = 0; tmpi < cur->nxt.size(); tmpi++) {
			printf("\n");
			for (int tmpj = 0; tmpj < deep; tmpj++) printf(" ");

			printf("#|%d|", deep);
			reverse(deep, cur->nxt[tmpi]);
		}
	}
	else if (cur->nxt.size() == 1) {
		//printf(", |%d|", deep);
		reverse(deep, cur->nxt[0]);
	}
	return 0;
}

int DevLancifolium::printfManual() {
	int deep = 0;
	printf("-----------------------------------\n");
	reverse(0, root);
	printf("\n");
}
