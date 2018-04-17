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

#include <hal/exti.hh>
#include <hal/gpio.hh>
#include <hal/nvic.hh>
#include <hal/rcc.hh>
#include <hal/syscfg.hh>

#include <isr_extend.hh>

#include <runner.hh>

extern "C" void isr::EXTI0() {
    if (hal::gpioa::get_value<hal::pins::p0>()) {
        // Enable PB2
        hal::gpiob::set_value<hal::pins::p2>();
    }
    else {
        // Disable PB2
        hal::gpiob::reset_value<hal::pins::p2>();
    }
}

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
    // Route PA0 to EXTI0
    hal::syscfg::set_route<
        hal::device::exti::gpio0,
        hal::device::exti_route::gpioa
    >();
    // Setup EXTI0 to rising edge
    hal::exti::rising_edge_en<hal::device::exti::gpio0>();
    // Setup EXTI0 to failing edge
    hal::exti::faling_edge_en<hal::device::exti::gpio0>();
    // Enable EXTI0 interrupt
    hal::exti::unmask_int<hal::device::exti::gpio0>();
    // Enable irq for EXTI0
    hal::nvic::enable_irq<hal::irq_dev_num_t::EXTI0>();
    // Infinity loop
    while (true);
}
