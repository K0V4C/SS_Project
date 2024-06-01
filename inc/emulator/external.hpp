#ifndef _EXTERNAL_HPP
#define _EXTERNAL_HPP


#include "emulator.hpp"

auto terminal_thread_body(Emulator *emu) -> void;

auto timer_thread_body(Emulator *emu) -> void;

#endif