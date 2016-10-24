CONTIKI_PROJECT = sensor-demo
all: $(CONTIKI_PROJECT)

DEFINES+=PROJECT_CONF_H=\"project-conf.h\"
DEFINES+=STARTUP_CONF_VERBOSE=1

CONTIKI_WITH_IPV6 = 1

USER_FOLDER = ../../..
include $(USER_FOLDER)/Makefile.user.include
