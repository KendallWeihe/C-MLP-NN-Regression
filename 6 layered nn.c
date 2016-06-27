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

  for (i = 0; i < NumGame; i++){
    // target[i] = log(1 + exp(target[i]));
    target[i] = 1.0/(1.0 + exp(-target[i]));
    // target[i] = (exp(target[i]) - exp(-target[i])) / (exp(target[i] + exp(-target[i])));
    // target[i] = target[i]/30;
  }

  int hidd1, hidd2, hidd3, hidd4, best_hidd1, best_hidd2, best_hidd3, best_hidd4;
  double min_error = 1000000;
  double error, eta = 0.6, alpha = 0.8, smallwt = 0.5, best_alpha, best_eta;
  for (hidd1 = 10; hidd1 < 40; hidd1++){
    for (hidd2 = 10; hidd2 < 40; hidd2++){
      for (hidd3 = 10; hidd3 < 40; hidd3++){
      for (hidd4 = 10; hidd4 < 40; hidd4++){
        // for (eta = .4; eta < .7; eta = eta + .1){
        //   for (alpha = .7; alpha < 1; alpha = alpha + .1){

      int NumHidd1 = hidd1, NumHidd2 = hidd2, NumHidd3 = hidd3, NumHidd4 = hidd4;
      double sumH1[NumHidd1], sumH2[NumHidd2], sumH3[NumHidd3], sumH4[NumHidd4], sumO;
      double hidden1[NumHidd1], hidden2[NumHidd2], hidden3[NumHidd3], hidden4[NumHidd4], output;
      double weightIn[NumIn][NumHidd1], weightHidd1[NumHidd1][NumHidd2], weightHidd2[NumHidd2][NumHidd3], weightHidd3[NumHidd3][NumHidd4], weightHidd4[NumHidd4];
      double deltaWeightIN[NumIn][NumHidd1], deltaWeightH1[NumHidd1][NumHidd2], deltaWeightH2[NumHidd2][NumHidd3], deltaWeightH3[NumHidd3][NumHidd4], deltaWeightH4[NumHidd4];
      double sumBP1[NumHidd1], sumBP2[NumHidd2], sumBP3[NumHidd3], sumBP4[NumHidd4];
      double deltaH1[NumHidd1], deltaH2[NumHidd2], deltaH3[NumHidd3], deltaH4[NumHidd4], deltaO;
      // double error, eta = 0.5, alpha = 0.9, smallwt = 05;

      for (i = 0; i < NumIn; i++){
        for (j = 0; j < NumHidd1; j++){
          weightIn[i][j] = 2.0 * ( rando() - 0.5 ) * smallwt ;
          deltaWeightIN[i][j] = 0.0;
        }
      }

      for (i = 0; i < NumHidd1; i++){
        for (j = 0; j < NumHidd2; j++){
          weightHidd1[i][j] = 2.0 * ( rando() - 0.5 ) * smallwt ;
          deltaWeightH1[i][j] = 0.0;
        }
      }

      for (i = 0; i < NumHidd2; i++){
        for (j = 0; j < NumHidd3; j++){
          weightHidd2[i][j] = 2.0 * ( rando() - 0.5 ) * smallwt ;
          deltaWeightH2[i][j] = 0.0;
        }
      }

      for (i = 0; i < NumHidd3; i++){
        for (j = 0; j < NumHidd4; j++){
          weightHidd3[i][j] = 2.0 * ( rando() - 0.5 ) * smallwt ;
          deltaWeightH3[i][j] = 0.0;
        }
      }

      for (i = 0; i < NumHidd4; i++){
        weightHidd4[i] = 2.0 * ( rando() - 0.5 ) * smallwt ;
        deltaWeightH4[i] = 0.0;
      }

      for (epoch = 0; epoch < 150; epoch++){

        error = 0.0;
        for (game = 0; game < NumGame; game++){

          for (i = 0; i < NumHidd1; i++){
            sumH1[i] = weightIn[0][i];
            for (j = 0; j < NumIn; j++){
              sumH1[i] += input[game][j] * weightIn[j][i];
            }
            // hidden1[i] = log(1 + exp(sumH1[i]));
            hidden1[i] = 1.0/(1.0 + exp(-sumH1[i]));
            // hidden1[i] = (exp(sumH1[i]) - exp(-sumH1[i])) / (exp(sumH1[i] + exp(-sumH1[i])));
            // hidden1[i] = hidden1[i] / 30;
          }

          for (i = 0; i < NumHidd2; i++){
            sumH2[i] = weightHidd1[0][i];
            for (j = 0; j < NumHidd1; j++){
              sumH2[i] += hidden1[j] * weightHidd1[j][i];
            }
            // hidden2[i] = log(1 + exp(sumH2[i]));
            hidden2[i] = 1.0/(1.0 + exp(-sumH2[i]));
            // hidden2[i] = (exp(sumH2[i]) - exp(-sumH2[i])) / (exp(sumH2[i] + exp(-sumH2[i])));
            // hidden2[i] = hidden2[i] / 30;
          }

          for (i = 0; i < NumHidd3; i++){
            sumH3[i] = weightHidd2[0][i];
            for (j = 0; j < NumHidd2; j++){
              sumH3[i] += hidden2[j] * weightHidd2[j][i];
            }
            // hidden3[i] = log(1 + exp(sumH3[i]));
            hidden3[i] = 1.0/(1.0 + exp(-sumH3[i]));
            // hidden3[i] = (exp(sumH3[i]) - exp(-sumH3[i])) / (exp(sumH3[i] + exp(-sumH3[i])));
            // hidden3[i] = hidden3[i] / 30;
          }

          for (i = 0; i < NumHidd4; i++){
            sumH4[i] = weightHidd3[0][i];
            for (j = 0; j < NumHidd3; j++){
              sumH4[i] += hidden3[j] * weightHidd3[j][i];
            }
            // hidden3[i] = log(1 + exp(sumH3[i]));
            hidden4[i] = 1.0/(1.0 + exp(-sumH4[i]));
            // hidden3[i] = (exp(sumH3[i]) - exp(-sumH3[i])) / (exp(sumH3[i] + exp(-sumH3[i])));
            // hidden3[i] = hidden3[i] / 30;
          }

          sumO = weightHidd4[0];
          for (i = 0; i < NumHidd4; i++){
            sumO += hidden4[i] * weightHidd4[i];
          }

          // output = log(1 + exp(sumO));
          output = 1.0/(1.0 + exp(-sumO));
          // output = (exp(sumO) - exp(-sumO)) / (exp(sumO + exp(-sumO)));
          // output = sumO / 30;
          // error += .5 * (target[game] - output) * (target[game] - output);
          error += (target[game] - output);
          deltaO = (target[game] - output) * output * (1.0 - output);

          for (i = 0; i < NumHidd4; i++){
            sumBP4[i] = weightHidd4[i] * deltaO;
            deltaH4[i] = sumBP4[i] * hidden4[i] * (1.0 - hidden4[i]);
          }

          for (i = 0; i < NumHidd3; i++){
            sumBP3[i] = 0.0;
            for(j = 0; j < NumHidd4; j++){
              sumBP3[i] += weightHidd3[i][j] * deltaH4[j];
            }
            deltaH3[i] = sumBP3[i] * hidden3[i] * (1.0 - hidden3[i]);
          }

          for (i = 0; i < NumHidd2; i++){
            sumBP2[i] = 0.0;
            for(j = 0; j < NumHidd3; j++){
              sumBP2[i] += weightHidd2[i][j] * deltaH3[j];
            }
            deltaH2[i] = sumBP2[i] * hidden2[i] * (1.0 - hidden2[i]);
          }

          for (i = 0; i < NumHidd1; i++){
            sumBP1[i] = 0.0;
            for (j = 0; j < NumHidd2; j++){
                sumBP1[i] += weightHidd1[i][j] * deltaH2[j];
            }
            deltaH1[i] = sumBP1[i] * hidden1[i] * (1.0 - hidden1[i]);
          }

          for (i = 0; i < NumHidd1; i++){
            deltaWeightIN[0][i] = eta * deltaH1[i] + alpha * deltaWeightIN[0][i];
            weightIn[0][i] += deltaWeightIN[0][i];
            for (j = 0; j < NumIn; j++){
              deltaWeightIN[j][i] = eta * input[game][j] * deltaH1[i] + alpha * deltaWeightIN[j][i];
              weightIn[j][i] += deltaWeightIN[j][i];
            }
          }

          for (i = 0; i < NumHidd2; i++){
            deltaWeightH1[0][i] = eta * deltaH2[i] + alpha * deltaWeightH1[0][i];
            weightHidd1[0][i] += deltaWeightH1[0][i];
            for (j = 0; j < NumHidd1; j++){
              deltaWeightH1[j][i] = eta * hidden1[j] * deltaH2[i] + alpha * deltaWeightH1[j][i];
              weightHidd1[j][i] += deltaWeightH1[j][i];
            }
          }

          for (i = 0; i < NumHidd3; i++){
            deltaWeightH2[0][i] = eta * deltaH3[i] + alpha * deltaWeightH2[0][i];
            weightHidd2[0][i] += deltaWeightH2[0][i];
            for (j = 0; j < NumHidd2; j++){
              deltaWeightH2[j][i] = eta * hidden2[j] * deltaH3[i] + alpha * deltaWeightH2[j][i];
              weightHidd2[j][i] += deltaWeightH2[j][i];
            }
          }

          for (i = 0; i < NumHidd4; i++){
            deltaWeightH3[0][i] = eta * deltaH4[i] + alpha * deltaWeightH3[0][i];
            weightHidd3[0][i] += deltaWeightH3[0][i];
            for (j = 0; j < NumHidd3; j++){
              deltaWeightH3[j][i] = eta * hidden3[j] * deltaH4[i] + alpha * deltaWeightH3[j][i];
              weightHidd3[j][i] += deltaWeightH3[j][i];
            }
          }

          deltaWeightH4[0] = eta * deltaO + alpha * deltaWeightH4[0];
          weightHidd4[0] += deltaWeightH4[0];
          for (i = 0; i < NumHidd4; i++){
            deltaWeightH4[i] = eta * hidden4[i] * deltaO + alpha * deltaWeightH4[i];
            weightHidd4[i] += deltaWeightH4[i];
          }

          printf("%G\n", output);

        }
        if (error < .0001)
          break;
        printf("Epoch = %d\n", epoch);
        printf("Error = %G\n", error);
        if (error < min_error){
          min_error = error;
          best_hidd1 = hidd1;
          best_hidd2 = hidd2;
          best_hidd3 = hidd3;
          best_hidd4 = hidd4;
          // best_alpha = alpha;
          // best_eta = eta;
        }
        printf("Min error = %G\n", min_error);
        printf("Best Hidd1 = %d\n", best_hidd1);
        printf("Best Hidd2 = %d\n", best_hidd2);
        printf("Best Hidd3 = %d\n", best_hidd3);
        printf("Best Hidd4 = %d\n", best_hidd4);
        // printf("Best alpha = %G ... eta = %G\n", alpha, eta);
        printf("Current Hidd 1 = %d ... Hidd2 = %d ... Hidd2 = %d ... Hidd4 = %d\n", hidd1, hidd2, hidd3, hidd4);

    //   }
    //
    // }
  }}}
}

  }
  //
  // printf("Input weights-------------------\n" );
  // for (i = 0; i < NumIn; i++){
  //   for (j = 0; j < NumHidd1; j++){
  //     printf("%G\n", weightIn[i][j]);
  //   }
  //   printf("\n" );
  // }
  //
  // printf("Hidd1 weights-------------------\n" );
  // for (i = 0; i < NumHidd1; i++){
  //   for (j = 0; j < NumHidd2; j++){
  //     printf("%G\n",weightHidd1[i][j]);
  //   }
  //   printf("\n" );
  // }
  //
  // printf("Hidd2 weights-------------------\n" );
  // for (i = 0; i < NumHidd2; i++){
  //     printf("%G\n", weightHidd2[i]);
  // }

}
