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

SP_AMOUNT SET $1235
    mult B
    mult [HL]
    mult $12
    mult [$1234]

    ;ldAny BC, DE

    ;ldAny HL, SP + 1
    ldAny [$1234], SP
    ldAny #[HramStart], BC


    ldAny #[HramStart], #[HramStart]
    ldAny BC, #[HramStart]    

    ldAny [HramStart], [HramStart]
    ldAny BC, [HramStart]        

    ld A, 4
    divide 0
    ld A, 5
    divide 2
    divide #[$ffff]
    divide [$1234]
    divide [BC]
    nop
    nop
    ldAny BC, $1234
    ldAny BC, #[HramStart]

    ;addAny #[HramStart], #[HramStart]
    ld16 SP, HL
    ld16 [$1234], SP
    ld16 SP, $1234
    ld16 HL, SP + 255
    ld16 HL, SP+255
    ld16 HL, SP_AMOUNT
    ld16 HL, [SP_AMOUNT]
    ld16 HL, $1234


    ldiAny [DE], [HL]
    ;ldiAny [HL], [HL]
    ldiAny [HL], [DE]
    ;ldiAny [DE], [DE]

    ldiAny #[HramStart], [HL]


    ldAny A, 0
    ldAny A, 23
    ldAny A, B
    ldAny A, [HL]
    ldAny A, [DE]
    ldAny A, [$1234]
    ldAny A, [JoypadIo]
    ldAny A, #[JoypadIo]
    ldAny A, [jrAliasTest]
    ldAny A, [HramStart]
    ldAny A, #[HramStart]

    ldAny B, A
    ldAny [HL], A
    ldAny [DE], A
    ldAny [$1234], A
    ldAny [JoypadIo], A
    ldAny #[JoypadIo], A
    ldAny [jrAliasTest], A
    ldAny [HramStart], A
    ldAny #[HramStart], A  

    ldAny B, 0
    ldAny B, 23
    ldAny B, B
    ldAny B, [HL]
    ldAny B, [DE]
    ldAny B, [$1234]
    ldAny B, [JoypadIo]
    ldAny B, #[JoypadIo]
    ldAny B, [jrAliasTest]
    ldAny B, [HramStart]
    ldAny B, #[HramStart] 

    ldAny B, B
    ldAny [HL], B
    ldAny [DE], B
    ldAny [$1234], B
    ldAny [JoypadIo], B
    ldAny #[JoypadIo], B
    ldAny [jrAliasTest], B
    ldAny [HramStart], B
    ldAny #[HramStart], B         

    ldAny [HL], 0
    ldAny [HL], 23
    ldAny [HL], B
    ldAny [HL], [HL]
    ldAny [HL], [DE]
    ldAny [HL], [$1234]
    ldAny [HL], [JoypadIo]
    ldAny [HL], #[JoypadIo]
    ldAny [HL], [jrAliasTest]
    ldAny [HL], [HramStart]
    ldAny [HL], #[HramStart]        
    ;ldAny [HL], #[jrAliasTest] ; should fail    

    ldAny B, [HL]
    ldAny [HL], [HL]
    ldAny [DE], [HL]
    ldAny [$1234], [HL]
    ldAny [JoypadIo], [HL]
    ldAny #[JoypadIo], [HL]
    ldAny [jrAliasTest], [HL]
    ldAny [HramStart], [HL]
    ldAny #[HramStart], [HL]      

    ldAny [DE], 0
    ldAny [DE], 23
    ldAny [DE], B
    ldAny [DE], [HL]
    ldAny [DE], [DE]
    ldAny [DE], [$1234]
    ldAny [DE], [JoypadIo]
    ldAny [DE], #[JoypadIo]
    ldAny [DE], [jrAliasTest]
    ldAny [DE], [HramStart]
    ldAny [DE], #[HramStart]        

    ldAny B, [DE]
    ldAny [HL], [DE]
    ldAny [DE], [DE]
    ldAny [$1234], [DE]
    ldAny [JoypadIo], [DE]
    ldAny #[JoypadIo], [DE]
    ldAny [jrAliasTest], [DE]
    ldAny [HramStart], [DE]
    ldAny #[HramStart], [DE]      
    ;ldAny [DE], #[jrAliasTest] ; should fail

    ldAny [$1234], 0
    ldAny [$1234], 23
    ldAny [$1234], B
    ldAny [$1234], [HL]
    ldAny [$1234], [DE]
    ldAny [$1234], [$1234]
    ldAny [$1234], [$ff12]    
    ldAny [$1234], #[$ff12]
    ldAny #[$ff12], [$ff12]        
    ldAny #[$ff12], #[$ff12]    
    ldAny [$1234], [JoypadIo]
    ldAny [$1234], #[JoypadIo]
    ldAny [$1234], [jrAliasTest]
    ldAny [$1234], [HramStart]
    ldAny [$1234], #[HramStart]        

    ldAny #[JoypadIo], #[HramStart]        
    ldAny #[HramStart], #[JoypadIo]    
    ldAny [jrAliasTest], [JoypadIo]        
    ldAny [HramStart], #[JoypadIo]        
    ldAny #[HramStart], #[HramStart]        

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
    jp main

ldilddTest:
    ldiAny [HL], B
    lddAny [HL], B

;;; 
; H=1 N=0 and H=0 N=1 cases 
;;;
jrAnyTest1:
.start

    and A, B
    jrAny NN, .nnyes
    jp .start
.nnyes
    xor A, B
    cp A,A
    jrAny NN, .start

    xor A, B
    cp A, B
    jrAny NH, .nhyes
    jp .start
.nhyes
    and A, B
    jrAny NH, .start

    and A, B
    jrAny H, .hyes
    jp .start

.hyes
    xor A, B
    cp A, B
    jrAny H, .end

    xor A, B
    cp A, B
    jrAny N, .nyes
    jp .start
.nyes
    and A, B
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

SECTION "HRAM", HRAM[$ff80]
HramStart: ds 1
    ENDC 
    

