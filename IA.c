#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ### STRUCTURES AND CONSTANTS ### */

typedef struct Line{
    int type; // a number for the type of the line (respectively 1 for horizontal, then diagonal (\), anti-diagonal (/) and vertical)
    int origin; // the origin of the line
    float x, y; // coordinates of the line "center". I know a line doesn't have a middle but a segment does so...
    int F; // number of pawns aligned
    int color; // color of the player (1 or 2)
} line;

typedef struct Coordinates{
    int x;
    int y;
} coos;

/* ### USUAL FUNCTIONS ### */

float find_max(float array[], int arr_len, coos* coordinates){ // Find the max in an array, return its value, and gives its coordinates
    float max = 0;
    for(int i = 0; i < arr_len; i++){
        for(int j = 0; j < arr_len; j++){

            if(array[j + i*arr_len] > max){
                max = array[j + i*arr_len]; // value of the max
                (*coordinates).x = j;
                (*coordinates).y = i;

            }
        }
    }
    return max;
}

float diagonal_mean(int type, int origin, int axe, int gameboard_lenght){
    int n = gameboard_lenght - abs(origin - (gameboard_lenght - 1));
    float sum = 0;

    for(int i = 0; i < n; i++){
        // diagonal (\)
        if(type == 2){
            if(axe == 0) { // this is for the abscissa
                if(origin < gameboard_lenght - 1){
                    sum += (i + gameboard_lenght - origin - 1);
                } else if (origin >= gameboard_lenght - 1){
                    sum += i;
                }
            } else { // this is for the ordered
                if(origin < gameboard_lenght - 1){
                    sum += (origin + i + 1 - gameboard_lenght);
                } else if (origin >= gameboard_lenght - 1){
                    sum += i;
                }
            }
        }
        // anti-diagonal (/)
        if(type == 3){
            if(axe == 0) { // this is for the abscissa
                if(origin < gameboard_lenght - 1){
                    sum += (origin - i);
                } else if (origin >= gameboard_lenght - 1){
                    sum += gameboard_lenght - 1 - i;
                }
            } else { // this is for the ordered
                if(origin < gameboard_lenght - 1){
                    sum += i;
                } else if (origin >= gameboard_lenght - 1){
                    sum += (origin - gameboard_lenght + 1 + i);
                }
            }
        }
    }
    return sum / n;
}

int maxi(float a, float b){
    if (a >= b) {
        return a;
    } else {
        return b;
    }
}

/* ### PROGRAM ### */

float force(int ia_color, float x_center, float y_center, float x_case, float y_case, float F, int color){
    // Returns a number which will be used in the probability of presence map
    // F is the number of pawns aligned on the line. Called like that because it determines the force of the function (greater it is, greater the number force will be)
    // ia_color is an int representing the color of the IA.

    int distance = maxi(abs(x_center - x_case), abs(y_center - y_case)); // The distance is given in number of square away, not a real distance
    if (color == ia_color){
        return pow(F, 4) * expf((-F/20) * pow(distance, 2));
    } else if (color == (ia_color % 2 + 1)) {
        return pow(F, 3.9) * expf((-F/20) * pow(distance, 2));
    } // the ally is slightly above the enemy in the decision meaning that the IA will chose to win over blocking the enemy
    else {
        return 0;
    }
}

int get_origin(coos coordinates, int type, int len){
    switch(type){
        case 1:
            return coordinates.y;
            break;
        case 2:
            return (len - 1) - (coordinates.x - coordinates.y);
            break;
        case 3:
            return (coordinates.x + coordinates.y);
            break;
        case 4:
            return coordinates.x;
            break;
    } return 0;
}

void ia_placement(int gameboard[], int n, float presence_proba_map[], int ia_color){

    // Initialize the probability map to an array of zero
    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
            presence_proba_map[x + y*n] = 0;
        }
    }

    // Create two lists containing the coordinates of the pawns, allies and enemies
    coos allies[n];
    coos enemies[n];
    int i = 0, j = 0, num_ally = 0, num_enemy = 0;

    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
            if(gameboard[x + y * n] == 1){
                allies[i].x = x;
                allies[i].y = y;
                i++;
                num_ally++;
            } else if(gameboard[x + y * n] == 2){
                enemies[j].x = x;
                enemies[j].y = y;
                j++;
                num_enemy++;
            }
        }
    }

    line lines_list[(num_ally + num_enemy) * 4];

    int k = 0;
    for(int color = 1; color < 3; color++){


        // horizontal
        for(int b = 0; b < n; b++){
            int F = 0;
            for(int i = 0; i < n; i++){
                if(gameboard[b + i*n] == color){
                    F++;
                }
            }
            if(F != 0){
                lines_list[k].type = 1;
                lines_list[k].origin = b;
                lines_list[k].x = b;
                lines_list[k].y = n/2;
                lines_list[k].F = F;
                lines_list[k].color = color;
                k++;
            }
        }

        // diagonal (\)
        for(int b = 0; b < 2*(n-1); b++){
            int F = 0;
            for(int i = 0; i < (n - abs(b - (n - 1))); i++){
                int x_case = 0, y_case = 0;
                if(b < (n-1)){
                    y_case = i;
                    x_case = i + n - b - 1;
                } else if(b > (n-1)){
                    x_case = i;
                    y_case = i - n + b + 1;
                } else {
                    x_case = i;
                    y_case = i;
                }

                if(gameboard[x_case + n*y_case] == color){
                    F++;
                }
            }
            if(F != 0){
                lines_list[k].type = 2;
                lines_list[k].origin = b;
                lines_list[k].x = diagonal_mean(2, b, 0, n);
                lines_list[k].y = diagonal_mean(2, b, 1, n);
                lines_list[k].F = F;
                lines_list[k].color = color;
                k++;
            }
        }

        // anti-diagonal (/)
        for(int b = 0; b < 2*(n-1); b++){
            int F = 0;
            for(int i = 0; i < (n - abs(b - (n - 1))); i++){
                int x_case = 0, y_case = 0;
                if(b < (n-1)){
                    y_case = i;
                    x_case = b - i;
                } else if(b > (n-1)){
                    x_case = n - 1 - i;
                    y_case = b - n + 1 + i;
                } else {
                    x_case = n - 1 - i;
                    y_case = i;
                }

                if(gameboard[x_case + n*y_case] == color){
                    F++;
                }
            }
            if(F != 0){
                lines_list[k].type = 3;
                lines_list[k].origin = b;
                lines_list[k].x = diagonal_mean(3, b, 0, n);
                lines_list[k].y = diagonal_mean(3, b, 1, n);
                lines_list[k].F = F;
                lines_list[k].color = color;
                k++;
            }
        }

        // vertical
        for(int b = 0; b < n; b++){
            int F = 0;
            for(int i = 0; i < n; i++){
                if(gameboard[i + b*n] == color){
                    F++;
                }
            }
            if(F != 0){
                lines_list[k].type = 4;
                lines_list[k].origin = b;
                lines_list[k].x = n/2;
                lines_list[k].y = b;
                lines_list[k].F = F;
                lines_list[k].color = color;
                k++;
            }
        }
    }

    line updated_lines_list[k];
    int len = 0;
    for(int i = 0; i < k; i++){
        if(!((lines_list[i].x < (n-1)/2 - 0.5 || lines_list[i].x > n/2 + 0.5) && (lines_list[i].y < (n-1)/2 - 0.5 || lines_list[i].y > n/2 + 0.5))){
            updated_lines_list[len].type = lines_list[i].type;
            updated_lines_list[len].origin = lines_list[i].origin;
            updated_lines_list[len].x = lines_list[i].x;
            updated_lines_list[len].y = lines_list[i].y;
            updated_lines_list[len].F = lines_list[i].F;
            updated_lines_list[len].color = lines_list[i].color;
            len++;
        }
    }

    // Use the list created just before
    for(int b = 0; b < len; b++){
        if(updated_lines_list[b].type == 1){  // horizontal
            for(int i = 0; i < n; i++){
                presence_proba_map[updated_lines_list[b].origin + i*n] += force(ia_color, updated_lines_list[b].x, i, updated_lines_list[b].y, updated_lines_list[b].origin, updated_lines_list[b].F, updated_lines_list[b].color);
            }

        } else if(updated_lines_list[b].type == 2){ // diagonal (\)
            for(int i = 0; i < (n - abs(updated_lines_list[b].origin - (n - 1))); i++){
                int x, y;
                if(i < (n-1)){
                    y = i;
                    x = i + n - updated_lines_list[b].origin - 1;
                } else if(i > (n-1)){
                    x = i;
                    y = i - n + updated_lines_list[b].origin + 1;
                } else {
                    x = i;
                    y = i;
                }
                presence_proba_map[x + n*y] += force(ia_color, updated_lines_list[b].x, updated_lines_list[b].y, x, y, updated_lines_list[b].F, updated_lines_list[b].color);
            }

        } else if(updated_lines_list[b].type == 3){ // anti-diagonal (/)
            for(int i = 0; i < (n - abs(updated_lines_list[b].origin - (n - 1))); i++){
                int x, y;
                if(i < (n-1)){
                    y = i;
                    x = updated_lines_list[b].origin - i;
                } else if(i > (n-1)){
                    x = n - 1 - i;
                    y = updated_lines_list[b].origin - n + 1 + i;
                } else {
                    x = n - 1 - i;
                    y = i;
                }
                presence_proba_map[x + y*n] += force(ia_color, updated_lines_list[b].x, updated_lines_list[b].y, x, y, updated_lines_list[b].F, updated_lines_list[b].color);
            }

        } else if(updated_lines_list[b].type == 4){ // vertical
            for(int i = 0; i < n; i++){
                presence_proba_map[i + updated_lines_list[b].origin*n] += force(ia_color, updated_lines_list[b].x, updated_lines_list[b].y, i, updated_lines_list[b].origin, updated_lines_list[b].F, updated_lines_list[b].color);
            }
        }
    }

    // Get rid of the places where there is already a pawn, to not interfere with the IA
    for(int i = 0; i < num_ally; i++){
        presence_proba_map[allies[i].x + allies[i].y * n] = 0;
    }
    for(int i = 0; i < num_enemy; i++){
        presence_proba_map[enemies[i].x + enemies[i].y * n] = 0;
    }
}

void ia_moving(int gameboard[], int n, int ia_color, int* x_actual_square, int* y_actual_square, int* x_future_square, int* y_future_square){

    coos allies[n];
    int k = 0;

    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
            if(gameboard[x + y*n] == ia_color){
                allies[k].x = x;
                allies[k].y = y;
                k++;
            }
        }
    }

    int gameboard_array[k][n*n];

    for(int i = 0; i < k; i++){
        for(int j = 0; j < n*n; j++){
            if(allies[i].x + allies[i].y*n != j){
                gameboard_array[i][j] = gameboard[j];
            } else {
                gameboard_array[i][j] = 3;
            }
        }
    }

    float presence_proba_map_array[k][n*n];

    for(int i = 0; i < k; i++){

        ia_placement(gameboard_array[i], n, presence_proba_map_array[i], ia_color);

        int x = 0, y = 0;
        for(int a = 0; a < n; a++){
            for(int b = 0; b < n; b++){
                if(gameboard_array[i][a + b*n] == 3){
                    x = a;
                    y = b;
                }
            }
        }
        // Here we put to 0 all the squares that are to far from the pawn
        for(int a = 0; a < n; a++){
            for(int b = 0; b < n; b++){
                if((abs(b - x) > 1 || abs(a - y) > 1) || ((b - x) == 0 && (a - y) == 0)){
                    presence_proba_map_array[i][b + a*n] = 0;
                }
            }
        }

/*
        for(int y = 0; y < 5; y++){
            for(int x = 0; x < 5; x++){
                printf("%d ", gameboard_array[i][x + y*5]);
            }
            printf("\n");
        }
        printf("\n");

        for(int y = 0; y < 5; y++){
            for(int x = 0; x < 5; x++){
                printf("%f ", presence_proba_map_array[i][x + y*5]);
            }
            printf("\n");
        }
        printf("\n");*/
    }

    // Get the max amongst all the probability of presence maps, to know what will be the next move
    float max = 0;
    int indice_max = 0;
    coos local_max_coo, coo_max;
    local_max_coo.x = 0;
    local_max_coo.y = 0;

    for(int i = 0; i < k; i++){
        float local_max = find_max(presence_proba_map_array[i], n, &local_max_coo);
            if(local_max > max){
                max = local_max;
                indice_max = i;
                coo_max = local_max_coo;
            }
        }

    // The program returns nothing, but gives the values via the pointers.
    *x_actual_square = allies[indice_max].x;
    *y_actual_square = allies[indice_max].y;

    *x_future_square = coo_max.x;
    *y_future_square = coo_max.y;

}
/*
int main(){
    int plateau[25] = {0};

    plateau[0 + 2*5] = 1;
    plateau[1 + 2*5] = 1;
    plateau[2 + 2*5] = 1;
    plateau[3 + 1*5] = 1;

    plateau[1 + 1*5] = 2;
    plateau[2 + 3*5] = 2;
    plateau[3 + 2*5] = 2;
    plateau[4 + 2*5] = 2;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            printf("%d ", plateau[j + i*5]);
        }
        printf("\n");
    }
    printf("\n\n");
    int a, b, c, d;
    ia_moving(plateau, 5, 2, &a, &b, &c, &d);

    printf("%d %d -> %d %d", a, b, c, d);
}
*/
