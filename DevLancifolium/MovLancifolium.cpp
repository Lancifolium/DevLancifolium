#include "MovLancifolium.h"

void MovLancifolium::init() {
	siz = 19;
	conflict = confmove = 0;
	memset(ston, 0, sizeof(char) * 676);
	memset(calcbord, 0, sizeof(char) * 676);
    //root = NULL;
    root = new GnNode();
	curNode = NULL;
	conflict = 0;
}

void MovLancifolium::deleteroot(struct GnNode *tmproot) {
	if (tmproot != NULL) {
		for (int tmpi = 0; tmpi < tmproot->nxt.size(); tmpi++) {
			deleteroot(tmproot->nxt[tmpi]);
		}
		delete tmproot;
	}
}

void MovLancifolium::clearall() {
	deleteroot(root);
	this->init();
}

MovLancifolium::MovLancifolium() {
	this->init();
}

int MovLancifolium::findStoneBlock(int colour, int tmpcolour, int pos, int tmpsiz, int &stonnum) { // 找一片棋，返回氣數
	;// colour是棋盤上子之顏色；tmpcolour是calcbord上該片棋之顏色
	;// siz是棋盤規格減一
	int tmpmov, breath;
	int tmpx, tmpy;
	std::stack<int> movStack; // 棧
	movStack.push(pos); // 當前節點入棧
	breath = 0; // 氣
	stonnum = 1;
	while (!movStack.empty()) { // 棧不空
		tmpmov = movStack.top(); movStack.pop(); // 取出棧頂
		tmpx = tmpmov / 100; tmpy = tmpmov % 100; // 坐標
		if ((tmpx > 0) && (calcbord[tmpx - 1][tmpy] != tmpcolour)) {
			if (ston[tmpx - 1][tmpy] == colour) { // 同色
				movStack.push(tmpmov - 100); // 入棧
				calcbord[tmpx - 1][tmpy] = tmpcolour; // 標記
				stonnum++;
			}
			else if (ston[tmpx - 1][tmpy] == 0) breath++; // 無子，異色不用考慮
		}
		if ((tmpx < tmpsiz) && (calcbord[tmpx + 1][tmpy] != tmpcolour)) {
			if (ston[tmpx + 1][tmpy] == colour) { // 同色
				movStack.push(tmpmov + 100); // 入棧
				calcbord[tmpx + 1][tmpy] = tmpcolour; // 標記
				stonnum++;
			}
			else if (ston[tmpx + 1][tmpy] == 0) breath++; // 無子，異色不用考慮
		}
		if ((tmpy > 0) && (calcbord[tmpx][tmpy - 1] != tmpcolour)) {
			if (ston[tmpx][tmpy - 1] == colour) { // 同色
				movStack.push(tmpmov - 1); // 入棧
				calcbord[tmpx][tmpy - 1] = tmpcolour; // 標記
				stonnum++;
			}
			else if (ston[tmpx][tmpy - 1] == 0) breath++; // 無子，異色不用考慮
		}
		if ((tmpy < tmpsiz) && (calcbord[tmpx][tmpy + 1] != tmpcolour)) {
			if (ston[tmpx][tmpy + 1] == colour) { // 同色
				movStack.push(tmpmov + 1); // 入棧
				calcbord[tmpx][tmpy + 1] = tmpcolour; // 標記
				stonnum++;
			}
			else if (ston[tmpx][tmpy + 1] == 0) breath++; // 無子，異色不用考慮
		}
	} // 結束while循環
	return breath; // 將氣返回
} // finished findStoneBlock

int MovLancifolium::configDropStone(int colour, int mov) { // 著子判斷，0不可落子，1有提子，2無提子
	;// calcbord標記原則：3同色，4異色，5提子，6異色但非提子
	;// tmpx處千百位，tmpy處個十位
	;// 返回：0禁著點（包括劫爭熱點），1提子並形成劫爭，2提子，3無需提子

	if (mov < 0) return 0; // 非法落子
	if (ston[mov / 100][mov % 100]) return 0; // 不能落子
	if (conflict && (mov == confmove)) return 0; // 劫爭的禁著點

	int opcolour; // 對手顏色
	int tmpx, tmpy;
	int tmpi, tmpj;
	int tmpsiz, lift, bnum; // 臨時棋盤大小、提子數目、本塊棋的大小
	tmpsiz = siz - 1; // 棋盤規則減一
	opcolour = (colour == 1) ? 2 : 1; // 異色棋子顏色
	memset(calcbord, 0, sizeof(char) * 676); // 臨時棋盤初始化
	tmpx = mov / 100; tmpy = mov % 100; // 落子坐標
	calcbord[tmpx][tmpy] = TB_SAMECOLOUR; // 置子
	ston[tmpx][tmpy] = colour; // 落子

	;// 必須先行判斷異色子有否可被提者
	lift = 0; // 提子數目
	if ((tmpx > 0) && (ston[tmpx - 1][tmpy] == opcolour)) { // 上
		calcbord[tmpx - 1][tmpy] = TB_DIFFCOLOUR;
		if (findStoneBlock(opcolour, TB_DIFFCOLOUR, mov - 100, tmpsiz, tmpi) == 0) { // 無氣，需要提子
			lift += tmpi;
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (calcbord[tmpi][tmpj] == TB_DIFFCOLOUR) calcbord[tmpi][tmpj] = TB_LIFTSTONES;
				}
			}
		}
		else { // 有氣，無需提子
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (calcbord[tmpi][tmpj] == TB_DIFFCOLOUR) calcbord[tmpi][tmpj] = TB_DIFFNOLIFT;
				}
			}
		}
	}
	if ((tmpx < tmpsiz) && (ston[tmpx + 1][tmpy] == opcolour) &&
		(calcbord[tmpx + 1][tmpy] != TB_LIFTSTONES) && (calcbord[tmpx + 1][tmpy] != TB_DIFFNOLIFT)) { // 下
		calcbord[tmpx + 1][tmpy] = TB_DIFFCOLOUR;
		if (findStoneBlock(opcolour, TB_DIFFCOLOUR, mov + 100, tmpsiz, tmpi) == 0) { // 無氣，需要提子
			lift += tmpi;
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (calcbord[tmpi][tmpj] == TB_DIFFCOLOUR) calcbord[tmpi][tmpj] = TB_LIFTSTONES;
				}
			}
		}
		else { // 有氣，無需提子
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (calcbord[tmpi][tmpj] == TB_DIFFCOLOUR) calcbord[tmpi][tmpj] = TB_DIFFNOLIFT;
				}
			}
		}
	}
	if ((tmpy > 0) && (ston[tmpx][tmpy - 1] == opcolour) &&
		(calcbord[tmpx][tmpy - 1] != TB_LIFTSTONES) && (calcbord[tmpx][tmpy - 1] != TB_DIFFNOLIFT)) { // 左
		calcbord[tmpx][tmpy - 1] = TB_DIFFCOLOUR;
		if (findStoneBlock(opcolour, TB_DIFFCOLOUR, mov - 1, tmpsiz, tmpi) == 0) { // 無氣，需要提子
			lift += tmpi;
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (calcbord[tmpi][tmpj] == TB_DIFFCOLOUR) calcbord[tmpi][tmpj] = TB_LIFTSTONES;
				}
			}
		}
		else { // 有氣，無需提子
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (calcbord[tmpi][tmpj] == TB_DIFFCOLOUR) calcbord[tmpi][tmpj] = TB_DIFFNOLIFT;
				}
			}
		}
	}
	if ((tmpy < tmpsiz) && (ston[tmpx][tmpy + 1] == opcolour) &&
		(calcbord[tmpx][tmpy + 1] != TB_LIFTSTONES) && (calcbord[tmpx][tmpy + 1] != TB_DIFFNOLIFT)) { // 右
		calcbord[tmpx][tmpy + 1] = TB_DIFFCOLOUR;
		if (findStoneBlock(opcolour, TB_DIFFCOLOUR, mov + 1, tmpsiz, tmpi) == 0) { // 無氣，需要提子
			lift += tmpi;
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (calcbord[tmpi][tmpj] == TB_DIFFCOLOUR) calcbord[tmpi][tmpj] = TB_LIFTSTONES;
				}
			}
		}
		else { // 有氣，無需提子
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (calcbord[tmpi][tmpj] == TB_DIFFCOLOUR) calcbord[tmpi][tmpj] = TB_DIFFNOLIFT;
				}
			}
		}
	}
	// 異色提子判斷完畢

	if ((findStoneBlock(colour, TB_SAMECOLOUR, mov, tmpsiz, bnum) == 0) && (lift == 0)) { // 本色棋子無氣且無提子
		ston[tmpx][tmpy] = 0; // 撤銷落子
		printf("No breath No kill: (%d, %d) %d", tmpx, tmpy, lift);
		system("pause");
		return 0; // 不能落子
	}
	if (lift == 1 && bnum == 1) { // 提子並形成劫爭
		savetree(colour, mov); // 新建節點
        curNode->liftcolour = (colour == 1 ? 2 : 1); // 當前死子顏色
		if (tmpx > 0 && calcbord[tmpx - 1][tmpy] == TB_LIFTSTONES) {
			ston[tmpx - 1][tmpy] = 0;
			confmove = (tmpx - 1) * 100 + tmpy;
			curNode->liftsave.push_back(confmove); // 保存死子
		}
		if (tmpx < tmpsiz && calcbord[tmpx + 1][tmpy] == TB_LIFTSTONES) {
			ston[tmpx + 1][tmpy] = 0;
			confmove = (tmpx + 1) * 100 + tmpy;
			curNode->liftsave.push_back(confmove); // 保存死子
		}
		if (tmpy > 0 && calcbord[tmpx][tmpy - 1] == TB_LIFTSTONES) {
			ston[tmpx][tmpy - 1] = 0;
			confmove = tmpx * 100 + tmpy - 1;
			curNode->liftsave.push_back(confmove); // 保存死子
		}
		if (tmpy < tmpsiz && calcbord[tmpx][tmpy + 1] == TB_LIFTSTONES) {
			ston[tmpx][tmpy + 1] = 0;
			confmove = tmpx * 100 + tmpy + 1;
			curNode->liftsave.push_back(confmove); // 保存死子
		}
		conflict = 1;
		printf("New conflict: %d, %d\n", conflict, confmove);  ///////////////
		return 1;
	}
	if (lift > 0) { // 提子
		savetree(colour, mov); // 新建節點
		curNode->liftcolour = (colour == 1 ? 2 : 1); // 當前死子顏色
		conflict = 0; // 撤銷劫爭
		for (tmpi = 0; tmpi < siz; tmpi++) { // 提子操作
			for (tmpj = 0; tmpj < siz; tmpj++) {
				if (calcbord[tmpi][tmpj] == TB_LIFTSTONES) { // 提子
					ston[tmpi][tmpj] = 0;
					curNode->liftsave.push_back(tmpi * 100 + tmpj); // 保存死子
				}
			}
		}
		return 2; // 需要提子
	}
    // default case 程序這裡存在構造缺陷，使得發現不了bug
	conflict = 0; // 撤銷劫爭
    savetree(colour, mov); //
	return 3; // 無需提子
} // finished configDropStone

int MovLancifolium::savetree(int colour, int mov) { // 
    if (curNode == NULL) curNode = root; // 防止curNode懸垂
        struct GnNode *tmpnode = curNode; // 暫存當前節點
    if (curNode->nxt.size() > 0) { // 有次分支
        int tmpi;
        for (tmpi = 0; tmpi < tmpnode->nxt.size(); tmpi++) { // 查找看看有沒有坐標相同的節點
            if (tmpnode->nxt[tmpi]->mov == mov) break; // 找到
        }
        if (tmpi == tmpnode->nxt.size()) { // 沒找到
            curNode = new GnNode(tmpnode);
            curNode->mov = mov;
            tmpnode->nxt.push_back(curNode);
			tmpnode->nxt[tmpi] = tmpnode->nxt[0];
			tmpnode->nxt[0] = curNode;
        }
        else { // 找到了
            curNode = tmpnode->nxt[tmpi];
            tmpnode->nxt[tmpi] = tmpnode->nxt[0]; // 交換主分支
            tmpnode->nxt[0] = curNode;
            curNode->liftsave.clear();
        }
        return 1;
    }
	else {
		curNode = new GnNode(tmpnode);
		curNode->mov = mov;
		tmpnode->nxt.push_back(curNode);
	}
}

int MovLancifolium::regainMove() { // 恢復提子
    printf("-----------\n");
    if (curNode == root) { // 到達根
        printf("Reaching top. \n");
        return 1;
	}
	else {
		if (curNode->addblacks.size() > 0) { // 撤銷置子
			for (int tmpi = 0; tmpi < curNode->addblacks.size(); tmpi++) {
				ston[curNode->addblacks[tmpi] / 100][curNode->addblacks[tmpi] % 100] = 0;
			}
		}
		if (curNode->addwhites.size() > 0) { // 撤銷置子
			for (int tmpi = 0; tmpi < curNode->addwhites.size(); tmpi++) {
				ston[curNode->addwhites[tmpi] / 100][curNode->addwhites[tmpi] % 100] = 0;
			}
		}
		if (curNode->liftsave.size() > 0) { // 回填提子
			for (int tmpi = 0; tmpi < curNode->liftsave.size(); tmpi++) {
				ston[curNode->liftsave[tmpi] / 100][curNode->liftsave[tmpi] % 100] = curNode->liftcolour;
			}
		}
		ston[curNode->mov / 100][curNode->mov % 100] = 0; // 撤銷落子
        printf(" @ mov: %d $ [%p] ", curNode->mov, curNode);
		curNode = curNode->parent; // 回溯一個節點
        printf("$ [%p]\n", curNode);
        return 0;
	}
}
