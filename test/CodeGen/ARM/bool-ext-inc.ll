; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=arm-eabi -mattr=neon | FileCheck %s

define i32 @sext_inc(i1 zeroext %x) {
; CHECK-LABEL: sext_inc:
; CHECK:       @ BB#0:
; CHECK-NEXT:    rsb r0, r0, #1
; CHECK-NEXT:    mov pc, lr
  %ext = sext i1 %x to i32
  %add = add i32 %ext, 1
  ret i32 %add
}

define <4 x i32> @sext_inc_vec(<4 x i1> %x) {
; CHECK-LABEL: sext_inc_vec:
; CHECK:       @ BB#0:
; CHECK-NEXT:    vmov d16, r0, r1
; CHECK-NEXT:    vmov.i32 q9, #0x1f
; CHECK-NEXT:    vmov.i32 q10, #0x1
; CHECK-NEXT:    vmovl.u16 q8, d16
; CHECK-NEXT:    vneg.s32 q9, q9
; CHECK-NEXT:    vshl.i32 q8, q8, #31
; CHECK-NEXT:    vshl.s32 q8, q8, q9
; CHECK-NEXT:    vadd.i32 q8, q8, q10
; CHECK-NEXT:    vmov r0, r1, d16
; CHECK-NEXT:    vmov r2, r3, d17
; CHECK-NEXT:    mov pc, lr
  %ext = sext <4 x i1> %x to <4 x i32>
  %add = add <4 x i32> %ext, <i32 1, i32 1, i32 1, i32 1>
  ret <4 x i32> %add
}

