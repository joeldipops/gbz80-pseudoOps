    IF !DEF(Z80_INCLUDED)
Z80_INCLUDED SET 1
    
; resources
; http://www.z80.info/z80-op.txt
; http://z80-heaven.wikidot.com/instructions-set:cpi

;;;
; sll r8
; shifts register to the left, and sets bit 0
;;;
sll: macro
    sla \1
    set 0, \1
endm

;;;
; cpd
;
; affects hl, bc
;;;
cpd: macro
    ; not quite right, 
    ;   N flag should be set, 
    ;   H flag should depend on operands of cp    
    ; but with this, both are 0
    jr NC, .noC\@
        cp A, [HL]
        scf
        jr .cEnd\@
.noC\@
        cp A, [HL]
        scf 
        ccf
.cEnd\@
    dec HL
    dec BC
endm

cpi: macro
    ; This theoretically works but is a huge waste of time since
    ; those flags are irrelevant more often than not.
    jr NC, .noC\@
        cp A, [HL]
        push AF
        inc SP
        pop AF
        set 7, A
        jr .cEnd\@
.noC\@
        cp A, [HL]
        push AF
        inc SP
        pop AF
        res 7, A
.cEnd\@        
    push AF
    dec SP
    pop AF

    inc HL
    dec BC
endm

cpdr: macro
.loop\@
    cpd
    jr NZ, .loop\@
endm

cpir: macro
.loop\@
    cpi
    jr NZ, .loop\@
endm


;;;
; djnz e8
;
; affects B
;;;;
djnz: macro
    dec B
    jr NZ, \1
endm

;;;
; ex r16, r16
;
;;;
ex: macro
    ; Both are registers
    IF STRIN(R16, "\1") != 0
        push \1
    ELSE
        FAIL "ex with non r16 not yet implemented"
    ENDC

    ldAny \1, \2

    IF STRIN(R16, "\2") != 0
        pop \2
    ELSE
        FAIL "ex with non r16 not yet implemented"
    ENDC
endm



    ENDC