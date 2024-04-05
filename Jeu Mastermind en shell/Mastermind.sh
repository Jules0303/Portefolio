#!/bin/bash

#n le nombre de chiffre du code
n=5
#bla valeur max de chaque chiffre
b=5

nombre_essaie=10
#generation du code avec n nombre aléatoire en 1 et b

codesecret(){
    local code=""
    for (( i=0; i<n; i++ )); do
        chiffre=$((1+ RANDOM % b))  # genère un chiffre aléatoire entre 1 et b
        code="$code$chiffre"  
    done
    echo "$code"
}

#recupération du nombre de l'utilisateur et test qu'il soit comforme
recupNombre(){
    local valeur
    valeur=-1
    local valmax=0
    local valmin=0

    for ((i=0; i<n;i++)) do
        valmax=$((valmax + b*10**i))
        valmin=$((valmin + 10**i))
    done


    while (( valeur < valmin || valeur > valmax )); 
        do
            read -r -p "Veuillez entrer un nombre de $n chiffres entre 1 et $b inclu:  " valeur
            if [ ${#valeur} -ne $n ]; then
                valeur=-1
            fi
        done
        echo "$valeur"
}

#renvoie le nombre de chiffes bien placé et mal placé
TestSequence(){

    local code=$2
    local nombre=$1
    local chiffres_identiques=0
    local chiffres_malplace=0
    local code_check=()

    # Vérification des chiffres corrects et mal placés
    for (( i = 0; i < n; i++ )); do
        if [[ "${nombre:$i:1}" == "${code:$i:1}" ]]; then
            ((chiffres_identiques++))
        else
            code_check[i]=${code:$i:1} #si un chiffre n'est pas correste on le stock dans un tableau
        fi
    done

    for (( i = 0; i < n; i++ )); do
        if [[ "${nombre:$i:1}" != "${code:$i:1}" ]]; then   #si un chiffre est malplace on verifie qu'il est dans ce tableau et on le supprime ensuite du tzbleau pour eviter les doublons
            for (( j = 0; j < ${#code_check[@]}; j++ )); do
                if [[ "${nombre:$i:1}" == "${code_check[$j]}" ]]; then
                    ((chiffres_malplace++))
                    code_check[j]=""
                    break
                fi
            done
        fi
    done

    echo "Chiffres corrects: $chiffres_identiques, Chiffres mal placés: $chiffres_malplace"
}



#programme principal que l'on va appeler pour chaque partie
jeu(){



    code=$(codesecret)
    #echo "$code"
    for (( i=0 ; i< nombre_essaie ; i++)); do 
        echo "Tour numéro $((i+1))"
        for (( j=0 ; j< nombre_joueur ; j++)); do
            echo "c'est au joueur n°$((j+1)) de jouer :"
            nombre=$(recupNombre)
            resultat=$(TestSequence "$nombre" "$code")
            echo "$resultat"

            if [ "$nombre" == "$code" ]; then
                echo "Félicitation au joueur n°$((j+1)) qui a trouvé en $((i+1)) coups"
                return
            fi
            
        done
    done

    echo "Partie Perdu"

}


#initialisation du programme
echo "Bienvenue dans le mastermind"

reponse="a"

#On demande si on joue seul
while true; do
        read -r -p "Voulez-vous jouer par groupe 'y' (oui) 'n' (non) ?" reponse
        if [[ "$reponse" = "y" || "$reponse" = "n" ]]; then
            break
        fi
done

#On recupere le nombre de joueur
if [[ "$reponse" = "y" ]]; then
    nombre_joueur=0
    while true; do
            read -r -p"Veuillez entrer le nombre de participant : " nombre_joueur
            if [[ $nombre_joueur -ge 1 ]]; then
                break
            fi
    done
else
    nombre_joueur=1
fi

reponse2='y'
while True; do #boucle infini pour pouvoir rejouer
jeu
read -r -p "Appuyer sur y pour rejouer" reponse2
if [[ "$reponse2" != "y" ]]; then
   echo "arrêt du jeu"
   break
fi

done