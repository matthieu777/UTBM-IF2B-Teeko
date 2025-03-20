#include <conio.h>
#include <windows.h>
#include <stdio.h>


#define DIMENSION 5


int enregistrement(int *plateau, int i,int phasencours){
  FILE*fichier = fopen("tableau.txt", "w");

  if (fichier == NULL) {
    printf("Impossible d'ouvrir le fichier\n");
    return 1;
  }

  for (int o = 0; o < DIMENSION; o++) {
    for (int j = 0; j < DIMENSION; j++) {
      fprintf(fichier,"%d ", plateau[o * DIMENSION + j]);
    }
    fprintf(fichier,"\n");
  }

  fprintf(fichier,"\n%d",i);
  fprintf(fichier,"\n%d",phasencours);

  // Fermeture du fichier
    fclose(fichier);

}

int open_enregistrement(int* pointeur_plateau ,int i,int* phasencours ){

  printf("utiliser sauvgarde \n");

  // Ouvrez un fichier en mode lecture
  FILE* fichier = fopen("tableau.txt", "r");

  // Vérifiez si l'ouverture du fichier a réussi
  if (fichier == NULL)
  {
      Color(4,0);
      printf("Impossible d'ouvrir le fichier\n");
      Color(15,0);
      return 1;
  }
  // ici tu va récuperer la DIMENSION parce qu'elle doit etre installer

  //int DIMENSION = 5;
  int* plateau = NULL; // On crée un pointeur sur int
  plateau = malloc((DIMENSION)*(DIMENSION)*sizeof(int));


  // Parcoure de chaque élément du tableau à partir du fichier
  for (int i = 0; i < DIMENSION; i++)
  {
      for (int j = 0; j < DIMENSION; j++)
      {
          fscanf(fichier, "%d", &plateau[i * DIMENSION + j]);
      }
  }

  int compteur = 0;
  char ligne[256];

  while (fgets(ligne, sizeof(ligne), fichier) != NULL) // lecture des lignes du fichier
  {
    compteur++; // incrémentation du compteur de lignes
    if (compteur == 3) // si c'est la 3 ligne
    {
      sscanf(ligne,"%d", &i); // lecture du nombre à partir de la chaîne de caractères
      i--;

    }
    if (compteur == 4) // si c'est la 3 ligne
    {
      sscanf(ligne,"%d", phasencours); // lecture du nombre à partir de la chaîne de caractères
      break; // sortie de la boucle
    }
  }

  //refaire la meme chose pour la ligne suivante :

  // Fermez le fichier
  fclose(fichier);



  for (int q = 0; q < DIMENSION; q++)
  {
      for (int j = 0; j < DIMENSION; j++)
      {
          printf("%d ", plateau[q * DIMENSION + j]);
      }
      printf("\n");
  }
  *pointeur_plateau = plateau;
  //affichage(*pointeur_plateau, 1, 4);

  return i;

}

/*
int enreeegistrement(int *plateau){
    FILE*fichier = fopen("tableau.txt", "w");
    if (fichier == NULL) {
    printf("Impossible d'ouvrir le fichier\n");
    return 1;
    }
    fprintf(fichier,"            ");
    for (int i = 0; i < DIMENSION; i++) {
    fprintf(fichier,"%c   ", i  + 48);
    }
    fprintf(fichier,"\n          ");
    for (int i = 0; i < DIMENSION; i++) {
    fprintf(fichier,"---------------------\n        ");
    fprintf(fichier,"%c |", i+48);
    for (int j = 0; j < DIMENSION; j++){
      switch (plateau[i*DIMENSION + j]) {
        case 0:
          fprintf(fichier,"   |");
          break;
        case 1 :
          Color(1, 0);
          fprintf(fichier," %c ", 2);
          Color(15, 0);
          fprintf(fichier,"|");
          break;
        case 2 :
          Color(4, 0);
          fprintf(fichier," %c ", 2);
          Color(15, 0);
          fprintf(fichier,"|");
          break;
      }
    }
    fprintf(fichier,"\n          ");
  }
  fprintf(fichier,"---------------------\n\n\n");
    // Fermeture du fichier
    fclose(fichier);
}
*/
