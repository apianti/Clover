;
; Library/Uefi/X64/cpuid.nasm
;
; UEFI implementation X64 CPUID intrinsic
;

  default rel
  section .text

  global EfiCpuidEx

; EfiCpuidEx
; Call CPUID
; @param Function    The function to execute
; @param SubFunction The sub function to execute
; @param Eax         On output, the register returned
; @param Ebx         On output, the register returned
; @param Ecx         On output, the register returned
; @param Edx         On output, the register returned
EfiCpuidEx:

  push  rbx
  mov   rax, rcx
  mov   rcx, rdx
  cpuid
  mov   r10, [rsp + 48]
  test  r10, r10
  jz    .1
  mov   [r10], ecx

.1:

  mov   rcx, r8
  jrcxz .2
  mov   [rcx], eax

.2:

  mov   rcx, r9
  jrcxz .3
  mov   [rcx], ebx

.3:

  mov   rcx, [rsp + 56]
  jrcxz .4
  mov   [rcx], edx

.4:

  pop   rbx
  ret
