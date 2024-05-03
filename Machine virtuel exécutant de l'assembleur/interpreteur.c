
#include "interpreteur.h"


char memo[65536]; //Adresse des memoires de 0 a 65535
char etat[3];  //Bit d'etat dans l'ordre Z,C,N
short registre[32]; // les registres r0 a r31

int verifierMemoire(int val){  // la fonction qui verifie l'adresse memoire est correcte
    if (val>65535 || val<0){
        printf("L'adresse de la memoire n'est pas correcte.");
        return  -1;
    }
    else{
        return 0;
    }
}

int verifierEntier(float val){ // une fonction qui verifie si la valeur est un entier
    if ((val - (int)val)!=0){
        printf("La valeur devait etre un entier");
        return -1;}
    else{
        return 0;
    }
}


int verifieLimite(int val){ // une fonction qui verifie si la valeur est representable par 16 bits
    if (val>32767 || val<-32768){
            printf("La valeur ne peut pas etre presentee par 16 bits");
            return -2;
    }
    else{
        return 0;
    }
}

void bitZ(int val){  // pour faire mise a jour le bit Z
    if (val){
        etat[0]=0; // si val est superieure a 0 alors bit Z vaut 0
    }
    else{
        etat[0]=1; //si val est egale a 0 alors bit Z vaut 1
    }
}

void bitN(int val){    // pour faire mise a jour le bit Z
    if (val<0){
        etat[2]=1; // si val est negative alors bit N vaut 1
    }
    else{
        etat[2]=0;//  si val est positive alors bit N vaut 0
    }
}

int add(int dest,int reg2, int imme, int S){ // faire l addition entre 2 valeur , et la variable imme permet de distinguer si S est un registre ou une valeur immediate
    if (imme==1){  // si imme vaut 1  alors S est une valeur immediate 
        registre[dest]=registre[reg2]+S;
        bitZ(registre[dest]);  //on fait le mise a jour pour le bit Z et bit N
        bitN(registre[dest]);
        if (dest==0){     // on remet r0 a valeur 0
        registre[dest]=0; 
        }
        return verifieLimite(registre[reg2]+S);
        }
    else{ //sinom on additionne la valeur qui est dans le registre <reg2> et le registre <S> et on le met dans le registre <dest>
        registre[dest]=registre[reg2]+registre[S];
        bitZ(registre[dest]);  //on fait le mise a jour pour le bit Z et bit N
        bitN(registre[dest]);
        if (dest==0){     // on remet r0 a valeur 0
        registre[dest]=0; 
        }
        return verifieLimite(registre[reg2]+registre[S]);
    }
    return 0;
}

int sub(int dest,int reg2, int imme, int S){ // on fait une soustraction et la meme facon de faire avec add
    if (imme==1){    // si imme vaut 1 alors S est une valeur immediate, donc on fait la soustraction directement
        registre[dest]=registre[reg2]-S;
        bitZ(registre[dest]);  //on fait le mise a jour pour le bit Z et bit N
        bitN(registre[dest]);
        if (dest==0){     // on remet r0 a valeur 0
        registre[dest]=0; 
        }
        return verifieLimite(registre[reg2]-S);
    }
    else{            // sinom on fait une soustraction entre les valeurs contiennent dans le registre [registre] et le registre[S]
        registre[dest]=registre[reg2]-registre[S];
        bitZ(registre[dest]);  //on fait le mise a jour pour le bit Z et bit N
        bitN(registre[dest]);
        if (dest==0){     // on remet r0 a valeur 0
        registre[dest]=0; 
        }
        return verifieLimite(registre[reg2]-registre[S]);
    }
}
char *ConvertirBinaire(int val){ // cette fonction permet de convertir les valeurs en binaire
    if (val >= 0){    // 1 er cas: une valeur positive
    char *str=malloc(sizeof(char)*17);// vu que les registres sont de valeur 16 bits donc on loue la memoire pour une chaine de charactere de taille 17 y compris \0
    for (int i=0;i<16;i++){    
        if (val-pow(2,(16-i-1))>=0){ // pour convertir en binaire on fait val -2**i puis on prend la difference , si est 1 alors i+1eme bit vaut 1 
            *(str+i)='1';
            val=val-pow(2,16-i-1);
        }
        else{
            *(str+i)='0'; //sinon il vaut 0
        }
    }
    *(str+16)='\0';
    return str;
    }
    else{    // cas ou la valeur est negative, on utilise le methode de complement a 2 mais reciproquement 
        char *str1= ConvertirBinaire((val*(-1))-1);// ici on prend valeur absolue de val et on fait -1, donc val est positif maintenant,grace a l algo dessus on obtient une liste chainee
      for (int i=0;i<16;i++){ //puis on les inverse
            if (*(str1+i)=='0'){
            *(str1+i)='1';
        }
        else{
            *(str1+i)='0';
        }
    }
    *(str1+16)='\0';
    return str1;}

}
char PoidsFaible(int val){ //cette fonction permet de trouver les PoidsFaible de val
    char *str=ConvertirBinaire(val); // on la converti en binaire
    char PoidsF=strtol(str+8,NULL,2);    //puis on le converti en decimal
    return PoidsF;    
}
char PoidsFort(int val){ // la meme logique que PoidsFaible
    char *str=ConvertirBinaire(val); // on le convertir en binaire
    char *p=malloc(sizeof(char)*9);
    for(int i=0;i<8;i++){ // on prends les 8 premieres caracteres
        *(p+i)=*(str+i);
    }
    *(p+8)='\0';
    char PoidsF=strtol(p,NULL,2); // on le converti en decimal
    return PoidsF;
}

int DecalageDroiteNeg(int val,int S){    //cette fonction permet de nous calculer la valeur apres les decalage a droite pour les valeurs negatives
    char *p=ConvertirBinaire(val);    // on la converti en binaire
    char *p1=(malloc(sizeof(char)*17));
    for (int i=0;i<S;i++){ //on met les nouveau bit en 0
        *(p1+i)='0';

    }
    for (int i=0;i<16-S;i++){// puis on effectue le decalage
        *(p1+S+i)=*(p+i);
    }
    return strtol(p1,NULL,2); // on le converti en decimal
}

void mul(int dest,int reg2, int imme, int S){ // la fonction qui fait la multiplication
    char PoidsF1=PoidsFaible(registre[reg2]); // on prend d'abord les poidsFaible de la valeur qui est dans le registre <reg2>
    char PoidsF2;                        
    if (imme==1){                    // si imme est 1 alors on fait la multiplication entre les poidsFaibles de S et reg <reg2>
        PoidsF2=PoidsFaible(S);
        registre[dest]=PoidsF1*PoidsF2;
    }
    else{
        PoidsF2=PoidsFaible(registre[S]); // sinon on fait la multiplication entre les poidsFaibles de reg<S> et reg <reg2>
        registre[dest]=PoidsF1*PoidsF2;
        }
    bitZ(registre[dest]);
    bitN(registre[dest]);
    if (dest==0){
        registre[dest]=0;
    }
}

int div1(int dest,int reg2,int imme,int S){ // la meme logique est la multiplication et l'addition
    if (imme==1){
        if (S==0){
            printf("le diviseur ne peut pas etre 0");
            return -1;
        }
        registre[dest]=registre[reg2]/S;
    }
    else{
         if (registre[S]==0){
            printf("le diviseur ne peut pas etre 0");
            return -1;
        }
        registre[dest]=registre[reg2]/registre[S];
    }
    bitZ(registre[dest]);
    bitN(registre[dest]);
    if (dest==0){
        registre[dest]=0;
    }
    return 0;
}


void and(int dest,int reg2, int imme, int S){    //on effectue l'operation logique and entre 2 valeur
    char *p1=ConvertirBinaire(registre[reg2]);
    char *p3=malloc(sizeof(char)*17);
    if (imme==1){                    // si imme vaut 1 alors on fait l'operation entre S et reg<reg2>
        char *p2=ConvertirBinaire(S);
        for (int i=0;i<16;i++){
            if (*(p1+i)!='1' || *(p2+i)!='1'){  // on compare les i+1 eme bits  ,si l'un des 2 n'est pas 1 alors ca nous donne 0
                *(p3+i)='0';
            }
            else{    // sinon il vaut 1
                *(p3+i)='1';
            }
        }
        *(p3+16)='\0';
        registre[dest]=strtol(p3,NULL,2);

    }
    else{    // sinon on effectue les memes demarches entre reg<S> et reg<reg2>
        char *p2=ConvertirBinaire(registre[S]);
        for (int i=0;i<16;i++){
            if (*(p1+i)!='1' || *(p2+i)!='1'){
                *(p3+i)='0';
            }
            else{
                *(p3+i)='1';
            }
        }
        *(p3+16)='\0';
        registre[dest]=strtol(p3,NULL,2);
    }
    bitZ(registre[dest]);
    bitN(registre[dest]);
    if (dest==0){
        registre[dest]=0;
    }
}

void or(int dest,int reg2, int imme, int S){ //on effectue l'operation logique or entre 2 valeur et le fait de la facon similaire que add
    char *p1=ConvertirBinaire(registre[reg2]);
    char *p3=malloc(sizeof(char)*17);
    if (imme==1){
        char *p2=ConvertirBinaire(S);
        for (int i=0;i<16;i++){
            if (*(p1+i)=='1' || *(p2+i)=='1'){
                *(p3+i)='1';
                }
            else{
                *(p3+i)='0';
                }
        }
            *(p3+16)='\0';
            registre[dest]=strtol(p3,NULL,2);
        }
    else{
        char *p2=ConvertirBinaire(registre[S]);
        for (int i=0;i<16;i++){
            if (*(p1+i)=='1' || *(p2+i)=='1'){
                *(p3+i)='1';
                }
            else{
                *(p3+i)='0';
                }
        }
        *(p3+16)='\0';
        registre[dest]=strtol(p3,NULL,2);
    }
    bitZ(registre[dest]);
    bitN(registre[dest]);
    if (dest==0){
        registre[dest]=0;
    }
}


void xor(int dest,int reg2, int imme, int S){ //on effectue l'operation logique xor entre 2 valeur et le fait de la facon similaire que or
    char *p1=ConvertirBinaire(registre[reg2]);
    char *p3=malloc(sizeof(char)*17);
    if (imme==1){
        char *p2=ConvertirBinaire(S);
        for (int i=0;i<16;i++){
            if ((*(p1+i)=='1' && *(p2+i)=='1')||(*(p1+i)=='0' && *(p2+i)=='0')){
                *(p3+i)='0';
            }
            else{
                *(p3+i)='1';
            }
        }
        *(p3+16)='\0';
        registre[dest]=strtol(p3,NULL,2);
    }
    else{
        char *p2=ConvertirBinaire(S);
        for (int i=0;i<16;i++){
            if ((*(p1+i)=='1' && *(p2+i)=='1')||(*(p1+i)=='0' && *(p2+i)=='0')){
                *(p3+i)='0';
            }
            else{
                *(p3+i)='1';
            }
        }
        *(p3+16)='\0';
        registre[dest]=strtol(p3,NULL,2);
    }
    bitZ(registre[dest]);
    bitN(registre[dest]);
    if (dest==0){
        registre[dest]=0;
    }
}

void shl(int dest,int reg2, int imme, int S){ //cette fonction permet de faire des decalages de bit 
    char *p=ConvertirBinaire(registre[reg2]);
        if (imme==1){        // si immme vaut 1 alors on effectue S fois decalages
            if (S>0){        //decalage a gauche
                if (S>16){   // si S superieur a 16 alors on a automatiquement 0
                    registre[dest]=0;
                    etat[1]=0;
                }
                else{              //sinon on effection S fois decalages
                    registre[dest]=registre[reg2]<<S;
                    etat[1]=*(p+S-1)-48;  // et on fait le mise a jour pour le bit C
                }
            }
            if (S<0){   //decalage a droite de la meme facon que le decalage a gauche
                S=S*(-1);
                if (S>16){
                    registre[dest]=0;
                    etat[1]=0;
                }
                else{
                    if (registre[reg2]>=0){ 
                     registre[dest]=registre[reg2]>>S;
                     etat[1]=*(p+16-S)-48;
                    }
                    else{
                        registre[dest]=DecalageDroiteNeg(registre[reg2],S); // le cas particulier ou registre[reg2] est negatif, alors on appelle la fonction DecalageDroiteNeg
                        etat[1]=*(p+16-S)-48; // on fait le mise a jour pour le bit C
                    }
                }
            }
    }
    else{        // de la meme facon mais ici on fait registre<S> fois, avec registre<S> la valeur du registre S
        if (registre[S]>0){
                if (registre[S]>16){
                    registre[dest]=0;
                    etat[1]=0;
                }
                else{
                    registre[dest]=registre[reg2]<<registre[S];
                    etat[1]=*(p+registre[S]-1)-48;
                }
            }
            if (registre[S]<0){
                int inder=registre[S]*(-1);
                if (inder>16){
                    registre[dest]=0;
                    etat[1]=0;
                }
                else{
                     registre[dest]=DecalageDroiteNeg(registre[reg2],inder);
                     etat[1]=*(p+16-inder)-48;
                }
            }
        }
    bitZ(registre[dest]);
    bitN(registre[dest]);
    if (dest==0){
        registre[dest]=0;
    }
}



int stb(int dest,int reg2, int imme, int S){ //cette fonction permet de stock les poidsFaibles de la valeur du registre reg2 dans la memoire
    int PoidsF=PoidsFaible(registre[reg2]);   
    if (imme==1){ // le cas ou imme vaut 1 alors on stock le poidsFaibles de la valeur du registre reg2 dans la memoire registre<dest> + S
        if (verifierMemoire(registre[dest]+S)!=0){
            return -2;
        }
        else{
        memo[registre[dest]+S]=PoidsF;
        }
    }
    else{// sinon on le stock dans la memo registre<dest> + registre<S>
        if (verifierMemoire(registre[dest]+registre[S])!=0){
            return -2;
        }
        else{
            memo[registre[dest]+registre[S]]=PoidsF;
        }
    }
    bitZ(PoidsF);
    bitN(PoidsF);
    return 0;
}

int stw(int dest,int reg2, int imme, int S){//cette fonction permet de stock les poidsFaibles de la valeur du registre reg2 dans la memoire
    // et les poidsFaibles que l'on met dans la memoire suivante
        if (imme==1){  // on fait de la meme facon que dans stb
            if (verifierMemoire(registre[dest]+S)!=0 || verifierMemoire(registre[dest]+S+1)!=0 ){
            return -2;
        }
            else{
            char PoidsF=PoidsFaible(registre[reg2]);
            memo[registre[dest]+S]=PoidsF;
            memo[registre[dest]+S+1]=PoidsFort(registre[reg2]); 
            }
        }
        else{
            if (verifierMemoire(registre[dest]+registre[S])!=0 || verifierMemoire(registre[dest]+registre[S]+1)!=0 ){
            return -2;
        }
            else{
            memo[registre[dest]+registre[S]]=PoidsFaible(registre[reg2]);
            memo[registre[dest]+registre[S]+1]=PoidsFort(registre[reg2]);
            }
        }
        bitZ(PoidsFort(registre[reg2]));
        bitN(PoidsFort(registre[reg2]));
        return 0;
}

int ldb(int dest,int reg2, int imme, int S){// on met la valeur qui est dans la memoire (registre reg2) + S ou dans la memoire (registre reg2 + registre S) 
    // et on le met dans le registre dest
        if (imme==1){
            if (verifierMemoire(registre[reg2]+S)!=0){
            return -2;
            }
            else{
            registre[dest]=memo[registre[reg2]+S];
            }
        }
        else{
            if (verifierMemoire(registre[reg2]+registre[S])!=0){
            return -2;
            }
            else{
            registre[dest]=memo[registre[reg2]+registre[S]];
            }
        }
    bitZ(registre[dest]);
    bitN(registre[dest]);
   if (dest==0){
        registre[dest]=0;
    }
    return 0;
}

int ldw(int dest,int reg2, int imme, int S){ // 
        if (imme==1){
            if (verifierMemoire(registre[reg2]+S)!=0 || verifierMemoire(registre[reg2]+S+1)!=0){
            return -2;
            }
            else{
            registre[dest]=256*memo[registre[reg2]+S+1]+memo[registre[reg2]+S];
            }
        }
        else{
            if (verifierMemoire(registre[reg2]+registre[S])!=0 || verifierMemoire(registre[reg2]+registre[S]+1)!=0){
            return -2;
            }
            else{
            registre[dest]=256*memo[registre[reg2]+registre[S]+1]+memo[registre[reg2]+registre[S]];
            }
        }
    bitZ(registre[dest]);
    bitN(registre[dest]);
    if (dest==0){
        registre[dest]=0;
    }
    return 0;
}

int jmp(int imme, int dest){ // cette fonction renvoie l'adresse du prochain instruction
     if (imme==1){
            return dest/4;
     }
    else{
        return registre[dest]/4;
    }
}
int jzs(int imme, int dest){ // cette fonction renvoie l'adresse du prochain instruction si le bit Z vaut 1 
    if (imme==1){
        if (etat[0]==1){
            return dest/4;
            }
        else{
            return -1;
        }
    }
    else{
        if (etat[0]==1){
            return registre[dest]/4;
        }
        else{
            return -1;
        }
    }
}

int jzc(int imme, int dest){ // cette fonction renvoie l'adresse du prochain instruction si le bit Z vaut 0
    if (imme==1){
        if (etat[0]==0){
            return dest/4;
            }
        else{
            return -1;
        }
    }
    else{
        if (etat[0]==0){
            return registre[dest]/4;
        }
        else{
            return -1;
        }
    }
}

int jcs(int imme, int dest){ // cette fonction renvoie l'adresse du prochain instruction si le bit C vaut 1 
    if (imme==1){
        if (etat[1]==1){
            return dest/4;
            }
        else{
            return -1;
        }
    }
    else{
        if (etat[1]==1){
            return registre[dest]/4;
        }
        else{
            return -1;
        }
    }
}

int jcc(int imme, int dest){ // cette fonction renvoie l'adresse du prochain instruction si le bit C vaut 0 
    if (imme==1){
        if (etat[0]==0){
            return dest/4;
            }
        else{
            return -1;
        }
    }
    else{
        if (etat[0]==0){
            return registre[dest]/4;
        }
        else{
            return -1;
        }

    }
}

int jns(int imme, int dest){ // cette fonction renvoie l'adresse du prochain instruction si le bit N vaut 1 
    if (imme==1){
        if (etat[2]==1){
            return dest/4;
            }
        else{
            return -1;
        }
    }
    else{
        if (etat[2]==1){
            return registre[dest]/4;
        }
        else{
            return -1;
        }
    }
}

int jnc(int imme, int dest){ // cette fonction renvoie l'adresse du prochain instruction si le bit N vaut 0
    if (imme==1){
        if (etat[2]==0){
            return dest/4;
            }
        else{
            return -1;
        }
    }
    else{
        if (etat[2]==0){
            return registre[dest]/4;
        }
        else{
            return -1;
        }
    }
}

int in(int dest){    //Cette fonction permet au utilisateur de saisir une valeur
    float x;
    printf("entrez une valeur\n");
    scanf("%f",&x);
    if (verifieLimite(x)==0 && verifierEntier(x)==0){
        registre[dest]=(int)x;
    }
    else{
        return -1;
    }
    bitZ(registre[dest]);
    bitN(registre[dest]);
    if (dest==0){
        registre[dest]=0;
    }
    return 0;
}

void out(int dest){ // Cette fonction permet afficher une valeur
    printf("%d\n",registre[dest]);
}

void rnd(int dest,int rn,int imme,int s){  // cette fonction permet d'obtenir une valeur aleatoire entre la valeur du registre rn et S-1 si imme vaut 1
    // sinon entre la valeur du registre et la valeur du registre S   -1
    srand(time(NULL));
    if (imme==1){
        registre[dest]=rand()%(s-rn) + rn;
    }
    else{
        registre[dest]=rand()%(registre[s]-rn) + rn;
    }
    bitZ(registre[dest]);
    bitN(registre[dest]);
    if (dest==0){
        registre[dest]=0;
    }
}

