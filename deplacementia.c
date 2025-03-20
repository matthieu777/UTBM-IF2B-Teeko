#include <time.h>

#include "ia.c"


int phaseDeJeu1ia(int *plateau, int i, int color_joueur1, int color_joueur2, int dimension){
  // cette fonction effectu un tour lors de la premiere phase de jeu lorsque l'ia joue
  // c'est une version legerement modifier et alégé des erreur humaine
  // return 1 le placement a ete effectue avec succes, on peut passer a la suite
  // return 2 si la partie est gagné

  if ((i%2)+1 == 1){
    Color(color_joueur1, 0);
    printf("Joueur 1 ");
  }
  else{
    Color(color_joueur2, 0);
    printf("Joueur 2 ");
  }
  Color(15, 0);
  printf("; rentre les coordonnees de la ou tu veux poser ta piece: \n");
  int x = 0, y = 0, booleen;

  float presence_proba_map[dimension*dimension];
  for(int i = 0; i < dimension * dimension; i++){
    presence_proba_map[i] = 0;
  }
  ia_placement(plateau, 5, presence_proba_map, (i%2)+1);
  float maximum = 0;
  x = 0;
  y = 0;
  for(int i = 0; i<dimension; i++){
    for(int j = 0; j<dimension; j++){
      if (presence_proba_map[j + i *dimension] > maximum){
        maximum = presence_proba_map[j + i *dimension];
        x = j;
        y = i;
      }
    }
  }


  booleen = place(plateau, (i%2) +1, x, y, dimension); // effectu le placement du pion
  if(fini(plateau, x, y, dimension) == dimension-1){ // verifie si la partie est fini
    booleen = 2;
  }
  printf("bool %d, %d, %d\n", booleen, x, y);
  return booleen;
}

int phaseDeJeu2ia(int *plateau, int i, int color_joueur1, int color_joueur2, int* super_coup_j1, int* super_coup_j2, int dimension){
  // cette fonction effectu un tour lors de la 2e phase de jeu pour l'ia
  // Comme pour la fonction precedente c'est une verson legerement modifier pour l'ia
  // return 1 le placement a ete effectue avec succe, on peut passer a la suite
  // return 2 si la partie est gagné

  int x = 0, y = 0, x2 = 0, y2 = 0, booleen = 1;
  printf("222\n");
  if ((i%2)+1 == 1){
    Color(color_joueur1, 0);
    printf("Joueur 1 ");
  }
  else{
    Color(color_joueur2, 0);
    printf("Joueur 2 ");
  }
  Color(15, 0);
  printf("; rentrez les coordonnees de la piece que vous voulez bouger: \n");
  int x_actual_square, y_actual_square, x_future_square, y_future_square;
  printf("333\n");
  ia_moving(plateau, dimension, 2, &x_actual_square, &y_actual_square, &x_future_square, &y_future_square);
  printf("444\n");
  printf("rentre les coordonnees de la ou tu veux poser ta piece: \n");

  booleen = deplacement(plateau, x_actual_square, y_actual_square, x_future_square, y_future_square, dimension);

  if(fini(plateau, x_future_square, y_future_square, dimension) == dimension-1){ // verifie si la partie est fini
    booleen = 2;
  }
  printf("555-%d, %d, %d, %d, %d\n", booleen, x_actual_square, y_actual_square, x_future_square, y_future_square);
  return booleen;
}
