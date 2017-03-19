# name of your application
APPLICATION = gpio_to_uart

# If no BOARD is found in the environment, use this default:
BOARD ?= samr21-xpro

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

USEMODULE += xtimer
FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_uart

# You can use another pin by simply replacing LED0_PIN
# with "GPIO_PIN\(<port>, <pin>\)", set <port> and <pin>
# to your needs.
LED_GPIO ?= LED0_PIN
CFLAGS += -DLED_GPIO=$(LED_GPIO)

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
