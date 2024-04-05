
#ifndef Fonctions_H
#define Fonctions_H



typedef struct maillon{
char forme;
char couleur;
struct maillon *ColorNext;
struct maillon *ColorPrev;
struct maillon *FormNext;
struct maillon *FormPrev;
struct maillon *ElemNext;
struct maillon *ElemPrec;
}Maillon;


typedef struct liste{
int taille;
Maillon *premier;
Maillon *dernier;  // dans le sujet on garde un pointeur sur le premier et le dernier
}Liste;



//Prototype de fonctions

Maillon *creer_maillon(char color, char shape);

void libereElem(Maillon *element);

Liste *liste_init(void);

void liste_resetColor(Liste *l);

void liste_resetShape(Liste *l);

void AddDebutColor(Maillon *element, Liste *Liste);

void AddFinColor(Maillon *element, Liste *Liste);

void AddDebutShape(Maillon *element, Liste *Liste);

void AddFinShape(Maillon *element, Liste *Liste);

void addListeSpec(Maillon *element, Liste **Tabliste,char a);

void list_ajout_debut(Liste *l,Maillon *m,Liste **Tabliste);

void list_insere_fin(Liste *l,Maillon *m,Liste *Tabliste[8]);

int AsssignForm(char cara);

int AsssignForm(char cara);

void SuppColor(Maillon *element, Liste* liste);

void SUppForm(Maillon *element, Liste *liste);

void Supp(Maillon *element, Liste *tabListe[8],Liste *jeu);

void Decalage(Liste *jeu,Liste *tabliste[8], char direction, char cara);

Maillon *GenerateurPiece(void);

int nbligne(char* nomfichier);

int chargersauvegarde(Liste *jeu, Maillon **piecesuiv, Liste **TabListe,int *combo);

void overwritesave(Liste *jeu, int score, int combo,Maillon* piecesuiv[5]);

void HighScore(int score, char* pseudo);

int detectionadd(Liste *jeu,char direction, Liste *TabListe[8]);

int detectionDecalage(Liste *jeu, Liste *tabListe[8]);

int ScoreDecalage(int score, int*combo,int indicecoup);

void affichepiece(Maillon *piece);

void setupterminal(void);

void afficheplateau(Liste*jeu);

void afficheListeColor(Liste* liste);

void afficheListeShape(Liste* liste);

void AfficheJeu(int score, Liste*jeu,int combo,Maillon *Piecesuiv[5]);

void AfficheSousListe(Liste *Tabliste[8]);


#endif