// Partially generated used emlearn

// Percent
#define EXPECTED_ACCURACY 84

static inline int32_t adl_model_tree_0(const int16_t *features, int32_t features_length) {
          if (features[1] < 549) {
              if (features[3] < 1557) {
                  if (features[4] < 1212) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[4] < 1135) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[1] < 671) {
                  if (features[4] < 1349) {
                      return 1;
                  } else {
                      return 3;
                  }
              } else {
                  if (features[5] < 2036) {
                      return 2;
                  } else {
                      return 3;
                  }
              }
          }
        }
        

static inline int32_t adl_model_tree_1(const int16_t *features, int32_t features_length) {
          if (features[2] < 1020) {
              if (features[4] < 1542) {
                  if (features[3] < 1550) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  return 3;
              }
          } else {
              if (features[0] < 600) {
                  if (features[4] < 1373) {
                      return 1;
                  } else {
                      return 3;
                  }
              } else {
                  if (features[5] < 1964) {
                      return 2;
                  } else {
                      return 3;
                  }
              }
          }
        }
        

static inline int32_t adl_model_tree_2(const int16_t *features, int32_t features_length) {
          if (features[1] < 549) {
              if (features[0] < 529) {
                  if (features[5] < 2162) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[4] < 1281) {
                      return 1;
                  } else {
                      return 3;
                  }
              }
          } else {
              if (features[0] < 593) {
                  if (features[4] < 1362) {
                      return 1;
                  } else {
                      return 3;
                  }
              } else {
                  if (features[5] < 1924) {
                      return 2;
                  } else {
                      return 3;
                  }
              }
          }
        }
        

static inline int32_t adl_model_tree_3(const int16_t *features, int32_t features_length) {
          if (features[1] < 549) {
              if (features[3] < 1555) {
                  if (features[5] < 2162) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[0] < 475) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[0] < 597) {
                  if (features[4] < 1373) {
                      return 1;
                  } else {
                      return 3;
                  }
              } else {
                  if (features[2] < 1352) {
                      return 3;
                  } else {
                      return 2;
                  }
              }
          }
        }
        

static inline int32_t adl_model_tree_4(const int16_t *features, int32_t features_length) {
          if (features[1] < 545) {
              if (features[3] < 1555) {
                  return 0;
              } else {
                  if (features[0] < 468) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[4] < 1352) {
                  if (features[2] < 980) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[5] < 2047) {
                      return 2;
                  } else {
                      return 3;
                  }
              }
          }
        }
        

int32_t adl_model_predict(const int16_t *features, int32_t features_length) {

        int32_t votes[4] = {0,};
        int32_t _class = -1;

        _class = adl_model_tree_0(features, features_length); votes[_class] += 1;
    _class = adl_model_tree_1(features, features_length); votes[_class] += 1;
    _class = adl_model_tree_2(features, features_length); votes[_class] += 1;
    _class = adl_model_tree_3(features, features_length); votes[_class] += 1;
    _class = adl_model_tree_4(features, features_length); votes[_class] += 1;
    
        int32_t most_voted_class = -1;
        int32_t most_voted_votes = 0;
        for (int32_t i=0; i<4; i++) {

            if (votes[i] > most_voted_votes) {
                most_voted_class = i;
                most_voted_votes = votes[i];
            }
        }
        return most_voted_class;
    }
    
