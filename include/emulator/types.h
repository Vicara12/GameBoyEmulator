#pragma once

#include <cstdint>
#include <array>

using Reg = uint8_t;
using Byte = uint8_t;
using SByte = int8_t;
using DReg = uint16_t;
using Short = uint16_t;

using GameRom = std::array<Byte, 0x8000>;