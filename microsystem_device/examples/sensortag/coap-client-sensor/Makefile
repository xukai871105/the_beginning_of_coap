all: coap-client-sensor

#CONTIKI=../..
#CONTIKI=/home/user/contiki-3.0

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

# linker optimizations
SMALL=1

# REST Engine shall use Erbium CoAP implementation
APPS += er-coap
APPS += rest-engine

CONTIKI_WITH_IPV6 = 1

USER_FOLDER = ../../..
include $(USER_FOLDER)/Makefile.user.include





