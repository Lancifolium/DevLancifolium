#include "DevLancifolium.h"

#include "DevLancifolium.cpp"
#include "GnCalculate.cpp"
#include "GnNode.cpp"

int main(void) {
	struct DevLancifolium sig;
	//sig.openfile("tmp.sgf"); // file
	char buff[1000];
	while (~scanf("%s", buff)) {
		sig.configManual(buff);
		sig.printfManual();
		sig.clearall();
	}
}
