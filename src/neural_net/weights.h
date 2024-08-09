#include "nnom.h"

/* Weights, bias and Q format */
#define TENSOR_CONV2D_KERNEL_0 {-33, 17, 44, 27, 76, 76, 72, 21, 29, 2, 52, 20, 57, 75, 66, -27, 74, 11, 70, 92, 66, -56, -37, 51, -23, -66, -90, 10, 45, 67, 61, 87, -8, 33, -10, -47}

#define TENSOR_CONV2D_KERNEL_0_DEC_BITS {7}

#define TENSOR_CONV2D_BIAS_0 {-1, 69, 59, -7}

#define TENSOR_CONV2D_BIAS_0_DEC_BITS {10}

#define CONV2D_BIAS_LSHIFT {4}

#define CONV2D_OUTPUT_RSHIFT {9}

#define TENSOR_CONV2D_1_KERNEL_0 {21, 45, -20, -11, 36, 6, -30, 9, -27, -9, -44, -32, 28, 4, -39, 37, 5, -3, -53, 35, -20, -37, -37, 9, 31, -22, -53, 12, 27, 5, -14, 10, 34, -9, -45, -24, -22, -3, 67, 37, 14, -29, 17, 47, -8, -34, 22, -43, -41, -41, 32, -55, -44, -23, -17, -46, 39, -5, -38, -12, 34, 13, -47, -30, 16, 14, -38, 0, 48, 0, -8, 32, 25, 27, 4, -23, 29, -12, -1, -20, 3, 31, -13, 3, 18, -12, 41, -4, -22, 2, 48, 20, 15, 39, 75, 35, -4, -14, 37, -37, -29, -16, -17, -40, -64, -39, 12, -49, 5, -38, -19, -20, -37, -27, -60, -18, -33, -40, -64, -25, 32, 16, -8, 12, 35, -5, 7, -9, 43, 36, -5, -3, -12, 30, 7, 0, -9, 5, -33, 4, 36, 0, -18, 34, -52, -44, 15, 24, -45, -25, 18, -12, -53, -53, 83, 5, -13, -38, -33, -34, -5, -9, -40, -6, 9, -23, -13, -5, 31, 42, 80, 31, -2, 1, 81, 38, 23, 11, 12, 31, 10, -28, -17, 19, -2, 3, 9, 45, 6, -11, 28, 40, 46, 18, 28, 58, -38, -45, 17, 20, -59, -28, 16, -46, -23, -37, -29, 8, -14, -27, -63, -5, 36, 33, 8, 40, -45, -30, -25, -18, 14, 3, -52, -19, 12, 48, -56, 49, -58, -13, -48, -35, 33, -2, -85, 2, 42, -8, -20, 53, 11, -40, -73, -56, 32, 28, -24, -3, -23, 19, -18, 15, 41, 55, 33, 31, 26, -11, 7, 43, -17, -17, -27, -35, -39, -2, 37, -17, -15, -10, -2, -40, 0, -19, -50, -69, -33, -50, 13, -11, -55, -1, 1, -64, -14, 17, -16, -21}

#define TENSOR_CONV2D_1_KERNEL_0_DEC_BITS {7}

#define TENSOR_CONV2D_1_BIAS_0 {-78, 2, -47, 55, -20, -5, -37, 78}

#define TENSOR_CONV2D_1_BIAS_0_DEC_BITS {9}

#define CONV2D_1_BIAS_LSHIFT {3}

#define CONV2D_1_OUTPUT_RSHIFT {8}

#define TENSOR_DENSE_KERNEL_0 {-18, -24, -44, -10, 13, 28, 6, 15, 6, -30, 0, -8, -10, 6, 7, -8, -11, -25, -26, -14, 7, 23, -8, 0, 1, -11, -4, 49, -17, -7, -4, -5, -9, -27, -32, -52, -12, 4, 15, 5, -7, 3, 5, -8, -4, 1, 9, 15, 4, 7, 5, 1, 9, 12, 11, 10, -26, -21, -13, 1, 4, 7, 21, -18, 3, 18, -4, -15, 15, 3, 19, -11, 4, -4, -16, 13, 4, 2, 0, 3, 5, -14, -14, 9, 1, 5, -14, -29, -7, -15, -8, -8, -7, -39, -4, -39, 11, 3, -20, -3, -6, 6, 11, -2, 5, 3, 0, -17, -25, -2, -3, 7, 8, -36, 8, 21, -17, -4, -37, -40, -27, 49, -34, -12, -25, -18, -43, -53, 9, 6, -12, -18, 11, 4, -4, -11, 8, -3, -1, -5, -61, -37, -10, 7, 7, -18, -27, 18, -22, -2, -57, -47, -8, 1, -43, -11, -37, -1, -47, -44, 6, -20, -40, -4, -18, -15, 17, 15, -3, -28, 9, -16, -2, -13, -19, -14, 8, 5, 4, -12, -8, 18, -18, -6, 6, -19, 3, 3, 0, 24, 4, 18, -12, -41, -1, -18, -44, -43, 9, 14, 21, -40, 16, -21, -18, -1, -18, -16, 12, -15, 4, -11, 21, 30, -6, -12, -11, -60, -16, 14, -13, 28, 19, -24, -4, 1, 10, -45, -38, -19, 7, -9, 0, 11, -20, 1, -18, 26, -28, -23, -13, -11, 0, 13, 4, 10, -13, -10, -9, -55, 10, 4, 3, 15, 12, -24, -2, 37, 6, 13, 2, 15, -10, -14, -50, 22, -18, 11, -12, -3, -33, -21, -22, -11, -6, 29, -17, -21, 0, 1, -14, 1, 7, 4, -2, 3, -21, -25, 21, -8, -8, -5, 4, 15, -4, -1, -18, -51, 21, -61, -42, -57, -34, -38, -19, -30, -13, 0, -39, -44, -13, -26, -29, -11, -59, -20, -3, 16, -76, -36, -5, -4, -42, 14, -17, -19, 13, 11, 7, -24, 2, -10, 32, -31, 9, -28, 17, 2, 20, -15, 25, 12, -12, -29, -21, -6, -23, 24, 53, 15, -5, 35, 21, -19, -15, 3, -13, -56, 6, 7, 9, 9, 3, 6, 20, 13, 2, 18, -36, -7, 20, -9, 26, 16, -12, -5, 2, 7, 24, 25, 24, 31, 28, 2, -8, 10, -16, -4, -16, -9, -22, 13, -50, 23, -48, 18, 26, 16, 0, -9, -60, 8, -17, 27, 20, -5, -13, 4, 32, -16, 5, 14, -11, 8, 25, -13, -36, 38, -33, -2, 5, -1, -23, 18, 4, -53, -35, -9, 14, -29, -5, -8, -38, -60, 16, -12, 4, 2, 28, -19, 2, 14, -9, -1, -17, 4, 33, -14, 2, -41, 14, -29, 6, -6, -7, 20, 12, -48, -31, -24, -6, -6, -12, -12, -54, -44, 46, -6, -27, 8, 28, -64, -23, -26, -39, -13, 16, 25, -4, 34, 19, -33, -16, 3, 24, 59, -40, -20, 19, 16, -10, 8, 22, -3, 30, 5, -47, -8, 6, -24, 10, 26, 3, -35, -61, -2, 5, 9, 7, 12, 22, -1, 26, -9, -28, -30, 8, -15, -20, 12, -16, 29, -10, -8, 31, -18, 25, -12, -60, 1, 1, -5, -16, 8, 10, -24, 21, -45, -30, 24, 9, -23, 2, -15, 24, 17, -35, 14, 16, -37, -2, 22, -2, 2, 8, 10, -11, -4, 4, -10, 2, -13, -27, 33, 2, 21, 3, -19, -7, -13, -10, 18, 11, -4, -19, -29, -24, 20, -14, 7, 5, -14, -3, -10, 23, 2, 33, 4, 9, 19, 22, -10, 12, -22, 6, -27, 4, 5, 4, -20, -29, 29, -26, 22, 3, -16, 4, -32, -5, -29, 5, -46, -1, 19, 7, -8, 16, 4, -10, 23, 33, -10, 17, -16, -10, 44, 49, -25, 54, -30, -20, -4, 0, -51, 32, -13, -2, -7, -4, -24, 3, -31, -4, -24, 22, 35, 13, 30, -11, 12, -22, 16, 3, 9, 7, 17, -43, 4, 24, 14, 4, 40, 25, 4, -38, -11, -13, 26, 9, 24, 16, -13, 31, 0, 13, 2, 0, 1, 14, 9, -41, -1, -11, 4, -6, 18, -8, 26, -18, 16, 14, 8, -21, 20, 12, -18, -19, -12, -17, 26, 4, 2, 5, -34, 23, 13, 5, 13, 4, 10, 18, 3, -18, -10, 33, 4, -21, 26, -18, 21, 16, -28, -21, 26, -20, 25, -9, -17, -10, -1, -1, 39, 16, -26, 14, -14, -4, -1, 10, -1, -33, 6, 5, -4, 26, -6, 30, 0, -17, 9, 12, -9, 29, -20, 9, -14, 8, 18, -33, 6, 22, -14, 26, -20, 9, 1, -9, 11, -21, 4, 8, -13, -6, 6, 25, 12, 14, -33, -19, 51, 15, -19, 61, -47, 10, 49, 18, -30, 76, -43, -11, 3, 11, -9, 28, -29, 11, 28, -6, 20, -3, -19, 2, -40, -22, 32, -20, -8, -10, -5, 11, -5, -11, 2, -4, -7, -2, -19, -6, -11, -3, 34, 22, -15, 1, 15, 15, 11, -34, 1, -19, -17, -8, -3, -7, -38, -1, 20, -12, -14, -7, 7, 2, 2, -19, 2, -14, 10, -12, 1, -16, 0, -27, 10, 3, -37, -4, -24, 3, 7, -10, 0, -7, -17, -19, 6, -16, 4, -4, 1, 20, -44, -20, -5, -19, -10, 3, 1, -19, -14, -13, -3, -8, -19, -32, 15, 4, -52, 0, -33, -16, 0, 31, -53, 15, -27, -8, -18, -6, 0, -20, -16, 15, -18, -11, 27, -2, 1, -20, 24, -11, -5, -26, 8, 35, -11, -19, 1, 42, -29, 41, -15, -50, 45, 15, -53, 42, -11, 4, 6, 20, 15, -1, -12, 32, -28, -5, 35, -11, 17, -17, 1, -10, -14, 8, 21, 29, 17, -10, 7, 2, -11, 35, 27, -9, 43, 29, -42, 30, -34, -7, -39, -45, -32, -13, 17, -72, 20, 22, -14, -5, 6, 4, -6, 12, 10, 2, -26, 0, -4, -24, 3, 16, -12, -10, -50, 7, -13, -15, -16, -34, 0, 38, 24, -46, -33, 5, 13, -36, 16, 2, -1, 9, 11, 12, -29, 9, 11, -1, -13, 25, 15, -3, 7, 16, 6, 4, -19, -13, -44, -1, -19, 14, 2, 30, 5, -43, 14, 1, 0, -1, 18, -23, -18, -12, -17, -10, -22, -2, 9, -33, -2, 8, -2, -19, 10, -1, -22, -10, 3, -10, -19, 19, -15, 18, 3, 10, -25, -48, 32, -10, 7, -1, -14, 5, 4, 5, 11, 34, -15, -7, 19, -27, 28, 49, -39, 10, 16, 5, -9, -14, 7, -22, 34, 28, -9, 25, -20, 7, -42, 16, 41, -34, 24, -3, 11, -17, 50, -12, 19, 55, -24, 1, 22, -7, 26, 49, -35, 32, -17, -27, -14, 36, 8, -12, 28, 11, 6, 28, -9, 12, -5, -9, 13, -15, -5, -47, 12, 26, -17, -3, -24, -4, 7, 9, -5, 14, -39, 13, 1, -7, 11, 18, -17, -9, -60, -3, -29, -33, 33, -15, -4, 21, 4, 6, 10, 23, -9, -38, 8, -15, -16, 13, -9, -10, 1, 16, -10, -40, -28, 15, 5, -47, 10, 18, -6, -13, -34, -21, -32, -2, 21, 4, 3, 24, 6, -13, 15, 17, -35, -12, -4, -3, -42, 13, -7, -4, 13, 14, -4, -1, 1, -16, -18, -27, -13, 9, 18, 1, 10, 1, -22, 13, 26, -5, 48, 10, -11, -23, 11, 2, 8, -13, -2, -15, -28, 13, 11, 17, -3, 24, -29, 3, 10, 36, -62, 41, -60, -13, 32, -34, -17, -21, -10, 7, 6, 3, 6, 21, -34, 13, 5, -6, 11, -28, 7, -24, 16, 1, 18, 9, 2, 18, -23, 9, 24, 12, -84, 19, 19, -4, 3, 17, -13, -23, 2, -26, 6, -9, -18, -23, 7, 22, 12, -33, -24, -30, -9, -5, -7, -11, 11, -5, -20, -14, -20, -8, 27, 1, -16, -49, 10, -5, -8, 2, 48, -2, -19, -58, -7, -27, 36, -4, -4, 17, 15, 20, 3, -27, -3, -11, -13, -32, -16, -30, 18, -9, 13, -33, 32, -10, -15, -47, -13, 3, -16, 13, 2, -14, -15, -44, -1, -43, 50, -6, 2, 24, 14, 6, -25, -2, -33, -5, 9, -2, -6, -11, 3, -7, 22, 24, 7, -15, -22, -25, 9, -2, 7, 2, -11, 1, 2, 10, -24, -19, 40, 4, -2, 23, -23, -7, -13, -16, 10, -21, 12, 10, 3, 1, -17, 12, -3, -5, 2, -7, 0, 10, -16, -6, 23, 5, -30, 8, -31, -5, 6, -15, 22, 2, 7, 23, -28, -1, -8, 13, 19, -3, -41, 5, -13, 3, -16, 4, -33, 11, -40, -7, -21, -14, -5, 39, -18, 21, -45, 11, -71, -2, 24, -31, 11, -42, -14, -17, 27, -25, 2, -2, -38, -11, -6, -39, 16, -36, 26, -36, -15, 24, 34, -37, 0, -44, -12, 14, -24, 33, 29, 4, -23, -30, 22, -4, 27, -55, -1, -6, 12, -26, -1, 28, -22, 4, -6, -21, 14, -69, 25, -54, -7, -10, 18, -12, -9, -56, -33, 18, -8, 14, 7, 4, 5, -53, -1, -30, 27, -17, 17, -2, 1, -1, 12, 24, -31, 14, 28, -35, 16, -45, -2, -5, -12, -52, 22, 14, -17, -20, -28, -45, 20, -7, -11, -15, 9, -12, -7, -26, 18, -12, 9, -18, 1, -17, -6, 8, -26, 3, 22, -75, -3, 2, -29, 4, -44, -63, 10, 8, 3, 13, -59, -17, 21, -3, -1, -8, 20, 14, 7, 2, 10, -2, -1, -15, 5, 3, -36, 27, -15, 8, -23, 1, 2, -8, 12, -35, -58, 15, 6, -16, 21, -19, -33, -13, -51, -25, 5, -9, -10, 31, -14, 7, 18, -8, 12, -47, 5, -11, 9, -10, 20, 2, 1, -11, 11, 14, 33, 15, 19, -5, 14, 26, -3, 18, 6, 6, -14, -9, 9, 5, 4, 11, 3, -3, 9, 15, -7, -1, 10, 22, 7, -6, -18, -16, 19, -25, 19, 20, -27, 8, -13, 18, 5, -4, 4, 5, -12, 17, -62, -7, -19, 9, -28, 3, 6, -1, -28, -5, 13, 4, -24, -4, 21, -12, -10, 16, 30, 1, 14, 1, 5, 4, -23, -2, -10, 25, -25, -17, -14, -4, -5, -1, -19, 3, -21, -13, -22, -2, -80, 5, -16, -3, -3, -25, -23, -6, -70, -4, -1, 23, 4, 8, 10, -3, -19, -12, 16, 39, -36, 11, 38, 14, 24, -36, 28, -15, 28, 0, 32, 14, -7, 2, 16, -8, 12, 13, 4, 9, -54, 14, -53, 12, -40, -61, 21, 2, -22, -8, -18, 5, -42, -22, 8, -53, -36, -12, 7, -5, 2, 4, -6, -30, -1, -14, 1, -8, -8, -28, -60, 5, -22, 4, -25, -5, -26, -45, 5, 21, -56, 21, -11, -8, -26, 16, -2, -24, -69, 6, -1, -7, -21, 11, -9, -1, -29, 8, -7, 7, -16, -19, 7, 4, 0, 6, 10, -27, -8, -21, 3, -34, 7, -10, 9, -25, 10, 8, 28, -21, 3, -6, 21, -32, -3, 21, 10, -25, 11, -20, 17, -22, 5, 19, -42, -35, -28, -19, -25, -35, -14, 13, -58, -60, -8, 18, 29, -57, 9, 15, 25, 8, -26, 4, 20, -4, 23, 13, 24, 20, -28, -5, 11, 5, -1, 1, 23, 3, 0, 10, -14, 0, 0, -19, 8, -35, -5, 16, -27, -8, -3, -16, -22, -55, -23, 32, -8, -13, -9, -22, -44, 13, -22, 17, -40, 9, -19, -42, 2, 8, -25, -10, -3, -1, -3, 9, 9, -22, -28, 15, -2, 10, -10, -8, -22, -1, -20, 21, -36, 11, -36, -70, 13, -2, -2, -2, -58, 21, -67, -44, -45, -4, -19, 4, -44, 16, -31, 3, -4, -23, -59, -31, 21, 20, -17, 27, 15, -19, 4, -6, -13, 0, -40, -14, 5, -2, 18, 6, -25, -1, -72, -98, 14, -4, 5, -23, -21, -14, -25, 0, 15, -24, -21, -31, 0, 14, -3, 16, 34, -8, -17, 3, 37, 6, -13, -15, 6, 10, 20, -9, -5, -18, -28, -30, -3, 12, 26, -5, -28, -36, -38, -18, -26, 0, -9}

#define TENSOR_DENSE_KERNEL_0_DEC_BITS {7}

#define TENSOR_DENSE_BIAS_0 {-10, 104, -28, -29, 25, 2, -10, -2, -15, -31}

#define TENSOR_DENSE_BIAS_0_DEC_BITS {9}

#define DENSE_BIAS_LSHIFT {2}

#define DENSE_OUTPUT_RSHIFT {9}


/* output q format for each layer */
#define INPUT_1_OUTPUT_DEC 7
#define INPUT_1_OUTPUT_OFFSET 0
#define CONV2D_OUTPUT_DEC 5
#define CONV2D_OUTPUT_OFFSET 0
#define MAX_POOLING2D_OUTPUT_DEC 5
#define MAX_POOLING2D_OUTPUT_OFFSET 0
#define CONV2D_1_OUTPUT_DEC 4
#define CONV2D_1_OUTPUT_OFFSET 0
#define MAX_POOLING2D_1_OUTPUT_DEC 4
#define MAX_POOLING2D_1_OUTPUT_OFFSET 0
#define FLATTEN_OUTPUT_DEC 4
#define FLATTEN_OUTPUT_OFFSET 0
#define DROPOUT_OUTPUT_DEC 4
#define DROPOUT_OUTPUT_OFFSET 0
#define DENSE_OUTPUT_DEC 2
#define DENSE_OUTPUT_OFFSET 0
#define SOFTMAX_OUTPUT_DEC 7
#define SOFTMAX_OUTPUT_OFFSET 0

/* bias shift and output shift for none-weighted layer */

/* tensors and configurations for each layer */
static int8_t nnom_input_data[784] = {0};

const nnom_shape_data_t tensor_input_1_dim[] = {28, 28, 1};
const nnom_qformat_param_t tensor_input_1_dec[] = {7};
const nnom_qformat_param_t tensor_input_1_offset[] = {0};
const nnom_tensor_t tensor_input_1 = {
    .p_data = (void*)nnom_input_data,
    .dim = (nnom_shape_data_t*)tensor_input_1_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_input_1_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_input_1_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 3,
    .bitwidth = 8
};

const nnom_io_config_t input_1_config = {
    .super = {.name = "input_1"},
    .tensor = (nnom_tensor_t*)&tensor_input_1
};
const int8_t tensor_conv2d_kernel_0_data[] = TENSOR_CONV2D_KERNEL_0;

const nnom_shape_data_t tensor_conv2d_kernel_0_dim[] = {3, 3, 1, 4};
const nnom_qformat_param_t tensor_conv2d_kernel_0_dec[] = TENSOR_CONV2D_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv2d_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_conv2d_kernel_0 = {
    .p_data = (void*)tensor_conv2d_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv2d_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv2d_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv2d_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 4,
    .bitwidth = 8
};
const int8_t tensor_conv2d_bias_0_data[] = TENSOR_CONV2D_BIAS_0;

const nnom_shape_data_t tensor_conv2d_bias_0_dim[] = {4};
const nnom_qformat_param_t tensor_conv2d_bias_0_dec[] = TENSOR_CONV2D_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv2d_bias_0_offset[] = {0};
const nnom_tensor_t tensor_conv2d_bias_0 = {
    .p_data = (void*)tensor_conv2d_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv2d_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv2d_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv2d_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t conv2d_output_shift[] = CONV2D_OUTPUT_RSHIFT;
const nnom_qformat_param_t conv2d_bias_shift[] = CONV2D_BIAS_LSHIFT;
const nnom_conv2d_config_t conv2d_config = {
    .super = {.name = "conv2d"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_conv2d_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_conv2d_bias_0,
    .output_shift = (nnom_qformat_param_t *)&conv2d_output_shift, 
    .bias_shift = (nnom_qformat_param_t *)&conv2d_bias_shift, 
    .filter_size = 4,
    .kernel_size = {3, 3},
    .stride_size = {1, 1},
    .padding_size = {0, 0},
    .dilation_size = {1, 1},
    .padding_type = PADDING_VALID
};

const nnom_pool_config_t max_pooling2d_config = {
    .super = {.name = "max_pooling2d"},
    .padding_type = PADDING_VALID,
    .output_shift = 0,
    .kernel_size = {2, 2},
    .stride_size = {2, 2},
    .num_dim = 2
};
const int8_t tensor_conv2d_1_kernel_0_data[] = TENSOR_CONV2D_1_KERNEL_0;

const nnom_shape_data_t tensor_conv2d_1_kernel_0_dim[] = {3, 3, 4, 8};
const nnom_qformat_param_t tensor_conv2d_1_kernel_0_dec[] = TENSOR_CONV2D_1_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv2d_1_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_conv2d_1_kernel_0 = {
    .p_data = (void*)tensor_conv2d_1_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv2d_1_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv2d_1_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv2d_1_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 4,
    .bitwidth = 8
};
const int8_t tensor_conv2d_1_bias_0_data[] = TENSOR_CONV2D_1_BIAS_0;

const nnom_shape_data_t tensor_conv2d_1_bias_0_dim[] = {8};
const nnom_qformat_param_t tensor_conv2d_1_bias_0_dec[] = TENSOR_CONV2D_1_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv2d_1_bias_0_offset[] = {0};
const nnom_tensor_t tensor_conv2d_1_bias_0 = {
    .p_data = (void*)tensor_conv2d_1_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv2d_1_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv2d_1_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv2d_1_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t conv2d_1_output_shift[] = CONV2D_1_OUTPUT_RSHIFT;
const nnom_qformat_param_t conv2d_1_bias_shift[] = CONV2D_1_BIAS_LSHIFT;
const nnom_conv2d_config_t conv2d_1_config = {
    .super = {.name = "conv2d_1"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_conv2d_1_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_conv2d_1_bias_0,
    .output_shift = (nnom_qformat_param_t *)&conv2d_1_output_shift, 
    .bias_shift = (nnom_qformat_param_t *)&conv2d_1_bias_shift, 
    .filter_size = 8,
    .kernel_size = {3, 3},
    .stride_size = {1, 1},
    .padding_size = {0, 0},
    .dilation_size = {1, 1},
    .padding_type = PADDING_VALID
};

const nnom_pool_config_t max_pooling2d_1_config = {
    .super = {.name = "max_pooling2d_1"},
    .padding_type = PADDING_VALID,
    .output_shift = 0,
    .kernel_size = {2, 2},
    .stride_size = {2, 2},
    .num_dim = 2
};

const nnom_flatten_config_t flatten_config = {
    .super = {.name = "flatten"}
};
const int8_t tensor_dense_kernel_0_data[] = TENSOR_DENSE_KERNEL_0;

const nnom_shape_data_t tensor_dense_kernel_0_dim[] = {200, 10};
const nnom_qformat_param_t tensor_dense_kernel_0_dec[] = TENSOR_DENSE_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_kernel_0 = {
    .p_data = (void*)tensor_dense_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_bias_0_data[] = TENSOR_DENSE_BIAS_0;

const nnom_shape_data_t tensor_dense_bias_0_dim[] = {10};
const nnom_qformat_param_t tensor_dense_bias_0_dec[] = TENSOR_DENSE_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_bias_0 = {
    .p_data = (void*)tensor_dense_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_output_shift[] = DENSE_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_bias_shift[] = DENSE_BIAS_LSHIFT;
const nnom_dense_config_t dense_config = {
    .super = {.name = "dense"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_bias_shift
};

const nnom_softmax_config_t softmax_config = {
    .super = {.name = "softmax"}
};
static int8_t nnom_output_data[10] = {0};

const nnom_shape_data_t tensor_output0_dim[] = {10};
const nnom_qformat_param_t tensor_output0_dec[] = {SOFTMAX_OUTPUT_DEC};
const nnom_qformat_param_t tensor_output0_offset[] = {0};
const nnom_tensor_t tensor_output0 = {
    .p_data = (void*)nnom_output_data,
    .dim = (nnom_shape_data_t*)tensor_output0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_output0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_output0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_io_config_t output0_config = {
    .super = {.name = "output0"},
    .tensor = (nnom_tensor_t*)&tensor_output0
};
/* model version */
#define NNOM_MODEL_VERSION (10000*0 + 100*4 + 3)

/* nnom model */
static nnom_model_t* nnom_model_create(void)
{
	static nnom_model_t model;
	nnom_layer_t* layer[9];

	check_model_version(NNOM_MODEL_VERSION);
	new_model(&model);

	layer[0] = input_s(&input_1_config);
	layer[1] = model.hook(conv2d_s(&conv2d_config), layer[0]);
	layer[2] = model.hook(maxpool_s(&max_pooling2d_config), layer[1]);
	layer[3] = model.hook(conv2d_s(&conv2d_1_config), layer[2]);
	layer[4] = model.hook(maxpool_s(&max_pooling2d_1_config), layer[3]);
	layer[5] = model.hook(flatten_s(&flatten_config), layer[4]);
	layer[6] = model.hook(dense_s(&dense_config), layer[5]);
	layer[7] = model.hook(softmax_s(&softmax_config), layer[6]);
	layer[8] = model.hook(output_s(&output0_config), layer[7]);
	model_compile(&model, layer[0], layer[8]);
	return &model;
}
