;
; Library/Uefi/IA32/intrinsics.asm
;
; UEFI implementation IA32 instrinics
;

.586p

.model flat, C

; Export these symbols from this source
public EfiCpuidEx
public EfiCpuPause
public EfiReadMsr
public EfiWriteMsr
public EfiByteSwap16
public EfiByteSwap32
public EfiByteSwap64
public EfiGetTaskRegister
public EfiGetLocalDescriptorTable
public EfiGetInterruptDescriptorTable
public EfiGetGlobalDescriptorTable
public EfiCompareAndExchange32
public EfiCompareAndExchange64

.code

; EfiCpuidEx
; Call CPUID
; @param Function    The function to execute
; @param SubFunction The sub function to execute
; @param Eax         On output, the register returned
; @param Ebx         On output, the register returned
; @param Ecx         On output, the register returned
; @param Edx         On output, the register returned
EfiCpuidEx  proc

  push  ebx
  mov   eax, dword ptr [esp + 8]
  mov   ecx, dword ptr [esp + 12]
  cpuid
  push  edx
  mov   edx, dword ptr [esp + 28]
  test  edx, edx
  jz    @f
  mov   dword ptr [edx], ecx
@@:
  pop   edx
  mov   ecx, dword ptr [esp + 16]
  jecxz @f
  mov   dword ptr [ecx], eax
@@:
  mov   ecx, dword ptr [esp + 20]
  jecxz @f
  mov   dword ptr [ecx], ebx
@@:
  mov   ecx, dword ptr [esp + 28]
  jecxz @f
  mov   dword ptr [ecx], edx
@@:
  pop   ebx
  ret

EfiCpuidEx  endp

; EfiCpuPause
; Pause the CPU for an idle cycle
EfiCpuPause  proc

  pause
  ret

EfiCpuPause  endp

; EfiReadMsr
; Read CPU model specific register
; @param Msr The model specific register to read
; @return The value of the model specific register
EfiReadMsr  proc

  mov ecx, dword ptr [esp + 4]
  rdmsr
  ret

EfiReadMsr  endp

; EfiWriteMsr
; Read CPU model specific register
; @param Msr The model specific register to read
; @return The value of the model specific register
EfiWriteMsr  proc

  mov   edx, dword ptr [esp + 12]
  mov   eax, dword ptr [esp + 8]
  mov   ecx, dword ptr [esp + 4]
  wrmsr
  ret

EfiWriteMsr  endp

; EfiByteSwap16
; Swap the endian byte order of a 16bit word
; @param Word The word to byte swap
; @return The byte swapped word
EfiByteSwap16  proc

  mov   cx, word ptr [esp + 4]
  xor   eax, eax
  mov   ah, cl
  mov   al, ch
  ret

EfiByteSwap16  endp

; EfiByteSwap16
; Swap the endian byte order of a 32bit word
; @param Dword The double word to byte swap
; @return The byte swapped double word
EfiByteSwap32  proc

  mov   ecx, dword ptr [esp + 4]
  xor   eax, eax
  mov   ah, cl
  mov   al, ch
  shl   eax, 16
  mov   ah, cl
  mov   al, ch
  ret

EfiByteSwap32  endp

; EfiByteSwap64
; Swap the endian byte order of a 64bit quad word
; @param Qword The quad word to byte swap
; @return The byte swapped word
EfiByteSwap64  proc

  mov   ecx, dword ptr [esp + 4]
  xor   edx, edx
  mov   dh, cl
  mov   dl, ch
  shl   edx, 16
  shr   ecx, 16
  mov   dh, cl
  mov   dl, ch
  mov   ecx, dword ptr [esp + 8]
  xor   eax, eax
  mov   ah, cl
  mov   al, ch
  shl   eax, 16
  shr   ecx, 16
  mov   ah, cl
  mov   al, ch
  ret

EfiByteSwap64  endp

; EfiGetTaskRegister
; Get the value of the task register
; @return The value of the task register
EfiGetTaskRegister  proc

  push  ebp
  mov   ebp, dword ptr [esp + 8]
  str   word ptr [ebp]
  pop   ebp
  ret

; EfiGetTaskRegister
; Get the current locale descriptor table offset
; @return The current local descriptor table offset
EfiGetTaskRegister  endp

EfiGetLocalDescriptorTable  proc

  push  ebp
  mov   ebp, dword ptr [esp + 8]
  sldt  word ptr [ebp]
  pop   ebp
  ret

EfiGetLocalDescriptorTable  endp

; EfiGetInterruptDescriptorTable
; Get the interrupt descriptor address and offset
; @return The current interrupt descriptor address and offset
EfiGetInterruptDescriptorTable  proc

  push  ebp
  mov   ebp, dword ptr [esp + 8]
  sidt  fword ptr [ebp]
  pop   ebp
  ret

EfiGetInterruptDescriptorTable  endp

; EfiGetGlobalDescriptorTable
; Get the global descriptor address and offset
; @return The current global descriptor address and offset
EfiGetGlobalDescriptorTable  proc

  push  ebp
  mov   ebp, dword ptr [esp + 8]
  sgdt  fword ptr [ebp]
  pop   ebp
  ret

EfiGetGlobalDescriptorTable  endp

; EfiCompareAndExchange32
; Compare and exchange a 32bit value atomically 
; @param Value         On output, the set or unchanged value
; @param CompareValue  The value in which to compare to set the value if equal
; @param ExchangeValue The value to set
; @return The initial value
EfiCompareAndExchange32  proc

  mov          ecx, dword ptr [esp + 4]
  mov          eax, dword ptr [esp + 8]
  mov          edx, dword ptr [esp + 12]
  lock cmpxchg dword ptr [ecx], edx
  ret

EfiCompareAndExchange32  endp

EfiCompareAndExchange64 proc

; EfiCompareAndExchange64
; Compare and exchange a 32bit value atomically 
; @param Value         On output, the set or unchanged value
; @param CompareValue  The value in which to compare to set the value if equal
; @param ExchangeValue The value to set
; @return The initial value
EfiCompareAndExchange64 endp

  push            esi
  push            ebx
  mov             esi, dword ptr [esp + 12]
  mov             eax, dword ptr [esp + 16]
  mov             edx, dword ptr [esp + 20]
  mov             ebx, dword ptr [esp + 24]
  mov             ecx, dword ptr [esp + 28]
  lock cmpxchg8b  qword ptr [esi]
  pop             ebx
  pop             esi
  ret

end

end
