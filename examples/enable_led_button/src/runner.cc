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

#include <runner.hh>

void runner::setup() {
    // Enable gpiob
    hal::rcc::device_enable<
        hal::rcc_device::gpioben,
        hal::rcc_device::gpioaen>();
    // Set PB2 to output
    hal::gpiob::set_mode<hal::pins::mode::output, hal::pins::p2>();
    // Set PA0 to input
    hal::gpioa::set_mode<hal::pins::mode::input, hal::pins::p0>();
}

void runner::run() {
    // Infinity loop
    while (true) {
        if (hal::gpioa::get_value<hal::pins::p0>()) {
            // Enable PB2
            hal::gpiob::set_value<hal::pins::p2>();
        }
        else {
            // Disable PB2
            hal::gpiob::reset_value<hal::pins::p2>();
        }
    }
}
