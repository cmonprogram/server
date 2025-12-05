################################################################################
#
# server
#
################################################################################

SERVER_VERSION = 1.0
SERVER_SITE = ./package/server
SERVER_SITE_METHOD = local

define SERVER_BUILD_CMDS
	# Change directory to the build folder and run the local Makefile
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D) server 
endef

	# Define the custom installation commands for the target filesystem
define SERVER_INSTALL_TARGET_CMDS
	# Copy the compiled 'server' binary to the target's /usr/bin directory
	$(INSTALL) -D -m 0755 $(@D)/server $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))