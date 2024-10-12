#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdint.h>

#define ARRAY_SIZE 640

typedef struct {
    int16_t x, y, z;
    float distance;
} Vertex;

Vertex vertices[] =  {
    {  24999, -17453, -31787, 0.0 },
    { -20888, -28191, -21767, 0.0 },
    { -19868,  13714,  -1729, 0.0 },
    { -30484, -28730, -30692, 0.0 },
    {  12594,  30703,  27990, 0.0 },
    { -13297, -20803,  -8770, 0.0 },
    { -17772, -30852,  31031, 0.0 },
    {    -23, -24306,  28298, 0.0 },
    { -23781, -21112, -26744, 0.0 },
    {   2567, -24501,   6871, 0.0 },
    {  30282,  13793,   2156, 0.0 },
    {   -112,  -6550,  26658, 0.0 },
    { -30239, -23516, -32011, 0.0 },
    {  10945, -13924,  29206, 0.0 },
    {  16691, -10865, -25668, 0.0 },
    { -22776,  32319, -11272, 0.0 },
    { -22896,  -4576, -13862, 0.0 },
    { -29213,  12613, -18234, 0.0 },
    { -14257,   7260,  26757, 0.0 },
    { -27579,  21790,   3510, 0.0 },
    { -27499,  -9524,   2840, 0.0 },
    { -12509,   6589, -12511, 0.0 },
    {  28115, -27386,  17849, 0.0 },
    {  13053,  32224,  12020, 0.0 },
    {  28589,  29613, -26827, 0.0 },
    {  28945, -10490,  24850, 0.0 },
    {  24624,   5987,   3282, 0.0 },
    { -16532,  -8439,  21800, 0.0 },
    {  24387, -17110, -19256, 0.0 },
    {  30828,  16622,    717, 0.0 },
    {  -7680,  12497, -18592, 0.0 },
    { -12168,   2533, -12472, 0.0 },
    {   9632,  25778, -23492, 0.0 },
    { -22480,  19037,  -4699, 0.0 },
    { -26512,   4736, -27788, 0.0 },
    {  -4012, -21278,   8779, 0.0 },
    {   7290,  27634, -24088, 0.0 },
    {    973, -23200,  23108, 0.0 },
    {  -6305, -19492,  19221, 0.0 },
    {  -7262,  27468,  -1793, 0.0 },
    {  14582,  -5888,  22436, 0.0 },
    {  -9872,  -6127,   6786, 0.0 },
    { -24863,  13433, -12707, 0.0 },
    {  32713,  29155,  10978, 0.0 },
    { -14585, -15607, -26468, 0.0 },
    { -11131, -13772, -23626, 0.0 },
    {   9867, -10352, -32545, 0.0 },
    { -24796, -21284, -13334, 0.0 },
    {  -4321, -27537,  23956, 0.0 },
    {  18538,   9753,  -5621, 0.0 },
    {  17196,   3938,   7147, 0.0 },
    {  22549,   9037, -12922, 0.0 },
    { -20596,   1352,   3672, 0.0 },
    {  21005,  19669,   3505, 0.0 },
    { -17861,  16429,   3175, 0.0 },
    {  -4734,   6218,  19443, 0.0 },
    { -25887,  13045,  24976, 0.0 },
    {  -2295,  31522, -11100, 0.0 },
    {  17110,   7591, -32448, 0.0 },
    { -23157,  30173,  19004, 0.0 },
    {   6346,  23450,  30092, 0.0 },
    {  -8444,  19727,  29879, 0.0 },
    { -12165,   4097,  15666, 0.0 },
    {  12646,  -5759, -10620, 0.0 },
    {  27186,  29416, -29060, 0.0 },
    {  28559, -19175, -29241, 0.0 },
    {  15287,  29598, -24224, 0.0 },
    { -14416,  10718,  12961, 0.0 },
    {   5197,   6372,  10562, 0.0 },
    {   5738,  -7266,   3805, 0.0 },
    { -23268,  -6030,  11748, 0.0 },
    {  11040,   9314,  27266, 0.0 },
    {   2294, -21633, -17090, 0.0 },
    {   -623,    937,  15974, 0.0 },
    { -14801, -29049,  18945, 0.0 },
    { -27872, -26992,  -5413, 0.0 },
    { -12477,   8258,  13818, 0.0 },
    {  24190, -23725,   7230, 0.0 },
    {   1497,  30876,  -3985, 0.0 },
    {  12811, -21906, -30389, 0.0 },
    {  10628,  -2674, -11465, 0.0 },
    {  29881,  32691, -14426, 0.0 },
    { -23825, -13693,   -881, 0.0 },
    { -11240,  18763,  10027, 0.0 },
    { -15870,  25734, -27409, 0.0 },
    {  15951, -23765,  15611, 0.0 },
    {  20761, -26919, -23475, 0.0 },
    {   8409,  -2894,  -6791, 0.0 },
    {  12607,  17011,  31822, 0.0 },
    { -25894, -28857,  25962, 0.0 },
    { -22259,   5657,  23120, 0.0 },
    {  11836, -13384,  30952, 0.0 },
    {  -1045, -28862,  13973, 0.0 },
    {  15913,  21627,  -9955, 0.0 },
    { -22982, -30434,  -4946, 0.0 },
    {  -7019, -14228, -15401, 0.0 },
    {    619, -26279,  28579, 0.0 },
    {  14621,  15963,   5034, 0.0 },
    {  19222,  20735,  15449, 0.0 },
    {  -2075,   4622, -31622, 0.0 },
    {   8224,   2097, -16978, 0.0 },
    { -28128,  -4121,  15650, 0.0 },
    {  -8664,   9468,   5626, 0.0 },
    {  14792,   3001,  -9659, 0.0 },
    {  27114, -10591, -32345, 0.0 },
    {  20789,  -1439,  -7639, 0.0 },
    {  25393,   3067,  -9930, 0.0 },
    {  13613,  20894,  30954, 0.0 },
    {  30291,  30172,  25595, 0.0 },
    {  16125, -19323,  19861, 0.0 },
    { -16117, -18862,  28751, 0.0 },
    {  31208, -20086, -15488, 0.0 },
    {    773,  -1446, -23480, 0.0 },
    {  11181,  18636,   5713, 0.0 },
    {  13759,  20379,  -8082, 0.0 },
    {  29219,  28270,   9115, 0.0 },
    {  25110, -18626,  27944, 0.0 },
    { -21513, -22800,  -6887, 0.0 },
    {  -9725,  11258, -10139, 0.0 },
    {  16745,   2817,  -6607, 0.0 },
    {   1910, -28917, -12039, 0.0 },
    {   8655, -19694, -26838, 0.0 },
    { -25469,   1356,  22805, 0.0 },
    {   9592,  11790,   7592, 0.0 },
    {  24082, -10976,  16482, 0.0 },
    {  18947, -31608,   5124, 0.0 },
    {   4417,   8732,   -152, 0.0 },
    { -12310,  -7301,  26600, 0.0 },
    {  25562,  21955,  11324, 0.0 },
    {   2458,  24257,  18583, 0.0 },
    {   1266,  31922,   8538, 0.0 },
    {   -303,   4745, -12085, 0.0 },
    { -11284,  21082, -12108, 0.0 },
    { -28254,  -2287,  13611, 0.0 },
    {  -5732,  23605,  25463, 0.0 },
    { -29596,  20233, -23700, 0.0 },
    { -24500, -23487,   -735, 0.0 },
    {  17187,  30861,  17000, 0.0 },
    {  -1640,   6910,  10128, 0.0 },
    {  26470, -24348,   6912, 0.0 },
    { -29345,  -9099, -14051, 0.0 },
    { -13518,  16442,  23365, 0.0 },
    {  -9560, -14818,   6734, 0.0 },
    { -20476,  31259,  11253, 0.0 },
    { -15263,   2231,  23718, 0.0 },
    {  29098,   3771,   8581, 0.0 },
    { -10813, -24127,  24804, 0.0 },
    {  -2585,  25215,    -33, 0.0 },
    { -24011,   2182, -12299, 0.0 },
    {   3170, -13780,  18883, 0.0 },
    {  -4212,  -6412, -13609, 0.0 },
    {  -4974,  12139,  21386, 0.0 },
    { -32526,  12620,  -7693, 0.0 },
    {  -3455, -30594, -30454, 0.0 },
    {  14833,  24938,  -1782, 0.0 },
    { -31317,  21542,  22590, 0.0 },
    {  32347,  22507,  10702, 0.0 },
    {    -91,  30779,   7271, 0.0 },
    {  28686, -30532, -30122, 0.0 },
    {   5515, -16592,  26518, 0.0 },
    {   -672,  19034,  22172, 0.0 },
    {  -5325, -27250,  23765, 0.0 },
    {  18564, -18151,  21314, 0.0 },
    {  -9439,  -9948, -23285, 0.0 },
    {  15527, -10415,  -2679, 0.0 },
    { -24476,  17030, -28439, 0.0 },
    { -16121,   2390, -28998, 0.0 },
    {  12462, -24912,   3233, 0.0 },
    { -30451, -26846,  21176, 0.0 },
    {  30307, -28781,   5025, 0.0 },
    {  -8836,  -2530,  31454, 0.0 },
    { -24670,   5846,   -866, 0.0 },
    {  25642,  27390,  -9822, 0.0 },
    {   1749, -11635,  21945, 0.0 },
    {   8089, -31519,   5522, 0.0 },
    {   9970,  -5886,  -8449, 0.0 },
    { -28508,   7517,  32729, 0.0 },
    { -30403,  22909, -11478, 0.0 },
    {  24215,  11672,   4826, 0.0 },
    { -20442, -22287,  22149, 0.0 },
    {  11194, -27786,  20750, 0.0 },
    {  25194,  -8824,  20427, 0.0 },
    { -18790,  10644,  -7849, 0.0 },
    { -19631,  19188, -21001, 0.0 },
    {   8905, -16332, -23270, 0.0 },
    {  10531,  30399,   1583, 0.0 },
    { -22762, -24414,  26300, 0.0 },
    { -31391,  13494, -28005, 0.0 },
    {  -4418, -18624,  25679, 0.0 },
    {  -9293, -27330,   2207, 0.0 },
    {  19668,   7920, -28536, 0.0 },
    {  11456,  12154,  -4913, 0.0 },
    { -21250,   -814, -19868, 0.0 },
    {   6523, -30564, -23816, 0.0 },
    { -21741, -29109,   6239, 0.0 },
    {  12510,  23747, -30403, 0.0 },
    {  18464,  26963, -21290, 0.0 },
    {  -4182, -22452,  12901, 0.0 },
    {  11099,  13471,  18121, 0.0 },
    {  13770, -20312, -28674, 0.0 },
    { -15827, -15251,  26367, 0.0 },
    {  14198, -16263, -20129, 0.0 },
    { -24679,   2129, -11989, 0.0 },
    { -28626,  11760,  31947, 0.0 },
    { -21992,  24299,  14427, 0.0 },
    { -30418,  22344,   8502, 0.0 },
    {  -7605, -11017,  -2719, 0.0 },
    {  22726, -10325,  23925, 0.0 },
    {  32709, -14464,  16954, 0.0 },
    {  29631,  17770,   7232, 0.0 },
    {   3521,   1277, -14037, 0.0 },
    {  14126, -15540,   1471, 0.0 },
    {   8058,   -147,   5936, 0.0 },
    { -10790, -12455,  26929, 0.0 },
    { -25093,   1493,  22221, 0.0 },
    {  15558, -26243,  12817, 0.0 },
    { -31619, -27860,  -6911, 0.0 },
    {   7409,  -7929,  29608, 0.0 },
    {  18066,   4227,   8680, 0.0 },
    {  17622,  21713,   2991, 0.0 },
    {  25636, -25401,  11077, 0.0 },
    {    912,  -7212,   8560, 0.0 },
    { -20344,  20051,  15085, 0.0 },
    {  15021,  23210,   6091, 0.0 },
    {  18192,  20785,   4169, 0.0 },
    {    909, -10551,  26777, 0.0 },
    {  14280,  -6435,  -8773, 0.0 },
    { -24614,  16655,  26769, 0.0 },
    { -15385,  -1698,  16456, 0.0 },
    {   7139,  15993,  21108, 0.0 },
    {    535,  -3493,  10028, 0.0 },
    {  25166,   8359, -16533, 0.0 },
    { -11037,    119,  -9662, 0.0 },
    {  20238,  26152,   1363, 0.0 },
    {   3501,  26300, -20855, 0.0 },
    { -11261,  26960,  31491, 0.0 },
    { -26497, -25166,   6681, 0.0 },
    {  -4995,  16906, -15382, 0.0 },
    {  21128, -23438,  -7952, 0.0 },
    { -12754,   5184,  16107, 0.0 },
    {   5438,  -9087,  24346, 0.0 },
    {  28785, -25909,  -1085, 0.0 },
    {  29814,  18476, -20188, 0.0 },
    { -11937, -25675, -17711, 0.0 },
    { -19995, -13557, -15774, 0.0 },
    { -16508,  28991,  15778, 0.0 },
    { -18964, -30013, -22114, 0.0 },
    {  -5767, -15090,   8336, 0.0 },
    {  16870,    836,  22779, 0.0 },
    {   5844,  14371,  26096, 0.0 },
    {  -2570, -24405,   8345, 0.0 },
    { -26475,  -5889,   6644, 0.0 },
    {  19871,  16376,  31959, 0.0 },
    {   7222,    887, -10972, 0.0 },
    {  -8509,   8822, -30035, 0.0 },
    { -27596,  12170,  10300, 0.0 },
    {  14548,  26826, -16645, 0.0 },
    {  24554, -22149,   5874, 0.0 },
    {  21946, -18910,   9658, 0.0 },
    { -14890,  18785,   2408, 0.0 },
    {  30392, -27151,  16593, 0.0 },
    { -31890,  20508, -29177, 0.0 },
    {  -3677, -24447, -24482, 0.0 },
    {  16036,  14933,  31373, 0.0 },
    { -27412, -24919, -32737, 0.0 },
    { -30233,  -3916, -16991, 0.0 },
    {  17679,   3655,  -2359, 0.0 },
    {    666,   6311, -24491, 0.0 },
    { -22850,   5390,  10866, 0.0 },
    {  12046,  22825,  29513, 0.0 },
    { -27860,  27043,  21863, 0.0 },
    {   9919,  -5014,  -5495, 0.0 },
    {  -6574,   2949, -31163, 0.0 },
    {  21149, -28761,  28670, 0.0 },
    { -31756,   2944,  24009, 0.0 },
    { -30821, -25664,  20610, 0.0 },
    {  22563, -10776, -21190, 0.0 },
    {  25833,  -1932, -27682, 0.0 },
    {  -3068, -20278,   1629, 0.0 },
    { -31651,  -5273,  19623, 0.0 },
    {  17536, -29376,  -8060, 0.0 },
    { -14042, -20046,  11546, 0.0 },
    { -19435, -22218, -20679, 0.0 },
    { -17520,  26483, -28621, 0.0 },
    {  -6312,  20088,  19277, 0.0 },
    {  -6483,  10364,  11486, 0.0 },
    { -25189, -24704,  22045, 0.0 },
    {  20881,  25372,  -3362, 0.0 },
    {  25508,  26495,  14803, 0.0 },
    {  10076,   4771,  31136, 0.0 },
    { -18200,   2418,  31499, 0.0 },
    {  14347,  28561,   9786, 0.0 },
    { -18947, -13798,  -8458, 0.0 },
    { -18811,   3173,  21029, 0.0 },
    {   5402,  26924,   2441, 0.0 },
    {   2915, -21077, -26660, 0.0 },
    {  29940,   9509,  25466, 0.0 },
    { -19512, -25577,   -372, 0.0 },
    { -28668,  -5408, -20950, 0.0 },
    { -20188,  19489,  19326, 0.0 },
    {  15238,  10542, -20272, 0.0 },
    {   2898,  -6982, -14424, 0.0 },
    {  12947,  14876,   3791, 0.0 },
    {  30503, -27431,  28342, 0.0 },
    { -27355,  22973, -29202, 0.0 },
    {  -7075, -18187,   6298, 0.0 },
    {  -9209,   1793,  23473, 0.0 },
    { -11846,   5382,  31807, 0.0 },
    { -23575, -22673,  -7113, 0.0 },
    {  12093,  27024, -20171, 0.0 },
    {  27680, -17929,  31282, 0.0 },
    {  32479,  18497,   4105, 0.0 },
    {   5379, -23158,  22223, 0.0 },
    {  -3369, -28255,   7598, 0.0 },
    { -12854,  32432,   1121, 0.0 },
    {  32535,   9787,  -1033, 0.0 },
    {   -284,  25714, -19285, 0.0 },
    {  19542, -17290, -21842, 0.0 },
    {  -6166, -24791,  18007, 0.0 },
    {   7080,   -612,  19340, 0.0 },
    {  -8598,  -7890,  -7246, 0.0 },
    { -15080,   -416, -16631, 0.0 },
    { -30939,   6773,   3448, 0.0 },
    {  -5855, -15726, -24693, 0.0 },
    {   5988,  18236,   8580, 0.0 },
    {  26157,  -1589, -10628, 0.0 },
    { -13614,  24303,  31894, 0.0 },
    { -15216,   8910, -10608, 0.0 },
    { -14619,  31953,  26520, 0.0 },
    {  10605,  12305,  -9951, 0.0 },
    {  -2855,  -5342, -18133, 0.0 },
    {   1278,  14525,  30335, 0.0 },
    {   4511,  -5568,  18505, 0.0 },
    { -22199,  14685,  19071, 0.0 },
    {  19333,  18176,  -3250, 0.0 },
    {  -7524,  14250,    740, 0.0 },
    { -11211, -14922, -14841, 0.0 },
    {  24275,    110,  27129, 0.0 },
    {   3025,  24323,   -271, 0.0 },
    {  18799,  18593, -22511, 0.0 },
    { -23621,   8441,   7551, 0.0 },
    { -25609, -26733, -20448, 0.0 },
    { -29820, -23855,  30678, 0.0 },
    {   1187,  -2592,    771, 0.0 },
    {  -8351, -20037, -22738, 0.0 },
    { -18221,  11880, -28759, 0.0 },
    {  15306,   7106, -14401, 0.0 },
    {  28297,  25740, -19868, 0.0 },
    {  22901,  25067, -18259, 0.0 },
    {  29764, -11169,  25782, 0.0 },
    { -14575,  13119, -29772, 0.0 },
    {  -5720,  -1800, -29488, 0.0 },
    { -16035,  12562,  13854, 0.0 },
    { -11466, -32569,  -1959, 0.0 },
    { -26807, -13279,  -4661, 0.0 },
    { -22148,   8151,  11568, 0.0 },
    {  18619,  14493, -24208, 0.0 },
    {    790,   3219,  14063, 0.0 },
    {  18221, -15566, -21944, 0.0 },
    { -28603,   -612,    -27, 0.0 },
    { -27944, -12814, -20213, 0.0 },
    {  23270,  28447,  13736, 0.0 },
    {  11177,  19272, -26224, 0.0 },
    {  24405,   6713,   8235, 0.0 },
    {  -9403, -22238,  14156, 0.0 },
    {  25903, -20580,   4118, 0.0 },
    {  23230,  25882,   -297, 0.0 },
    { -17351,  -4701,  18584, 0.0 },
    {  26543,   3151,   1825, 0.0 },
    { -18062,   6806,  21100, 0.0 },
    {  15989, -19848,    381, 0.0 },
    {  26638,  15986,   7018, 0.0 },
    { -20456,  25550, -17802, 0.0 },
    {   4900,  14160,  32719, 0.0 },
    { -10518,  -7972, -25725, 0.0 },
    {  16974,  13727,  11477, 0.0 },
    { -27396,  13401,   3278, 0.0 },
    {  27902,  -3153,  26706, 0.0 },
    { -30138, -28392,  32659, 0.0 },
    { -19185,   9207, -16926, 0.0 },
    {  -5082,   1189,  12346, 0.0 },
    {  23412,  -7873, -28593, 0.0 },
    {   7613,     62,  15653, 0.0 },
    {  19439,  25843,   9053, 0.0 },
    {  -1395,  -8915,  22351, 0.0 },
    { -30811,   2109,  29728, 0.0 },
    {   4543,   7535,  12108, 0.0 },
    {   4071,  14724,  20441, 0.0 },
    {  -8713,   6572, -11674, 0.0 },
    { -19953, -14689, -29222, 0.0 },
    { -23635,  12279,   8552, 0.0 },
    { -28890, -19015,   5757, 0.0 },
    {  31800,  10587,  29881, 0.0 },
    {  22882,  -1879, -24779, 0.0 },
    { -15958,  21160,  20042, 0.0 },
    {  21504,   1343,  10362, 0.0 },
    { -18134,  23036,  -8521, 0.0 },
    { -30219,   4805,  20271, 0.0 },
    { -31495,  30338, -22687, 0.0 },
    {   3133, -10705,  32761, 0.0 },
    {   3630, -10211, -26220, 0.0 },
    {  17174,  18767, -24643, 0.0 },
    { -26671,  11967,   9076, 0.0 },
    { -25844,   6044,  13516, 0.0 },
    {  -4026, -17513,  19339, 0.0 },
    { -14321,  20644,  -6881, 0.0 },
    {  22644,  31024,   1998, 0.0 },
    { -27263,  27313,  27334, 0.0 },
    {  -9861, -28296,  19432, 0.0 },
    { -23481,  25001, -13638, 0.0 },
    { -23192,    844,  -3326, 0.0 },
    {  19182,  -1456,  24456, 0.0 },
    {   9653, -28820,  -5312, 0.0 },
    { -15707,  -6584,  11059, 0.0 },
    {   5898,  12729,  16244, 0.0 },
    {  -1105, -12179,    892, 0.0 },
    { -26352,  10049, -19909, 0.0 },
    {   3255, -14407, -19662, 0.0 },
    { -28820, -26699,  12698, 0.0 },
    {  -3091, -14704, -29125, 0.0 },
    { -29652,  20742,  31590, 0.0 },
    {  10398,  12479, -12440, 0.0 },
    {  31934, -12879,  27946, 0.0 },
    {    174,  12953,  13404, 0.0 },
    {  32499,   3829, -16757, 0.0 },
    {  27844,   2173, -30533, 0.0 },
    {  -4050,  28042,  24884, 0.0 },
    {   1971, -24683,   7296, 0.0 },
    {   1671,  -1932,  30517, 0.0 },
    {  16589,   3396,   7962, 0.0 },
    { -26740, -12232, -24947, 0.0 },
    { -21010, -18833,   8826, 0.0 },
    { -27288,  17341,  12718, 0.0 },
    {   1756,  32750,    878, 0.0 },
    {  -5841,  18398, -24631, 0.0 },
    {  32557,  14391, -16951, 0.0 },
    { -11338,  22431,  10326, 0.0 },
    { -14580, -22561, -23498, 0.0 },
    { -20452,  26260,  12513, 0.0 },
    {  32525, -14409,  -6196, 0.0 },
    { -31285,   4874,   1070, 0.0 },
    {  -2539, -12170, -14820, 0.0 },
    { -14855, -24387, -30474, 0.0 },
    {  32368,  24017,   -394, 0.0 },
    { -11348,    176,  22756, 0.0 },
    {  21477, -23313, -13025, 0.0 },
    {  13572,  11826,   1817, 0.0 },
    { -12433,  14571,  23385, 0.0 },
    { -26322,   5522, -20772, 0.0 },
    { -29128, -13532,  23483, 0.0 },
    {  -6762,   6717,  17007, 0.0 },
    { -14326, -18591,  25909, 0.0 },
    {  -3442,  32407, -16873, 0.0 },
    {  22314, -18972,   3706, 0.0 },
    { -14954, -17123, -14534, 0.0 },
    {    402,  18342,  11257, 0.0 },
    {  -7140,  17330, -28225, 0.0 },
    { -20807, -27528, -20499, 0.0 },
    {  28154,  17926,  24565, 0.0 },
    {   3526,  22199,   2129, 0.0 },
    {   6169,  -9441, -11557, 0.0 },
    {   5362,   7995,  -4500, 0.0 },
    {   -256,  12199, -19232, 0.0 },
    { -17923,    494,  27822, 0.0 },
    {   9915,  18960,   7204, 0.0 },
    {  22917, -25705,  11598, 0.0 },
    {   5628,   1777, -13561, 0.0 },
    {  -8987, -27581, -26008, 0.0 },
    { -28104, -30220,   2142, 0.0 },
    {  12311,  31220,  -8379, 0.0 },
    {    972,  13497,  15722, 0.0 },
    { -19682, -18771, -28296, 0.0 },
    {  26021,   9372,   6093, 0.0 },
    {  24031,  -4778, -30678, 0.0 },
    { -15258, -14619,  24033, 0.0 },
    {   3348, -32739,  -5520, 0.0 },
    { -18656,  11104,  24182, 0.0 },
    {   -933,  21466,  27623, 0.0 },
    {  28914, -26618, -24359, 0.0 },
    { -18655, -17955,  11069, 0.0 },
    {   1645,   3127,  27578, 0.0 },
    { -23004,   6651, -23966, 0.0 },
    {   1638, -13263,  13860, 0.0 },
    { -29417,  15934, -24210, 0.0 },
    { -27014, -30175,  28016, 0.0 },
    {   6807, -31121, -27008, 0.0 },
    { -16406,  31077,  15981, 0.0 },
    {  -5325,  -8678,   8192, 0.0 },
    { -13290, -27209, -25051, 0.0 },
    {  30182, -20370,  -2926, 0.0 },
    { -16390,   3642,   5150, 0.0 },
    { -10698,   4419, -30380, 0.0 },
    { -30297,  27111, -23512, 0.0 },
    { -15890,  -4747, -23488, 0.0 },
    {  31214, -29531,  31335, 0.0 },
    {  27928,  20677,  10871, 0.0 },
    {  -6440, -10446,  -6983, 0.0 },
    {  10065, -21583, -12824, 0.0 },
    {   6735,  13888,  29345, 0.0 },
    {  21873,   6416,   5315, 0.0 },
    {  19450, -22346, -12832, 0.0 },
    {   6208,   4388,  21036, 0.0 },
    {  -7799,   -191,  21993, 0.0 },
    {  -5092,   2969,  11927, 0.0 },
    {  15832,  -1632, -13268, 0.0 },
    {  11437,   3292,  -6982, 0.0 },
    {  -6150,   4593, -23869, 0.0 },
    {  31543, -30254, -23214, 0.0 },
    {   -367,  23350, -30534, 0.0 },
    {   8861,  12911, -14701, 0.0 },
    {  22315, -19109,   5570, 0.0 },
    { -21623,  15331, -25671, 0.0 },
    { -12991,   8021,   9492, 0.0 },
    { -16175,  -4556,   -484, 0.0 },
    {  16117,  27249,   8780, 0.0 },
    {  16212,  19980,  -3332, 0.0 },
    { -26249,  18622, -18918, 0.0 },
    { -16718,  24716, -13264, 0.0 },
    { -30386,  24927, -11459, 0.0 },
    {   3285,  25710,   3582, 0.0 },
    {   6043, -31554,  -7663, 0.0 },
    {  15355,  16265, -19103, 0.0 },
    {  18602,  16930, -26008, 0.0 },
    {   -640,  25425, -32640, 0.0 },
    { -31223,  -6154,  17774, 0.0 },
    {  -2155,  22220, -18415, 0.0 },
    { -22030, -24953, -19408, 0.0 },
    {   7910, -25658,  -7702, 0.0 },
    {  27788,  21693, -10240, 0.0 },
    { -15109, -19079,  15509, 0.0 },
    {  -1994,  10293, -12507, 0.0 },
    { -17077,   2862, -12953, 0.0 },
    {  -2385,  17093, -29929, 0.0 },
    {  -1318, -19275,  -4087, 0.0 },
    { -32227,  28849, -10426, 0.0 },
    {  26823,   7642,   6075, 0.0 },
    {  -2057,   2486,   8766, 0.0 },
    { -32040, -21697,  22296, 0.0 },
    {   2457,  23889,   4459, 0.0 },
    { -22991,   1342,  32571, 0.0 },
    { -11193,  26458, -15607, 0.0 },
    {   5668,  10660,  10347, 0.0 },
    {   5336,   9631, -13981, 0.0 },
    {  -7010,   6021,  -6000, 0.0 },
    {  24832, -25221, -32681, 0.0 },
    {  30942,  17363,  14598, 0.0 },
    {  19854,  29684,   8587, 0.0 },
    {  -8857,  27168,   -656, 0.0 },
    {  20288,  -6821, -31391, 0.0 },
    { -12524,  13297,  28220, 0.0 },
    { -22450,  21332, -14918, 0.0 },
    {  17670, -21173, -11022, 0.0 },
    {  -7158,  22428,  -8944, 0.0 },
    { -26182,   9205, -26742, 0.0 },
    {  25212,  24486,   7597, 0.0 },
    { -10996,  30585,   -608, 0.0 },
    { -20777, -14042,  16202, 0.0 },
    {  -3022,  16672, -15687, 0.0 },
    {   8329, -19303,  30929, 0.0 },
    {  -5583,   7728,  12820, 0.0 },
    {  -9983, -27013, -26818, 0.0 },
    { -18534, -16617,  -9402, 0.0 },
    {  15421,  -3930,  14663, 0.0 },
    {  -9831,  11919, -22808, 0.0 },
    { -14760,  11028, -16296, 0.0 },
    {   7576, -26461,  27982, 0.0 },
    {   9408,  15308, -29721, 0.0 },
    {   1523,  20596, -26834, 0.0 },
    {   3040,   6859, -16068, 0.0 },
    {  29050, -14978, -30694, 0.0 },
    { -19967, -17892,   1883, 0.0 },
    {   -782,  21656,   2859, 0.0 },
    { -19522,  -5277, -30014, 0.0 },
    { -15612, -29118,  24145, 0.0 },
    {  -4601,   7717, -27482, 0.0 },
    {  -6052,  21576, -12663, 0.0 },
    { -22541, -20567, -15086, 0.0 },
    { -27838,  17442,  20114, 0.0 },
    {  23452,  23315,    -94, 0.0 },
    {  21404,  25238, -15412, 0.0 },
    { -13701,  -5152,   1119, 0.0 },
    {   5012, -20353, -28916, 0.0 },
    { -27286, -26448, -12724, 0.0 },
    {  19027,   1823, -24372, 0.0 },
    {   2303,   6257,    952, 0.0 },
    {  -5418, -31554, -20279, 0.0 },
    {  -3863, -22223, -24874, 0.0 },
    {  25692,  17719, -25212, 0.0 },
    { -27258,  10688, -22871, 0.0 },
    { -10849,  10650, -17158, 0.0 },
    {  18859,  -9940,  -6751, 0.0 },
    {   8966,  -4906, -30439, 0.0 },
    { -19080,  20575,  -6759, 0.0 },
    {  10802, -27501, -31218, 0.0 },
    { -10283, -16211,  32055, 0.0 },
    {  31330,  16795, -25659, 0.0 },
    {  -1086,  15665,  14651, 0.0 },
    {    688, -17154,  25347, 0.0 },
    {   4798, -21020,  23410, 0.0 },
    {   -821,  23071,  -8861, 0.0 },
    {   7095,  12550,  29434, 0.0 },
    {  20790,   -400, -17081, 0.0 },
    {  20676,  14442,  10163, 0.0 },
    {   4872,  20732, -23720, 0.0 },
    {  18910,   4630, -27446, 0.0 },
    {  10462, -10935,   3637, 0.0 },
    {   2293,  -3506, -23491, 0.0 },
    { -20384,  26390,  -1605, 0.0 },
    { -28022,  24801, -31045, 0.0 },
    {  25755, -26505, -18233, 0.0 },
    {  27110,  19656,  -1107, 0.0 },
    { -26554,  -8969, -12803, 0.0 },
    {  31738, -19573,   6631, 0.0 },
    {   1619,  -9045,  16679, 0.0 },
    {  13671,   3444,  29686, 0.0 },
    {  25309,  16413, -15251, 0.0 },
    {  24501,  24722, -23270, 0.0 },
    { -31788,  28639, -28408, 0.0 },
    {   8709, -12454, -17468, 0.0 },
    { -10621,   7335,  22225, 0.0 },
    {  28263,   8571,   -181, 0.0 },
    { -20254,  22037,  18343, 0.0 },
    {    744,   8897,  26468, 0.0 },
    { -24770,  -1291,  12778, 0.0 },
    {  30594, -25843,   9609, 0.0 },
    { -12261, -26566, -13313, 0.0 },
    { -16212,  -9233,  -3625, 0.0 },
    { -25031,  18781,  25589, 0.0 },
    {  29025,  26038, -30742, 0.0 },
    { -18807, -26702, -13036, 0.0 },
    {  14038,  -9069, -13254, 0.0 },
    { -24391,  32046,  15084, 0.0 },
    {  23566, -10737, -18459, 0.0 },
    {  22564,  -4396, -19345, 0.0 },
    {  -4579, -28352,   7974, 0.0 },
    {  22240,  -9831,   8149, 0.0 },
    {  14289,  -9456, -26684, 0.0 },
    {  17617,    824,  -5015, 0.0 },
    {  -7713,  14838,  17376, 0.0 },
    {  16233, -16028,   1734, 0.0 },
};

#endif  // __INPUT_H__