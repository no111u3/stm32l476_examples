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
    // Direction to change value
    volatile bool foward = true;
    static constexpr auto add_value = 1;
}

extern "C" void isr::TIM4() {
    if (hal::tim4::get_status<hal::tim4::status::update_int>()) {
        if (foward) {
            if (hal::tim4::channel1::get() < add_value)
                foward = false;
            else {
                hal::tim4::channel1::get() -= add_value;
                hal::tim4::channel2::get() -= add_value;
            }
        } else {
            if (hal::tim4::channel1::get() > 1000)
                foward = true;
            else {
                hal::tim4::channel1::get() += add_value;
                hal::tim4::channel2::get() += add_value;
            }
        }

        hal::tim4::clear_status<hal::tim4::status::update_int>();
    }
}

void runner::setup() {
    // Enable gpiob, tim4
    hal::rcc::device_enable<
        hal::rcc_device::gpioben,
        hal::rcc_device::tim4en>();
    // Set PB6, PD7 to alternative TIM4
    hal::gpiob::set_mode<
        hal::pins::mode::alt_func,
        hal::pins::p6, hal::pins::p7>();
    hal::gpiob::set_alt_func<
        hal::device::alt::tim1_2_3_4_5,
        hal::pins::p6, hal::pins::p7>();
}

void runner::run() {
    // Enable irq for TIM4
    hal::nvic::enable_irq<hal::irq_dev_num_t::TIM4>();
    // Start blinking pwm throw TIM4 with 1kHz clock
    hal::tim4::channel_setup<
        hal::tim4::channel_config::ch1_output_enable,
        hal::tim4::channel_config::ch2_output_enable,
        hal::tim4::channel_config::ch1_mode<
            hal::tim4::channel_config::mode::output>,
        hal::tim4::channel_config::ch2_mode<
            hal::tim4::channel_config::mode::output>,
        hal::tim4::channel_config::ch1_output_mode<
            hal::tim4::channel_config::output_mode::pwm1>,
        hal::tim4::channel_config::ch2_output_mode<
            hal::tim4::channel_config::output_mode::pwm2>,
        hal::tim4::channel_config::ch1_value<500>,
        hal::tim4::channel_config::ch2_value<500>
    >();
    hal::tim4::setup<
        hal::tim4::config::update_int_enable,
        hal::tim4::config::prescaller<msc/1000/1000>,
        hal::tim4::config::reload_value<1000>
    >();
    // Infinity loop
    while (true);
}
