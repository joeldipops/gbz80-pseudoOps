    IF !DEF(JUMPS_INCLUDED)
JUMPS_INCLUDED SET 1

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

;;;
; Jumps to address n16 if in previous cp, sub or sbc, A < x
; jrlt n16
; Cycles: 3 if jump occurs, 2 otherwise
; Bytes: 2
; Flags:
;;;
jrlt: macro
    jr C, \1
endm

;;;
; jplte n16
; Jumps to address n16 if in previous cp, sub or sbc, A <= x
; Cycles: 3 - 6 depending on result
; Bytes: 4
; Flags: None
;;;
jrlte: macro
    jr C, \1
    ; If \2 starts like a label, not a number
    IF (STRIN(LBL_CHARS, STRSUB("\1", 1, 1)) != 0)
        jr Z, \1
    ELSE
        jr Z, \1 - 2 ; -2 since jr is two bytes.
    ENDC
endm

;;;
; jrgt n16
; Jumps to address n16 if in previous cp, sub or sbc, A > x
; Cycles: 3 - 6 depending on result
; Bytes: 4
; Flags: None
;;;
jrgt: macro
    jr C, .endCompare\@
    ; If \2 starts like a label, not a number
    IF (STRIN(LBL_CHARS, STRSUB("\1", 1, 1)) != 0)
        jr NZ, \1
    ELSE
        jr NZ, \1 - 2 ; -2 since jr is two bytes.
    ENDC
.endCompare\@
endm

;;;
; jrgte n16
; Jumps to address n16 if in previous cp, sub or sbc, A >= x
; Cycles:  3 if jump occurs, 2 otherwise
; Bytes: 4
; Flags: None
;;;
jrgte: macro
    jr nc, \1
endm

;;;
; jreq n16
; Jumps to address n16 if in previous cp, sub or sbc, A == x
; Cycles: 3 if jump occurs, 2 otherwise
; Bytes: 4
; Flags: None
;;;
jreq: macro
    jr NZ, \1
endm

;;;
; jrne n16
; Jumps to address n16 if in previous cp, sub or sbc, A != x
; Cycles: 3 if jump occurs, 2 otherwise
; Bytes: 4
; Flags: None
;;;
jrne: macro
    jr NZ, \1
endm

;;;
; calllt n16
; if A < x, in previous cp, sub or sbc, 
; Pushes current address to the stack then jumps to n16 
; Cycles: 6 if jump occurs, 3 otherwise
; Bytes: 3
; Flags: None
;;;
calllt: macro
    call C, \1
endm

;;;
; calllte n16
; if A <= x, in previous cp, sub or sbc, 
; Pushes current address to the stack then jumps to n16 
; Cycles: 6 - 9 depending on condition
; Bytes: 6
; Flags: None
;;;
calllte: macro
    call C, \1
    call Z, \1
endm

;;;
; callgt n16
; if A > x, in previous cp, sub or sbc, 
; Pushes current address to the stack then jumps to n16 
; Cycles: 3 - 9 depending on condition
; Bytes: 5
; Flags: None
;;;
callgt: macro
    jr C, .endCompare@
    call NZ, \1
.endCompare\@
endm

;;;
; callgte n16
; if A >= x, in previous cp, sub or sbc, 
; Pushes current address to the stack then jumps to n16 
; Cycles: 6 if call occurs, 3 otherwise.
; Bytes: 3
; Flags: None
;;;
callgte: macro
    call NC, \1
endm

;;;
; calleq n16
; if A == x, in previous cp, sub or sbc, 
; Pushes current address to the stack then jumps to n16 
; Cycles: 6 if call occurs, 3 otherwise.
; Bytes: 3
; Flags: None
;;;
calleq: macro
    call Z, \1
endm

;;;
; calleq n16
; if A != x in previous cp, sub or sbc, 
; Pushes current address to the stack then jumps to n16 
; Cycles: 6 if call occurs, 3 otherwise.
; Bytes: 3
; Flags: None
;;;
callne: macro
    call NZ, \1
endm

;;;
; retlt
; if A < x in previous cp, sub or sbc, 
; pops the stack and returns to that address.
; Cycles: 5 if jump occurs, 2 otherwise
; Bytes: 1
; Flags: None
;;;
retlt: macro
    ret C
endm

;;;
; retlt
; if A <= x in previous cp, sub or sbc, 
; pops the stack and returns to that address.
; Cycles: 4 - 7 depending on condition.
; Bytes: 2
; Flags: None
;;;
retlte: macro
    ret C
    ret Z
endm

;;;
; retgt
; if A > x in previous cp, sub or sbc, 
; pops the stack and returns to that address.
; Cycles: 3 - 7 depending on condition.
; Bytes: 3
; Flags: None
;;;
retgt: macro
    jr C, .endCompare@
    ret NZ
.endCompare\@
endm

;;;
; retgte
; if A >= x in previous cp, sub or sbc, 
; pops the stack and returns to that address.
; Cycles: 5 if return occurs, 2 otherwise
; Bytes: 1
; Flags: None
;;;
retgte: macro
    ret NC
endm

;;;
; reteq
; if A == x in previous cp, sub or sbc, 
; pops the stack and returns to that address.
; Cycles: 5 if return occurs, 2 otherwise
; Bytes: 1
; Flags: None
;;;
reteq: macro
    ret Z
endm

;;;
; retne
; if A != x in previous cp, sub or sbc, 
; pops the stack and returns to that address.
; Cycles: 5 if return occurs, 2 otherwise
; Bytes: 1
; Flags: None
;;;
retne: macro
    ret NZ
endm
    ENDC