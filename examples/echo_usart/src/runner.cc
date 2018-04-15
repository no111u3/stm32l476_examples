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
#include <hal/usart.hh>

#include <in.hh>
#include <out.hh>

#include <runner.hh>

namespace {
    // Universal output for USART2
    using sout = lp::out<hal::usart2>;
    // Universal input for USART2
    using sin = lp::in<hal::usart2>;
    // Main system clock
    static constexpr auto msc = 4'000'000;
}

void runner::setup() {
    // Enable gpiod ports, usart2
    hal::rcc::device_enable<
        hal::rcc_device::gpioden,
        hal::rcc_device::usart2en>();
    // Set PD5, PD6 to alternative USART2
    hal::gpiod::set_mode<hal::pins::mode::alt_func, hal::pins::p5, hal::pins::p6>();
    hal::gpiod::set_alt_func<hal::device::alt::usart1_2_3, hal::pins::p5, hal::pins::p6>();
    // Setup USART2 -> enable tx, rx pins and set baudrate to 115200
    hal::usart2::setup<
        hal::usart2::config::txrx,
        hal::usart2::config::baudrate<msc, 115200>
    >();
}

void runner::run() {
    // Send one char to output
    sout::send("\r\nHello from stm32l476-discovery\r\n");
    sout::send("This is echo serial. ver:", 1, '.', 0, 0, "\r\n");
    // Echo loop
    char input;
    do {
        sin::recv(input);
        if (input == '\r')
            sout::send('\n');
    } while (input);
    // Infinity loop
    while (true);
}
