.section .text

.global _start

_start:
  la sp, _MEM_END
  jal main
_end:
  j _end
