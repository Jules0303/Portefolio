
#include "traducteur.h"


//But faire la partie traduction



// Fonction de base, affiche , cree et libere les types defini

void afficheinstru(instruction* instru){
    int i=0;
    printf(" nom: %s \n", instru->nom);
    while(i<(instru->taille)){
    printf("arg : %s\n",instru->arg[i]);
    i++;}
    printf("%d\n",instru->taille);
    printf("%d\n",instru->immediat);
    return;
}

void afficheEtiquette(etiquette* eti){
    printf(" nom de l'etiquette %s sa ligne :%d\n",eti->etiq, eti->ligne);
    return;
}

void libereinstr(instruction* instru){
    free(instru->nom);
    for(int i=0;i<instru->taille;i++){
    free(instru->arg[i]);}
    free(instru);
    return;
}

void libereetiquette(etiquette* etiq){
    free(etiq->etiq);
    free(&(etiq->ligne));
    free(etiq);
    return;
}

etiquette *cree_etiquette( char *etiq, int ligne){
    etiquette *m=malloc(sizeof(etiquette));
    if (m==NULL){
        printf("erreur manque de m¨¦moire\n");
        return NULL;
    }
    m->etiq=strdup(etiq);
    m->ligne=ligne;
    return m;

}

instruction *cree_instr( char *nom, char** arg,int taille,int immediat){
    instruction *m=malloc(sizeof(instruction));
    if (m==NULL){
        printf("erreur manque de m¨¦moire\n");
        return NULL;
    }
    m->nom=strdup(nom);
    m->arg=arg;
    m->taille=taille;
    m->immediat=immediat;
    return m;

}

/* -----lecture de fichier-------*/

// il nous faut le nombre de ligne du fichier
int nbligne(char* nomfichier){
    FILE* f=fopen(nomfichier, "r");
    int nb=0;
    char buffer[Taille];
    while(fgets(buffer,Taille,f)!=NULL){ //on incremente tant que la ligne n'est pas vide
        nb+=1;}
    fclose(f);
    if (nb==0)
        printf("erreur fichier vide\n");
    return nb;
}

int isetiquette(char* ligne){
    // regarde s'il y a une etiquette dans la ligne
    if(strchr(ligne, ':')!=NULL){
        return 1;
    }
    return 0;
}


etiquette* etiquettechr(char*ligne, int numligne){
    //s'occupe d'isoler l'etiquette de la retourner dans sa structure
        int i=0;
        char *c=strpbrk(ligne,":");  //on va compter le nombre de caractère de l'étiquette
        int p=c-ligne;
        char *etiq= malloc(sizeof(char)*p+1);
         if(etiq==NULL) {
            printf("erreur de memoire\n");
            return NULL;
        }
        while(*ligne!=':'){
            if(*ligne!=' '){
                *(etiq+i)=*ligne;
                i++;}
            ligne++;
        }
        *(etiq+i)='\0';
        if(*etiq=='\0'){
            printf("etiquette vide ligne %d",numligne+1);
            return NULL;
        }
        etiquette *etiquette=cree_etiquette(etiq,numligne);
        if (etiquette==NULL){
            printf("erreur de memoire");
            return NULL;
        }
        return etiquette;
}


int lectureligne(char* ligne, etiquette **Tabetiq,char **  document,int numligne,int cpt){
    // Le but de cette fonction et de recuperer les etiquettes dans un tableau d'etiquette et de stocker
    //la partie apres les : des lignes dans un tableau de lignes
    // return -1 s'il y a une erreur sinon retourne le nombre d'etiquette
    if(isetiquette(ligne)==1){
        etiquette* etiq=malloc(sizeof(etiquette));
        etiq=etiquettechr(ligne,numligne);
        if (etiq==NULL) return -1;
        *(Tabetiq+cpt)=etiq;
        cpt++;}
    while(*ligne==' ') ligne++;
    *(document+numligne)=ligne;
    return cpt;
}




int lecturefichier(int nb, char* nomfichier,etiquette **Tabetiq,char **  document){
    FILE* f=fopen(nomfichier, "r");
    int cpt=0;  //compte d'etiquette
    int i;
    for (i=0; i<nb;i++){
        char *buffer=malloc(sizeof(char)*Taille);
        fgets(buffer,Taille,f);
        cpt=lectureligne(buffer,Tabetiq,document,i,cpt);
        if (cpt==-1) return -1;
       }
    *(Tabetiq+cpt)=NULL;
    fclose(f);
return 0;
}


void LigneVersInstruction(char* ligne,instruction** listeinstr, int* codeErreur){
    // but separer la ligne et en faire une instruction avec les arguments qui vont
    //on s'est occuper des etiquette alors on se place apres : s'il y en a
    char* d=malloc(sizeof(char)*Taille);
    //check au cas ou
    if(strchr(ligne,':')!=strrchr(ligne,':')){
        printf("erreur bizzare vos etiquettes");
        *codeErreur=0;
        return;
    }
    d=strchr(ligne,':');
    if(d!=NULL) ligne=d+1;
    while(*ligne==' ')ligne++;
    int size=strlen(ligne);
    instruction *instr=malloc(sizeof(instruction));
    if (instr==NULL){
        printf("erreur de memoire");
        *codeErreur=0;
        return;
    }
    instr->nom=malloc(sizeof(char)*size);
        if (instr->nom==NULL){
        printf("erreur de memoire");
        *codeErreur=0;
        return;
    }
    // recup du nom de l'instruction
    int i=0;
    while((*(ligne+i) !=' ') && (*(ligne+i)!=',') && (i<size)){
        (instr->nom)[i]=*(ligne+i);
        i++;
    }
    (instr->nom)[i]='\0'; // on oublie pas le caractere de fin
    while(*(ligne+i)==' ' && i<size) i++; //on se place devant le mot suivant
    instr->arg=malloc(sizeof(char*)*6); //on met 6 argument max on est large
    if (instr->arg==NULL){
        printf("erreur de memoire");
        *codeErreur=0;
        return;
    }
    int comptearg=0;
    instr->immediat=0;
    if( strchr(ligne+i,'#')!=NULL) instr->immediat=1;
    //avec strtoken ca separe tout seul les mots selon les separteurs noté
    char * separateur=" ,()#\n\t";
    char *strToken=strtok(ligne+i,separateur);
    if (strToken==NULL) instr->taille=0; //cas ou c'est hlt
    while ( strToken != NULL ) {
        instr->arg[comptearg]=strToken;   //on met chaque argument dans l'instru
        comptearg++;
        // On demande le token suivant.
        strToken=strtok( NULL, separateur );
    }
    if (comptearg>3) {
        *codeErreur=0;
        return;}
    instr->taille=comptearg;
    //on update notre liste d'instru avec notre nouvelle ligne
    *listeinstr=instr;
    return;
}

//fonction pour verifer les doublons
int testEtiq(etiquette **tabetiq, int doublonligne[2]){
    int cpt=0; //nb d'etiquette
    while(tabetiq[cpt]!=NULL) cpt++;
    int i=0;
    for(int j=i+1;j<cpt;j++){ //on compare les tailles
        int lenght1=strlen(tabetiq[i]->etiq);
        int lenght2=strlen(tabetiq[j]->etiq);
        if(lenght1!=lenght2) break;
        if (strncmp(tabetiq[i]->etiq,tabetiq[j]->etiq,lenght1)==0){ //si on a la meme taille on compare les lettres
            doublonligne[0]=tabetiq[i]->ligne;
            doublonligne[1]=tabetiq[j]->ligne;
            return 0;
        }
    }
    return 1;
}



/*-------traduction en code hexa---------*/
//quand on a r# on verifer qu'on a bien r puis un nombre et on va -1 si erreur et strtop sinon
int checkregsitre(instruction *instru, int numarg){
if(**(instru->arg) !='r') return -1;
int i;
int len=strlen(*(instru->arg+numarg)+1);
if (len==0) return -1;
for(i=0;i<len;i++){
    if(isdigit(*(*(instru->arg+numarg)+1+i))==0) return-1;
}
//on a bien r quelque chose
int r=strtol(*(instru->arg+numarg)+1,NULL,10);
if( (-1<r) && (r<32)){
    return r;
}
return -1;

}

//check que la valeur immediate ait du sens
int CheckvaleurImmediate(instruction *instru, int indice, int hexa,int ligne){
    if(hexa==1){//cas avec la valeur en hexa
        for(int i=0;i<(int)strlen(*(instru->arg+indice)+1);i++){
            char lettre=*(*(instru->arg+indice)+1+i);
            if((lettre>47 && lettre<58)||(lettre>64 && lettre <71)){
                continue;
            }
            else {
                printf("erreur valeur immediate non comforme ligne : %d\n",ligne+1);
                return -1;}
        }

    }
    else{
        if(**(instru->arg+indice)=='-'){//cas negatif
            for(int i=1;i<(int)strlen(*(instru->arg+indice))-1;i++){
                if(isdigit(*(*(instru->arg+indice)+i))==0){
                    printf("erreur valeur immediate non comforme ligne %d\n", ligne+1);
                return -1;
        }}
        return 0;
        }
        for(int i=0;i<(int)strlen(*(instru->arg+indice));i++){
        if(isdigit(*(*(instru->arg+indice)+i))==0){
            printf("erreur valeur immediate non comforme ligne %d\n", ligne+1);
            return -1;
        }}
    }
    return 0;
}




int CodeOp(instruction *TabInstru ){
    if (strcmp(TabInstru->nom, "add")==0) return 0;
    if (strcmp(TabInstru->nom, "sub")==0) return 1;
    if (strcmp(TabInstru->nom, "mul")==0) return 2;
    if (strcmp(TabInstru->nom, "div")==0) return 3;
    if (strcmp(TabInstru->nom, "and")==0) return 4;
    if (strcmp(TabInstru->nom, "or")==0)  return 5;
    if (strcmp(TabInstru->nom, "xor")==0) return 6;
    if (strcmp(TabInstru->nom, "shl")==0) return 7;
    if (strcmp(TabInstru->nom, "ldb")==0) return 10;
    if (strcmp(TabInstru->nom, "ldw")==0) return 11;
    if (strcmp(TabInstru->nom, "stb")==0) return 12;
    if (strcmp(TabInstru->nom, "stw")==0) return 13;
    if (strcmp(TabInstru->nom, "jmp")==0) return 20;
    if (strcmp(TabInstru->nom, "jzs")==0) return 21;
    if (strcmp(TabInstru->nom, "jzc")==0) return 22;
    if (strcmp(TabInstru->nom, "jcs")==0) return 23;
    if (strcmp(TabInstru->nom, "jcc")==0) return 24;
    if (strcmp(TabInstru->nom, "jns")==0) return 25;
    if (strcmp(TabInstru->nom, "jnc")==0) return 26;
    if (strcmp(TabInstru->nom, "in")==0) return 27;
    if (strcmp(TabInstru->nom, "out")==0) return 28;
    if (strcmp(TabInstru->nom, "rnd")==0) return 29;
    if (strcmp(TabInstru->nom, "hlt")==0) return 31;
    return 32;
}

int recupRegisteDest(instruction* instru,int op,int nb){
    //d'abord la destination
    int dest=0;
    int i=(instru->taille);
    if(op==31) {
        if(i!=0) return-1;
        return 0;}
    // cas de hlt
    if((19<op)&& (op<27)) return 0; //ici la destination ne compte pas
    if(((-1<op)&& (op<8)) || ((9<op)&& (op<14))||(op==29)){ // cas avec 3 arg
        if(i!=3){
            printf("erreur d'argument ligne %d \n",nb+1);
            return -1;
        }
        dest=checkregsitre(instru,0);
        if(dest==-1) {
            printf("erreur dans les registre ligne %d\n", nb+1);
            return -1;}
        return dest;
        
    }
     if((op==27 || op==28)){ //cas avec 1 arg
        if(i!=1){
            printf("erreur d'argument ligne %d \n",nb+1);
            return -1;
        }
        dest=checkregsitre(instru,0);
        if(dest==-1) {
            printf("erreur dans les registre ligne %d\n", nb+1);
            return -1;}
        return dest;
     }
    return -1;
}

int recupSrc1(instruction* instru,int op,int nb){
    int src=0;
    int i=(instru->taille);
    if(op==31) return 0; // cas de hlt
    if((19<op && op<29)||op==27|| op==28) return 0; //ici la src1 ne compte pas
    if((-1<op && op<8) ||op==29||op==10||op==11){
        if(i!=3){
            printf("erreur d'argument ligne %d \n",nb+1);
            return -1;
        }
        src=checkregsitre(instru,1);
        if(src==-1) {
            printf("erreur dans les registre ligne %d\n", nb+1);
            return -1;}
        return src;
        
    }
    if(op==12 || op==13){
        if(i!=3){
            printf("erreur d'argument ligne %d \n",nb+1);
            return -1;
        }
        src=checkregsitre(instru,2);
        if(src==-1) {
            printf("erreur dans les registre ligne %d\n", nb+1);
            return -1;}
        return src;
    }
        return -1;
    }

int RecupSrc2(instruction* instru,int op,etiquette **tabetiq,int nb,int *error){
    int immediat=instru->immediat;
    int i=instru->taille;
    int src=0;
    //OOn recup Src2 en fonction du code op on le fait differamment
    if(op==31|| op==27 || op==28) return 0;
 //dans le cas de registre on fait en gros comme pour src 1
 // Une particularité si S est immedit hexa decimal donc si on a hS
        if((-1<op && op<8)||(op==10||op==11||op==29)){
            if(i!=3){
                printf("erreur d'argument ligne %d \n",nb+1);
                *error=1;
                return -1;
            }
            if(immediat==1 && strchr((*(instru->arg+2)),'h')!=NULL){  // on a 3 cas a chaque fois, immediat, immediat hexa, registre
                if( CheckvaleurImmediate(instru,2,1,nb)==-1) 
                    {*error=1;
                    return -1;}
                src=strtol(*(instru->arg+2)+1,NULL,16); 
                return src;}
            if(immediat==1){
                if( (CheckvaleurImmediate(instru,2,0,nb))==-1) 
                    {*error=1;
                    return -1;}
                src=strtol(*(instru->arg+2),NULL,10);
                return src;}
            src=checkregsitre(instru,2);
            if(src==-1) {
                printf("erreur dans les registre ligne %d\n", nb+1);
                *error=1;
                return -1;}
            return src;}
        if(op==12 ||op==13){
                if(i!=3){
                printf("erreur d'argument ligne %d \n",nb+1);
                *error=1;
                return -1;
            }
            if(immediat==1 && strchr((*(instru->arg+1)),'h')!=NULL){
                if( CheckvaleurImmediate(instru,1,1,nb)==-1) 
                    {*error=1;
                    return -1;}
                src=strtol(*(instru->arg+1),NULL,16);
                return src;}
            if(immediat==1){
                if( (CheckvaleurImmediate(instru,1,0,nb))==-1) 
                {*error=1;
                return -1;}
                src=strtol(*(instru->arg+1),NULL,10);
                return src;}
            src=checkregsitre(instru,1);
            if(src==-1) {
                printf("erreur dans les registre ligne %d\n", nb+1);
                *error=1;
                return -1;
            return src;}}
        if (19<op && op<27){
            if(i!=1){
                printf("erreur d'argument ligne %d \n",nb+1);
                *error=1;
                return -1;}
            if(immediat==1){
            if(strchr((*(instru->arg)),'h')!=NULL){
                if( CheckvaleurImmediate(instru,0,1,nb)==-1) {*error=1;
                return -1;}
                src=strtol(*(instru->arg)+1,NULL,16); 
                return src;}
            if( (CheckvaleurImmediate(instru,0,0,nb))==-1) {*error=1;
                return -1;}
            src=strtol(*(instru->arg)+1,NULL,10);
            return src;}
            //on vient de faire le cas immediat mainteant si c'est un vrai etiquette on return la ligne de l'etiquette
            //on compare notre arg avec les etiquette et si on a le meme on return la ligne
            int cpt=0;
            //Cas ou S est r#
            if(immediat==0){
                src=checkregsitre(instru,0);
                if (src!=-1) return src;
            }
            instru->immediat=1;
            //on a des espace en trop on dirait
            while(tabetiq[cpt]!=NULL){
                if(strcmp(tabetiq[cpt]->etiq ,*(instru->arg))==0){
                    src=tabetiq[cpt]->ligne;
                    return src*4;
                }
                cpt++;
            }
            printf("etiquette associé introuvable ligne %d \n",nb+1);
        }
    *error=1;
                return -1;
    }
