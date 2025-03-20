#include <stdio.h>
#include <stdlib.h>



#include "affichage.c"
#include "deplacement.c"
#include "enregistrement.c"
#include "menu.c"
#include "deplacementIa.c"



int main(){
  int isecondephase = -1, color_joueur1 = 1, color_joueur2 = 4, dimension = 5, nbr_pion = 4, nbr_largeur = 5;
  int i =1, etat_de_jeu, booleen;

  int phasencours = 1;

  int iajoue = 0;
  etat_de_jeu = menu(&color_joueur1, &color_joueur2, &i, &dimension, &nbr_pion, &nbr_largeur, &iajoue);




  if(etat_de_jeu == 1){
    dimension = open_enregistrement_dim() ;
  }

  printf("dim %d\n", dimension);

  int plateau[dimension*dimension];

  if(etat_de_jeu == 1){


    i=open_enregistrement(plateau ,i,&phasencours, dimension);

    if(phasencours != 1){
      booleen = 1;
    }
    printf("phase %d \n",phasencours);
  }
  else{

    for(int i = 0; i < dimension * dimension; i++){
      plateau[i] = 0;
    }
  }


   // init du plateau
  affichage(plateau, color_joueur1, color_joueur2, dimension);
  //1er phase
  int x , y;


  if (phasencours == 1){
    do{
      i++;
      if(iajoue == 1 && (i%2)+1 == 2){
        booleen = phaseDeJeu1ia(plateau, i, color_joueur1, color_joueur2, dimension);
        affichage(plateau, color_joueur1, color_joueur2, dimension);
      }
      else{
        booleen = phaseDeJeu1(plateau, i, color_joueur1, color_joueur2, dimension);
        while (booleen == 0) {
          Color(4, 0);
          printf("ERREUR");
          Color(15, 0);
          printf(" le pion ne peux pas etre poser \n");
          booleen = phaseDeJeu1(plateau, i, color_joueur1, color_joueur2, dimension);
        }
        affichage(plateau, color_joueur1, color_joueur2, dimension);
      }
    }
    while (i < 2 *(dimension-1)+1 && booleen!=2 && booleen !=4);

    if (booleen != 4){
      phasencours = 2;
    }
  }

  //2e phase
  int x2= 0, y2=0, super_coup_j1 = 0, super_coup_j2 = 0;

  while(booleen == 1){
    if(((i%2)+1 == 1 && super_coup_j1 ==1) || ((i%2)+1 == 2 && super_coup_j2 ==1)){
      if(iajoue == 1 && (i%2)+1 == 2){
        booleen = phaseDeJeu1ia(plateau, i, color_joueur1, color_joueur2, dimension);
        printf("fin %d\n", booleen);
      }
      else{
        printf("Vous aviez utilise precedament le super coup\n");
        booleen = phaseDeJeu1(plateau, i, color_joueur1, color_joueur2, dimension);
        while (booleen == 0) {
          Color(4, 0);
          printf("ERREUR");
          Color(15, 0);
          printf(" le pion ne peux pas etre poser \n");
          booleen = phaseDeJeu1(plateau, i, color_joueur1, color_joueur2, dimension);
        }
        if ((i%2)+1 == 1){
          super_coup_j1 = 0;
        }
        else if ((i%2)+1 == 2){
          super_coup_j2 = 0;
        }
      }
    }
    else{
      if(iajoue == 1 && (i%2)+1 == 2){
        printf("111\n");
        booleen = phaseDeJeu2ia(plateau, i, color_joueur1, color_joueur2, &super_coup_j1, &super_coup_j2, dimension);
        printf("666-%d\n", booleen);
      }
      else{
        booleen = phaseDeJeu2(plateau, i, color_joueur1, color_joueur2, &super_coup_j1, &super_coup_j2, dimension);
        while (booleen == 0){
          Color(4, 0);
          printf("ERREUR");
          Color(15, 0);
          printf(" le pion n'a pas pu etre deplacer\n");
          booleen = phaseDeJeu2(plateau, i, color_joueur1, color_joueur2, &super_coup_j1, &super_coup_j2, dimension);
        }
      }
    }
    affichage(plateau, color_joueur1, color_joueur2, dimension);
    i++;
  }
  if (booleen == 4){
    Color(11, 0);
    printf("sauvgarde ici\n");
    Color(15,0);
      // Ouverture du fichier en mode écriture
    enregistrement(plateau,i,phasencours, dimension);
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
