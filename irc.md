# NOTES EN VRAC SUR IRC

IRC Internet Relay Chat est un protocol simpliste basé sur du texte qui met en relation un client et un server

Le projet nous apprend a mettre en oeuvre une architecture en c++ structuré et l utilisation d un gestionnaire de fd

GESTIONNAIRE DE FD:
Ca sert a gerer simultanement plusieurs fd pour des actions de lecture et d ecriture

## Table des matieres

- [SELECT](#select)
- [SOCKET](#socket)
- [RFC](#rfc)
- [CHANNEL](#channel)
- [USER](#user)
- [OPERATOR](#operateur)
- [FONCTIONNEMENT](#fonctionnement)
- [IRSSI](#irssi)
- [COMMANDE_UTILISATEUR](#commandes-de-base-pour-user)
- [COMMANDE_MSG](#messages)
- [COMMANDE_CHANNEL](#channel-operations)

##                    SELECT

Select est le gestionnaire de fd qui va nous interesser.  
int select(int nfds, fd_set *_Nullable restrict readfds, fd_set *_Nullable restrict writefds, fd_set *_Nullable restrict exceptfds, struct timeval *_Nullable restrict timeout);  

       nfds correspond au fd du server qui lui meme sera tjs 4 puisque la fonction socket utilise le premier fd disponible qui est le 3 et qu il faut tjs ajouter 1
       readfds correspond au groupe des fd sur lesquels on peut executer des actions d ecriture donc lire de donnees sans les changer
       writefds correspond au groupe des fd sur lesquels on peut executer des actions de lecture donc ajouter des donnees 
       exceptfds porte bien son nom et balec donc NULL
       timeout osef aussi c pour mettre des limites de temps donc NULL

On a donc 2 listes de fd actif qu il faut gerer et il ne faut en oublier aucun des deux sinon problemes.  
Pour gerer ces listes de fd, il y a 4 fonctions importantes:  
- FD_ZERO qui sert a enlever tous les fd d une liste de fd actif
- FD_CLR qui sert a supprimer un fd d une liste de fd actif
- FD_SET qui sert a ajouter un fd a une liste de fd actif
- FD_ISSET qui sert a verifier si un fd appartient a une liste de fd actif 
	   
	   void FD_ZERO(fd_set *set);
	   void FD_CLR(int fd, fd_set *set);
	   int  FD_SET(int fd, fd_set *set);
	   void FD_ISSET(int fd, fd_set *set);

Donc par exemple quand on supprime un client, on doit donc enlever son fd de la liste des fd actif d ecriture et de lecture.  


##                    SOCKET                     

La fonction socket fonctionne comme un open pour un fd random sauf que socket prend le premier fd dispo  
int socket(int domain, int type, int protocol);  

		domain correspond a une specification de domaine de communication de <sys/socket.h> mais ce qui nous interesse c est le sujet donc IPv4 
		ou IPv6 donc seulement AF_INET et AF_INET6 donc balec en gros
		type correspond a des types different qui commencent par SOCK_ et on peut surtout porter attention a SOCK_NONBLOCK a cause du sujet qui creer une liste
		d attente des actions a realiser sur les differents fd actifs
		protocol correspond bah a des protocoles xD par exemple IPPROTO_TCP est un protocole de controle de transmission (TCP) qui cree une socket orientee connexion 
		(dans le cadre IPv4 tout du moins) qui offre un flux de donnees fiables et bidirectionnel entre les parties. Mais sinon on met 0 et ca choisit le mieux adapte normalement

Avec la fonction socket on va utiliser plusieurs fonctions:  
- setsockopt qui permet de changer les parametres de cette socket
- bind qui permet d associer une addresse IP et un numero de port a la socket
- listen qui est un mode ecoute qui attend donc de pouvoir ajouter des nouveaux fd avec accept
- accept

En gros, ajouter un nouveau client signifie d accept son fd comme etant valide pour ensuite pouvoir read/write (recv/send sont equivalent avec un flag en plus) dessus comme dans un GNL  


##                     RFC  

RFC (Requests For Comments) sont des documents qui contiennent  des spécifications, des normes, des méthodes et des informations liées à divers aspects de l Internet.  
Ceux qui peuvent nous interesser sont les 1459, 2810, 2811, 2812, 2813.  
- Le 1459 est l original qui n est donc pas detaille.  
- Le 2810 nous parle de l architecture general d un server IRC qui nous interesse donc tres peu  
- Le 2811 nous parle de comment gerer les channels qui nous interesse donc a peine  
- Le 2812 nous parle du protocol client a server  
- Le 2813 nous parle du protocol server a server et ne nous interesse donc pas du tout  

On va enormement utiliser le [RFC 2812](https://www.tech-invite.com/y25/tinv-ietf-rfc-2812.html) ou sont specifier les differentes reponses de IRC qui sont de 3 types:  
-	RPL_ reponse dans un cas d utilisation correct mais juste a titre informatif (par exemple RPL 001 nous dit bonjour c est tout ca sert a rien mais certains clients ne se lancent pas sans avoir recu bonjour)
-	ERR_ reponse dans un cas d erreur genre pas assez de parametres (ERR_NEEDMOREPARAMS)
-	et une reponse qui n a pas de nom mais qui va permettre au server d executer des actions, par exemple quand on JOIN un channel sans la reponse un tel a JOIN tel server, les clients ne vont pas comprendre
	qu il a bien JOIN et il peut y avoir conflit entre le server et les clients  

C est aussi dans celui la qu on va trouver le fonctionnement de toutes les methodes qu on doit implementer.  
La, pas de secret il faut juste lire le fonctionnement et les differentes reponses de types RPL et ERR qu il faudra implementer et c est tout bete on fait juste ce qui est demander et encore pas forcement tout  


##                    CHANNEL                    

Il y a bcp de protocoles dans IRC mais on n a pas besoin de les suivre on ne fait pas un vrai server puisque deja il n est relie a aucun autre donc enormement de choses vont etre la pour etre compris du client de reference mais ne sert a rien.  

On a par exemple les noms de channel qui doivent commencer par &,#,+,!,*,@ mais nique sa mere pas besoin de tout ca on peut partir du principe que seul # existe puisque c le plus commun et ne veut rien dire en particulier  
En revanche c est tres important d avoir le # qui permet de distinguer un utilisateur "lfchouch" et un channel "#lfchouch" pour insulter sa mere  

Pas besoin de gerer les wildcard ce qui nous evite de gerer les groupes de channel donc pas besoin d en savoir plus.  

Le sujet demande de faire la fonction MODE avec les options i, t, k, o et l donc la on peut s interesser au [RFC 2811](https://www.tech-invite.com/y25/tinv-ietf-rfc-2811.html)  

**A NE PAS CONFONDRE MODE ET MODE!! IL EXISTE 2 MODES, UN USER ET UN CHANNEL ET ON NE GERE QUE CELUI CHANNEL DONC MODE +i VEUT DIRE INVITEONLY PAS INVISIBLE**  


##                     USER 

Le nom d utilisateur (username) est le nom d utilisateur de l utilisateur sur le système où il exécute son client IRC.  

Le nom réel (realname) est un champ optionnel permettant à l utilisateur de fournir une description plus détaillée de lui-même ou de son compte.  

Le nom d hôte (hostname) est le nom de la machine sur laquelle l utilisateur exécute son client IRC. Mais on ne s occupe pas de l hostname parce qu il est normalement automatiquement enregistre quand on rejoint le server grace a son IP.  


difference entre nickname et username:  
**Nickname (pseudo) :**  

 Le nickname est un identifiant unique utilisé par un utilisateur sur le serveur IRC.  
 Il peut être choisi librement par l utilisateur, mais il doit être unique parmi tous les utilisateurs connectés au serveur.  
 Le nickname est utilisé pour identifier un utilisateur lorsqu il rejoint des canaux, envoie des messages privés, etc.  
 Le nickname peut être visible aux autres utilisateurs sur le serveur.  

**Username (nom d utilisateur) :**  

 Le username est une autre forme d identification utilisée sur le serveur IRC.  
 Il est souvent associé à un compte utilisateur enregistré sur le serveur IRC.  
 Contrairement au nickname, le username n est pas choisi librement par l utilisateur, mais est généralement déterminé par le serveur ou associé au compte de l utilisateur.  
 Le username peut être utilisé pour authentifier l utilisateur lors de la connexion au serveur IRC.  
 Le username n est pas aussi visible que le nickname et n est généralement pas utilisé dans les interactions courantes sur le serveur.  


##                  OPERATEUR

Il y a different degres d operateurs dont les plus communs sont operateurs servers et operateurs channels.  
Il faut bien faire la difference parce que le sujet veut qu on gere des trucs sur les channels mais pas sur le server lui meme.  
Donc devenir operateur server ne sert a rien. Neanmoins, pour le devenir il faut utiliser la commande OPER qui necessite un mot de passe server.  
Techniquement un operateur server a des privileges qui dependent du server, il peut outrepasser ou pas les droits d un operateur channel donc mieux vaut ne pas y toucher du tout.  
Mais en gros un operateur server peut utiliser les commandes KILL, SQUIT, REHASH, DIE, RESTART, CONNECT et aucune de ces fonctions etant utiles...  

On va s interesser uniquement aux operateurs channels qui est un privilege qui s acquiert de 2 facons. Soit en creant un channel on devient par defaut operateur channel.  
Soit en realisant un `MODE +o nom_du_nouvel_operateur` qui est une commande necessitant le privilege operateur channel UwU  


##                FONCTIONNEMENT                

Le server n est pas oblige de traiter un message de plus de 512 caracteres qui est la taille max d un input de client dont les deux caracteres CRLF de fin (\r\n)  
Un message ne se terminant pas par CRLF (\r\n) n est pas interpreter donc un client envoie une requete "PRIVMSG user :salut ca va" le server ne fait rien  
il doit recevoir "PRIVMSG user :salut ca va\r\n"  
Pour se faire, avec ncat, on ne doit pas lancer "nc localhost port" mais "nc -C localhost port" parce que le -C ajoute "\r\n" a la fin de chaque message automatiquement.  

//? Dans l exemple au dessus `PRIVMSG user :salut ca va\r\n` on peut remarquer le ":". Il correspond au dernier argument et signifie que ce dernier argument peut contenir des espaces (a gerer comme on veut hein).  
**par argument j entend que ca fait partie des parametres de la commande**  

**Attention, une commande IRC a cette forme: PREFIX COMMANDE PARAMS**  
Le prefix commence aussi par ":" mais est inutile dans le contexte du projet IRC puisqu on est pas en relation avec d autres servers donc on peut l ignorer mais attention a ne pas crash  
Commande est donc un nom de commande et params les parametres associe a la commande.  

**Si on part sur le client irssi comme client de base, on doit taper par exemple "/msg user yo ca va?" pour ecrire un mp a un utilisateur qui s appelle user, ca c est ce que l utilisateur tape mais le server recoit "PRIVMSG user :yo ca va?\r\n"**  


##                     IRSSI

Irssi est un client qui peut utiliser les servers IRC et le plus commun a utiliser pour faire le projet IRC qui consiste a pouvoir utiliser ncat et un client existant.  
Je n ai pas utilise d autres clients, ne connait meme pas l existence dautres xD Donc je sais pas si seulement irssi est chiant mais j imagine que non.  
Par exemple comme dit au dessus la commande PRIVMSG commence par /msg et pas /privmsg.  
Une commande commence forcement par "/" suivi de msg, notice, join, mode, nick, invite, kick, part, who, topic, quit (user et pass etant inutile, dans irssi, parce que realise tout seul des le debut ainsi que toutes les autres commandes n etant pas obligatoires)  
Ensuite on peut mettre des parametres mais pas besoin de mettre le ":" pour signifier que c le dernier parametre et pas besoin de mettre un "#" au nom des channels quand on veut join ca se rajoute tout seul de base  

irssi possede aussi ses propres fonctions comme /wc qui permet de fermer une fenetre et la difference avec part est que ca enleve forcement la fenetre, on peut tjs avoir la fenetre quand on part, elle devient juste inutilisable.  
Mais irssi possede aussi par exemple une fonction quit de base qui va quitter meme si du coup on a pas coder la fonction QUIT... Bon c est faux puisque ca va leak mais sans check ca on le voit pas....  
ctrl+n permet de se balader entre les fenetres, par exemple entre les mp et l ecran principal  

irssi attend des reponses precises de ce format:  
- Pour les RPL: 					":"nom_du_server + " " + numero_du_RPL + " " + message_du_rpl + "\r\n"; le message_du_rpl pouvant contenir des varibales telles que nickname, channel_name...
- Pour les ERR:						":"nom_du_server + " " + numero_du_ERR + " " + nom_de_la_commande + " " + message_du_err + "\r\n"; pour les erreurs pouvant intervenir dans differentes commandes pour que le server interprete bien la bonne commande comme par exemple ERR_NEEDMOREPARAMS
									":"nom_du_server + " " + numero_du_ERR + " " + message_du_err + "\r\n"; avec le message_du_err pouvant tjs contenir des variables
- Pour les reponses informatives:	":"nickname_du_client"!~"+username_du_client"@"nom_du_server + " " + nom_de_la_commande + " :" + variable + "\r\n"; variable tjs pareil genre channel_name, nickname...

**on remarquera dans "username_du_client"@"nom_du_server" qu il n y a pas d espace entre "client", "@" et "nom". C normal il ne faut pas en mettre, pareil a chaque fois**  

301 secondes d inactivite sur irssi le force a se restart c super chiant...  


#                   COMMANDES                  

 Je parle ici d un strict minimum des commandes a implementer on peut en rajouter pleins si on veut  
 Je ne liste que des RPL et des ERR pas les reponses que j appelle informative et qui sont necessaire au bon fonctionnement de irssi  
 Il n y a pas toutes les RPL/ERR du [RFC 2812](https://www.tech-invite.com/y25/tinv-ietf-rfc-2812.html) parce que certaines ERR/RPL sont inutiles dans le cadre de notre projet  


## COMMANDES DE BASE POUR USER

- [Summary](#table-des-matieres)
- [PASS](#pass)
- [NICK](#nick)
- [USER](#user)
- [QUIT](#quit)

###	PASS 
parametres: <password>  
C est un mot de passe de connection set dans le main et s il est faux on ne peut pas se connecter sur un client  

Erreurs possibles:
- ERR_NEEDMOREPARAMS		461		"<command> :Not enough parameters"
- ERR_ALREADYREGISTRED	462		":Unauthorized command (already registered)"
- ERR_PASSWDMISMATCH		464		":Password incorrect"



###	NICK
parametres: <nickname>  
Permet de donner ou changer le nom du user avec un max de 9 characters.  

Erreurs possibles:
- ERR_NONICKNAMEGIVEN		431		":No nickname given"    	//	remplace donc le ERR_NEEDMOREPARAMS ici  
- ERR_ERRONEUSNICKNAME	432		"<nick> :Erroneous nickname"	//	intervient si on ne respecte pas la convention d ecriture d un nom (je l ai gere comme le RFC mais comme on veut)
- ERR_NICKNAMEINUSE		433		"<nick> :Nickname is already in use"

**par convention d ecriture il faudrait au moins interdire d avoir un nick qui commence par # pour differencier un user d un channel**  



###	USER
parametres: <user><mode><unused><realname>  
Le parametre <user> a une longueur max de 9 characters.  

Cette commande est bizarre, dans irssi les arguments ne sont pas les memes en gros on peut juste ignorer les parametres 2 et 3 (et donc le com en dessous) et juste prendre en compte le 1 et 4  
IRSSI se base sur la version d origine du protocole [RFC 1459](https://www.tech-invite.com/y10/tinv-ietf-rfc-1459.html) mais dans les 2 cas osef du 2 et 3  

Le parametre <unused> est litteralement non utilise qu on doit souvent remplir par un tiret ou du coup un wilcard  

Le parametre <mode> est numerique et permet de set un mode automatiquement a l enregistrement a un server.  
Si le bit 2 est set on passe en mode w.  
Le mode w (wallops) permet que les messages de type wallops soient envoye a tous les utilisateurs avec le mode w actif.  
Il suffit de decaller le nombre de 2 bit et comparer avec 1.  
(ex: 9 >> 2 & 1 = 0 donc le bit 2 n est pas a 1, pour rappel 9 vaut 1*0*01)  
Si le bit 3 est set on passe en mode i.  
Le mode i (invisible) permet qu on ne le voit pas dans la liste des utilisateurs connecte au channel ou au reseau sauf pour les autres utilisateurs qui l ont dans leur liste d ami. Mais le sujet ne
parle pas d amis donc nique.  
  
<realname> peut contenir des espaces

exemple de USER:  
        USER guest 8 * :Bob  
                        On enregistre un user qui a pour username "guest" avec un realname "Bob"  
						8>>2&1=0  
						8>>3&1=1  
						Donc on rajoute MODE+i  
USER guest guest guest :very guest  On enregistre un user qui a pour username "guest" avec un realname "very guest"  
					
Erreurs possibles:	pareil que PASS  
- ERR_NEEDMOREPARAMS		461		"<command> :Not enough parameters"
- ERR_ALREADYREGISTRED	462		":Unauthorized command (already registered)"


###	QUIT
parametres: [ <message envoye a sois meme quand on quitte, les autres ne le voient pas> ]  
**les parametres entre crochet [] sont des parametres optionnels**  

Sert a quitter le client mais pas le server donc pas d erreur possible ca quitte.  


##                  MESSAGES

- [Summary](#table-des-matieres)
- [PRIVMSG](#privmsg)
- [NOTICE](#notice)

###	PRIVMSG
parametres: <msgtarget> <text to be sent>  
<msgtarget> est la cible qui recevra le <text to be sent> et peut etre un channel ou un <user>.  
Les messages IRC sont limites a 510 chars avec "\r\n" a la fin  

Erreurs possibles:
- ERR_NORECIPIENT			411		":No recipient given (<command>)"          
- ERR_NOTEXTTOSEND		412		":No text to send"
- ERR_NOSUCHNICK			401		"<nickname> :No such nick/channel"
           
**la difference entre NORECIPIENT et NOTEXTTOSEND est s il manque juste le text (NOTEXTTOSEND) et s il manque les 2 arguments donc une cible et le text (NORECIPIENT)**  
**il existe une erreur CANNOTSENDTOCHAN mais elle n intervient que si les modes n,v,m ou b sont actifs donc si on ne gere pas ces modes pas besoin de faire cette erreur**  


###	NOTICE
parametres: <msgtarget> <text to be sent>  
NOTICE est pareil que PRIVMSG mais ne recoit pas d erreurs.  
Pas tres utile dans notre projet mais ca evite a des vrais clients/servers des boucles inf quand ils ont des reponses auto a l envoie/reception d un message.  
S il y avait une erreur on aurait donc un message auto envoye qui necessite aussi une reponse.... boucle inf  



##                  CHANNEL OPERATIONS

- [Summary](#table-des-matieres)
- [MODE](#mode)
- [JOIN](#join)
- [KICK](#kick)
- [PART](#part)
- [INVITE](#invite)
- [TOPIC](#topic)

###	MODE
**Tout d abord attention a ne pas confondre MODE et MODE. Il y a un MODE pour les user et un pour les channels, on n utilise que celui pour les chan**  
**Pour se renseigner plus precisement sur les modes channel il faut consulter le [RFC2811](https://www.tech-invite.com/y25/tinv-ietf-rfc-2811.html)**  

parametres: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )  
Cette commande sert a influencer les caracteristiques d un channel et le sujet veut quon fasse les modes l, i, t, k et o.  
Utiliser la commande MODE necessite le privilege channel operator.  
Si on met juste un nom de channel, on doit envoyer un RPL_CHANNELMODEIS pour le chan en question  

On doit donc renseigner un channel sur lequel executer les instructions puis si on veut ajouter ou enlever un mode  
"+" pour ajouter  
"-" pour enlever  

- Le mode +k sert a sert un mot de passe au channel, mode +k a donc un parametre (ex: MODE #test +k 1234)  
- Le mode -k sert donc a enlever ce mot de passe  
- Le mode +o octroie le privilege operateur channel a un user qui est donc renseigne en parametre  
- Le mode -o enleve le privilege operateur d un user en parametre  
- Le mode +l set une limite du nombre d users dans le channel en question avec donc un parametre (ex: MODE #test +l 5)
- Le mode -l enleve cette limite
- Le mode +t sert a autoriser les user sans le privilege operateur d utiliser la commande TOPIC
- Le mode -t restreint l utilisation de TOPIC a des channel operator only
- Le mode +i permet de rendre le channel inviteonly, c est a dire qu on ne peut pas le join sans avoir recu d invitation
- Le mode -i sert a enlever cette restriction inviteonly sinon ne sert a rien

On peut mixer les modes et faire un "MODE #test +itk 1234" mais ca peut etre chiant mais on est libre de gerer comme on veut mais faut pas que ca crash!!  

Il faut prevenir tous les users qu un mode change  

Erreurs possibles:  
- ERR_NEEDMOREPARAMS pour le +k, +l et +o
- ERR_KEYSET				467		"<channel> :Channel key already set"
- ERR_CHANOPRIVSNEEDED	482		"<channel> :You're not channel operator"
- ERR_USERNOTINCHANNEL	441		"<nick> <channel> :They aren't on that channel"
- ERR_UNKNOWNMODE			472		"<char> :is unknown mode char to me for <channel>"
- ERR_NOSUCHCHANNEL		403		"<channel name> :No such channel"

RPL:  
- RPL_UNIQOPIS			325		"<channel> <nickname>"
- RPL_CHANNELMODEIS		324		"<channel> <mode> <mode params>"	// liste les modes actifs sur le channel


###	JOIN
parametres: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] ) / "0"  
**les parametres entre crochet [] sont des parametres optionnels**  
les parametres quand il y a un "/" signifie qu il a plusieurs types d arguments differents possibles, donc soit a droite du / soit a gauche  

La commande join est utilisee pour creer un nouveau chan s il n a pas encore ete cree sinon juste le rejoindre.  
Si on le cree on devient chanop (channel operator) de ce channel.  

*L argument special "0" fait en sorte que le user quitte tous les channel ou il est actuellement membre.*  
**Cet argument special n existe pas sur irssi puisque "/join 0" se transforme en "JOIN #0" donc on rejoint un channel qui sappelle 0 au lieu de suppr**  
En revanche on peut l implementer pour ncat  

L argument <key> est un mot de passe pour restreindre l acces a un channel.  
On peut set une <key> avec MODE +k ou en creant le channel avec JOIN.  

Pour rejoindre un channel il faut donc faire tout ca et a la fin emettre une reponse qui ne soit ni un RPL ni une ERR (voir plus haut) a tous les utilisateurs presents dans le channel  
Ensuite faire les RPL_NAMEREPLY (donc une liste des utilisateurs present dans le channel) puis un RPL_ENDOFNAMES (donc fin du RPL_NAMEREPLY)  

Erreurs possibles:  
- ERR_NEEDMOREPARAMS              
- ERR_INVITEONLYCHAN      473 	"<channel> :Cannot join channel (+i)" 	//	voir MODE
- ERR_BADCHANNELKEY		475		"<channel> :Cannot join channel (+k)"	//	voir MODE
- ERR_CHANNELISFULL       471    	"<channel> :Cannot join channel (+l)"   //	voir MODE
- ERR_NOSUCHCHANNEL               
- ERR_TOOMANYCHANNELS		405		"<channel name> :You have joined too many channels" // pas utile sauf si on veut mettre une limite de channel que chaque user peut rejoindre

RPL:  
- RPL_NAMEREPLY			353		"=<channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )"	// @ pour les chanop et + pour les user normaux
- RPL_ENDOFNAMES			366		"<channel> :End of NAMES list"



###	KICK
parametres: <channel> *( "," <channel> ) <user> *( "," <user> ) [<comment>]  
**les parametres entre crochet [] sont des parametres optionnels**  

On peut donc mettre autant de chan et de user a KICK qu on veut avec une potentielle raison <comment> genre "il parle francais".  
**On peut mettre plusieurs chan et un user ou un chan et plusieurs user pas plusieurs des 2 en meme temps**  
La commande KICK utilise la commande PART (juste en dessous).  
Necessite une reponse non RPL/ERR (voir plus haut)  

Erreurs possibles:  
- ERR_NEEDMOREPARAMS		461		"<command> :Not enough parameters"
- ERR_NOSUCHCHANNEL		403		"<channel name> :No such channel"
- ERR_CHANOPRIVSNEEDED	482		"<channel> :You're not channel operator"
- ERR_USERNOTINCHANNEL	441		"<nick> <channel> :They aren't on that channel"         
- ERR_NOTONCHANNEL		442		"<channel> :You're not on that channel"


###	PART
parametres: <channel> *( "," <channel> ) [ <Part Message> ]  
Fait en sorte que le user qui utilise la fonction PART quitte les channels de la liste en parametre.  
Si <Part Message> est renseigne alors on remplace le message par defaut (qui est le nickname) par <Part Message>.  
Il faut envoyer une reponse non RPL/ERR a tous les autre users du/des channel que l utilisateur quitte avec PART  

Erreurs possibles:  
- ERR_NEEDMOREPARAMS		461		"<command> :Not enough parameters"
- ERR_NOSUCHCHANNEL		403		"<channel name> :No such channel"
- ERR_NOTONCHANNEL		442		"<channel> :You're not on that channel"


###	INVITE
parametres: <nickname> <channel>  
Sert a inviter un utilisateur a rejoindre un channel, il recoit une notif qui lui dit que qqun veut qu il rejoigne tel channel.  
Est ce qu on JOIN directement quand on invite on fait comme on veut.  
Pas besoin d inviter sur un chan valide mais dans ce cas ca ne fait rien.  
En revanche il faut bien etre membre du channel anyway pour INVITE dedans enfin pour que la commande reussisse.  
L utilisateur invite recoit une notification alors que l utilisateur qui invite recoit juste une RPL_INVITING.  
Cette notification est une reponse non ERR/RPL (voir plus haut)  

Erreurs possibles:  
- ERR_NEEDMOREPARAMS		461		"<command> :Not enough parameters"
- ERR_NOSUCHNICK			401		"<nickname> :No such nick/channel"
- ERR_NOTONCHANNEL		442		"<channel> :You're not on that channel"
- ERR_USERONCHANNEL		443		"<user> <channel> :is already on channel"
- ERR_CHANOPRIVSNEEDED	482		"<channel> :You're not channel operator"

Reponses commande:  
- RPL_INVITING			341		"<channel> <nick>"             


###	TOPIC
parametres: <channel> [ <topic> ]  
La commande topic sert a set, montrer ou supprimer un topic d un channel.  
On montre le topic si on ne met pas le parametre optionnel <topic>.  
On supprime le topic si on met le parametre <topic> mais qu il vaut une string vide. (pour ca il faut implementer le ":" qui est le parametre de fin qui peut contenir des espaces et donc des messages vides mais pas obligatoire)  
On set le topic si on renseigne le parametre optionnel <topic>.  
Donc si on fait un "TOPIC #test" il faut renvoye un RPL_TOPIC si on topic existe deja, sinon un RPL_NOTOPIC  
Si on change le topic il faut prevenir tous les users du channel avec une reponse non ERR/RPL (voir plus bcp plus haut)  

Erreurs possibles:  
- ERR_NEEDMOREPARAMS		461		"<command> :Not enough parameters"
- ERR_NOTONCHANNEL		442		"<channel> :You're not on that channel"
- ERR_CHANOPRIVSNEEDED	482		"<channel> :You're not channel operator"
- ERR_NOCHANMODES			477		"<channel> :Channel doesn't support modes"		je comprend pas cette erreur

Reponses commande:  
- RPL_NOTOPIC             331		"<channel> :No topic is set"        
- RPL_TOPIC				332		"<channel> :<topic>"


## TIPS

```c++
#define ERR_NEEDMOREPARAMS	":nom_du_server 461 <command> :Not enough parameters\r\n"
```

peut etre ecrit comme suit:  

```c++
#define ERR_NEEDMOREPARAMS(server_name, command)	":" + server_name + " 461 " + command + " :Not enough parameters\r\n"
```
dans ce cas la, command et server_name sont des variables donc on les appelle comme on veut et on leurs donne la valeur qu on veut  
bon la variable server_name n est pas vraiment une variable on a un seul server donc le nom changera pas on peut le mettre en brut direct...   