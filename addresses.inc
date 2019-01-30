    IF !DEF(ADDRESSES_INCLUDED)
ADDRESSES_INCLUDED SET 1

;;;;;;;;;;;;;;; Hardware Addresses ;;;;;;;;;;;;;;;;;;;;;
MemoryBankSelector EQU $2000

TileData EQU $8000
BackgroundMap1 EQU $9800
BackgroundMap2 EQU $9c00

JoypadIo EQU $ff00              ; P1
LinkData EQU $ff01              ; SB
LinkControl EQU $ff02           ; SC

TimerControl EQU $ff07          ; TAC
InterruptFlags EQU $ff0f        ; IF

AudioSpeakerControl EQU $ff24   ; NR 50
AudioSpeakerChannels EQU $ff25  ; NR 51
AudioChannelSwitch EQU $ff26    ; NR 52

AudioChannel3Data EQU $ff30     ; 16 byte Wave Patterm

LcdControl EQU $ff40            ; LCDC
LcdStatus EQU $ff41             ; STAT

BackgroundScrollY EQU $ff42     ; SCY
BackgroundScrollX EQU $ff43     ; SCX
CurrentLine EQU $ff44           ; LY

DmaSourceRegister EQU $ff46     ; DMA
BackgroundPalette EQU $ff47     ; BGP
SpritePalette0 EQU $ff48        ; OBP0
SpritePalette1 EQU $ff49        ; OBP1

WindowTop EQU $ff4a             ; WY
WindowLeft EQU $ff4b            ; WX
InterruptSwitch EQU $ffff       ; IE

    ENDC
