;
; @file Library/Uefi/AARCH64/intrinsics.asm
;
; UEFI implementation ARM64 intrinsics
;

  area |.text|, CODE

  export EfiStore32
  export EfiStore64
  export EfiCpuPause
  export EfiCompareAndExchange32
  export EfiCompareAndExchange64

  align

; EfiStore32
; Store a 32bit value atomically 
; @param Value     On output, the set or unchanged value
; @param NewValue  The value in which to compare to set the value if equal
EfiStore32  proc

EfiStore32  endp

  align

; EfiStore64
; Store a 64bit value atomically 
; @param Value     On output, the set or unchanged value
; @param NewValue  The value in which to compare to set the value if equal
EfiStore64  proc

EfiStore64  endp

  align

EfiCpuPause  proc

EfiCpuPause  endp
  
  align

EfiCompareAndExchange32  proc

EfiCompareAndExchange32  endp

  align

EfiCompareAndExchange64  proc

EfiCompareAndExchange64  endp

  end
