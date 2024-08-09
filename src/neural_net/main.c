/*
 * Copyright (c) 2018-2020, Jianjia Ma
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-06-30     Jianjia Ma   The first version
 */

// Modified for standalone execution on MSP430 by Harrison Williams

#include <stdint.h>
#include <stdio.h>

#include "common.h"

#include "nnom.h"
#include "weights.h"
#include "data.h"

#ifdef NNOM_USING_STATIC_MEMORY
uint8_t static_buf[8192];
#endif
 
int benchmark_main(void)
{

  //WDTCTL = WDTPW | WDTHOLD;

  //initClocks();
  //initUart();

	nnom_model_t* model;
	float prob;
	uint32_t pred, errors;

	// when use static memory buffer, we need to set it before create
	nnom_set_static_buf(static_buf, sizeof(static_buf)); 

	model = nnom_model_create();				// create NNoM model

  errors = 0;

  // Run predictions
  for(size_t i = 0; i < TESTCASES; i++){
    printf("Starting classification %d\r\n", i);
    memcpy(nnom_input_data, data[i], sizeof(nnom_input_data));
    nnom_predict(model, &pred, &prob);
    if(pred != labels[i]){
      errors++;
    }
  }

  uint16_t accuracy = errors * 100;
  accuracy /= TESTCASES;
  accuracy = 100 - accuracy;
  printf("Reported model accuracy: %d%\r\n", accuracy);
  printf("Expected model accuracy: %d%\r\n", EXPECTED_ACCURACY);

	// model
	//model_stat(model);
	model_delete(model);
	return 0;
}
