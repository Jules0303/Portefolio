
#include"interpreteur.c"
#include"traducteur.c"

//on fixe la taille max d'une ligne  100 characteres dans traducteur


int main(int argc,char* argv[]){
    //char* nomfichier=argv[1];
    //on fait la lecture du fichier
        if (argc!=2) {printf("erreur dans l'importation de fichier");
                return -1;}
    int coderreur=1;
    int nb=-10;
    nb=nbligne(argv[1]);
    if(nb==0) return 0;
    //printf("nombre de ligne : %d\n",nb);
    etiquette **tabetiq=malloc(sizeof(etiquette*)*nb+1);
    char** document=malloc(sizeof(char*)*nb+1);
    int erreur=lecturefichier(nb,argv[1],tabetiq,document);
    if (erreur==-1) return 0;
    //On verifie les ettiquette
    int doublonsligne[2];
    if (testEtiq(tabetiq, doublonsligne)==0){
        printf("doublon dans les etiquettes aux ligne %d et %d\n",doublonsligne[0]+1,doublonsligne[1]+1);
        return 0;
    }
    //on s¨¦pare les instructions
    instruction **listedesinstr=malloc(sizeof(instruction*)*nb+1);
    //apr¨¨s cette boucle on un un tableau avec toutes les instruction et les etiquette
    for ( int i=0;i<nb;i++){
        LigneVersInstruction(*(document+i),listedesinstr+i,&coderreur);
        if(coderreur==0) {
            printf("erreur dans les instructions ligne %d", i+1);
            return 0;}
    }
    *(listedesinstr+nb)=NULL;
    /* ----Convertion en code hexa----*/
    //boucle ligne par ligne sur les instru
    int TabCode[nb][5];// on declare une matrice de dimension nb * 5 pour stocker tous les codes d'instruction
    FILE *f=fopen("hexa.txt","w");
    int error=0;
    for(int i=0;i<nb;i++){
        int codenum=0;
        int op=CodeOp(listedesinstr[i]);
        if(op==32){
            printf("erreur de code operatoire ligne %d , mauvaise instruction",i+1);
            return 0;
        }
        //printf("code op %d\n",op);
        int dest=recupRegisteDest(listedesinstr[i], op,i);
        if(dest==-1) return 0;
        //printf("dest %d    ",dest);
        int src1=recupSrc1( listedesinstr[i],op,i);
        if(src1==-1) return 0;
        //printf("src1 %d    ",src1);
        int src2=RecupSrc2(listedesinstr[i],op,tabetiq,i,&error);
        if(error==1) return 0;
        
        //printf("src2 %d   \n\n",src2);
        TabCode[i][4]=src2;  // on stock le code dans le bon endroit de la matrice
        TabCode[i][2]=src1;
        TabCode[i][1]=dest;
        TabCode[i][0]=op;
        int dest1=dest<<22;
        int src3=src1<<17;
        int op1=op<<27;
        int imm=listedesinstr[i]->immediat;
        TabCode[i][3]=imm;
        int imm1=imm<<16;
        codenum=op1+dest1+src3+imm1+src2;
        fprintf(f,"%08X",codenum);//08 pour force de mettre les 8 caracteres devant le code
        if(i<nb-1) fprintf(f,"\n");
    }
    fclose(f);
    int i=0; // une variable qui verifie l adresse du saut
    int verifier=0; // une variable qui verifie si les valeurs sont bien entier et represente par 16 bits
    int temp=0;
    while (i<nb){
        temp=i; // une variable qui indique le numero de la ligne ou se trouve l erreur
        switch(TabCode[i][0]){ // on execute le bon l instruction par rapport au code operation
            case 0: verifier=add(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 1: verifier=sub(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 2: mul(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 3: div1(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 4: and(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 5: or(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 6: xor(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 7: shl(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 10: verifier=ldb(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 11: verifier=ldw(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 12: verifier=stb(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 13: verifier=stw(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 20: i=jmp(TabCode[i][3],TabCode[i][4])-1;
                    break;
            case 21: if (jzs(TabCode[i][3],TabCode[i][4])>-1){
                        i= jzs(TabCode[i][3],TabCode[i][4])-1;}
                    else{
                        if (jzs(TabCode[i][3],TabCode[i][4])==-2){
                            i=-2;
                        }
                    }
                    break;
            case 22:  if (jzc(TabCode[i][3],TabCode[i][4])>-1){
                        i= jzc(TabCode[i][3],TabCode[i][4])-1;}
                    else{
                        if (jzc(TabCode[i][3],TabCode[i][4])==-2){
                            i=-2;
                        }
                    }
                    break;
            case 23: if (jcs(TabCode[i][3],TabCode[i][4])>-1){
                        i= jcs(TabCode[i][3],TabCode[i][4])-1;}
                    else{
                        if (jcs(TabCode[i][3],TabCode[i][4])==-2){
                            i=-2;
                        }
                    }
                    break;
            case 24: if (jcc(TabCode[i][3],TabCode[i][4])>-1){
                        i= jcc(TabCode[i][3],TabCode[i][4])-1;}
                    else{
                        if (jcc(TabCode[i][3],TabCode[i][4])==-2){
                            i=-2;
                        }
                    }
                    break;
            case 25: if (jns(TabCode[i][3],TabCode[i][4])>-1){
                        i= jns(TabCode[i][3],TabCode[i][4])-1;}
                    else{
                        if (jns(TabCode[i][3],TabCode[i][4])==-2){
                            i=-2;
                        }
                    }
                    break;
            case 26: if (jnc(TabCode[i][3],TabCode[i][4])>-1){
                        i= jnc(TabCode[i][3],TabCode[i][4])-1;
                    }
                    else{
                        if (jnc(TabCode[i][3],TabCode[i][4])==-2){
                            i=-2;
                        }
                    }
                    break;
            case 27:verifier=in(TabCode[i][1]);
                    break;
            case 28: out(TabCode[i][1]);
                    break;
            case 29: rnd(TabCode[i][1],TabCode[i][2],TabCode[i][3],TabCode[i][4]);
                    break;
            case 31: fclose(f);
                    printf("Traduction Reussite\n");
                    return 0;
        }
        if (verifier<0){ 
            return 0;
        }
        i++;
        if (i<0){
            printf("Erreur de saut a la ligne %d",temp);
            return 0;
        }
    }
}



