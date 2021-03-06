    IF !DEF(PSEUDO_OPS_INCLUDED)
PSEUDO_OPS_INCLUDED SET 1

INCLUDE "config.inc"

N8 EQUS "\"$ % 0 1 2 3 4 5 6 7 8 9\""
R16 EQUS "\"BC DE HL bc de hl\""
R8 EQUS "\"A B C D E H L a b c d e h l\""
LBL_CHARS EQUS "\"_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\""

;;;
; Shows an assembly warning message unless they have been surpressed in the config.
; @param \1 The text of the warning.
;;;
_warn: macro
    IF WARNINGS_ENABLED == 1
        WARN \1
    ENDC
endm

;;;
; Adds two values, Result in A
; addAny r8, [r16]
;
; addAny r8, [n16]
;
;;;;
addAny: macro
; If the second param is an address, we need to load it first, since add can't take [n16]
IS_P2_N16\@ SET ((STRIN("\2", "[") == 1) && (STRIN("\2", "]") == STRLEN("\2")) && (STRIN(R16, "\2") != 0 || STRLEN("\2") != 4))
    IF IS_P2_N16\@
        ldAny A, \2
        add A, \1
    ELSE
        ldAny A, \1
        add A, \2
    ENDC
    ldAny \1, A
endm

;;;
; Adds two values + 1 if carry flag set. Result in A.
; adcAny r8, [r16]
;;;
adcAny: macro
    ldAny A, \1
    adc A, \2
    ldAny \1, A 
endm

;;;
; Inserts a null terminated string into ROM
; dbs string, ...
; Bytes: Length of String + 1
; Cycles: N/A
; Flags: N/A
;;;
dbs: macro
    REPT _NARG
        db \1
        SHIFT
    ENDR
    db 0
endm

;;;
; Pushes an immediate value on to the stack.
; pushAny [n16]
;
; Affects HL
;;;
pushAny: macro
    ld16 HL, \1
    push HL
endm

;;;
; Multiplies two values, result in HL
; mult v8, v8, ?r16
;;;
multAny: macro
    ldAny A, \1
    IF _NARG == 3
        SHIFT
        mult \1, \2
    ELSE
        mult \2
    ENDC
endm

;;;
; Loads byte from anywhere to anywhere else that takes a byte.
;
; ldAny [n16], 0
; Cycles: 5
; Bytes: 4
; Flags: Z=1 N=0 H=0 C=0
;
; ldAny [r16], 0
; Cycles: 3
; Bytes: 2
; Flags: Z=1 N=0 H=0 C=0
;
; ldAny r8, [n16]
; Cycles: 5 
; Bytes: 4 
; Flags: None
;
; ldAny r8, [r16]
; Cycles: 3
; Bytes: 2 
; Flags: None
;
; ldAny [n16], r8
; Cycles: 5
; Bytes: 4
; Flags: None
;
; ldAny [r16], r8
; Cycles: 3
; Bytes: 2
; Flags: None
;
; ldAny [r16], [r16]
; Cycles: 4
; Bytes: 2
; Flags: None
;
; ldAny [r16], [n16]
; Cycles: 6
; Bytes: 4
; Flags: None
;
; ldAny [n16], [r16]
; Cycles: 6
; Bytes: 4
; Flags: None
;
; ldAny [n16], [n16]
; Cycles: 8
; Bytes: 6
; Flags: None
;
; ldAny [n16], n8
; Cycles: 
; Bytes:
; Flags: None
;
; ldAny r16, SP
; Cycles:
; Bytes:
; Flags: 
; Affects: HL
;
; ldAny SP, r16
; Cycles:
; Bytes:
; Flags: 
; Affects: HL
;;;
ldAny: macro
    ;FAIL "\1 \2"
    ; Are 16 bits involved?
    IF ((STRIN(STRCAT(R16,"SP"), "\1") != 0) && (STRLEN("\1") == 2)) \
    || ((STRIN(STRCAT(R16,"SP"), "\2") != 0) && (STRLEN("\2") == 2)) \
    || (STRIN("\2", "SP ") == 1) \
    || (STRIN("\2", "SP+") == 1)
        ld16 \1, \2
    ELSE
        ; Force an ldh?
        IF (STRSUB("\1", 1, STRLEN(LDH_TOKEN)) == LDH_TOKEN) 
IS_P1_HRAM\@ SET 1
; strip off the #
P1\@ EQUS STRSUB("\1", STRLEN(LDH_TOKEN) + 1, STRLEN("\1") - STRLEN(LDH_TOKEN))
        ELSE
P1\@ EQUS "\1"
IS_P1_HRAM\@ SET 0
        ENDC

        ; Force an ldh?
        IF (STRSUB("\2", 1, STRLEN(LDH_TOKEN)) == LDH_TOKEN) 
IS_P2_HRAM\@ SET 1
; strip off the #
P2\@ EQUS STRSUB("\2", 2, STRLEN("\2") - 1)
        ELSE
P2\@ EQUS "\2"
IS_P2_HRAM\@ SET 0
        ENDC

        ; If loading to or from A, we only need a single instruction.
        IF STRUPR("\1") == "A"
            IF IS_P2_HRAM\@ == 1
                ldh A, P2\@
            ELIF "\2" == "0"
                xor A
            ELSE
                ld A, \2
            ENDC
        ELIF STRUPR("\2") == "A"
            IF IS_P1_HRAM\@ == 1
                ldh P1\@, A
            ELSE
                ld \1, A
            ENDC
        ELSE
            ; We only need a single instruction when loading 
            ; * r8, r8/n8/[HL]
            ; * [HL], r8/n8
            ; * (so anything without [] unless it's [HL])
            IF ((STRIN("{P1\@}", "[") == 1) && (STRIN("\1", "]") == STRLEN("\1")) &&  ("{P1\@}" != "[HL]"))
IS_P1_DIRECT\@ SET 0
            ELSE
IS_P1_DIRECT\@ SET 1
            ENDC

            IF ((STRIN("{P2\@}", "[") == 1) && (STRIN("\2", "]") == STRLEN("\2")) &&  ("{P2\@}" != "[HL]"))
IS_P2_DIRECT\@ SET 0
            ELSE
IS_P2_DIRECT\@ SET 1
            ENDC

            IF ((IS_P1_DIRECT\@ == 1) && (IS_P2_DIRECT\@ == 1))
                IF ("{P1\@}" == "[HL]") && ("{P2\@}" == "[HL]")
                    ld A, [HL]
                    ld [HL], A
                ELSE
                    ld P1\@, P2\@
                ENDC
            ELSE
                ; otherwise, we need to load into A first.

                IF "\2" == "0"
                    ; xor A is cheaper than a ld A, 0
                    xor A
                ELIF IS_P2_HRAM\@ == 1
                    ldh A, P2\@
                ELSE
                    ld A, \2
                ENDC

                IF IS_P1_HRAM\@ == 1
                    ldh P1\@, A
                ELSE
                    ld \1, A
                ENDC
            ENDC
        ENDC
    ENDC
endm

;;;
; Loads from [HL] then increments HL
; ldiAny r8, [HL]
; Cycles: 3
; Bytes: 2
; Flags: None
;
; ldiAny [r16], [HL]
; Cycles: 4
; Bytes: 2
; Flags: None
;
; ldiAny [n16], [HL]
; Cycles: 6
; Bytes: 4
; Flags: None
;
; ldiAny [HL], n8
; Cycles: 6
; Bytes: 4
; Flags: None
;
; ldiAny [HL], r8
; Cycles: 3
; Bytes: 2
; Flags: None
;
; ldiAny [HL], [r16]
; Cycles: 6
; Bytes: 4
; Flags: None
;
; ldiAny [HL], [n16]
; Cycles: 6
; Bytes: 4
; Flags: None
;;;
ldiAny: macro
    IF ("\1" == "[HL]") && ("\2" == "[HL]")
        FAIL "ambiguous operation"
    ELIF "\1" == "[HL]"
        ldAny A, \2
        ldi [HL], A
    ELIF "\2" == "[HL]"
        ldi A, [HL]
        ldAny \1, A
    ELSE
        FAIL "ldi requires [HL]"
    ENDC
endm

lddAny: macro
    IF ("\1" == "[HL]") && ("\2" == "[HL]")
        FAIL "ambiguous operation"
    ELIF "\1" == "[HL]"
        ldAny A, \2
        ldd [HL], A
    ELIF "\2" == "[HL]"
        ldd A, [HL]
        ldAny \1, A
    ELSE
        FAIL "ldd requires [HL]"
    ENDC
endm

;;;
; ORs the bits of two registers, result in A
;
; orAny r8, r8
; Cycles: 2
; Bytes: 2
; Flags: Z=? N=0 H=0 C=0
; 
; orAny r8, n8
; Cycles: 4
; Bytes: 4 
; Flags: Z=? N=0 H=0 C=0
;
; orAny r8, [HL]
; Cycles: 3
; Bytes: 2
; Flags: Z=? N=0 H=0 C=0
;
; orAny [r16], r8
; Cycles: 3
; Bytes: 2
; Flags: Z=? N=0 H=0 C=0
; 
; orAny [r16], n8
; Cycles: 4
; Bytes: 3 
; Flags: Z=? N=0 H=0 C=0
;
; orAny [r16], [HL]
; Cycles: 4
; Bytes: 2
; Flags: Z=? N=0 H=0 C=0
;
; orAny [n16], r8
; Cycles: 5
; Bytes: 4
; Flags: Z=? N=0 H=0 C=0
; 
; orAny [n16], n8
; Cycles: 6
; Bytes: 5 
; Flags: Z=? N=0 H=0 C=0
;
; orAny [n16], [HL]
; Cycles: 6
; Bytes: 4
; Flags: Z=? N=0 H=0 C=0
;;;
orAny: macro
    IF "\1" != "A"
        ldAny A, \1
    ENDC
    IF "\1" == "\2"
        or A, A
        _warn "1: Avoid orAny if you intended to read from \1 more than once."
    ELSE
        or A, \2
    ENDC
endm

;;;
; ANDs the bit of two registers, result in A
;
; andAny r8, r8
; Cycles: 2
; Bytes: 2
; Flags: Z=? N=0 H=1 C=0
; 
; andAny r8, n8
; Cycles: 4
; Bytes: 4 
; Flags: Z=? N=0 H=1 C=0
;
; andAny r8, [HL]
; Cycles: 3
; Bytes: 2
; Flags: Z=? N=0 H=1 C=0
;
; andAny [r16], r8
; Cycles: 3
; Bytes: 2
; Flags: Z=? N=0 H=1 C=0
; 
; andAny [r16], n8
; Cycles: 4
; Bytes: 3 
; Flags: Z=? N=0 H=1 C=0
;
; andAny [r16], [HL]
; Cycles: 4
; Bytes: 2
; Flags: Z=? N=0 H=1 C=0
;
; andAny [n16], r8
; Cycles: 5
; Bytes: 4
; Flags: Z=? N=0 H=1 C=0
; 
; andAny [n16], n8
; Cycles: 6
; Bytes: 5 
; Flags: Z=? N=0 H=1 C=0
;
; andAny [n16], [HL]
; Cycles: 6
; Bytes: 4
; Flags: Z=? N=0 H=1 C=0
;;;
andAny: macro
    IF "\1" != "A"
        ldAny A, \1
    ENDC
    IF "\1" == "\2"
        and A, A
        _warn "1: Avoid andAny if you intended to read from \1 more than once."
    ELSE
        and A, \2
    ENDC
endm

;;;
; Performs an XOR on any two 8bit registers. Usual flags affected and A set to the result.
;
; xorAny r8, r8
; Cycles: 2
; Bytes: 2
; Flags: Z=? N=0 H=0 C=0
; 
; xorAny r8, n8
; Cycles: 4
; Bytes: 4 
; Flags: Z=? N=0 H=0 C=0
;
; xorAny r8, [HL]
; Cycles: 3
; Bytes: 2
; Flags: Z=? N=0 H=0 C=0
;
; xorAny [r16], r8
; Cycles: 3
; Bytes: 2
; Flags: Z=? N=0 H=0 C=0
; 
; xorAny [r16], n8
; Cycles: 4
; Bytes: 3 
; Flags: Z=? N=0 H=0 C=0
;
; xorAny [r16], [HL]
; Cycles: 4
; Bytes: 2
; Flags: Z=? N=0 H=0 C=0
;
; xorAny [n16], r8
; Cycles: 5
; Bytes: 4
; Flags: Z=? N=0 H=0 C=0
;
; xorAny [n16], n8
; Cycles: 6
; Bytes: 5 
; Flags: Z=? N=0 H=0 C=0
;
; xorAny [n16], [HL]
; Cycles: 6
; Bytes: 4
; Flags: Z=? N=0 H=0 C=0
;;;
xorAny: macro
    IF "\1" != "A"
        ldAny A, \1
    ENDC
    IF "\1" == "\2"
        xor A, A
        _warn "1: Avoid xorAny if you intended to read from \1 more than once."
    ELSE
        xor A, \2
    ENDC
endm

;;;
; Resets a bit of an 8bit piece of memory
;
; resAny u3, [n16]
; Cycles: 10 
; Bytes: 8
; Flags: None
;
; resAny u3, [r16]
; Cycles: 6
; Bytes: 4
; Flags: None
;;;
resAny: macro
    ldAny A, \2
    res \1, A
    ldAny \2, A
endm

;;;
; Compares two values, setting appropriate flags.
;
; cpAny r8, n8
; Cycles: 4
; Bytes: 4
; Flags: Z=? N=1, H=? C=?
;
; cpAny r8, r8
; Cycles: 3
; Bytes: 3
; Flags: Z=? N=1, H=? C=?
;
; cpAny r8, [HL]
; Cycles: 4
; Bytes: 3
; Flags: Z=? N=1, H=? C=?
;
; cpAny [r16], n8
; Cycles: 4
; Bytes: 3
; Flags: Z=? N=1, H=? C=?
;
; cpAny [r16], r8
; Cycles: 3
; Bytes: 2
; Flags: Z=? N=1, H=? C=?
;
; cpAny [r16], [HL]
; Cycles: 4
; Bytes: 2
; Flags: Z=? N=1, H=? C=?
;
; cpAny [n16], n8
; Cycles: 6
; Bytes: 5
; Flags: Z=? N=1, H=? C=?
;
; cpAny [n16], r8
; Cycles: 5
; Bytes: 4
; Flags: Z=? N=1, H=? C=?
;
; cpAny [n16], [HL]
; Cycles: 6
; Bytes: 4
; Flags: Z=? N=1, H=? C=?
;;;
cpAny: macro
    ldAny A, \1
    cp \2
endm

;;;
; Increment an 8bit value
;
; incAny [n16]
; Cycles: 9
; Bytes: 7
; Flags: Z=? N=0 H=? C=C
;
; incAny [r16]
; Cycles: 5
; Bytes: 3
; Flags: Z=? N=0 H=? C=C
;;;
incAny: macro
    ldAny A, \1
    inc A
    ldAny \1, A
endm

;;;
; Decrement an 8bit value
;
; decAny [n16]
; Cycles: 9
; Bytes: 7
; Flags: Z=? N=1 H=? C=C
;
; decAny [r16]
; Cycles: 5
; Bytes: 3
; Flags: Z=? N=1 H=? C=C
;;;
decAny: macro
    ldAny A, \1
    dec A
    ldAny \1, A
endm

;;;
; Loads a 16 bit register
;
; ld16 r16, r16
; Cycles: 2
; Bytes: 2
; Flags: None
;
; ld16 r16, [n16]
; 
;;;
ld16: macro
    IF (("\2" == "[HL]") || ("\2" == "[BC]")  || ("\2" == "[DE]"))
        FAIL "register indirection not yet implemented"
    ENDC

IS_P1_R16\@ SET (STRLEN("\1") == 2 && STRIN(R16, "\1") != 0)
IS_P2_R16\@ SET (STRLEN("\2") == 2 && STRIN(R16, "\2") != 0)

    ; Remove the ldh token and square brackets so we can find the following address ie. [\2 + 1]
    IF (STRSUB("\2", 1, STRLEN(LDH_TOKEN)) == LDH_TOKEN)
IS_P2_HRAM\@ SET 1
P2PREFIX\@ EQUS LDH_TOKEN
        ; Remove #[ and ]
        IF ((STRIN("\2", "[") == 2) && (STRIN("\2", "]") == STRLEN("\2")))
P2\@ EQUS STRSUB("\2", STRLEN(LDH_TOKEN) + 2, STRLEN("\2") - (STRLEN(LDH_TOKEN) + 2))
        ELSE
P2\@ EQUS "\2"
        ENDC    
    ELSE
P2PREFIX\@ EQUS ""
        ; Remove [ and ]
        IF ((STRIN("\2", "[") == 1) && (STRIN("\2", "]") == STRLEN("\2")))
P2\@ EQUS STRSUB("\2", 2, STRLEN("\2") - 2)
        ELSE
P2\@ EQUS "\2"
        ENDC    
    ENDC

    ; Remove the ldh token and square brackets so we can find the following address ie. [\2 + 1]
    IF (STRSUB("\1", 1, STRLEN(LDH_TOKEN)) == LDH_TOKEN)
IS_P1_HRAM\@ SET 1
P1PREFIX\@ EQUS LDH_TOKEN
        ; Remove #[ and ]
        IF ((STRIN("\1", "[") == 2) && (STRIN("\1", "]") == STRLEN("\1")))
P1\@ EQUS STRSUB("\1", STRLEN(LDH_TOKEN) + 2, STRLEN("\1") - (STRLEN(LDH_TOKEN) + 2))
        ELSE
P1\@ EQUS "\1"
        ENDC    
    ELSE
P1PREFIX\@ EQUS ""    
        ; Remove [ and ]
        IF ((STRIN("\1", "[") == 1) && (STRIN("\1", "]") == STRLEN("\1")))
P1\@ EQUS STRSUB("\1", 2, STRLEN("\1") - 2)
        ELSE
P1\@ EQUS "\1"
        ENDC    
    ENDC

    ; ops that support SP are a bit of a mixed bag, so will need special cases for them.
    IF ("\1" == "SP")
        ; Natively supported SP ops are `ld SP, HL` and `ld SP, n16
        IF ("\2" == "HL") || ((IS_P2_R16\@ == 0) && (STRIN("\2", "[") == 0))
            ld \1, \2
        ELSE
            ; HL will be affected
            ldAny HL, \2
            ld SP, HL
        ENDC
    ELIF (STRIN("\2", "SP ") != 0) || (STRIN("\2", "SP+") != 0)
        ; normal 'LDHL'
        ld HL, \2        
        IF ("\1" != "HL")
            ldAny \1, HL
        ENDC
    ELIF ("\2" == "SP")
        IF (STRIN("\1", "[") != 0)
            ld \1, SP
        ELSE
            ; assembler won't assemble without the +0
            ld HL, SP+0
            IF ("\1" != "HL")
                ldAny \1, HL
            ENDC
        ENDC
    ELIF IS_P1_R16\@
        IF IS_P2_R16\@
            ; If both operands are registers
            ld LOW(\1), LOW(\2)
            ld HIGH(\1), HIGH(\2)
        ELSE
            ; If only first operand is a register
            ldAny HIGH(\1), {P2PREFIX\@}[P2\@]
            ldAny LOW(\1), {P2PREFIX\@}[P2\@ + 1]
        ENDC
    ELIF IS_P2_R16\@
        ; If only second operand is a register
        ldAny {P1PREFIX\@}[P1\@], HIGH(\2)
        ldAny {P1PREFIX\@}[P1\@ + 1], LOW(\2)
    ELSE
        ; If both are addresses
        ldAny {P1PREFIX\@}[P1\@], {P2PREFIX\@}[P2\@]
        ldAny {P1PREFIX\@}[P1\@ + 1], {P2PREFIX\@}[P2\@ + 1]
    ENDC
endm

;;;
; Subtracts one 16 bit register from another with result in \1
; 
; sub16 r16, r16
; Cycles: 6
; Bytes: 6
; Flags: Z=? N=1 H=? C=?
;;;
sub16: macro
    ldAny A, LOW(\1)
    sub A, LOW(\2)
    ldAny LOW(\1), A

    ldAny A, HIGH(\1)
    sbc HIGH(\2)
    ldAny HIGH(\1), A
endm

;;;
; Shifts a 16 bit register to the left
;
; sl16 r16
;;;
sl16: macro
    IF "\1" == "HL"
        add HL, HL
    ELSE
        FAIL "sl16 BC and sl16 DE not yet implemented"
    ENDC
endm

;;;
; and16 r16 r16
; result in HL
;;;
and16: macro
IS_P1_R16\@ SET (STRIN(R16, "\1") != 0) && (STRLEN("\1") == 2)
IS_P2_R16\@ SET (STRIN(R16, "\2") != 0) && (STRLEN("\2") == 2)

    IF IS_P1_R16\@ 
        IF IS_P2_R16\@
            andAny LOW(\1), LOW(\2)
            ld L, A
            andAny HIGH(\1), HIGH(\2)
            ld H, A
        ELSE
            FAIL "Not yet implemented"
        ENDC
    ELSE
        FAIL "Not yet implemented"
    ENDC
endm

;;;
; and16 r16 r16
; result in HL
;;;
or16: macro
IS_P1_R16\@ SET (STRIN(R16, "\1") != 0) && (STRLEN("\1") == 2)
IS_P2_R16\@ SET (STRIN(R16, "\2") != 0) && (STRLEN("\2") == 2)

    IF IS_P1_R16\@ 
        IF IS_P2_R16\@
            orAny LOW(\1), LOW(\2)
            ld L, A
            orAny HIGH(\1), HIGH(\2)
            ld H, A
        ELSE
            FAIL "Not yet implemented"
        ENDC
    ELSE
        FAIL "Not yet implemented"
    ENDC
endm



;;;
; Shifts a 16bit register to the right.
;
; sr16 r16
;;;
sr16: macro
    IF (STRLEN("\1")) != 2 || (STRIN(R16, "\1") == 0)
        FAIL "Must be used with a 16bit register"
    ENDC

    sra HIGH(\1)
    rr LOW(\1)
endm

;;;
; Rotates a 16 bit register to the right.
;
; [0] -> [7 ->> 0] -> [7], C
; rrc16 r16
;;;
rrc16: macro
    IF (STRLEN("\1")) != 2 || (STRIN(R16, "\1") == 0)
        FAIL "Must be used with a 16bit register"
    ENDC

    rr LOW(\1)
    rr HIGH(\1)
    rl LOW(\1)
    rrc LOW(\1)
endm



;;;
; Adds two 16bit registers together. Result in the first.
;
; add16 r16, r16
; Cycles: 6
; Bytes: 6
; Flags: Z=? N=0 H=? C=?
;;;
add16: macro
    ld A, LOW(\1)
    add A, LOW(\2)
    ld LOW(\1), A 

    ld A, HIGH(\1)
    adc A, HIGH(\2)
    ld HIGH(\1), A
endm

;;;
; mult r8, ?r16
;
; mult n8, ?r16
;
; mult [n16], ?r16
;
; mult [r16], ?r16
;
; Multiplies A with r8, result in HL
; r16 is optional register to trash (BC or DE)
;;;
mult: macro
HAS_SIDE_AFFECTS\@ SET 0
VALUE\@ EQUS "\1"
    ; If a second param is supplied, use that as our temp register and don't both pushpopping
    IF _NARG == 2
        SHIFT
TEMP\@ EQUS "\1"
        IF !("{TEMP\@}" == "BC") || ("{TEMP\@}" == "DE")
            FAIL "r16 must be either BC or DE"
        ENDC
    ELSE
TEMP\@ EQUS "BC"
HAS_SIDE_AFFECTS\@ SET 1        
        push BC
    ENDC

    ; Skip this step if we're already using that register.
    IF (STRSUB("{TEMP\@}", 2, 1) != "{VALUE\@}")
        ldAny LOW(TEMP\@), {VALUE\@}
    ENDC    

    ld H, A
    ld L, 0
    ld HIGH(TEMP\@), L

    ld A, 7

    ; Optimized first iteration
    sla H
    jr NC, .loop\@
    ld L, LOW({TEMP\@})
.loop\@
        add HL, HL
        jr NC, .noAdd\@
        add HL, TEMP\@
.noAdd\@
        dec A
    jr NZ, .loop\@

    IF HAS_SIDE_AFFECTS\@ == 1
        pop BC
    ENDC 
endm

;;;
; div A, r8
;
; Divides A by r8, quotient in HL, remainder in A.
; If r8 is not one of B,C,D or E, C will be trashed
;;;
divide: macro
    IF STRIN("D E B C", "\1") != 0
P1\@ EQUS "\1"
    ELSE
        ldAny C, \1
P1\@ EQUS "C"
    ENDC
    ld H, 8
    ld L, A

    xor A
    IF "{ERROR_HANDLER}" != ""
        FAIL "blah"
        cp P1\@
        jr NZ, .noError\@
        ERROR_HANDLER
.noError\@
    ENDC    
.loop\@
        sla L
        rla
        cp P1\@
        jr C, .carry\@
            inc L
            sub P1\@
.carry\@
        dec H
        jr NZ, .loop\@
endm

INCLUDE "jumps.inc"
    ENDC