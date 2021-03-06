    IF !DEF(CONSTANTS_INCLUDED)
CONSTANTS_INCLUDED SET 1

;;;;;;;;;;;;;;; HEADER OPTIONS ;;;;;;;;;;;;;;;;;;;;;;

;; CGB Support ;;
NO_COLOUR_SUPPORT EQU $00
COLOUR_SUPPORT EQU $80
COLOUR_ONLY EQU $C0

;; Licensee  code ;;
UNLICENSED EQU "00"

;; SGB Support ;;
NO_SGB_SUPPORT EQU $00
SGB_SUPPORTED EQU $03


;; Catridge types ;;

; 32kB ROM
ROM_ONLY EQU $00 

; MBC1 - max 2MB ROM and/or 32kB RAM)
MBC1 EQU $01
MBC1_RAM EQU $02
MBC1_BATTERY EQU $03

; MBC2 - max 256kB ROM and 256B RAM
MBC2 EQU $05
MBC2_BATTERY EQU $06
MBC2_RAM EQU $08
MBC2_RAM_BATTERY EQU $09

; MMO1 - max 8MB ROM and 128kB RAM
MMM01 EQU $0b
MMM01_RAM EQU $0c
MMM01_RAM_BATTERY EQU $0d

; MBC3 - max 2MB ROM and/or 32kB RAM and Timer
MBC3_TIMER_BATTERY EQU $0f
MBC3_TIMER_RAM_BATTERY EQU $10
MBC3 EQU $11
MBC3_RAM EQU $12
MBC3_RAM_BATTERY EQU $13

; MBC5 - max 8MB ROM and/or 128kB RAM
MBC5 EQU $19
MBC5_RAM EQU $1a
MBC5_RAM_BATTERY EQU $1b
MBC5_RUMBLE EQU $1c
MBC5_RUMBLE_RAM EQU $1d
MBC5_RUMBLE_RAM_BATTER EQU $1e

; MBC6 - who knows?
MBC6 EQU $20

; MBC7 - max 8MB ROM or 256kB RAM and Accelerometer
MBC7_SENSOR_RUMBLE_RAM_BATTERY EQU $22


;; ROM SIZE ;;

ROM_32kB EQU $00 ; no banks
ROM_64kB EQU $01 ; 4 banks
ROM_128kB EQU $02 ; 8 banks
ROM_256kB EQU $03 ; 16 banks
ROM_512kB EQU $04 ; 32 banks
ROM_1MB EQU $05 ; 64 banks
ROM_2MB EQU $06 ; 128 banks
ROM_4MB EQU $07 ; 256 banks
ROM_8MB EQU $08 ; 512 banks
ROM_1_1MB EQU $52 ; 1.1MB, 72 banks
ROM_1_2MB EQU $53 ; 1.2MB 80 banks
ROM_1_5MB EQU $54 ; 1/5MB 96 banks

 
 ;; RAM size ;;

NO_RAM EQU $00
RAM_2kB EQU $01
RAM_8kB EQU $02
RAM_32Kb EQU $03
RAM_128kB EQU $04
RAM_64kB EQU $05

;; Destination code ;;
FOREIGN_DESTINATION EQU $01

;; Original Licensee code ;;
USE_NEW_LICENSEE_CODE EQU $33


;;;;;;;;;;;;;;; IO Register Options ;;;;;;;;;;;;;;;;;;;;;;


; Interrupt Flags
INTERRUPT_VBLANK EQU %00000001

; Joypad Flags
JOYPAD_GET_BUTTONS  EQU %00100000
JOYPAD_GET_DPAD     EQU %00010000
JOYPAD_CLEAR        EQU %00110000

;; Audio Related ;;
LEFT_SPEAKER_ON EQU %10000000
RIGHT_SPEAKER_ON EQU %00001000
MAX_VOLUME EQU %11111111

; TODO
CHANNEL_4_LEFT_SPEAKER  EQU %00000000
CHANNEL_3_LEFT_SPEAKER  EQU %00000000
CHANNEL_2_LEFT_SPEAKER  EQU %00000000
CHANNEL_1_LEFT_SPEAKER  EQU %00000000
CHANNEL_4_RIGHT_SPEAKER EQU %00000000
CHANNEL_3_RIGHT_SPEAKER EQU %00000000
CHANNEL_2_RIGHT_SPEAKER EQU %00000000
CHANNEL_1_RIGHT_SPEAKER EQU %00000000

AUDIO_ON_BIT EQU 7

;; LCD Related ;;

; LCD Control Flags
LCD_OFF EQU %00000000
LCD_ON EQU %10000000
LCD_ON_BIT EQU 7
WINDOW_MAP_1 EQU %00000000
WINDOW_MAP_2 EQU %01000000
WINDOW_OFF EQU %00000000
WINDOW_ON EQU %00100000
TILE_DATA_88 EQU %00000000
TILE_DATA_80 EQU %00010000
BACKGROUND_MAP_1 EQU %00000000
BACKGROUND_MAP_2 EQU %00001000
SQUARE_TILES EQU %00000000
LONG_TILES EQU %00000100
SPRITES_OFF EQU %00000000
SPRITES_ON EQU %00000010
BACKGROUND_OFF EQU %00000000
BACKGROUND_ON EQU %00000001
VRAM_BUSY EQU %00000010

SCREEN_HEIGHT EQU 144
SCREEN_WIDTH EQU 160
BACKGROUND_HEIGHT EQU 18
BACKGROUND_WIDTH EQU 20
CANVAS_WIDTH EQU 256
CANVAS_HEIGHT EQU 256

SCREEN_BYTE_WIDTH EQU 32
SCREEN_BYTE_HEIGHT EQU 32
SPRITE_WIDTH EQU 8


;;;;;;;;;;;;;;; DMA/OAM Related ;;;;;;;;;;;;;;;;;;

DMA_WAIT_TIME EQU $28
OAM_SIZE EQU $A0
SPRITE_SIZE EQU 4
SPRITE_Y EQU 0
SPRITE_X EQU 1
SPRITE_TILE EQU 2
SPRITE_FLAGS EQU 3
SPRITE_COUNT EQU 40

HAS_PRIORITY    EQU %00000000
NO_PRIORITY     EQU %10000000
IS_Y_FLIPPED    EQU %01000000
IS_X_FLIPPED    EQU %00100000
USE_PALETTE_0   EQU %00000000
USE_PALETTE_1   EQU %00010000


;;;;;;;;;;;;;;; Mmemory Bank Related ;;;;;;;;;;;;;

DISABLE_RAM EQU $00
ENABLE_RAM EQU  $0a

ROM_BANK_MODE EQU $00
RAM_BANK_MODE EQU $01

CLOCK_MODE_SECONDS EQU  $08
CLOCK_MODE_MINUTES EQU  $09
CLOCK_MODE_HOURS EQU    $0a
CLOCK_MODE_DAY EQU      $0b
CLOCK_MODE_FLAGS EQU    $0c

CLOCK_HIGH_DAY EQU      %00000001
CLOCK_HALT EQU          %01000000
CLOCK_DAY_OVERFLOW EQU  %10000000

    ENDC
