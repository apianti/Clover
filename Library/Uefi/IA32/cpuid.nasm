;
; Library/Uefi/X64/cpuid.nasm
;
; UEFI implementation IA32 CPUID intrinsic
;

  default rel
  section .text

  global _EfiCpuidEx

; EfiCpuidEx
; Call CPUID
; @param Function    The function to execute
; @param SubFunction The sub function to execute
; @param Eax         On output, the register returned
; @param Ebx         On output, the register returned
; @param Ecx         On output, the register returned
; @param Edx         On output, the register returned
_EfiCpuidEx:

  push   ebx
  mov    eax, [esp + 8]
  mov    ecx, [esp + 12]
  cpuid
  push   edx
  mov    edx, [esp + 28]
  test   edx, edx
  jz     .1
  mov    [edx], ecx

.1:

  pop    edx
  mov    ecx, [esp + 16]
  jecxz  .2
  mov    [ecx], eax

.2:

  mov    ecx, [esp + 20]
  jecxz  .3
  mov    [ecx], ebx

.3:

  mov    ecx, [esp + 28]
  jecxz  .4
  mov    [ecx], edx

.4:

  pop    ebx
  ret
