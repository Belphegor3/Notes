# Philo

Le projet philosophers veut que nous fassions manger simultanement des philosophes autour d une table ronde.  
Le probleme etant cette notion de "simultaner", qui va donc introduire la notion de **thread**.  
Il vous faudra aussi rajouter un flag de compilation que ca compile: *-pthread*  

## Table des matieres

- [definition thread](#que-sont-les-thread)
- [data race](#data-race)
- [vol de fourchettes](#el-famoso-vol-de-fourchette)
- [eviter les problemes](#comment-eviter-tout-ces-problemes)
- [comprehension du sujet](#comprehension-du-sujet)
- [creation de thread](#creation-de-thread)
- [stopper un thread](#stopper-un-thread)
- [quand stopper un thread](#quand-stopper-un-thread)
- [comment stopper un thread en cours d utilisation](#comment-stopper-un-thread-en-cours-d-utilisation)

## Que sont les thread?

Pour definir la notion de thread, imaginons que nous sommes dans un restaurant, qui comporte une cuisine (un processus).  
Si on n a qu un seul employe (qui serait donc le main) qui fait tout, tout sera donc tres lent et on n aura aucun client.  
Pour palier a ce probleme, on veut plusieurs employes (qui representent donc les thread, tout en gardant un manager qui gere tout qui represente le main), un pour les commades, un pour la cuisine et un qui sert.  
Evidemment on peut avoir bien plus d employes, mais c pour bien illustre que, juste avec ces 3 la, on va beaucoup plus vite et ce sont eux les thread parce qu ils travaillent en meme temps.  

## Les problemes associes aux threads?

### Data race

Les data race sont les plus courant et les plus simples des problemes a corriger.  
Imaginons qu on ait 2 employes qui prennent les commandes.  
Maintenant les 2 prennent des commandes differentes a differentes tables et arrivent pour ecrire en meme temps sur le meme tableau pour informer les cuisiniers de ce qu ils doivent faire.  
Les ecritures vont se melanger, ca devient illisibles et on ne sait plus trop qui a commander quoi.  
Nos ordinateurs eux, vont a peu pres reussir a bien lire la commande mais quand on va utiliser valgrind ou fsanitize, ils vont crier a l aide.  
On pourrait aussi avoir le probleme d un cuisinier qui lit une commande pendant qu elle est ecrite, sauf qu il est tres presse donc il ne prend pas le temps de tout lire et au final, il pourrait faire le mauvais plat.  

### Deadlock

Les deadlock sont plus specifiques, c est quand 2 threads attendent chacun que l autre libere une ressource.  

Par exemple un cuisinier A prend le pain a burger et attend la viande alors que cuisinier B a pris la viande mais attend le pain a burger.  
Ils vont tous les 2 attendendre indefiniment.  

### El famoso vol de fourchette

Ce probleme de vol de fourchette typique du projet philosophers est en fait du a des threads qui prennent trop souvent une ressource.  

C est en gros un cuisinier trop rapide qui prend tout le temps la plaque de cuisson et ne la laisse jamais a son collegue qui attend donc indefiniment.  
Il faudra potentiellement laisser le temps a l ordi de souffler avec des possibles ajustements a base de petits **usleep**.  

Malheureusement, nous sommes limites dans les fonctions autorisees et donc ce projet ne reflete pas trop la realite. On devrait utiliser des variables de conditions **pthread_cond_t** mais c est interdit donc on ne va pas s attarder dessus.  

## Comment eviter tout ces problemes?

Pour eviter toutes ces erreurs recurrentes, on va devoir faire en sorte que tous les employes ne puissent acceder que 1 par 1 a tout ce qui est specifique, donc au tableau pour lire les commandes (on ne veut pas ecrire en meme temps ou lire et en meme temps ecrire), utiliser les ustensiels (on ne voudrait pas mettre 2 plats differents dans le four)...  

Pour cela on va introduire la notion de **pthread_mutex_lock**. Ca va nous permettre de mettre de restreindre les actions pour que si un serveur ecrit sur le tableau, ses collegues doivent attendre qu il ait finit pour le lire ou ecrire a leur tour par exemple.  

**Mais il faut bien faire attention a l ordre de ces restrictions puisque c est celle ci meme qui peuvent provoquer des deadlock.**  

Qui dit lock, dit unlock ou plutot **pthread_mutex_unlock** qui permet donc de retirer cette restriction et autorise le prochain a interagir.  
Il va donc falloir faire attention a l ordre dans lequel on lock/unlock puisque qu on peut tres bien lock la cuisine puis lock les toilettes par exemple. Par contre si on veut sortir de la cuisine dans ce cas la, il va falloir unlock les toilettes avant puisqu on est restreint aux toilettes tant qu on les unlock pas.

## Comprehension du sujet

Le projet veut que nos philosophes mangent puis dorment puis pensent. Que sont ces etats:

- manger signifie prendre une fourchette a sa droite et a sa gauche, dans l ordre qu on veut mais 1 par 1. Une fois qu on a les 2 on peut manger et le repas est fini apres avoir passe **time_to_eat** temps a manger.
- dormir signifie faire une sieste directement apres avoir fini de manger, sieste qui se finit apres **time_to_sleep**.
- penser signifie juste qu on est pas entrain de manger ou de dormir, on a finit notre sieste mais ce n est pas encore l heure de manger mais ce temps peut tres bien etre 0.
- mort signifie qu un philosophe n a pas reussi a manger avant que **time_to_die** ne se soit passe entre son dernier repas et son repas d apres (le timeur commencant donc des qu on a nos 2 fourchettes)

On peut aussi specifie un nombre de repas (disons **N**) que chaque philosophe doit manger avant de finir la simulation. Ce qui signifie juste que tous les philosophes doivent manger **N** fois avant d arreter le programme.

## Commencer ou arreter le programme

On vient de parler d arreter le programme, mais comment on le commence et comment on l arrete?  
C est un programme normal avec son main, main etant un thread, on pourrait le voir comme le manager qui peut engager du personnel (qui sont d autres threads) mais qui n en a pas besoin dans la plupart de nos petits programmes.


### Creation de thread

On va utiliser la fonction **pthread_create** mais je vous laisse regarder le man, rien de bien complique puisqu on met a NULL le deuxieme argument chiant.  
Cette fonction permet simplement de lier un thread a la tache qu il va devoir realiser. Cette tache etant communement appeler la routine.  
Dans notre exemple du restaurant on voit bien qu il y a plusieurs taches differentes a realiser donc plusieurs routines differentes. Dans philosophers, pas besoin, on aura une seule routine puisque tout le monde fait la meme chose mais on peut evidemment creer autant de routines differentes que de threads.  
Quand un thread (= un employe) a finit sa tache (= thread) on doit donc le stopper

### Stopper un thread

Il existe plusieurs facons de stopper un thread mais on est limite a la seule utilisation de **pthread_join** qui est le comportement normal de fin.  
Si on a un **pthread_create**, il faut son **pthread_join** associe.  
Il n y a pas vraiment grand chose a dire, juste un peu de man et ca ira.  
**pthread_detach** ne sert pas directement a arreter un thread alors ne perdez pas trop de temps la dessus, ca pourrait nous servir mais c est chiant.  

### Quand stopper un thread?

Il existe 2 facons pour que notre programme s arrete:
1. Soit un philo meurt
2. Soit tous les philos ont mange au minimum le nombre de repas en argument

Il va donc falloir check si un philo meurt et pour cela on peut tout simplement se faire une fonction qui verifie en boucle (tous les certains ou non, on a pas de contrainte d utilisation du GPU) si un des philo est mort. Cette fonction se fait dans notre thread principal, inutile de creer un thread pour cela sauf si vous voulez que votre manager ne fasse vraiment rien.  
Il faut donc iterer sur tous les philos en verifiant que le temps entre 2 repas ne depasse pas le **time_to_die** et quand on a finit tous les philos, on recommance.

### Comment stopper un thread en cours d utilisation?

Je vous vois venir avec toutes les protections partout mais ce n est pas possible avec ce qu on nous autorise.  
En revanche, le sujet precise bien que la simulation s arrete a la mort d un philo donc si j ai un **time_to_die** a 410ms et un **time_to_eat** a 2 000 000ms on doit bien mourir a au moins 410 et max a 420ms et on revient immediatement au prompt, on attend pas 1 999 590ms avant de revenir au prompt.  
Pour cela au lieu de faire directement des **usleep(2000000)**[^1], on va faire une fonction qui va faire pleins de **usleep** jusqu a atteindre les 2 000 000 auquel cas, on pourra bel et bien verifier si on a un mort a chaque mini **usleep** et arreter la simulation pendant ces fameuses 1999.6s.

## Conclusion

Rien de bien complique mais necessite beaucoup de rigueur pour eviter des problemes du genre:
1. Deadlock avec un cuisinier A qui lock viande puis lock pain pour faire un burger avant de les unlock et un cuisinier B qui lock pain puis lock viande pour faire un burger avant de les unlock. Comme les 2 attendent que l autre lache un ingredient bah ils attendent indefiniment.  

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t viande, pain;

void* employeA(void* arg) {
    pthread_mutex_lock(&viande);
    sleep(1); // Simule le temps d'action
    pthread_mutex_lock(&pain); // Attend le pain (bloqué !)
    printf("Employé A fait un burger\n");
    pthread_mutex_unlock(&pain);
    pthread_mutex_unlock(&viande);
    return NULL;
}

void* employeB(void* arg) {
    pthread_mutex_lock(&pain);
    sleep(1);
    pthread_mutex_lock(&viande); // Attend la viande (bloqué !)
    printf("Employé B fait un burger\n");
    pthread_mutex_unlock(&viande);
    pthread_mutex_unlock(&pain);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&viande, NULL);
    pthread_mutex_init(&pain, NULL);

    pthread_create(&thread1, NULL, employeA, NULL);
    pthread_create(&thread2, NULL, employeB, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Service terminé !\n");
    return 0;
}
```

2. Data race avec des serveurs qui notent sur le meme tableau, en meme temps et on pourrait meme etendre ca au manager (= main) qui essaye de le lire

```c
#include <pthread.h>
#include <stdio.h>

int commandes = 0;  // Nombre de commandes

void* prendre_commande(void* arg) {
    for (int i = 0; i < 100000; i++) {
        commandes++;  // Problème ici : accès non sécurisé
    }
    return NULL;
}

int main() {
    pthread_t employe1, employe2;
    pthread_create(&employe1, NULL, prendre_commande, NULL);
    pthread_create(&employe2, NULL, prendre_commande, NULL);
    
    pthread_join(employe1, NULL);
    pthread_join(employe2, NULL);
    
    printf("Total commandes enregistrées : %d\n", commandes); // Ca devrait pas faire les 100000 attendues !
    return 0;
}
```

3. Eviter les vols de fourchettes avec un petit **usleep(1000)** comme **time_to_think** minimum pour laisser le temps a l ordi de calculer

4. **La prise de fourchettes doit se faire differements entre tous les philosophers pour eviter des problemes de valgrind/fsanitize. Le plus simple etant de faire en sorte que tous les philos paires prennent la fourchette de droite puis celle de gauche et les philos impaires prennent la fourchette de gauche puis celle de droite ou l inverse evidemment hein. Je ne parle pas du nombre de philos ici, qu on ait 4 philos ou 5 philos ne changent rien, les paires prennent d puis g et les impaires prennent g puis d.**

[^1]: a noter que la fonction usleep est en microsecondes (µs) alors que sleep est en secondes (s) mais dans le sujet on ne parle qu en microsecondes (ms) et **1s == 1 000ms == 1 000 000µs**.