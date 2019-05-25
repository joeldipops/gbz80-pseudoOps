    IF !DEF(MAIN)
MAIN SET 1

INCLUDE "gbz80-pseudoOps.inc"
SECTION "mult", ROM0[$0000]
    placeMultiplyRoutine BC
SECTION "Header", ROM0[$0100]
    nop 
    jr main
SECTION "Home", ROM0[$0150]
main:
    nop

jrAliasTest:
    jrlte @+12
    jrlte @+12
    jrgt @+12
    jrgte @+12


_iamalabel:
    nop

multTest:
    ld A, 7
    ld HL, $c000
    ldAny [HL], 7
    mult [HL]
    jr main

ldilddTest:
    ldiAny [HL], A
    lddAny [HL], A

;;; 
; H=1 N=0 and H=0 N=1 cases 
;;;
jrAnyTest1:
.start

    and A, A
    jrAny NN, .nnyes
    jp .start
.nnyes
    xor A, A
    cp A,A
    jrAny NN, .start

    xor A, A
    cp A, A
    jrAny NH, .nhyes
    jp .start
.nhyes
    and A, A
    jrAny NH, .start

    and A, A
    jrAny H, .hyes
    jp .start

.hyes
    xor A, A
    cp A, A
    jrAny H, .end

    xor A, A
    cp A, A
    jrAny N, .nyes
    jp .start
.nyes
    and A, A
    jrAny N, .end

    jp jrAnyTest2
.end
    jp .start

;;; 
; H=0 N=0 and H=1 N=1 cases 
;;;
jrAnyTest2:
.start
    ccf
    jrAny NN, .nnyes
    jp .start

.nnyes
    cpl
    jrAny NN, .start

    ccf
    jrAny NH, .nhyes
    jp .start
.nhyes
    cpl
    jrAny NH, .start

    cpl
    jrAny H, .hyes
    jp .start

.hyes
    ccf
    jrAny H, .end

    cpl
    jrAny N, .nyes
    jp .end
.nyes
    ccf
    jrAny N, .end
    jp passed
    nop
    nop
.end
    jp .start

passed:
    nop

    ENDC 
    

