#include <stdio.h>
#include <math.h>

#include "common.h"

static unsigned curtask;
unsigned count = 0;
unsigned seed = 1;

// Number of samples to discard before recording training set
#define NUM_WARMUP_SAMPLES 3

#define ACCEL_WINDOW_SIZE 3
#define MODEL_SIZE 16
#define SAMPLE_NOISE_FLOOR 10

// Number of classifications to complete in one experiment
#define SAMPLES_TO_COLLECT 128

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} threeAxis_t;

typedef struct {
    int8_t x;
    int8_t y;
    int8_t z;
} threeAxis_t_8;

#ifdef ACCEL_16BIT_TYPE
typedef threeAxis_t accelReading;
#else // !ACCEL_16BIT_TYPE
typedef threeAxis_t_8 accelReading;
#endif // !ACCEL_16BIT_TYPE

typedef accelReading accelWindow[ACCEL_WINDOW_SIZE];

typedef struct {
    unsigned meanmag;
    unsigned stddevmag;
} features_t;

typedef enum {
    CLASS_STATIONARY,
    CLASS_MOVING,
} class_t;

typedef struct {
    features_t stationary[MODEL_SIZE];
    features_t moving[MODEL_SIZE];
} model_t;

typedef enum {
    MODE_IDLE = 3,
    MODE_TRAIN_STATIONARY = 2,
    MODE_TRAIN_MOVING = 1,
    MODE_RECOGNIZE = 0, // default
} run_mode_t;

typedef struct {
    unsigned totalCount;
    unsigned movingCount;
    unsigned stationaryCount;
} stats_t;


void accel_sample(unsigned seed, accelReading* result){
	result->x = (seed*17)%85;
	result->y = (seed*17*17)%85;
	result->z = (seed*17*17*17)%85;
}
void acquire_window(accelWindow window)
{
    accelReading sample;
    unsigned samplesInWindow = 0;

    while (samplesInWindow < ACCEL_WINDOW_SIZE) {
        accel_sample(seed, &sample);
	      seed++;
        printf("acquire: sample %u %u %u\r\n", sample.x, sample.y, sample.z);

        window[samplesInWindow++] = sample;
    }
}

void transform(accelWindow window)
{
    unsigned i = 0;

    printf("transform\r\n");

    for (i = 0; i < ACCEL_WINDOW_SIZE; i++) {
        accelReading *sample = &window[i];

        if (sample->x < SAMPLE_NOISE_FLOOR ||
            sample->y < SAMPLE_NOISE_FLOOR ||
            sample->z < SAMPLE_NOISE_FLOOR) {

            printf("transform: sample %u %u %u\r\n",
                sample->x, sample->y, sample->z);

            sample->x = (sample->x > SAMPLE_NOISE_FLOOR) ? sample->x : 0;
            sample->y = (sample->y > SAMPLE_NOISE_FLOOR) ? sample->y : 0;
            sample->z = (sample->z > SAMPLE_NOISE_FLOOR) ? sample->z : 0;
        }
    }
}

void featurize(features_t *features, accelWindow aWin)
{
    accelReading mean;
    accelReading stddev;

    mean.x = mean.y = mean.z = 0;
    stddev.x = stddev.y = stddev.z = 0;
    int i;
    for (i = 0; i < ACCEL_WINDOW_SIZE; i++) {
        mean.x += aWin[i].x;  // x
        mean.y += aWin[i].y;  // y
        mean.z += aWin[i].z;  // z
    }
    /*
       mean.x = mean.x / ACCEL_WINDOW_SIZE;
       mean.y = mean.y / ACCEL_WINDOW_SIZE;
       mean.z = mean.z / ACCEL_WINDOW_SIZE;
       */
    mean.x >>= 2;
    mean.y >>= 2;
    mean.z >>= 2;

    for (i = 0; i < ACCEL_WINDOW_SIZE; i++) {
        stddev.x += aWin[i].x > mean.x ? aWin[i].x - mean.x
            : mean.x - aWin[i].x;  // x
        stddev.y += aWin[i].y > mean.y ? aWin[i].y - mean.y
            : mean.y - aWin[i].y;  // y
        stddev.z += aWin[i].z > mean.z ? aWin[i].z - mean.z
            : mean.z - aWin[i].z;  // z
    }
    /*
       stddev.x = stddev.x / (ACCEL_WINDOW_SIZE - 1);
       stddev.y = stddev.y / (ACCEL_WINDOW_SIZE - 1);
       stddev.z = stddev.z / (ACCEL_WINDOW_SIZE - 1);
       */
    stddev.x >>= 2;
    stddev.y >>= 2;
    stddev.z >>= 2;

    unsigned meanmag = mean.x*mean.x + mean.y*mean.y + mean.z*mean.z;
    unsigned stddevmag = stddev.x*stddev.x + stddev.y*stddev.y + stddev.z*stddev.z;

    features->meanmag   = sqrt(meanmag);
    features->stddevmag = sqrt(stddevmag);

    printf("featurize: mean %u sd %u\r\n", features->meanmag, features->stddevmag);
}

class_t classify(features_t *features, model_t *model)
{
    int move_less_error = 0;
    int stat_less_error = 0;
    features_t *model_features;
    int i;

    for (i = 0; i < MODEL_SIZE; ++i) {
        model_features = &model->stationary[i];

        long int stat_mean_err = (model_features->meanmag > features->meanmag)
            ? (model_features->meanmag - features->meanmag)
            : (features->meanmag - model_features->meanmag);

        long int stat_sd_err = (model_features->stddevmag > features->stddevmag)
            ? (model_features->stddevmag - features->stddevmag)
            : (features->stddevmag - model_features->stddevmag);

        model_features = &model->moving[i];

        long int move_mean_err = (model_features->meanmag > features->meanmag)
            ? (model_features->meanmag - features->meanmag)
            : (features->meanmag - model_features->meanmag);

        long int move_sd_err = (model_features->stddevmag > features->stddevmag)
            ? (model_features->stddevmag - features->stddevmag)
            : (features->stddevmag - model_features->stddevmag);

        if (move_mean_err < stat_mean_err) {
            move_less_error++;
        } else {
            stat_less_error++;
        }

        if (move_sd_err < stat_sd_err) {
            move_less_error++;
        } else {
            stat_less_error++;
        }
    }

    class_t class = move_less_error > stat_less_error ?
                        CLASS_MOVING : CLASS_STATIONARY;
    printf("classify: class %u\r\n", class);

    return class;
}

void record_stats(stats_t *stats, class_t class)
{
    stats->totalCount++;

    switch (class) {
        case CLASS_MOVING:
            stats->movingCount++;
            break;
        case CLASS_STATIONARY:
            stats->stationaryCount++;
            break;
    }
    printf("stats: s %u m %u t %u\r\n",
        stats->stationaryCount, stats->movingCount, stats->totalCount);
}

void print_stats(stats_t *stats)
{
    unsigned resultStationaryPct = stats->stationaryCount * 100 / stats->totalCount;
    unsigned resultMovingPct = stats->movingCount * 100 / stats->totalCount;

    unsigned sum = stats->stationaryCount + stats->movingCount;

    printf("stats: s %u (%u%%) m %u (%u%%) sum/tot %u/%u: %c\r\n",
           stats->stationaryCount, resultStationaryPct,
           stats->movingCount, resultMovingPct,
           stats->totalCount, sum,
           sum == stats->totalCount && sum == SAMPLES_TO_COLLECT ? 'V' : 'X');
}

void warmup_sensor()
{
    unsigned discardedSamplesCount = 0;
    accelReading sample;

    printf("warmup\r\n");

    while (discardedSamplesCount++ < NUM_WARMUP_SAMPLES) {
        accel_sample(seed, &sample);
	seed++;
    }
}

void train(features_t *classModel)
{
    accelWindow sampleWindow;
    features_t features;
    unsigned i;

    warmup_sensor();

    for (i = 0; i < MODEL_SIZE; ++i) {
        acquire_window(sampleWindow);
        transform(sampleWindow);
        featurize(&features, sampleWindow);

        classModel[i] = features;
    }

 printf("train: done: mn %u sd %u\r\n",
        features.meanmag, features.stddevmag);
}

void recognize(model_t *model)
{
    stats_t stats;
    accelWindow sampleWindow;
    features_t features;
    class_t class;
    unsigned i;

    stats.totalCount = 0;
    stats.stationaryCount = 0;
    stats.movingCount = 0;

    for (i = 0; i < SAMPLES_TO_COLLECT; ++i) {
        acquire_window(sampleWindow);
        transform(sampleWindow);
        featurize(&features, sampleWindow);
        class = classify(&features, model);
        record_stats(&stats, class);
    }

    print_stats(&stats);
}

run_mode_t select_mode(uint8_t *prev_pin_state)
{
    uint8_t pin_state = 1;
	  count++;
	  printf("count: %u\r\n", count);
	  if(count >= 3) pin_state = 2;
	  if(count >= 5) pin_state = 0;
	  if(count >= 7) {   
        printf("Done\r\n");
		    while(1);
		}
    // Don't re-launch training after finishing training
    if ((pin_state == MODE_TRAIN_STATIONARY ||
        pin_state == MODE_TRAIN_MOVING) &&
        pin_state == *prev_pin_state) {
        pin_state = MODE_IDLE;
    } else {
        *prev_pin_state = pin_state;
    }

    printf("selectMode: pins %04x\r\n", pin_state);

    return (run_mode_t)pin_state;
}

static void init_accel()
{
#ifdef ACCEL_16BIT_TYPE
    threeAxis_t accelID = {0};
#else
    threeAxis_t_8 accelID = {0};
#endif
}

int benchmark_main(void)
{
    // "Globals" must be on the stack because Mementos doesn't handle real
    // globals correctly
    uint8_t prev_pin_state = MODE_IDLE;
    model_t model;

    while (1)
    {
        run_mode_t mode = select_mode(&prev_pin_state);
        switch (mode) {
            case MODE_TRAIN_STATIONARY:
                printf("mode: train stationary\r\n");
                train(model.stationary);
                break;
            case MODE_TRAIN_MOVING:
                printf("mode: train moving\r\n");
                train(model.moving);
                break;
            case MODE_RECOGNIZE:
                printf("mode: recognize\r\n");
                recognize(&model);
                break;
            default:
                printf("mode: idle\r\n");
                break;
        }
    }

    return 0;
}

