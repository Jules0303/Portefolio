import sys
import signal
import sys
import math
import time
import numpy as np
import random
#https://www.optil.io/optilion/help/signals#python3
#Initialisation du programme

class Killer:
  exit_now = False
  def __init__(self):
    signal.signal(signal.SIGINT, self.exit)
    signal.signal(signal.SIGTERM, self.exit)

  #put the boolean to true on SIGTERM
  def exit(self,signum, frame):
    self.exit_now = True
    global t
    t=time.time()

def LireProgramme():
    graphA={}
    graphB={}
    edges = list()
    for line in sys.stdin:
      sline=line.split()
      while (sline[0]!="c" and sline[0]!="p"):
          #Création du dictionnaire de A: [B] et de B:[A]
          if int(sline[0]) not in graphA:
            graphA[int(sline[0])]=list()
          if int(sline[1]) not in graphB:
            graphB[int(sline[1])]=list()
          graphA[int(sline[0])].append(int(sline[1]))
          graphB[int(sline[1])].append(int(sline[0]))
          edges.append((int(sline[0]),int(sline[1])))
          break
    return graphA,graphB,edges

#envoie de la solution
def EnvoyerSolDico(sequence,nb): #Fonction pour envoyer la soluce
  print("sol envoyée : " + str(nb),file=sys.stderr)
  for terme in sequence:
    print(terme)
  if killer.exit_now:
    print("fin, pas assez de temps ", file=sys.stderr)
    t1=time.time()
    print("arret en : "+str(t1-t), file=sys.stderr)
  return

#Compte les croisement d'ue sequence donnée dans le graph donnée
def Compter_Croisement_fast(graph, seqB):
    if killer.exit_now:
        return math.inf
    
    # Créer un dictionnaire pour l'ordre des indices des sommets B dans seqB
    seqB = {b: i for i, b in enumerate(seqB)}
    
    # Construire les groupes d'indices pour chaque sommet A
    groups = []
    for a, b in graph.items():
        group = sorted(set(seqB[v] for v in b))  # Supprimer les doublons
        groups.append(group)
        if killer.exit_now: return math.inf
    
    # Concaténer les groupes avec des marqueurs de groupe
    concatenated_indices = []
    for group_id, group in enumerate(groups):
        concatenated_indices.extend((index, group_id) for index in group) #Liste des arretes a,b avec b la position du sommet b
        if killer.exit_now: return math.inf

    # Compter les inversions globalement entre groupes
    def compte_inversions(arr):  # Méthode du Merge Sort
        def merge_compte(arr, temp_arr, left, right):
            if left == right:
                return 0
            mid = (left + right) // 2
            inv_count = merge_compte(arr, temp_arr, left, mid)
            inv_count += merge_compte(arr, temp_arr, mid + 1, right)
            inv_count += merge_and_count(arr, temp_arr, left, mid, right)
            if killer.exit_now: return math.inf
            return inv_count

        def merge_and_count(arr, temp_arr, left, mid, right):
            if killer.exit_now: return 0
            
            i = left    # Début de la sous-liste gauche
            j = mid + 1 # Début de la sous-liste droite
            k = left    # Indice pour la liste temporaire
            inv_count = 0

            while i <= mid and j <= right:
                if killer.exit_now: return 0
                if arr[i][0] <= arr[j][0]:  # Comparer les indices
                    temp_arr[k] = arr[i]
                    i += 1
                else:
                    # Compter uniquement si les groupes et indices diffèrent
                    if arr[i][0] != arr[j][0] and arr[i][1] != arr[j][1]:
                        inv_count += (mid - i + 1)
                    temp_arr[k] = arr[j]
                    j += 1
                k += 1

            while i <= mid:
                if killer.exit_now: return 0
                temp_arr[k] = arr[i]
                i += 1
                k += 1
            while j <= right:
                if killer.exit_now: return 0
                temp_arr[k] = arr[j]
                j += 1
                k += 1
            for i in range(left, right + 1):
                if killer.exit_now: return 0
                arr[i] = temp_arr[i]
            return inv_count
        
        temp_arr = [None] * len(arr)
        return merge_compte(arr, temp_arr, 0, len(arr) - 1)
    return compte_inversions(concatenated_indices)

def cc2(graph): # En gros version iterative de dfs car l'autre en recursive sur gros graph
    if killer.exit_now:return [],[]
    CC_GB = []  # Liste des composantes connexes trouvées
    visited_a = set()
    visited_b = set()  
    listeB=list(graphB.keys())
    
    def DFSS(start, graph_a, graph_b):
        component = {} 
        queue = [start] 
        while queue:
            node = queue.pop(0) 
            if node in graph_b and node not in visited_b:
                visited_b.add(node)
                neighbors = graph_b[node]
                component[node] = list(neighbors) 
                for neighbor in neighbors:
                    if killer.exit_now:return component
                    if neighbor not in visited_a:
                      visited_a.add(neighbor)
                      queue.extend(graph_a.get(neighbor, []))
        return component

    # Parcourt les nœuds de lb (ensemble B)
    for node in listeB:
        if killer.exit_now:
            return [],[]
        if node not in visited_b:  # Si non visité, explore la composante connexe
            component = DFSS(node, graph, graphB)
            if component:  # Ajoute la composante non vide
                CC_GB.append(component)
            if killer.exit_now:
              return [],[]
    sequenceB=[]
    for elem in CC_GB:
      sequenceB.append([b for b in elem.keys()])
    return CC_GB,sequenceB

#Methode du barycentre
def barycentre(sequence,graphB):
  if killer.exit_now:return
  Bary={}
  for b in sequence: #pour tous les sommet b
    a_connecter_B=graphB[b] #Tous les sommet a auxquel est connecter le sommet b
    #On calcul la moyenne de ces sommet a
    Bary[b]= sum(a_connecter_B)/len(a_connecter_B)
    if killer.exit_now:
      return []
  return sorted(sequence,key=lambda b:Bary[b]) #retourn la liste b barycentré

#Methode mediane, en gros c'est un peu pareil
def Mediane(sequence,graphB):
  if killer.exit_now:return
  mediane={}
  for b in sequence: 
    a_connecter_B=sorted(graphB[b]) #Tous les sommet a auxquel est connecter le sommet b
    mediane[b]=np.median(a_connecter_B)
    if killer.exit_now:return 
  return sorted(sequence,key=lambda b:mediane[b])#retourn la liste b trie par la mediane

#Methode ou en fait la moyenne des deux
def Moyenne_Mediane_Bary(seqMediane,seqBary,graphB):
    if killer.exit_now:return
    Temp_seqMCC = {b: i for i, b in enumerate(seqMediane)}  #On fait la moyenne en mediane et bary en CC voir si ca peut etre bien
    if killer.exit_now: return
    Temp_seqBCC={b: i for i, b in enumerate(seqBary)}
    if killer.exit_now:return
    Mediane_baryCC={b:(Temp_seqBCC[b]+Temp_seqMCC[b]) for b in list(graphB.keys())}
    if killer.exit_now:return
    Mediane_baryCC=sorted(original_list,key=lambda b:Mediane_baryCC[b])
    return Mediane_baryCC

def graphB_Vers_A(graphB):
  graphA={}
  for b in graphB:
    for a in graphB[b]:
      if a not in graphA:
        graphA[a]=[]
      graphA[a].append(b)
      if killer.exit_now:return
  return graphB

#Check si la nouvelle solution est meilleur qu'avant
def Check_Sol(old_sequence,sequence,graphA,nb_actuel):
    if killer.exit_now:return old_sequence,nb_actuel
    nb_new=Compter_Croisement_fast(graphA,sequence)
    if nb_new<nb_actuel:
        return sequence,nb_new
    else: return old_sequence,nb_actuel

def Tri_GraphCC(graph_CC):
    if killer.exit_now:
        return []
    Poids=[]
    for CC in graph_CC:
        CC_A=graphB_Vers_A(CC)
        if killer.exit_now:return []
        CC_AClé=[a for a in CC_A]
        Moyenne=sum(CC_AClé)/len(CC_AClé)
        Poids.append(Moyenne)
    if killer.exit_now:
        return []
    print(Poids,file=sys.stderr)
    graph_CC=[x for _, x in sorted(zip(Poids, graph_CC), key=lambda pair: pair[0])]
    return graph_CC

def Calcul_CC(graph_CC,sequence_CC,sequence_old,nb_actuel):
  if killer.exit_now:return sequence_old,nb_actuel
  seqMediane=[]
  seqBary=[]
  n=len(graph_CC)
  for i in range(n): #on applique les algo sur chaque cc et on colle les sequences gagne
    seqMediane+=Mediane(sequence_CC[i],graph_CC[i])
    if killer.exit_now:return sequence_old,nb_actuel
  sequence_old,nb_actuel=Check_Sol(sequence_old,seqMediane,graphA,nb_actuel)
  if killer.exit_now:return sequence_old,nb_actuel
  #On test bary
  for i in range(n): #on applique les algo sur chaque cc et on colle les sequence gagné
    seqBary+=barycentre(sequence_CC[i],graph_CC[i])
    if killer.exit_now:return sequence_old,nb_actuel
  sequence_old,nb_actuel=Check_Sol(sequence_old,seqBary,graphA,nb_actuel)
  if killer.exit_now:return sequence_old,nb_actuel
  
  #On test Moyenne des 2
  seqMediane_bary=Moyenne_Mediane_Bary(seqMediane,seqBary,graphB)
  if killer.exit_now:return sequence_old,nb_actuel
  sequence_old,nb_actuel=Check_Sol(sequence_old,seqMediane_bary,graphA,nb_actuel)
  return sequence_old,nb_actuel

def AlgoSurCC(graphA,nb_actuel,sequence_old):
  if killer.exit_now:return sequence_old,nb_actuel
  graph_CC,sequence_CC=cc2(graphA)
  if killer.exit_now:return sequence_old,nb_actuel
  n=len(graph_CC)
  print("nb de cc :"+str(n),file=sys.stderr)
  #On test Mediane
  sequence_old,nb_actuel=Calcul_CC(graph_CC,sequence_CC,sequence_old,nb_actuel)
  if killer.exit_now:return sequence_old,nb_actuel
  D=False
  if D==True:
    graph_CC=Tri_GraphCC(graph_CC)
    if killer.exit_now:return sequence_old,nb_actuel
    sequence_CC=[]
    for elem in graph_CC:
            sequence_CC.append([b for b in elem.keys()])
    sequence_old,nb_actuel=Calcul_CC(graph_CC,sequence_CC,sequence_old,nb_actuel)
  return sequence_old,nb_actuel

def Recherche_local(sequence_actuel,nb_old, graph): #On echange les sommets deux à deux et si pas meilleur on fait la methode du pivot
    while True:
        Meilleur = False
        for i in range(len(sequence_actuel)):
            if killer.exit_now:break
            for j in range(i + 1, len(sequence_actuel)):
                if killer.exit_now:break
                New_seq = sequence_actuel[:]
                New_seq[i], New_seq[j] = New_seq[j], New_seq[i]
                nbA=nb_old
                sequence_actuel,nb_old=Check_Sol(sequence_actuel,New_seq,graph,nb_old)
                if killer.exit_now:break
                if nb_old<nbA:
                  Meilleur=True
                else : New_seq[i], New_seq[j] = New_seq[j], New_seq[i]
            if Meilleur:
                break
        if killer.exit_now:break
        # si aucune amélioration par le voisin, tenter une permutation aléatoire
        if not Meilleur:
            if killer.exit_now:break
            i, j = random.sample(range(len(sequence_actuel)), 2)
            New_seq = sequence_actuel[:]
            New_seq[i], New_seq[j] = New_seq[j], New_seq[i]
            sequence_actuel,nb_old=Check_Sol(New_seq,sequence_actuel,graph,nb_old)
            if killer.exit_now:break
            if nb_old<nbA: 
              Meilleur=True
    return sequence_actuel,nb_old

#Le Main : 
while True:
    #Initialisation
    killer = Killer()
    graphA,graphB,edges=LireProgramme()
    original_list=list(graphB.keys())
    Best_seq=original_list
    Best_nb=math.inf
    #On commence les heuristic
    Seq_Mediane=Mediane(original_list,graphB)
    if killer.exit_now:break
    Best_seq,Best_nb=Check_Sol(Best_seq,Seq_Mediane,graphA,Best_nb)
    if killer.exit_now:break
    Seq_Bary=barycentre(original_list,graphB)
    if killer.exit_now:break
    Best_seq,Best_nb=Check_Sol(Best_seq,Seq_Bary,graphA,Best_nb)
    if killer.exit_now:break
    Seq_bary_Mediane=Moyenne_Mediane_Bary(Seq_Mediane,Seq_Bary,graphB)
    if killer.exit_now:break
    Best_seq,Best_nb=Check_Sol(Best_seq,Seq_bary_Mediane,graphA,Best_nb)
    if killer.exit_now:break
    Best_seq,Best_nb=AlgoSurCC(graphA,Best_nb,Best_seq)
    if killer.exit_now:break
    Best_seq,Best_nb=Check_Sol(Best_seq,sorted(original_list),graphA,Best_nb)
    if killer.exit_now:break
    Best_seq,Best_nb=Recherche_local(Best_seq,Best_nb,graphA)
    #Fin du programme
    break
    
EnvoyerSolDico(Best_seq,Best_nb)