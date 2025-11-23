INCLUDE Irvine32.inc

typEntry struc
    date db 0
    month db 0
    year db 0
    hour db 0
    min db 0
    second db 0
    message db 93 dup(?)
typEntry ends

.DATA

filename    BYTE "test.bin", 0
fileError BYTE "Error: Could not open file!", 0
ReminderReached BYTE "Reminder! ",0
pressAnyKeyMsg BYTE "Press any key to return to main menu....",0

reminderAdded byte "Reminder Added!",0
LoadNextReminder byte "Next reminder",0
ReminderReachedMSG byte "Reminder has been reached",0
menuText BYTE "1. Add Reminder",13,10,
              "2. View Next Reminder",13,10,
              "3. Reminder Reached",13,10,
              "4. Default Timer / Check Reminders",13,10,
              "5. Exit",13,10,
              "Enter choice: ",13,10,0
choice BYTE ?
msg BYTE "here",0


date BYTE ?
month BYTE ?
year WORD ?
hour BYTE ?
min BYTE ?
second BYTE ?
message BYTE 93 dup(?)


bytesWritten DWORD ?
bytesRead DWORD ?
buffer BYTE 1000 dup(?)
entry BYTE 100 DUP(?)


promptDate      BYTE "Enter date (1-31): ",0
promptMonth     BYTE "Enter month (1-12): ",0
promptYear      BYTE "Enter year (e.g. 2024): ",0
promptHour      BYTE "Enter hour (0-23): ",0
promptMinute    BYTE "Enter minute (0-59): ",0
promptSecond    BYTE "Enter second (0-59): ",0
promptMessage   BYTE "Enter message: ",0


timeLabel BYTE "Time: ",0
dateLabel BYTE "Date: ",0
msgLabel  BYTE "Message: ",0


colon     BYTE ":",0
slash     BYTE "/",0

; Windows constants
GENERIC_WRITE       equ 40000000h
GENERIC_READ        equ 80000000h
FILE_SHARE_READ     equ 1
OPEN_ALWAYS         equ 4
FILE_ATTRIBUTE_NORMAL equ 80h
FILE_END            equ 2

currentTime SYSTEMTIME <>

.CODE
main PROC

    ; External function declarations (from kernel32.dll)
    EXTERN CreateFileA@28:PROC
    EXTERN WriteFile@20:PROC
    EXTERN ReadFile@20:PROC
    EXTERN CloseHandle@4:PROC
    EXTERN SetFilePointer@16:PROC

    call Menu
    
    exit

main ENDP

MENU proc

MenuLoop:

call clrscr
mov edx,offset menutext
call writestring

call readchar
sub al,'0'
mov choice,al

cmp choice,1
je AddReminder

cmp choice,2
je readReminder

cmp choice,3
je ReminderDone

cmp choice,4
je DefaultTimer

cmp choice,5
je exitProgram

jmp Menuloop

AddReminder:
call SAVE_REMINDER 
jmp menuloop

readReminder:
call LOAD_REMINDER 
jmp menuloop

ReminderDone:
call NEXT_REMINDER 
jmp menuloop

DefaultTimer:
call COMPARE_TIME
jmp menuloop


exitProgram:
exit

MENU endp

CLEAR_REGS PROC
mov date, 0
    mov month, 0
    mov year, 0
    mov hour, 0
    mov min, 0
    mov second, 0
    mov ecx, 93
    mov edi, offset message
    xor eax,eax
clear_msg:
    mov [edi], al
    inc edi
    loop clear_msg
ret 
CLEAR_REGS endp

LOAD_REMINDER proc

    INVOKE CreateFileA, ADDR filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
    mov ebx, eax       
    
    cmp ebx, INVALID_HANDLE_VALUE
    je  file_open_failed


    INVOKE ReadFile, ebx, ADDR date, SIZEOF date, ADDR bytesRead, 0
    INVOKE ReadFile, ebx, ADDR month, SIZEOF month, ADDR bytesRead, 0
    INVOKE ReadFile, ebx, ADDR year, SIZEOF year, ADDR bytesRead, 0
    INVOKE ReadFile, ebx, ADDR hour, SIZEOF hour, ADDR bytesRead, 0
    INVOKE ReadFile, ebx, ADDR min, SIZEOF min, ADDR bytesRead, 0
    INVOKE ReadFile, ebx, ADDR second, SIZEOF second, ADDR bytesRead, 0
    INVOKE ReadFile, ebx, ADDR message, SIZEOF message, ADDR bytesRead, 0
    INVOKE CloseHandle, ebx
    call DisplayRecord
    jmp returning

    file_open_failed:
    mov edx, OFFSET fileError
    call WriteString



    returning:
    call crlf
    mov edx,offset loadnextReminder
    call writestring
    call crlf
    mov edx, OFFSET pressAnyKeyMsg
    call WriteString
    call ReadChar   

    ret


LOAD_REMINDER endp


DisplayRecord PROC
    call crlf
    call crlf
    
    ; --- Display Time ---
    mov  edx, OFFSET timeLabel
    call WriteString
    movzx eax, hour
    call WriteDec
    mov  edx, OFFSET colon
    call WriteString
    movzx eax, min
    call WriteDec
    mov  edx, OFFSET colon
    call WriteString
    movzx eax, second
    call WriteDec
    call Crlf

    ; --- Display Date ---
    mov  edx, OFFSET dateLabel
    call WriteString
    movzx eax, date
    call WriteDec
    mov  edx, OFFSET slash
    call WriteString
    movzx eax, month
    call WriteDec
    mov  edx, OFFSET slash
    call WriteString
    movzx eax, year
    call WriteDec
    call Crlf

    ; --- Display Message ---
    mov  edx, OFFSET msgLabel
    call WriteString
    mov  edx, OFFSET message
    call WriteString
    call Crlf


    ret
DisplayRecord ENDP

SAVE_REMINDER PROC
    
    LOCAL userInput:typEntry localBuffer:typEntry

    ;POPULATE userInput
    mov esi, OFFSET userInput
    call CONSOLE_TO_OBJ

    ;OPEN FOR READING
    INVOKE CreateFileA, ADDR filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
    mov ebx, eax
    cmp ebx, INVALID_HANDLE_VALUE
    je save_open_failed
    
    ;READ TO BUFFER
    INVOKE ReadFile, ebx, ADDR buffer, SIZEOF buffer, ADDR bytesRead, 0

    ;CLOSE READ HANDLE
    INVOKE CloseHandle, ebx
    
    ;OPEN FOR WRITING
    INVOKE CreateFileA, ADDR filename, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
    mov ebx, eax
    cmp ebx, INVALID_HANDLE_VALUE
    je save_open_failed

    ;CMP & WRITE
    write_&_loop:
    ;call TO_LOCAL to fill localBuffer here and move to edi
    INVOKE IS_GREATER
    cmp eax, 0 ;IS_GREATER compares structures in esi and edi. if esi greater(or equal), eax is 1 & vice versa
    je write_input
    ;write localBuffer to file here
    jmp write_&_loop
    write_input:
    ;write userInput to file here

    ;WRITE REMAINING

    ;CLOSE WRITE HANDLE
    INVOKE CloseHandle, ebx
    jmp returning

save_open_failed:
    mov edx, OFFSET fileError
    call WriteString
    jmp endof_proc

    returning:
    
    call crlf
    mov edx,offset reminderadded
    call writestring
    call crlf
    mov edx, OFFSET pressAnyKeyMsg
    call WriteString
    call ReadChar   
    
    endof_proc:
    ret

SAVE_REMINDER ENDP

TO_LOCAL PROC ;loads from buffer into localBuffer

    ret
TO_LOCAL ENDP

CONSOLE_TO_OBJ ;takes input from console to populate typEntry Obj in esi
    
    ;init SYSTEMTIME obj here
    ;for each input, do while input < SYSTEMTIME obj

    mov edx, OFFSET promptDate
    call WriteString
    call ReadInt
    mov [esi].date, eax

    mov edx, OFFSET promptMonth
    call WriteString
    call ReadInt
    mov [esi].month, eax

    mov edx, OFFSET promptYear
    call WriteString
    call ReadInt
    mov [esi].year, eax

    mov edx, OFFSET promptHour
    call WriteString
    call ReadInt
    mov [esi].hour, eax

    mov edx, OFFSET promptMinute
    call WriteString
    call ReadInt
    mov [esi].minute, eax

    mov edx, OFFSET promptSecond
    call WriteString
    call ReadInt
    mov [esi].second, eax

    mov edx, esi
    add edx, 6
    mov ecx, 93
    call ReadString

    ret
CONSOLE_TO_OBJ ENDP

IS_GREATER PROC ;compares 2 obj(args) of type typEntry loads 1 into eax if arg1 greater or equal else 0

    ret
IS_GREATER ENDP

NEXT_REMINDER PROC

    INVOKE CreateFileA, ADDR filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
    mov ebx, eax       
    INVOKE SetFilePointer, ebx,100, 0, FILE_BEGIN
    INVOKE ReadFile,ebx,ADDR buffer,SIZEOF buffer,ADDR bytesRead,0
    INVOKE CloseHandle, ebx

    mov edx,offset buffer
    call writeString

    INVOKE CreateFileA, ADDR filename, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
    mov ebx,eax

    INVOKE WriteFile, ebx, ADDR buffer, bytesRead, ADDR bytesWritten, 0
    INVOKE CloseHandle,ebx

    call CLEAR_REGS 

    returning:
    
    call crlf
    mov edx,offset ReminderReachedMSG
    call writestring
    call crlf
    mov edx, OFFSET pressAnyKeyMsg
    call WriteString
    call ReadChar   
    
    ret

NEXT_REMINDER ENDP

COMPARE_TIME proc

check:
call clrscr
invoke GetLocalTime, addr currentTime

call displayRecord

mov ax,year
cmp ax,currentTime.wYear
jne NotEqual

movzx ax,month
cmp ax,currentTime.wMonth
jne NotEqual

movzx ax,date
cmp ax,currentTime.wDay
jne NotEqual

movzx ax,hour
cmp ax,currentTime.wHour
jne NotEqual

movzx ax,min
cmp ax,currentTime.wMinute
jne NotEqual


mov eax, offset message   
mov edx, offset ReminderReached
call MessageBox
call NEXT_REMINDER
jmp skipCheck

NotEqual:
    mov eax,1000
    call delay
    jmp check

skipCheck:
    call crlf
    mov edx, OFFSET pressAnyKeyMsg
    call WriteString
    call ReadChar   


ret

COMPARE_TIME endp

END main