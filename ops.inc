    IF !DEF(PSEUDO_OPS_INCLUDED)
PSEUDO_OPS_INCLUDED SET 1

R16 EQUS "\"BC DE HL\""

;;;
; Adds two values, Result in r8
; addAny r8, [r16]
;;;;
addAny: macro
    ld A, \1
    add \2
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
; mult r8, ?r16
; Multiples two numbers, result in HL
;;;
mult: macro
HAS_SIDE_AFFECTS SET 0
VALUE EQUS "\1"
    ; If a second param is supplied, use that as our temp register and don't both pushpopping
    IF _NARG == 2
        SHIFT
TEMP EQUS "\1"
        ; I should be able to use || here, but my second condition was never true, despite working as expected im the ELIF
        IF STRLEN("{TEMP}") == 2 && STRIN(R16, "{TEMP}") == 0
            FAIL "r16 must be either BC or DE"
        ELIF "{TEMP}" == "HL"
            FAIL "r16 must be either BC or DE"
        ENDC
    ELSE
TEMP EQUS "BC"
HAS_SIDE_AFFECTS SET 1        
        push BC
    ENDC

    ld HIGH(TEMP), 0
    ld LOW(TEMP), A
    ld H, HIGH(TEMP)
    ld L, HIGH(TEMP)

    ; If either operand is 0, finish now.
    or A
        jr Z, .end\@

    ld A, VALUE
    or A
        jr Z, .end\@

.loop\@
        add HL, TEMP
        dec A
        jr NZ, .loop\@

.end\@

    IF HAS_SIDE_AFFECTS == 1
        pop BC
    ENDC
    PURGE VALUE
    PURGE TEMP
endm

;;;
; Loads byte from anywhere to anywhere else that takes a byte.
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
    ld A, \2
    ld \1, A
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
    or \2
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
    and \2
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
    xor \2
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
;;;
ld16: macro
    IF (STRLEN("\1") == 2 && STRIN(R16, "\1") != 0) && (STRLEN("\2") == 2 && STRIN(R16, "\2") != 0)
        ; If both operands are registers
        ld LOW(\1), LOW(\2)
        ld HIGH(\1), HIGH(\2)
    ELIF STRLEN("\1") == 2 && STRIN(R16, "\1") != 0
        ; If first operand is a register
        ldAny HIGH(\1), [\2]
        ldAny LOW(\1), [\2 + 1]
    ELIF STRLEN("\2") == 2 && STRIN(R16, "\2") != 0        
        ; If second operand is a register
        ldAny [\1], HIGH(\2)
        ldAny [\1 + 1], LOW(\2)
    ELSE
        ; If both are addresses
        ldAny [\1], [\2]
        ldAny [\1 + 1], [\2 + 1]
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
    sub LOW(\2)
    ld LOW(\1), A

    ld A, HIGH(\1)
    sbc HIGH(\2)
    ld HIGH(\1), A
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
    add LOW(\2)
    ld LOW(\1), A 

    ld A, HIGH(\1)
    adc HIGH(\2)
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
