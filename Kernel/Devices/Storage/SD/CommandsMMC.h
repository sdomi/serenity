/*
 * Copyright (c) 2023, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/StdLibExtras.h>
#include <AK/Types.h>

namespace Kernel::MMC { 

// todo: do we even have this?
enum class CommandType : u8 {
    Normal,
    Suspend,
    Resume,
    Abort
};

enum class ResponseType : u8 {
    NoResponse,
    ResponseOf136Bits,
    ResponseOf48Bits,
    ResponseOf48BitsWithBusy
};

enum class DataTransferDirection : u8 {
    HostToCard,
    CardToHost
};

enum class SendAutoCommand : u8 {
    Disabled,
    Command12,
    Command23
};

// 6.10.4 Detailed command description
enum class CommandIndex : u8 {
    GoIdleState = 0,
    SendOpCond = 1,
    AllSendCid = 2,
    SetRelativeAddr = 3,
    SetDsr = 4,
    SleepAwake = 5,
    Switch = 6,
    SelectDeselectCard = 7,
    SendExtCsd = 8,
    SendCsd = 9,
    SendCid = 10,
    StopTransmission = 12,
    SendStatus = 13,
    BusTestR = 14,
    GoInactiveState = 15,
    SetBlocklen = 16,
    ReadSingleBlock = 17,
    ReadMultipleBlock = 18,
    BusTestW = 19,
    SendTuningBlock = 21,
    SetBlockCount = 23,
    WriteBlock = 24,
    WriteMultipleBlock = 25,
    ProgramCid = 26,
    ProgramCsd = 27,
    SetWriteProt = 28,
    ClrWriteProt = 29,
    SendWriteProt = 30,
    SendWriteProtType = 31,
    EraseGroupStart = 35,
    EraseGroupEnd = 36,
    Erase = 38,
    FastIO = 39,
    GoIRQState = 40,
    LockUnlock = 41,
    QueuedTaskParams = 44,
    QueuedTaskAddress = 45,
    ExecuteReadTask = 46,
    ExecuteWriteTask = 47,
    Cmdq_Task_Mgmt = 48,
    SetTime = 49,
    ProtocolRd = 53,
    ProtocolWr = 54,
    AppCmd = 55,
    GenCmd = 56,
};

union Command {
    u32 raw;

    struct {
        u32 dma_enable : 1;
        u32 block_counter : 1;
        SendAutoCommand auto_command : 2;
        DataTransferDirection direction : 1;
        u32 multiblock : 1;
        u32 response_type_r1r5 : 1;         // v4.10
        u32 response_error_check : 1;       // v4.10
        u32 response_interrupt_disable : 1; // v4.10
        u32 reserved1 : 7;

        ResponseType response_type : 2;
        u32 sub_command_flag : 1; // v4.10
        u32 crc_enable : 1;
        u32 idx_enable : 1;
        u32 is_data : 1;
        CommandType type : 2;
        CommandIndex index : 6;
        u32 reserved3 : 2;
    };

    bool requires_dat_line() const
    {
        return is_data;
    }

    bool uses_transfer_complete_interrupt() const
    {
        // FIXME: I don't know how to determine this.
        return false;
    }
};
static_assert(AssertSize<Command, 4>());

namespace Commands {

constexpr Command go_idle_state = {
    .dma_enable = 0,
    .block_counter = 0,
    .auto_command = SendAutoCommand::Disabled,
    .direction = DataTransferDirection::HostToCard,
    .multiblock = 0,
    .response_type_r1r5 = 0,
    .response_error_check = 0,
    .response_interrupt_disable = 0,
    .reserved1 = 0,
    .response_type = ResponseType::NoResponse,
    .sub_command_flag = 0,
    .crc_enable = 0,
    .idx_enable = 0,
    .is_data = 0,
    .type = CommandType::Normal,
    .index = CommandIndex::GoIdleState,
    .reserved3 = 0
};

constexpr Command send_op_cond = {
    .dma_enable = 0,
    .block_counter = 0,
    .auto_command = SendAutoCommand::Disabled,
    .direction = DataTransferDirection::HostToCard,
    .multiblock = 0,
    .response_type_r1r5 = 0,
    .response_error_check = 0,
    .response_interrupt_disable = 0,
    .reserved1 = 0,
    .response_type = ResponseType::ResponseOf48Bits,
    .sub_command_flag = 0,
    .crc_enable = 0,
    .idx_enable = 0,
    .is_data = 0,
    .type = CommandType::Normal,
    .index = CommandIndex::SendOpCond,
    .reserved3 = 0
};

constexpr Command select_card = {
    .dma_enable = 0,
    .block_counter = 0,
    .auto_command = SendAutoCommand::Disabled,
    .direction = DataTransferDirection::HostToCard,
    .multiblock = 0,
    .response_type_r1r5 = 0,
    .response_error_check = 0,
    .response_interrupt_disable = 0,
    .reserved1 = 0,
    .response_type = ResponseType::ResponseOf48Bits,
    .sub_command_flag = 0,
    .crc_enable = 0,
    .idx_enable = 0,
    .is_data = 0,
    .type = CommandType::Normal,
    .index = CommandIndex::SelectDeselectCard,
    .reserved3 = 0
};

}

}
