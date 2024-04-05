
#ifndef interpreteur_H
#define interpreteur_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

extern char memo[65536];
extern char etat[3];
extern short reg1[32];
int verifierMemoire(int val);
int verifierEntier(float val);
int verifieLimite(int val);
void bitZ(int val);
void bitN(int val);
int add(int dest, int reg2, int imme, int S);
int sub(int dest, int reg2, int imme, int S);
char *ConvertirBinaire(int val);
char PoidsFaible(int val);
char PoidsFort(int val);
int DecalageDroiteNeg(int val, int S);
void mul(int dest, int reg2, int imme, int S);
int div1(int dest, int reg2, int imme, int S);
void and(int dest, int reg2, int imme, int S);
void or(int dest, int reg2, int imme, int S);
void xor(int dest, int reg2, int imme, int S);
void shl(int dest, int reg2, int imme, int S);
int stb(int dest, int reg2, int imme, int S);
int stw(int dest, int reg2, int imme, int S);
int ldb(int dest, int reg2, int imme, int S);
int ldw(int dest, int reg2, int imme, int S);
int jmp(int imme, int dest);
int jzs(int imme, int dest);
int jzc(int imme, int dest);
int jcs(int imme, int dest);
int jcc(int imme, int dest);
int jns(int imme, int dest);
int jnc(int imme, int dest);
int in(int dest);
void out(int dest);
void rnd(int dest, int rn, int imme, int s);


#endif
