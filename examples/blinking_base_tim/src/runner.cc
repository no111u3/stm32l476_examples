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
#include <hal/nvic.hh>
#include <hal/rcc.hh>
#include <hal/tim.hh>

#include <isr_extend.hh>

#include <runner.hh>

namespace {
    // Main system clock
    static constexpr auto msc = 4'000'000;
}

extern "C" void isr::TIM6_DACUNDER() {
    if (hal::tim6::get_status<tim6::sr_uif>()) {
        // Change PB2 state
        hal::gpiob::output::modify<hal::pins::p2::op_xor>();

        hal::tim6::clear_status<tim6::sr_uif>();
    }
}

extern "C" void isr::TIM7() {
    if (hal::tim7::get_status<tim7::sr_uif>()) {
        // Change PE8 state
        hal::gpioe::output::modify<hal::pins::p8::op_xor>();

        hal::tim7::clear_status<tim7::sr_uif>();
    }
}


void runner::setup() {
    // Enable gpiob, gpioe, tim6, tim7
    hal::rcc::device_enable<
        hal::rcc_device::gpioben,
        hal::rcc_device::gpioeen,
        hal::rcc_device::tim6en,
        hal::rcc_device::tim7en>();
    // Set PB2, PE8 to output
    hal::gpiob::set_mode<hal::pins::mode::output, hal::pins::p2>();
    hal::gpioe::set_mode<hal::pins::mode::output, hal::pins::p8>();
}

void runner::run() {
    // Enable PB2, PE8
    hal::gpiob::set_value<hal::pins::p2>();
    hal::gpioe::set_value<hal::pins::p8>();
    // Enable irq for TIM6, TIM7
    hal::nvic::enable_irq<hal::irq_dev_num_t::TIM6_DACUNDER>();
    hal::nvic::enable_irq<hal::irq_dev_num_t::TIM7>();
    // Start blinking throw TIM6, TIM7 timer every 1s, 1/3s
    hal::tim6::setup<
        hal::tim6::config::update_int_enable,
        hal::tim6::config::prescaller<msc/4000>,
        hal::tim6::config::reload_value<4000>
    >();
    hal::tim7::setup<
        hal::tim7::config::update_int_enable,
        hal::tim7::config::prescaller<msc/3/4000>,
        hal::tim7::config::reload_value<4000>
    >();
    // Infinity loop
    while (true);
}
