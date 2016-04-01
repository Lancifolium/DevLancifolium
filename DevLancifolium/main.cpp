#include "DevLancifolium.h"

#include "DevLancifolium.cpp"
#include "GnCalculate.cpp"
#include "GnNode.cpp"

int main(void) {
	struct DevLancifolium sig;
	//sig.openfile("tmp.sgf"); // file
	sig.configManual("tmp.sgf");
	sig.printfManual();
}