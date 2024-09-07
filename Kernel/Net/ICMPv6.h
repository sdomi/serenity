/*
 * Copyright (c) 2024, kleines Filmröllchen <filmroellchen@serenityos.org>
 * Copyright (c) 2024, sdomi <ja@sdomi.pl>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/MACAddress.h>
#include <Kernel/Net/IPv4/IPv6.h>

// https://www.rfc-editor.org/rfc/rfc4443

// Section 2.1
// TODO: can i make this a class like klfr suggested? that broke some cast, tho..
struct ICMPv6Type {
    enum {
        DestinationUnreachable = 1,
        PacketTooBig = 2,
        TimeExceeded = 3,
        ParameterProblem = 4,
        EchoRequest = 128,
        EchoReply = 129,
        NeighborSolicitation = 135,
        NeighborAdvertisement = 136,
    };
};

class [[gnu::packed]] ICMPv6Header {
public:
    ICMPv6Header() = default;
    ~ICMPv6Header() = default;

    u8 type() const { return m_type; }
    void set_type(u8 b) { m_type = b; }

    u8 code() const { return m_code; }
    void set_code(u8 b) { m_code = b; }

    u16 checksum() const { return m_checksum; }
    void set_checksum(u16 w) { m_checksum = w; }

    void const* payload() const { return this + 1; }
    void* payload() { return this + 1; }

private:
    u8 m_type { 0 };
    u8 m_code { 0 };
    NetworkOrdered<u16> m_checksum { 0 };
};

static_assert(AssertSize<ICMPv6Header, 4>());

struct [[gnu::packed]] ICMPv6EchoPacket {
    ICMPv6Header header;
    NetworkOrdered<u16> identifier;
    NetworkOrdered<u16> sequence_number;

    void* payload() { return this + sizeof(ICMPv6EchoPacket); }
    void const* payload() const { return this + sizeof(ICMPv6EchoPacket); }
};

static_assert(AssertSize<ICMPv6EchoPacket, 8>());

struct [[gnu::packed]] ICMPv6NeighborSolicitation {
    ICMPv6Header header;
    u32 reserved;
    IPv6Address target_address;

    MACAddress* source_link_layer_address() { return bit_cast<MACAddress*>(this + sizeof(ICMPv6NeighborSolicitation)); }
    MACAddress const* source_link_layer_address() const { return bit_cast<MACAddress const*>(this + sizeof(ICMPv6NeighborSolicitation)); }
};

static_assert(AssertSize<ICMPv6NeighborSolicitation, 6 * 32 / 8>());

struct [[gnu::packed]] ICMPv6NeighborAdvertisement {
    ICMPv6Header header;
    union {
        struct [[gnu::packed]] {
            u32 _reserved1 : 5;
            bool override : 1;
            bool solicited : 1;
            bool router : 1;
            u32 _reserved2 : 24;
        };
        u32 flags;
    };
    IPv6Address target_address;
};

static_assert(AssertSize<ICMPv6NeighborAdvertisement, 6 * 32 / 8>());

struct [[gnu::packed]] ICMPv6OptionLinkLayerAddress {
    u8 type; // default: Target link-layer address
    u8 length; // fight me
    MACAddress address;
};

struct [[gnu::packed]] ICMPv6Echo {
    ICMPv6Header header;
    u16 identifier;
    u16 sequence_number;

    void* payload() { return this + 1; }
    void const* payload() const { return this + 1; }
};
