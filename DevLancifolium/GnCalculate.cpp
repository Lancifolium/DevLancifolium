#include "GnCalculate.h"

void GnCalculate::init() {
	siz = 19;
	conflict = confmove = 0;
	memset(ston, 0, sizeof(char) * 676);
	memset(tmpbord, 0, sizeof(char) * 676);
	while (!this->bordList.empty()) this->bordList.pop_back();
	while (!this->moveList.empty()) this->moveList.pop_back();
}

int GnCalculate::findStoneBlock(int colour, int tmpcolour, int pos, int tmpsiz, int &stonnum) { // 找一片棋，返回氣數
	;// colour是棋盤上子之顏色；tmpcolour是tmpbord上該片棋之顏色
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
		if ((tmpx > 0) && (tmpbord[tmpx - 1][tmpy] != tmpcolour)) {
			if (ston[tmpx - 1][tmpy] == colour) { // 同色
				movStack.push(tmpmov - 100); // 入棧
				tmpbord[tmpx - 1][tmpy] = tmpcolour; // 標記
				stonnum++;
			}
			else if (ston[tmpx - 1][tmpy] == 0) breath++; // 無子，異色不用考慮
		}
		if ((tmpx < tmpsiz) && (tmpbord[tmpx + 1][tmpy] != tmpcolour)) {
			if (ston[tmpx + 1][tmpy] == colour) { // 同色
				movStack.push(tmpmov + 100); // 入棧
				tmpbord[tmpx + 1][tmpy] = tmpcolour; // 標記
				stonnum++;
			}
			else if (ston[tmpx + 1][tmpy] == 0) breath++; // 無子，異色不用考慮
		}
		if ((tmpy > 0) && (tmpbord[tmpx][tmpy - 1] != tmpcolour)) {
			if (ston[tmpx][tmpy - 1] == colour) { // 同色
				movStack.push(tmpmov - 1); // 入棧
				tmpbord[tmpx][tmpy - 1] = tmpcolour; // 標記
				stonnum++;
			}
			else if (ston[tmpx][tmpy - 1] == 0) breath++; // 無子，異色不用考慮
		}
		if ((tmpy < tmpsiz) && (tmpbord[tmpx][tmpy + 1] != tmpcolour)) {
			if (ston[tmpx][tmpy + 1] == colour) { // 同色
				movStack.push(tmpmov + 1); // 入棧
				tmpbord[tmpx][tmpy + 1] = tmpcolour; // 標記
				stonnum++;
			}
			else if (ston[tmpx][tmpy + 1] == 0) breath++; // 無子，異色不用考慮
		}
	} // 結束while循環
	return breath; // 將氣返回
} // finished findStoneBlock

int GnCalculate::configDropStone(int colour, int mov) { // 著子判斷，0不可落子，1有提子，2無提子
	;// tmpbord標記原則：3同色，4異色，5提子，6異色但非提子
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
	memset(tmpbord, 0, sizeof(char) * 676); // 臨時棋盤初始化
	tmpx = mov / 100; tmpy = mov % 100; // 落子坐標
	tmpbord[tmpx][tmpy] = TB_SAMECOLOUR; // 置子
	ston[tmpx][tmpy] = colour; // 落子

	;// 必須先行判斷異色子有否可被提者
	lift = 0; // 提子數目
	if ((tmpx > 0) && (ston[tmpx - 1][tmpy] == opcolour)) { // 上
		tmpbord[tmpx - 1][tmpy] = TB_DIFFCOLOUR;
		if (findStoneBlock(opcolour, TB_DIFFCOLOUR, mov - 100, tmpsiz, tmpi) == 0) { // 無氣，需要提子
			lift += tmpi;
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (tmpbord[tmpi][tmpj] == TB_DIFFCOLOUR) tmpbord[tmpi][tmpj] = TB_LIFTSTONES;
				}
			}
		}
		else { // 有氣，無需提子
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (tmpbord[tmpi][tmpj] == TB_DIFFCOLOUR) tmpbord[tmpi][tmpj] = TB_DIFFNOLIFT;
				}
			}
		}
	}
	if ((tmpx < tmpsiz) && (ston[tmpx + 1][tmpy] == opcolour) &&
		(tmpbord[tmpx + 1][tmpy] != TB_LIFTSTONES) && (tmpbord[tmpx + 1][tmpy] != TB_DIFFNOLIFT)) { // 下
		tmpbord[tmpx + 1][tmpy] = TB_DIFFCOLOUR;
		if (findStoneBlock(opcolour, TB_DIFFCOLOUR, mov + 100, tmpsiz, tmpi) == 0) { // 無氣，需要提子
			lift += tmpi;
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (tmpbord[tmpi][tmpj] == TB_DIFFCOLOUR) tmpbord[tmpi][tmpj] = TB_LIFTSTONES;
				}
			}
		}
		else { // 有氣，無需提子
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (tmpbord[tmpi][tmpj] == TB_DIFFCOLOUR) tmpbord[tmpi][tmpj] = TB_DIFFNOLIFT;
				}
			}
		}
	}
	if ((tmpy > 0) && (ston[tmpx][tmpy - 1] == opcolour) &&
		(tmpbord[tmpx][tmpy - 1] != TB_LIFTSTONES) && (tmpbord[tmpx][tmpy - 1] != TB_DIFFNOLIFT)) { // 左
		tmpbord[tmpx][tmpy - 1] = TB_DIFFCOLOUR;
		if (findStoneBlock(opcolour, TB_DIFFCOLOUR, mov - 1, tmpsiz, tmpi) == 0) { // 無氣，需要提子
			lift += tmpi;
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (tmpbord[tmpi][tmpj] == TB_DIFFCOLOUR) tmpbord[tmpi][tmpj] = TB_LIFTSTONES;
				}
			}
		}
		else { // 有氣，無需提子
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (tmpbord[tmpi][tmpj] == TB_DIFFCOLOUR) tmpbord[tmpi][tmpj] = TB_DIFFNOLIFT;
				}
			}
		}
	}
	if ((tmpy < tmpsiz) && (ston[tmpx][tmpy + 1] == opcolour) &&
		(tmpbord[tmpx][tmpy + 1] != TB_LIFTSTONES) && (tmpbord[tmpx][tmpy + 1] != TB_DIFFNOLIFT)) { // 右
		tmpbord[tmpx][tmpy + 1] = TB_DIFFCOLOUR;
		if (findStoneBlock(opcolour, TB_DIFFCOLOUR, mov + 1, tmpsiz, tmpi) == 0) { // 無氣，需要提子
			lift += tmpi;
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (tmpbord[tmpi][tmpj] == TB_DIFFCOLOUR) tmpbord[tmpi][tmpj] = TB_LIFTSTONES;
				}
			}
		}
		else { // 有氣，無需提子
			for (tmpi = 0; tmpi <= tmpsiz; tmpi++) {
				for (tmpj = 0; tmpj <= tmpsiz; tmpj++) {
					if (tmpbord[tmpi][tmpj] == TB_DIFFCOLOUR) tmpbord[tmpi][tmpj] = TB_DIFFNOLIFT;
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
		saveCurrBord(colour, mov); // 保存棋盤
		if (tmpx > 0 && tmpbord[tmpx - 1][tmpy] == TB_LIFTSTONES) {
			ston[tmpx - 1][tmpy] = 0;
			confmove = (tmpx - 1) * 100 + tmpy;
		}
		if (tmpx < tmpsiz && tmpbord[tmpx + 1][tmpy] == TB_LIFTSTONES) {
			ston[tmpx + 1][tmpy] = 0;
			confmove = (tmpx + 1) * 100 + tmpy;
		}
		if (tmpy > 0 && tmpbord[tmpx][tmpy - 1] == TB_LIFTSTONES) {
			ston[tmpx][tmpy - 1] = 0;
			confmove = tmpx * 100 + tmpy - 1;
		}
		if (tmpy < tmpsiz && tmpbord[tmpx][tmpy + 1] == TB_LIFTSTONES) {
			ston[tmpx][tmpy + 1] = 0;
			confmove = tmpx * 100 + tmpy + 1;
		}
		conflict = 1;
		printf("New conflict: %d, %d\n", conflict, confmove);  ///////////////
		return 1;
	}
	if (lift > 0) { // 提子
		saveCurrBord(colour, mov); // 保存棋盤
		conflict = 0; // 撤銷劫爭
		for (tmpi = 0; tmpi < siz; tmpi++) { // 提子操作
			for (tmpj = 0; tmpj < siz; tmpj++) {
				if (tmpbord[tmpi][tmpj] == TB_LIFTSTONES) ston[tmpi][tmpj] = 0;
			}
		}
		return 2; // 需要提子
	}
	conflict = 0; // 撤銷劫爭
    moveList.push_back(mov);
	return 3; // 無需提子
} // finished configDropStone

int GnCalculate::saveCurrBord(int colour, int mov) {
	// 務必注意是後一手給前一手保存棋盤 ## 暫且做成這樣，或者給上一個保存棋盤也可以
	moveList.push_back(mov + 10000);
	ston[mov / 100][mov % 100] = 0;
	currsave.init();
	int tmpi, tmpj; long long tmpa;
	tmpj = 0; tmpa = 1;
	for (; tmpj < siz; tmpj++) { // 逐列進行
		for (tmpi = 0; tmpi < siz; tmpi++) { // 行標
			currsave.ston[tmpi] = currsave.ston[tmpi] * tmpa + ston[tmpi][tmpj];
		}
		tmpa *= 3;
	}
	ston[mov / 100][mov % 100] = colour;
	return 0;
}

int GnCalculate::regainMove() { // 根據棧頂恢復
	if (moveList.empty()) {
		printf("Reach top! \n");
		return 1;
	}
	else if (moveList.back() >= 10000) {
		if (bordList.empty()) {
			printf("No bord save! \n");
			int tmp = moveList.back() % 10000;
			ston[tmp / 100][tmp % 100] = 0;
			moveList.pop_back();
			return 2;
		}
		int tmpi, tmpj;
		long long tmpa, tmpb;
		tmpi = 0; tmpa = 1, tmpb = 3;
		for (tmpj = siz - 1; tmpj >= 0; tmpj--) { // 恢復棋盤
			for (tmpi = 0; tmpi < siz; tmpi++) {
				ston[tmpi][tmpj] = (bordList.back().ston[tmpi] % tmpb) / tmpa;
			}
			tmpa *= 3;
			tmpb *= 3;
		}
		moveList.pop_back();
		bordList.pop_back();
	}
	else {
		if (moveList.back() < 0) {
			printf("Invalid move! \n");
			return 3;
		}
		ston[moveList.back() / 100][moveList.back() % 100] = 0;
		moveList.pop_back();
	}
	return 0;
}
