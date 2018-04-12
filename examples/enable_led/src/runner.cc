/* (c) 2018 Boris Vinogradov <no111u3@gmail.com>
 * System setup and run main thread
 * @file runner.cc
 * @author Boris Vinogradov
 */

#include <hal/gpio.hh>
#include <hal/rcc.hh>

#include <runner.hh>

void runner::setup() {
    // Enable gpiob
    hal::rcc::device_enable<hal::rcc_device::gpioben>();
    // Set PB2 to output
    hal::gpiob::set_mode<hal::pins::mode::output, hal::pins::p2>();
}

void runner::run() {
    // Enable PB2
    hal::gpiob::set_value<hal::pins::p2>();
    // Infinity loop
    while (true);
}
