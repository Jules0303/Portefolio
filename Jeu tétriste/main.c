#include"Fonctions.c"
#include<time.h>
#define Plateau 15


int Partie(Liste *jeu,Liste *Tabliste[8],Maillon *Piecesuiv[5],int score,int combo,int running,char *pseudo){ //La Partie en elle meme
char reponse,reponse2;
while(1){ // Boucle infinie temps que pas 15 pieces ou stop
    if(jeu->taille==Plateau){
        running=0;
        setupterminal();
        printf("Taille du plateau maximal fin de la partie\n");
        printf("votre score est : \033[31m%d\n",score);
        printf("\033[39m\n\n");
        break;
    }
    setupterminal();
    AfficheJeu(score,jeu,combo,Piecesuiv);

    reponse='a';
    printf("\n voulez ajouter une piece a gauche(g) ou a droite(d)?\n");
    while(reponse!='g' && reponse!='d' && reponse!='s'){
        scanf(" %c",&reponse);
    }
    if(reponse=='s'){
        break;
    }
    if(reponse=='g'){
        list_ajout_debut(jeu,Piecesuiv[0],Tabliste);
    }
    else{
        list_insere_fin(jeu,Piecesuiv[0],Tabliste);
    }
    //On update les pieces suivantes
    for( int n=0;n<4;n++){
        Piecesuiv[n]=Piecesuiv[n+1];
    }
    Piecesuiv[4]=GenerateurPiece();

    // On va lancer la detection de triples ou mieux
    int indicecoup=0;
    if(jeu->taille>=3){
        indicecoup=detectionadd(jeu,reponse,Tabliste);
    }
    if(indicecoup!=0){
        combo++;
        if(combo<3){
        score+=(5*indicecoup);}
        else score=score+(5*indicecoup*combo);

    }
    else combo=0;
    setupterminal();

    /*-----Partie decalage-------*/

    reponse='a';
    AfficheJeu(score,jeu,combo,Piecesuiv);
    while(reponse!='y' && reponse!='n' && reponse!='s'){
            printf("voulez vous faire un decalage? (y)yes or (n)no?\n");
            scanf(" %c",&reponse);
    }
    if(reponse=='s') break;
    if(reponse=='n') continue;
    reponse='a';
    while(reponse!='r' && reponse!='b' && reponse!='v' && reponse!='j' && reponse!='C' && reponse!='R' && reponse!='L' && reponse!='T' && reponse!='s'){
            printf("Sur les formes :Triangle(T), Carre(C), Losange(L),Rond(R)\n les couleur: bleu(b),vert(v),rouge(r),jaune(j)?\n");
            scanf(" %c",&reponse);
    }
    if(reponse=='s') break;
    reponse2='a';
    while(reponse2!='g' && reponse2!='d' && reponse2!='s'){
            printf("vers la droite(d) ou la gauche(g)\n");
            scanf(" %c",&reponse2);
    }
    if(reponse2=='s') break;
    Decalage(jeu,Tabliste,reponse2,reponse);


    if(jeu->taille==3) {setupterminal();combo=0;continue;} //Si y'a que 3 element après decalage peut pas y avoir de suite
    indicecoup=detectionDecalage(jeu,Tabliste); //indice coup et le nb d'element de la suite
    if(indicecoup!=0){
        score=ScoreDecalage(score,&combo,indicecoup);
    }
    while(indicecoup!=0){
        indicecoup=detectionDecalage(jeu,Tabliste);
        score=ScoreDecalage(score,&combo,indicecoup);
        }
    setupterminal();
}
    //-----à la fin ------//;
    if(running==0)HighScore(score,pseudo); // On met le score si la partie est vraiment fini;
    else{
        reponse='a';
        while(reponse !='y' && reponse!='n'){
            printf("\n\n");
            printf("voulez vous sauvegarder la partie ? (y) ou (n)");
            scanf("%c",&reponse);}
        if(reponse=='y'){
         overwritesave(jeu,score,combo,Piecesuiv);
         }
    }   
    printf("\n\n\n\n---------------Fin de la partie------------------");
    //On supprime tout, dans le jeu et les pieces suivants
    for(int j=0;j<jeu->taille;j=0){
        Supp(jeu->premier,Tabliste,jeu);
    }
    for(int k=0;k<5;k++){
    	libereElem(Piecesuiv[k]);
    }

    if(running==1) return 0;
    return 1;

}





//-----------main-------------//
int main(){ 

    srand(time(NULL)); // on modifie la seed sinon c'est toujours les memes pieces pour la generation aleatoire
    //-------- initalisation du programme------------//
    /* Listes pour les couleur et les formes */
    Liste *Triangle=liste_init();
    Liste *Carre=liste_init();
    Liste *Losange=liste_init();
    Liste *Rond=liste_init();
    Liste *Rouge=liste_init();
    Liste *Bleu=liste_init();
    Liste *Vert=liste_init();
    Liste *Jaune=liste_init();
    Liste *Tabliste[8]={Triangle,Carre,Losange,Rond,Rouge,Bleu,Vert,Jaune}; // On accedera a ce tableau avec ls indices correspondant
    Liste *jeu=liste_init();
    int score=0; // On va stocker le score
    int combo=0; // Indice de combo on l'affiche si >0 et sera un multiplicateur de point si >2 coups d'affilé

    //-------- le blaba------//
    char reponse='a';
    char pseudo[TAILLE];
    setupterminal();

    printf("entrer votre pseudo %d char max",(TAILLE-10));
    scanf(" %s",pseudo);
    while(pseudo[0]=='\0' || pseudo[0]=='\n'){ // verifie que le pseudo est pas vide, mais ca marchera toujourd pas si y a un espace dans le pseudo
        setupterminal();
        printf("entrer votre pseudo %d char max",(TAILLE-10));
        scanf(" %s",pseudo);
    }
    printf(" Les règles : des pièces aleatoires sont générées vous pouvez les mettre à droite (d) ou à gauche (g) \n ");
    printf("vous aurez aussi la possibilité de faire des décalage vers la gauche ou la droite par couleur ou forme en tapant quand demandé :  (g ou d) + (f ou c) +(1er lettre de la forme ou couleur)\n\n\n ");
    while(reponse !='y' && reponse!='n'){
            setupterminal();
            printf("voulez vous lancez une partie? (y)yes or (n)no?\n");
            scanf(" %c",&reponse);
    }
    setupterminal();
    if (reponse=='n'){
        printf("bizzare comme réponse");
        return 0;
    }

    
    reponse='a';


    /*----initalise le jeu-----*/
    while(reponse !='y' && reponse!='n'){
            setupterminal();
            printf("voulez vous charger la sauvegarde? (y)yes or (n)no?\n");
            scanf(" %c",&reponse);
    }
    //On initialise les peices suiv avant la charge de sauvegarde
    Maillon *Piecesuiv[5]; // tab des 5 prochaines pièces chacune stocké en pointeur
    for (int j=0;j<5;j++){
        Piecesuiv[j]=GenerateurPiece();
    }
    if(reponse=='y'){
        score=chargersauvegarde(jeu,Piecesuiv,Tabliste,&combo);
    }

    //-----la partie commence pour de vrai------//

    /*---Le plan est : print les picèces actuelles en colonnes et afficher les 4 suivantes*/
    /* ---- Ensuite on demande ou ajouter la pièce puis si on fait un decalage, puis les pièces puis le sens*/

    setupterminal();
    printf("Début de la partie, Tapez s dans n'importe quel dialogue pour arreter\n");





    /*  ----- le programme tourne ------*/
    int running=1; //indice de stop, si on a stop alors le plateau est pas plein donc on peut save
    //on lance la partie, si on a stop on propose pas d'en faire une autre
    int stop=Partie(jeu,Tabliste,Piecesuiv,score,combo,running,pseudo);
    char temp='a';
    while(stop!=0){
        printf("\n\n");
        printf("\nVoulez refaire une partie ? yes(y)or no (n)\n");
            while(temp!='y' && temp!='n'){
                scanf(" %c",&temp);
                setupterminal();
            }
            if (temp=='n') break;
        for (int j=0;j<5;j++){
            Piecesuiv[j]=GenerateurPiece(); // On recree les pieces suivantes
        }
        score=0; //On reset nos variables
        combo=0;
        stop=Partie(jeu,Tabliste,Piecesuiv,score,combo,running,pseudo);
        temp='a';
    }
    for(int i=7;i>-1;i--){
        free(Tabliste[i]);
    }
    free(jeu);
    return 0;
    }
