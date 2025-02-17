# Copyright (C) 2022, RTE (http://www.rte-france.com)
# SPDX-License-Identifier: Apache-2.0

DESCRIPTION = "A production image for Seapath"
require seapath-common.inc
require ${@bb.utils.contains('DISTRO_FEATURES', 'seapath-clustering', 'seapath-host-common-ha.inc', '', d)}
require seapath-host-common-virtu.inc

inherit ${@bb.utils.contains('DISTRO_FEATURES', 'seapath-security', 'security/qa-host-images', '', d)}

# Hypervisor and cluster tests
IMAGE_INSTALL_append = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'seapath-clustering', 'cukinia-tests-cluster', '', d)} \
    cukinia-tests-hypervisor \
    ${@bb.utils.contains('DISTRO_FEATURES', 'seapath-readonly', 'cukinia-tests-hypervisor-readonly', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'seapath-security', 'cukinia-tests-hypervisor-security', '', d)} \
    syslog-ng-client \
    system-config-host \
    ${@bb.utils.contains('DISTRO_FEATURES', 'seapath-security', 'system-config-security', '', d)} \
    system-upgrade \
    net-snmp-server \
    net-snmp-client \
    net-snmp-dev \
    python3-vm-manager \
    linuxptp \
"

IMAGE_INSTALL_append = " cukinia-tests-hypervisor-iommu"
IMAGE_INSTALL_remove_votp-no-iommu = "cukinia-tests-hypervisor-iommu"

# Host images are compatible with votp-host only
# GRUB static configuration for host machines
# depends on votp-host
COMPATIBLE_MACHINE = "votp-host"

def get_seapath_kernel_parameters(d):
    kernel_parameters = ""
    rt_cores = d.getVar("SEAPATH_RT_CORES", "")
    disable_ipv6 = d.getVar("SEAPATH_DISABLE_IPV6", "")
    if rt_cores:
        kernel_parameters +="isolcpus={} nohz_full={} rcu_nocbs={}".format(
            rt_cores,
            rt_cores,
            rt_cores)
    if disable_ipv6 == "true":
        kernel_parameters += " ipv6.disable=1"
    return kernel_parameters


APPEND += "${@get_seapath_kernel_parameters(d)}"
