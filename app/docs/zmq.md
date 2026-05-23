# Mémento : ZeroMQ et C Avancé

## 1. Concepts Fondamentaux

### Le Pointeur Générique (`void *`) et l'Encapsulation
* **Qu'est-ce que c'est ?** C'est une adresse mémoire brute sans "plan de construction" (le compilateur ne sait pas ce qu'il y a dedans).
* **Pourquoi ZMQ l'utilise ?** Pour créer une **structure opaque**. ZMQ te donne un "ticket" (`void *`) pour accéder à ses services sans te laisser voir ni modifier ses variables internes (ce qui évite de faire crasher la bibliothèque).

### Le "Contexte" ZMQ
* **Définition :** C'est l'environnement global de ton application réseau. Il contient la piscine de threads en arrière-plan et garde la trace de toutes tes sockets.
* **Le nom de la variable :** Le mot `context` n'est pas obligatoire en C (tu pourrais l'appeler `moteur`), mais c'est la **convention** universelle pour plus de lisibilité.

---

## 2. Bonnes Pratiques en C

### `size_t` vs `int`
* **`size_t`** est un entier non signé (toujours positif) spécialement conçu pour représenter des tailles en mémoire. Un tableau ne peut pas avoir une taille négative.
* L'ajout de **`static const`** permet de figer la valeur (`const`) et de ne créer la variable qu'une seule fois en mémoire au lieu de la recréer à chaque tour de boucle (`static`).

### `assert` vs `if / else`
* **`assert(condition)` :** Outil de **débogage**. Si la condition est fausse, le programme panique, affiche la ligne de l'erreur et s'arrête immédiatement. Le compilateur supprime les `assert` dans la version finale (Release) pour alléger le programme.
* **`if / else` :** Outil de **logique**. Utilisé pour gérer les erreurs prévisibles (ex: un mot de passe incorrect) dans le produit fini.

---

## 3. Les Fonctions Réseau ZeroMQ

### L'ouverture du serveur : `zmq_bind`
* **Rôle :** Attacher une socket à une adresse locale pour écouter.
* **Exemple :** `zmq_bind(socket, "tcp://*:5555");` 
  * `tcp://` : Le protocole.
  * `*` : Toutes les interfaces réseau de la machine.
  * `5555` : Le numéro du port réservé sur le système d'exploitation.
* **Attention :** La socket est le pointeur (`void *`). La fonction `zmq_bind` renvoie simplement un `int` qui est le **code de statut** (`0` = succès, `-1` = échec).

### La Communication : `zmq_send` et `zmq_recv`
Elles s'utilisent en miroir et prennent 4 arguments :

1. **`void *socket`** : La porte d'entrée (recv) ou de sortie (send).
2. **`void *buf` / `const void *buf`** : Le pointeur vers les données. (C'est `const` pour l'envoi car ZMQ promet de ne pas modifier ton message, mais pas pour la réception car ZMQ doit écrire dans ta variable).
3. **`size_t len`** : 
   * *Pour send :* La taille exacte en octets du message à envoyer.
   * *Pour recv :* La taille maximale de la "boîte" de réception (pour éviter un dépassement de mémoire ou *Buffer Overflow*).
4. **`int flags`** : Les options. 
   * `0` = Mode bloquant (le programme s'arrête et attend). 
   * `ZMQ_DONTWAIT` = Mode non-bloquant (n'attend pas si le réseau n'est pas prêt).