    IF !DEF(Z80_INCLUDED)
Z80_INCLUDED SET 1
    
; resources
; http://www.z80.info/z80-op.txt
; http://z80-heaven.wikidot.com/instructions-set:cpi

;;;
; Helper for shared cpi and cpd behaviour.
; @param ?n1 1 to correctly emulate N and H flags, 0 reset them 
; @private
;
; There may be a faster way to acheive this using SP loads and manipulations.
;;;
_cpid: macro
    IF (_NARG == 1) && ("\1" != "0")
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
    ELSE
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
    ENDC
endm

;;;
; cpd
;
; affects hl, bc
;;;
cpd: macro
    IF _NARG == 1
        _cpid \1
    ELSE
        _cpid
    ENDC

    dec HL
    dec BC
endm

;;;
; cpi ?n1
;
; @param ?n1 1 to correctly emulate N and H flags
;;;
cpi: macro
    IF _NARG == 1
        _cpid \1
    ELSE
        _cpid
    ENDC

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
; sll r8
; shifts register to the left, and sets bit 0
;;;
sll: macro
    sla \1
    set 0, \1
endm

;;;
; neg
;
;
;;;
neg: macro
    dec A 
    cpl
endm

ldir: macro
.loop\@
        ldiAny [DE], [HL]
        inc DE
        dec BC
    orAny B, C
    jr NZ, .loop\@
endm

lddr: macro
.loop\@
        lddAny [DE], [HL]
        dec DE
        dec BC
    orAny B, C
    jr NZ, .loop\@
endm

rld: macro
    ;; TODO
    sla [HL]
    rlca

    bit 5, A
    jr NZ, .noZ
        set 1, [HL]
.noZ


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