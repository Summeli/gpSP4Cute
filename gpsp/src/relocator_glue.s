@
@ TheRelocator, dynamic text segment relocation routine v1.0 for SymbianOS.
@ Copyright (C) 2009,2010 Olli Hinkka
@
@ This program is free software; you can redistribute it and/or
@ modify it under the terms of the GNU General Public License
@ as published by the Free Software Foundation; version 2
@ of the License.
@
@ This program is distributed in the hope that it will be useful,
@ but WITHOUT ANY WARRANTY; without even the implied warranty of
@ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@ GNU General Public License for more details.
@ 
@ You should have received a copy of the GNU General Public License
@ along with this program; if not, write to the Free Software
@ Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
@


.align 2

.global  _ZN12TheRelocatorC1EPv
.global  _ZN12TheRelocatorD1Ev
.global  __begin_relocated_code
.global  __end_relocated_code

.global  _ZN12TheRelocator8RelocateEPv
.global  _ZN11RHandleBase5CloseEv
.global  _Znwj
.global  _ZdlPv
      
      
@AREA ||.text||, CODE, READONLY, ALIGN=2
.text  
        
@ TheRelocator::TheRelocator(TAny* aTargetAddr)
_ZN12TheRelocatorC1EPv:
push    {r4}
mov     r2,#0
mov     r4,r0
str     r2,[r0,#8]
push    {lr}
bl      _ZN12TheRelocator8RelocateEPv
pop     {lr}
ldmia     r4,{r1,r2}
sub     lr,r1
add     lr,r2
mov     r0,r4
pop     {r4}
bx      lr       
        
@ TheRelocator::~TheRelocator()
_ZN12TheRelocatorD1Ev:
        push    {r4}
        ldr     r1,[r0,#4]
        ldr     r2,[r0,#0]
        mov     r4,r0
        sub     lr,r1
        add     lr,r2
        add     r0,r4,#8
        push    {lr}
        adr     lr,trg
        sub     lr,r1
        add     lr,r2
        bx      lr
trg:   bl       _ZN11RHandleBase5CloseEv
        pop {lr}
        mov      r0,r4
        pop      {r4}
        bx       lr
                


__begin_relocated_code:
        push        {r4-r6}
        mov         r5,r0
        mov         r0,#0xc
        push        {lr}
        bl          _Znwj
        pop         {lr}
        movs        r4,r0
        popeq       {r4-r6}
        bxeq        lr
        mov         r1,r5
        pop         {r4-r6}
        b           _ZN12TheRelocatorC1EPv
        

__end_relocated_code:
        cmp         r0,#0
        bxeq        lr
        push        {r4}
        ldr         r1,[r0,#4]
        ldr         r2,[r0,#0]
        mov         r4,r0
        sub         lr,r1
        add         lr,r2
        add         r0,r4,#8
        push        {lr}
        adr         lr,trg2
        sub         lr,r1
        add         lr,r2
        bx          lr
trg2:  bl          _ZN11RHandleBase5CloseEv
        pop         {lr}
        mov         r0,r4
        pop         {r4}
        b          _ZdlPv


