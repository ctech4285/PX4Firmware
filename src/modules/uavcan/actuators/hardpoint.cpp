/****************************************************************************
 *
 *   Copyright (C) 2014 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file hardpoint.cpp
 *
 * @author Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#include "hardpoint.hpp"
#include <systemlib/err.h>


UavcanHardpointController::UavcanHardpointController(uavcan::INode &node) :
    _node(node),
    _uavcan_pub_raw_cmd(node),
    _orb_timer(node)
{
    if (_perfcnt_invalid_input == nullptr) {
        errx(1, "uavcan: couldn't allocate _perfcnt_invalid_input");
    }

    if (_perfcnt_scaling_error == nullptr) {
        errx(1, "uavcan: couldn't allocate _perfcnt_scaling_error");
    }
}

UavcanHardpointController::~UavcanHardpointController()
{
    perf_free(_perfcnt_invalid_input);
    perf_free(_perfcnt_scaling_error);
}

int UavcanHardpointController::init()
{
    return true; // res;
}

void UavcanHardpointController::update_output(int output)
{


    /*
     * Rate limiting - we don't want to congest the bus
     */
    const auto timestamp = _node.getMonotonicTime();

    if ((timestamp - _prev_cmd_pub).toUSec() < (1000000 / MAX_RATE_HZ)) {
        return;
    }

    _prev_cmd_pub = timestamp;

    /*
     * Fill the command message
     */
    uavcan::equipment::hardpoint::Command msg;
    msg.command = output;
    // msg.cmd.push_back(output);
    // msg.Command_.
    /*
     * Publish the command message to the bus
     */
    (void)_uavcan_pub_raw_cmd.broadcast(msg);
}

