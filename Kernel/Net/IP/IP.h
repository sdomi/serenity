/*
 * Copyright (c) 2024, kleines Filmröllchen <filmroellchen@serenityos.org>
 * Copyright (c) 2024, sdomi <ja@sdomi.pl>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Types.h>

namespace Kernel {

// IPv4 "Protocol" field or IPv6 "Next Header" field.
// Included in this enum are only transport layer protocols, not IPv6 extended headers.
// https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
enum class TransportProtocol : u8 {
    ICMP = 1,
    TCP = 6,
    UDP = 17,
    ICMPv6 = 58,
};

NetworkOrdered<u16> internet_checksum(void const*, size_t);
inline NetworkOrdered<u16> internet_checksum(void const* ptr, size_t count)
{
    u32 checksum = 0;
    auto* w = (u16 const*)ptr;
    while (count > 1) {
        checksum += AK::convert_between_host_and_network_endian(*w++);
        if (checksum & 0x80000000)
            checksum = (checksum & 0xffff) | (checksum >> 16);
        count -= 2;
    }
    while (checksum >> 16)
        checksum = (checksum & 0xffff) + (checksum >> 16);
    return ~checksum & 0xffff;
}

}
