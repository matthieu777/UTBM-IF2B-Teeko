#include <stdio.h>



#include "affichage.c"
#include "deplacement.c"
#include "enregistrement.c"
#include "menu.c"

#define DIMENSION 5

int main(){
  int isecondephase = -1, color_joueur1 = 1, color_joueur2 = 4, dimension = DIMENSION, nbr_pion = 4, nbr_largeur = 5;
  int i =1, etat_de_jeu, booleen;

  int phasencours = 1;

  etat_de_jeu = menu(&color_joueur1, &color_joueur2, &i, &dimension, &nbr_pion, &nbr_largeur);
  int plateau[DIMENSION*DIMENSION] = {0};
  int* pointeur_plateau = NULL;
  if(etat_de_jeu == 1){
    i=open_enregistrement(*pointeur_plateau,i,&phasencours);
    plateau = *pointeur_plateau;
    if(phasencours != 1){
      booleen = 1;
    }
    printf("phase %d \n",phasencours);
  }


   // init du plateau
  affichage(plateau, color_joueur1, color_joueur2);
  //1er phase
  int x , y;


  if (phasencours == 1){
    do{
      i++;
      printf("i = %d\n", i);
      booleen = phaseDeJeu1(plateau, i, color_joueur1, color_joueur2);
      while (booleen == 0) {
        Color(4, 0);
        printf("ERREUR");
        Color(15, 0);
        printf(" le pion ne peux pas etre poser \n");
        booleen = phaseDeJeu1(plateau, i, color_joueur1, color_joueur2);
      }
      affichage(plateau, color_joueur1, color_joueur2);
    }
    while (i < 2 *(DIMENSION-1)+1 && booleen!=2 && booleen !=4);
    if (booleen != 4){
      phasencours = 2;
      i = 0;
    }
  }



  //2e phase
  int x2= 0, y2=0;



  while(booleen == 1){
    booleen = phaseDeJeu2(plateau, i, color_joueur1, color_joueur2);
    while (booleen == 0) {
      Color(4, 0);
      printf("ERREUR");
      Color(15, 0);
      printf(" le pion n'a pas pu etre deplacer\n");
      booleen = phaseDeJeu2(plateau, i, color_joueur1, color_joueur2);
    }
    affichage(plateau, color_joueur1, color_joueur2);
    i++;
    printf("i = %d\n", i);
  }


  if (booleen == 4){
    Color(11, 0);
    printf("sauvgarde ici\n");
    Color(15,0);
      // Ouverture du fichier en mode écriture
    enregistrement(plateau,i,phasencours);
  }
  else if(booleen == 2) {
    Color(15, 0);
    printf("Bravo, le ");
    if ((i%2)+1 == 1){
      Color(color_joueur1, 0);
      printf("Joueur 1 ");
    }
    else{
      Color(color_joueur2, 0);
      printf("Joueur 2 ");
    }
    Color(15, 0);
    printf("a gagner");
  }

}
