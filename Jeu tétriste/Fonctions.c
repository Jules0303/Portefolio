#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Fonctions.h"
#define TAILLE 30

// --------fonctions création des éléments----------------

Maillon *creer_maillon(char color, char shape){
        Maillon *m=malloc(sizeof(Maillon));
        if (m==NULL) {
            printf("erreur de mémoire");
            return NULL;}
        m->ColorNext=malloc(sizeof(Maillon));
        m->ColorPrev=malloc(sizeof(Maillon));
        m->FormNext=malloc(sizeof(Maillon));
        m->FormPrev=malloc(sizeof(Maillon));
        m->ElemNext=malloc(sizeof(Maillon));
        m->ElemPrec=malloc(sizeof(Maillon));
        if((m->ColorNext || m->FormNext || m->ColorPrev || m->FormPrev || m->ElemNext)==0){
            printf("erreur de mémoire");
            return NULL;}
        //On initialise en doublement chainé directement why not après tout
        m->ColorNext=m;
        m->ColorPrev=m;
        m->FormNext=m;
        m->FormPrev=m;
        m->ElemNext=m;
        m->ElemPrec=m;
        m->couleur=color;
        m->forme=shape;
        return m;
}

void libereElem(Maillon *element){
    element->ColorNext=NULL;
    element->ColorPrev=NULL;
    element->FormNext=NULL;
    element->FormPrev=NULL;
    element->ElemNext=NULL;
    element->ElemPrec=NULL;
    free(element->ColorNext);
    free(element->ColorPrev);
    free(element->FormNext);
    free(element->FormPrev);
    free(element->ElemNext);
    free(element->ElemPrec);
    free(element);
    return;
}
// -----------création de listes lié circulaire---------//
Liste *liste_init(void){
    Liste *l=malloc(sizeof(Liste));
    if(l==NULL) {
        printf("erreur de memoires");
        return NULL;}
    l->taille=0;
    l->premier=NULL;
    l->dernier=NULL;
    return l;
}

void liste_resetColor(Liste *l){
        l->taille=0;
        l->dernier=NULL;
        l->premier=NULL;
        
}

void liste_resetShape(Liste *l){
        l->taille=0;
        l->dernier=NULL;
        l->premier=NULL;
}

void AddDebutColor(Maillon *element, Liste *Liste){
        if(Liste->taille==0){
            Liste->premier=element;
            Liste->dernier=element;
            Liste->taille++;
            return;
        }
        element->ColorNext=Liste->premier;
        element->ColorPrev=Liste->dernier;
        Liste->dernier->ColorNext=element;
        Liste->premier->ColorPrev=element;
        Liste->premier=element;
        Liste->taille++;
    return;
}

void AddFinColor(Maillon *element, Liste *Liste){
        if(Liste->taille==0){
            Liste->premier=element;
            Liste->dernier=element;
            Liste->taille++;
            return;
        }
        element->ColorNext=Liste->premier;
        element->ColorPrev=Liste->dernier;
        Liste->dernier->ColorNext=element;
        Liste->premier->ColorPrev=element;
        Liste->dernier=element;
        Liste->taille++;
    return;
}

void AddDebutShape(Maillon *element, Liste *Liste){
        if(Liste->taille==0){
            Liste->premier=element;
            Liste->dernier=element;
            Liste->taille++;
            return;
        }
        element->FormNext=Liste->premier;
        element->FormPrev=Liste->dernier;
        Liste->dernier->FormNext=element;
        Liste->premier->FormPrev=element;
        Liste->premier=element;
        Liste->taille++;
    return;
}

void AddFinShape(Maillon *element, Liste *Liste){
    if(Liste->taille==0){
            Liste->premier=element;
            Liste->dernier=element;
            Liste->taille++;
            return;
        }
        element->FormNext=Liste->premier;
        element->FormPrev=Liste->dernier;
        Liste->premier->FormPrev=element;
        Liste->dernier->FormNext=element;
        Liste->dernier=element;
        Liste->taille++;
    return;
}

//Met à jour nos tab de color et shape avec un nouvel élément à droite ou à gauche
void addListeSpec(Maillon *element, Liste **Tabliste,char a){
//But met notre nouvelle element dans le bon tab forme et couleur à droite ou a gauche
switch ((element->couleur))
    {
    case 'r':
        if(a=='g'){
            AddDebutColor(element,Tabliste[4]);
        }
        if(a=='d'){
            AddFinColor(element,Tabliste[4]);
        }
        break;
    case 'b':
         if(a=='g'){
            AddDebutColor(element,Tabliste[5]);
        }
        if(a=='d'){
            AddFinColor(element,Tabliste[5]);
        }
        break;
    case 'v':
         if(a=='g'){
            AddDebutColor(element,Tabliste[6]);
        }
        if(a=='d'){
            AddFinColor(element,Tabliste[6]);
        }
        break;
    case 'j':
        if(a=='g'){
            AddDebutColor(element,Tabliste[7]);
        }
        if(a=='d'){
            AddFinColor(element,Tabliste[7]);
        }
        break;
    default:
        break;
    }
switch (element->forme)
    {
    case 'T':
        if(a=='g')
            AddDebutShape(element,Tabliste[0]);
        if(a=='d')
            AddFinShape(element,Tabliste[0]);
        break;
    case 'C':
        if(a=='g')
            AddDebutShape(element,Tabliste[1]);
        if(a=='d')
            AddFinShape(element,Tabliste[1]);
        break;
    case 'L':
        if(a=='g')
            AddDebutShape(element,Tabliste[2]);
        if(a=='d')
            AddFinShape(element,Tabliste[2]);
        break;
    case 'R':
        if(a=='g')
            AddDebutShape(element,Tabliste[3]);
        if(a=='d')
            AddFinShape(element,Tabliste[3]);
        break;
    default:
        break;
    }
}



void list_ajout_debut(Liste *l,Maillon *m,Liste **Tabliste){
    //cas de la liste vide
    if(l->taille==0){
        //la liste global
        l->premier=m;
        l->dernier=m;
        //les listes sont initialisé comme circulaire
        l->taille++;
        addListeSpec(m,Tabliste,'g'); //on fait les liens color et shape
        return;
    }

    else{
        m->ElemNext=l->premier; //on met devant
        m->ElemPrec=l->dernier;
        l->dernier->ElemNext=m; // le cote circulaire
        l->premier->ElemPrec=m;
        l->premier=m;
        l->taille++;
        addListeSpec(m,Tabliste,'g'); //les liens sont fait a gauche et on est bon
        }
    return;
}

void list_insere_fin(Liste *l,Maillon *m,Liste *Tabliste[8]){
    //cas de la liste vide
    if(l->taille==0){
        //la liste global
        l->premier=m;
        l->dernier=m;
        //les listes sont initialisé comme circulaire
        l->taille++;
        addListeSpec(m,Tabliste,'d'); //on fait les liens color et shape
        return;
    }
    else{
        m->ElemNext=l->premier;
        m->ElemPrec=l->dernier; 
        l->dernier->ElemNext=m; // le cote circulaire
        l->premier->ElemPrec=m;
        l->dernier=m;
        l->taille++;
        addListeSpec(m,Tabliste,'d'); //les liens sont fait a droite et on est bon
        }
    return;
}






/*-----focntions de suppression au début de listes*/
//Nos caracteristique sont en char il nous faut les indices correspondant en int
int AsssignForm(char cara){
    switch (cara)
    {
    case 'T':
        return 0;
        break;
    case 'C':
    return 1;
    case 'L':
    return 2;
    case 'R':
    return 3;
    default:
        break;
    }
    return 10;
}

int AsssignColor(char cara){
    switch (cara)
    {
    case 'r':
        return 4;
    case 'b':
        return 5;
    case 'v':
        return 6;
    case 'j':
        return 7;
    }
    return 10;
}

void SuppColor(Maillon *element, Liste* liste){
    if(liste->taille==1){ // cas avec un element
        liste->dernier=NULL;
        liste->premier=NULL;
        liste->taille=0;
        return;
    }

    //3 cas
    if(liste->premier==element){
        element->ColorNext->ColorPrev=liste->dernier; // le 2nd pointe sur le dernier
        liste->premier=element->ColorNext;
        liste->dernier->ColorNext=liste->premier;
        liste->taille--;
        return;
    }
    if(liste->dernier==element){
        element->ColorPrev->ColorNext=liste->premier;
        liste->dernier=element->ColorPrev;
        liste->premier->ColorPrev=liste->dernier;
        liste->taille--;
        return;
    }
    //cas du milieu,
    element->ColorNext->ColorPrev=element->ColorPrev;
    element->ColorPrev->ColorNext=element->ColorNext;
    liste->taille--;
    return;
}


void SUppForm(Maillon *element, Liste *liste){
    if(liste->taille==1){ // cas avec un element
        liste->dernier=NULL;
        liste->premier=NULL;
        liste->taille=0;
        return;
    }
    if(liste->premier==element){
        element->FormNext->FormPrev=liste->dernier; // le 2nd pointe sur le dernier
        liste->premier=element->FormNext;
        liste->dernier->FormNext=liste->premier;
        liste->taille--;
        return;
    }
    if(liste->dernier==element){
        element->FormPrev->FormNext=liste->premier;
        liste->dernier=element->FormPrev;
        liste->premier->FormPrev=liste->dernier;
        liste->taille--;
        return;
    }
    //cas du milieu, va falloir faire une boucle
    element->FormNext->FormPrev=element->FormPrev;
    element->FormPrev->FormNext=element->FormNext;
    liste->taille--;
    return;
}

void Supp(Maillon *element, Liste *tabListe[8],Liste *jeu){ // On va supprimer un element de la liste 3 cas debut fin et milieu
    int color=AsssignColor(element->couleur);
    int shape=AsssignForm(element->forme);
    SuppColor(element,tabListe[color]); //on supprime l'elment dans les lsites color et shape
    SUppForm(element,tabListe[shape]);
    //On va le supp du jeu, 4 cas debut milieu, fin, et si 1 element dans le jeu
    
    if(jeu->taille==1){//cas 1 element
        libereElem(element);
        jeu->dernier=NULL;
        jeu->premier=NULL;
        jeu->taille--;
        return;
    }
    if(element==jeu->premier){//cas debut
        jeu->dernier->ElemNext=element->ElemNext;
        jeu->premier=element->ElemNext;
        libereElem(element);
        jeu->taille--;
        return;
    }
    if(element==jeu->dernier){//cas fin
        jeu->dernier=element->ElemPrec;
        jeu->premier->ElemPrec=jeu->dernier;
        libereElem(element);
        jeu->taille--;
        return;
    }//cas milieu
    element->ElemNext->ElemPrec=element->ElemPrec;
    element->ElemPrec->ElemNext=element->ElemNext;
    libereElem(element);
    jeu->taille--;
    return;
}



//Fonction des decalage
void Decalage(Liste *jeu,Liste *tabliste[8], char direction, char cara){
//4 cas en fonction du sens et couleur ou forme
int i;
int j;
if (cara<91) i=AsssignForm(cara); //on cherche le tableau qui nous interrese
else i=AsssignColor(cara);
if(tabliste[i]->taille==1 ||tabliste[i]->taille==0) return;
//On va echanger par rotation les arguments du tableau d'indice i , si i<4 alors c'est les formes et on change leur couleur
if(direction=='g'){
    Maillon *temp=tabliste[i]->premier;
    if(i<4){//Les formes
        char Colortemp=temp->couleur;
        for(j=0;j<tabliste[i]->taille-1;j++){ // On va parcourir la liste
            temp->couleur=temp->FormNext->couleur;
            temp=temp->FormNext;
        }
        temp->couleur=Colortemp;
    }
    else{//Les couleurs
        char Formtemp=temp->forme;
        for(j=0;j<tabliste[i]->taille-1;j++){ // On va parcourir la liste
            temp->forme=temp->ColorNext->forme;
            temp=temp->ColorNext;
        }
        temp->forme=Formtemp;
    }
}
else{
    Maillon *temp=tabliste[i]->dernier;
    if(i<4){ //Les formes
        char Colortemp=temp->couleur;
        for(j=0;j<tabliste[i]->taille-1;j++){ // On va parcourir la liste
            temp->couleur=temp->FormPrev->couleur;
            temp=temp->FormPrev;
        }
        temp->couleur=Colortemp;
    }
    else{ //Les couleurs
        char FormTemp=temp->forme;
        for(j=0;j<tabliste[i]->taille-1;j++){ // On va parcourir la liste
            temp->forme=temp->ColorPrev->forme;
            temp=temp->ColorPrev;
        }
        temp->forme=FormTemp;    
    }
}
//En théorie on a update la liste de jeu mais on a cassé tout les liens de formes et couleurs
//On va supprimer toutes listes couleurs et les reformer avec insertion fin mais sur les spec, un peu comme dans les saves
for(int k=0;k<4;k++){
    liste_resetColor(tabliste[k+4]);
    liste_resetShape(tabliste[k]);
}
Maillon *Inter=jeu->premier;
for(j=0;j<jeu->taille;j++){
    addListeSpec(Inter,tabliste,'d');
    Inter=Inter->ElemNext;
}
return;
}



//---------creation aléatoire des pièces-------//

Maillon *GenerateurPiece(void){
    // on va générer 2 nombre aleatoire qui representerons la forme et la couleur
    int n1,n2;
    char forme,couleur;
    n1=rand()%4;        // on prend le reste du modulo 4 d'un entier aleatoire
    n2=rand()%4;
    if(n1==0) forme='C';
    if(n1==1) forme='L';
    if(n1==2) forme='T';
    if(n1==3) forme='R';
    if(n2==0) couleur='r';
    if(n2==1) couleur='j';
    if(n2==2) couleur='b';
    if(n2==3) couleur='v';
    return creer_maillon(couleur, forme);
}



//-----ça c'est toujours utile-----//
int nbligne(char* nomfichier){
    FILE* f=fopen(nomfichier, "r");
    int nb=0;
    char buffer[TAILLE];
    while(fgets(buffer,TAILLE,f)!=NULL){
        nb+=1;}
    fclose(f);
    if (nb==0)
        printf("erreur fichier vide\n");

    return nb;
}

//-----------partie sauvegarde-----------//
// But lire le fichier sauvegarde 
//Format : Color+Shape ( ex : rC on va utiliste getsc) puis les 5derniers sont les piece suive
int chargersauvegarde(Liste *jeu, Maillon **piecesuiv, Liste **TabListe, int *combo){
    int nb=nbligne("save.txt"); // Nombre de ligne et donc de maillon
    if(nb==0){ //fichier vide
        return 0;
    }
    FILE *f=fopen("save.txt","r");
    int score;
    fscanf(f,"%d",&score);
    fscanf(f,"%d",combo);
    char color;
    char shape;
    Maillon *m;
    char buffer[5];
    for(int i=0;i<nb-5-2;i++){
        fscanf(f,"%s",buffer);
        color=buffer[0];
        shape=buffer[1];
        m=creer_maillon(color,shape);
        list_insere_fin(jeu,m,TabListe); //On ajoute a la fin les maillon lu au fur et a mesure
    }
    for(int i=0;i<5;i++){
        fscanf(f,"%s",buffer);
        color=buffer[0];
        shape=buffer[1];
        piecesuiv[i]=creer_maillon(color,shape); //On ajoute a la fin les maillon lu au fur et a mesure
    }
    fclose(f);
    return score;
}

void overwritesave(Liste *jeu, int score, int combo,Maillon* piecesuiv[5]){
    //On ecrit la save dans le format au dessus
    FILE *f=fopen("save.txt","w");
    fprintf(f,"%d",score);
    fprintf(f,"\n%d",combo);
    int n;
    Maillon *temp=jeu->premier;
    for(n=0;n<jeu->taille;n++){// on met le plateau
        fprintf(f,"\n%c%c",temp->couleur,temp->forme);
        temp=temp->ElemNext;
    }
    for(n=0;n<5;n++){ //on met les pieces suivantes
    fprintf(f,"\n%c%c",piecesuiv[n]->couleur,piecesuiv[n]->forme);
    }
    fclose(f);

    printf("\nSauvegarde Réussite\n");
    return;
}

//------partie score ------//

// Format
//Score NOM

void HighScore(int score, char* pseudo){
    //but mettre le nouveau score ou il faut
    //Tout d'abord il nous faut le nb de ligne du fichier
    int nb=nbligne("score.txt");
    if (nb==0){ //si le fichier est vide on met notre score
        FILE* f=fopen("score.txt", "w");
        fprintf(f,"%d",score);
        fprintf(f,"%s",pseudo);
        fclose(f);
        return;
    }
    char noms[nb][TAILLE]; // les nb lignes
    // On recup les lignes
    int scoring[nb];
    FILE* f=fopen("score.txt", "r");
    for(int i=0;i<nb;i++){
        fscanf(f,"%d",scoring+i);
        fscanf(f,"%s",noms[i]);
    }
    fclose(f);
    //On va chercher l'indice ou on doit inserer notre score
    int i=0;
    int stop=0;
    int cpt=0; //compteur des lignes mise dans le doc
    f=fopen("score.txt", "w");
    //On met les lignes une par une dans le bonne ordre
    printf("Les 10 meilleurs scores :\n");
   while(cpt<10 && cpt<nb+1){
        if(score>scoring[i] && stop==0){
            fprintf(f,"%d %s\n",score,pseudo);
            printf("%d %s", score,pseudo);
            stop++;
        }
        else{
            fprintf(f,"%d %s",scoring[i],noms[i]);
            printf("%d %s\n",scoring[i], noms[i] );
            i++;}        
        if(cpt!=nb && cpt!=nb) fprintf(f,"\n");
        cpt++;
   }
    fclose(f);
    return;
}

//--------partie decalage------//


int detectionadd(Liste *jeu,char direction, Liste *TabListe[8]){
    //But : detection des suites de 3 forme/couleur consecutive après l'ajout et return le nombre de piece consecutive
    // Elle fera ensuite appel à supression pour supprimer toutes les pieces conserné
    //Le return va servir pour les maj de score et combo on return 1 si triple et 0 sinon
    //marche bien pour quand add gauche mais pas a droite donc ou va utiliser des liste global double circulaire
    int points=0;
    if(direction=='g'){ //On check le debut 
        Maillon *elem1=jeu->premier;
        Maillon *elem2=jeu->premier->ElemNext;
        if((elem1->couleur==elem2->couleur )|| (elem1->forme==elem2->forme)){
            Maillon *elem3=elem2->ElemNext;
            if(elem2==elem1||elem3==elem1) return 0; //checks pour verifer que on fait pas le tour de la boucle
            if((elem1->couleur==elem2->couleur && elem3->couleur==elem1->couleur)|| (elem1->forme==elem2->forme && elem3->forme==elem1->forme)){
                if((elem1->couleur==elem2->couleur && elem3->couleur==elem1->couleur) && (elem1->forme==elem2->forme && elem3->forme==elem1->forme))
                points=3; //points troplé  si couleur et forme
                else points=1;
                //On supp les element concernés
                Supp(elem1,TabListe,jeu);
                Supp(elem2,TabListe,jeu);   
                Supp(elem3,TabListe,jeu);
                return points;
            }
        }
    }
    if(direction=='d'){ //on check la fin
        Maillon *elem1=jeu->dernier;
        Maillon *elem2=jeu->dernier->ElemPrec;
        if((elem1->couleur==elem2->couleur) || (elem1->forme==elem2->forme)){
            Maillon *elem3=elem2->ElemPrec;
            if(elem1==elem2||elem1==elem3) return 0;
            if((elem1->couleur==elem2->couleur && elem3->couleur==elem1->couleur)|| (elem1->forme==elem2->forme && elem3->forme==elem1->forme)){
                if((elem1->couleur==elem2->couleur && elem3->couleur==elem1->couleur) && (elem1->forme==elem2->forme && elem3->forme==elem1->forme))
                points=3; //points troplé  si couleur et forme
                else points=1;
                //dans ce cas on un un triple a la fin               
                Supp(elem3,TabListe,jeu); 
                Supp(elem2,TabListe,jeu);
                Supp(elem1,TabListe,jeu);
                return points;
            }
        }
    }





    return points;
}


int detectionDecalage(Liste *jeu, Liste *tabListe[8]){
    //A Appeler après decalage jusqu'a il y ai plus de suites
    //parcour la liste jusqu'a trouver une suite ou être à la fin, compte le nombre d'element d'affilé les supprimer et return leur nombre
    Maillon *temp=jeu->premier;
    Maillon *tempNext=temp->ElemNext;
    int nbcolor=1;
    int nbshape=1;
    for(int i=0;i<jeu->taille-1;i++){
        if(temp->couleur==tempNext->couleur)// On incremente le compteur color si meme color
            nbcolor++;
        if(tempNext->ElemNext==jeu->dernier){// cas ou on arrive à la fin
            if(nbcolor>2){ //On a une suite, phase suppression
                    for(int j=0;j<nbcolor;j++){ // supp dans les listes color et form
                        Supp(tempNext,tabListe,jeu); // On supprime l'element de fin de la suite et on remonte
                        tempNext=temp;
                        temp=temp->ElemPrec;}
                    return nbcolor;}
            if(nbshape>2){ 
                for(int j=0;j<nbshape;j++){
                    Supp(tempNext,tabListe,jeu);
                    tempNext=temp;
                    temp=temp->ElemPrec;
                }
                return nbshape;
                }
                
        }
        if(temp->couleur!=tempNext->ElemNext->couleur)
            { //Si le prochain de la suite est different alors on est au max d'affilé
                if(nbcolor>2){ //On a une suite, phase suppression
                    for(int j=0;j<nbcolor;j++){ // supp dans les listes color et form
                        Supp(tempNext,tabListe,jeu); // On supprime l'element de fin de la suite et on remonte
                        tempNext=temp;
                        temp=temp->ElemPrec;}
                    return nbcolor;}
                else {
                    nbcolor=1;
                    }
                }
        if(temp->forme==tempNext->forme)
            nbshape++;
        if(temp->forme!=tempNext->ElemNext->forme){
            if(nbshape>2){ 
                for(int j=0;j<nbshape;j++){
                    Supp(tempNext,tabListe,jeu);
                    tempNext=temp;
                    temp=temp->ElemPrec;
                }
                return nbshape;
                }
            else nbshape=1;
        }
    temp=tempNext;
    tempNext=tempNext->ElemNext;
    }
    return 0;
}


int ScoreDecalage(int score, int*combo,int indicecoup){
    if(indicecoup==0){
        return score;
    }
    (*combo)++;
        if(*combo>2){
            score=score+((*combo)*5*indicecoup);
        }
        else score=score+5*indicecoup;
        return score;
}
/*-------Fonctions d'affichage--------*/
void affichepiece(Maillon *piece){
    char color=piece->couleur;
    char shape=piece->forme;
    if(color=='b'){
        printf("\033[34m%C   ",shape);
    }
    if(color=='j'){
        printf("\033[33m%C   ",shape);
    }
    if(color=='v'){
        printf("\033[32m%C   ",shape);
    }
    if(color=='r'){
        printf("\033[31m%C   ",shape);
    }
    printf("\033[39m");
    return;
}

void setupterminal(void){
    printf("\033[2J");
    printf("\033[0;0H");
     printf("\033[39m");
    return;
}

void afficheplateau(Liste*jeu){
    Maillon *piece=jeu->premier;
    for (int n=0;n<jeu->taille;n++){
        if(piece==NULL) return;
        affichepiece(piece);
        piece=piece->ElemNext; //c'est circulaire donc on fais le tour puis on rearrive magiquement au debut
    }
    piece=piece->ElemNext;
    return;
}

void afficheListeColor(Liste* liste){ //affiche une liste color
    Maillon *piece=liste->premier;
    for (int n=0;n<liste->taille;n++){
        if(piece==NULL) return;
        affichepiece(piece);
        piece=piece->ColorNext; //c'est circulaire donc on fais le tour puis on rearrive magiquement au debut
    }
    return;
}

void afficheListeShape(Liste* liste){ //affiche une liste de forme
    Maillon *piece=liste->premier;
    for (int n=0;n<liste->taille;n++){
        affichepiece(piece);
        piece=piece->FormNext; //c'est circulaire donc on fais le tour puis on rearrive magiquement au debut
    }
    return;
}

void AfficheJeu(int score, Liste*jeu,int combo,Maillon *Piecesuiv[5]){
    printf("votre score est : \033[31m%d \n",score);
    printf("\033[39mvotre combo est : \033[31m%d \n",combo);
    printf("\033[39mTaille du plateau: %d\n",jeu->taille);
    printf("\033[35mPlateau actuel : \033[39m\n");
    printf("\n\n");
    if(jeu->taille!=0) afficheplateau(jeu);
    printf("\n\n\n\n\n");
    printf("----------   Pieces Suivantes  -----------\n");
    for (int n=0;n<5;n++){
        affichepiece(Piecesuiv[n]);
    }
    printf("\n");
}

void AfficheSousListe(Liste *Tabliste[8]){
    char test;
    printf("Les listes apres le tour\n");
    afficheListeShape(Tabliste[0]);
    printf("\n");
    afficheListeShape(Tabliste[1]);
    printf("\n");
    afficheListeShape(Tabliste[2]);
    printf("\n");
    afficheListeShape(Tabliste[3]);
    printf("\n");
    afficheListeColor(Tabliste[4]);
    printf("\n");
    afficheListeColor(Tabliste[5]);
    printf("\n");
    afficheListeColor(Tabliste[6]);
    printf("\n");
    afficheListeColor(Tabliste[7]);
    printf("\n");
    printf("ecire c");
    scanf(" %c",&test);
    return;
}
