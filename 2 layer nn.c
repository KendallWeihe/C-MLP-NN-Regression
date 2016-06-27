#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>

#define rando() ((double)rand()/(RAND_MAX+1))

void print(double input[39][20]){
  int i, j;
  for (i = 0; i < 39; i++){
    for (j = 0; j < 20; j++){
      printf("%G, ", input[i][j]);
    }
    printf("\n");
  }
}

int main(){

  int i, j, k, game, epoch, NumGame = 81, NumIn = 20;
  double input[NumGame][NumIn];
  double target[NumGame];

  char line[1024];
  double num;
  int line_count = 0, target_index = 0, game_index = 0, input_index = 0;
  FILE *csvFile = fopen("training set.csv", "r");

  if (csvFile){
    char *token;
    while (fgets(line, 1024, csvFile)){
      token = strtok(&line[0], ",");
      if (!strcmp(token, "eof"))
        break;
      if (!strcmp(token,"\r\n")){
        line_count++;
        input_index = 0;
      }

      while(token){
        num = atof(token);
        if (line_count == 2 && strcmp(token,"\r\n")){
          target[target_index] = num;
          target_index++;
        }
        else if (line_count == 0){
          input[game_index][input_index] = num;
          input_index++;
        }
        token = strtok(NULL, ",");
      }
      if (line_count == 4){
        line_count = 0;
        game_index++;
      }

    }
    fclose(csvFile);
  }

  double output, sum, error, smallwt = .5, total_error, min_error = 1000000;
  double weights[NumIn], deltaWeights[NumIn];

  for (i = 0; i < NumGame; i++){
    // target[i] = log(1 + exp(target[i]));
    target[i] = 1.0/(1.0 + exp(-target[i]));
    // target[i] = (exp(target[i]) - exp(-target[i])) / (exp(target[i] + exp(-target[i])));
    // target[i] = target[i]/30;
  }

  for (i = 0; i < NumIn; i++){
    weights[i] = 2.0 * ( rando() - 0.5 ) * smallwt ;
    deltaWeights[i] = 0.0;
  }

  for (epoch = 0; epoch < 15000; epoch++){
    total_error = 0.0;
    for (game = 0; game < NumGame; game++){

      sum = 0.0;
      for (i = 0; i < NumIn; i++){
        sum += input[game][i] * weights[i];
      }

      output = 1.0/(1.0 + exp(-sum));
      error = (target[game] - output);
      total_error += fabs(error);

      for (i = 0; i < NumIn; i++){
        deltaWeights[i] = error * (1.0/(1.0 + exp(-sum))) * (1 - (1.0/(1.0 + exp(-sum))));
        weights[i] += deltaWeights[i];
      }

      printf("%G\n", output);

    }
    if (total_error < min_error)
      min_error = total_error;
    printf("Min error = %G\n", min_error);
    if (min_error < 38)
      break;
  }

}
