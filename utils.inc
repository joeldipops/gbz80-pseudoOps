    IF !DEF(UTILS_INCLUDED)
UTILS_INCLUDED SET 1

INCLUDE "ops.inc"
INCLUDE "constants.inc"

;;;
; Creates a routine to multiply A with L so that the mult macro won't appear in too many places eating up precious bytes.
; @param r16 Specifies a 16bit register to be trashed by the operation.
;;;
placeMultiplyRoutine: macro
    IF !DEF(MULT_PLACED)
MULT_PLACED SET 1
multiply:
        mult L, \1
        ret      
    ELSE
        FAIL "Multiply already placed"
    ENDC
endm

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
;;;
setSpriteY: macro
    ldAny [OAMStage + (\1 * SPRITE_SIZE)], \2
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
        ldAny [OAMStage + (\1 * SPRITE_SIZE) + 2], \4        
        ldAny [OAMStage + (\1 * SPRITE_SIZE) + 3], \5        
    ELSE
        ld HL, OAMStage + (\1 * SPRITE_SIZE)
        ldiAny [HL], \3
        ldiAny [HL], \2
        ldiAny [HL], \4
        ldiAny [HL], \5
    ENDC
endm

;;;
; ors a register/value with itself to see if it's zero
; @param \1 The value.
;;; 
if0: macro
    ld A, \1
    or A, A
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

;;; 
; jpAny cc, [n16]
;
; I doubt there are any reason anyone would use this, but at least I learnt about daa
;;;
jrAny: macro
    IF ("\1" == "NC") || ("\1" == "NZ") || ("\1" == "C") || ("\1" == "Z")
        jr \1, \2
    ELSE
        IF ("\1" == "N") || ("\1" == "NN")
            ld A, $0c
            daa
            bit 2, A
        ELIF ("\1" == "H") || ("\1" == "NH")
            ; can't xor A because that will mess up the flags.
            ld A, 0
            daa
            bit 1, A            
        ELSE
            FAIL "unknown value for cc"            
        ENDC

        IF STRLEN("\1") == 2
            jr Z, \2
        ELSE
            jr NZ, \2
        ENDC
    ENDC
endm

    ENDC