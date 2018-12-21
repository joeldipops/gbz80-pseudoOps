    IF !DEF(ADDRESSES_INCLUDED)
ADDRESSES_INCLUDED SET 1

BackgroundMap1 EQU $9800
BackgroundMap2 EQU $9c00

JoypadIo EQU $ff00
InterruptFlags EQU $ff0f

LcdControl EQU $ff40

BackgroundScrollY EQU $ff42
BackgroundScrollX EQU $ff43

DmaSourceRegister EQU $ff46
BackgroundPalette EQU $ff47
SpritePalette1 EQU $ff48
SpritePalette2 EQU $ff49

    ENDC
