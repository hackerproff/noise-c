/*
 * Copyright (C) 2016 Southern Storm Software, Pty Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef NOISE_INTERNAL_H
#define NOISE_INTERNAL_H

#include <noise/noise.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file internal.h
 * \brief Internal definitions for the library.
 *
 * \note This file and its definitions are not part of the public API.
 */

/**
 * \brief Maximum hash length over all supported hash algorithms.
 */
#define NOISE_MAX_HASHLEN 64

struct NoiseCipherState_s
{
    size_t size;
    int cipher_id;
    uint8_t has_key;
    uint8_t nonce_overflow;
    uint8_t key_len;
    uint8_t mac_len;
    uint64_t n;

    NoiseCipherState *(*create)(void);
    void (*init_key)(NoiseCipherState *state, const uint8_t *key);
    int (*encrypt)(NoiseCipherState *state, const uint8_t *ad, size_t ad_len,
                   uint8_t *data, size_t len);
    int (*decrypt)(NoiseCipherState *state, const uint8_t *ad, size_t ad_len,
                   uint8_t *data, size_t len);
    void (*destroy)(NoiseCipherState *state); /* Optional, can be NULL */
};

struct NoiseHashState_s
{
    size_t size;
    int hash_id;
    size_t hash_len;
    size_t block_len;

    void (*reset)(NoiseHashState *state);
    void (*update)(NoiseHashState *state, const uint8_t *data, size_t len);
    void (*finalize)(NoiseHashState *state, uint8_t *hash);
    void (*clean)(NoiseHashState *state);
    void (*destroy)(NoiseHashState *state); /* Optional, can be NULL */
};

struct NoiseSymmetricState_s
{
    size_t size;
    NoiseProtocolId id;
    NoiseCipherState *cipher;
    NoiseHashState *hash;
    uint8_t ck[NOISE_MAX_HASHLEN];
    uint8_t h[NOISE_MAX_HASHLEN];
};

#define noise_new(type) ((type *)noise_calloc(sizeof(type)))
void *noise_calloc(size_t size);
void noise_free(void *ptr, size_t size);

void noise_clean(void *data, size_t size);
int noise_secure_is_equal(const void *s1, const void *s2, size_t size);

void noise_rand_bytes(void *bytes, size_t size);

NoiseCipherState *noise_chachapoly_new(void);
NoiseCipherState *noise_aesgcm_new(void);

NoiseHashState *noise_blake2s_new(void);
NoiseHashState *noise_blake2b_new(void);
NoiseHashState *noise_sha256_new(void);
NoiseHashState *noise_sha512_new(void);

#ifdef __cplusplus
};
#endif

#endif
