
                .686p
                .mmx
                .model flat, C

WINVER equ 0501h

EXTERN windux_exit:PROC
EXTERN __iob_func:PROC
EXTERN memmove:PROC
EXTERN getc:PROC
EXTERN putc:PROC
EXTERN strcpy:PROC
EXTERN strtok:PROC
EXTERN fprintf:PROC
EXTERN atoi:PROC
EXTERN sscanf:PROC
EXTERN fputs:PROC
EXTERN strcmp:PROC
EXTERN strlen:PROC
EXTERN strncmp:PROC
EXTERN perror:PROC
EXTERN exit:PROC
EXTERN calloc:PROC 
	;num: DWORD, 
	;elementSize: DWORD
EXTERN free:PROC
	;ptrBlock : PTR
EXTERN yyin:dword
EXTERN yyout:dword


; for C+++ use public

public C s_level
public C symtab_inited
public C pdfsym_root;
public C pdfsym_list;
public C symtab_next;
public C p_symtab_curr
public C symtab
public C p_sym_node
public C str_crt;

; struct var

symtab_t        struc ; (sizeof=0x1C)
s_type          dd ?                    
s_class         dd ?                    
s_level         dd ?                    
s_param         dd ?                   
s_curr          dd ?                   
s_prev          dd ?                    
s_next          dd ?                    
symtab_t        ends
; -------------------------------------------------------------------------
symbol_node_t   struc ; (sizeof=0x8)
first           dd ?                   
curr            dd ?                    
symbol_node_t   ends
; -------------------------------------------------------------------------

symtab_node_t   struc ; (sizeof=0x14)
p_par           dd ?                    
s_level         dd ?                    
name            db 8 dup(?)             
p_sym           dd ?                    
symtab_node_t   ends

.DATA  ;Êý¾Ý¶Î

___security_cookie dd 0BB40E64Eh

aLevel0_0       db 'Level0',0           ; 
                align 4
aLevel1_0       db 'Level1',0           ; 
                align 10h
pdfsym_root     symtab_t <offset aLevel0_0, 0, 0, 0, offset pdfsym_root, 0, \
                          offset pdfsym_root.s_type+1Ch>
                symtab_t <offset aLevel1_0, 1, 1, 0, offset pdfsym_root.s_type+1Ch, \
                          offset pdfsym_root, 0>
pdfsym_list     symbol_node_t <offset pdfsym_root, offset pdfsym_root.s_type+1Ch>
symtab_next     symtab_t <0, 0, 0, 0, offset symtab_next, 0, 0>
p_symtab_curr   dd offset pdfsym_root.s_type+1Ch
s_level         dd 1                   
symtab_inited   dd 1                    
symtab          dd offset pdfsym_root   
                dd 13h dup(0)
aLevel0         db 'Level0',0           ; 
                align 10h
aLevel1         db 'Level1',0           ; 
                align 4
aRead           db 'read',0             ; 
                align 10h
aRead_0         db 'read',0             ; 
                align 4
aRead_1         db 'read',0             ; 
                align 10h
aRead_2         db 'read',0             ; 
                align 4
aRead_3         db 'read',0             ; 
                align 10h
aRead_4         db 'read',0             ; 
                align 4
aRead_5         db 'read',0             ; 
                align 10h
aRead_6         db 'read',0             ; 
                align 4
aRead_7         db 'read',0             ; 
                align 10h
aClose          db 'close',0            ; 
                align 4
Source          db 'crt?',0             ; 
                align 10h
aPlt            db 'plt?',0             ; 
                align 4
aPlt_0          db 'plt?',0             ; 
                align 10h
aCrt_0          db 'crt?',0             ; 
                align 4
aPdfappInvalidD db 0Ah                  ; 
                db 'pdfapp: Invalid device type.',0Ah,0
                align 4
ErrMsg          db 0Ah                  ; 
                db 'PDF interface: Out of memory!',0Ah,0

byte_418F1D     db ?                    ; 
                db    ? ;
                db    ? ;
yy_buffer_stack db 600 dup(?)           
yy_c_buf_p      db 604 dup(?) 
;yyin            dd ?
;yyout           dd ? 
oldend          dd ?
oldstart        dd ?
oldlength       dd ?
dword_4193E8    dd 12Eh dup(?) 
str_error_texts dd ?
fp_out          dd ?
yy_accept_0     dw 1, 9, 0Bh, 1, 9, 0Bh, 1, 9, 0Bh, 4, 0Bh, 9, 0Bh, 5, 9, 0Bh
                dw 9, 0Bh, 9, 0Bh, 0Ah, 0Bh, 0Ah, 0Bh, 9, 6, 3 dup(9), 6, 9, 5, 9, 7
                dw 3 dup(9), 8, 9, 6, 7, 3 dup(9), 7, 3 dup(9), 6, 7, 2 dup(9), 1, 9, 1, 9, 1
                dw 2 dup(9), 3, 9, 3, 9, 1, 9, 1, 2 dup(9), 2, 9, 2, 2 dup(9), 0Bh, 9, 7 dup(0)
                dw 3, 6, 9, 0Bh, 0Dh, 10h, 12h, 14h, 16h, 17h, 18h, 19h, 1Bh, 1Ch, 1Dh, 1Fh
                dw 21h, 23h, 24h, 25h, 27h, 2Ah, 2Bh, 2Ch, 2Eh, 2Fh, 30h, 33h, 34h, 36h, 38h, 3Ah
                dw 3Bh, 3Dh, 3Fh, 41h, 43h, 44h, 46h, 2 dup(48h), 2 dup(4Ah), 2 dup(4Bh), 4 dup(4Ch), 0
byte_40EFE8     db 0, 29h, 6 dup(0)     ; 
byte_40EFF0     db 9 dup(8), 9, 0Ah, 2 dup(8), 9, 12h dup(8), 9, 8, 4, 8 dup(8), 5, 0Bh, 5, 7, 8, 0Ah dup(6)
                db 8, 0Bh, 5 dup(8), 2 dup(1), 2, 0Ch dup(1), 3, 0Ah dup(1), 3 dup(8), 0Bh, 1, 8, 1Ah dup(1), 0Bh, 8, 0Bh
                db 2 dup(8), 9 dup(0Ch), 2Ah, 32h, 2 dup(0Ch), 32h, 12h dup(0Ch), 32h, 0Ch, 32h, 9 dup(0Ch), 32h, 0Eh dup(0Ch), 32h, 22h dup(0Ch), 32h
                db 1Ch dup(0Ch), 32h, 0Ch, 30h, 2 dup(0Ch), 14h dup(0Dh), 30h, 6 dup(32h), 0Fh, 32h, 0Fh, 32h, 0Eh, 1Eh dup(10h), 12h, 0Eh
                db 0Ah dup(11h), 7 dup(32h), 14h, 32h, 14h, 32h, 13h, 1Eh dup(15h), 32h, 13h, 0Ah dup(16h), 7 dup(32h), 18h, 32h, 18h, 32h
                db 17h, 1Eh dup(19h), 32h, 17h, 0Ah dup(16h), 7 dup(32h), 1Bh, 32h, 1Bh, 32h, 1Ah, 1Eh dup(1Ch), 12h, 1Ah, 0Ah dup(11h), 0Ah dup(0Dh)
                db 1Dh, 13h, 0Ah dup(11h), 5 dup(32h), 0Ah dup(1Eh), 6 dup(32h), 13h, 0Bh dup(1Eh), 1Fh, 0Eh dup(1Eh), 4 dup(32h), 1Eh, 32h, 24h dup(1Eh), 7 dup(32h), 13h dup(1Eh)
                db 20h, 6 dup(1Eh), 4 dup(32h), 1Eh, 32h, 24h dup(1Eh), 5 dup(32h), 21h, 32h, 1Ah dup(1Eh), 4 dup(32h), 1Eh, 32h, 1Ah dup(1Eh), 1Ah dup(22h), 4 dup(32h)
                db 22h, 32h, 1Ah dup(22h), 0Ah dup(23h), 7 dup(32h), 1Ah dup(23h), 4 dup(32h), 23h, 32h, 24h dup(23h), 7 dup(32h), 1Ah dup(23h), 4 dup(32h), 23h, 32h, 1Ah dup(23h)
                db 0Ah dup(1Eh), 7 dup(32h), 1Ah dup(1Eh), 4 dup(32h), 1Eh, 32h, 24h dup(1Eh), 7 dup(32h), 11h dup(1Eh), 24h, 8 dup(1Eh), 4 dup(32h), 1Eh, 32h, 24h dup(1Eh), 7 dup(32h)
                db 13h dup(1Eh), 25h, 6 dup(1Eh), 4 dup(32h), 1Eh, 32h, 24h dup(1Eh), 5 dup(32h), 26h, 32h, 1Ah dup(1Eh), 4 dup(32h), 1Eh, 32h, 1Ah dup(1Eh), 1Ah dup(27h)
                db 4 dup(32h), 27h, 32h, 1Ah dup(27h), 0Ah dup(28h), 7 dup(32h), 1Ah dup(28h), 4 dup(32h), 28h, 32h, 24h dup(28h), 7 dup(32h), 1Ah dup(28h), 4 dup(32h), 28h, 32h
                db 1Ah dup(28h), 0Ah dup(1Eh), 7 dup(32h), 1Ah dup(1Eh), 4 dup(32h), 1Eh, 32h, 1Ah dup(1Eh), 2Bh, 9 dup(32h), 2Dh, 2Eh, 0Bh dup(32h), 2Bh, 9 dup(32h), 2Dh
                db 5 dup(32h), 14h dup(2Ch), 9 dup(30h), 2Fh, 2Eh, 15h dup(30h), 2Fh, 5Fh dup(30h), 2Bh, 9 dup(32h), 2Dh, 2Eh, 0Bh dup(32h), 2Bh, 9 dup(32h), 2Dh
                db 5 dup(32h), 14h dup(31h), 3 dup(0)
yy_chk          db 80h dup(0), 9 dup(8), 29h, 0FFh, 2 dup(8), 0FFh, 12h dup(8), 0FFh, 8, 0FFh, 9 dup(8), 0FFh, 0Eh dup(8), 0FFh, 22h dup(8), 0FFh

                db 1Ch dup(8), 0FFh, 8, 30h, 2 dup(8), 0Ah dup(7), 0Ah dup(0Dh), 30h, 6 dup(0FFh), 0Eh, 0FFh, 0Eh, 0FFh, 0Dh, 0Ah dup(0Eh), 0Ah dup(10h)
                db 0Ah dup(0Fh), 6, 0Dh, 0Ah dup(6), 7 dup(0FFh), 13h, 0FFh, 13h, 0FFh, 6, 0Ah dup(13h), 0Ah dup(15h), 0Ah dup(14h), 0FFh, 6, 0Ah dup(12h)
                db 7 dup(0FFh), 17h, 0FFh, 17h, 0FFh, 12h, 0Ah dup(17h), 0Ah dup(19h), 0Ah dup(18h), 0FFh, 12h, 0Ah dup(16h), 7 dup(0FFh), 1Ah, 0FFh, 1Ah
                db 0FFh, 16h, 0Ah dup(1Ah), 0Ah dup(1Ch), 0Ah dup(1Bh), 11h, 16h, 0Ah dup(11h), 0Ah dup(1Dh), 5, 11h, 0Ah dup(5), 5 dup(0FFh), 0Ah dup(3), 6 dup(0FFh), 11h
                db 1Ah dup(3), 4 dup(0FFh), 3, 0FFh, 1Ah dup(3), 0Ah dup(1Fh), 7 dup(0FFh), 1Ah dup(1Fh), 4 dup(0FFh), 1Fh, 0FFh, 1Ah dup(1Fh), 0Ah dup(20h), 5 dup(0FFh), 20h, 0FFh
                db 1Ah dup(20h), 4 dup(0FFh), 20h, 0FFh, 1Ah dup(20h), 1Ah dup(21h), 4 dup(0FFh), 21h, 0FFh, 1Ah dup(21h), 0Ah dup(22h), 7 dup(0FFh), 1Ah dup(22h), 4 dup(0FFh), 22h, 0FFh
                db 1Ah dup(22h), 0Ah dup(23h), 7 dup(0FFh), 1Ah dup(23h), 4 dup(0FFh), 23h, 0FFh, 1Ah dup(23h), 0Ah dup(1Eh), 7 dup(0FFh), 1Ah dup(1Eh), 4 dup(0FFh), 1Eh, 0FFh, 1Ah dup(1Eh), 0Ah dup(2)
                db 7 dup(0FFh), 1Ah dup(2), 4 dup(0FFh), 2, 0FFh, 1Ah dup(2), 0Ah dup(24h), 7 dup(0FFh), 1Ah dup(24h), 4 dup(0FFh), 24h, 0FFh, 1Ah dup(24h), 0Ah dup(25h), 5 dup(0FFh), 25h
                db 0FFh, 1Ah dup(25h), 4 dup(0FFh), 25h, 0FFh, 1Ah dup(25h), 1Ah dup(26h), 4 dup(0FFh), 26h, 0FFh, 1Ah dup(26h), 0Ah dup(27h), 7 dup(0FFh), 1Ah dup(27h), 4 dup(0FFh), 27h
                db 0FFh, 1Ah dup(27h), 0Ah dup(28h), 7 dup(0FFh), 1Ah dup(28h), 4 dup(0FFh), 28h, 0FFh, 1Ah dup(28h), 0Ah dup(1), 7 dup(0FFh), 1Ah dup(1), 4 dup(0FFh), 1, 0FFh, 1Ah dup(1)
                db 2Ah, 9 dup(0FFh), 2 dup(2Ch), 0Bh dup(0FFh), 2Ah, 9 dup(0FFh), 2Ch, 5 dup(0FFh), 0Ah dup(2Ah), 0Ah dup(2Ch), 80h dup(2Dh), 2Bh, 9 dup(0FFh), 2 dup(31h), 0Bh dup(0FFh), 2Bh
                db 9 dup(0FFh), 31h, 5 dup(0FFh), 0Ah dup(2Bh), 0Ah dup(31h), 3 dup(0)
byte_40FDE0     db 32h, 3 dup(8), 32h, 3 dup(8), 4 dup(32h), 1Dh dup(8), 0, 8, 32h, 8, 2 dup(32h), 2 dup(2Dh), 32h, 0
                align 8
yy_base         dw 0, 584h, 3D3h, 1D7h, 6F5h, 1C8h, 110h, 0D0h, 80h, 4 dup(6F5h), 0DAh, 0F0h, 104h, 0FAh, 1B2h, 146h
                dw 126h, 13Ah, 130h, 17Ch, 15Ch, 170h, 166h, 192h, 1A6h, 19Ch, 1BCh, 388h, 222h, 26Dh, 2A7h, 2F2h
                dw 33Dh, 41Eh, 469h, 4A3h, 4EEh, 539h, 66h, 5F6h, 6B1h, 600h, 63Ah, 2 dup(6F5h), 0F4h, 6BBh, 6F5h, 0
oldline         dd 1
oldlastc        dd 0Ah
yytext_ptr      dd offset yy_c_buf_p 
aTokenBufferOve db 'Token buffer overflow',0 ;
                align 10h
aYylexS         db 'yylex: %s',0Ah,0    ;
                align 4
byte_40FEEC     db 1, 2, 4, 8, 10h, 20h, 40h, 80h
aLd             db '%ld',0              ;
aLf             db '%lf',0              ; 
aPushBackBuffer db 'Push-back buffer overflow',0
                align 4
aYylexS_0       db 'yylex: %s',0Ah,0    ; 
                align 8
                align 4
yy_accept       dw 5, 0FFFFh, 0FFFBh, 0FFF7h, 0FFF3h, 0FFEBh, 1Eh, 2 dup(0), 21h, 0FFFFh, 1, 2 dup(0), 0FFFFh, 1, 3Bh, 20h
                dw 0FFFFh, 1, 100h, 3, 104h, 3, 7Dh, 4, 0FFFFh, 1, 3Bh, 1Fh, 0FFFFh, 1, 2 dup(0), 0FFE3h
                dw 0FFFFh, 104h, 100h, 0FFFBh, 7Bh, 0FFE2h, 0FFFFh, 104h, 100h, 0FFDEh, 0FFDCh, 0FFDBh, 0FFECh, 0FFEDh, 0FFF0h, 105h
                dw 104h, 103h, 102h, 101h, 100h, 0FFF7h, 0FFF9h, 0FFDCh, 0FFDBh, 0FFECh, 0FFEDh, 0FFF0h, 105h, 104h, 103h, 102h
                dw 101h, 100h, 2Ch, 0FFF2h, 3Bh, 0FFF1h, 7Dh, 0FFDCh, 0FFDBh, 0FFECh, 0FFEDh, 0FFF0h, 105h, 104h, 103h, 102h
                dw 101h, 0FFDDh, 0FFDCh, 0FFDBh, 0FFECh, 0FFEDh, 0FFF0h, 105h, 104h, 103h, 102h, 101h, 100h, 0FFF1h, 0FFE2h, 0FFFFh
                dw 104h, 100h, 7Dh, 0FFFFh, 0
word_410034     dw 5, 8, 0Ah, 10h, 16h, 1Dh, 2Dh, 41h, 38h, 2, 27h, 25h, 2, 0
word_410050     dw 0FFEEh, 0FFEFh, 0FFFDh, 0FFF8h, 0FFFEh, 9, 2 dup(0FFE5h), 0FFE4h, 7, 1, 0FFE1h, 0FFE0h, 0FFF6h, 0FFE6h, 0FFE7h, 7
                dw 0FFF5h, 0FFFCh, 0FFF4h, 0FFE8h, 0FFFAh, 0FFE9h, 0FFEBh, 0FFEAh, 0FFDFh, 8, 6, 5, 0FFFFh, 2 dup(0)
word_410090     dw 3 dup(0), 0Dh, 11h, 12h, 14h, 0Fh, 5 dup(19h), 3 dup(15h)
                dw 0Ch, 2 dup(0Bh), 4 dup(4), 2 dup(8), 3 dup(15h), 0
                dw 1, 15h, 2 dup(13h), 2, 2 dup(0)
word_4100D8     dw 6 dup(0), 7 dup(1), 2 dup(3), 2, 2 dup(4), 5, 2, 2 dup(1)
                dw 2, 4 dup(1), 3, 2 dup(1), 2, 1, 0, 1, 2, 0
aParserStackOve db 'Parser stack overflow',0 ; 
                db    0
                db    0
aSyntaxError    db 'Syntax error',0     ; 
                db    0
                db    0
                db    0
asc_410148      db '""',0               ; 
                db    0
asc_41014C      db './',0               ; 
                db    0
aFileS          db 'file %.*s',0,0,0    ; 
asc_41015C      db ', ',0               ; 
                db    0
aLineD          db 'line %d',0          ; 
aNearS          db 'near "%.*s"',0      ; 
asc_410174      db ': ',0               ; 
                align 4
asc_410178      db '# ',9,0             ; 
asc_41017C      db 0Dh,0Ah,0            ; 
                align 10h
aErrorD         db '[error %d] ',0      ; 
aErrorD_0       db '[error %d] ',0      ; 
aUnexpectedNewl db 'Unexpected newline in quoted string',0
                                        ;
str_crt         db 8 dup(?) 
p_sym_node      symtab_node_t <?>
dword_420854    dd ?                 
dword_420858    dd ?                 
dword_42085C    dd ?
dword_420860    dd ?                 
dword_420864    dd ?               
yychar          dd ?             
yy_error_no     dd ?
dword_420870    dd ?                    ; 
                db    ? ;
                db    ? ;
                db    ? ;
                db    ? ;
                db    ? ;
                db    ? ;
                db    ? ;
                db    ? ;
                db    ? ;
                db    ? ;
                db    ? ;
byte_42087F     db ?                    ; 
yytext          db 25Ch dup(?)


.CODE

INCLUDELIB kernel32.lib
INCLUDELIB msvcrt.lib ;msvcrtd.lib ;
INCLUDELIB legacy_stdio_definitions.lib

 my_alloc        proc near               ; 
 var_4           = dword ptr -4
 Count           = dword ptr  8
 ;Size            = dword ptr  0Ch

                 push    ebp
                 mov     ebp, esp
                 push    ecx
                 mov     eax, [ebp+0Ch]
                 push    eax
                 mov     ecx, [ebp+Count]
                 push    ecx
                 call    calloc
                 add     esp, 8
                 mov     [ebp+var_4], eax
                 cmp     [ebp+var_4], 0
                 jnz     short loc_404954
                 push    offset ErrMsg
                 call    ds:perror
                 add     esp, 4
                 push    1
                 call    ds:exit

 loc_404954:                             ; 
                 mov     eax, [ebp+var_4]
                 mov     esp, ebp
                 pop     ebp
                 retn
 my_alloc        endp

alloc_copy      proc near               ; 
Destination     = dword ptr -4
;Str             = dword ptr  8

                push    ebp
                mov     ebp, esp
                push    ecx
                mov     eax, [ebp+8]
                push    eax
                call    strlen
                add     esp, 4
                add     eax, 1
                push    eax
                push    1
                call    my_alloc
                add     esp, 8
                mov     [ebp+Destination], eax
                mov     ecx, [ebp+8]
                push    ecx
                mov     edx, [ebp+Destination]
                push    edx
                call    strcpy
                add     esp, 8
                mov     eax, [ebp+Destination]
                mov     esp, ebp
                pop     ebp
                retn
alloc_copy      endp

get_true proc near             ; 
                push    ebp
                mov     ebp, esp
                mov     eax, 1
                pop     ebp
                retn
get_true endp

sub_404F80      proc near              
                push    ebp
                mov     ebp, esp
                mov     eax, [ebp+8]
                push    eax
                call    sub_404FC0
                add     esp, 4
                mov     ecx, p_symtab_curr
                mov     dword ptr [ecx+4], 1
                mov     edx, s_level
                add     edx, 1
                mov     s_level, edx
                mov     eax, p_symtab_curr
                mov     ecx, s_level
                mov     [eax+8], ecx
                pop     ebp
                retn
sub_404F80      endp

sub_404FC0      proc near               

p_tab           = dword ptr -4

                push    ebp
                mov     ebp, esp
                push    ecx
                push    1Ch
                push    1
                call    my_alloc
                add     esp, 8
                mov     [ebp+p_tab], eax
                mov     eax, [ebp+p_tab]
                mov     ecx, p_symtab_curr
                mov     [eax+14h], ecx
                mov     edx, [ebp+8]
                push    edx
                call    alloc_copy
                add     esp, 4
                mov     ecx, [ebp+p_tab]
                mov     [ecx], eax
                mov     edx, p_symtab_curr
                mov     eax, [ebp+p_tab]
                mov     [edx+18h], eax
                mov     ecx, [ebp+p_tab]
                mov     p_symtab_curr, ecx
                mov     edx, p_symtab_curr
                mov     dword ptr [edx+18h], offset symtab_next
                mov     eax, p_symtab_curr
                mov     symtab_next.s_prev, eax
                mov     esp, ebp
                pop     ebp
                retn
sub_404FC0      endp

sub_405020      proc near               ; 
                push    ebp
                mov     ebp, esp
                mov     eax, [ebp+8]
                push    eax
                call    sub_404FC0
                add     esp, 4
                mov     ecx, p_symtab_curr
                mov     dword ptr [ecx+4], 2
                mov     edx, p_symtab_curr
                mov     eax, s_level
                mov     [edx+8], eax
                mov     ecx, p_symtab_curr
                mov     edx, [ecx+0Ch]
                mov     p_sym_node.p_par, edx
                pop     ebp
                retn
sub_405020      endp


sub_405060      proc near               ; 

var_4           = dword ptr -4
arg_0           = dword ptr  8

                push    ebp
                mov     ebp, esp
                push    ecx
                push    10h
                push    1
                call    my_alloc
                add     esp, 8
                mov     [ebp+var_4], eax
                mov     eax, p_symtab_curr
                cmp     dword ptr [eax+0Ch], 0
                jnz     short loc_40508C
                mov     ecx, p_symtab_curr
                mov     edx, [ebp+var_4]
                mov     [ecx+0Ch], edx
                jmp     short loc_405096


loc_40508C:                             ; 
                mov     eax, p_sym_node.p_par
                mov     ecx, [ebp+var_4]
                mov     [eax], ecx

loc_405096:                             ; 
                mov     edx, [ebp+var_4]
                mov     p_sym_node.p_par, edx
                mov     eax, p_sym_node.p_par
                mov     dword ptr [eax+4], 103h
                mov     ecx, p_sym_node.p_par
                mov     edx, [ebp+arg_0]
                mov     [ecx+8], edx
                mov     esp, ebp
                pop     ebp
                retn
sub_405060      endp

sub_4050C0      proc near               ; 

var_4           = dword ptr -4
arg_0           = dword ptr  8

                push    ebp
                mov     ebp, esp
                push    ecx
                push    10h
                push    1
                call    my_alloc
                add     esp, 8
                mov     [ebp+var_4], eax
                mov     eax, p_symtab_curr
                cmp     dword ptr [eax+0Ch], 0
                jnz     short loc_4050EC
                mov     ecx, p_symtab_curr
                mov     edx, [ebp+var_4]
                mov     [ecx+0Ch], edx
                jmp     short loc_4050F6


loc_4050EC:                             ; 
                mov     eax, p_sym_node.p_par
                mov     ecx, [ebp+var_4]
                mov     [eax], ecx

loc_4050F6:                             ; 
                mov     edx, [ebp+var_4]
                mov     p_sym_node.p_par, edx
                mov     eax, p_sym_node.p_par
                mov     dword ptr [eax+4], 101h
                mov     ecx, p_sym_node.p_par
                mov     edx, [ebp+arg_0]
                mov     [ecx+8], edx
                mov     esp, ebp
                pop     ebp
                retn
sub_4050C0      endp


sub_405120      proc near               ; 

var_4           = dword ptr -4
arg_0           = qword ptr  8

                push    ebp
                mov     ebp, esp
                push    ecx
                push    10h
                push    1
                call    my_alloc
                add     esp, 8
                mov     [ebp+var_4], eax
                mov     eax, p_symtab_curr
                cmp     dword ptr [eax+0Ch], 0
                jnz     short loc_40514C
                mov     ecx, p_symtab_curr
                mov     edx, [ebp+var_4]
                mov     [ecx+0Ch], edx
                jmp     short loc_405156


loc_40514C:                             ; 
                mov     eax, p_sym_node.p_par
                mov     ecx, [ebp+var_4]
                mov     [eax], ecx

loc_405156:                             ; 
                mov     edx, [ebp+var_4]
                mov     p_sym_node.p_par, edx
                mov     eax, p_sym_node.p_par
                mov     dword ptr [eax+4], 102h
                mov     ecx, p_sym_node.p_par
                fld     [ebp+arg_0]
                fstp    qword ptr [ecx+8]
                mov     esp, ebp
                pop     ebp
                retn
sub_405120      endp



yylex           proc near               ;

var_44          = dword ptr -44h
var_40          = dword ptr -40h
var_3C          = dword ptr -3Ch
yyleng          = dword ptr -38h
var_34          = dword ptr -34h
var_30          = dword ptr -30h
var_2C          = dword ptr -2Ch
number_to_move  = dword ptr -28h
var_24          = dword ptr -24h
var_20          = dword ptr -20h
yy_c            = dword ptr -1Ch
var_18          = dword ptr -18h
var_14          = dword ptr -14h
yy_current_state= dword ptr -10h
var_C           = dword ptr -0Ch
length__          = dword ptr -8
var_4           = dword ptr -4

                push    ebp
                mov     ebp, esp
                sub     esp, 44h
                push    esi
                cmp     yyin, 0
                jnz     short loc_4063BB
                call    __iob_func
                mov     yyin, eax

loc_4063BB:                             ; 
                cmp     yyout, 0
                jnz     short loc_4063D2
                call    __iob_func
                add     eax, 20h
                mov     yyout, eax

loc_4063D2:                             ; 
                mov     [ebp+var_14], 0
                mov     eax, oldlength
                mov     [ebp+length__], eax
                mov     ecx, offset yy_c_buf_p
                sub     ecx, yytext_ptr
                mov     [ebp+number_to_move], ecx
                jz      short loc_406411
                mov     edx, [ebp+number_to_move]
                push    edx
                mov     eax, yytext_ptr
                push    eax
                mov     ecx, oldlength
                add     ecx, offset yytext
                push    ecx
                call    ds:memmove
                add     esp, 0Ch

loc_406411:                             ; 
                mov     edx, oldlength
                add     edx, [ebp+number_to_move]
                mov     oldend, edx

loc_406420:                             ; 

                mov     eax, [ebp+length__]
                mov     oldlength, eax
                cmp     [ebp+length__], 0
                jle     short loc_40643E
                mov     ecx, [ebp+length__]
                movsx   edx, byte_42087F[ecx]
                mov     oldlastc, edx

loc_40643E:                             ; 

                cmp     [ebp+length__], 0
                jle     short loc_40646B
                mov     eax, [ebp+length__]
                sub     eax, 1
                mov     [ebp+length__], eax
                mov     ecx, [ebp+length__]
                movsx   edx, yytext[ecx]
                cmp     edx, 0Ah
                jnz     short loc_406469
                mov     eax, oldline
                add     eax, 1
                mov     oldline, eax

loc_406469:                             ; 
                jmp     short loc_40643E


loc_40646B:                             ; 
                mov     ecx, oldend
                sub     ecx, oldlength
                mov     oldend, ecx
                mov     edx, oldend
                push    edx
                mov     eax, oldlength
                add     eax, offset yytext
                push    eax
                push    offset yytext
                call    ds:memmove
                add     esp, 0Ch
                mov     [ebp+length__], 0

loc_4064A4:                             ; 
                mov     ecx, [ebp+length__]
                mov     [ebp+var_20], ecx
                xor     edx, edx
                cmp     oldlastc, 0Ah
                setz    dl
                mov     eax, oldstart
                movzx   ecx, byte_40EFE8[eax+edx]
                mov     [ebp+yy_current_state], ecx
                mov     edx, [ebp+length__]
                mov     al, byte ptr [ebp+yy_current_state]
                mov     yy_c_buf_p[edx], al

loc_4064D2:                             ; 

                cmp     [ebp+length__], 258h
                jl      short loc_406502
                push    offset aTokenBufferOve
                push    offset aYylexS
                call    ds:__iob_func
                add     eax, 40h
                push    eax
                call    ds:fprintf
                add     esp, 0Ch
                push    1
                call    windux_exit
                add     esp, 4

loc_406502:                             ; 
                mov     ecx, [ebp+length__]
                cmp     ecx, oldend
                jge     short loc_40651C
                mov     edx, [ebp+length__]
                movsx   eax, yytext[edx]
                mov     [ebp+yy_c], eax
                jmp     short loc_406590


loc_40651C:                             ; 
                cmp     [ebp+var_14], 0
                jnz     short loc_406555
                mov     ecx, yyin
                push    ecx
                call    ds:getc
                add     esp, 4
                mov     [ebp+yy_c], eax
                cmp     [ebp+yy_c], 0FFFFFFFFh
                jz      short loc_406555
                mov     edx, [ebp+length__]
                add     edx, 1
                mov     oldend, edx
                mov     eax, [ebp+length__]
                mov     cl, byte ptr [ebp+yy_c]
                mov     yytext[eax], cl
                jmp     short loc_406590


loc_406555:                             ; 

                mov     [ebp+yy_c], 0FFFFFFFFh
                mov     edx, [ebp+length__]
                cmp     edx, [ebp+var_20]
                jnz     short loc_406584
                mov     [ebp+var_14], 0
                call    get_true
                test    eax, eax
                jz      short loc_40657D
                xor     eax, eax
                jmp     loc_40699B

                jmp     short loc_406582


loc_40657D:                             ; 
                jmp     loc_406420


loc_406582:                             ; 
                jmp     short loc_406590


loc_406584:                             ; 
                mov     [ebp+var_14], 1
                jmp     loc_406634


loc_406590:                             ; 

                mov     eax, [ebp+yy_current_state]
                movsx   ecx, yy_base[eax*2]
                movzx   edx, byte ptr [ebp+yy_c]
                add     ecx, edx
                mov     [ebp+var_C], ecx
                cmp     [ebp+var_C], 6F4h
                jg      short loc_4065BF
                mov     eax, [ebp+var_C]
                movzx   ecx, yy_chk[eax]
                movzx   edx, byte ptr [ebp+yy_current_state]
                cmp     ecx, edx
                jz      short loc_4065D6

loc_4065BF:                             ; 
                cmp     [ebp+yy_current_state], 32h
                jnz     short loc_4065C7
                jmp     short loc_4065E3


loc_4065C7:                             ; 
                mov     eax, [ebp+yy_current_state]
                movzx   ecx, byte_40FDE0[eax]
                mov     [ebp+yy_current_state], ecx
                jmp     short loc_406590


loc_4065D6:                             ; 
                mov     edx, [ebp+var_C]
                movzx   eax, byte_40EFF0[edx]
                mov     [ebp+yy_current_state], eax

loc_4065E3:                             ; 
                mov     ecx, [ebp+length__]
                add     ecx, 1
                mov     [ebp+length__], ecx
                mov     edx, [ebp+length__]
                mov     al, byte ptr [ebp+yy_current_state]
                mov     yy_c_buf_p[edx], al
                cmp     [ebp+yy_current_state], 32h
                jz      short loc_406634
                mov     ecx, 1
                test    ecx, ecx
                jz      loc_4064D2
                mov     edx, [ebp+yy_current_state]
                movsx   eax, yy_base[edx*2]
                cmp     eax, 6F4h
                jle     loc_4064D2
                mov     ecx, [ebp+yy_current_state]
                movzx   edx, byte_40FDE0[ecx]
                cmp     edx, 32h
                jnz     loc_4064D2

loc_406634:                             ; 

                cmp     [ebp+yy_current_state], 32h
                jz      short loc_406643
                mov     eax, [ebp+length__]
                add     eax, 1
                mov     [ebp+length__], eax

loc_406643:                             ; 

                mov     ecx, [ebp+length__]
                sub     ecx, 1
                mov     [ebp+length__], ecx
                mov     edx, [ebp+length__]
                cmp     edx, [ebp+var_20]
                jle     short loc_406689
                mov     eax, [ebp+length__]
                movzx   ecx, yy_c_buf_p[eax]
                mov     [ebp+yy_current_state], ecx
                mov     edx, [ebp+yy_current_state]
                movsx   eax, yy_accept_0+0A0h[edx*2]
                mov     [ebp+var_24], eax
                mov     ecx, [ebp+yy_current_state]
                movsx   edx, yy_accept_0+0A2h[ecx*2]
                mov     [ebp+var_4], edx
                mov     eax, [ebp+var_24]
                cmp     eax, [ebp+var_4]
                jge     short loc_406687
                jmp     short loc_4066B2


loc_406687:                             ; 
                jmp     short loc_406643


loc_406689:                             ; 
                mov     ecx, [ebp+var_20]
                add     ecx, 1
                mov     [ebp+length__], ecx
                mov     edx, yyout
                push    edx
                mov     eax, [ebp+var_20]
                movsx   ecx, yytext[eax]
                push    ecx
                call    ds:putc
                add     esp, 8
                jmp     loc_406420


loc_4066B2:                             ; 

                mov     edx, [ebp+length__]
                mov     [ebp+var_18], edx
                mov     eax, [ebp+var_24]
                movzx   ecx, yy_accept_0[eax*2]
                sar     ecx, 9
                sub     ecx, 1
                mov     [ebp+yy_c], ecx
                js      short loc_406734
                mov     edx, [ebp+yy_c]
                imul    edx, 7
                add     edx, (offset yy_c_buf_p+259h)
                mov     [ebp+var_2C], edx

loc_4066DD:                             ; 
                mov     eax, 1
                test    eax, eax
                jz      short loc_406734
                mov     ecx, [ebp+length__]
                sub     ecx, 1
                mov     [ebp+length__], ecx
                mov     edx, [ebp+length__]
                cmp     edx, [ebp+var_20]
                jge     short loc_4066FF
                mov     eax, [ebp+var_18]
                mov     [ebp+length__], eax
                jmp     short loc_406734


loc_4066FF:                             ; 
                mov     ecx, [ebp+length__]
                movzx   edx, yy_c_buf_p[ecx]
                mov     [ebp+yy_current_state], edx
                mov     eax, [ebp+yy_current_state]
                shr     eax, 3
                mov     ecx, [ebp+var_2C]
                movzx   ecx, byte ptr [ecx+eax]
                mov     eax, [ebp+yy_current_state]
                xor     edx, edx
                mov     esi, 8
                div     esi
                movzx   edx, byte_40FEEC[edx]
                and     ecx, edx
                jz      short loc_406732
                jmp     short loc_406734


loc_406732:                             ; 
                jmp     short loc_4066DD


loc_406734:                             ; 

                mov     eax, [ebp+length__]
                mov     oldlength, eax
                mov     ecx, oldend
                sub     ecx, oldlength
                mov     [ebp+var_30], ecx
                mov     edx, offset yy_c_buf_p
                sub     edx, [ebp+var_30]
                mov     yytext_ptr, edx
                cmp     [ebp+var_30], 0
                jbe     short loc_406780
                mov     eax, [ebp+var_30]
                push    eax
                mov     ecx, oldlength
                add     ecx, offset yytext
                push    ecx
                mov     edx, yytext_ptr
                push    edx
                call    ds:memmove
                add     esp, 0Ch

loc_406780:                             ; 
                mov     eax, oldlength
                mov     yytext[eax], 0
                mov     ecx, [ebp+var_24]
                movzx   edx, yy_accept_0[ecx*2]
                and     edx, 1FFh
                mov     [ebp+var_44], edx
                cmp     [ebp+var_44], 0Bh
                ja      def_4067AD
                mov     eax, [ebp+var_44]
                jmp     ds:jpt_4067AD[eax*4]
loc_4067B4:                             ; 
                call    get_error_text
                jmp     def_4067AD


loc_4067BE:                             ; 

                push    offset yytext
                call    alloc_copy
                add     esp, 4
                mov     dword_420858, eax
                mov     eax, 104h
                jmp     loc_40699B

                jmp     def_4067AD


loc_4067DF:                             ; 

                call    yyinput
                mov     [ebp+var_34], eax
                cmp     [ebp+var_34], 0Ah
                jnz     short loc_4067F9
                mov     eax, 105h
                jmp     loc_40699B

                jmp     short loc_406815


loc_4067F9:                             ; 
                push    offset yytext
                call    alloc_copy
                add     esp, 4
                mov     dword_420858, eax
                mov     eax, 103h
                jmp     loc_40699B


loc_406815:                             ; 
                jmp     short def_4067AD


loc_406817:                             ; 

                push    offset dword_420858
                push    offset aLd
                push    offset yytext
                call    sscanf
                add     esp, 0Ch
                mov     eax, 101h
                jmp     loc_40699B

                jmp     short def_4067AD


loc_40683B:                             ; 

                push    offset dword_420858
                push    offset aLf
                push    offset yytext
                call    sscanf
                add     esp, 0Ch
                mov     eax, 102h
                jmp     loc_40699B

                jmp     short def_4067AD


loc_40685F:                             ; 

                push    offset yytext
                call    alloc_copy
                add     esp, 4
                mov     dword_420858, eax
                mov     eax, 103h
                jmp     loc_40699B

                jmp     short def_4067AD

                jmp     short def_4067AD


loc_40687F:                             ; 

                movsx   eax, yytext
                jmp     loc_40699B


def_4067AD:                             ; 
                mov     ecx, offset yy_c_buf_p
                sub     ecx, yytext_ptr
                mov     [ebp+yyleng], ecx
                jz      short loc_4068BB
                mov     edx, [ebp+yyleng]
                push    edx
                mov     eax, yytext_ptr
                push    eax
                mov     ecx, oldlength
                add     ecx, offset yytext
                push    ecx
                call    ds:memmove
                add     esp, 0Ch

loc_4068BB:                             ; 
                mov     edx, oldlength
                add     edx, [ebp+yyleng]
                mov     oldend, edx
                mov     eax, oldlength
                mov     [ebp+length__], eax
                jmp     loc_406420

                mov     ecx, offset yy_c_buf_p
                sub     ecx, yytext_ptr
                mov     [ebp+var_3C], ecx
                jz      short loc_406907
                mov     edx, [ebp+var_3C]
                push    edx
                mov     eax, yytext_ptr
                push    eax
                mov     ecx, oldlength
                add     ecx, offset yytext
                push    ecx
                call    ds:memmove
                add     esp, 0Ch

loc_406907:                             ; 
                mov     edx, oldlength
                add     edx, [ebp+var_3C]
                mov     oldend, edx
                mov     eax, [ebp+var_18]
                mov     [ebp+length__], eax
                mov     ecx, [ebp+var_24]
                add     ecx, 1
                mov     [ebp+var_24], ecx
                mov     edx, [ebp+var_24]
                cmp     edx, [ebp+var_4]
                jge     short loc_406934
                jmp     loc_4066B2

                jmp     short loc_406939


loc_406934:                             ; 
                jmp     loc_406643


loc_406939:                             ; 
                mov     eax, offset yy_c_buf_p
                sub     eax, yytext_ptr
                mov     [ebp+var_40], eax
                jz      short loc_406968
                mov     ecx, [ebp+var_40]
                push    ecx
                mov     edx, yytext_ptr
                push    edx
                mov     eax, oldlength
                add     eax, offset yytext
                push    eax
                call    ds:memmove
                add     esp, 0Ch

loc_406968:                             ; 
                mov     ecx, oldlength
                add     ecx, [ebp+var_40]
                mov     oldend, ecx
                mov     edx, oldlength
                mov     [ebp+length__], edx
                cmp     [ebp+length__], 0
                jle     short loc_406996
                mov     eax, [ebp+length__]
                movsx   ecx, byte_42087F[eax]
                mov     oldlastc, ecx

loc_406996:                             ; 
                jmp     loc_4064A4


loc_40699B:                             ; 
                pop     esi
                mov     esp, ebp
                pop     ebp
                retn
                align 10h
jpt_4067AD      dd offset loc_4067B4 
                dd offset loc_4067BE
                dd offset loc_4067BE
                dd offset loc_4067BE
                dd offset loc_4067DF
                dd offset loc_406817
                dd offset loc_40683B
                dd offset loc_40683B
                dd offset loc_40683B
                dd offset loc_40685F
                dd offset def_4067AD
                dd offset loc_40687F

yylex           endp



failure         proc near    ; 
                cmp     ecx, ___security_cookie
                jnz     short $failure$26820
                retn
$failure$26820:                         ; 
                int 3;
failure endp

yyinput         proc near               ; 

var_C           = dword ptr -0Ch
var_8           = dword ptr -8
var_4           = dword ptr -4

                push    ebp
                mov     ebp, esp
                sub     esp, 0Ch
                mov     oldlength, 0

loc_406C90:                             ;
                call    input_c
                mov     [ebp+var_8], eax
                cmp     [ebp+var_8], 0FFFFFFFFh
                jnz     short loc_406CA3
                jmp     loc_406DCA


loc_406CA3:                             ; 
                mov     eax, [ebp+var_8]
                mov     [ebp+var_C], eax
                cmp     [ebp+var_C], 0Ah
                jz      loc_406D9A
                cmp     [ebp+var_C], 0Dh
                jz      loc_406D98
                cmp     [ebp+var_C], 5Ch
                jz      short loc_406CC8
                jmp     loc_406DA3


loc_406CC8:                             ; 
                call    input_c
                mov     [ebp+var_4], eax
                cmp     [ebp+var_4], 0Dh
                jnz     short loc_406CDB
                jmp     loc_406D96


loc_406CDB:                             ; 
                cmp     [ebp+var_4], 0Ah
                jnz     short loc_406CE6
                jmp     loc_406D96


loc_406CE6:                             ; 
                cmp     [ebp+var_4], 5Ch
                jnz     short loc_406D0D
                mov     ecx, oldlength
                mov     dl, byte ptr [ebp+var_4]
                mov     yytext[ecx], dl
                mov     eax, oldlength
                add     eax, 1
                mov     oldlength, eax
                jmp     loc_406D96


loc_406D0D:                             ; 
                cmp     [ebp+var_4], 6Eh
                jnz     short loc_406D31
                mov     ecx, oldlength
                mov     yytext[ecx], 0Ah
                mov     edx, oldlength
                add     edx, 1
                mov     oldlength, edx
                jmp     short loc_406D96


loc_406D31:                             ; 
                cmp     [ebp+var_4], 22h
                jnz     short loc_406D56
                mov     eax, oldlength
                mov     cl, byte ptr [ebp+var_4]
                mov     yytext[eax], cl
                mov     edx, oldlength
                add     edx, 1
                mov     oldlength, edx
                jmp     short loc_406D96


loc_406D56:                             ; 
                mov     eax, oldlength
                mov     cl, byte ptr [ebp+var_8]
                mov     yytext[eax], cl
                mov     edx, oldlength
                add     edx, 1
                mov     oldlength, edx
                cmp     [ebp+var_4], 0FFFFFFFFh
                jz      short loc_406D96
                mov     eax, oldlength
                mov     cl, byte ptr [ebp+var_4]
                mov     yytext[eax], cl
                mov     edx, oldlength
                add     edx, 1
                mov     oldlength, edx

loc_406D96:                             ; 
                jmp     short loc_406DC0


loc_406D98:                             ; 
                jmp     short loc_406DC0


loc_406D9A:                             ; 
                mov     eax, 0Ah
                jmp     short loc_406DEF

                jmp     short loc_406DC0


loc_406DA3:                             ; 
                mov     eax, oldlength
                mov     cl, byte ptr [ebp+var_8]
                mov     yytext[eax], cl
                mov     edx, oldlength
                add     edx, 1
                mov     oldlength, edx

loc_406DC0:                             ; 
                cmp     [ebp+var_8], 22h
                jnz     loc_406C90

loc_406DCA:                             ; 
                cmp     oldlength, 0
                jle     short loc_406DE0
                mov     eax, oldlength
                sub     eax, 1
                mov     oldlength, eax

loc_406DE0:                             ; 
                mov     ecx, oldlength
                mov     yytext[ecx], 0
                xor     eax, eax

loc_406DEF:                             ; 
                mov     esp, ebp
                pop     ebp
                retn
yyinput         endp


input_c         proc near               ; 

cc               = dword ptr -4

                push    ebp
                mov     ebp, esp
                push    ecx
                cmp     yytext_ptr, offset yy_c_buf_p
                jnb     short loc_406BCC
                mov     eax, yytext_ptr
                movsx   ecx, byte ptr [eax]
                mov     [ebp+cc], ecx
                mov     edx, yytext_ptr
                add     edx, 1
                mov     yytext_ptr, edx
                jmp     short loc_406BDE


loc_406BCC:                             ; 
                mov     eax, yyin
                push    eax
                call    ds:getc
                add     esp, 4
                mov     [ebp+cc], eax

loc_406BDE:                             ; 
                mov     ecx, [ebp+cc]
                mov     oldlastc, ecx
                cmp     [ebp+cc], 0Ah
                jnz     short loc_406BFC
                mov     edx, oldline
                add     edx, 1
                mov     oldline, edx

loc_406BFC:                             ; 
                mov     eax, [ebp+cc]
                mov     esp, ebp
                pop     ebp
                retn
input_c         endp


get_error_text  proc near               ; 

String          = dword ptr -3F4h
Destination     = byte ptr -3F0h
var_4           = dword ptr -4

                push    ebp
                mov     ebp, esp
                sub     esp, 3F4h
                mov     eax, ___security_cookie
                xor     eax, ebp
                mov     [ebp+var_4], eax
                push    offset yytext
                lea     eax, [ebp+Destination]
                push    eax
                call    strcpy
                add     esp, 8
                cmp     str_error_texts, 0
                jz      short loc_40771F
                mov     ecx, str_error_texts
                push    ecx
                call    free
                add     esp, 4

loc_40771F:                             ; 
                push    1
                mov     edx, oldlength
                push    edx
                call    calloc
                add     esp, 8
                mov     str_error_texts, eax
                cmp     str_error_texts, 0
                jz      short loc_407796
                push    offset asc_410178
                lea     eax, [ebp+Destination]
                push    eax
                call    strtok
                add     esp, 8
                mov     [ebp+String], eax
                mov     ecx, [ebp+String]
                push    ecx
                call    atoi
                add     esp, 4
                mov     oldline, eax
                push    offset asc_41017C
                push    0
                call    strtok
                add     esp, 8
                mov     [ebp+String], eax
                mov     edx, [ebp+String]
                push    edx
                mov     eax, str_error_texts
                push    eax
                call    strcpy
                add     esp, 8

loc_407796:                             ; 
                mov     ecx, [ebp+var_4]
                xor     ecx, ebp
                call    failure
                mov     esp, ebp
                pop     ebp
                retn
get_error_text  endp



yyoverflow      proc near               ; 

Buffer          = dword ptr  8

                push    ebp
                mov     ebp, esp
                cmp     fp_out, 0
                jnz     short loc_4077CA
                call    ds:__iob_func
                add     eax, 40h
                mov     fp_out, eax

loc_4077CA:                             ; 
                mov     eax, yy_error_no
                add     eax, 1
                push    eax
                push    offset aErrorD
                mov     ecx, fp_out
                push    ecx
                call    ds:fprintf
                add     esp, 0Ch
                call    yyerror
                mov     edx, fp_out
                push    edx
                mov     eax, [ebp+Buffer]
                push    eax
                call    ds:fputs
                add     esp, 8
                mov     ecx, fp_out
                push    ecx
                push    0Ah
                call    ds:putc
                add     esp, 8
                pop     ebp
                retn
yyoverflow      endp



yyerror         proc near               ; 

var_14          = dword ptr -14h
var_10          = dword ptr -10h
var_C           = dword ptr -0Ch
Str1            = dword ptr -8
var_1           = byte ptr -1

                push    ebp
                mov     ebp, esp
                sub     esp, 14h
                mov     [ebp+var_1], 0
                cmp     str_error_texts, 0
                jz      loc_4075C4
                mov     eax, str_error_texts
                movsx   ecx, byte ptr [eax]
                test    ecx, ecx
                jz      loc_4075C4
                push    offset asc_410148
                mov     edx, str_error_texts
                push    edx
                call    strcmp
                add     esp, 8
                test    eax, eax
                jz      loc_4075C4
                mov     eax, str_error_texts
                mov     [ebp+Str1], eax
                mov     ecx, str_error_texts
                push    ecx
                call    strlen
                add     esp, 4
                mov     [ebp+var_C], eax
                mov     edx, [ebp+Str1]
                movsx   eax, byte ptr [edx]
                cmp     eax, 22h
                jnz     short loc_40757A
                mov     ecx, [ebp+Str1]
                add     ecx, 1
                mov     [ebp+Str1], ecx
                mov     edx, [ebp+var_C]
                sub     edx, 2
                mov     [ebp+var_C], edx

loc_40757A:                             ; 
                push    2
                push    offset asc_41014C
                mov     eax, [ebp+Str1]
                push    eax
                call    strncmp
                add     esp, 0Ch
                test    eax, eax
                jnz     short loc_4075A3
                mov     ecx, [ebp+Str1]
                add     ecx, 2
                mov     [ebp+Str1], ecx
                mov     edx, [ebp+var_C]
                sub     edx, 2
                mov     [ebp+var_C], edx

loc_4075A3:                             ; 
                mov     eax, [ebp+Str1]
                push    eax
                mov     ecx, [ebp+var_C]
                push    ecx
                push    offset aFileS
                mov     edx, fp_out
                push    edx
                call    ds:fprintf
                add     esp, 10h
                mov     [ebp+var_1], 1

loc_4075C4:                             ; 

                cmp     oldline, 0
                jle     short loc_407634
                movsx   eax, [ebp+var_1]
                test    eax, eax
                jz      short loc_4075EA
                mov     ecx, fp_out
                push    ecx
                push    offset asc_41015C
                call    ds:fputs
                add     esp, 8

loc_4075EA:                             ; 
                movsx   edx, yytext
                cmp     edx, 0Ah
                jz      short loc_40760A
                movsx   eax, yytext
                test    eax, eax
                jz      short loc_40760A
                mov     [ebp+var_14], 0
                jmp     short loc_407611


loc_40760A:                             ; 

                mov     [ebp+var_14], 1

loc_407611:                             ; 
                mov     ecx, oldline
                sub     ecx, [ebp+var_14]
                push    ecx
                push    offset aLineD
                mov     edx, fp_out
                push    edx
                call    ds:fprintf
                add     esp, 0Ch
                mov     [ebp+var_1], 1

loc_407634:                             ; 
                movsx   eax, yytext
                test    eax, eax
                jz      short loc_4076B9
                mov     [ebp+var_10], 0
                jmp     short loc_407651


loc_407648:                             ; 
                mov     ecx, [ebp+var_10]
                add     ecx, 1
                mov     [ebp+var_10], ecx

loc_407651:                             ; 
                cmp     [ebp+var_10], 14h
                jge     short loc_407678
                mov     edx, [ebp+var_10]
                movsx   eax, yytext[edx]
                test    eax, eax
                jz      short loc_407674
                mov     ecx, [ebp+var_10]
                movsx   edx, yytext[ecx]
                cmp     edx, 0Ah
                jnz     short loc_407676

loc_407674:                             ; 
                jmp     short loc_407678


loc_407676:                             ; 
                jmp     short loc_407648


loc_407678:                             ; 

                cmp     [ebp+var_10], 0
                jz      short loc_4076B9
                movsx   eax, [ebp+var_1]
                test    eax, eax
                jz      short loc_407698
                mov     ecx, fp_out
                push    ecx
                push    20h
                call    ds:putc
                add     esp, 8

loc_407698:                             ; 
                push    offset yytext
                mov     edx, [ebp+var_10]
                push    edx
                push    offset aNearS
                mov     eax, fp_out
                push    eax
                call    ds:fprintf
                add     esp, 10h
                mov     [ebp+var_1], 1

loc_4076B9:                             ; 

                movsx   ecx, [ebp+var_1]
                test    ecx, ecx
                jz      short loc_4076D6
                mov     edx, fp_out
                push    edx
                push    offset asc_410174
                call    ds:fputs
                add     esp, 8

loc_4076D6:                             ; 
                mov     esp, ebp
                pop     ebp
                retn
yyerror         endp



sub_407820      proc near               ; 
                push    ebp
                mov     ebp, esp
                cmp     fp_out, 0
                jnz     short loc_40783A
                call    ds:__iob_func
                add     eax, 40h
                mov     fp_out, eax

loc_40783A:                             ; 
                mov     eax, yy_error_no
                add     eax, 1
                mov     yy_error_no, eax
                mov     ecx, yy_error_no
                push    ecx
                push    offset aErrorD_0
                mov     edx, fp_out
                push    edx
                call    ds:fprintf
                add     esp, 0Ch
                call    yyerror
                mov     eax, fp_out
                push    eax
                push    offset aUnexpectedNewl
                call    ds:fputs
                add     esp, 8
                mov     ecx, fp_out
                push    ecx
                push    0Ah
                call    ds:putc
                add     esp, 8
                pop     ebp
                retn
sub_407820      endp


yyparse         proc near               ; 

var_164         = qword ptr -164h
var_15C         = dword ptr -15Ch
var_158         = dword ptr -158h
var_154         = dword ptr -154h
yy_act_1        = word ptr -150h
var_14C         = dword ptr -14Ch
var_148         = word ptr -148h
var_144         = dword ptr -144h
var_140         = word ptr -140h
var_14          = byte ptr -14h
var_10          = dword ptr -10h
var_C           = dword ptr -0Ch
var_8           = dword ptr -8
var_4           = dword ptr -4

                push    ebp
                mov     ebp, esp
                sub     esp, 15Ch
                mov     eax, ___security_cookie
                xor     eax, ebp
                mov     [ebp+var_10], eax
                mov     yy_error_no, 0
                mov     dword_420854, 0
                mov     yychar, 0FFFFFFFFh
                lea     eax, [ebp+var_140]
                mov     [ebp+var_8], eax
                mov     [ebp+var_C], offset dword_4193E8
                mov     [ebp+var_148], 0Ch
                mov     ecx, [ebp+var_8]
                mov     dx, [ebp+var_148]
                mov     [ecx], dx

loc_406E67:                             ; 

                mov     eax, [ebp+var_8]
                add     eax, 2
                mov     [ebp+var_8], eax
                lea     ecx, [ebp+var_14]
                cmp     [ebp+var_8], ecx
                jbe     short loc_406E8F
                push    offset aParserStackOve
                call    yyoverflow
                add     esp, 4
                mov     eax, 1
                jmp     loc_407490


loc_406E8F:                             ; 
                mov     edx, [ebp+var_8]
                mov     ax, [ebp+var_148]
                mov     [edx], ax
                mov     ecx, [ebp+var_C]
                add     ecx, 8
                mov     [ebp+var_C], ecx
                mov     edx, [ebp+var_C]
                mov     eax, dword_420860
                mov     [edx], eax
                mov     ecx, dword_420864
                mov     [edx+4], ecx

loc_406EB8:                             ; 

                movsx   edx, [ebp+var_148]
                cmp     edx, 0Dh
                jb      short loc_406EDA
                movsx   eax, [ebp+var_148]
                sub     eax, 7
                mov     [ebp+yy_act_1], ax
                jmp     loc_4070BA


loc_406EDA:                             ; 
                movsx   ecx, [ebp+var_148]
                movsx   edx, word_410034[ecx*2]
                lea     eax, (yy_accept+44h)[edx*2]
                mov     [ebp+var_144], eax
                mov     ecx, [ebp+var_144]
                movsx   edx, word ptr [ecx]
                test    edx, edx
                jl      loc_406FE9
                cmp     yychar, 0
                jge     short loc_406F40
                call    yylex
                mov     yychar, eax
                cmp     yychar, 0
                jge     short loc_406F40
                cmp     yychar, 0FFFFFFFEh
                jnz     short loc_406F36
                mov     eax, 1
                jmp     loc_407490


loc_406F36:                             ; 
                mov     yychar, 0

loc_406F40:                             ; 

                mov     eax, [ebp+var_144]
                mov     [ebp+var_4], eax
                mov     cx, word ptr yychar
                mov     [ebp+yy_act_1], cx

loc_406F57:                             ; 
                mov     edx, [ebp+var_144]
                movsx   eax, word ptr [edx]
                movsx   ecx, [ebp+yy_act_1]
                mov     edx, [ebp+var_144]
                add     edx, 2
                mov     [ebp+var_144], edx
                cmp     ecx, eax
                jge     short loc_406F7C
                jmp     short loc_406F57


loc_406F7C:                             ; 
                movsx   eax, [ebp+yy_act_1]
                mov     ecx, [ebp+var_144]
                movsx   edx, word ptr [ecx-2]
                cmp     eax, edx
                jnz     short loc_406FE9
                mov     eax, [ebp+var_4]
                sub     eax, [ebp+var_144]
                sar     eax, 1
                mov     ecx, [ebp+var_4]
                movsx   edx, word ptr [ecx+eax*2]
                not     edx
                mov     [ebp+var_148], dx
                mov     eax, dword_420858
                mov     dword_420860, eax
                mov     ecx, dword_42085C
                mov     dword_420864, ecx
                mov     yychar, 0FFFFFFFFh
                cmp     dword_420854, 0
                jz      short loc_406FE4
                mov     edx, dword_420854
                sub     edx, 1
                mov     dword_420854, edx

loc_406FE4:                             ; 
                jmp     loc_406E67


loc_406FE9:                             ; 

                movsx   eax, [ebp+var_148]
                cmp     eax, 7
                jb      short loc_406FFA
                jmp     loc_407349


loc_406FFA:                             ; 
                movsx   ecx, [ebp+var_148]
                mov     dx, yy_accept[ecx*2]
                mov     [ebp+yy_act_1], dx
                movsx   eax, [ebp+yy_act_1]
                test    eax, eax
                jge     loc_4070BA
                movsx   ecx, [ebp+yy_act_1]
                not     ecx
                lea     edx, (yy_accept+10h)[ecx*2]
                mov     [ebp+var_144], edx
                cmp     yychar, 0
                jge     short loc_40706E
                call    yylex
                mov     yychar, eax
                cmp     yychar, 0
                jge     short loc_40706E
                cmp     yychar, 0FFFFFFFEh
                jnz     short loc_407064
                mov     eax, 1
                jmp     loc_407490


loc_407064:                             ; 
                mov     yychar, 0

loc_40706E:                             ; 

                mov     eax, [ebp+var_144]
                mov     cx, [eax]
                mov     [ebp+yy_act_1], cx
                movsx   edx, [ebp+yy_act_1]
                test    edx, edx
                jl      short loc_4070A9
                movsx   eax, [ebp+yy_act_1]
                cmp     eax, yychar
                jz      short loc_4070A9
                mov     ecx, [ebp+var_144]
                add     ecx, 4
                mov     [ebp+var_144], ecx
                jmp     short loc_40706E


loc_4070A9:                             ; 

                mov     edx, [ebp+var_144]
                mov     ax, [edx+2]
                mov     [ebp+yy_act_1], ax

loc_4070BA:                             ; 

                movsx   ecx, [ebp+yy_act_1]
                movsx   edx, word_4100D8[ecx*2]
                mov     [ebp+var_14C], edx
                mov     eax, [ebp+var_14C]
                shl     eax, 1
                mov     ecx, [ebp+var_8]
                sub     ecx, eax
                mov     [ebp+var_8], ecx
                mov     edx, [ebp+var_C]
                mov     dword_420870, edx
                mov     eax, [ebp+var_14C]
                shl     eax, 3
                mov     ecx, [ebp+var_C]
                sub     ecx, eax
                mov     [ebp+var_C], ecx
                mov     edx, [ebp+var_C]
                mov     eax, [edx+8]
                mov     ecx, [edx+0Ch]
                mov     dword_420860, eax
                mov     dword_420864, ecx
                movsx   edx, [ebp+yy_act_1]
                mov     [ebp+var_154], edx
                cmp     [ebp+var_154], 13h
                ja      def_40712D
                mov     eax, [ebp+var_154]
                jmp     ds:jpt_40712D[eax*4]


loc_407134:                             ; 

                mov     dword_420854, 0
                jmp     def_40712D


loc_407143:                             ; 

                mov     ecx, dword_420870
                mov     edx, [ecx-8]
                push    edx
                call    sub_404F80
                add     esp, 4
                jmp     def_40712D


loc_40715A:                             ; 

                mov     eax, s_level
                sub     eax, 1
                mov     s_level, eax
                jmp     def_40712D


loc_40716C:                             ; 

                mov     ecx, dword_420870
                mov     edx, [ecx-8]
                push    edx
                call    sub_404F80
                add     esp, 4
                jmp     def_40712D


loc_407183:                             ; 

                mov     eax, s_level
                sub     eax, 1
                mov     s_level, eax
                jmp     def_40712D


loc_407195:                             ; 

                mov     ecx, dword_420870
                mov     edx, [ecx]
                push    edx
                call    sub_405020
                add     esp, 4
                jmp     def_40712D


loc_4071AB:                             ; 

                mov     dword_420854, 0
                jmp     def_40712D


loc_4071BA:                             ; 

                mov     dword_420854, 0
                jmp     def_40712D


loc_4071C9:                             ; 

                mov     dword_420854, 0
                jmp     def_40712D


loc_4071D8:                             ; 

                mov     dword_420854, 0
                jmp     short def_40712D


loc_4071E4:                             ; 

                mov     eax, dword_420870
                mov     ecx, [eax]
                push    ecx
                call    sub_405060
                add     esp, 4
                jmp     short def_40712D


loc_4071F6:                             ; 

                mov     edx, dword_420870
                mov     eax, [edx]
                push    eax
                call    sub_405060
                add     esp, 4
                jmp     short def_40712D


loc_407209:                             ; 

                mov     ecx, dword_420870
                mov     edx, [ecx]
                push    edx
                call    sub_4050C0
                add     esp, 4
                jmp     short def_40712D


loc_40721C:                             ; 

                mov     eax, dword_420870
                sub     esp, 8
                fld     qword ptr [eax]
                fstp    [esp+164h+var_164]
                call    sub_405120
                add     esp, 8
                jmp     short def_40712D


loc_407233:                             ; 

                call    sub_407820
                jmp     short def_40712D


loc_40723A:                             ; 

                mov     dword_420854, 0
                jmp     short def_40712D


loc_407246:                             ; 

                mov     dword_420854, 0
                jmp     short def_40712D


loc_407252:                             ; 

                xor     eax, eax
                jmp     loc_407490


loc_407259:                             ; 

                jmp     loc_407349


def_40712D:                             ; 

                movsx   ecx, [ebp+yy_act_1]
                movsx   edx, word_410090[ecx*2]
                lea     eax, word_410050[edx*2]
                mov     [ebp+var_144], eax
                mov     ecx, [ebp+var_144]
                mov     [ebp+var_4], ecx
                mov     edx, [ebp+var_144]
                add     edx, 2
                mov     [ebp+var_144], edx
                mov     eax, [ebp+var_8]
                mov     cx, [eax]
                mov     [ebp+yy_act_1], cx

loc_40729F:                             ; 
                mov     edx, [ebp+var_144]
                movsx   eax, word ptr [edx]
                movsx   ecx, [ebp+yy_act_1]
                mov     edx, [ebp+var_144]
                add     edx, 2
                mov     [ebp+var_144], edx
                cmp     ecx, eax
                jge     short loc_4072C4
                jmp     short loc_40729F


loc_4072C4:                             ; 
                mov     eax, [ebp+var_144]
                sub     eax, 2
                mov     [ebp+var_144], eax
                mov     ecx, [ebp+var_144]
                movsx   edx, word ptr [ecx]
                movsx   eax, [ebp+yy_act_1]
                cmp     eax, edx
                jnz     short loc_407301
                mov     ecx, [ebp+var_4]
                sub     ecx, [ebp+var_144]
                sar     ecx, 1
                mov     edx, [ebp+var_4]
                movsx   eax, word ptr [edx+ecx*2]
                mov     [ebp+var_158], eax
                jmp     short loc_40730D


loc_407301:                             ; 
                mov     ecx, [ebp+var_4]
                movsx   edx, word ptr [ecx]
                mov     [ebp+var_158], edx

loc_40730D:                             ; 
                mov     eax, [ebp+var_158]
                not     eax
                mov     [ebp+var_148], ax
                jmp     loc_406E67

                mov     dword_420854, 1
                movsx   ecx, [ebp+yy_act_1]
                cmp     ecx, 1
                jnz     short loc_407349
                mov     edx, [ebp+var_8]
                sub     edx, 2
                mov     [ebp+var_8], edx
                mov     eax, [ebp+var_C]
                sub     eax, 8
                mov     [ebp+var_C], eax

loc_407349:                             ; 

                mov     ecx, dword_420854
                mov     [ebp+var_15C], ecx
                cmp     [ebp+var_15C], 3
                ja      def_407368
                mov     edx, [ebp+var_15C]
                jmp     ds:jpt_407368[edx*4]


loc_40736F:                             ; 

                mov     eax, yy_error_no
                add     eax, 1
                mov     yy_error_no, eax
                mov     cx, word ptr yychar
                mov     [ebp+yy_act_1], cx
                push    offset aSyntaxError
                call    yyoverflow
                add     esp, 4
                movsx   edx, [ebp+yy_act_1]
                cmp     edx, yychar
                jz      short loc_4073B8
                mov     eax, dword_420854
                add     eax, 1
                mov     dword_420854, eax
                jmp     loc_406EB8


loc_4073B8:                             ; 


                mov     dword_420854, 3
                jmp     short loc_4073D6


loc_4073C4:                             ; 

                mov     ecx, [ebp+var_8]
                sub     ecx, 2
                mov     [ebp+var_8], ecx
                mov     edx, [ebp+var_C]
                sub     edx, 8
                mov     [ebp+var_C], edx

loc_4073D6:                             ; 
                mov     eax, [ebp+var_8]
                lea     ecx, [ebp+var_140]
                cmp     eax, ecx
                jbe     loc_40746F
                mov     edx, [ebp+var_8]
                movsx   eax, word ptr [edx]
                cmp     eax, 0Dh
                jb      short loc_4073F4
                jmp     short loc_4073C4


loc_4073F4:                             ; 
                mov     ecx, [ebp+var_8]
                movsx   edx, word ptr [ecx]
                movsx   eax, word_410034[edx*2]
                lea     ecx, (yy_accept+44h)[eax*2]
                mov     [ebp+var_144], ecx
                mov     edx, [ebp+var_144]
                mov     [ebp+var_4], edx

loc_407418:                             ; 
                mov     eax, [ebp+var_144]
                movsx   ecx, word ptr [eax]
                mov     edx, [ebp+var_144]
                add     edx, 2
                mov     [ebp+var_144], edx
                cmp     ecx, 100h
                jg      short loc_407418
                mov     eax, [ebp+var_144]
                movsx   ecx, word ptr [eax-2]
                cmp     ecx, 100h
                jnz     short loc_40746A
                mov     edx, [ebp+var_4]
                sub     edx, [ebp+var_144]
                sar     edx, 1
                mov     eax, [ebp+var_4]
                movsx   ecx, word ptr [eax+edx*2]
                not     ecx
                mov     [ebp+var_148], cx
                jmp     loc_406E67


loc_40746A:                             ; 
                jmp     loc_4073C4


loc_40746F:                             ; 
                jmp     short def_407368


loc_407471:                             ; 

                cmp     yychar, 0
                jnz     short loc_40747C
                jmp     short def_407368


loc_40747C:                             ; 
                mov     yychar, 0FFFFFFFFh
                jmp     loc_406EB8


def_407368:                             ; 

                mov     eax, 1

loc_407490:                             ; 

                mov     ecx, [ebp+var_10]
                xor     ecx, ebp
                call    failure
                mov     esp, ebp
                pop     ebp
                retn
                align 10h
jpt_40712D      dd offset loc_407252    ; 
                dd offset loc_407259
                dd offset def_40712D
                dd offset loc_407143
                dd offset loc_40716C
                dd offset loc_407195
                dd offset loc_407246
                dd offset loc_40723A
                dd offset loc_407233
                dd offset loc_40721C
                dd offset loc_407209
                dd offset loc_4071F6
                dd offset loc_4071E4
                dd offset loc_4071D8
                dd offset loc_4071C9
                dd offset loc_4071BA
                dd offset loc_4071AB
                dd offset loc_407183
                dd offset loc_40715A
                dd offset loc_407134
jpt_407368      dd offset loc_40736F    ; 
                dd offset loc_4073B8
                dd offset loc_4073B8
                dd offset loc_407471

yyparse         endp



end
