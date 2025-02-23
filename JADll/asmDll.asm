.CODE
MyProc1 proc
    ;todo popraw zeby nie bylo zapisaywania i odwolywania sie do pamieci

; zapisanie rejestr�w na stosie
    push rbx
    push rbp
    push rdi
    push rsi
    push r12
    push r13
    push r14
    push r15
    
    ; �adowanie argument�w do rejestr�w
    mov r10, rcx               ; r10 = resultMatrix
    mov r11, rdx               ; r11 = matrixA
    mov r12, r8                ; r12 = matrixB
    mov r13, r9                ; r13 = =rows
    mov r14, [rsp+104]          ; r14 = columns
    mov r15, [rsp+112]          ; r15 = start row index
    mov rbx, [rsp+120]          ; rbx = end row index
    sub rsp, 32                 ;wyr�wnanie stosu



;program zaaczyna si� tutaj
;obliczanie ile razy wykona� petle ktora przechodzi przez kolumny
    mov rax, r14      
    mov rbx, 4      
    xor rdx, rdx    ; Wyczy�� rejestr rdx (wymagane przed operacj� div)

    div rbx         ; rax = rax / rbx, rdx = rax % rbx

    cmp rdx, 0      ; Sprawd� czy reszta z dzielenia jest r�wna 0
    je evenColumns  ; Je�li tak, to liczba kolumn jest podzielna przez 4

    add rax, 1  ;je�li nie to dodaj 1 do wyniku
    
evenColumns:
    mov rdi, rax   ;wynik ile razy wykona� p�tle kolumn rdi  = (kolumny / 4) je�li jest reszta to plus 1
    
    mov rsi, rdi
    



rowLoop:

    mov rax, r15
    shl rax, 3
    mov rbx, r14
    mul rbx
    mov [rsp + 104], rax ;pierwsza cz�� obliczania adresu przesuni�cia wiersza
    
    mov r9, 0  ;ustaw r9 jako iterator kolumn
    

columnLoop:


    mov rdi, 0  ;ustaw rdi jako iterator kolumny


    mov rax, r15
    shl rax, 3
    mov rbx, r13
    mul rbx
    mov rbx, rax
    mov rax, r9
    shl rax, 3
    add rbx, rax
    mov r8, rbx  ;r8 to przesuni�cie adresu uzywane w zapisie wyniku

    mov rax, r9
    push r9
    shl rax, 3
    mov rbx, r13
    mul rbx
    ;mov [rsp + 112], rax
    mov r9, rax


operationLoop:

    
    mov rax, rdi
    shl rax, 5
    mov rcx, [rsp + 112] ;rsp 104 + 8 bo jeden push to 8 bajt�w
    add rcx, rax
    ;rcx to przesuni�cie adresu wiersza

    
    mov rax, rdi
    shl rax, 5
    ;mov rdx, [rsp + 112] ; to jest bez pusha 
    mov rdx, r9
    add rdx, rax
    ;rdx to przesuni�cie adresu kolumny



   
    vmovupd ymm0, [r11 + rcx] ;za�aduj matrixA do ymm0

    vmovupd ymm1, [r12 + rdx]  ;za�aduj matrixB do ymm1   

    vmulpd ymm0, ymm1, ymm0 ; Pomn� elementy ymm0 przez elementy ymm1

    vperm2f128 ymm1, ymm0, ymm0, 1  ; Przestaw elementy w ymm0
    
    vaddpd ymm0, ymm0, ymm1  ;dodaj przestawione elementy do siebie      

    vhaddpd ymm0, ymm0, ymm0  ;dodaj elementy w pary, to da nam w ka�dym miejscu w ymm0 wynik

    vaddpd ymm2, ymm2, ymm0  ;zakumuluj wynik w ymm2

    
    inc rdi
    cmp rdi, rsi
    jl operationLoop

continueLoop:

    pextrq rax, xmm2, 1 ; Wyodr�bnij drugi element (nie ma r�nicy kt�ry bo s� takie same) z xmm1 do rax, obliczyli�my pierwszy element wyniku     
    mov [r10 + r8], rax ;za�aduj wynik do macierzy wynikowej  ;przesuni�te o 8 razy iteracja po �rodkowej p�tli

    vxorps ymm2, ymm2, ymm2 ;wyczy�� ymm2
    pop r9
 	inc r9
    cmp r9, r14

    jl columnLoop


checkIfEnd:

   mov rbx,[rsp + 152];[rsp + 152] miejsce end row w pami�ci
   dec rbx
   mov rax, r15  ;wpisz obecny wiersz do rax
   cmp rax, rbx;[rsp + 152] miejsce end row w pami�ci  por�wnaj obecny wiersz z ko�cowym endRow
   jge endProcedure ;je�li obecny wiersz jest wi�kszy lub r�wny ko�cowemu to zako�cz program
   inc r15      ;je�li nie to zwi�ksz obecny wiersz
   jmp rowLoop  ;i wr�� do p�tli zewn�trznej

endProcedure:
   
   vzeroall

   pop r15
   pop r14
   pop r13
   pop r12
   pop rsi
   pop rdi
   pop rbp
   pop rbx
   
   add rsp, 32
   
   ret

MyProc1 endp
END