    IF !DEF(ADDRESSES_INCLUDED)
ADDRESSES_INCLUDED SET 1

; VRAM addresses
TileData EQU $8000
BackgroundMap1 EQU $9800
BackgroundMap2 EQU $9c00

; HRAM addresses
JoypadIo EQU $ff00              ; P1
LinkData EQU $ff01              ; SB
LinkControl EQU $ff02           ; SC

TimerClock EQU $ff04            ; DIV
TimerCounter EQU $ff05          ; TIMA
TimerResetValue EQU $ff06       ; TMA
TimerControl EQU $ff07          ; TAC
InterruptFlags EQU $ff0f        ; IF

AudioChannel1Sweep EQU $ff10            ; NR 10
AudioChannel1PatternAndLength EQU $ff11 ; NR 11
AudioChannel1Envelope EQU $ff12         ; NR 12
AudioChannel1Frequency EQU $ff13        ; NR 13
AudioChannel1Flags EQU $ff14            ; NR 14

AudioChannel2PatternAndLength EQU $ff16 ; NR 21
AudioChannel2Envelope EQU $ff17         ; NR 22
AudioChannel2Frequency EQU $ff18        ; NR 23
AudioChannel2Flags EQU $ff19            ; NR 24

AudioChannel3Control EQU $ff1a          ; NR 30
AudioChannel3Length EQU $ff1b           ; NR 31
AudioChannel3Level EQU $ff1c            ; NR 32
AudioChannel3Frequency EQU $ff1d        ; NR 33
AudioChannel3Flags EQU $ff1e            ; NR 34

AudioChannel4Length EQU $ff20           ; NR 41
AudioChannel4Envelope EQU $ff21         ; NR 42
AudioChannel4RNGParameters EQU $ff22    ; NR 43
AudioChannel4Flags EQU $ff23            ; NR 44

AudioSpeakerControl EQU $ff24   ; NR 50
AudioSpeakerChannels EQU $ff25  ; NR 51
AudioChannelSwitch EQU $ff26    ; NR 52

AudioChannel3Data EQU $ff30     ; 16 byte Wave Patterm

LcdControl EQU $ff40            ; LCDC
LcdStatus EQU $ff41             ; STAT

BackgroundScrollY EQU $ff42     ; SCY
BackgroundScrollX EQU $ff43     ; SCX
CurrentLine EQU $ff44           ; LY
NextInterruptLine EQU $ff45     ; LYC 

DmaSourceRegister EQU $ff46     ; DMA
BackgroundPalette EQU $ff47     ; BGP
SpritePalette0 EQU $ff48        ; OBP0
SpritePalette1 EQU $ff49        ; OBP1

WindowTop EQU $ff4a             ; WY
WindowLeft EQU $ff4b            ; WX
InterruptSwitch EQU $ffff       ; IE


; Memory Bank Controller Write-only addresses

RomBankSelector EQU $2000
HighRomBankSelector EQU $3000   ; For MBCs with more than 256 banks, the highest bit is set here.
RamSwitch EQU $0000

RamSelector EQU $4000
BankMode EQU $6000

RealTimeClockMode EQU $4000
LatchRealTimeClock EQU $6000    ; Stops the RTC from updating the registers, so the same value can be read until it is unlatched.

    ENDC