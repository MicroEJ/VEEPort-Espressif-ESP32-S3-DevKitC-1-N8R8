/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief MicroEJ Security low level API implementation for MbedTLS Library.
 * @author MicroEJ Developer Team
 * @version 1.2.0
 */

#include <LLSEC_mbedtls.h>

#include <LLSEC_RSA_CIPHER_impl.h>
#include <LLSEC_ERRORS.h>
#include <LLSEC_configuration.h>
#include <sni.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 include mbedtls
*/
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
// cppcheck-suppress preprocessorErrorDirective // macro include
#include MBEDTLS_CONFIG_FILE
#endif
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/rsa.h"
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"

#define LLSEC_RSA_CIPHER_SUCCESS 0
#define LLSEC_RSA_CIPHER_ERROR   -1

//#define LLSEC_RSA_CIPHER_DEBUG

#ifdef LLSEC_RSA_CIPHER_DEBUG
// cppcheck-suppress misra-c2012-21.6 // Include only in debug
#include <stdio.h>
#define LLSEC_RSA_CIPHER_DEBUG_TRACE(...) (void)printf(__VA_ARGS__)
#else
#define LLSEC_RSA_CIPHER_DEBUG_TRACE(...) ((void)0)
#endif

/**
 * RSA Cipher init function type
 */
typedef int32_t (*LLSEC_RSA_CIPHER_init)(int32_t transformation_id, void** native_id, uint8_t is_decrypting, int32_t key_id, int32_t padding, int32_t hash);
typedef int32_t (*LLSEC_RSA_CIPHER_decrypt)(void* native_id, uint8_t* buffer, int32_t buffer_length, uint8_t* output);
typedef int32_t (*LLSEC_RSA_CIPHER_encrypt)(void* native_id, uint8_t* buffer, int32_t buffer_length, uint8_t* output);
typedef void (*LLSEC_RSA_CIPHER_close)(void* native_id);

typedef struct {
    char* name; // the name of the transformation
    LLSEC_RSA_CIPHER_init init;
    LLSEC_RSA_CIPHER_decrypt decrypt;
    LLSEC_RSA_CIPHER_encrypt encrypt;
    LLSEC_RSA_CIPHER_close close;
    LLSEC_RSA_CIPHER_transformation_desc description;
} LLSEC_RSA_CIPHER_transformation;

typedef struct {
    LLSEC_RSA_CIPHER_transformation* transformation;
    mbedtls_rsa_context mbedtls_ctx;
    mbedtls_ctr_drbg_context ctr_drbg;
} LLSEC_RSA_CIPHER_ctx;

static int32_t llsec_rsa_cipher_init(int32_t transformation_id, void** native_id, uint8_t is_decrypting, int32_t key_id, int32_t padding_type, int32_t oaep_hash_algorithm);
static int32_t llsec_rsa_cipher_decrypt(void* native_id, uint8_t* buffer, int32_t buffer_length, uint8_t* output);
static int32_t llsec_rsa_cipher_encrypt(void* native_id, uint8_t* buffer, int32_t buffer_length, uint8_t* output);
static void llsec_rsa_cipher_close(void* native_id);

// cppcheck-suppress misra-c2012-5.9 // Define here for code readability even if it called once in this file.
static LLSEC_RSA_CIPHER_transformation available_transformations[3] = {
    {
        .name = "RSA/ECB/PKCS1Padding",
        .init = llsec_rsa_cipher_init,
        .decrypt = llsec_rsa_cipher_decrypt,
        .encrypt = llsec_rsa_cipher_encrypt,
        .close = llsec_rsa_cipher_close,
        {
            .padding_type = PAD_PKCS1_TYPE,
            .oaep_hash_algorithm = 0,
        },
    },
    {
        .name = "RSA/ECB/OAEPWithSHA-1AndMGF1Padding",
        .init = llsec_rsa_cipher_init,
        .decrypt = llsec_rsa_cipher_decrypt,
        .encrypt = llsec_rsa_cipher_encrypt,
        .close = llsec_rsa_cipher_close,
        {
            .padding_type = PAD_OAEP_MGF1_TYPE,
            .oaep_hash_algorithm = OAEP_HASH_SHA_1_ALGORITHM,
        },
    },
    {
        .name = "RSA/ECB/OAEPWithSHA-256AndMGF1Padding",
        .init = llsec_rsa_cipher_init,
        .decrypt = llsec_rsa_cipher_decrypt,
        .encrypt = llsec_rsa_cipher_encrypt,
        .close = llsec_rsa_cipher_close,
        {
            .padding_type = PAD_OAEP_MGF1_TYPE,
            .oaep_hash_algorithm = OAEP_HASH_SHA_256_ALGORITHM,
        },
    }
};

static int32_t llsec_rsa_cipher_init(int32_t transformation_id, void** native_id, uint8_t is_decrypting, int32_t key_id, int32_t padding_type, int32_t oaep_hash_algorithm)
{
    int return_code = LLSEC_RSA_CIPHER_SUCCESS;
    mbedtls_rsa_context* key_ctx = NULL;
    LLSEC_RSA_CIPHER_ctx* p_cipher_ctx;
    LLSEC_RSA_CIPHER_DEBUG_TRACE("%s\n", __func__);

    p_cipher_ctx = LLSEC_calloc(1, (int32_t)sizeof(LLSEC_RSA_CIPHER_ctx));
    // cppcheck-suppress misra-c2012-11.4 // Abstract data type for SNI usage
    p_cipher_ctx->transformation = (LLSEC_RSA_CIPHER_transformation*) transformation_id;

    if ((uint8_t)0 != is_decrypting) {
        // cppcheck-suppress misra-c2012-11.4 // Abstract data type for SNI usage
        LLSEC_priv_key* key = (LLSEC_priv_key*)key_id;
        key_ctx = (mbedtls_rsa_context*)key->key;
    } else {
        // cppcheck-suppress misra-c2012-11.4 // Abstract data type for SNI usage
        LLSEC_pub_key* key = (LLSEC_pub_key*)key_id;
        key_ctx = (mbedtls_rsa_context*)key->key;
    }

    return_code = mbedtls_rsa_copy(&p_cipher_ctx->mbedtls_ctx, key_ctx);
    if (return_code == LLSEC_RSA_CIPHER_SUCCESS) {
        int32_t padding = (padding_type == PAD_PKCS1_TYPE) ? MBEDTLS_RSA_PKCS_V15 : MBEDTLS_RSA_PKCS_V21;
        int32_t hash_id = (oaep_hash_algorithm == OAEP_HASH_SHA_1_ALGORITHM) ? MBEDTLS_MD_SHA1 : MBEDTLS_MD_SHA256;
        mbedtls_rsa_set_padding(&p_cipher_ctx->mbedtls_ctx, padding, hash_id);
    } else {
        mbedtls_rsa_free(&p_cipher_ctx->mbedtls_ctx);
        SNI_throwNativeException(return_code, "mbedtls_rsa_copy failed");
        return_code = LLSEC_RSA_CIPHER_ERROR;
    }

    if (return_code == LLSEC_RSA_CIPHER_SUCCESS) {
        mbedtls_entropy_context entropy;
        const char* pers = llsec_gen_random_str_internal(8);

        mbedtls_entropy_init(&entropy);
        mbedtls_ctr_drbg_init(&p_cipher_ctx->ctr_drbg);

        return_code = mbedtls_ctr_drbg_seed(&p_cipher_ctx->ctr_drbg, mbedtls_entropy_func, &entropy,
                          (const uint8_t*)pers, strlen(pers));
        if (return_code != LLSEC_RSA_CIPHER_SUCCESS) {
            mbedtls_ctr_drbg_free(&p_cipher_ctx->ctr_drbg);
            mbedtls_entropy_free(&entropy);
            // cppcheck-suppress misra-c2012-11.8 // Cast for matching free function signature 
            mbedtls_free((void*)pers);
            SNI_throwNativeException(return_code, "mbedtls_ctr_drbg_seed failed");
            return_code = LLSEC_RSA_CIPHER_ERROR;
        }
    }

    if (return_code != LLSEC_RSA_CIPHER_SUCCESS) {
        return_code = LLSEC_RSA_CIPHER_ERROR;
    } else {
        *native_id = p_cipher_ctx;
    }
    return return_code;
}

static int32_t llsec_rsa_cipher_decrypt(void* native_id, uint8_t* buffer, int32_t buffer_length, uint8_t* output)
{
    // cppcheck-suppress misra-c2012-11.5 // Abstract data type for SNI usage
    LLSEC_RSA_CIPHER_ctx* p_cipher_ctx = (LLSEC_RSA_CIPHER_ctx*)native_id;
    LLSEC_RSA_CIPHER_DEBUG_TRACE("%s\n", __func__);

    (void)buffer_length;

    size_t out_len = 0;
    int32_t return_code = mbedtls_rsa_pkcs1_decrypt(&p_cipher_ctx->mbedtls_ctx, mbedtls_ctr_drbg_random, &p_cipher_ctx->ctr_drbg,
                      &out_len, buffer, output, mbedtls_rsa_get_len(&p_cipher_ctx->mbedtls_ctx));
    if (return_code == LLSEC_RSA_CIPHER_SUCCESS) {
        return_code = out_len;
    } else {
        SNI_throwNativeException(return_code, "llsec_rsa_cipher_decrypt failed");
        return_code = LLSEC_RSA_CIPHER_ERROR;
    }

    return return_code;
}

static int32_t llsec_rsa_cipher_encrypt(void* native_id, uint8_t* buffer, int32_t buffer_length, uint8_t* output)
{
    // cppcheck-suppress misra-c2012-11.5 // Abstract data type for SNI usage
    LLSEC_RSA_CIPHER_ctx* p_cipher_ctx = (LLSEC_RSA_CIPHER_ctx*)native_id;
    LLSEC_RSA_CIPHER_DEBUG_TRACE("%s\n", __func__);

    int32_t return_code = mbedtls_rsa_pkcs1_encrypt(&p_cipher_ctx->mbedtls_ctx, mbedtls_ctr_drbg_random, &p_cipher_ctx->ctr_drbg,
                      buffer_length, buffer, output);
    if (return_code == LLSEC_RSA_CIPHER_SUCCESS) {
        return_code = mbedtls_rsa_get_len(&p_cipher_ctx->mbedtls_ctx);
    } else {
        SNI_throwNativeException(return_code, "llsec_rsa_cipher_encrypt failed");
        return_code = LLSEC_RSA_CIPHER_ERROR;
    }

    return return_code;
}

static void llsec_rsa_cipher_close(void* native_id)
{
    LLSEC_RSA_CIPHER_DEBUG_TRACE("%s native_id:%p\n", __func__, native_id);
    // cppcheck-suppress misra-c2012-11.5 // Abstract data type for SNI usage
    LLSEC_RSA_CIPHER_ctx* p_cipher_ctx = (LLSEC_RSA_CIPHER_ctx*)native_id;
    mbedtls_rsa_free(&p_cipher_ctx->mbedtls_ctx);
    LLSEC_free(native_id);
}

/**
 * @brief Gets for the given transformation the RSA cipher description.
 * <p>
 * <code>transformation_desc</code> must be filled-in with:
 * <ul>
 *     <li>[0-3]: native transformation ID</li>
 *     <li>[4-7]: padding_type: one of the <code>PAD_*</code> values defined in {@link AbstractRSACipherSpi}</li>
 *     <li>[8-11]: oaep_hash_algorithm: one of the <code>OAEP_HASH_*</code> values defined in {@link AbstractRSACipherSpi}</li>
 * </ul>
 *
 * @param transformation_name[in]        Null terminated string that describes the transformation.
 * @param transformation_desc[out]        Description of the cipher.
 *
 * @return The transformation ID on success or -1 on error.
 *
 * @warning <code>transformation_name</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_RSA_CIPHER_IMPL_get_transformation_description(uint8_t* transformation_name, LLSEC_RSA_CIPHER_transformation_desc* transformation_desc)
{
    int32_t return_code = LLSEC_RSA_CIPHER_SUCCESS;
    LLSEC_RSA_CIPHER_DEBUG_TRACE("%s transformation_name %s\n", __func__, transformation_name);
    int32_t nb_transformations = sizeof(available_transformations) / sizeof(LLSEC_RSA_CIPHER_transformation);
    LLSEC_RSA_CIPHER_transformation* transformation = &available_transformations[0];

    while (--nb_transformations >= 0) {
        if (strcmp((const char*)transformation_name, transformation->name) == 0) {
            (void) memcpy(transformation_desc, &(transformation->description), sizeof(LLSEC_RSA_CIPHER_transformation_desc));
            break;
        }
        transformation++;
    }

    if (nb_transformations >= 0)
    {
        // cppcheck-suppress misra-c2012-11.4 // Abstract data type for SNI usage
        return_code = (int32_t)transformation;
    } else {
        return_code = LLSEC_RSA_CIPHER_ERROR;
    }
    return return_code;
}

/**
 * @brief Initializes a RSA Cipher resource.
 *
 * @param[in] tranformation_id            The transformation ID.
 * @param[in] is_decrypting                '1' for decrypting, '0' for encryting.
 * @param[in] key_id                    The key id (either a {@link NativePublicKey} or a {@link NativePrivateKey}).
 * @param[in] padding_type                The RSA padding type.
 * @param[in] oaep_hash_algorithm        The hash algorithm for OAEP RSA padding type.
 *
 * @return The nativeId of the newly initialized resource.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_RSA_CIPHER_IMPL_init(int32_t transformation_id, uint8_t is_decrypting, int32_t key_id, int32_t padding_type, int32_t oaep_hash_algorithm)
{
    int32_t return_code = LLSEC_RSA_CIPHER_SUCCESS;
    LLSEC_RSA_CIPHER_DEBUG_TRACE("%s\n", __func__);
    void* native_id = NULL;
    // cppcheck-suppress misra-c2012-11.4 // Abstract data type for SNI usage
    LLSEC_RSA_CIPHER_transformation* transformation = (LLSEC_RSA_CIPHER_transformation*)transformation_id;

    if (key_id == 0) {
        SNI_throwNativeException(key_id, "LLSEC_RSA_CIPHER_IMPL_init invalid key_id");
        return_code = LLSEC_RSA_CIPHER_ERROR;
    }

    if ((padding_type != PAD_PKCS1_TYPE) && (padding_type != PAD_OAEP_MGF1_TYPE)) {
        SNI_throwNativeException(padding_type, "LLSEC_RSA_CIPHER_IMPL_init invalid padding_type");
        return_code = LLSEC_RSA_CIPHER_ERROR;
    }

    if ((key_id == PAD_OAEP_MGF1_TYPE) && ((oaep_hash_algorithm != OAEP_HASH_SHA_1_ALGORITHM) && (oaep_hash_algorithm != OAEP_HASH_SHA_256_ALGORITHM))) {
        SNI_throwNativeException(oaep_hash_algorithm, "LLSEC_RSA_CIPHER_IMPL_init invalid oaep_hash_algorithm");
        return_code = LLSEC_RSA_CIPHER_ERROR;
    }

    if (return_code == LLSEC_RSA_CIPHER_SUCCESS) {
        return_code = transformation->init(transformation_id, (void**)&native_id, is_decrypting, key_id, padding_type, oaep_hash_algorithm);

        if (return_code != LLSEC_RSA_CIPHER_SUCCESS) {
            SNI_throwNativeException(return_code, "LLSEC_RSA_CIPHER_IMPL_init failed");
            return_code = LLSEC_RSA_CIPHER_ERROR;
        }
    }

    if (return_code == LLSEC_RSA_CIPHER_SUCCESS) {
        // register SNI native resource
        if (SNI_registerResource(native_id, transformation->close, NULL) != SNI_OK) {
            SNI_throwNativeException(LLSEC_RSA_CIPHER_ERROR, "Can't register SNI native resource");
            transformation->close((void*)native_id);
            return_code = LLSEC_RSA_CIPHER_ERROR;
        } 
    }

    if (return_code == LLSEC_RSA_CIPHER_SUCCESS){
        // cppcheck-suppress misra-c2012-11.6 // Abstract data type for SNI usage
        return_code = (int32_t)(native_id);
    }

    return return_code;
}

/**
 * @brief Decrypts the message contained in <code>buffer</code>.
 *
 * @param[in] transformation_id            The transformation ID.
 * @param[in] native_id                    The resource's native ID.
 * @param[in] buffer                    The buffer containing the message to decrypt.
 * @param[in] buffer_offset                The buffer offset.
 * @param[in] buffer_length                The buffer length.
 * @param[out] output                    The output buffer containing the plaintext message.
 * @param[out] output_offset            The output offset.
 *
 * @return The length of the buffer.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>buffer</code> must not be used outside of the VM task or saved.
 * @warning <code>output</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_RSA_CIPHER_IMPL_decrypt(int32_t transformation_id, int32_t native_id, uint8_t* buffer, int32_t buffer_offset, int32_t buffer_length, uint8_t* output, int32_t output_offset)
{
    LLSEC_RSA_CIPHER_DEBUG_TRACE("%s\n", __func__);
    // cppcheck-suppress misra-c2012-11.4 // Abstract data type for SNI usage
    LLSEC_RSA_CIPHER_transformation* transformation = (LLSEC_RSA_CIPHER_transformation*)transformation_id;
    // cppcheck-suppress misra-c2012-11.6 // Abstract data type for SNI usage
    int32_t return_code = transformation->decrypt((void*)native_id, &buffer[buffer_offset], buffer_length, &output[output_offset]);
    if (return_code < 0) {
        SNI_throwNativeException(return_code, "LLSEC_RSA_CIPHER_IMPL_decrypt failed");
        return_code = LLSEC_RSA_CIPHER_ERROR;
    }
    return return_code;
}

/**
 * @brief Encrypts the message contained in <code>buffer</code>.
 *
 * @param[in] transformation_id            The transformation ID.
 * @param[in] native_id                    The resource's native ID.
 * @param[in] buffer                       The buffer containing the plaintext message to encrypt.
 * @param[in] buffer_offset                The buffer offset.
 * @param[in] buffer_length                The buffer length.
 * @param[out] output                      The output buffer containing the encrypted message.
 * @param[in] output_offset                The output offset.
 *
 * @return The length of the buffer.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>buffer</code> must not be used outside of the VM task or saved.
 * @warning <code>output</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_RSA_CIPHER_IMPL_encrypt(int32_t transformation_id, int32_t native_id, uint8_t* buffer, int32_t buffer_offset, int32_t buffer_length, uint8_t* output, int32_t output_offset)
{
    LLSEC_RSA_CIPHER_DEBUG_TRACE("%s\n", __func__);
    // cppcheck-suppress misra-c2012-11.4 // Abstract data type for SNI usage
    LLSEC_RSA_CIPHER_transformation* transformation = (LLSEC_RSA_CIPHER_transformation*)transformation_id;
    // cppcheck-suppress misra-c2012-11.6 // Abstract data type for SNI usage
    int32_t return_code = transformation->encrypt((void*)native_id, &buffer[buffer_offset], buffer_length, &output[output_offset]);
    if (return_code < 0) {
        SNI_throwNativeException(return_code, "LLSEC_RSA_CIPHER_IMPL_encrypt failed");
        return_code = LLSEC_RSA_CIPHER_ERROR;
    }
    return return_code;
}

/**
 * @brief Closes the resource related to the native ID.
 *
 * @param[in] transformation_id            The transformation ID.
 * @param[in] native_id                    The resource's native ID.
 *
 * @note Throws NativeException on error.
 */
void LLSEC_RSA_CIPHER_IMPL_close(int32_t transformation_id, int32_t native_id)
{
    LLSEC_RSA_CIPHER_DEBUG_TRACE("%s\n", __func__);
    // cppcheck-suppress misra-c2012-11.4 // Abstract data type for SNI usage
    LLSEC_RSA_CIPHER_transformation* transformation = (LLSEC_RSA_CIPHER_transformation*)transformation_id;

    // cppcheck-suppress misra-c2012-11.6 // Abstract data type for SNI usage
    transformation->close((void*)native_id);
    // cppcheck-suppress misra-c2012-11.6 // Abstract data type for SNI usage
    // cppcheck-suppress misra-c2012-11.1 // Abstract data type for SNI usage
    if (SNI_unregisterResource((void*)native_id, (SNI_closeFunction)transformation->close) != SNI_OK) {
        SNI_throwNativeException(LLSEC_RSA_CIPHER_ERROR, "Can't unregister SNI native resource");
    }
}

/**
 * @brief Gets the id of the native close function.
 * @param[in] transformation_id            The transformation ID.
 *
 * @return the id of the static native close function.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_RSA_CIPHER_IMPL_get_close_id(int32_t transformation_id)
{
    LLSEC_RSA_CIPHER_DEBUG_TRACE("%s\n", __func__);
    // cppcheck-suppress misra-c2012-11.4 // Abstract data type for SNI usage
    LLSEC_RSA_CIPHER_transformation* transformation = (LLSEC_RSA_CIPHER_transformation*)transformation_id;
    // cppcheck-suppress misra-c2012-11.1 // Abstract data type for SNI usage
    return (int32_t)transformation->close;
}
