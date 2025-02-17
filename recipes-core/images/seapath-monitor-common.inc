# Copyright (C) 2022, RTE (http://www.rte-france.com)
# SPDX-License-Identifier: Apache-2.0

USERS_LIST_LOCKED_remove = "qemu"

IMAGE_INSTALL_append = " \
    cukinia-tests-monitor \
    system-upgrade \
"

COMPATIBLE_MACHINE = "votp-monitor"

def get_seapath_kernel_parameters(d):
    disable_ipv6 = d.getVar("SEAPATH_DISABLE_IPV6", "")
    if disable_ipv6 == "true":
        return "ipv6.disable=1"
    return ""

APPEND += "${@get_seapath_kernel_parameters(d)}"
