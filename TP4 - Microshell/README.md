Ceci est un README a destination des enseignants de l'UE Informatique Système d'année 4 de spécialité Robotique à Polytech Sorbonne dans le cadre du projet micro-shell du TP4 (2025-2026). 

Il reste intéressant pour quiconque souhaiterait en apprendre davantage sur le fonctionnement du code.

Premièrement, le shell est fractionné en 1) le gestionnaire d'entrées utilisateurs (prompt) 2) les analyseurs (parser et lexer) et 3) l'exécution. 

L'ensemble des fonctions attendues, excepté le chaînage de tuyaux, ont été implémentées. Il resterait néanmoins à réaliser de nombreux test pour s'assurer de leur robustesse (certains seront ravis de s'en charger). 

L'analyse à été réalisé sans appel à strdup() afin de réduire le nombre d'allocations dynamique : les appels successifs à strtok() permettent de fractionner le prompt utilisateur contenu dans le buffer. Cela demande quelques gymnastique (en particulier lorsqu'on souhaite différencier les délimiteurs comme dans le cas des séparateurs) qui jusqu'ici n'ont pas induis de problème.

À l'avenir (sans chercher à ajouter de fonctionnalités avancées), il faudrait :
- Améliorer la façon dont est calculé le nombre commandes unitaires à traiter
- Utiliser les arguments de la fonction main() pour ajouter un -verbose, avec un booléen partagé entre les fichiers pour activer les commentaires de déboggages
- Affiner la fragmentation initiale, avec un strtok() par séparateurs (en fonctionnant par ordre de priorité), pour permettre des compositions un peu plus farfelues que celles prises en charge dans l'état
- Supprimer le #define MAX_ARGC 128 haha (je ne connaissais pas encore le fonctionnement de strtok() et execvp())
- Utiliser ou supprimer le champ "char* outfile" (pour les redirections farfelues et composées)
- Nettoyer et mieux segmenter le code
- Gérer les chaînes de charactères passées entre guillemets !

Remarque : certains commentaires sont en anglais, d'autres en français, ça dépendait de mon humeur.