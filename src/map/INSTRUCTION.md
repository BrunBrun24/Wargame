Pour construire la map : 

on imagine une matrice : 

0 0 0 0 0...
0 0 0 0 0...
0 0 0 0 0...
0 0 0 0 0...
0 0 0 0 0...
... (elle continue)
la première ligne est toujours pleine (je donne un exemple avec les cotés qui contiennent un autre hexagone)

2 4 4 4 4 (0)
0 5 6 6 6 (1)
3 6 6 6 6 (2)
0 5 6 6 6 (3)
3 6 6 6 6 (4)

Si la ligne est pair : 
    => on a les elements a coté du sens suivant : 
        => en haut sera celui en haut a gauche dans un hexagone, 
        => en haut a droite sera celui en haut a droite dans un hexagone,
        => a gauche sera celui a gauche dans un hexagone,
        => a droite sera celui a droite dans un hexagone,
        => en bas sera celui en bas a gauche dans un hexagone,
        => en bas a droite sera celui en bas a droite dans un hexagone.

Si la ligne est impaire:
    => on a les elements a coté du sens suivant : 
        => en haut a gauche sera celui en haut a gauche dans un hexagone, 
        => en haut = sera celui en haut a droite dans un hexagone,
        => a gauche sera celui a gauche dans un hexagone,
        => a droite sera celui a droite dans un hexagone,
        => en bas a gauche sera celui en bas a gauche dans un hexagone,
        => en bas = sera celui en bas a droite dans un hexagone.
