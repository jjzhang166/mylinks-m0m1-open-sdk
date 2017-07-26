
//==========================================================================
//
//      include/lib/lib_sha256.h
//
//==========================================================================

#ifndef __LIB_SHA256_H__
#define __LIB_SHA256_H__

#define SHA256_MAC_LEN	32

/* lib/crypto/sha256.c */
void hmac_sha256_vector(const u8 *key, size_t key_len, size_t num_elem,
			const u8 *addr[], const size_t *len, u8 *mac);
void hmac_sha256(const u8 *key, size_t key_len, const u8 *data,
		 size_t data_len, u8 *mac);
void sha256_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac);

#endif /* __LIB_SHA256_H__ */
