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
 * @file hardpoint.hpp
 *
 *
 * @author Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#pragma once

#include <uavcan/uavcan.hpp>
#include <uavcan/equipment/hardpoint/Command.hpp>
#include <uavcan/equipment/hardpoint/Status.hpp>
#include <systemlib/perf_counter.h>
//#include <uORB/topics/esc_status.h>


class UavcanHardpointController
{
public:
	UavcanHardpointController(uavcan::INode &node);
	~UavcanHardpointController();

	int init();

	void update_output(int output);

private:

	static constexpr unsigned MAX_RATE_HZ = 1;			///< XXX make this configurable
	static constexpr unsigned HARDPOINT_STATUS_UPDATE_RATE_HZ = 1;

	typedef uavcan::MethodBinder<UavcanHardpointController *,
	void (UavcanHardpointController::*)(const uavcan::ReceivedDataStructure<uavcan::equipment::hardpoint::Status>&)>
	StatusCbBinder;

	typedef uavcan::MethodBinder<UavcanHardpointController *, void (UavcanHardpointController::*)(const uavcan::TimerEvent &)>
	TimerCbBinder;

	//    hardpoint_status_s	_hardpoint_status = {};
	orb_advert_t	_hardpoint_status_pub = nullptr;

	/*
	 * libuavcan related things
	 */
	uavcan::MonotonicTime							_prev_cmd_pub;   ///< rate limiting
	uavcan::INode								&_node;
	uavcan::Publisher<uavcan::equipment::hardpoint::Command>			_uavcan_pub_raw_cmd;
	//	uavcan::Subscriber<uavcan::equipment::hardpoint::Status, StatusCbBinder>	_uavcan_sub_status;
	uavcan::TimerEventForwarder<TimerCbBinder>				_orb_timer;

	/*
	 * Perf counters
	 */
	perf_counter_t _perfcnt_invalid_input = perf_alloc(PC_COUNT, "uavcan_hardpoint_invalid_input");
	perf_counter_t _perfcnt_scaling_error = perf_alloc(PC_COUNT, "uavcan_hardpoint_scaling_error");
};
