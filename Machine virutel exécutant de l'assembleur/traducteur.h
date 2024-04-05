

#ifndef traducteur_H
#define traducteur_H

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#define Taille 100

// on cree des types utiles
// On doit traduire des instruction en char en code hexa decimal
typedef struct{
    char* nom;
    char** arg;
    int taille;
    int immediat;
}instruction;


typedef struct{
    char* etiq;
    int ligne;
} etiquette;



void afficheinstru(instruction* instru);

void afficheEtiquette(etiquette* eti);

void libereinstr(instruction* instru);

void libereetiquette(etiquette* etiq);

etiquette *cree_etiquette( char *etiq, int ligne);

instruction *cree_instr( char *nom, char** arg,int taille,int immediat);

int nbligne(char* nomfichier);

int isetiquette(char* ligne);

etiquette* etiquettechr(char*ligne, int numligne);

int lectureligne(char* ligne, etiquette **Tabetiq,char **  document,int numligne,int cpt);

int lecturefichier(int nb, char* nomfichier,etiquette **Tabetiq,char **  document);

void LigneVersInstruction(char* ligne,instruction** listeinstr, int* codeErreur);

int testEtiq(etiquette **tabetiq, int doublonligne[2]);

int checkregsitre(instruction *instru, int numarg);

int CheckvaleurImmediate(instruction *instru, int indice, int hexa,int ligne);

int CodeOp(instruction *TabInstru );

int recupRegisteDest(instruction* instru,int op,int nb);

int recupSrc1(instruction* instru,int op,int nb);

int RecupSrc2(instruction* instru,int op,etiquette **tabetiq,int nb,int *error);


#endif