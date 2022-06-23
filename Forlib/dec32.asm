#include    macros.inc
#include    bios.inc

            proc     dec32

            inc      r7                ; move to lsb
            ldn      r7                ; retrieve it
            smi      1                 ; add 1
            str      r7                ; and put it back
            inc      r7                ; point to next byte
            ldn      r7                ; retrieve it
            smbi     0                 ; propagate carry
            str      r7                ; and put it back
            inc      r7                ; point to next byte
            ldn      r7                ; retrieve it
            smbi     0                 ; propagate carry
            str      r7                ; and put it back
            inc      r7                ; point to next byte
            ldn      r7                ; retrieve it
            smbi     0                 ; propagate carry
            str      r7                ; and put it back
            dec      r7                ; move expr pointer back
            dec      r7
            dec      r7
            dec      r7
            sep      sret              ; and return to caller

            endp

