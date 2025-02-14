import subprocess
import sys

def execute_command(script_path, input_path, output_path,i):
    try:
        # Construction de la commande
        command = [
            "gtimeout", "10", "python3", script_path
        ]

        # Ouverture des fichiers d'entrée et de sortie
        with open(input_path, "r") as input_file, open(output_path, "w") as output_file:
            # Exécution de la commande
            result = subprocess.run(command, stdin=input_file, stdout=output_file, stderr=subprocess.PIPE)

        # Vérification des erreurs
        if result.returncode != 0:
            print(f"Erreur lors de l'exécution: {result.stderr.decode('utf-8')}", file=sys.stderr)
        else:
            print("Commande exécutée avec succès : "+str(i))

    except FileNotFoundError as e:
        print(f"Erreur : {e}", file=sys.stderr)
    except Exception as e:
        print(f"Une erreur s'est produite : {e}", file=sys.stderr)

    command = ["pace2024verifier",input_path, output_path]    
        # Exécuter la commande
    result = subprocess.run(command, capture_output=True, text=True)
    resultat=str(result)
    chaine=resultat.split("]")
    vrai_chaine=chaine[2]
    vchaine=vrai_chaine.split()
    print("Nombre de croisements : "+str(vchaine[7]))
    C=False
    if C==True:
        with open("/Users/jules/Desktop/Graph/src/bary.txt","a") as f:
            f.write("fichier "+str(i)+" :  "+str(vchaine[7]+"\n"))
        
if __name__ == "__main__":
    # Exemple d'utilisation
    chemin_script = "/Users/jules/Desktop/Graph/src/main.py"  # Remplacez par le chemin de votre script Python
    chemin_sortie = "/Users/jules/Desktop/Graph/src/solution.sol"  # Remplacez par le chemin du fichier de sortie

    for i in range(1,101):
        chemin_entree = "/Users/jules/Desktop/Graph/real-instances/"+str(i)+".gr"
        execute_command(chemin_script, chemin_entree, chemin_sortie,i)
