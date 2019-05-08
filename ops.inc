    IF !DEF(PSEUDO_OPS_INCLUDED)
PSEUDO_OPS_INCLUDED SET 1

R16 EQUS "\"BC DE HL\""
R8 EQUS "\"A B C D E H L\""

;;;
; Adds two values, Result in r8
; addAny r8, [r16]
;
; addAny r8, [n16]
;
;;;;
addAny: macro
IS_P2_N16\@ SET ((STRIN("\2", "[") == 1) && (STRIN("\2", "]") == STRLEN("\2")) && (STRIN(R16, "\2") != 0 || STRLEN("\2") != 4))
    IF IS_P2_N16\@
        ld A, \2
        add A, \1
    ELSE
        ld A, \1
        add A, \2
    ENDC
    ld \1, A 
endm

;;;
; Adds two values + 1 if carry flag set. Result in r8.
; adcAny r8, [r16]
;;;
adcAny: macro
    ld A, \1
    adc \2
    ld \1, A 
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
; pushAny r16
; Affects HL
;;;
pushAny: macro
    ld HL, \1
    push HL
endm

;;;
; Multiples A with another value, result in HL
; mult r8, ?r16
;
; mult n8, ?r16
;
; mult [r16], ?r16
;
; mult [n16], ?r16
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

    IF ("{VALUE\@}" == "[HL]") || ("{VALUE\@}" == "H") || ("{VALUE\@}" == "L")
        FAIL "multiplying by H or L is not yet implemented."
    ENDC

    ;IF ("{VALUE\@}" == "[HL]") || ("{VALUE\@}" == "H") || ("{VALUE\@}" == "L")    
    ;    push HL
    ;    ld H, 0
    ;    ld L, A
     ;   pop TEMP\@
    ;ELSE
        ld HIGH(TEMP\@), 0
        ld LOW(TEMP\@), A
        ld H, HIGH(TEMP\@)
        ld L, HIGH(TEMP\@)
    ;ENDC

    ; If either operand is 0, finish now.
    or A, A
        jr Z, .end\@

    ld A, VALUE\@
    or A, A
        jr Z, .end\@

.loop\@
        add HL, TEMP\@
        dec A
        jr NZ, .loop\@

.end\@
    IF HAS_SIDE_AFFECTS\@ == 1
        pop BC
    ENDC
endm

;;;
; Multiples two values, result in HL
; mult v8, v8, ?r16
;;;
multAny: macro
    ld A, \1
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
; ldAny [r16], 0
; Cycles:
; Bytes:
; Flags: None
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
;;;
ldAny: macro
    IF "\2"  == "0"
        xor A, A
    ELSE
        ld A, \2
    ENDC
    ld \1 , A
endm

;;;
; Loads to an address in IO space
;
; ldhAny [$ff00 + n8], n8
; Cycles: 5
; Bytes: 4 
; Flags: None 
;
; ldhAny [$ff00 + n8], r8
; Cycles: 4
; Bytes: 3 
; Flags: None
;
; ldhAny [$ff00 + n8], [r16]
; Cycles: 5
; Bytes: 3 
; Flags: None 
;
; ldhAny [$ff00 + n8], [n16]
; Cycles: 7
; Bytes: 5 
; Flags: None 
;;;
ldhAny: macro
    ld A, \2
    ldh \1, A
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
    IF "\1" == "[HL]"
        ld A, \2
        ldi [HL], A
    ELIF "\2" == "[HL]"
        ldi A, [HL]
        ld \1, A
    ELSE
        FAIL "ldi requires [HL]"
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
    ld A, \1
    IF "\1" == "\2"
        ; Idiot proofing phase 1
        or A, A
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
    ld A, \1
    and A, \2
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
    ld A, \1
    xor A, \2
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
    ld A, \2
    res \1, A
    ld \2, A
endm

;;;
; Resets a bit of an 8bit piece of IO space memory
;
; resH u3, [$ff00 + n8]
; Cycles: 8 
; Bytes: 6
; Flags: None
;;;
resH: macro
    ldh A, \2
    res \1, A
    ldh \2, A
endm

;;;
; Compares a value with a value in the IO space of memory and sets flags.
;
; cpH [$ff00 + n8], n8
; Cycles: 5
; Bytes: 4
; Flags: Z=? N=1, H=? C=?
;
; cpH [$ff00 + n8], r8 
; Cycles: 4
; Bytes: 3
; Flags: Z=? N=1, H=? C=?
;
; cpH [$ff00 + n8], [HL] 
; Cycles: 5
; Bytes: 3
; Flags: Z=? N=1, H=? C=?
;;;
cpH: macro
    ldh A, \1
    cp \2
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
    ld A, \1
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
    ld A, \1
    inc A
    ld \1, A
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
    ld A, \1
    dec A
    ld \1, A
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

    ; Remove the square brackets around \2 so we can find the following address ie. [\2 + 1]
    IF ((STRIN("\2", "[") == 1) && (STRIN("\2", "]") == STRLEN("\2")))
P2\@ EQUS STRSUB("\2", 2, STRLEN("\2") - 2)
    ELSE
P2\@ EQUS "\2"
    ENDC    

    IF IS_P1_R16\@
        IF IS_P2_R16\@
            ; If both operands are registers
            ld LOW(\1), LOW(\2)
            ld HIGH(\1), HIGH(\2)
        ELSE
            ; If only first operand is a register
            ldAny HIGH(\1), [P2\@]
            ldAny LOW(\1), [P2\@ + 1]
        ENDC
    ELIF IS_P2_R16\@
        ; Remove the square brackets around \1 so we can find the following address ie. [\1 + 1]
        IF STRIN("\1", "[") == 1 && STRIN("\1", "]") == STRLEN("\1")
P1\@ EQUS STRSUB("\1", 2, STRLEN("\1") - 2)
        ELSE
P1\@ EQUS "\1"
        ENDC

        ; If only second operand is a register
        ldAny [P1\@], HIGH(\2)
        ldAny [P1\@ + 1], LOW(\2)
    ELSE
        ; If both are addresses
        ldAny [P1\@], [P2\@]
        ldAny [P1\@ + 1], [P2\@ + 1]
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
    ld A, LOW(\1)
    sub A, LOW(\2)
    ld LOW(\1), A

    ld A, HIGH(\1)
    sbc HIGH(\2)
    ld HIGH(\1), A
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
; Jumps to address n16 if in previous cp, sub or sbc, A < x
; jplt n16
; Cycles: 4 if jump occurs, 3 otherwise
; Bytes: 3
; Flags:
;;;
jplt: macro
    jp C, \1
endm

;;;
; Jumps to address n16 if in previous cp, sub or sbc, A <= x
; jplte n16
; Cycles: 4 - 7 depending on result
; Bytes: 6
; Flags: None
;;;
jplte: macro
    jp C, \1
    jp Z, \1
endm

;;;
; Jumps to address n16 if in previous cp, sub or sbc, A > x
; jpgt n16
; Cycles: 3 - 6 depending on result.
; Bytes: 5
; Flags: None
;;;
jpgt: macro
    jr C, .end\@
    jp NZ, \1
.end\@
endm

;;;
; Jumps to address n16 if in previous cp, sub or sbc, A >= x
; jpgte n16
; Cycles: 4 if jump occurs, 3 otherwise
; Bytes: 3
; Flags: None
;;;
jpgte: macro
    jp NC, \1
endm

;;;
; Jumps to address n16 if in previous cp, sub or sbc, A = x
; jpeq n16
; Cycles: 4 if jump occurs, 3 otherwise
; Bytes: 3
; Flags: None
;;;
jpeq: macro
    jp Z, \1
endm

;;;
; Jumps to address n16 if in previous cp, sub or sbc, A != x
; jpne n16
; Cycles: 4 if jump occurs, 3 otherwise
; Bytes: 3
; Flags: None
;;;
jpne: macro
    jp NZ, \1
endm
    ENDC
