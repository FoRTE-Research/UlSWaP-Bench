#include <stdio.h>

#include "common.h"
#include "adl_model.h" // emlearn generated model
#include "data.h"

int benchmark_main(void){
  uint16_t errors = 0;
  uint8_t out;
  int i;
  for(i = TESTCASES-1; i >= 0; i--){
    out = adl_model_predict(testFeatures[i], 6) + 1;
    if(out != testClasses[i]){
      errors++;
    }
  }
  // Stick to int division
  uint16_t accuracy = errors * 100;
  accuracy /= TESTCASES;
  accuracy = 100 - accuracy;
  printf("Reported model accuracy: %d%\r\n", accuracy);
  printf("Expected model accuracy: %d%\r\n", EXPECTED_ACCURACY);
  return 0;
}
