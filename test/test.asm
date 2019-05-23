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
    ccf
    jrAny NN, nnyes
    jp $0100

nnyes:
    cpl
    jrAny NN, $0100

    ccf
    jrAny NH, nhyes
    jp $0100
nhyes:
    cpl
    jrAny NH, $0100

    cpl
    jrAny H, hyes
    jp $0100

hyes:
    ccf
    jrAny H, end

    cpl
    jrAny N, nyes
    jp $0100
nyes:
    ccf
    jrAny N, end
    nop
    nop
end:
    jp $0100

    ENDC
    
    

