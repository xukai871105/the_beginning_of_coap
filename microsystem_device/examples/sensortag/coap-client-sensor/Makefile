# make BOARD=sensortag/cc2650
DEFINES+=PROJECT_CONF_H=\"project-conf.h\"
DEFINES+=STARTUP_CONF_VERBOSE=1

all: coap-client-sensor
# linker optimizations
# SMALL=1

# REST Engine shall use Erbium CoAP implementation
APPS += er-coap
APPS += rest-engine

CONTIKI_WITH_IPV6 = 1

USER_FOLDER = ../../..
include $(USER_FOLDER)/Makefile.user.include





