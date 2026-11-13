PIPE-ADDICT - Projet C avec SDL2
================================

CONTENU
-------
main.c       : initialisation SDL2 et boucle principale
game.c/h     : état global et règles générales de la partie
grid.c/h     : génération et gestion des cases
input.c/h    : gestion des clics et boutons
fluid.c/h    : circulation du fluide, connexions, malus
render.c/h   : affichage graphique
Pipe-Addict.cbp : projet Code::Blocks pour MinGW

PREREQUIS
---------
- Windows
- Code::Blocks avec compilateur MinGW
- SDL2 development package (MinGW)
- SDL2.dll accessible à côté de l'exécutable ou dans le PATH

CONFIGURATION CODE::BLOCKS
--------------------------
Ouvrir Pipe-Addict.cbp.
Dans Settings > Compiler > Search directories > Compiler :
ajouter le dossier include de SDL2, par exemple :
C:\Libraries\SDL2\x86_64-w64-mingw32\include\SDL2

Dans Search directories > Linker :
ajouter le dossier lib de SDL2, par exemple :
C:\Libraries\SDL2\x86_64-w64-mingw32\lib

Dans Linker settings > Link libraries, ajouter :
mingw32
SDL2main
SDL2

Copier SDL2.dll à côté de l'exécutable généré (bin\Debug ou bin\Release).
Le chemin exact dépend de l'emplacement de SDL2 sur votre ordinateur.

COMPILATION
-----------
Build > Build, puis Run.

COMMANDES
---------
- Clic sur une case cachée : révèle son contenu.
- Nouveau clic sur une case révélée contenant un tuyau : échange le tuyau
  avec celui de la case Swap.
- Les bombes et cases électriques ne sont pas échangeables.
- Le fluide démarre automatiquement après 5 secondes.
- REFAIRE : restaure la grille initiale et redémarre le délai.
- NOUVEAU : génère une nouvelle grille et de nouveaux endpoints.
- TERMINER : ferme le jeu.

REGLES IMPLEMENTEES ET LIMITES A CONNAITRE
------------------------------------------
- La grille est composée de 8 x 8 cases.
- Génération aléatoire : 4 cases électriques, 3 bombes et des tuyaux.
- Le fluide entre à gauche, sur la ligne d'entrée.
- Il doit ressortir à droite, sur la ligne de sortie.
- Les tuyaux doivent avoir la connexion correspondant au côté d'arrivée.
- Une case électrique laisse passer le fluide tout droit et accélère.
- Une bombe interrompt le fluide 5 secondes, masque les cases non traversées,
  puis le fluide reprend dans la même direction.
- Le programme est une base pédagogique à tester. La difficulté et la
  distribution des pièces peuvent être adaptées à l'énoncé exact de votre sujet.
- Le fluide démarre automatiquement après 5 secondes, même si le joueur n'a
  pas terminé ses échanges : à discuter/adapter selon votre interprétation
  de la règle et les consignes de l'enseignant.

UTILISATION DE L'IA
-------------------
Le fichier main.c contient un exemple de commentaire de transparence IA.
Complétez-le dans chaque fichier source .c selon les consignes du professeur :
outil, estimation honnête de la contribution, exemples de prompts.
Ne recopiez pas un pourcentage sans mesurer votre usage réel.

IMPORTANT POUR LE RAPPORT ET LA SOUTENANCE
-----------------------------------------
Chaque membre doit comprendre les fonctions qu'il présente. Faites vos propres
tests, notez les bugs corrigés, et gardez les traces Git de vos contributions.
