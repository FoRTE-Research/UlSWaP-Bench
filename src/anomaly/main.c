#include <stdio.h>
#include <stdint.h>

#include "svm.h"

int main(void){
  printf("Hello, world!\r\n");

  double normal_features[] = {389.242838, 4.02576450, 110.064290, 4.06126274, 1009.53132, -0.499241570};
  double abnormal_features[] = {412.965497, 4.18921196, 110.454395, 3.03292593, 1020.00860, -0.577446114};

  int32_t normal_features_fixedpoint[] = {389242, 4026, 110064, 4061, 1009531, -499};
  int32_t abnormal_features_fixedpoint[] = {412965, 4189, 110454, 3033, 1020009, -577};

  double pred_normal = score(normal_features);
  printf("Normal prediction: %f\r\n\r\n", pred_normal);
  double pred_abnormal = score(abnormal_features);
  printf("Abnormal prediction: %f\r\n", pred_abnormal);

  printf("\r\n\r\n");

  int32_t pred_normal_fixedpoint = scoreFixedPoint(normal_features_fixedpoint);
  printf("Normal prediction, fixed point: %i\r\n\r\n", pred_normal_fixedpoint);
  int32_t pred_abnormal_fixedpoint = scoreFixedPoint(abnormal_features_fixedpoint);
  printf("Abnormal prediction, fixed point: %i\r\n", pred_abnormal_fixedpoint);
}
