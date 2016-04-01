#ifndef FUNLANCIFOLIUM
#define FUNLANCIFOLIUM

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

struct Funs { // Some functions
    static int iswhite(char tmpc) { // 判斷空白符號
        if (tmpc == ' ' || tmpc == '\n' || tmpc == '\t' || tmpc == 13) return 1;
        return 0;
    }
    static int ManualAdjustment(char *infile, char *oufile) { // 棋譜調整函數
        FILE *fil, *wrl;
        fil = fopen(infile, "r");
        wrl = fopen(oufile, "w");

        int deep, tmpi, mod;
        char tmpc, tmpb;
        deep = 0; tmpb = '0'; mod = 0;
        while (tmpc = fgetc(fil)) {
            if ((tmpc == '(') && (mod == 0)) {
                fprintf(wrl, "\n");
                for (tmpi = 0; tmpi < deep; tmpi++) fprintf(wrl, "  ");
                fprintf(wrl, "(\n");
                deep++;
                for (tmpi = 0; tmpi < deep; tmpi++) fprintf(wrl, "  ");
            }
            else if ((tmpc == ')') && (mod == 0)) {
                fprintf(wrl, "\n");
                deep--;
                for (tmpi = 0; tmpi < deep; tmpi++) fprintf(wrl, "  ");
                fprintf(wrl, ")");
            }
            else if ((tmpc == '\t') || (tmpc == '\n') || (tmpc == ' ')) {
                if (mod == 1) fprintf(wrl, " ");
            }
            else if ((tmpc == '[') && (tmpb != '\\') && (mod == 0)) {
                fprintf(wrl, "["); mod = 1;
            }
            else if ((tmpc == ']') && (tmpb != '\\') && (mod == 1)) {
                fprintf(wrl, "]"); mod = 0;
            }
            else if (tmpc == EOF) break;
            else fprintf(wrl, "%c", tmpc);

            tmpb = tmpc;
            printf("|%c|%d|", tmpc, tmpc); //system("pause");
        }
        return 0;
    }
};

#endif // FUNLANCIFOLIUM

