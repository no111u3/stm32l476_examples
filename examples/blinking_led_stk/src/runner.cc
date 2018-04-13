/* (c) 2018 Boris Vinogradov <no111u3@gmail.com>
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
