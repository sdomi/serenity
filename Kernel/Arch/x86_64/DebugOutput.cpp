/*
 * Copyright (c) 2022, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Kernel/Arch/DebugOutput.h>
#include <Kernel/Arch/x86_64/BochsDebugOutput.h>
#include <Kernel/Arch/x86_64/IO.h>

#if !defined(PREKERNEL)
#    include <Kernel/Arch/Processor.h>
#endif

namespace Kernel {

static constexpr u16 serial_com1_io_port = 0x80;

void bochs_debug_output(char ch)
{
    IO::out8(IO::BOCHS_DEBUG_PORT, ch);
}

void debug_output(char ch)
{
    // static bool was_cr = false;
    (void)ch;
//     if (ch == '\n' && !was_cr) {
//         IO::out8(serial_com1_io_port, '\r');        
//         // for (int i=0; i<47768000; i++)
//         for (int i=0; i<27768000; i++)
//             asm volatile("nop");
//     }
// 
//     IO::out8(serial_com1_io_port, ch);
// 
//     // for (int i=0; i<47768000; i++)
//     for (int i=0; i<27768000; i++)
//         asm volatile("nop");
// 
//     was_cr = ch == '\r';
}

void debug_output_cool(char ch)
{
    static bool was_cr = false;
    if (ch == '\n' && !was_cr) {
        IO::out8(serial_com1_io_port, '\r');
        for (int i=0; i<27768000; i++)
            asm volatile("nop");
    }

    IO::out8(serial_com1_io_port, ch);

    for (int i=0; i<27768000; i++)
        asm volatile("nop");

    was_cr = ch == '\r';
}

}
