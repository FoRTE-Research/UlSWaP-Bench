#ifndef HASH_H
#define HASH_H

#include "md5.h"

typedef MD5_CTX hash_ctx_t;
typedef uint8_t hash_result_t[16];

#define hash_init(ctx) MD5_Init(ctx)
#define hash_update(ctx, data, size) MD5_Update(ctx, data, size)
#define hash_final(result, ctx) MD5_Final(result, ctx)
#define hash_get_lowest32bits(hash) MD5_GetLowest32Bits(hash)

#endif  // HASH_H
