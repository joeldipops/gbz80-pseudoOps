;;;
; Loads a byte from anywhere into anything that holds a byte
; @usage ldAny
;;;
ldAny: macro
    ld A, \2
    ld \1, A
endm

;;;
; Loads a byte from anywhere into a memory address greater than $ff00
; @usage ldhAny [Channel2Envelope], [nextEnvelope]
;;;
ldhAny: macro
    ld A, \2
    ldh \1, A
endm

;;;
; Loads a byte from [HL] into anything that holds a byte and then increments HL
; @usage ldiAny [$c123], [HL]
;;;
ldiAny: macro
    ldi A, [HL]
    ld \1, A
endm

;;;
; Performs an OR on any two bytes. Usual flags affected and A set to the result.
; @usage orAny B, C
;;;
orAny: macro
    ld A, \1
    or \2
endm

;;;
; Performs an AND on any two bytes. Usual flags affected and A set to the result.
; @usage andAny C, [HL]
;;;
andAny: macro
    ld A, \1
    and \2
endm

;;;
; Performs an AND on any two bytes. Usual flags affected and A set to the result.
; @usage xorAny D, $32 
;;;
xorAny: macro
    ld A, \1
    xor \2
endm
