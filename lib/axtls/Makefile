include $(TOPDIR)/.lib_config

EXTRA_CFLAGS += -I./crypto -I./ssl -I./config -I$(TOPDIR)/include/freertos 

CRYPT_OBJS += $(patsubst %, crypto/%, aes.o bigint.o hmac.o rc4.o rsa.o sha256.o crypto_misc.o)

ifdef (CONFIG_TLS_SHA384_SHA512)
CRYPT_OBJS += sha384.o sha512.o 
endif

SRC_OBJS += $(patsubst %, ssl/%, asn1.o loader.o p12.o tls1.o tls1_clnt.o x509.o os_port.o)

OBJS +=  $(SRC_OBJS) $(CRYPT_OBJS) _muldi3.o _udivdi3.o _umoddi3.o


include $(TOPDIR)/rules.mak
all: $(LIBA)
clean:
	find -regex '.*\.[oasd]' -print | xargs rm -f
