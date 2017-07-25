include $(TOPDIR)/.lib_config

ifdef CONFIG_LWMQTT
OBJS += mqtt.o
else
OBJS += MQTTClient.o MQTTConnectClient.o MQTTFreeRTOS.o MQTTPacket.o
OBJS += MQTTSubscribeClient.o MQTTUnsubscribeClient.o
OBJS += MQTTSerializePublish.o MQTTDeserializePublish.o
endif

include $(TOPDIR)/rules.mak
all: $(LIBA)
clean:
	find -regex '.*\.[oasd]' -print | xargs rm -f
