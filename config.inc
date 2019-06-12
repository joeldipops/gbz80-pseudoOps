    IF !DEF(CONFIG_INCLUDED)
CONFIG_INCLUDED SET 1

;;;
; Prepending the token set here to a label/symbol/address will force 
; the assembler to use ldh when loading to/from it within an op.
; eg. 
; cpAny B, #[labelInHram]
; expands to
;
; ldh A, [labelInHram]
; cp B
;
;;;
LDH_TOKEN EQUS "\"#\""

;;;
; If you understand the warnings and don't want them polluting your assembler output, set this to 0
;;;
WARNINGS_ENABLED SET 1

;;;
; If you define an instruction or macro here, pseudoops with defined error states will
; include instructions to check for errors and call your handler.
; So far this just means checking for a divide by zero.
;;;
ERROR_HANDLER EQUS ""
;ERROR_HANDLER EQUS "rst $38"

    ENDC