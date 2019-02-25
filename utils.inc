    IF !DEF(UTILS_INCLUDED)
UTILS_INCLUDED SET 1

INCLUDE "ops.inc"
INCLUDE "constants.inc"

;;;
; For sprite updates to work, we need to know where in memory they sit.
; Call this before using any sprite macros.
; @param \1 Memory address of sprite attributes before they are copied to V-RAM
;;;
setOAMStage: macro
OAMStage SET \1
endm

;;;
; Updates the flags of a given sprite.
; @param \1 Sprite number
; @param \2 The flag byte.
;;;
setSpriteFlags: macro
    ldAny [OAMStage + (\1 * SPRITE_SIZE) + 3], \2
endm

;;;
; Sets the sprite to a new Y location.
; @param \1 Sprite number
; @param \2 Y co-ordinate.
; @param \3 1 if \2 is the accumulator, 0 if it's not.
;;;
setSpriteY: macro
    IF \3 == 1
        ld [OAMStage + (\1 * SPRITE_SIZE)], \2
    ELSE
        ldAny [OAMStage + (\1 * SPRITE_SIZE)], \2
    ENDC
endm

;;;
; Sets all of a given sprite's attributes at once.
; @param \1 Sprite number
; @param \2 X position
; @param \3 Y position
; @param \4 Pointer to image tile
; @param \5 Flags
; @param \6 preserve HL - if 0, HL will be affected but will use fewer cycles/bytes, if 1, HL will be unchanged.
;;;
updateSprite: macro
    IF (\6 == 1)
        ldAny [OAMStage + (\1 * SPRITE_SIZE)], \3
        ldAny [OAMStage + (\1 * SPRITE_SIZE) + 1], \2
        ldAny [OAMStage + (\1 * SPRITE_SIZE) + 1], \4        
        ldAny [OAMStage + (\1 * SPRITE_SIZE) + 1], \5        
    ELSE
        ld HL, OAMStage + (\1 * SPRITE_SIZE)
        ldiAny [HL], \3
        ldiAny [HL], \2
        ldiAny [HL], \4
        ldiAny [HL], \5
    ENDC
endm

;;;
; Pad out a number of bytes.
; @param \1 number of bytes required.
; @param \2 0 to pad with nop, 1 to pad with rst $00
;;;
pad: macro
    IF \2 == 0
        REPT \1
            nop
        ENDR
    ELIF \2 == 1
        REPT \1
            rst $00
        ENDR
    ENDC
endm    

    ENDC