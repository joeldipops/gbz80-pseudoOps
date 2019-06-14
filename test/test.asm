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
    ldAny [$A987], SP+0

    ldAny SP, [$A987]

    ldAny BC, BC
    ldAny BC, DE
    ldAny BC, $4321
    ldAny BC, [$4321]

    ldAny SP, BC
    ldAny SP, DE
    ldAny SP, HL
    ldAny SP, $4321


    ldAny BC, SP
    ldAny HL, SP
    ldAny [$A987], SP

    ldAny BC, SP+0
    ldAny HL, SP+0
    ldAny [$A987], SP+0
    

SECTION "HRAM", HRAM[$ff80]
HramStart: ds 1
    ENDC 
    

