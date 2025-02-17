# Copyright (C) 2022, RTE (http://www.rte-france.com)
# SPDX-License-Identifier: Apache-2.0

DESCRIPTION = "A common base for guest image"

require seapath-common.inc

inherit ${@bb.utils.contains('DISTRO_FEATURES', 'seapath-security', 'security/qa-guest-images', '', d)}

IMAGE_INSTALL_append = " \
    cukinia-tests-vm \
    syslog-ng-client \
    system-config-security \
    net-snmp-server \
    net-snmp-client \
    net-snmp-dev \
    net-snmp-configuration \
    linuxptp \
"
#add docker
IMAGE_INSTALL += " docker-ce docker-ce-contrib python3-docker-compose"

IMAGE_FSTYPES = "wic.qcow2 wic.vmdk"
WKS_FILE = "mkefidisk-guest.wks.in"

# Guest images are compatible with votp-vm only
# GRUB static configuration for guest machines
# depends on votp-guest
COMPATIBLE_MACHINE = "votp-guest"

# Remove users that do not exist in
# guest images
USERS_LIST_LOCKED_remove = "qemu ceph"

def get_seapath_kernel_parameters(d):
    disable_ipv6 = d.getVar("SEAPATH_GUEST_DISABLE_IPV6", "")
    if disable_ipv6 == "true":
        return "ipv6.disable=1"
    return ""

APPEND += "${@get_seapath_kernel_parameters(d)}"
