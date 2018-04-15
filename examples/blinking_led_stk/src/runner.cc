/* Copyright 2018 Boris Vinogradov <no111u3@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * System setup and run main thread
 * @file runner.cc
 * @author Boris Vinogradov
 */

#include <hal/gpio.hh>
#include <hal/rcc.hh>
#include <hal/stk.hh>

#include <isr_base.hh>

#include <runner.hh>

namespace {
    // Main system clock
    static constexpr auto msc = 4'000'000;
}

extern "C" void isr::sys_tick_timer() {
    // Change PB2 state
    hal::gpiob::output::modify<hal::pins::p2::op_xor>();
}

void runner::setup() {
    // Enable gpiob
    hal::rcc::device_enable<hal::rcc_device::gpioben>();
    // Set PB2 to output
    hal::gpiob::set_mode<hal::pins::mode::output, hal::pins::p2>();
}

void runner::run() {
    // Enable PB2
    hal::gpiob::set_value<hal::pins::p2>();
    // Start blinking throw SysTick timer every 1s
    hal::stk::config<msc>();
    // Infinity loop
    while (true);
}
