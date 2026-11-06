# Instruction readings from hostfacts

These entries record instruction facts rendered from the routines below. They describe software behavior, not hardware behavior, and each locator names an address in the identified binary.

- `ioc3_early_pckminit` in `6.5.22/unix.IP27` at 0xc000000000097c50 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc000000000097c50-0xc000000000097c58 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000097c50 has v0 = ?.
- `ioc3_pckm_kbdexists` in `6.5.22/unix.IP27` at 0xc0000000000994f0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000000994f0-0xc00000000009952c (60 bytes).
  It occupies 60 bytes.
  The `lw` at 0xc0000000000994fc loads 4 bytes from [0xc0000000014d15a8]:8 +0x28.
  The `ld` at 0xc00000000009950c loads 8 bytes from ([0xc0000000014d15a8]:8 + ([[0xc0000000014d15a8]:8+40] << 0x3)) +0x18.
  The `lw` at 0xc000000000099510 loads 4 bytes from [([0xc0000000014d15a8]:8 + ([[0xc0000000014d15a8]:8+40] << 0x3))+24]:8 +0x4.
  The `beq` branch at 0xc0000000000994f4 targets 0xc000000000099524, is taken when a2(=[0xc0000000014d15a8]:8) == 0, and falls through when a2(=[0xc0000000014d15a8]:8) != 0.
  The `beq` branch at 0xc000000000099500 targets 0xc000000000099524, is taken when a3(=[[0xc0000000014d15a8]:8+40]) == v0(=0x13), and falls through when a3(=[[0xc0000000014d15a8]:8+40]) != v0(=0x13).
  The return at 0xc00000000009951c has v0 = ?.
  The return at 0xc000000000099524 has v0 = 0x0.
- `ioc3_pckm_setleds` in `6.5.22/unix.IP27` at 0xc00000000009952c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000009952c-0xc000000000099568 (60 bytes).
  It occupies 60 bytes.
  The `lw` at 0xc000000000099544 loads 4 bytes from [0xc0000000014d15a8]:8 +0x28.
  The `ld` at 0xc000000000099558 loads 8 bytes from ([0xc0000000014d15a8]:8 + ([[0xc0000000014d15a8]:8+40] << 0x3)) +0x18.
  The `jal` at 0xc000000000099554 calls ioc3_lcl_pckm_setleds [0xc0000000000995e0] with a0=[([0xc0000000014d15a8]:8 + ([[0xc0000000014d15a8]:8+40] << 0x3))+24]:8, a1=a0 at entry (arg0), a2=a2 at entry (arg2), a3=[0xc0000000014d15a8]:8, a4=0x13, a5=[[0xc0000000014d15a8]:8+40].
  The `beq` branch at 0xc00000000009953c targets 0xc00000000009955c, is taken when a3(=[0xc0000000014d15a8]:8) == 0, and falls through when a3(=[0xc0000000014d15a8]:8) != 0.
  The `beq` branch at 0xc000000000099548 targets 0xc00000000009955c, is taken when a5(=[[0xc0000000014d15a8]:8+40]) == a4(=0x13), and falls through when a5(=[[0xc0000000014d15a8]:8+40]) != a4(=0x13).
  The return at 0xc000000000099560 has v0 = one of ?, ret(ioc3_lcl_pckm_setleds).
- `ioc3_pckm_bell` in `6.5.22/unix.IP27` at 0xc000000000099568 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc000000000099568-0xc0000000000995ac (68 bytes).
  It occupies 68 bytes.
  The `lw` at 0xc00000000009957c loads 4 bytes from [0xc0000000014d15a8]:8 +0x28.
  The `lw` at 0xc000000000099588 loads 4 bytes from [0xc0000000014d15a8]:8 +0x34.
  The `ld` at 0xc00000000009959c loads 8 bytes from ([0xc0000000014d15a8]:8 + ([[0xc0000000014d15a8]:8+40] << 0x3)) +0x18.
  The `jal` at 0xc000000000099598 calls ioc3_lcl_pckm_setleds [0xc0000000000995e0] with a0=[([0xc0000000014d15a8]:8 + ([[0xc0000000014d15a8]:8+40] << 0x3))+24]:8, a1=([[0xc0000000014d15a8]:8+52] | 0x8), a2=a2 at entry (arg2), a3=[0xc0000000014d15a8]:8, a4=[[0xc0000000014d15a8]:8+40].
  The `beq` branch at 0xc000000000099574 targets 0xc0000000000995a0, is taken when a3(=[0xc0000000014d15a8]:8) == 0, and falls through when a3(=[0xc0000000014d15a8]:8) != 0.
  The `beq` branch at 0xc000000000099580 targets 0xc0000000000995a4, is taken when a4(=[[0xc0000000014d15a8]:8+40]) == a1(=0x13), and falls through when a4(=[[0xc0000000014d15a8]:8+40]) != a1(=0x13).
  The return at 0xc0000000000995a4 has v0 = ?.
- `ioc3_gl_setporthandler` in `6.5.22/unix.IP27` at 0xc0000000000995ac (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000000995ac-0xc0000000000995d8 (44 bytes).
  It occupies 44 bytes.
  The `lw` at 0xc0000000000995b8 loads 4 bytes from [0xc0000000014d15a8]:8 +0x28.
  The `ld` at 0xc0000000000995c8 loads 8 bytes from ([0xc0000000014d15a8]:8 + ([[0xc0000000014d15a8]:8+40] << 0x3)) +0x18.
  The `sd` at 0xc0000000000995cc stores 8 bytes to [([0xc0000000014d15a8]:8 + ([[0xc0000000014d15a8]:8+40] << 0x3))+24]:8 +0x18.
  The `beq` branch at 0xc0000000000995b0 targets 0xc0000000000995d0, is taken when a2(=[0xc0000000014d15a8]:8) == 0, and falls through when a2(=[0xc0000000014d15a8]:8) != 0.
  The `beq` branch at 0xc0000000000995bc targets 0xc0000000000995d0, is taken when a4(=[[0xc0000000014d15a8]:8+40]) == a3(=0x13), and falls through when a4(=[[0xc0000000014d15a8]:8+40]) != a3(=0x13).
  The return at 0xc0000000000995d0 has v0 = ?.
- `ioc3_du_keyboard_port` in `6.5.22/unix.IP27` at 0xc0000000000995d8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000000995d8-0xc0000000000995e0 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000000995d8 has v0 = -0x1.
- `ioc3_lcl_pckm_setleds` in `6.5.22/unix.IP27` at 0xc0000000000995e0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000000995e0-0xc000000000099634 (84 bytes).
  It occupies 84 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x48, including `ld` at 0xc0000000000995ec, `ld` at 0xc000000000099614.
  The `jal` at 0xc000000000099600 calls mutex_lock [0xc00000000018dda4] with a0=[a0+72]:8+72, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc00000000009960c calls ioc3_lcl_pckm_setleds_lock [0xc000000000099634] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xc00000000009961c calls mutex_unlock [0xc00000000018dcdc] with a0=[a0+72]:8+72, a1=?, a2=?, a3=?.
  The return at 0xc00000000009962c has v0 = ret(mutex_unlock).
- `ioc3_lcl_pckm_setleds_lock` in `6.5.22/unix.IP27` at 0xc000000000099634 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc000000000099634-0xc0000000000997ac (376 bytes).
  It occupies 376 bytes.
  The `ld` at 0xc000000000099640 loads 8 bytes from a0 (arg0 at entry) +0x48.
  5 such `lw` accesses load 4 bytes from [a0+72]:8 +0x34, including `lw` at 0xc000000000099648, `lw` at 0xc0000000000996bc, `lw` at 0xc0000000000996dc.
  The `lw` at 0xc000000000099678 loads 4 bytes from [a0+72]:8 +0x38.
  The `sw` at 0xc0000000000996a4 stores 4 bytes to [a0+72]:8 +0x38.
  2 such `sw` accesses store 4 bytes to [a0+72]:8 +0x34, including `sw` at 0xc0000000000996b0, `sw` at 0xc0000000000996e4.
  The `jal` at 0xc0000000000996b8 calls pckm_pollcmd [0xc000000000099414] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xc0000000000996c4 calls data_check [0xc000000000099874] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc000000000099710 calls pckm_pollcmd [0xc000000000099414] with a0=a0 at entry (arg0), a1=0xed, a2=?, a3=?.
  The `jal` at 0xc00000000009971c calls data_check [0xc000000000099874] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc00000000009973c calls pckm_pollcmd [0xc000000000099414] with a0=a0 at entry (arg0), a1=0xf4, a2=?, a3=?.
  The `jal` at 0xc000000000099748 calls data_check [0xc000000000099874] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc000000000099760 calls default_kbd_setting [0xc000000000098d14] with a0=a0 at entry (arg0), a1=0x0, a2=[[a0+72]:8+52], a3=?.
  The `jal` at 0xc000000000099780 calls pckm_pollcmd [0xc000000000099414] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xc00000000009978c calls data_check [0xc000000000099874] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc00000000009979c calls pckm_reinit_core [0xc000000000098c9c] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xc000000000099668 targets 0xc0000000000996e8, is taken when at(=[[a0+72]:8+52]) == a3(?), and falls through when at(=[[a0+72]:8+52]) != a3(?).
  The `beq` branch at 0xc000000000099670 targets 0xc0000000000996ac, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xc000000000099684 targets 0xc000000000099694, is taken when a7(?) >= 0xe (signed), and falls through when a7(?) < 0xe (signed).
  The `bne` branch at 0xc00000000009968c targets 0xc0000000000996ec, is taken when a4(?) != 0, and falls through when a4(?) == 0.
  The `beq` branch at 0xc000000000099694 targets 0xc0000000000996a4, is taken when a0(?) == 0, and falls through when a0(?) != 0.
  The `bne` branch at 0xc00000000009969c targets 0xc0000000000996ec, is taken when a5(?) != 0, and falls through when a5(?) == 0.
  The `beq` branch at 0xc0000000000996ac targets 0xc00000000009970c and is always taken.
  The `beq` branch at 0xc0000000000996cc targets 0xc000000000099780, is taken when v0(=ret(data_check)) == 0, and falls through when v0(=ret(data_check)) != 0.
  The `beq` branch at 0xc0000000000996d4 targets 0xc0000000000996ec, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xc000000000099708 targets 0xc000000000099770, is taken when t1(?) == 0, and falls through when t1(?) != 0.
  The `bne` branch at 0xc000000000099724 targets 0xc0000000000996b4, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `bne` branch at 0xc000000000099730 targets 0xc000000000099704, is taken when s3(?) < 0x2 (unsigned), and falls through when s3(?) >= 0x2 (unsigned).
  The `bne` branch at 0xc000000000099750 targets 0xc000000000099708, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `beq` branch at 0xc000000000099768 targets 0xc000000000099708 and is always taken.
  The `bne` branch at 0xc000000000099770 targets 0xc0000000000996dc, is taken when s2(?) != 0, and falls through when s2(?) == 0.
  The `beq` branch at 0xc000000000099778 targets 0xc0000000000996ec and is always taken.
  The `bne` branch at 0xc000000000099794 targets 0xc0000000000996d4, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `beq` branch at 0xc0000000000997a4 targets 0xc0000000000996d4 and is always taken.
  The return at 0xc0000000000996fc has v0 = ?.
- `dev_chain` in `6.5.22/unix.IP27` at 0xc0000000001475ac (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001475ac-0xc000000000147698 (236 bytes).
  It occupies 236 bytes.
  The `ld` at 0xc0000000001475b8 loads 8 bytes from a0 (arg0 at entry) +0x38.
  2 such `lw` accesses load 4 bytes from abs 0xc00000000153c920 (v+48), including `lw` at 0xc0000000001475cc, `lw` at 0xc000000000147630.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x50, including `lw` at 0xc0000000001475d4, `lw` at 0xc000000000147618.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x28, including `ld` at 0xc0000000001475f4, `ld` at 0xc000000000147608.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x30, including `ld` at 0xc0000000001475f8, `ld` at 0xc000000000147600.
  The `sd` at 0xc0000000001475fc stores 8 bytes to [a0+40]:8 +0x30.
  The `sd` at 0xc000000000147614 stores 8 bytes to [a0+48]:8 +0x28.
  2 such `ld` accesses load 8 bytes from ?, including `ld` at 0xc000000000147650, `ld` at 0xc00000000014765c.
  The `sd` at 0xc000000000147654 stores 8 bytes to a0 (arg0 at entry) +0x30.
  The `sd` at 0xc000000000147658 stores 8 bytes to a0 (arg0 at entry) +0x28.
  2 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc000000000147660, `sd` at 0xc000000000147668.
  The `sd` at 0xc000000000147674 stores 8 bytes to a0 (arg0 at entry) +0x38.
  The `jal` at 0xc0000000001475ec calls mutex_spinlock [0xc000000000287214] with a0=[a0+56]:8+56, a1=a1 at entry (arg1), a2=[a0+56]:8, a3=[0xc00000000153c920].
  The `jal` at 0xc000000000147610 calls mutex_spinunlock [0xc000000000287260] with a0=[a0+56]:8+56, a1=ret(mutex_spinlock), a2=?, a3=?, a6=[a0+40]:8, a7=[a0+48]:8.
  The `jal` at 0xc000000000147648 calls mutex_spinlock [0xc000000000287214] with a0=?, a1=?, a2=0xc000000001506a00, a3=0xc00000000153c8f0.
  The `jal` at 0xc000000000147670 calls mutex_spinunlock [0xc000000000287260] with a0=?, a1=ret(mutex_spinlock), a2=?, a3=?.
  The `beq` branch at 0xc0000000001475c8 targets 0xc000000000147618, is taken when a2(=[a0+56]:8) == 0, and falls through when a2(=[a0+56]:8) != 0.
  The `beq` branch at 0xc0000000001475e4 targets 0xc00000000014767c, is taken when a2(=[a0+56]:8) == a4(?), and falls through when a2(=[a0+56]:8) != a4(?).
  The `beq` branch at 0xc000000000147628 targets 0xc000000000147690, is taken when a0(=[a0+80]) == t2(=-0x1), and falls through when a0(=[a0+80]) != t2(=-0x1).
  The `beq` branch at 0xc000000000147690 targets 0xc000000000147644 and is always taken.
  The return at 0xc000000000147688 has v0 = one of ?, ret(mutex_spinunlock).
- `ioc3_config` in `6.5.22/unix.IP27` at 0xc00000000017aa7c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017aa7c-0xc00000000017ae10 (916 bytes).
  It occupies 916 bytes.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc00000000017aafc, `lw` at 0xc00000000017ac34, `lw` at 0xc00000000017adc4.
  4 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc00000000017ab18, `ld` at 0xc00000000017ab98, `ld` at 0xc00000000017ac54.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc00000000017ab2c, `ld` at 0xc00000000017abac, `ld` at 0xc00000000017ac68.
  4 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc00000000017ab38, `lw` at 0xc00000000017abb8, `lw` at 0xc00000000017ac74.
  6 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc00000000017ab44, `ld` at 0xc00000000017abc4, `ld` at 0xc00000000017ac80.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc00000000017ab50, `ld` at 0xc00000000017ab60, `ld` at 0xc00000000017ac8c.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc00000000017ab5c, `sw` at 0xc00000000017ac90.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc00000000017ab64, `lw` at 0xc00000000017ad4c.
  The `ld` at 0xc00000000017abf4 loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xc00000000017abf8 loads 1 byte from [a0+72]:8 +0x3.
  The `sb` at 0xc00000000017ac30 stores 1 byte to [a0+72]:8 +0x3.
  The `sw` at 0xc00000000017ad2c stores 4 bytes to a0 (arg0 at entry) +0x80.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x84, including `sw` at 0xc00000000017ad34, `sw` at 0xc00000000017ad78.
  The `lw` at 0xc00000000017ad40 loads 4 bytes from a0 (arg0 at entry) +0x88.
  The `jal` at 0xc00000000017aaa0 calls set_baud_ti [0xc000000000179e7c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jalr` at 0xc00000000017abd8 calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8], a2=[[0xa800000000001020]:8+80]:8, a3=[0xc0000000014d1dd4], a7=0x40000000.
  The `jal` at 0xc00000000017abe0 calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jalr` at 0xc00000000017ace4 calls (unknown callee) with a0=0x1, a1=0xfffffffffeeddead, a2=[[0xa800000000001020]:8+80]:8, a3=a1 at entry (arg1).
  The `jal` at 0xc00000000017acec calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000017ad3c calls ioc3_rx_timeout [0xc00000000017c0dc] with a0=a0 at entry (arg0), a1=[a0+136], a2=?, a3=a1 at entry (arg1), a6=0xa.
  The `jal` at 0xc00000000017ada4 calls spl7 [0xc00000000003b0ac] with a0=[a0+148], a1=([a0+148] << 0x23), a2=0xa800000000001020, a3=[0xa800000000001000]:8, a4=[[0xa800000000001020]:8], a5=0xfffffffffeeddead, a6=[[0xa800000000001020]:8+80]:8.
  The `jalr` at 0xc00000000017adbc calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000017add4 calls spl7 [0xc00000000003b0ac] with a0=[a0+148], a1=?, a2=[a0+72]:8, a3=?, a4=[0xc0000000014d1dd8]:8, a5=[0xa800000000001000]:8, a6=[[0xa800000000001020]:8], a7=0xfffffffffeeddead.
  The `jalr` at 0xc00000000017ade8 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `beq` branch at 0xc00000000017aaa8 targets 0xc00000000017aac4, is taken when v0(=ret(set_baud_ti)) == 0, and falls through when v0(=ret(set_baud_ti)) != 0.
  The `beq` branch at 0xc00000000017aac8 targets 0xc00000000017aaf8, is taken when s1(=a2) == at(=0x5), and falls through when s1(=a2) != at(=0x5).
  The `beq` branch at 0xc00000000017aad0 targets 0xc00000000017ad7c, is taken when s1(=a2) == v0(=0x6), and falls through when s1(=a2) != v0(=0x6).
  The `beq` branch at 0xc00000000017aad8 targets 0xc00000000017ad84, is taken when s1(=a2) == v1(=0x7), and falls through when s1(=a2) != v1(=0x7).
  The `beq` branch at 0xc00000000017aae0 targets 0xc00000000017aaf0, is taken when s1(=a2) == a0(=0x8), and falls through when s1(=a2) != a0(=0x8).
  The `beq` branch at 0xc00000000017aae8 targets 0xc00000000017aab0 and is always taken.
  The `beq` branch at 0xc00000000017aaf0 targets 0xc00000000017aafc and is always taken.
  The `bgez` branch at 0xc00000000017ab04 targets 0xc00000000017abf4, is taken when a1(=([a0+148] << 0x23)) >= 0 (signed), and falls through when a1(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc00000000017ab0c targets 0xc00000000017ab50, is taken when a2(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a2(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017ab28 targets 0xc00000000017ab50, is taken when a3(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a3(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc00000000017ab30 targets 0xc00000000017ab50, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017ab3c targets 0xc00000000017ab50, is taken when a4(=[[0xa800000000001020]:8]) != a5(=0xfffffffffeeddead), and falls through when a4(=[[0xa800000000001020]:8]) == a5(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017ab48 targets 0xc00000000017ada4, is taken when a6(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bltz` branch at 0xc00000000017ab6c targets 0xc00000000017ab84, is taken when a7(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when a7(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `beq` branch at 0xc00000000017ab7c targets 0xc00000000017ae00 and is always taken.
  The `bne` branch at 0xc00000000017ab8c targets 0xc00000000017abec, is taken when a1(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a1(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017aba8 targets 0xc00000000017abec, is taken when v1(=[0xa800000000001000]:8) != a1(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a1(=0x53435241).
  The `beq` branch at 0xc00000000017abb0 targets 0xc00000000017abec, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017abbc targets 0xc00000000017abec, is taken when a1(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a1(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017abc8 targets 0xc00000000017abec, is taken when a2(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017abec targets 0xc00000000017ad9c, is taken when a0(=one of 0x0, 0x1) == 0, and falls through when a0(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc00000000017ac08 targets 0xc00000000017ac20, is taken when v1(=a4) == 0, and falls through when v1(=a4) != 0.
  The `beq` branch at 0xc00000000017ac18 targets 0xc00000000017ad8c, is taken when a0(=a5) == 0, and falls through when a0(=a5) != 0.
  The `beq` branch at 0xc00000000017ac24 targets 0xc00000000017ac30, is taken when a1(=a3) == 0, and falls through when a1(=a3) != 0.
  The `beq` branch at 0xc00000000017ac40 targets 0xc00000000017acf4, is taken when (a3(=0x10000000) & a0(=[a0+148])) == 0, and falls through when (a3(=0x10000000) & a0(=[a0+148])) != 0.
  The `bne` branch at 0xc00000000017ac48 targets 0xc00000000017ac8c, is taken when a4(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a4(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017ac64 targets 0xc00000000017ac8c, is taken when a5(=[0xa800000000001000]:8) != a6(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a6(=0x53435241).
  The `beq` branch at 0xc00000000017ac6c targets 0xc00000000017ac8c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017ac78 targets 0xc00000000017ac8c, is taken when a6(=[[0xa800000000001020]:8]) != a7(=0xfffffffffeeddead), and falls through when a6(=[[0xa800000000001020]:8]) == a7(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017ac84 targets 0xc00000000017add4, is taken when t0(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when t0(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc00000000017ac98 targets 0xc00000000017acf8, is taken when t1(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when t1(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017acb8 targets 0xc00000000017acf4, is taken when v1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc00000000017acc0 targets 0xc00000000017acf8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017accc targets 0xc00000000017acf8, is taken when a0(=[[0xa800000000001020]:8]) != a1(=0xfffffffffeeddead), and falls through when a0(=[[0xa800000000001020]:8]) == a1(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017acd8 targets 0xc00000000017acf8, is taken when a2(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017ad30 targets 0xc00000000017ad70, is taken when a2(?) == 0, and falls through when a2(?) != 0.
  The `beq` branch at 0xc00000000017ad44 targets 0xc00000000017aab0 and is always taken.
  The `bne` branch at 0xc00000000017ad58 targets 0xc00000000017ab84, is taken when (t0(=[[a0+64]:8]) & a7(=0x40000000)) != 0, and falls through when (t0(=[[a0+64]:8]) & a7(=0x40000000)) == 0.
  The `beq` branch at 0xc00000000017ad60 targets 0xc00000000017ad4c, is taken when t1(?) == 0, and falls through when t1(?) != 0.
  The `beq` branch at 0xc00000000017ad68 targets 0xc00000000017ab88 and is always taken.
  The `beq` branch at 0xc00000000017ad74 targets 0xc00000000017ad38 and is always taken.
  The `beq` branch at 0xc00000000017ad7c targets 0xc00000000017aafc and is always taken.
  The `beq` branch at 0xc00000000017ad84 targets 0xc00000000017aafc and is always taken.
  The `bne` branch at 0xc00000000017ad8c targets 0xc00000000017ac2c, is taken when t3(=a3) != 0, and falls through when t3(=a3) == 0.
  The `beq` branch at 0xc00000000017ad94 targets 0xc00000000017ac30 and is always taken.
  The `beq` branch at 0xc00000000017ad9c targets 0xc00000000017aab0 and is always taken.
  The `beq` branch at 0xc00000000017adcc targets 0xc00000000017ab50 and is always taken.
  The `beq` branch at 0xc00000000017adf8 targets 0xc00000000017ac8c and is always taken.
  The `beq` branch at 0xc00000000017ae08 targets 0xc00000000017ad60 and is always taken.
  The return at 0xc00000000017aabc has v0 = one of -0x1, 0x0, 0x1.
- `ioc3_set_extclk` in `6.5.22/unix.IP27` at 0xc00000000017af34 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017af34-0xc00000000017af3c (8 bytes).
  It occupies 8 bytes.
  The return at 0xc00000000017af34 has v0 = a1.
- `ioc3_write` in `6.5.22/unix.IP27` at 0xc00000000017b1b8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017b1b8-0xc00000000017b1d4 (28 bytes).
  It occupies 28 bytes.
  The `jal` at 0xc00000000017b1c0 calls do_ioc3_write [0xc00000000017af3c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The return at 0xc00000000017b1cc has v0 = ret(do_ioc3_write).
- `ioc3_sync_write` in `6.5.22/unix.IP27` at 0xc00000000017b1d4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017b1d4-0xc00000000017b234 (96 bytes).
  It occupies 96 bytes.
  The `lw` at 0xc00000000017b200 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xc00000000017b1e8 calls do_ioc3_write [0xc00000000017af3c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc00000000017b214 calls ioc3_enable_tx [0xc00000000017b59c] with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `blez` branch at 0xc00000000017b1f0 targets 0xc00000000017b21c, is taken when s3(=a2) <= 0 (signed), and falls through when s3(=a2) > 0 (signed).
  The `bne` branch at 0xc00000000017b1f8 targets 0xc00000000017b21c, is taken when v0(=ret(do_ioc3_write)) != 0, and falls through when v0(=ret(do_ioc3_write)) == 0.
  The `beq` branch at 0xc00000000017b208 targets 0xc00000000017b220, is taken when (at(=[a0+120]) & 0x10) == 0, and falls through when (at(=[a0+120]) & 0x10) != 0.
  The return at 0xc00000000017b22c has v0 = ret(do_ioc3_write).
- `ioc3_wrflush` in `6.5.22/unix.IP27` at 0xc00000000017b234 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017b234-0xc00000000017b2f8 (196 bytes).
  It occupies 196 bytes.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x38, including `ld` at 0xc00000000017b234, `ld` at 0xc00000000017b270, `ld` at 0xc00000000017b2b8.
  3 such `lw` accesses load 4 bytes from [a0+56]:8 +0x1c, including `lw` at 0xc00000000017b238, `lw` at 0xc00000000017b274, `lw` at 0xc00000000017b2bc.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc00000000017b254, `ld` at 0xc00000000017b2ac, `ld` at 0xc00000000017b2d4.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc00000000017b258, `lw` at 0xc00000000017b2d8.
  The `lw` at 0xc00000000017b290 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xc00000000017b2a0 calls ioc3_enable_tx [0xc00000000017b59c] with a0=a0 at entry (arg0), a1=0x1, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc00000000017b2cc calls us_delay [0xc000000000047274] with a0=0x5, a1=?, a2=one of 0x10000000, 0x50000000, a3=?.
  The `beq` branch at 0xc00000000017b24c targets 0xc00000000017b290, is taken when (at(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (at(=[[a0+56]:8+28]) & 0x1) != 0.
  The `bne` branch at 0xc00000000017b268 targets 0xc00000000017b284, is taken when a0(=0x10000000) != v0(?), and falls through when a0(=0x10000000) == v0(?).
  The `beq` branch at 0xc00000000017b27c targets 0xc00000000017b2b0, is taken when (v1(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (v1(=[[a0+56]:8+28]) & 0x1) != 0.
  The `beq` branch at 0xc00000000017b298 targets 0xc00000000017b254, is taken when (a0(=[a0+120]) & 0x10) == 0, and falls through when (a0(=[a0+120]) & 0x10) != 0.
  The `beq` branch at 0xc00000000017b2a8 targets 0xc00000000017b258 and is always taken.
  The `beq` branch at 0xc00000000017b2b0 targets 0xc00000000017b2cc and is always taken.
  The `bne` branch at 0xc00000000017b2c4 targets 0xc00000000017b2f0, is taken when (a1(=[[a0+56]:8+28]) & 0x1) != 0, and falls through when (a1(=[[a0+56]:8+28]) & 0x1) == 0.
  The `beq` branch at 0xc00000000017b2e8 targets 0xc00000000017b2b8, is taken when a2(=0x10000000) == a3(?), and falls through when a2(=0x10000000) != a3(?).
  The `beq` branch at 0xc00000000017b2f0 targets 0xc00000000017b284 and is always taken.
  The return at 0xc00000000017b288 has v0 = one of ?, ret(us_delay).
- `ioc3_break` in `6.5.22/unix.IP27` at 0xc00000000017b2f8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017b2f8-0xc00000000017b59c (676 bytes).
  It occupies 676 bytes.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc00000000017b2f8, `lw` at 0xc00000000017b424, `lw` at 0xc00000000017b54c.
  4 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc00000000017b32c, `ld` at 0xc00000000017b3b8, `ld` at 0xc00000000017b440.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc00000000017b340, `ld` at 0xc00000000017b3bc, `ld` at 0xc00000000017b454.
  4 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc00000000017b34c, `lw` at 0xc00000000017b3d0, `lw` at 0xc00000000017b460.
  6 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc00000000017b358, `ld` at 0xc00000000017b3dc, `ld` at 0xc00000000017b46c.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc00000000017b364, `ld` at 0xc00000000017b374, `ld` at 0xc00000000017b478.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc00000000017b370, `sw` at 0xc00000000017b47c.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc00000000017b378, `lw` at 0xc00000000017b4f4.
  The `ld` at 0xc00000000017b410 loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xc00000000017b418 loads 1 byte from [a0+72]:8 +0x3.
  2 such `sb` accesses store 1 byte to [a0+72]:8 +0x3, including `sb` at 0xc00000000017b420, `sb` at 0xc00000000017b524.
  The `jalr` at 0xc00000000017b3f0 calls (unknown callee) with a0=0x1, a1=a1 at entry (arg1), a2=[[0xa800000000001020]:8], a3=0xfffffffffeeddead, a4=[[0xa800000000001020]:8+80]:8, a5=[0xc0000000014d1dd4], a6=0x10000000, a7=[0xc0000000014d1dd8]:8.
  The `jal` at 0xc00000000017b3f8 calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jalr` at 0xc00000000017b4cc calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=?, a4=[[0xa800000000001020]:8+80]:8.
  The `jal` at 0xc00000000017b4d4 calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000017b530 calls spl7 [0xc00000000003b0ac] with a0=[a0+72]:8, a1=a1 at entry (arg1), a2=?, a3=?, a4=[a0+148], a6=0x10000000.
  The `jalr` at 0xc00000000017b544 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a5=[[0xa800000000001020]:8+80]:8.
  The `jal` at 0xc00000000017b55c calls spl7 [0xc00000000003b0ac] with a0=[[0xa800000000001020]:8], a1=a1 at entry (arg1), a2=0xfffffffffeeddead, a3=[[0xa800000000001020]:8+80]:8, a4=[a0+148], a6=0x10000000.
  The `jalr` at 0xc00000000017b570 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+80]:8.
  The `bgez` branch at 0xc00000000017b314 targets 0xc00000000017b410, is taken when at(=([a0+148] << 0x23)) >= 0 (signed), and falls through when at(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc00000000017b320 targets 0xc00000000017b364, is taken when v0(=[0xc0000000014d1dd8]:8) != a0, and falls through when v0(=[0xc0000000014d1dd8]:8) == a0.
  The `bne` branch at 0xc00000000017b33c targets 0xc00000000017b364, is taken when v1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc00000000017b344 targets 0xc00000000017b364, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017b350 targets 0xc00000000017b364, is taken when a0(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a0(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017b35c targets 0xc00000000017b55c, is taken when a3(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a3(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bltz` branch at 0xc00000000017b38c targets 0xc00000000017b39c, is taken when t0(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t0(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `beq` branch at 0xc00000000017b394 targets 0xc00000000017b590 and is always taken.
  The `bne` branch at 0xc00000000017b3b4 targets 0xc00000000017b408, is taken when a7(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a7(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017b3c0 targets 0xc00000000017b408, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc00000000017b3c8 targets 0xc00000000017b408, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017b3d4 targets 0xc00000000017b408, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017b3e0 targets 0xc00000000017b408, is taken when a4(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a4(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017b408 targets 0xc00000000017b528, is taken when s1(=one of 0x0, 0x1) == 0, and falls through when s1(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc00000000017b414 targets 0xc00000000017b518, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc00000000017b42c targets 0xc00000000017b4dc, is taken when (a6(=0x10000000) & a4(=[a0+148])) == 0, and falls through when (a6(=0x10000000) & a4(=[a0+148])) != 0.
  The `bne` branch at 0xc00000000017b434 targets 0xc00000000017b478, is taken when t1(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when t1(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017b450 targets 0xc00000000017b478, is taken when t2(=[0xa800000000001000]:8) != t3(=0x53435241), and falls through when t2(=[0xa800000000001000]:8) == t3(=0x53435241).
  The `beq` branch at 0xc00000000017b458 targets 0xc00000000017b478, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017b464 targets 0xc00000000017b478, is taken when t3(=[[0xa800000000001020]:8]) != t8(=0xfffffffffeeddead), and falls through when t3(=[[0xa800000000001020]:8]) == t8(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017b470 targets 0xc00000000017b530, is taken when t9(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when t9(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc00000000017b484 targets 0xc00000000017b4e0, is taken when ra(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when ra(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017b49c targets 0xc00000000017b4e0, is taken when v0(=[0xa800000000001000]:8) != v1(=0x53435241), and falls through when v0(=[0xa800000000001000]:8) == v1(=0x53435241).
  The `beq` branch at 0xc00000000017b4ac targets 0xc00000000017b4dc, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017b4b8 targets 0xc00000000017b4dc, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017b4c4 targets 0xc00000000017b4dc, is taken when a4(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a4(=[[0xa800000000001020]:8+80]:8) != 0.
  The `bne` branch at 0xc00000000017b500 targets 0xc00000000017b39c, is taken when (a2(=[[a0+64]:8]) & a7(=0x40000000)) != 0, and falls through when (a2(=[[a0+64]:8]) & a7(=0x40000000)) == 0.
  The `beq` branch at 0xc00000000017b508 targets 0xc00000000017b4f4, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0xc00000000017b510 targets 0xc00000000017b3a4 and is always taken.
  The `beq` branch at 0xc00000000017b520 targets 0xc00000000017b424 and is always taken.
  The `beq` branch at 0xc00000000017b528 targets 0xc00000000017b4e0 and is always taken.
  The `beq` branch at 0xc00000000017b554 targets 0xc00000000017b478 and is always taken.
  The `beq` branch at 0xc00000000017b588 targets 0xc00000000017b364 and is always taken.
  The `beq` branch at 0xc00000000017b594 targets 0xc00000000017b508 and is always taken.
  The return at 0xc00000000017b4ec has v0 = one of -0x1, 0x0.
- `ioc3_enable_tx` in `6.5.22/unix.IP27` at 0xc00000000017b59c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017b59c-0xc00000000017b7ac (528 bytes).
  It occupies 528 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xc00000000017b5a8, `lw` at 0xc00000000017b674.
  The `ld` at 0xc00000000017b5b0 loads 8 bytes from a0 (arg0 at entry) +0x70.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc00000000017b5dc, `ld` at 0xc00000000017b6bc.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc00000000017b5e0, `ld` at 0xc00000000017b6c0, `ld` at 0xc00000000017b788.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc00000000017b5f4, `lw` at 0xc00000000017b6e4.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc00000000017b600, `ld` at 0xc00000000017b6f0, `ld` at 0xc00000000017b78c.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc00000000017b60c, `lw` at 0xc00000000017b698.
  4 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc00000000017b618, `ld` at 0xc00000000017b628, `ld` at 0xc00000000017b6b0.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc00000000017b624, `sw` at 0xc00000000017b6b4.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc00000000017b62c, `lw` at 0xc00000000017b65c.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x98, including `lw` at 0xc00000000017b678, `lw` at 0xc00000000017b754.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xc00000000017b684, `sw` at 0xc00000000017b73c.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x4, including `sw` at 0xc00000000017b688, `sw` at 0xc00000000017b770.
  2 such `lw` accesses load 4 bytes from [a0+112]:8 +0x8, including `lw` at 0xc00000000017b694, `lw` at 0xc00000000017b744.
  The `lw` at 0xc00000000017b734 loads 4 bytes from [a0+64]:8 +0x8.
  The `jal` at 0xc00000000017b690 calls enable_intrs [0xc00000000017983c] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=?, a3=?, a4=[a0+64]:8.
  The `jalr` at 0xc00000000017b6fc calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8+80]:8, a2=[0xc0000000014d1dd8]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=0xa800000000001000.
  The `jal` at 0xc00000000017b704 calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000017b74c calls disable_intrs [0xc00000000017988c] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=?, a3=?, a4=[a0+64]:8, a6=([a0+120] | 0x10).
  The `jal` at 0xc00000000017b77c calls spl7 [0xc00000000003b0ac] with a0=0x53435241, a1=a1 at entry (arg1), a2=[[0xa800000000001020]:8], a3=0xfffffffffeeddead, a4=[[0xa800000000001020]:8+80]:8, a5=0xa800000000001020, a6=[0xc0000000014d1dd8]:8.
  The `jalr` at 0xc00000000017b794 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `beq` branch at 0xc00000000017b5b8 targets 0xc00000000017b724, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc00000000017b5c0 targets 0xc00000000017b72c, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `bne` branch at 0xc00000000017b5d8 targets 0xc00000000017b60c, is taken when a6(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a6(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017b5e4 targets 0xc00000000017b60c, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc00000000017b5ec targets 0xc00000000017b60c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017b5f8 targets 0xc00000000017b60c, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017b604 targets 0xc00000000017b77c, is taken when a4(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a4(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bgez` branch at 0xc00000000017b614 targets 0xc00000000017b670, is taken when a5(=([a0+148] << 0x23)) >= 0 (signed), and falls through when a5(=([a0+148] << 0x23)) < 0 (signed).
  The `bltz` branch at 0xc00000000017b644 targets 0xc00000000017b670, is taken when t0(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t0(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `bne` branch at 0xc00000000017b654 targets 0xc00000000017b774, is taken when t2(?) != 0, and falls through when t2(?) == 0.
  The `beq` branch at 0xc00000000017b668 targets 0xc00000000017b654, is taken when (t3(=[[a0+64]:8]) & a6(=0x40000000)) == 0, and falls through when (t3(=[[a0+64]:8]) & a6(=0x40000000)) != 0.
  The `beq` branch at 0xc00000000017b670 targets 0xc00000000017b734, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc00000000017b6a8 targets 0xc00000000017b6b8, is taken when (ra(=0x10000000) & a0(=[a0+148])) == 0, and falls through when (ra(=0x10000000) & a0(=[a0+148])) != 0.
  The `bne` branch at 0xc00000000017b6cc targets 0xc00000000017b70c, is taken when a2(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a2(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017b6d4 targets 0xc00000000017b70c, is taken when a1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc00000000017b6dc targets 0xc00000000017b70c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017b6e8 targets 0xc00000000017b70c, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017b6f4 targets 0xc00000000017b70c, is taken when a1(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a1(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017b724 targets 0xc00000000017b5c8, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc00000000017b72c targets 0xc00000000017b710 and is always taken.
  The `beq` branch at 0xc00000000017b760 targets 0xc00000000017b698, is taken when a7(=[a0+152]) == v0(?), and falls through when a7(=[a0+152]) != v0(?).
  The `beq` branch at 0xc00000000017b76c targets 0xc00000000017b698 and is always taken.
  The `beq` branch at 0xc00000000017b774 targets 0xc00000000017b714 and is always taken.
  The `beq` branch at 0xc00000000017b7a4 targets 0xc00000000017b60c and is always taken.
  The return at 0xc00000000017b71c has v0 = one of -0x1, 0x0.
- `ioc3_read` in `6.5.22/unix.IP27` at 0xc00000000017b7ac (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017b7ac-0xc00000000017bedc (1840 bytes).
  It occupies 1840 bytes.
  The `ld` at 0xc00000000017b7b8 loads 8 bytes from a0 (arg0 at entry) +0x70.
  11 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc00000000017b7d0, `ld` at 0xc00000000017b7e8, `ld` at 0xc00000000017b8dc.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x9c, including `lw` at 0xc00000000017b7d4, `lw` at 0xc00000000017b7f4.
  3 such `sw` accesses store 4 bytes to [a0+64]:8 +0x10, including `sw` at 0xc00000000017b7e4, `sw` at 0xc00000000017bb88, `sw` at 0xc00000000017be88.
  4 such `lw` accesses load 4 bytes from [a0+64]:8 +0xc, including `lw` at 0xc00000000017b7f0, `lw` at 0xc00000000017b8e0, `lw` at 0xc00000000017b9ac.
  5 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc00000000017b814, `lw` at 0xc00000000017b9bc, `lw` at 0xc00000000017bb9c.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc00000000017b838, `ld` at 0xc00000000017b890, `ld` at 0xc00000000017b8f8.
  9 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc00000000017b850, `ld` at 0xc00000000017b8a4, `ld` at 0xc00000000017b90c.
  6 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc00000000017b85c, `lw` at 0xc00000000017b8b0, `lw` at 0xc00000000017b918.
  9 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc00000000017b86c, `ld` at 0xc00000000017b8c0, `ld` at 0xc00000000017b924.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc00000000017b880, `sw` at 0xc00000000017ba14.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc00000000017b930, `lw` at 0xc00000000017bd08.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xc00000000017b9c8, `sw` at 0xc00000000017bd9c.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xc00000000017ba70, `lw` at 0xc00000000017bae8, `lw` at 0xc00000000017bb8c.
  The `ld` at 0xc00000000017ba74 loads 8 bytes from a0 (arg0 at entry) +0x60.
  4 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xc00000000017ba84, `sw` at 0xc00000000017bd34, `sw` at 0xc00000000017bd8c.
  6 such `lw` accesses load 4 bytes from ?, including `lw` at 0xc00000000017baa8, `lw` at 0xc00000000017bb68, `lw` at 0xc00000000017bce0.
  7 such `lbu` accesses load 1 byte from ?, including `lbu` at 0xc00000000017bac4, `lbu` at 0xc00000000017bb2c, `lbu` at 0xc00000000017bc20.
  5 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x8c, including `lw` at 0xc00000000017bad8, `lw` at 0xc00000000017bb50, `lw` at 0xc00000000017bc30.
  5 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x8, including `ld` at 0xc00000000017bb10, `ld` at 0xc00000000017bc04, `ld` at 0xc00000000017bc40.
  4 such `ld` accesses load 8 bytes from [a0+8]:8 +0x10, including `ld` at 0xc00000000017bb1c, `ld` at 0xc00000000017bc10, `ld` at 0xc00000000017bc4c.
  4 such `sb` accesses store 1 byte to ?, including `sb` at 0xc00000000017bb38, `sb` at 0xc00000000017bcb0, `sb` at 0xc00000000017bcc4.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x9c, including `sw` at 0xc00000000017bb98, `sw` at 0xc00000000017be98.
  The `lw` at 0xc00000000017bd40 loads 4 bytes from [a0+112]:8 +0x10.
  The `ld` at 0xc00000000017bea8 loads 8 bytes from [a0+8]:8 +0x18.
  The `jalr` at 0xc00000000017b8cc calls (unknown callee) with a0=0x1, a1=?, a2=[[0xa800000000001020]:8+80]:8, a3=?, a5=[a0+64]:8, a7=[0xc0000000014d1dd8]:8.
  The `jal` at 0xc00000000017b8d4 calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jalr` at 0xc00000000017b990 calls (unknown callee) with a0=0x1, a1=[0xa800000000001000]:8, a2=[[0xa800000000001020]:8+80]:8, a3=?, a5=[a0+64]:8.
  The `jal` at 0xc00000000017b998 calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jalr` at 0xc00000000017ba60 calls (unknown callee) with a0=0x1, a1=?, a2=[[0xa800000000001020]:8+80]:8, a3=?, a5=[a0+64]:8, a7=[0xc0000000014d1dd8]:8.
  The `jal` at 0xc00000000017ba68 calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jalr` at 0xc00000000017bb24 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x8, a2=[a0+140], a3=?.
  The `jalr` at 0xc00000000017bc18 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x2, a2=[a0+140], a3=?, a7=[[a0+8]:8+16]:8.
  The `jalr` at 0xc00000000017bc54 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x4, a2=?, a3=?.
  The `jalr` at 0xc00000000017bc90 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `jal` at 0xc00000000017bd3c calls enable_intrs [0xc00000000017983c] with a0=a0 at entry (arg0), a1=[[a0+112]:8+16], a2=?, a3=?.
  The `jal` at 0xc00000000017bdd8 calls spl7 [0xc00000000003b0ac] with a0=?, a1=[0xc0000000014d1dd8]:8, a2=[0xa800000000001000]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[a0+64]:8, a6=[[0xa800000000001020]:8+80]:8.
  The `jalr` at 0xc00000000017bdec calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a6=[[0xa800000000001020]:8+80]:8.
  The `jal` at 0xc00000000017be04 calls spl7 [0xc00000000003b0ac] with a0=?, a1=0xffffffffefffffff, a2=[0xa800000000001000]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[a0+64]:8, a6=[[0xa800000000001020]:8+80]:8.
  The `jalr` at 0xc00000000017be18 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+80]:8.
  The `jal` at 0xc00000000017be30 calls spl7 [0xc00000000003b0ac] with a0=?, a1=?, a2=[0xc0000000014d1dd8]:8, a3=?, a5=[a0+64]:8.
  The `jalr` at 0xc00000000017be44 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `jalr` at 0xc00000000017beb0 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x0, a2=[a0+140], a3=?.
  The `bne` branch at 0xc00000000017b80c targets 0xc00000000017ba70, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bgez` branch at 0xc00000000017b81c targets 0xc00000000017bd90, is taken when v1(=([a0+148] << 0x23)) >= 0 (signed), and falls through when v1(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc00000000017b82c targets 0xc00000000017b87c, is taken when a1(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a1(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017b844 targets 0xc00000000017b87c, is taken when a2(=[0xa800000000001000]:8) != a3(=0x53435241), and falls through when a2(=[0xa800000000001000]:8) == a3(=0x53435241).
  The `beq` branch at 0xc00000000017b854 targets 0xc00000000017b878, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017b864 targets 0xc00000000017b878, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017b870 targets 0xc00000000017bdd8, is taken when a6(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc00000000017b888 targets 0xc00000000017b8dc, is taken when a7(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a7(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017b89c targets 0xc00000000017b8dc, is taken when t1(=[0xa800000000001000]:8) != t2(=0x53435241), and falls through when t1(=[0xa800000000001000]:8) == t2(=0x53435241).
  The `beq` branch at 0xc00000000017b8a8 targets 0xc00000000017b8dc, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017b8b8 targets 0xc00000000017b8dc, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017b8c4 targets 0xc00000000017b8dc, is taken when a2(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+80]:8) != 0.
  The `bne` branch at 0xc00000000017b8e8 targets 0xc00000000017b9b4, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bne` branch at 0xc00000000017b8f0 targets 0xc00000000017b930, is taken when s0(=a0) != a2(=[0xc0000000014d1dd8]:8), and falls through when s0(=a0) == a2(=[0xc0000000014d1dd8]:8).
  The `bne` branch at 0xc00000000017b904 targets 0xc00000000017b930, is taken when t8(=[0xa800000000001000]:8) != t9(=0x53435241), and falls through when t8(=[0xa800000000001000]:8) == t9(=0x53435241).
  The `beq` branch at 0xc00000000017b910 targets 0xc00000000017b930, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017b91c targets 0xc00000000017b930, is taken when t9(=[[0xa800000000001020]:8]) != ra(=0xfffffffffeeddead), and falls through when t9(=[[0xa800000000001020]:8]) == ra(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017b928 targets 0xc00000000017be30, is taken when at(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when at(=[[0xa800000000001020]:8+80]:8) == 0.
  The `beq` branch at 0xc00000000017b938 targets 0xc00000000017b94c, is taken when (v0(=[[a0+64]:8]) & s2(=0x8000000)) == 0, and falls through when (v0(=[[a0+64]:8]) & s2(=0x8000000)) != 0.
  The `beq` branch at 0xc00000000017b944 targets 0xc00000000017becc and is always taken.
  The `bne` branch at 0xc00000000017b950 targets 0xc00000000017b9a0, is taken when s0(=a0) != a2(=[0xc0000000014d1dd8]:8), and falls through when s0(=a0) == a2(=[0xc0000000014d1dd8]:8).
  The `bne` branch at 0xc00000000017b964 targets 0xc00000000017b9a0, is taken when a1(=[0xa800000000001000]:8) != a2(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a2(=0x53435241).
  The `beq` branch at 0xc00000000017b970 targets 0xc00000000017b9a0, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017b97c targets 0xc00000000017b9a0, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017b988 targets 0xc00000000017b9a0, is taken when a2(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017b9a0 targets 0xc00000000017bdd0, is taken when s2(=one of 0x0, 0x1) == 0, and falls through when s2(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc00000000017b9b4 targets 0xc00000000017ba70, is taken when s3(=one of 0x0, 0x1) == 0, and falls through when s3(=one of 0x0, 0x1) != 0.
  The `bne` branch at 0xc00000000017b9d0 targets 0xc00000000017ba14, is taken when t8(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when t8(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017b9e4 targets 0xc00000000017ba14, is taken when a2(=[0xa800000000001000]:8) != a3(=0x53435241), and falls through when a2(=[0xa800000000001000]:8) == a3(=0x53435241).
  The `beq` branch at 0xc00000000017b9f0 targets 0xc00000000017ba14, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017ba00 targets 0xc00000000017ba14, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017ba0c targets 0xc00000000017be04, is taken when a6(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc00000000017ba1c targets 0xc00000000017ba70, is taken when a7(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a7(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017ba30 targets 0xc00000000017ba70, is taken when t0(=[0xa800000000001000]:8) != t1(=0x53435241), and falls through when t0(=[0xa800000000001000]:8) == t1(=0x53435241).
  The `beq` branch at 0xc00000000017ba3c targets 0xc00000000017ba70, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017ba4c targets 0xc00000000017ba70, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017ba58 targets 0xc00000000017ba70, is taken when a2(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017ba80 targets 0xc00000000017bb80, is taken when s5(?) == s1(=[a0+156]), and falls through when s5(?) != s1(=[a0+156]).
  The `blez` branch at 0xc00000000017ba88 targets 0xc00000000017bda8, is taken when s6(=a2) <= 0 (signed), and falls through when s6(=a2) > 0 (signed).
  The `beq` branch at 0xc00000000017bab0 targets 0xc00000000017bd4c, is taken when (s4(=0xffffffffc0c0c0c0) & t8(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & t8(?)) != 0.
  The `blez` branch at 0xc00000000017bab8 targets 0xc00000000017bce0, is taken when s6(?) <= 0 (signed), and falls through when s6(?) > 0 (signed).
  The `beq` branch at 0xc00000000017bad0 targets 0xc00000000017bb3c, is taken when (a0(?) & 0x40) == 0, and falls through when (a0(?) & 0x40) != 0.
  The `beq` branch at 0xc00000000017bae0 targets 0xc00000000017bb00, is taken when (a2(=[a0+140]) & 0x40) == 0, and falls through when (a2(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xc00000000017baf0 targets 0xc00000000017bb00, is taken when (v0(=[a0+120]) & 0x2) == 0, and falls through when (v0(=[a0+120]) & 0x2) != 0.
  The `beq` branch at 0xc00000000017baf8 targets 0xc00000000017bdb0, is taken when a6(?) == 0, and falls through when a6(?) != 0.
  The `beq` branch at 0xc00000000017bb00 targets 0xc00000000017bb34, is taken when a5(?) == 0, and falls through when a5(?) != 0.
  The `beq` branch at 0xc00000000017bb08 targets 0xc00000000017bb34, is taken when a7(?) == 0, and falls through when a7(?) != 0.
  The `beq` branch at 0xc00000000017bb14 targets 0xc00000000017bb30, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc00000000017bb40 targets 0xc00000000017bcc8, is taken when (a0(?) & 0x80) == 0, and falls through when (a0(?) & 0x80) != 0.
  The `beq` branch at 0xc00000000017bb48 targets 0xc00000000017bca0, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc00000000017bb58 targets 0xc00000000017bca0, is taken when (a2(=[a0+140]) & 0x1c) == 0, and falls through when (a2(=[a0+140]) & 0x1c) != 0.
  The `blez` branch at 0xc00000000017bb60 targets 0xc00000000017bbf0, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xc00000000017bb78 targets 0xc00000000017bb84 and is always taken.
  The `beq` branch at 0xc00000000017bb94 targets 0xc00000000017bbbc, is taken when (v0(=[a0+120]) & 0x1) == 0, and falls through when (v0(=[a0+120]) & 0x1) != 0.
  The `bne` branch at 0xc00000000017bbb4 targets 0xc00000000017bd2c, is taken when a4(?) < a5(?) (unsigned), and falls through when a4(?) >= a5(?) (unsigned).
  The `beq` branch at 0xc00000000017bbf0 targets 0xc00000000017bc28, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc00000000017bbfc targets 0xc00000000017bc28, is taken when (a2(=[a0+140]) & 0x8) == 0, and falls through when (a2(=[a0+140]) & 0x8) != 0.
  The `beq` branch at 0xc00000000017bc08 targets 0xc00000000017bc28, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc00000000017bc28 targets 0xc00000000017bc64, is taken when t0(?) == 0, and falls through when t0(?) != 0.
  The `beq` branch at 0xc00000000017bc38 targets 0xc00000000017bc64, is taken when (t1(=[a0+140]) & 0x10) == 0, and falls through when (t1(=[a0+140]) & 0x10) != 0.
  The `beq` branch at 0xc00000000017bc44 targets 0xc00000000017bc64, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc00000000017bc64 targets 0xc00000000017bca0, is taken when t3(?) == 0, and falls through when t3(?) != 0.
  The `beq` branch at 0xc00000000017bc74 targets 0xc00000000017bca0, is taken when (t8(=[a0+140]) & 0x4) == 0, and falls through when (t8(=[a0+140]) & 0x4) != 0.
  The `beq` branch at 0xc00000000017bc80 targets 0xc00000000017bca0, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc00000000017bcd0 targets 0xc00000000017bce0, is taken when s8(?) >= 0x4 (signed), and falls through when s8(?) < 0x4 (signed).
  The `bgtz` branch at 0xc00000000017bcd8 targets 0xc00000000017bac4, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xc00000000017bce8 targets 0xc00000000017bda0, is taken when (s4(=0xffffffffc0c0c0c0) & a0(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a0(?)) != 0.
  The `beq` branch at 0xc00000000017bcf0 targets 0xc00000000017bb84, is taken when s5(?) == s1(?), and falls through when s5(?) != s1(?).
  The `bgtz` branch at 0xc00000000017bcf8 targets 0xc00000000017baa0, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xc00000000017bd00 targets 0xc00000000017bb88 and is always taken.
  The `beq` branch at 0xc00000000017bd14 targets 0xc00000000017b94c, is taken when (a1(=[[a0+64]:8]) & s2(=0x8000000)) == 0, and falls through when (a1(=[[a0+64]:8]) & s2(=0x8000000)) != 0.
  The `beq` branch at 0xc00000000017bd1c targets 0xc00000000017bd08, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0xc00000000017bd24 targets 0xc00000000017b950 and is always taken.
  The `beq` branch at 0xc00000000017bd44 targets 0xc00000000017bbbc and is always taken.
  The `beq` branch at 0xc00000000017bd68 targets 0xc00000000017bd80, is taken when v0(?) == a0(?), and falls through when v0(?) != a0(?).
  The `bne` branch at 0xc00000000017bd78 targets 0xc00000000017bab8, is taken when (s4(=0xffffffffc0c0c0c0) & a3(?)) != 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a3(?)) == 0.
  The `beq` branch at 0xc00000000017bd88 targets 0xc00000000017bb88 and is always taken.
  The `beq` branch at 0xc00000000017bd98 targets 0xc00000000017b828 and is always taken.
  The `beq` branch at 0xc00000000017bda0 targets 0xc00000000017bcf0 and is always taken.
  The `beq` branch at 0xc00000000017bda8 targets 0xc00000000017bb84 and is always taken.
  The `blez` branch at 0xc00000000017bdb0 targets 0xc00000000017be5c, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xc00000000017bdc8 targets 0xc00000000017bb84 and is always taken.
  The `beq` branch at 0xc00000000017bdd0 targets 0xc00000000017bbbc and is always taken.
  The `beq` branch at 0xc00000000017bdfc targets 0xc00000000017b878 and is always taken.
  The `beq` branch at 0xc00000000017be28 targets 0xc00000000017ba14 and is always taken.
  The `beq` branch at 0xc00000000017be54 targets 0xc00000000017b930 and is always taken.
  The `beq` branch at 0xc00000000017be7c targets 0xc00000000017bec0, is taken when (s4(=0xffffffffc0c0c0c0) & t1(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & t1(?)) != 0.
  The `beq` branch at 0xc00000000017be94 targets 0xc00000000017beb8, is taken when (t9(=[a0+140]) & 0x40) == 0, and falls through when (t9(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xc00000000017bea0 targets 0xc00000000017beb8, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc00000000017beb8 targets 0xc00000000017bbbc and is always taken.
  The `beq` branch at 0xc00000000017bec4 targets 0xc00000000017be84 and is always taken.
  The `beq` branch at 0xc00000000017bed4 targets 0xc00000000017bd1c and is always taken.
  The return at 0xc00000000017bbe8 has v0 = one of ?, -0x1, 0x0.
- `ioc3_set_DTR` in `6.5.22/unix.IP27` at 0xc00000000017c468 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017c468-0xc00000000017c488 (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xc00000000017c474 calls set_DTRRTS [0xc00000000017c294] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x1, a3=0x1000000.
  The return at 0xc00000000017c480 has v0 = ret(set_DTRRTS).
- `ioc3_set_RTS` in `6.5.22/unix.IP27` at 0xc00000000017c488 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017c488-0xc00000000017c4a8 (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xc00000000017c494 calls set_DTRRTS [0xc00000000017c294] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x2, a3=0x2000000.
  The return at 0xc00000000017c4a0 has v0 = ret(set_DTRRTS).
- `ioc3_query_DCD` in `6.5.22/unix.IP27` at 0xc00000000017c4a8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017c4a8-0xc00000000017c5ac (260 bytes).
  It occupies 260 bytes.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc00000000017c4c4, `ld` at 0xc00000000017c514.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc00000000017c4c8, `ld` at 0xc00000000017c51c, `ld` at 0xc00000000017c580.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc00000000017c4dc, `lw` at 0xc00000000017c528.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc00000000017c4e8, `ld` at 0xc00000000017c534, `ld` at 0xc00000000017c584.
  The `ld` at 0xc00000000017c4f8 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xc00000000017c50c loads 4 bytes from [a0+64]:8 +0x18.
  The `jalr` at 0xc00000000017c54c calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=[[0xa800000000001020]:8+80]:8, a4=[[a0+64]:8+24], a5=0xa800000000001020, a6=[0xa800000000001000]:8, a7=[[0xa800000000001020]:8].
  The `jal` at 0xc00000000017c554 calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000017c574 calls spl7 [0xc00000000003b0ac] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=[0xc0000000014d1dd8]:8, a4=0x53435241, a5=[[0xa800000000001020]:8], a6=0xfffffffffeeddead, a7=[[0xa800000000001020]:8+80]:8.
  The `jalr` at 0xc00000000017c58c calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc00000000017c4c0 targets 0xc00000000017c4f8, is taken when a0 != a3(=[0xc0000000014d1dd8]:8), and falls through when a0 == a3(=[0xc0000000014d1dd8]:8).
  The `bne` branch at 0xc00000000017c4cc targets 0xc00000000017c4f8, is taken when a5(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc00000000017c4d4 targets 0xc00000000017c4f8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017c4e0 targets 0xc00000000017c4f8, is taken when a5(=[[0xa800000000001020]:8]) != a6(=0xfffffffffeeddead), and falls through when a5(=[[0xa800000000001020]:8]) == a6(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017c4f0 targets 0xc00000000017c574, is taken when a7(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a7(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc00000000017c508 targets 0xc00000000017c564, is taken when a0 != a3(=[0xc0000000014d1dd8]:8), and falls through when a0 == a3(=[0xc0000000014d1dd8]:8).
  The `bne` branch at 0xc00000000017c518 targets 0xc00000000017c564, is taken when a6(=[0xa800000000001000]:8) != a7(=0x53435241), and falls through when a6(=[0xa800000000001000]:8) == a7(=0x53435241).
  The `beq` branch at 0xc00000000017c520 targets 0xc00000000017c564, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017c52c targets 0xc00000000017c564, is taken when a7(=[[0xa800000000001020]:8]) != t0(=0xfffffffffeeddead), and falls through when a7(=[[0xa800000000001020]:8]) == t0(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017c53c targets 0xc00000000017c564, is taken when a3(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017c5a4 targets 0xc00000000017c4f8 and is always taken.
  The return at 0xc00000000017c56c has v0 = ?.
- `ioc3_query_CTS` in `6.5.22/unix.IP27` at 0xc00000000017c5ac (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017c5ac-0xc00000000017c6b0 (260 bytes).
  It occupies 260 bytes.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc00000000017c5c8, `ld` at 0xc00000000017c618.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc00000000017c5cc, `ld` at 0xc00000000017c620, `ld` at 0xc00000000017c684.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc00000000017c5e0, `lw` at 0xc00000000017c62c.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc00000000017c5ec, `ld` at 0xc00000000017c638, `ld` at 0xc00000000017c688.
  The `ld` at 0xc00000000017c5fc loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xc00000000017c610 loads 4 bytes from [a0+64]:8 +0x18.
  The `jalr` at 0xc00000000017c650 calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=[[0xa800000000001020]:8+80]:8, a4=[[a0+64]:8+24], a5=0xa800000000001020, a6=[0xa800000000001000]:8, a7=[[0xa800000000001020]:8].
  The `jal` at 0xc00000000017c658 calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000017c678 calls spl7 [0xc00000000003b0ac] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=[0xc0000000014d1dd8]:8, a4=0x53435241, a5=[[0xa800000000001020]:8], a6=0xfffffffffeeddead, a7=[[0xa800000000001020]:8+80]:8.
  The `jalr` at 0xc00000000017c690 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc00000000017c5c4 targets 0xc00000000017c5fc, is taken when a0 != a3(=[0xc0000000014d1dd8]:8), and falls through when a0 == a3(=[0xc0000000014d1dd8]:8).
  The `bne` branch at 0xc00000000017c5d0 targets 0xc00000000017c5fc, is taken when a5(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc00000000017c5d8 targets 0xc00000000017c5fc, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017c5e4 targets 0xc00000000017c5fc, is taken when a5(=[[0xa800000000001020]:8]) != a6(=0xfffffffffeeddead), and falls through when a5(=[[0xa800000000001020]:8]) == a6(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017c5f4 targets 0xc00000000017c678, is taken when a7(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a7(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc00000000017c60c targets 0xc00000000017c668, is taken when a0 != a3(=[0xc0000000014d1dd8]:8), and falls through when a0 == a3(=[0xc0000000014d1dd8]:8).
  The `bne` branch at 0xc00000000017c61c targets 0xc00000000017c668, is taken when a6(=[0xa800000000001000]:8) != a7(=0x53435241), and falls through when a6(=[0xa800000000001000]:8) == a7(=0x53435241).
  The `beq` branch at 0xc00000000017c624 targets 0xc00000000017c668, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017c630 targets 0xc00000000017c668, is taken when a7(=[[0xa800000000001020]:8]) != t0(=0xfffffffffeeddead), and falls through when a7(=[[0xa800000000001020]:8]) == t0(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017c640 targets 0xc00000000017c668, is taken when a3(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017c6a8 targets 0xc00000000017c5fc and is always taken.
  The return at 0xc00000000017c670 has v0 = ?.
- `ioc3_get_mapid` in `6.5.22/unix.IP27` at 0xc00000000017c704 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017c704-0xc00000000017c764 (96 bytes).
  It occupies 96 bytes.
  The `ld` at 0xc00000000017c718 loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `jal` at 0xc00000000017c73c calls copyout [0xc00000000001f7ec] with a0=sp-16, a1=a1 at entry (arg1), a2=0x8, a3=0xc000000001463b80.
  The `beq` branch at 0xc00000000017c70c targets 0xc00000000017c754, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc00000000017c74c targets 0xc00000000017c758 and is always taken.
  The return at 0xc00000000017c75c has v0 = one of 0x0, 0xe.
- `ioc3_map` in `6.5.22/unix.IP27` at 0xc00000000017c764 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017c764-0xc00000000017c7f4 (144 bytes).
  It occupies 144 bytes.
  The `ld` at 0xc00000000017c790 loads 8 bytes from a0 (arg0 at entry) +0x58.
  The `ld` at 0xc00000000017c7c4 loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `ld` at 0xc00000000017c7d4 loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `jal` at 0xc00000000017c798 calls disable_intrs [0xc00000000017988c] with a0=a0 at entry (arg0), a1=-0x1, a2=?, a3=?, a5=0x16.
  The `jal` at 0xc00000000017c7a8 calls v_mapphys [0xc0000000001ebbdc] with a0=a1 at entry (arg1), a1=?, a2=0x4000, a3=?.
  The `beq` branch at 0xc00000000017c76c targets 0xc00000000017c788, is taken when a2 == 0, and falls through when a2 != 0.
  The `beq` branch at 0xc00000000017c778 targets 0xc00000000017c7c4, is taken when a2 == at(=0x4000), and falls through when a2 != at(=0x4000).
  The `beq` branch at 0xc00000000017c780 targets 0xc00000000017c7bc and is always taken.
  The `beq` branch at 0xc00000000017c7ec targets 0xc00000000017c798 and is always taken.
  The return at 0xc00000000017c7bc has v0 = one of 0x16, ret(v_mapphys).
- `ioc3_unmap` in `6.5.22/unix.IP27` at 0xc00000000017c7f4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017c7f4-0xc00000000017c930 (316 bytes).
  It occupies 316 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc00000000017c7fc, `lw` at 0xc00000000017c920.
  The `sw` at 0xc00000000017c810 stores 4 bytes to a0 (arg0 at entry) +0x94.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc00000000017c820, `ld` at 0xc00000000017c880.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc00000000017c828, `ld` at 0xc00000000017c874, `ld` at 0xc00000000017c908.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc00000000017c844, `lw` at 0xc00000000017c894.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc00000000017c850, `ld` at 0xc00000000017c8a0, `ld` at 0xc00000000017c90c.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc00000000017c85c, `ld` at 0xc00000000017c8c8, `ld` at 0xc00000000017c8dc.
  The `sw` at 0xc00000000017c864 stores 4 bytes to [a0+64]:8 +0x0.
  The `lw` at 0xc00000000017c8cc loads 4 bytes from [a0+64]:8 +0x8.
  The `sw` at 0xc00000000017c8d4 stores 4 bytes to a0 (arg0 at entry) +0x98.
  The `sw` at 0xc00000000017c8d8 stores 4 bytes to [a0+64]:8 +0x4.
  The `lw` at 0xc00000000017c8e0 loads 4 bytes from [a0+64]:8 +0xc.
  The `sw` at 0xc00000000017c8e8 stores 4 bytes to a0 (arg0 at entry) +0x9c.
  The `sw` at 0xc00000000017c8ec stores 4 bytes to [a0+64]:8 +0x10.
  The `jalr` at 0xc00000000017c8b4 calls (unknown callee) with a0=0x1, a1=[a0+64]:8, a2=[[0xa800000000001020]:8], a3=[[0xa800000000001020]:8+80]:8, a4=[0xc0000000014d1dd4], a5=0xa800000000001020, a6=[0xc0000000014d1dd8]:8.
  The `jal` at 0xc00000000017c8bc calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000017c8fc calls spl7 [0xc00000000003b0ac] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=?, a4=[0xa800000000001000]:8, a5=0x53435241, a6=[[0xa800000000001020]:8], a7=0xfffffffffeeddead.
  The `jalr` at 0xc00000000017c914 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+80]:8.
  The `bne` branch at 0xc00000000017c82c targets 0xc00000000017c85c, is taken when at(=[0xc0000000014d1dd8]:8) != a0, and falls through when at(=[0xc0000000014d1dd8]:8) == a0.
  The `bne` branch at 0xc00000000017c834 targets 0xc00000000017c85c, is taken when a4(=[0xa800000000001000]:8) != a5(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a5(=0x53435241).
  The `beq` branch at 0xc00000000017c83c targets 0xc00000000017c85c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017c848 targets 0xc00000000017c85c, is taken when a6(=[[0xa800000000001020]:8]) != a7(=0xfffffffffeeddead), and falls through when a6(=[[0xa800000000001020]:8]) == a7(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017c854 targets 0xc00000000017c8fc, is taken when t0(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when t0(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc00000000017c87c targets 0xc00000000017c8c8, is taken when a6(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a6(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017c884 targets 0xc00000000017c8c8, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc00000000017c88c targets 0xc00000000017c8c8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017c898 targets 0xc00000000017c8c8, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017c8a4 targets 0xc00000000017c8c8, is taken when a3(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017c928 targets 0xc00000000017c85c and is always taken.
  The return at 0xc00000000017c8f4 has v0 = one of [0xa800000000001020]:8, ret(splx).
- `ioc3_set_sscr` in `6.5.22/unix.IP27` at 0xc00000000017c930 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017c930-0xc00000000017ca54 (292 bytes).
  It occupies 292 bytes.
  3 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc00000000017c930, `lw` at 0xc00000000017ca04, `lw` at 0xc00000000017ca44.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xc00000000017c94c, `sw` at 0xc00000000017ca1c.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc00000000017c968, `ld` at 0xc00000000017c9bc.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc00000000017c96c, `ld` at 0xc00000000017c9b0, `ld` at 0xc00000000017ca2c.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc00000000017c980, `lw` at 0xc00000000017c9d0.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc00000000017c98c, `ld` at 0xc00000000017c9dc, `ld` at 0xc00000000017ca30.
  The `ld` at 0xc00000000017c998 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `sw` at 0xc00000000017c9a0 stores 4 bytes to [a0+64]:8 +0x0.
  The `jalr` at 0xc00000000017c9f0 calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8+80]:8, a2=[0xc0000000014d1dd8]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[0xc0000000014d1dd4].
  The `jal` at 0xc00000000017c9f8 calls splx [0xc00000000003b0e4] with a0=[0xc0000000014d1dd4], a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000017ca20 calls spl7 [0xc00000000003b0ac] with a0=0x53435241, a1=[[0xa800000000001020]:8], a2=0xfffffffffeeddead, a3=[[0xa800000000001020]:8+80]:8, a4=0xa800000000001020.
  The `jalr` at 0xc00000000017ca38 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a6=[[0xa800000000001020]:8+80]:8.
  The `beq` branch at 0xc00000000017c940 targets 0xc00000000017ca14, is taken when a2 == 0, and falls through when a2 != 0.
  The `bne` branch at 0xc00000000017c964 targets 0xc00000000017c998, is taken when a2(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a2(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017c970 targets 0xc00000000017c998, is taken when a1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc00000000017c978 targets 0xc00000000017c998, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017c984 targets 0xc00000000017c998, is taken when a1(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a1(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017c990 targets 0xc00000000017ca20, is taken when a3(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a3(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc00000000017c9b8 targets 0xc00000000017ca04, is taken when a2(=[0xc0000000014d1dd8]:8) != s0(=a0), and falls through when a2(=[0xc0000000014d1dd8]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017c9c0 targets 0xc00000000017ca04, is taken when a0(=[0xa800000000001000]:8) != a1(=0x53435241), and falls through when a0(=[0xa800000000001000]:8) == a1(=0x53435241).
  The `beq` branch at 0xc00000000017c9c8 targets 0xc00000000017ca04, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017c9d4 targets 0xc00000000017ca04, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017c9e0 targets 0xc00000000017ca04, is taken when a1(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a1(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017ca18 targets 0xc00000000017c950 and is always taken.
  The `beq` branch at 0xc00000000017ca4c targets 0xc00000000017c998 and is always taken.
  The return at 0xc00000000017ca0c has v0 = [a0+148].
- `ioc3_console_write` in `6.5.22/unix.IP27` at 0xc00000000017ca54 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000017ca54-0xc00000000017cba8 (340 bytes).
  It occupies 340 bytes.
  7 such `lw` accesses load 4 bytes from [0xc0000000014c9eb8]:8 +0xb8, including `lw` at 0xc00000000017ca58, `lw` at 0xc00000000017cb34, `lw` at 0xc00000000017cb44.
  The `sd` at 0xc00000000017ca80 stores 8 bytes to a2 (arg2 at entry) +0x0.
  The `sb` at 0xc00000000017caa8 stores 1 byte to [0xc0000000014c9eb8]:8 +0x20178.
  6 such `lbu` accesses load 1 byte from [0xc0000000014c9eb8]:8 +0x2017d, including `lbu` at 0xc00000000017caac, `lbu` at 0xc00000000017cac4, `lbu` at 0xc00000000017cad4.
  The `lbu` at 0xc00000000017cab8 loads 1 byte from ?.
  The `sw` at 0xc00000000017cb30 stores 4 bytes to [0xc0000000014c9eb8]:8 +0xb8.
  The `bne` branch at 0xc00000000017ca68 targets 0xc00000000017ca80, is taken when t0(=0x10000000) != t2(?), and falls through when t0(=0x10000000) == t2(?).
  The `beq` branch at 0xc00000000017ca78 targets 0xc00000000017cb9c and is always taken.
  The `bne` branch at 0xc00000000017ca94 targets 0xc00000000017caac, is taken when a1 != 0, and falls through when a1 == 0.
  The `beq` branch at 0xc00000000017ca9c targets 0xc00000000017cb1c and is always taken.
  The `beq` branch at 0xc00000000017caa4 targets 0xc00000000017cb1c, is taken when a1(?) == 0, and falls through when a1(?) != 0.
  The `bne` branch at 0xc00000000017cabc targets 0xc00000000017caa4, is taken when (a6(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) != 0, and falls through when (a6(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc00000000017cacc targets 0xc00000000017caa4, is taken when (a7(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) != 0, and falls through when (a7(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc00000000017cadc targets 0xc00000000017caa4, is taken when (t0(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) != 0, and falls through when (t0(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc00000000017caec targets 0xc00000000017caa4, is taken when (t1(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) != 0, and falls through when (t1(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc00000000017cafc targets 0xc00000000017caa4, is taken when (t2(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) != 0, and falls through when (t2(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) == 0.
  The `beq` branch at 0xc00000000017cb0c targets 0xc00000000017cac4, is taken when (t3(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) == 0, and falls through when (t3(=[[0xc0000000014c9eb8]:8+131453]:1) & 0x20) != 0.
  The `beq` branch at 0xc00000000017cb14 targets 0xc00000000017caa4 and is always taken.
  The `bne` branch at 0xc00000000017cb28 targets 0xc00000000017ca80, is taken when t0(=0x10000000) != t8(?), and falls through when t0(=0x10000000) == t8(?).
  The `bne` branch at 0xc00000000017cb3c targets 0xc00000000017cb28, is taken when a5(=0x30000000) != t9(?), and falls through when a5(=0x30000000) == t9(?).
  The `bne` branch at 0xc00000000017cb4c targets 0xc00000000017cb24, is taken when a5(=0x30000000) != v0(?), and falls through when a5(=0x30000000) == v0(?).
  The `bne` branch at 0xc00000000017cb5c targets 0xc00000000017cb24, is taken when a5(=0x30000000) != v1(?), and falls through when a5(=0x30000000) == v1(?).
  The `bne` branch at 0xc00000000017cb6c targets 0xc00000000017cb24, is taken when a5(=0x30000000) != a3(?), and falls through when a5(=0x30000000) == a3(?).
  The `bne` branch at 0xc00000000017cb7c targets 0xc00000000017cb24, is taken when a5(=0x30000000) != t3(?), and falls through when a5(=0x30000000) == t3(?).
  The `beq` branch at 0xc00000000017cb8c targets 0xc00000000017cb44, is taken when a5(=0x30000000) == t8(?), and falls through when a5(=0x30000000) != t8(?).
  The `beq` branch at 0xc00000000017cb94 targets 0xc00000000017cb24 and is always taken.
  The `beq` branch at 0xc00000000017cba0 targets 0xc00000000017cb30 and is always taken.
  The return at 0xc00000000017cb1c has v0 = 0x0.
- `idbg_gdb` in `6.5.22/unix.IP27` at 0xc00000000019b844 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000019b844-0xc00000000019b878 (52 bytes).
  It occupies 52 bytes.
  The `jal` at 0xc00000000019b854 calls dogang [0xc00000000019b444] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc00000000019b868 calls idbg_gang [0xc00000000019b6ec] with a0=-0x1, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `beq` branch at 0xc00000000019b84c targets 0xc00000000019b868, is taken when a0 == at(=-0x1), and falls through when a0 != at(=-0x1).
  The `beq` branch at 0xc00000000019b870 targets 0xc00000000019b860 and is always taken.
  The return at 0xc00000000019b860 has v0 = one of ret(dogang), ret(idbg_gang).
- `panicregs` in `6.5.22/unix.IP27` at 0xc0000000001a5a88 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001a5a88-0xc0000000001a5b3c (180 bytes).
  It occupies 180 bytes.
  The `ld` at 0xc0000000001a5ad8 loads 8 bytes from a0 (arg0 at entry) +0x120.
  The `ld` at 0xc0000000001a5b04 loads 8 bytes from a0 (arg0 at entry) +0x110.
  The `ld` at 0xc0000000001a5b08 loads 8 bytes from table(0xc000000001463ed8) +0x0.
  The `jal` at 0xc0000000001a5aa4 calls spl1 [0xc00000000003affc] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001a5aac calls set_leds [0xc000000000056d5c] with a0=0x3, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001a5acc calls machine_error_dump [0xc0000000001fc1b8] with a0=0xc0000000003e1c30, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001a5b24 calls cmn_err [0xc0000000001f8090] with a0=0x0, a1=0xc0000000003e1c40, a2=a4 at entry (arg4), a3=[a0+288]:8, a4=a0 at entry (arg0), a5=a1 at entry (arg1), a6=entry(0xc000000001463ed8), a7=[a0+272]:8.
  The `bne` branch at 0xc0000000001a5ac4 targets 0xc0000000001a5ad4, is taken when v0(=[0xc0000000014ca4d0]) != 0, and falls through when v0(=[0xc0000000014ca4d0]) == 0.
  The return at 0xc0000000001a5b34 has v0 = ret(cmn_err).
- `mem_range_init` in `6.5.22/unix.IP27` at 0xc0000000001ae3b0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001ae3b0-0xc0000000001ae724 (884 bytes).
  It occupies 884 bytes.
  6 such `ld` accesses load 8 bytes from abs 0xffffffffffffc400, including `ld` at 0xc0000000001ae440, `ld` at 0xc0000000001ae46c, `ld` at 0xc0000000001ae48c.
  5 such `ld` accesses load 8 bytes from [-0x3c00]:8 +0x0, including `ld` at 0xc0000000001ae444, `ld` at 0xc0000000001ae470, `ld` at 0xc0000000001ae490.
  3 such `ld` accesses load 8 bytes from ((a0 << 0x3) + [[-0x3c00]:8]:8) +0x0, including `ld` at 0xc0000000001ae450, `ld` at 0xc0000000001ae478, `ld` at 0xc0000000001ae498.
  The `lw` at 0xc0000000001ae458 loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x614.
  The `sw` at 0xc0000000001ae468 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x614.
  The `lw` at 0xc0000000001ae47c loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x624.
  The `sw` at 0xc0000000001ae488 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x624.
  The `lw` at 0xc0000000001ae49c loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x6a0.
  The `sw` at 0xc0000000001ae4a8 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x6a0.
  2 such `ld` accesses load 8 bytes from ([[-0x3c00]:8]:8 + (a0 << 0x3)) +0x0, including `ld` at 0xc0000000001ae4b8, `ld` at 0xc0000000001ae58c.
  The `ll` at 0xc0000000001ae4c4 loads 4 bytes from [([[-0x3c00]:8]:8 + (a0 << 0x3))]:8 +0x61c.
  The `sc` at 0xc0000000001ae4d0 stores 4 bytes to [([[-0x3c00]:8]:8 + (a0 << 0x3))]:8 +0x61c.
  The `ld` at 0xc0000000001ae4e8 loads 8 bytes from abs 0xc000000001539850 (Nodepdaindr).
  3 such `lw` accesses load 4 bytes from ?, including `lw` at 0xc0000000001ae4fc, `lw` at 0xc0000000001ae57c, `lw` at 0xc0000000001ae638.
  The `ld` at 0xc0000000001ae504 loads 8 bytes from [0xc000000001539850]:8 +0x608.
  4 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc0000000001ae510, `sw` at 0xc0000000001ae518, `sw` at 0xc0000000001ae624.
  The `ld` at 0xc0000000001ae520 loads 8 bytes from ([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  The `sd` at 0xc0000000001ae554 stores 8 bytes to ([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  3 such `ld` accesses load 8 bytes from ?, including `ld` at 0xc0000000001ae598, `ld` at 0xc0000000001ae5e4, `ld` at 0xc0000000001ae6b0.
  9 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc0000000001ae5ac, `sd` at 0xc0000000001ae5e8, `sd` at 0xc0000000001ae5ec.
  The `lw` at 0xc0000000001ae5c0 loads 4 bytes from [-0x3c00]:8 +0x63c.
  3 such `ld` accesses load 8 bytes from ([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0, including `ld` at 0xc0000000001ae5c8, `ld` at 0xc0000000001ae690, `ld` at 0xc0000000001ae6e0.
  2 such `lhu` accesses load 2 bytes from ?, including `lhu` at 0xc0000000001ae5e0, `lhu` at 0xc0000000001ae69c.
  The `ld` at 0xc0000000001ae65c loads 8 bytes from table(0xc0000000015270a0) +0x0.
  The `ld` at 0xc0000000001ae670 loads 8 bytes from table(0xc000000001516d68) +0x0.
  2 such `ld` accesses load 8 bytes from ([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x48, including `ld` at 0xc0000000001ae674, `ld` at 0xc0000000001ae6c8.
  The `sd` at 0xc0000000001ae6f8 stores 8 bytes to ([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x80.
  The `jal` at 0xc0000000001ae3d4 calls is_specified [0xc0000000001aa634] with a0=0xc0000000014d219b, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001ae438 calls setupbadmem [0xc000000000029864] with a0=?, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=?.
  The `jal` at 0xc0000000001ae658 calls bset [0xc0000000002685c4] with a0=entry(0xc0000000015270a0), a1=?, a2=?, a3=ret(is_specified).
  The `jal` at 0xc0000000001ae66c calls bset [0xc0000000002685c4] with a0=entry(0xc000000001516d68), a1=?, a2=?, a3=?.
  The `beql` branch at 0xc0000000001ae4d4 targets 0xc0000000001ae4c4, is taken when t9(?) == 0, and falls through when t9(?) != 0; its branch-likely delay slot `nop` at 0xc0000000001ae4d8 runs only when taken and is annulled on fall-through.
  The `bne` branch at 0xc0000000001ae538 targets 0xc0000000001ae5b0, is taken when t8(=[([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) != 0, and falls through when t8(=[([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) == 0.
  The `bne` branch at 0xc0000000001ae5a8 targets 0xc0000000001ae560, is taken when a1(?) < 0x6 (signed), and falls through when a1(?) >= 0x6 (signed).
  The `bltz` branch at 0xc0000000001ae5d4 targets 0xc0000000001ae6ec, is taken when s3(?) < 0 (signed), and falls through when s3(?) >= 0 (signed).
  The `beq` branch at 0xc0000000001ae5dc targets 0xc0000000001ae6a0 and is always taken.
  The `bne` branch at 0xc0000000001ae688 targets 0xc0000000001ae694, is taken when a1(=[([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s0(?), and falls through when a1(=[([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s0(?).
  The `bltz` branch at 0xc0000000001ae694 targets 0xc0000000001ae6ec, is taken when a0(?) < 0 (signed), and falls through when a0(?) >= 0 (signed).
  The `beq` branch at 0xc0000000001ae6a0 targets 0xc0000000001ae6c8, is taken when v0(=0xffff) == a2(?), and falls through when v0(=0xffff) != a2(?).
  The `bne` branch at 0xc0000000001ae6a8 targets 0xc0000000001ae5e4, is taken when a3(=ret(is_specified)) != 0, and falls through when a3(=ret(is_specified)) == 0.
  The `beq` branch at 0xc0000000001ae6c0 targets 0xc0000000001ae5f8 and is always taken.
  The `bne` branch at 0xc0000000001ae6d8 targets 0xc0000000001ae6e4, is taken when a5(=[([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s0(?), and falls through when a5(=[([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s0(?).
  The `bgez` branch at 0xc0000000001ae6e4 targets 0xc0000000001ae69c, is taken when a0(?) >= 0 (signed), and falls through when a0(?) < 0 (signed).
  The return at 0xc0000000001ae71c has v0 = 0xffff.
- `idbg_init` in `6.5.22/unix.IP27` at 0xc0000000001cf794 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cf794-0xc0000000001cf808 (116 bytes).
  It occupies 116 bytes.
  2 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc0000000001cf7bc, `sw` at 0xc0000000001cf7dc.
  The `jal` at 0xc0000000001cf7f0 calls init_mutex [0xc00000000018dc14] with a0=0xc000000001538268, a1=0x0, a2=0xc0000000003e4978, a3=-0x1, a5=0xc000000001537738.
  The `beq` branch at 0xc0000000001cf7c0 targets 0xc0000000001cf7e4, is taken when a2(?) >= 0x14 (signed), and falls through when a2(?) < 0x14 (signed).
  The `bne` branch at 0xc0000000001cf7d8 targets 0xc0000000001cf7ac, is taken when a2(?) < 0x14 (signed), and falls through when a2(?) >= 0x14 (signed).
  The return at 0xc0000000001cf800 has v0 = 0x1.
- `idbg_id` in `6.5.22/unix.IP27` at 0xc0000000001cf808 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cf808-0xc0000000001cf810 (8 bytes).
  It occupies 8 bytes.
  The `lw` at 0xc0000000001cf80c loads 4 bytes from a0 (arg0 at entry) +0x3c.
  The return at 0xc0000000001cf808 has v0 = [a0+60].
- `idbg_lock` in `6.5.22/unix.IP27` at 0xc0000000001cf810 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cf810-0xc0000000001cf818 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001cf810 has v0 = 0x0.
- `idbg_unlock` in `6.5.22/unix.IP27` at 0xc0000000001cf818 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cf818-0xc0000000001cf820 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001cf818 has v0 = ?.
- `idbg_copydata` in `6.5.22/unix.IP27` at 0xc0000000001cf820 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cf820-0xc0000000001cf828 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001cf820 has v0 = ?.
- `idbg_makedata` in `6.5.22/unix.IP27` at 0xc0000000001cf828 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cf828-0xc0000000001cf8a0 (120 bytes).
  It occupies 120 bytes.
  The `ld` at 0xc0000000001cf850 loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `sd` at 0xc0000000001cf890 stores 8 bytes to ret(kern_calloc) +0x8.
  The `sd` at 0xc0000000001cf894 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xc0000000001cf840 calls kern_calloc [0xc000000000181e14] with a0=0x1, a1=0x18, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001cf854 calls copyin [0xc00000000001f760] with a0=[a0+24]:8, a1=ret(kern_calloc), a2=0x4, a3=?.
  The `jal` at 0xc0000000001cf864 calls kern_free [0xc000000000181f2c] with a0=ret(kern_calloc), a1=0xa0, a2=?, a3=?.
  The `jal` at 0xc0000000001cf888 calls kern_calloc [0xc000000000181e14] with a0=0x1, a1=0xa0, a2=?, a3=?.
  The `beq` branch at 0xc0000000001cf85c targets 0xc0000000001cf888, is taken when v0(=ret(copyin)) == 0, and falls through when v0(=ret(copyin)) != 0.
  The `beq` branch at 0xc0000000001cf898 targets 0xc0000000001cf870 and is always taken.
  The return at 0xc0000000001cf880 has v0 = one of 0x0, 0xa.
- `idbg_freedata` in `6.5.22/unix.IP27` at 0xc0000000001cf8a0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cf8a0-0xc0000000001cf8e8 (72 bytes).
  It occupies 72 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x18, including `ld` at 0xc0000000001cf8a0, `ld` at 0xc0000000001cf8c8.
  The `ld` at 0xc0000000001cf8b4 loads 8 bytes from [a0+24]:8 +0x8.
  The `sd` at 0xc0000000001cf8d8 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xc0000000001cf8c0 calls kern_free [0xc000000000181f2c] with a0=[[a0+24]:8+8]:8, a1=a1 at entry (arg1), a2=[a0+24]:8, a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001cf8cc calls kern_free [0xc000000000181f2c] with a0=[a0+24]:8, a1=?, a2=[a0+24]:8, a3=?.
  The `beq` branch at 0xc0000000001cf8ac targets 0xc0000000001cf8dc, is taken when a2(=[a0+24]:8) == 0, and falls through when a2(=[a0+24]:8) != 0.
  The `beq` branch at 0xc0000000001cf8b8 targets 0xc0000000001cf8cc, is taken when a0(=[[a0+24]:8+8]:8) == 0, and falls through when a0(=[[a0+24]:8+8]:8) != 0.
  The return at 0xc0000000001cf8e0 has v0 = one of ?, ret(kern_free).
- `idbg_load` in `6.5.22/unix.IP27` at 0xc0000000001cfd20 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cfd20-0xc0000000001cfdc4 (164 bytes).
  It occupies 164 bytes.
  The `ld` at 0xc0000000001cfd64 loads 8 bytes from a0 (arg0 at entry) +0x28.
  The `ld` at 0xc0000000001cfd6c loads 8 bytes from [a0+40]:8 +0x50.
  The `jal` at 0xc0000000001cfd3c calls mutex_lock [0xc00000000018dda4] with a0=0xc000000001538268, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001cfd44 calls debug_check [0xc0000000001cfc00] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001cfd5c calls debug_freeid [0xc0000000001cfbc0] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001cfd88 calls mutex_unlock [0xc00000000018dcdc] with a0=0xc000000001538268, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001cfda4 calls mloadfile [0xc0000000001d0cc8] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001cfdb4 calls debug_init [0xc0000000001cf8e8] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xc0000000001cfd4c targets 0xc0000000001cfda4, is taken when v0(=ret(debug_check)) == 0, and falls through when v0(=ret(debug_check)) != 0.
  The `beq` branch at 0xc0000000001cfd54 targets 0xc0000000001cfd84, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `bne` branch at 0xc0000000001cfdac targets 0xc0000000001cfd54, is taken when v0(=ret(mloadfile)) != 0, and falls through when v0(=ret(mloadfile)) == 0.
  The `beq` branch at 0xc0000000001cfdbc targets 0xc0000000001cfd54 and is always taken.
  The return at 0xc0000000001cfd9c has v0 = ?.
- `idbg_unload` in `6.5.22/unix.IP27` at 0xc0000000001cfdc4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cfdc4-0xc0000000001cff30 (364 bytes).
  It occupies 364 bytes.
  The `ld` at 0xc0000000001cfddc loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `ld` at 0xc0000000001cfdf4 loads 8 bytes from a0 (arg0 at entry) +0x50.
  The `ld` at 0xc0000000001cfe20 loads 8 bytes from [a0+24]:8 +0x8.
  The `ld` at 0xc0000000001cfe28 loads 8 bytes from [[a0+24]:8+8]:8 +0x68.
  The `sw` at 0xc0000000001cfe94 stores 4 bytes to abs 0xc000000001538218.
  The `ld` at 0xc0000000001cfe9c loads 8 bytes from [a0+24]:8 +0x10.
  The `ld` at 0xc0000000001cff04 loads 8 bytes from [0xc0000000014d2350]:8 +0x28.
  The `lw` at 0xc0000000001cff08 loads 4 bytes from [[0xc0000000014d2350]:8+40]:8 +0x48.
  The `sw` at 0xc0000000001cff14 stores 4 bytes to [[0xc0000000014d2350]:8+40]:8 +0x48.
  The `ld` at 0xc0000000001cff18 loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `sw` at 0xc0000000001cff2c stores 4 bytes to table(0xc000000001538218) +0x0.
  The `jal` at 0xc0000000001cfde4 calls mutex_lock [0xc00000000018dda4] with a0=0xc000000001538268, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jalr` at 0xc0000000001cfe74 calls (unknown callee) with a0=?, a1=?, a2=?, a3=-0x1, a4=0xc000000000000000, a5=0xc0000fbf00000000, a6=0xe000.
  The `jal` at 0xc0000000001cfe84 calls mutex_unlock [0xc00000000018dcdc] with a0=0xc000000001538268, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001cfea0 calls symtab_rele [0xc0000000001d0798] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001cfeac calls mutex_unlock [0xc00000000018dcdc] with a0=0xc000000001538268, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001cfed8 calls mfindname [0xc0000000001d1298] with a0=[a0+80]:8, a1=0xc0000000003e4688, a2=sp-32, a3=-0x1, a4=0xc000000000000000, a5=0xc0000fbf00000000, a6=0xe000, a7=0xffffffff00000000.
  The `jal` at 0xc0000000001cfee8 calls mutex_unlock [0xc00000000018dcdc] with a0=0xc000000001538268, a1=?, a2=?, a3=?.
  The `jalr` at 0xc0000000001cfef8 calls (unknown callee) with a0=?, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc0000000001cfe18 targets 0xc0000000001cfecc, is taken when v0(=[0xc0000000014d2350]:8) != a0(=[a0+80]:8), and falls through when v0(=[0xc0000000014d2350]:8) == a0(=[a0+80]:8).
  The `bne` branch at 0xc0000000001cfe4c targets 0xc0000000001cfe5c, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) < t3(=0xa800000000000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) >= t3(=0xa800000000000000) (unsigned).
  The `bne` branch at 0xc0000000001cfe54 targets 0xc0000000001cfe74, is taken when a7(?) != 0, and falls through when a7(?) == 0.
  The `bne` branch at 0xc0000000001cfe5c targets 0xc0000000001cfe94, is taken when a0(?) != 0, and falls through when a0(?) == 0.
  The `beq` branch at 0xc0000000001cfe6c targets 0xc0000000001cfe94, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) >= t9(=0xc0000fbfe0000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) < t9(=0xc0000fbfe0000000) (unsigned).
  The `beq` branch at 0xc0000000001cfe7c targets 0xc0000000001cfe94, is taken when v0(=ret(?)) == 0, and falls through when v0(=ret(?)) != 0.
  The `beq` branch at 0xc0000000001cfe8c targets 0xc0000000001cfeb8 and is always taken.
  The `beq` branch at 0xc0000000001cfee0 targets 0xc0000000001cfef8, is taken when v0(=ret(mfindname)) == 0, and falls through when v0(=ret(mfindname)) != 0.
  The `beq` branch at 0xc0000000001cfef0 targets 0xc0000000001cfeb8 and is always taken.
  The `beq` branch at 0xc0000000001cff28 targets 0xc0000000001cfea8 and is always taken.
  The return at 0xc0000000001cfec4 has v0 = one of 0x0, 0x33.
- `idbg_reg` in `6.5.22/unix.IP27` at 0xc0000000001cff30 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cff30-0xc0000000001cff38 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001cff30 has v0 = 0x59.
- `idbg_unreg` in `6.5.22/unix.IP27` at 0xc0000000001cff38 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001cff38-0xc0000000001cff40 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001cff38 has v0 = 0x59.
- `setup_lowmem` in `6.5.22/unix.IP27` at 0xc0000000001ee418 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001ee418-0xc0000000001ee784 (876 bytes).
  It occupies 876 bytes.
  The `sd` at 0xc0000000001ee4bc stores 8 bytes to table(0xc000000001539850) +0x0.
  3 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc0000000001ee4e0, `sd` at 0xc0000000001ee530, `sd` at 0xc0000000001ee6e8.
  The `ld` at 0xc0000000001ee4e4 loads 8 bytes from abs 0xffffffffffffc400.
  The `ld` at 0xc0000000001ee4e8 loads 8 bytes from [-0x3c00]:8 +0x0.
  2 such `ld` accesses load 8 bytes from table(0xc000000001539850) +0x0, including `ld` at 0xc0000000001ee4f0, `ld` at 0xc0000000001ee780.
  The `sd` at 0xc0000000001ee500 stores 8 bytes to ([[-0x3c00]:8]:8 + (a0 << 0x3)) +0x0.
  2 such `sd` accesses store 8 bytes to table(0xc0000000015270a0) +0x0, including `sd` at 0xc0000000001ee550, `sd` at 0xc0000000001ee5bc.
  2 such `sd` accesses store 8 bytes to table(0xc000000001516d68) +0x0, including `sd` at 0xc0000000001ee554, `sd` at 0xc0000000001ee5b4.
  The `lh` at 0xc0000000001ee570 loads 2 bytes from table(0xc0000000014d3800) +0x0.
  The `ld` at 0xc0000000001ee640 loads 8 bytes from abs 0xc000000001539850 (Nodepdaindr).
  The `ld` at 0xc0000000001ee650 loads 8 bytes from [0xc000000001539850]:8 +0x608.
  2 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc0000000001ee67c, `sw` at 0xc0000000001ee6c4.
  The `sd` at 0xc0000000001ee698 stores 8 bytes to ([[0xc000000001539850]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0.
  The `sd` at 0xc0000000001ee778 stores 8 bytes to [0xc0000000014d2690]:8 +0x400.
  The `jal` at 0xc0000000001ee468 calls blkclr [0xc00000000001fadc] with a0=?, a1=?, a2=0xa800000000000000, a3=?, a4=0xfffffffffffc0000.
  The `jal` at 0xc0000000001ee470 calls flush_cache [0xc00000000002b714] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001ee480 calls mktables [0xc0000000001ee784] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xc0000000001ee4a0 calls low_mem_alloc [0xc0000000001ee13c] with a0=0x53e0, a1=sp-112, a2=0xc0000000003e5e68, a3=?.
  The `jal` at 0xc0000000001ee4d8 calls low_mem_alloc [0xc0000000001ee13c] with a0=((([0xc0000000014d269c] + [0xc0000000014d26a0]) << 0x3) << 0x0), a1=sp-112, a2=0xc0000000003e5e78, a3=?.
  The `jal` at 0xc0000000001ee4fc calls nodepda_numa_init [0xc0000000002771f4] with a0=a0 at entry (arg0), a1=sp-112, a2=entry(0xc000000001539850), a3=([[-0x3c00]:8]:8 + (a0 << 0x3)).
  The `jal` at 0xc0000000001ee524 calls low_mem_alloc [0xc0000000001ee13c] with a0=(((([0xc0000000014d269c] << 0x4) + [0xc0000000014d269c]) << 0x3) << 0x0), a1=sp-112, a2=0xc0000000003e5e88, a3=?.
  The `jal` at 0xc0000000001ee568 calls slot_getsize [0xc000000000029d50] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001ee59c calls low_mem_alloc [0xc0000000001ee13c] with a0=?, a1=sp-112, a2=0xc0000000003e5ea0, a3=?.
  The `jal` at 0xc0000000001ee5b0 calls low_mem_alloc [0xc0000000001ee13c] with a0=?, a1=sp-112, a2=0xc0000000003e5eb8, a3=?.
  The `jal` at 0xc0000000001ee680 calls low_mem_alloc [0xc0000000001ee13c] with a0=?, a1=sp-112, a2=0xc0000000003e5ed0, a3=?, a7=[0xc0000000014d28d4].
  The `jal` at 0xc0000000001ee6c8 calls low_mem_alloc [0xc0000000001ee13c] with a0=?, a1=sp-112, a2=0xc0000000003e5ed0, a3=?.
  The `jal` at 0xc0000000001ee75c calls cmn_err_tag [0xc0000000001f80dc] with a0=0x6ec, a1=0x4, a2=0xc0000000003e5ef0, a3=?.
  The `beq` branch at 0xc0000000001ee4b8 targets 0xc0000000001ee774, is taken when s7(=a0) == 0, and falls through when s7(=a0) != 0.
  The `beq` branch at 0xc0000000001ee548 targets 0xc0000000001ee564 and is always taken.
  The `beq` branch at 0xc0000000001ee560 targets 0xc0000000001ee5c0, is taken when s0(?) >= 0x20 (signed), and falls through when s0(?) < 0x20 (signed).
  The `beq` branch at 0xc0000000001ee58c targets 0xc0000000001ee550, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `beq` branch at 0xc0000000001ee5b8 targets 0xc0000000001ee558 and is always taken.
  The `beq` branch at 0xc0000000001ee6e4 targets 0xc0000000001ee6a0, is taken when s3(=0x1000000) >= s1(?) (signed), and falls through when s3(=0x1000000) < s1(?) (signed).
  The `bne` branch at 0xc0000000001ee710 targets 0xc0000000001ee74c, is taken when t0(?) < a6(?) (unsigned), and falls through when t0(?) >= a6(?) (unsigned).
  The `beq` branch at 0xc0000000001ee76c targets 0xc0000000001ee718 and is always taken.
  The `beq` branch at 0xc0000000001ee77c targets 0xc0000000001ee4c0 and is always taken.
  The return at 0xc0000000001ee744 has v0 = ?.
- `panic_coproc_select` in `6.5.22/unix.IP27` at 0xc0000000001fa330 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc0000000001fa330-0xc0000000001fa3b8 (136 bytes).
  It occupies 136 bytes.
  2 such `lh` accesses load 2 bytes from table(0xc0000000014d3880) +0x0, including `lh` at 0xc0000000001fa370, `lh` at 0xc0000000001fa374.
  The `ll` at 0xc0000000001fa384 loads 4 bytes from abs 0xc0000000014ca774.
  The `sc` at 0xc0000000001fa390 stores 4 bytes to abs 0xc0000000014ca774.
  The `jal` at 0xc0000000001fa350 calls getcpuid [0xc000000000026ec4] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001fa3a8 calls dumpsys_helper [0xc000000000211f68] with a0=[0xc0000000014ca774]+1, a1=0x1, a2=0x1, a3=0xc0000000014ca774, a4=[0xc0000000014ca774]+1.
  The `beq` branch at 0xc0000000001fa33c targets 0xc0000000001fa350, is taken when at(=[0xc0000000014ca764]) == v0(=-0x1), and falls through when at(=[0xc0000000014ca764]) != v0(=-0x1).
  The `bne` branch at 0xc0000000001fa37c targets 0xc0000000001fa344, is taken when v1(=entry(0xc0000000014d3880)) != a0(=entry(0xc0000000014d3880)), and falls through when v1(=entry(0xc0000000014d3880)) == a0(=entry(0xc0000000014d3880)).
  The `beql` branch at 0xc0000000001fa394 targets 0xc0000000001fa384, is taken when a4(=[0xc0000000014ca774]+1) == 0, and falls through when a4(=[0xc0000000014ca774]+1) != 0; its branch-likely delay slot `nop` at 0xc0000000001fa398 runs only when taken and is annulled on fall-through.
  The `bne` branch at 0xc0000000001fa3a0 targets 0xc0000000001fa348, is taken when a0(=[0xc0000000014ca774]+1) != a1(=0x1), and falls through when a0(=[0xc0000000014ca774]+1) == a1(=0x1).
  The `beq` branch at 0xc0000000001fa3b0 targets 0xc0000000001fa348 and is always taken.
  The return at 0xc0000000001fa348 has v0 = one of ?, ret(getcpuid), ret(dumpsys_helper).
- `ioc3_early_pckminit` in `6.5.22/unix.IP30` at 0xa80000002005771c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002005771c-0xa800000020057724 (8 bytes).
  It occupies 8 bytes.
  The return at 0xa80000002005771c has v0 = ?.
- `ioc3_pckm_kbdexists` in `6.5.22/unix.IP30` at 0xa800000020058fbc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020058fbc-0xa800000020058ff8 (60 bytes).
  It occupies 60 bytes.
  The `lw` at 0xa800000020058fc8 loads 4 bytes from [0xa8000000203e15a8]:8 +0x28.
  The `ld` at 0xa800000020058fd8 loads 8 bytes from ([0xa8000000203e15a8]:8 + ([[0xa8000000203e15a8]:8+40] << 0x3)) +0x18.
  The `lw` at 0xa800000020058fdc loads 4 bytes from [([0xa8000000203e15a8]:8 + ([[0xa8000000203e15a8]:8+40] << 0x3))+24]:8 +0x4.
  The `beq` branch at 0xa800000020058fc0 targets 0xa800000020058ff0, is taken when a2(=[0xa8000000203e15a8]:8) == 0, and falls through when a2(=[0xa8000000203e15a8]:8) != 0.
  The `beq` branch at 0xa800000020058fcc targets 0xa800000020058ff0, is taken when a3(=[[0xa8000000203e15a8]:8+40]) == v0(=0x13), and falls through when a3(=[[0xa8000000203e15a8]:8+40]) != v0(=0x13).
  The return at 0xa800000020058fe8 has v0 = ?.
  The return at 0xa800000020058ff0 has v0 = 0x0.
- `ioc3_pckm_setleds` in `6.5.22/unix.IP30` at 0xa800000020058ff8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020058ff8-0xa800000020059034 (60 bytes).
  It occupies 60 bytes.
  The `lw` at 0xa800000020059010 loads 4 bytes from [0xa8000000203e15a8]:8 +0x28.
  The `ld` at 0xa800000020059024 loads 8 bytes from ([0xa8000000203e15a8]:8 + ([[0xa8000000203e15a8]:8+40] << 0x3)) +0x18.
  The `jal` at 0xa800000020059020 calls ioc3_lcl_pckm_setleds [0xa8000000200590ac] with a0=[([0xa8000000203e15a8]:8 + ([[0xa8000000203e15a8]:8+40] << 0x3))+24]:8, a1=a0 at entry (arg0), a2=a2 at entry (arg2), a3=[0xa8000000203e15a8]:8, a4=0x13, a5=[[0xa8000000203e15a8]:8+40].
  The `beq` branch at 0xa800000020059008 targets 0xa800000020059028, is taken when a3(=[0xa8000000203e15a8]:8) == 0, and falls through when a3(=[0xa8000000203e15a8]:8) != 0.
  The `beq` branch at 0xa800000020059014 targets 0xa800000020059028, is taken when a5(=[[0xa8000000203e15a8]:8+40]) == a4(=0x13), and falls through when a5(=[[0xa8000000203e15a8]:8+40]) != a4(=0x13).
  The return at 0xa80000002005902c has v0 = one of ?, ret(ioc3_lcl_pckm_setleds).
- `ioc3_pckm_bell` in `6.5.22/unix.IP30` at 0xa800000020059034 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020059034-0xa800000020059078 (68 bytes).
  It occupies 68 bytes.
  The `lw` at 0xa800000020059048 loads 4 bytes from [0xa8000000203e15a8]:8 +0x28.
  The `lw` at 0xa800000020059054 loads 4 bytes from [0xa8000000203e15a8]:8 +0x34.
  The `ld` at 0xa800000020059068 loads 8 bytes from ([0xa8000000203e15a8]:8 + ([[0xa8000000203e15a8]:8+40] << 0x3)) +0x18.
  The `jal` at 0xa800000020059064 calls ioc3_lcl_pckm_setleds [0xa8000000200590ac] with a0=[([0xa8000000203e15a8]:8 + ([[0xa8000000203e15a8]:8+40] << 0x3))+24]:8, a1=([[0xa8000000203e15a8]:8+52] | 0x8), a2=a2 at entry (arg2), a3=[0xa8000000203e15a8]:8, a4=[[0xa8000000203e15a8]:8+40].
  The `beq` branch at 0xa800000020059040 targets 0xa80000002005906c, is taken when a3(=[0xa8000000203e15a8]:8) == 0, and falls through when a3(=[0xa8000000203e15a8]:8) != 0.
  The `beq` branch at 0xa80000002005904c targets 0xa800000020059070, is taken when a4(=[[0xa8000000203e15a8]:8+40]) == a1(=0x13), and falls through when a4(=[[0xa8000000203e15a8]:8+40]) != a1(=0x13).
  The return at 0xa800000020059070 has v0 = ?.
- `ioc3_gl_setporthandler` in `6.5.22/unix.IP30` at 0xa800000020059078 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020059078-0xa8000000200590a4 (44 bytes).
  It occupies 44 bytes.
  The `lw` at 0xa800000020059084 loads 4 bytes from [0xa8000000203e15a8]:8 +0x28.
  The `ld` at 0xa800000020059094 loads 8 bytes from ([0xa8000000203e15a8]:8 + ([[0xa8000000203e15a8]:8+40] << 0x3)) +0x18.
  The `sd` at 0xa800000020059098 stores 8 bytes to [([0xa8000000203e15a8]:8 + ([[0xa8000000203e15a8]:8+40] << 0x3))+24]:8 +0x18.
  The `beq` branch at 0xa80000002005907c targets 0xa80000002005909c, is taken when a2(=[0xa8000000203e15a8]:8) == 0, and falls through when a2(=[0xa8000000203e15a8]:8) != 0.
  The `beq` branch at 0xa800000020059088 targets 0xa80000002005909c, is taken when a4(=[[0xa8000000203e15a8]:8+40]) == a3(=0x13), and falls through when a4(=[[0xa8000000203e15a8]:8+40]) != a3(=0x13).
  The return at 0xa80000002005909c has v0 = ?.
- `ioc3_du_keyboard_port` in `6.5.22/unix.IP30` at 0xa8000000200590a4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa8000000200590a4-0xa8000000200590ac (8 bytes).
  It occupies 8 bytes.
  The return at 0xa8000000200590a4 has v0 = -0x1.
- `ioc3_lcl_pckm_setleds` in `6.5.22/unix.IP30` at 0xa8000000200590ac (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa8000000200590ac-0xa800000020059100 (84 bytes).
  It occupies 84 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x48, including `ld` at 0xa8000000200590b8, `ld` at 0xa8000000200590e0.
  The `jal` at 0xa8000000200590cc calls mutex_lock [0xa80000002013b39c] with a0=[a0+72]:8+72, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa8000000200590d8 calls ioc3_lcl_pckm_setleds_lock [0xa800000020059100] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xa8000000200590e8 calls mutex_unlock [0xa80000002013b2d4] with a0=[a0+72]:8+72, a1=?, a2=?, a3=?.
  The return at 0xa8000000200590f8 has v0 = ret(mutex_unlock).
- `ioc3_lcl_pckm_setleds_lock` in `6.5.22/unix.IP30` at 0xa800000020059100 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020059100-0xa800000020059278 (376 bytes).
  It occupies 376 bytes.
  The `ld` at 0xa80000002005910c loads 8 bytes from a0 (arg0 at entry) +0x48.
  5 such `lw` accesses load 4 bytes from [a0+72]:8 +0x34, including `lw` at 0xa800000020059114, `lw` at 0xa800000020059188, `lw` at 0xa8000000200591a8.
  The `lw` at 0xa800000020059144 loads 4 bytes from [a0+72]:8 +0x38.
  The `sw` at 0xa800000020059170 stores 4 bytes to [a0+72]:8 +0x38.
  2 such `sw` accesses store 4 bytes to [a0+72]:8 +0x34, including `sw` at 0xa80000002005917c, `sw` at 0xa8000000200591b0.
  The `jal` at 0xa800000020059184 calls pckm_pollcmd [0xa800000020058ee0] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xa800000020059190 calls data_check [0xa800000020059340] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xa8000000200591dc calls pckm_pollcmd [0xa800000020058ee0] with a0=a0 at entry (arg0), a1=0xed, a2=?, a3=?.
  The `jal` at 0xa8000000200591e8 calls data_check [0xa800000020059340] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xa800000020059208 calls pckm_pollcmd [0xa800000020058ee0] with a0=a0 at entry (arg0), a1=0xf4, a2=?, a3=?.
  The `jal` at 0xa800000020059214 calls data_check [0xa800000020059340] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xa80000002005922c calls default_kbd_setting [0xa8000000200587e0] with a0=a0 at entry (arg0), a1=0x0, a2=[[a0+72]:8+52], a3=?.
  The `jal` at 0xa80000002005924c calls pckm_pollcmd [0xa800000020058ee0] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xa800000020059258 calls data_check [0xa800000020059340] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xa800000020059268 calls pckm_reinit_core [0xa800000020058768] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xa800000020059134 targets 0xa8000000200591b4, is taken when at(=[[a0+72]:8+52]) == a3(?), and falls through when at(=[[a0+72]:8+52]) != a3(?).
  The `beq` branch at 0xa80000002005913c targets 0xa800000020059178, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xa800000020059150 targets 0xa800000020059160, is taken when a7(?) >= 0xe (signed), and falls through when a7(?) < 0xe (signed).
  The `bne` branch at 0xa800000020059158 targets 0xa8000000200591b8, is taken when a4(?) != 0, and falls through when a4(?) == 0.
  The `beq` branch at 0xa800000020059160 targets 0xa800000020059170, is taken when a0(?) == 0, and falls through when a0(?) != 0.
  The `bne` branch at 0xa800000020059168 targets 0xa8000000200591b8, is taken when a5(?) != 0, and falls through when a5(?) == 0.
  The `beq` branch at 0xa800000020059178 targets 0xa8000000200591d8 and is always taken.
  The `beq` branch at 0xa800000020059198 targets 0xa80000002005924c, is taken when v0(=ret(data_check)) == 0, and falls through when v0(=ret(data_check)) != 0.
  The `beq` branch at 0xa8000000200591a0 targets 0xa8000000200591b8, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xa8000000200591d4 targets 0xa80000002005923c, is taken when t1(?) == 0, and falls through when t1(?) != 0.
  The `bne` branch at 0xa8000000200591f0 targets 0xa800000020059180, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `bne` branch at 0xa8000000200591fc targets 0xa8000000200591d0, is taken when s3(?) < 0x2 (unsigned), and falls through when s3(?) >= 0x2 (unsigned).
  The `bne` branch at 0xa80000002005921c targets 0xa8000000200591d4, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `beq` branch at 0xa800000020059234 targets 0xa8000000200591d4 and is always taken.
  The `bne` branch at 0xa80000002005923c targets 0xa8000000200591a8, is taken when s2(?) != 0, and falls through when s2(?) == 0.
  The `beq` branch at 0xa800000020059244 targets 0xa8000000200591b8 and is always taken.
  The `bne` branch at 0xa800000020059260 targets 0xa8000000200591a0, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `beq` branch at 0xa800000020059270 targets 0xa8000000200591a0 and is always taken.
  The return at 0xa8000000200591c8 has v0 = ?.
- `dev_chain` in `6.5.22/unix.IP30` at 0xa80000002010308c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002010308c-0xa800000020103178 (236 bytes).
  It occupies 236 bytes.
  The `ld` at 0xa800000020103098 loads 8 bytes from a0 (arg0 at entry) +0x38.
  2 such `lw` accesses load 4 bytes from abs 0xa80000002041de60 (v+48), including `lw` at 0xa8000000201030ac, `lw` at 0xa800000020103110.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x50, including `lw` at 0xa8000000201030b4, `lw` at 0xa8000000201030f8.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x28, including `ld` at 0xa8000000201030d4, `ld` at 0xa8000000201030e8.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x30, including `ld` at 0xa8000000201030d8, `ld` at 0xa8000000201030e0.
  The `sd` at 0xa8000000201030dc stores 8 bytes to [a0+40]:8 +0x30.
  The `sd` at 0xa8000000201030f4 stores 8 bytes to [a0+48]:8 +0x28.
  2 such `ld` accesses load 8 bytes from ?, including `ld` at 0xa800000020103130, `ld` at 0xa80000002010313c.
  The `sd` at 0xa800000020103134 stores 8 bytes to a0 (arg0 at entry) +0x30.
  The `sd` at 0xa800000020103138 stores 8 bytes to a0 (arg0 at entry) +0x28.
  2 such `sd` accesses store 8 bytes to ?, including `sd` at 0xa800000020103140, `sd` at 0xa800000020103148.
  The `sd` at 0xa800000020103154 stores 8 bytes to a0 (arg0 at entry) +0x38.
  The `jal` at 0xa8000000201030cc calls mutex_spinlock [0xa800000020210f74] with a0=[a0+56]:8+56, a1=a1 at entry (arg1), a2=[a0+56]:8, a3=[0xa80000002041de60].
  The `jal` at 0xa8000000201030f0 calls mutex_spinunlock [0xa800000020210fc0] with a0=[a0+56]:8+56, a1=ret(mutex_spinlock), a2=?, a3=?, a6=[a0+40]:8, a7=[a0+48]:8.
  The `jal` at 0xa800000020103128 calls mutex_spinlock [0xa800000020210f74] with a0=?, a1=?, a2=0xa80000002040c900, a3=0xa80000002041de30.
  The `jal` at 0xa800000020103150 calls mutex_spinunlock [0xa800000020210fc0] with a0=?, a1=ret(mutex_spinlock), a2=?, a3=?.
  The `beq` branch at 0xa8000000201030a8 targets 0xa8000000201030f8, is taken when a2(=[a0+56]:8) == 0, and falls through when a2(=[a0+56]:8) != 0.
  The `beq` branch at 0xa8000000201030c4 targets 0xa80000002010315c, is taken when a2(=[a0+56]:8) == a4(?), and falls through when a2(=[a0+56]:8) != a4(?).
  The `beq` branch at 0xa800000020103108 targets 0xa800000020103170, is taken when a0(=[a0+80]) == t2(=-0x1), and falls through when a0(=[a0+80]) != t2(=-0x1).
  The `beq` branch at 0xa800000020103170 targets 0xa800000020103124 and is always taken.
  The return at 0xa800000020103168 has v0 = one of ?, ret(mutex_spinunlock).
- `ioc3_config` in `6.5.22/unix.IP30` at 0xa80000002012b160 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012b160-0xa80000002012b334 (468 bytes).
  It occupies 468 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xa80000002012b1f8, `lw` at 0xa80000002012b2a4.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa80000002012b208, `ld` at 0xa80000002012b218, `ld` at 0xa80000002012b2b4.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xa80000002012b214, `sw` at 0xa80000002012b2b8.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xa80000002012b21c, `lw` at 0xa80000002012b248.
  The `ld` at 0xa80000002012b25c loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xa80000002012b260 loads 1 byte from [a0+72]:8 +0x3.
  The `sb` at 0xa80000002012b2a0 stores 1 byte to [a0+72]:8 +0x3.
  The `sw` at 0xa80000002012b2e4 stores 4 bytes to a0 (arg0 at entry) +0x80.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x84, including `sw` at 0xa80000002012b2ec, `sw` at 0xa80000002012b308.
  The `lw` at 0xa80000002012b2f8 loads 4 bytes from a0 (arg0 at entry) +0x88.
  The `jal` at 0xa80000002012b184 calls set_baud_ti [0xa80000002012a8c4] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa80000002012b2f4 calls ioc3_rx_timeout [0xa80000002012be2c] with a0=a0 at entry (arg0), a1=[a0+136], a2=[a0+148], a3=?, a6=a1 at entry (arg1), a7=0x3e8.
  The `beq` branch at 0xa80000002012b18c targets 0xa80000002012b1ac, is taken when v0(=ret(set_baud_ti)) == 0, and falls through when v0(=ret(set_baud_ti)) != 0.
  The `beq` branch at 0xa80000002012b1c0 targets 0xa80000002012b1f0, is taken when s1(=a2) == at(=0x5), and falls through when s1(=a2) != at(=0x5).
  The `beq` branch at 0xa80000002012b1c8 targets 0xa80000002012b30c, is taken when s1(=a2) == a0(=0x6), and falls through when s1(=a2) != a0(=0x6).
  The `beq` branch at 0xa80000002012b1d0 targets 0xa80000002012b314, is taken when s1(=a2) == a2(=0x7), and falls through when s1(=a2) != a2(=0x7).
  The `beq` branch at 0xa80000002012b1d8 targets 0xa80000002012b1e8, is taken when s1(=a2) == a7(=0x8), and falls through when s1(=a2) != a7(=0x8).
  The `beq` branch at 0xa80000002012b1e0 targets 0xa80000002012b19c and is always taken.
  The `beq` branch at 0xa80000002012b1e8 targets 0xa80000002012b1f8 and is always taken.
  The `bgez` branch at 0xa80000002012b200 targets 0xa80000002012b25c, is taken when v0(=([a0+148] << 0x23)) >= 0 (signed), and falls through when v0(=([a0+148] << 0x23)) < 0 (signed).
  The `bltz` branch at 0xa80000002012b230 targets 0xa80000002012b25c, is taken when t3(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t3(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `bne` branch at 0xa80000002012b240 targets 0xa80000002012b31c, is taken when t9(?) != 0, and falls through when t9(?) == 0.
  The `beq` branch at 0xa80000002012b254 targets 0xa80000002012b240, is taken when (ra(=[[a0+64]:8]) & t1(=0x40000000)) == 0, and falls through when (ra(=[[a0+64]:8]) & t1(=0x40000000)) != 0.
  The `beq` branch at 0xa80000002012b280 targets 0xa80000002012b294, is taken when v1(=a4) == 0, and falls through when v1(=a4) != 0.
  The `beq` branch at 0xa80000002012b28c targets 0xa80000002012b324, is taken when a0(=a5) == 0, and falls through when a0(=a5) != 0.
  The `beq` branch at 0xa80000002012b294 targets 0xa80000002012b2a0, is taken when a1(=a3) == 0, and falls through when a1(=a3) != 0.
  The `beq` branch at 0xa80000002012b2ac targets 0xa80000002012b2bc, is taken when (t2(=0x10000000) & a2(=[a0+148])) == 0, and falls through when (t2(=0x10000000) & a2(=[a0+148])) != 0.
  The `beq` branch at 0xa80000002012b2e8 targets 0xa80000002012b304, is taken when a5(?) == 0, and falls through when a5(?) != 0.
  The `beq` branch at 0xa80000002012b2fc targets 0xa80000002012b198 and is always taken.
  The `beq` branch at 0xa80000002012b304 targets 0xa80000002012b2f0 and is always taken.
  The `beq` branch at 0xa80000002012b30c targets 0xa80000002012b1f4 and is always taken.
  The `beq` branch at 0xa80000002012b314 targets 0xa80000002012b1f8 and is always taken.
  The `beq` branch at 0xa80000002012b31c targets 0xa80000002012b19c and is always taken.
  The `bne` branch at 0xa80000002012b324 targets 0xa80000002012b29c, is taken when t3(=a3) != 0, and falls through when t3(=a3) == 0.
  The `beq` branch at 0xa80000002012b32c targets 0xa80000002012b2a0 and is always taken.
  The return at 0xa80000002012b1a4 has v0 = one of -0x1, 0x0, 0x1.
- `ioc3_set_extclk` in `6.5.22/unix.IP30` at 0xa80000002012b368 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012b368-0xa80000002012b370 (8 bytes).
  It occupies 8 bytes.
  The return at 0xa80000002012b368 has v0 = a1.
- `ioc3_write` in `6.5.22/unix.IP30` at 0xa80000002012b50c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012b50c-0xa80000002012b528 (28 bytes).
  It occupies 28 bytes.
  The `jal` at 0xa80000002012b514 calls do_ioc3_write [0xa80000002012b370] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The return at 0xa80000002012b520 has v0 = ret(do_ioc3_write).
- `ioc3_sync_write` in `6.5.22/unix.IP30` at 0xa80000002012b528 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012b528-0xa80000002012b58c (100 bytes).
  It occupies 100 bytes.
  The `lw` at 0xa80000002012b558 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xa80000002012b53c calls do_ioc3_write [0xa80000002012b370] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa80000002012b56c calls ioc3_enable_tx [0xa80000002012b700] with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `blez` branch at 0xa80000002012b544 targets 0xa80000002012b574, is taken when s3(=a2) <= 0 (signed), and falls through when s3(=a2) > 0 (signed).
  The `bne` branch at 0xa80000002012b54c targets 0xa80000002012b578, is taken when v0(=ret(do_ioc3_write)) != 0, and falls through when v0(=ret(do_ioc3_write)) == 0.
  The `beq` branch at 0xa80000002012b560 targets 0xa80000002012b578, is taken when (at(=[a0+120]) & 0x10) == 0, and falls through when (at(=[a0+120]) & 0x10) != 0.
  The return at 0xa80000002012b584 has v0 = ret(do_ioc3_write).
- `ioc3_wrflush` in `6.5.22/unix.IP30` at 0xa80000002012b58c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012b58c-0xa80000002012b650 (196 bytes).
  It occupies 196 bytes.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x38, including `ld` at 0xa80000002012b58c, `ld` at 0xa80000002012b5c8, `ld` at 0xa80000002012b610.
  3 such `lw` accesses load 4 bytes from [a0+56]:8 +0x1c, including `lw` at 0xa80000002012b590, `lw` at 0xa80000002012b5cc, `lw` at 0xa80000002012b614.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa80000002012b5ac, `ld` at 0xa80000002012b604, `ld` at 0xa80000002012b62c.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xa80000002012b5b0, `lw` at 0xa80000002012b630.
  The `lw` at 0xa80000002012b5e8 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xa80000002012b5f8 calls ioc3_enable_tx [0xa80000002012b700] with a0=a0 at entry (arg0), a1=0x1, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa80000002012b624 calls us_delay [0xa8000000200053c0] with a0=0x5, a1=?, a2=one of 0x10000000, 0x50000000, a3=?.
  The `beq` branch at 0xa80000002012b5a4 targets 0xa80000002012b5e8, is taken when (at(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (at(=[[a0+56]:8+28]) & 0x1) != 0.
  The `bne` branch at 0xa80000002012b5c0 targets 0xa80000002012b5dc, is taken when a0(=0x10000000) != v0(?), and falls through when a0(=0x10000000) == v0(?).
  The `beq` branch at 0xa80000002012b5d4 targets 0xa80000002012b608, is taken when (v1(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (v1(=[[a0+56]:8+28]) & 0x1) != 0.
  The `beq` branch at 0xa80000002012b5f0 targets 0xa80000002012b5ac, is taken when (a0(=[a0+120]) & 0x10) == 0, and falls through when (a0(=[a0+120]) & 0x10) != 0.
  The `beq` branch at 0xa80000002012b600 targets 0xa80000002012b5b0 and is always taken.
  The `beq` branch at 0xa80000002012b608 targets 0xa80000002012b624 and is always taken.
  The `bne` branch at 0xa80000002012b61c targets 0xa80000002012b648, is taken when (a1(=[[a0+56]:8+28]) & 0x1) != 0, and falls through when (a1(=[[a0+56]:8+28]) & 0x1) == 0.
  The `beq` branch at 0xa80000002012b640 targets 0xa80000002012b610, is taken when a2(=0x10000000) == a3(?), and falls through when a2(=0x10000000) != a3(?).
  The `beq` branch at 0xa80000002012b648 targets 0xa80000002012b5dc and is always taken.
  The return at 0xa80000002012b5e0 has v0 = one of ?, ret(us_delay).
- `ioc3_break` in `6.5.22/unix.IP30` at 0xa80000002012b650 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012b650-0xa80000002012b700 (176 bytes).
  It occupies 176 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xa80000002012b650, `lw` at 0xa80000002012b6cc.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa80000002012b664, `ld` at 0xa80000002012b674, `ld` at 0xa80000002012b6dc.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xa80000002012b670, `sw` at 0xa80000002012b6e0.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xa80000002012b678, `lw` at 0xa80000002012b6a4.
  The `ld` at 0xa80000002012b6b8 loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xa80000002012b6c0 loads 1 byte from [a0+72]:8 +0x3.
  2 such `sb` accesses store 1 byte to [a0+72]:8 +0x3, including `sb` at 0xa80000002012b6c8, `sb` at 0xa80000002012b6fc.
  The `bgez` branch at 0xa80000002012b658 targets 0xa80000002012b6b8, is taken when at(=([a0+148] << 0x23)) >= 0 (signed), and falls through when at(=([a0+148] << 0x23)) < 0 (signed).
  The `bltz` branch at 0xa80000002012b68c targets 0xa80000002012b6b8, is taken when t1(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t1(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `bne` branch at 0xa80000002012b69c targets 0xa80000002012b6e8, is taken when t3(?) != 0, and falls through when t3(?) == 0.
  The `beq` branch at 0xa80000002012b6b0 targets 0xa80000002012b69c, is taken when (t8(=[[a0+64]:8]) & a7(=0x40000000)) == 0, and falls through when (t8(=[[a0+64]:8]) & a7(=0x40000000)) != 0.
  The `beq` branch at 0xa80000002012b6bc targets 0xa80000002012b6f0, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xa80000002012b6d4 targets 0xa80000002012b6e8, is taken when (t0(=0x10000000) & a4(=[a0+148])) == 0, and falls through when (t0(=0x10000000) & a4(=[a0+148])) != 0.
  The `beq` branch at 0xa80000002012b6f8 targets 0xa80000002012b6cc and is always taken.
  The return at 0xa80000002012b6e8 has v0 = one of -0x1, 0x0.
- `ioc3_enable_tx` in `6.5.22/unix.IP30` at 0xa80000002012b700 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012b700-0xa80000002012b840 (320 bytes).
  It occupies 320 bytes.
  The `ld` at 0xa80000002012b700 loads 8 bytes from a0 (arg0 at entry) +0x70.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xa80000002012b708, `lw` at 0xa80000002012b78c.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xa80000002012b728, `lw` at 0xa80000002012b7b8.
  4 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa80000002012b734, `ld` at 0xa80000002012b748, `ld` at 0xa80000002012b7cc.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xa80000002012b744, `sw` at 0xa80000002012b7d0.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xa80000002012b74c, `lw` at 0xa80000002012b778.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x98, including `lw` at 0xa80000002012b798, `lw` at 0xa80000002012b818.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xa80000002012b7a4, `sw` at 0xa80000002012b800.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x4, including `sw` at 0xa80000002012b7a8, `sw` at 0xa80000002012b834.
  2 such `lw` accesses load 4 bytes from [a0+112]:8 +0x8, including `lw` at 0xa80000002012b7b4, `lw` at 0xa80000002012b808.
  The `lw` at 0xa80000002012b7f8 loads 4 bytes from [a0+64]:8 +0x8.
  The `jal` at 0xa80000002012b7b0 calls enable_intrs [0xa80000002012a4d0] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=[a0+64]:8, a5=[a0+112]:8.
  The `jal` at 0xa80000002012b810 calls disable_intrs [0xa80000002012a520] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=[a0+64]:8, a5=[a0+112]:8.
  The `beq` branch at 0xa80000002012b718 targets 0xa80000002012b7e8, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xa80000002012b720 targets 0xa80000002012b7f0, is taken when a4(?) == 0, and falls through when a4(?) != 0.
  The `bgez` branch at 0xa80000002012b730 targets 0xa80000002012b790, is taken when at(=([a0+148] << 0x23)) >= 0 (signed), and falls through when at(=([a0+148] << 0x23)) < 0 (signed).
  The `bltz` branch at 0xa80000002012b760 targets 0xa80000002012b78c, is taken when t1(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t1(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `bne` branch at 0xa80000002012b770 targets 0xa80000002012b838, is taken when t3(?) != 0, and falls through when t3(?) == 0.
  The `beq` branch at 0xa80000002012b784 targets 0xa80000002012b770, is taken when (t8(=[[a0+64]:8]) & a7(=0x40000000)) == 0, and falls through when (t8(=[[a0+64]:8]) & a7(=0x40000000)) != 0.
  The `beq` branch at 0xa80000002012b790 targets 0xa80000002012b7f8, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xa80000002012b7c4 targets 0xa80000002012b7d8, is taken when (at(=0x10000000) & a0(=[a0+148])) == 0, and falls through when (at(=0x10000000) & a0(=[a0+148])) != 0.
  The `beq` branch at 0xa80000002012b7e8 targets 0xa80000002012b728, is taken when a4(?) == 0, and falls through when a4(?) != 0.
  The `beq` branch at 0xa80000002012b7f0 targets 0xa80000002012b7d8 and is always taken.
  The `beq` branch at 0xa80000002012b824 targets 0xa80000002012b7b8, is taken when a1(=[a0+152]) == v0(?), and falls through when a1(=[a0+152]) != v0(?).
  The `beq` branch at 0xa80000002012b830 targets 0xa80000002012b7b8 and is always taken.
  The `beq` branch at 0xa80000002012b838 targets 0xa80000002012b7dc and is always taken.
  The return at 0xa80000002012b7e0 has v0 = one of -0x1, 0x0.
- `ioc3_read` in `6.5.22/unix.IP30` at 0xa80000002012b840 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012b840-0xa80000002012bcf4 (1204 bytes).
  It occupies 1204 bytes.
  The `ld` at 0xa80000002012b84c loads 8 bytes from a0 (arg0 at entry) +0x70.
  8 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa80000002012b864, `ld` at 0xa80000002012b87c, `ld` at 0xa80000002012b8d8.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x9c, including `lw` at 0xa80000002012b868, `lw` at 0xa80000002012b88c.
  3 such `sw` accesses store 4 bytes to [a0+64]:8 +0x10, including `sw` at 0xa80000002012b878, `sw` at 0xa80000002012ba54, `sw` at 0xa80000002012bcb0.
  4 such `lw` accesses load 4 bytes from [a0+64]:8 +0xc, including `lw` at 0xa80000002012b884, `lw` at 0xa80000002012b8dc, `lw` at 0xa80000002012b924.
  3 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xa80000002012b8b4, `lw` at 0xa80000002012b934, `lw` at 0xa80000002012ba68.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xa80000002012b8d4, `sw` at 0xa80000002012b940.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xa80000002012b8f4, `lw` at 0xa80000002012b910.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xa80000002012b93c, `sw` at 0xa80000002012bc4c.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xa80000002012b944, `lw` at 0xa80000002012b9b8, `lw` at 0xa80000002012ba58.
  The `ld` at 0xa80000002012b948 loads 8 bytes from a0 (arg0 at entry) +0x60.
  4 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xa80000002012b95c, `sw` at 0xa80000002012bbdc, `sw` at 0xa80000002012bc3c.
  6 such `lw` accesses load 4 bytes from ?, including `lw` at 0xa80000002012b978, `lw` at 0xa80000002012ba38, `lw` at 0xa80000002012bbac.
  7 such `lbu` accesses load 1 byte from ?, including `lbu` at 0xa80000002012b994, `lbu` at 0xa80000002012b9fc, `lbu` at 0xa80000002012baec.
  5 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x8c, including `lw` at 0xa80000002012b9a8, `lw` at 0xa80000002012ba20, `lw` at 0xa80000002012bafc.
  5 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x8, including `ld` at 0xa80000002012b9e0, `ld` at 0xa80000002012bad0, `ld` at 0xa80000002012bb0c.
  4 such `ld` accesses load 8 bytes from [a0+8]:8 +0x10, including `ld` at 0xa80000002012b9ec, `ld` at 0xa80000002012badc, `ld` at 0xa80000002012bb18.
  4 such `sb` accesses store 1 byte to ?, including `sb` at 0xa80000002012ba08, `sb` at 0xa80000002012bb7c, `sb` at 0xa80000002012bb90.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x9c, including `sw` at 0xa80000002012ba64, `sw` at 0xa80000002012bcc0.
  The `lw` at 0xa80000002012bbe8 loads 4 bytes from [a0+112]:8 +0x10.
  The `ld` at 0xa80000002012bcd0 loads 8 bytes from [a0+8]:8 +0x18.
  The `jalr` at 0xa80000002012b9f4 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x8, a2=[a0+140], a3=?.
  The `jalr` at 0xa80000002012bae4 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x2, a2=[a0+140], a3=?, a7=[[a0+8]:8+16]:8.
  The `jalr` at 0xa80000002012bb20 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x4, a2=?, a3=?.
  The `jalr` at 0xa80000002012bb5c calls (unknown callee) with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `jal` at 0xa80000002012bbe4 calls enable_intrs [0xa80000002012a4d0] with a0=a0 at entry (arg0), a1=[[a0+112]:8+16], a2=?, a3=?.
  The `jalr` at 0xa80000002012bcd8 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x0, a2=[a0+140], a3=?.
  The `bne` branch at 0xa80000002012b8ac targets 0xa80000002012b944, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bgez` branch at 0xa80000002012b8c0 targets 0xa80000002012bc40, is taken when v1(=([a0+148] << 0x23)) >= 0 (signed), and falls through when v1(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xa80000002012b8ec targets 0xa80000002012b92c, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bgez` branch at 0xa80000002012b8fc targets 0xa80000002012b924, is taken when t9(=([[a0+64]:8] << 0x24)) >= 0 (signed), and falls through when t9(=([[a0+64]:8] << 0x24)) < 0 (signed).
  The `bne` branch at 0xa80000002012b908 targets 0xa80000002012bbf4, is taken when ra(?) != 0, and falls through when ra(?) == 0.
  The `bne` branch at 0xa80000002012b91c targets 0xa80000002012b908, is taken when (at(=[[a0+64]:8]) & a6(=0x8000000)) != 0, and falls through when (at(=[[a0+64]:8]) & a6(=0x8000000)) == 0.
  The `beq` branch at 0xa80000002012b92c targets 0xa80000002012b944, is taken when a7(=one of 0x0, 0x1) == 0, and falls through when a7(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xa80000002012b958 targets 0xa80000002012ba4c, is taken when s5(?) == s1(=[a0+156]), and falls through when s5(?) != s1(=[a0+156]).
  The `blez` branch at 0xa80000002012b960 targets 0xa80000002012bc58, is taken when s6(=a2) <= 0 (signed), and falls through when s6(=a2) > 0 (signed).
  The `beq` branch at 0xa80000002012b980 targets 0xa80000002012bbfc, is taken when (s4(=0xffffffffc0c0c0c0) & v0(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & v0(?)) != 0.
  The `blez` branch at 0xa80000002012b988 targets 0xa80000002012bbac, is taken when s6(?) <= 0 (signed), and falls through when s6(?) > 0 (signed).
  The `beq` branch at 0xa80000002012b9a0 targets 0xa80000002012ba0c, is taken when (a0(?) & 0x40) == 0, and falls through when (a0(?) & 0x40) != 0.
  The `beq` branch at 0xa80000002012b9b0 targets 0xa80000002012b9d0, is taken when (a2(=[a0+140]) & 0x40) == 0, and falls through when (a2(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xa80000002012b9c0 targets 0xa80000002012b9d0, is taken when (v0(=[a0+120]) & 0x2) == 0, and falls through when (v0(=[a0+120]) & 0x2) != 0.
  The `beq` branch at 0xa80000002012b9c8 targets 0xa80000002012bc64, is taken when a6(?) == 0, and falls through when a6(?) != 0.
  The `beq` branch at 0xa80000002012b9d0 targets 0xa80000002012ba04, is taken when a5(?) == 0, and falls through when a5(?) != 0.
  The `beq` branch at 0xa80000002012b9d8 targets 0xa80000002012ba04, is taken when a7(?) == 0, and falls through when a7(?) != 0.
  The `beq` branch at 0xa80000002012b9e4 targets 0xa80000002012ba00, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xa80000002012ba10 targets 0xa80000002012bb94, is taken when (a0(?) & 0x80) == 0, and falls through when (a0(?) & 0x80) != 0.
  The `beq` branch at 0xa80000002012ba18 targets 0xa80000002012bb6c, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xa80000002012ba28 targets 0xa80000002012bb6c, is taken when (a2(=[a0+140]) & 0x1c) == 0, and falls through when (a2(=[a0+140]) & 0x1c) != 0.
  The `blez` branch at 0xa80000002012ba30 targets 0xa80000002012babc, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xa80000002012ba44 targets 0xa80000002012ba50 and is always taken.
  The `beq` branch at 0xa80000002012ba60 targets 0xa80000002012ba88, is taken when (v0(=[a0+120]) & 0x1) == 0, and falls through when (v0(=[a0+120]) & 0x1) != 0.
  The `bne` branch at 0xa80000002012ba80 targets 0xa80000002012bbd4, is taken when a4(?) < a5(?) (unsigned), and falls through when a4(?) >= a5(?) (unsigned).
  The `beq` branch at 0xa80000002012babc targets 0xa80000002012baf4, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xa80000002012bac8 targets 0xa80000002012baf4, is taken when (a2(=[a0+140]) & 0x8) == 0, and falls through when (a2(=[a0+140]) & 0x8) != 0.
  The `beq` branch at 0xa80000002012bad4 targets 0xa80000002012baf4, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xa80000002012baf4 targets 0xa80000002012bb30, is taken when t0(?) == 0, and falls through when t0(?) != 0.
  The `beq` branch at 0xa80000002012bb04 targets 0xa80000002012bb30, is taken when (t1(=[a0+140]) & 0x10) == 0, and falls through when (t1(=[a0+140]) & 0x10) != 0.
  The `beq` branch at 0xa80000002012bb10 targets 0xa80000002012bb30, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xa80000002012bb30 targets 0xa80000002012bb6c, is taken when t3(?) == 0, and falls through when t3(?) != 0.
  The `beq` branch at 0xa80000002012bb40 targets 0xa80000002012bb6c, is taken when (t8(=[a0+140]) & 0x4) == 0, and falls through when (t8(=[a0+140]) & 0x4) != 0.
  The `beq` branch at 0xa80000002012bb4c targets 0xa80000002012bb6c, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xa80000002012bb9c targets 0xa80000002012bbac, is taken when s8(?) >= 0x4 (signed), and falls through when s8(?) < 0x4 (signed).
  The `bgtz` branch at 0xa80000002012bba4 targets 0xa80000002012b994, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xa80000002012bbb4 targets 0xa80000002012bc50, is taken when (s4(=0xffffffffc0c0c0c0) & a0(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a0(?)) != 0.
  The `beq` branch at 0xa80000002012bbbc targets 0xa80000002012ba50, is taken when s5(?) == s1(?), and falls through when s5(?) != s1(?).
  The `bgtz` branch at 0xa80000002012bbc4 targets 0xa80000002012b970, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xa80000002012bbcc targets 0xa80000002012ba54 and is always taken.
  The `beq` branch at 0xa80000002012bbec targets 0xa80000002012ba88 and is always taken.
  The `beq` branch at 0xa80000002012bbf4 targets 0xa80000002012ba8c and is always taken.
  The `beq` branch at 0xa80000002012bc18 targets 0xa80000002012bc30, is taken when v0(?) == a0(?), and falls through when v0(?) != a0(?).
  The `bne` branch at 0xa80000002012bc28 targets 0xa80000002012b988, is taken when (s4(=0xffffffffc0c0c0c0) & a3(?)) != 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a3(?)) == 0.
  The `beq` branch at 0xa80000002012bc38 targets 0xa80000002012ba54 and is always taken.
  The `beq` branch at 0xa80000002012bc48 targets 0xa80000002012b8cc and is always taken.
  The `beq` branch at 0xa80000002012bc50 targets 0xa80000002012bbbc and is always taken.
  The `beq` branch at 0xa80000002012bc5c targets 0xa80000002012ba54 and is always taken.
  The `blez` branch at 0xa80000002012bc64 targets 0xa80000002012bc84, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xa80000002012bc7c targets 0xa80000002012ba50 and is always taken.
  The `beq` branch at 0xa80000002012bca4 targets 0xa80000002012bce8, is taken when (s4(=0xffffffffc0c0c0c0) & a6(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a6(?)) != 0.
  The `beq` branch at 0xa80000002012bcbc targets 0xa80000002012bce0, is taken when (t2(=[a0+140]) & 0x40) == 0, and falls through when (t2(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xa80000002012bcc8 targets 0xa80000002012bce0, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xa80000002012bce0 targets 0xa80000002012ba88 and is always taken.
  The `beq` branch at 0xa80000002012bcec targets 0xa80000002012bcac and is always taken.
  The return at 0xa80000002012bab4 has v0 = one of ?, -0x1.
- `ioc3_set_DTR` in `6.5.22/unix.IP30` at 0xa80000002012bfb0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012bfb0-0xa80000002012bfd0 (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xa80000002012bfbc calls set_DTRRTS [0xa80000002012beec] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x1, a3=0x1000000.
  The return at 0xa80000002012bfc8 has v0 = ret(set_DTRRTS).
- `ioc3_set_RTS` in `6.5.22/unix.IP30` at 0xa80000002012bfd0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012bfd0-0xa80000002012bff0 (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xa80000002012bfdc calls set_DTRRTS [0xa80000002012beec] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x2, a3=0x2000000.
  The return at 0xa80000002012bfe8 has v0 = ret(set_DTRRTS).
- `ioc3_query_DCD` in `6.5.22/unix.IP30` at 0xa80000002012bff0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012bff0-0xa80000002012c004 (20 bytes).
  It occupies 20 bytes.
  The `ld` at 0xa80000002012bff0 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xa80000002012bff4 loads 4 bytes from [a0+64]:8 +0x18.
  The return at 0xa80000002012bffc has v0 = ?.
- `ioc3_query_CTS` in `6.5.22/unix.IP30` at 0xa80000002012c004 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012c004-0xa80000002012c018 (20 bytes).
  It occupies 20 bytes.
  The `ld` at 0xa80000002012c004 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xa80000002012c008 loads 4 bytes from [a0+64]:8 +0x18.
  The return at 0xa80000002012c010 has v0 = ?.
- `ioc3_get_mapid` in `6.5.22/unix.IP30` at 0xa80000002012c06c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012c06c-0xa80000002012c0cc (96 bytes).
  It occupies 96 bytes.
  The `ld` at 0xa80000002012c080 loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `jal` at 0xa80000002012c0a4 calls copyout [0xa80000002000ad0c] with a0=sp-16, a1=a1 at entry (arg1), a2=0x8, a3=0xa8000000203c5270.
  The `beq` branch at 0xa80000002012c074 targets 0xa80000002012c0bc, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xa80000002012c0b4 targets 0xa80000002012c0c0 and is always taken.
  The return at 0xa80000002012c0c4 has v0 = one of 0x0, 0xe.
- `ioc3_map` in `6.5.22/unix.IP30` at 0xa80000002012c0cc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012c0cc-0xa80000002012c15c (144 bytes).
  It occupies 144 bytes.
  The `ld` at 0xa80000002012c0f8 loads 8 bytes from a0 (arg0 at entry) +0x58.
  The `ld` at 0xa80000002012c12c loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `ld` at 0xa80000002012c13c loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `jal` at 0xa80000002012c100 calls disable_intrs [0xa80000002012a520] with a0=a0 at entry (arg0), a1=-0x1, a2=?, a3=?, a5=0x16.
  The `jal` at 0xa80000002012c110 calls v_mapphys [0xa800000020186ee8] with a0=a1 at entry (arg1), a1=?, a2=0x4000, a3=?.
  The `beq` branch at 0xa80000002012c0d4 targets 0xa80000002012c0f0, is taken when a2 == 0, and falls through when a2 != 0.
  The `beq` branch at 0xa80000002012c0e0 targets 0xa80000002012c12c, is taken when a2 == at(=0x4000), and falls through when a2 != at(=0x4000).
  The `beq` branch at 0xa80000002012c0e8 targets 0xa80000002012c124 and is always taken.
  The `beq` branch at 0xa80000002012c154 targets 0xa80000002012c100 and is always taken.
  The return at 0xa80000002012c124 has v0 = one of 0x16, ret(v_mapphys).
- `ioc3_unmap` in `6.5.22/unix.IP30` at 0xa80000002012c15c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012c15c-0xa80000002012c1a4 (72 bytes).
  It occupies 72 bytes.
  The `lw` at 0xa80000002012c15c loads 4 bytes from a0 (arg0 at entry) +0x94.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa80000002012c164, `ld` at 0xa80000002012c178, `ld` at 0xa80000002012c18c.
  The `sw` at 0xa80000002012c170 stores 4 bytes to a0 (arg0 at entry) +0x94.
  The `sw` at 0xa80000002012c174 stores 4 bytes to [a0+64]:8 +0x0.
  The `lw` at 0xa80000002012c17c loads 4 bytes from [a0+64]:8 +0x8.
  The `sw` at 0xa80000002012c184 stores 4 bytes to a0 (arg0 at entry) +0x98.
  The `sw` at 0xa80000002012c188 stores 4 bytes to [a0+64]:8 +0x4.
  The `lw` at 0xa80000002012c190 loads 4 bytes from [a0+64]:8 +0xc.
  The `sw` at 0xa80000002012c198 stores 4 bytes to a0 (arg0 at entry) +0x9c.
  The `sw` at 0xa80000002012c1a0 stores 4 bytes to [a0+64]:8 +0x10.
  The return at 0xa80000002012c19c has v0 = [a0+64]:8.
- `ioc3_set_sscr` in `6.5.22/unix.IP30` at 0xa80000002012c1a4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012c1a4-0xa80000002012c1d4 (48 bytes).
  It occupies 48 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xa80000002012c1a4, `lw` at 0xa80000002012c1c4.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xa80000002012c1b4, `sw` at 0xa80000002012c1d0.
  The `ld` at 0xa80000002012c1b8 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `sw` at 0xa80000002012c1bc stores 4 bytes to [a0+64]:8 +0x0.
  The `beq` branch at 0xa80000002012c1a8 targets 0xa80000002012c1c8, is taken when a2 == 0, and falls through when a2 != 0.
  The `beq` branch at 0xa80000002012c1cc targets 0xa80000002012c1b8 and is always taken.
  The return at 0xa80000002012c1c0 has v0 = [a0+148].
- `ioc3_console_write` in `6.5.22/unix.IP30` at 0xa80000002012c1d4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002012c1d4-0xa80000002012c328 (340 bytes).
  It occupies 340 bytes.
  7 such `lw` accesses load 4 bytes from [0xa8000000203db470]:8 +0xb8, including `lw` at 0xa80000002012c1d8, `lw` at 0xa80000002012c2b4, `lw` at 0xa80000002012c2c4.
  The `sd` at 0xa80000002012c200 stores 8 bytes to a2 (arg2 at entry) +0x0.
  The `sb` at 0xa80000002012c228 stores 1 byte to [0xa8000000203db470]:8 +0x20178.
  6 such `lbu` accesses load 1 byte from [0xa8000000203db470]:8 +0x2017d, including `lbu` at 0xa80000002012c22c, `lbu` at 0xa80000002012c244, `lbu` at 0xa80000002012c254.
  The `lbu` at 0xa80000002012c238 loads 1 byte from ?.
  The `sw` at 0xa80000002012c2b0 stores 4 bytes to [0xa8000000203db470]:8 +0xb8.
  The `bne` branch at 0xa80000002012c1e8 targets 0xa80000002012c200, is taken when t0(=0x10000000) != t2(?), and falls through when t0(=0x10000000) == t2(?).
  The `beq` branch at 0xa80000002012c1f8 targets 0xa80000002012c31c and is always taken.
  The `bne` branch at 0xa80000002012c214 targets 0xa80000002012c22c, is taken when a1 != 0, and falls through when a1 == 0.
  The `beq` branch at 0xa80000002012c21c targets 0xa80000002012c29c and is always taken.
  The `beq` branch at 0xa80000002012c224 targets 0xa80000002012c29c, is taken when a1(?) == 0, and falls through when a1(?) != 0.
  The `bne` branch at 0xa80000002012c23c targets 0xa80000002012c224, is taken when (a6(=[[0xa8000000203db470]:8+131453]:1) & 0x20) != 0, and falls through when (a6(=[[0xa8000000203db470]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xa80000002012c24c targets 0xa80000002012c224, is taken when (a7(=[[0xa8000000203db470]:8+131453]:1) & 0x20) != 0, and falls through when (a7(=[[0xa8000000203db470]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xa80000002012c25c targets 0xa80000002012c224, is taken when (t0(=[[0xa8000000203db470]:8+131453]:1) & 0x20) != 0, and falls through when (t0(=[[0xa8000000203db470]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xa80000002012c26c targets 0xa80000002012c224, is taken when (t1(=[[0xa8000000203db470]:8+131453]:1) & 0x20) != 0, and falls through when (t1(=[[0xa8000000203db470]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xa80000002012c27c targets 0xa80000002012c224, is taken when (t2(=[[0xa8000000203db470]:8+131453]:1) & 0x20) != 0, and falls through when (t2(=[[0xa8000000203db470]:8+131453]:1) & 0x20) == 0.
  The `beq` branch at 0xa80000002012c28c targets 0xa80000002012c244, is taken when (t3(=[[0xa8000000203db470]:8+131453]:1) & 0x20) == 0, and falls through when (t3(=[[0xa8000000203db470]:8+131453]:1) & 0x20) != 0.
  The `beq` branch at 0xa80000002012c294 targets 0xa80000002012c224 and is always taken.
  The `bne` branch at 0xa80000002012c2a8 targets 0xa80000002012c200, is taken when t0(=0x10000000) != t8(?), and falls through when t0(=0x10000000) == t8(?).
  The `bne` branch at 0xa80000002012c2bc targets 0xa80000002012c2a8, is taken when a5(=0x30000000) != t9(?), and falls through when a5(=0x30000000) == t9(?).
  The `bne` branch at 0xa80000002012c2cc targets 0xa80000002012c2a4, is taken when a5(=0x30000000) != v0(?), and falls through when a5(=0x30000000) == v0(?).
  The `bne` branch at 0xa80000002012c2dc targets 0xa80000002012c2a4, is taken when a5(=0x30000000) != v1(?), and falls through when a5(=0x30000000) == v1(?).
  The `bne` branch at 0xa80000002012c2ec targets 0xa80000002012c2a4, is taken when a5(=0x30000000) != a3(?), and falls through when a5(=0x30000000) == a3(?).
  The `bne` branch at 0xa80000002012c2fc targets 0xa80000002012c2a4, is taken when a5(=0x30000000) != t3(?), and falls through when a5(=0x30000000) == t3(?).
  The `beq` branch at 0xa80000002012c30c targets 0xa80000002012c2c4, is taken when a5(=0x30000000) == t8(?), and falls through when a5(=0x30000000) != t8(?).
  The `beq` branch at 0xa80000002012c314 targets 0xa80000002012c2a4 and is always taken.
  The `beq` branch at 0xa80000002012c320 targets 0xa80000002012c2b0 and is always taken.
  The return at 0xa80000002012c29c has v0 = 0x0.
- `idbg_gdb` in `6.5.22/unix.IP30` at 0xa800000020148440 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020148440-0xa800000020148474 (52 bytes).
  It occupies 52 bytes.
  The `jal` at 0xa800000020148450 calls dogang [0xa800000020148070] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa800000020148464 calls idbg_gang [0xa800000020148304] with a0=-0x1, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `beq` branch at 0xa800000020148448 targets 0xa800000020148464, is taken when a0 == at(=-0x1), and falls through when a0 != at(=-0x1).
  The `beq` branch at 0xa80000002014846c targets 0xa80000002014845c and is always taken.
  The return at 0xa80000002014845c has v0 = one of ret(dogang), ret(idbg_gang).
- `panicregs` in `6.5.22/unix.IP30` at 0xa800000020151cd8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020151cd8-0xa800000020151d8c (180 bytes).
  It occupies 180 bytes.
  The `ld` at 0xa800000020151d28 loads 8 bytes from a0 (arg0 at entry) +0x120.
  The `ld` at 0xa800000020151d54 loads 8 bytes from a0 (arg0 at entry) +0x110.
  The `ld` at 0xa800000020151d58 loads 8 bytes from table(0xa8000000203c55c8) +0x0.
  The `jal` at 0xa800000020151cf4 calls spl1 [0xa80000002000f47c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa800000020151cfc calls set_leds [0xa800000020006590] with a0=0x3, a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020151d1c calls machine_error_dump [0xa80000002016acbc] with a0=0xa80000002035e8a0, a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020151d74 calls cmn_err [0xa8000000201a63b4] with a0=0x0, a1=0xa80000002035e8b0, a2=a4 at entry (arg4), a3=[a0+288]:8, a4=a0 at entry (arg0), a5=a1 at entry (arg1), a6=entry(0xa8000000203c55c8), a7=[a0+272]:8.
  The `bne` branch at 0xa800000020151d14 targets 0xa800000020151d24, is taken when v0(=[0xa8000000203dba38]) != 0, and falls through when v0(=[0xa8000000203dba38]) == 0.
  The return at 0xa800000020151d84 has v0 = ret(cmn_err).
- `mem_range_init` in `6.5.22/unix.IP30` at 0xa8000000201589f8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa8000000201589f8-0xa800000020158c88 (656 bytes).
  It occupies 656 bytes.
  The `lw` at 0xa800000020158a5c loads 4 bytes from [0xa8000000203e2910]:8 +0x5b4.
  The `sw` at 0xa800000020158a6c stores 4 bytes to [0xa8000000203e2910]:8 +0x5b4.
  The `lw` at 0xa800000020158a74 loads 4 bytes from [0xa8000000203e2910]:8 +0x5c4.
  The `sw` at 0xa800000020158a80 stores 4 bytes to [0xa8000000203e2910]:8 +0x5c4.
  The `lw` at 0xa800000020158a88 loads 4 bytes from [0xa8000000203e2910]:8 +0x640.
  The `sw` at 0xa800000020158a94 stores 4 bytes to [0xa8000000203e2910]:8 +0x640.
  The `ll` at 0xa800000020158aa4 loads 4 bytes from [0xa8000000203e2910]:8 +0x5bc.
  The `sc` at 0xa800000020158ab0 stores 4 bytes to [0xa8000000203e2910]:8 +0x5bc.
  3 such `lw` accesses load 4 bytes from ?, including `lw` at 0xa800000020158ae0, `lw` at 0xa800000020158b3c, `lw` at 0xa800000020158bb0.
  The `ld` at 0xa800000020158ae8 loads 8 bytes from [0xa8000000203e2910]:8 +0x5a8.
  4 such `sw` accesses store 4 bytes to ?, including `sw` at 0xa800000020158af4, `sw` at 0xa800000020158afc, `sw` at 0xa800000020158bac.
  The `ld` at 0xa800000020158b00 loads 8 bytes from ([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  The `sd` at 0xa800000020158b14 stores 8 bytes to ([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  3 such `ld` accesses load 8 bytes from ?, including `ld` at 0xa800000020158b48, `ld` at 0xa800000020158b98, `ld` at 0xa800000020158c1c.
  9 such `sd` accesses store 8 bytes to ?, including `sd` at 0xa800000020158b64, `sd` at 0xa800000020158b9c, `sd` at 0xa800000020158ba0.
  The `lw` at 0xa800000020158b70 loads 4 bytes from [0xa8000000203e2910]:8 +0x5dc.
  3 such `ld` accesses load 8 bytes from ([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0, including `ld` at 0xa800000020158b7c, `ld` at 0xa800000020158bfc, `ld` at 0xa800000020158c4c.
  2 such `lhu` accesses load 2 bytes from ?, including `lhu` at 0xa800000020158b94, `lhu` at 0xa800000020158c08.
  2 such `ld` accesses load 8 bytes from ([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x48, including `ld` at 0xa800000020158be4, `ld` at 0xa800000020158c34.
  The `sd` at 0xa800000020158c64 stores 8 bytes to ([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x80.
  The `jal` at 0xa800000020158a20 calls is_specified [0xa8000000201b69a0] with a0=0xa8000000203e2973, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa800000020158a4c calls setupbadmem [0xa800000020011ee8] with a0=?, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=?.
  The `jal` at 0xa800000020158bc8 calls bset [0xa80000002020839c] with a0=[0xa8000000203e2210]:8, a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020158bdc calls bset [0xa80000002020839c] with a0=[0xa8000000203e2208]:8, a1=?, a2=?, a3=?.
  The `beql` branch at 0xa800000020158ab4 targets 0xa800000020158aa4, is taken when a1(?) == 0, and falls through when a1(?) != 0; its branch-likely delay slot `nop` at 0xa800000020158ab8 runs only when taken and is annulled on fall-through.
  The `bne` branch at 0xa800000020158b0c targets 0xa800000020158b68, is taken when t8(=[([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) != 0, and falls through when t8(=[([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) == 0.
  The `bne` branch at 0xa800000020158b60 targets 0xa800000020158b18, is taken when a1(?) < 0x6 (signed), and falls through when a1(?) >= 0x6 (signed).
  The `bltz` branch at 0xa800000020158b88 targets 0xa800000020158c54, is taken when s3(?) < 0 (signed), and falls through when s3(?) >= 0 (signed).
  The `beq` branch at 0xa800000020158b90 targets 0xa800000020158c0c and is always taken.
  The `bne` branch at 0xa800000020158bf4 targets 0xa800000020158c00, is taken when a2(=[([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s1(?), and falls through when a2(=[([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s1(?).
  The `bltz` branch at 0xa800000020158c00 targets 0xa800000020158c58, is taken when a1(?) < 0 (signed), and falls through when a1(?) >= 0 (signed).
  The `beq` branch at 0xa800000020158c0c targets 0xa800000020158c34, is taken when s4(=0xffff) == a3(?), and falls through when s4(=0xffff) != a3(?).
  The `bne` branch at 0xa800000020158c14 targets 0xa800000020158b98, is taken when s5(=ret(is_specified)) != 0, and falls through when s5(=ret(is_specified)) == 0.
  The `beq` branch at 0xa800000020158c2c targets 0xa800000020158bac and is always taken.
  The `bne` branch at 0xa800000020158c44 targets 0xa800000020158c50, is taken when a5(=[([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s1(?), and falls through when a5(=[([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s1(?).
  The `bgez` branch at 0xa800000020158c50 targets 0xa800000020158c08, is taken when v0(?) >= 0 (signed), and falls through when v0(?) < 0 (signed).
  The return at 0xa800000020158c80 has v0 = one of ret(bset), ?.
- `idbg_init` in `6.5.22/unix.IP30` at 0xa8000000201780e0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa8000000201780e0-0xa800000020178154 (116 bytes).
  It occupies 116 bytes.
  2 such `sw` accesses store 4 bytes to ?, including `sw` at 0xa800000020178108, `sw` at 0xa800000020178128.
  The `jal` at 0xa80000002017813c calls init_mutex [0xa80000002013b20c] with a0=0xa80000002041c728, a1=0x0, a2=0xa800000020361278, a3=-0x1, a5=0xa80000002041bbf8.
  The `beq` branch at 0xa80000002017810c targets 0xa800000020178130, is taken when a2(?) >= 0x14 (signed), and falls through when a2(?) < 0x14 (signed).
  The `bne` branch at 0xa800000020178124 targets 0xa8000000201780f8, is taken when a2(?) < 0x14 (signed), and falls through when a2(?) >= 0x14 (signed).
  The return at 0xa80000002017814c has v0 = 0x1.
- `idbg_id` in `6.5.22/unix.IP30` at 0xa800000020178154 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020178154-0xa80000002017815c (8 bytes).
  It occupies 8 bytes.
  The `lw` at 0xa800000020178158 loads 4 bytes from a0 (arg0 at entry) +0x3c.
  The return at 0xa800000020178154 has v0 = [a0+60].
- `idbg_lock` in `6.5.22/unix.IP30` at 0xa80000002017815c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002017815c-0xa800000020178164 (8 bytes).
  It occupies 8 bytes.
  The return at 0xa80000002017815c has v0 = 0x0.
- `idbg_unlock` in `6.5.22/unix.IP30` at 0xa800000020178164 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020178164-0xa80000002017816c (8 bytes).
  It occupies 8 bytes.
  The return at 0xa800000020178164 has v0 = ?.
- `idbg_copydata` in `6.5.22/unix.IP30` at 0xa80000002017816c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002017816c-0xa800000020178174 (8 bytes).
  It occupies 8 bytes.
  The return at 0xa80000002017816c has v0 = ?.
- `idbg_makedata` in `6.5.22/unix.IP30` at 0xa800000020178174 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020178174-0xa8000000201781ec (120 bytes).
  It occupies 120 bytes.
  The `ld` at 0xa80000002017819c loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `sd` at 0xa8000000201781dc stores 8 bytes to ret(kern_calloc) +0x8.
  The `sd` at 0xa8000000201781e0 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xa80000002017818c calls kern_calloc [0xa80000002013089c] with a0=0x1, a1=0x18, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa8000000201781a0 calls copyin [0xa80000002000ac80] with a0=[a0+24]:8, a1=ret(kern_calloc), a2=0x4, a3=?.
  The `jal` at 0xa8000000201781b0 calls kern_free [0xa8000000201309b4] with a0=ret(kern_calloc), a1=0xa0, a2=?, a3=?.
  The `jal` at 0xa8000000201781d4 calls kern_calloc [0xa80000002013089c] with a0=0x1, a1=0xa0, a2=?, a3=?.
  The `beq` branch at 0xa8000000201781a8 targets 0xa8000000201781d4, is taken when v0(=ret(copyin)) == 0, and falls through when v0(=ret(copyin)) != 0.
  The `beq` branch at 0xa8000000201781e4 targets 0xa8000000201781bc and is always taken.
  The return at 0xa8000000201781cc has v0 = one of 0x0, 0xa.
- `idbg_freedata` in `6.5.22/unix.IP30` at 0xa8000000201781ec (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa8000000201781ec-0xa800000020178234 (72 bytes).
  It occupies 72 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x18, including `ld` at 0xa8000000201781ec, `ld` at 0xa800000020178214.
  The `ld` at 0xa800000020178200 loads 8 bytes from [a0+24]:8 +0x8.
  The `sd` at 0xa800000020178224 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xa80000002017820c calls kern_free [0xa8000000201309b4] with a0=[[a0+24]:8+8]:8, a1=a1 at entry (arg1), a2=[a0+24]:8, a3=a3 at entry (arg3).
  The `jal` at 0xa800000020178218 calls kern_free [0xa8000000201309b4] with a0=[a0+24]:8, a1=?, a2=[a0+24]:8, a3=?.
  The `beq` branch at 0xa8000000201781f8 targets 0xa800000020178228, is taken when a2(=[a0+24]:8) == 0, and falls through when a2(=[a0+24]:8) != 0.
  The `beq` branch at 0xa800000020178204 targets 0xa800000020178218, is taken when a0(=[[a0+24]:8+8]:8) == 0, and falls through when a0(=[[a0+24]:8+8]:8) != 0.
  The return at 0xa80000002017822c has v0 = one of ?, ret(kern_free).
- `idbg_load` in `6.5.22/unix.IP30` at 0xa80000002017866c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002017866c-0xa800000020178710 (164 bytes).
  It occupies 164 bytes.
  The `ld` at 0xa8000000201786b0 loads 8 bytes from a0 (arg0 at entry) +0x28.
  The `ld` at 0xa8000000201786b8 loads 8 bytes from [a0+40]:8 +0x50.
  The `jal` at 0xa800000020178688 calls mutex_lock [0xa80000002013b39c] with a0=0xa80000002041c728, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa800000020178690 calls debug_check [0xa80000002017854c] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xa8000000201786a8 calls debug_freeid [0xa80000002017850c] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xa8000000201786d4 calls mutex_unlock [0xa80000002013b2d4] with a0=0xa80000002041c728, a1=?, a2=?, a3=?.
  The `jal` at 0xa8000000201786f0 calls mloadfile [0xa800000020179614] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020178700 calls debug_init [0xa800000020178234] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xa800000020178698 targets 0xa8000000201786f0, is taken when v0(=ret(debug_check)) == 0, and falls through when v0(=ret(debug_check)) != 0.
  The `beq` branch at 0xa8000000201786a0 targets 0xa8000000201786d0, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `bne` branch at 0xa8000000201786f8 targets 0xa8000000201786a0, is taken when v0(=ret(mloadfile)) != 0, and falls through when v0(=ret(mloadfile)) == 0.
  The `beq` branch at 0xa800000020178708 targets 0xa8000000201786a0 and is always taken.
  The return at 0xa8000000201786e8 has v0 = ?.
- `idbg_unload` in `6.5.22/unix.IP30` at 0xa800000020178710 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020178710-0xa80000002017887c (364 bytes).
  It occupies 364 bytes.
  The `ld` at 0xa800000020178728 loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `ld` at 0xa800000020178740 loads 8 bytes from a0 (arg0 at entry) +0x50.
  The `ld` at 0xa80000002017876c loads 8 bytes from [a0+24]:8 +0x8.
  The `ld` at 0xa800000020178774 loads 8 bytes from [[a0+24]:8+8]:8 +0x68.
  The `sw` at 0xa8000000201787e0 stores 4 bytes to abs 0xa80000002041c6d8.
  The `ld` at 0xa8000000201787e8 loads 8 bytes from [a0+24]:8 +0x10.
  The `ld` at 0xa800000020178850 loads 8 bytes from [0xa8000000203e2280]:8 +0x28.
  The `lw` at 0xa800000020178854 loads 4 bytes from [[0xa8000000203e2280]:8+40]:8 +0x48.
  The `sw` at 0xa800000020178860 stores 4 bytes to [[0xa8000000203e2280]:8+40]:8 +0x48.
  The `ld` at 0xa800000020178864 loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `sw` at 0xa800000020178878 stores 4 bytes to table(0xa80000002041c6d8) +0x0.
  The `jal` at 0xa800000020178730 calls mutex_lock [0xa80000002013b39c] with a0=0xa80000002041c728, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jalr` at 0xa8000000201787c0 calls (unknown callee) with a0=?, a1=?, a2=?, a3=-0x1, a4=0xc000000000000000, a5=0xc0000fbf00000000, a6=0xe000.
  The `jal` at 0xa8000000201787d0 calls mutex_unlock [0xa80000002013b2d4] with a0=0xa80000002041c728, a1=?, a2=?, a3=?.
  The `jal` at 0xa8000000201787ec calls symtab_rele [0xa8000000201790e4] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xa8000000201787f8 calls mutex_unlock [0xa80000002013b2d4] with a0=0xa80000002041c728, a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020178824 calls mfindname [0xa800000020179be4] with a0=[a0+80]:8, a1=0xa800000020360f88, a2=sp-32, a3=-0x1, a4=0xc000000000000000, a5=0xc0000fbf00000000, a6=0xe000, a7=0xffffffff00000000.
  The `jal` at 0xa800000020178834 calls mutex_unlock [0xa80000002013b2d4] with a0=0xa80000002041c728, a1=?, a2=?, a3=?.
  The `jalr` at 0xa800000020178844 calls (unknown callee) with a0=?, a1=?, a2=?, a3=?.
  The `bne` branch at 0xa800000020178764 targets 0xa800000020178818, is taken when v0(=[0xa8000000203e2280]:8) != a0(=[a0+80]:8), and falls through when v0(=[0xa8000000203e2280]:8) == a0(=[a0+80]:8).
  The `bne` branch at 0xa800000020178798 targets 0xa8000000201787a8, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) < t3(=0xa800000000000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) >= t3(=0xa800000000000000) (unsigned).
  The `bne` branch at 0xa8000000201787a0 targets 0xa8000000201787c0, is taken when a7(?) != 0, and falls through when a7(?) == 0.
  The `bne` branch at 0xa8000000201787a8 targets 0xa8000000201787e0, is taken when a0(?) != 0, and falls through when a0(?) == 0.
  The `beq` branch at 0xa8000000201787b8 targets 0xa8000000201787e0, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) >= t9(=0xc0000fbfe0000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) < t9(=0xc0000fbfe0000000) (unsigned).
  The `beq` branch at 0xa8000000201787c8 targets 0xa8000000201787e0, is taken when v0(=ret(?)) == 0, and falls through when v0(=ret(?)) != 0.
  The `beq` branch at 0xa8000000201787d8 targets 0xa800000020178804 and is always taken.
  The `beq` branch at 0xa80000002017882c targets 0xa800000020178844, is taken when v0(=ret(mfindname)) == 0, and falls through when v0(=ret(mfindname)) != 0.
  The `beq` branch at 0xa80000002017883c targets 0xa800000020178804 and is always taken.
  The `beq` branch at 0xa800000020178874 targets 0xa8000000201787f4 and is always taken.
  The return at 0xa800000020178810 has v0 = one of 0x0, 0x33.
- `idbg_reg` in `6.5.22/unix.IP30` at 0xa80000002017887c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa80000002017887c-0xa800000020178884 (8 bytes).
  It occupies 8 bytes.
  The return at 0xa80000002017887c has v0 = 0x59.
- `idbg_unreg` in `6.5.22/unix.IP30` at 0xa800000020178884 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020178884-0xa80000002017888c (8 bytes).
  It occupies 8 bytes.
  The return at 0xa800000020178884 has v0 = 0x59.
- `setup_lowmem` in `6.5.22/unix.IP30` at 0xa800000020193c78 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP30`.
  Range: 0xa800000020193c78-0xa800000020193fc8 (848 bytes).
  It occupies 848 bytes.
  The `sd` at 0xa800000020193d7c stores 8 bytes to ret(low_mem_alloc) +0x5a8.
  The `ld` at 0xa800000020193dd4 loads 8 bytes from [0xa8000000203e2910]:8 +0x5a8.
  The `sw` at 0xa800000020193e7c stores 4 bytes to ret(low_mem_alloc) +0x5dc.
  The `sd` at 0xa800000020193ea0 stores 8 bytes to ([[0xa8000000203e2910]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0.
  The `sw` at 0xa800000020193ecc stores 4 bytes to ?.
  The `sd` at 0xa800000020193ef0 stores 8 bytes to ?.
  The `jal` at 0xa800000020193d08 calls blkclr [0xa80000002000affc] with a0=?, a1=?, a2=0xa800000000000000, a3=a3 at entry (arg3), a5=[one of 0xa8000000203e278c, 0xa8000000203e2794], a6=[0xa8000000203e2afc].
  The `jal` at 0xa800000020193d10 calls flush_cache [0xa80000002000f040] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020193d20 calls mktables [0xa800000020193fc8] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xa800000020193d3c calls low_mem_alloc [0xa80000002019399c] with a0=0xcb8, a1=sp-112, a2=0xa800000020362408, a3=?.
  The `jal` at 0xa800000020193d64 calls low_mem_alloc [0xa80000002019399c] with a0=(((([0xa8000000203e28f0] << 0x4) + [0xa8000000203e28f0]) << 0x3) << 0x0), a1=sp-112, a2=0xa800000020362418, a3=?.
  The `jal` at 0xa800000020193d80 calls low_mem_alloc [0xa80000002019399c] with a0=?, a1=sp-112, a2=0xa800000020362430, a3=?.
  The `jal` at 0xa800000020193d98 calls low_mem_alloc [0xa80000002019399c] with a0=?, a1=sp-112, a2=0xa800000020362448, a3=?.
  The `jal` at 0xa800000020193e8c calls low_mem_alloc [0xa80000002019399c] with a0=?, a1=sp-112, a2=0xa800000020362460, a3=?, a4=[0xa8000000203e27b4].
  The `jal` at 0xa800000020193ed0 calls low_mem_alloc [0xa80000002019399c] with a0=?, a1=sp-112, a2=0xa800000020362460, a3=?.
  The `jal` at 0xa800000020193fb0 calls cmn_err_tag [0xa8000000201a6400] with a0=0x6ec, a1=0x4, a2=0xa800000020362480, a3=?.
  The `beq` branch at 0xa800000020193cb0 targets 0xa800000020193f88, is taken when at(=0x60000) >= a6(=[0xa8000000203e2afc]) (unsigned), and falls through when at(=0x60000) < a6(=[0xa8000000203e2afc]) (unsigned).
  The `beq` branch at 0xa800000020193ccc targets 0xa800000020193ce4, is taken when a0(?) >= v0(?) (unsigned), and falls through when a0(?) < v0(?) (unsigned).
  The `beq` branch at 0xa800000020193cdc targets 0xa800000020193ce8 and is always taken.
  The `bne` branch at 0xa800000020193df0 targets 0xa800000020193dfc, is taken when a1(?) < a2(?) (signed), and falls through when a1(?) >= a2(?) (signed).
  The `bne` branch at 0xa800000020193e14 targets 0xa800000020193e20, is taken when a2(?) < s1(?) (signed), and falls through when a2(?) >= s1(?) (signed).
  The `bne` branch at 0xa800000020193e38 targets 0xa800000020193e44, is taken when a2(?) < s1(?) (signed), and falls through when a2(?) >= s1(?) (signed).
  The `bne` branch at 0xa800000020193e64 targets 0xa800000020193e70, is taken when a1(?) < s1(?) (signed), and falls through when a1(?) >= s1(?) (signed).
  The `beq` branch at 0xa800000020193eec targets 0xa800000020193ea8, is taken when s3(=0x1000000) >= s2(?) (signed), and falls through when s3(=0x1000000) < s2(?) (signed).
  The `bne` branch at 0xa800000020193f4c targets 0xa800000020193fa0, is taken when t0(?) < a6(?) (unsigned), and falls through when t0(?) >= a6(?) (unsigned).
  The `bne` branch at 0xa800000020193f90 targets 0xa800000020193ce4, is taken when v0(?) < 0x801 (unsigned), and falls through when v0(?) >= 0x801 (unsigned).
  The `beq` branch at 0xa800000020193f98 targets 0xa800000020193cd4 and is always taken.
  The `beq` branch at 0xa800000020193fc0 targets 0xa800000020193f54 and is always taken.
  The return at 0xa800000020193f80 has v0 = ?.
- `ioc3_early_pckminit` in `6.5.22/unix.IP35` at 0xc0000000000cdbbc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000000cdbbc-0xc0000000000cdbc4 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000000cdbbc has v0 = ?.
- `ioc3_pckm_kbdexists` in `6.5.22/unix.IP35` at 0xc0000000000cf3dc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000000cf3dc-0xc0000000000cf418 (60 bytes).
  It occupies 60 bytes.
  The `lw` at 0xc0000000000cf3e8 loads 4 bytes from [0xc00000000159ad60]:8 +0x28.
  The `ld` at 0xc0000000000cf3f8 loads 8 bytes from ([0xc00000000159ad60]:8 + ([[0xc00000000159ad60]:8+40] << 0x3)) +0x18.
  The `lw` at 0xc0000000000cf3fc loads 4 bytes from [([0xc00000000159ad60]:8 + ([[0xc00000000159ad60]:8+40] << 0x3))+24]:8 +0x4.
  The `beq` branch at 0xc0000000000cf3e0 targets 0xc0000000000cf410, is taken when a2(=[0xc00000000159ad60]:8) == 0, and falls through when a2(=[0xc00000000159ad60]:8) != 0.
  The `beq` branch at 0xc0000000000cf3ec targets 0xc0000000000cf410, is taken when a3(=[[0xc00000000159ad60]:8+40]) == v0(=0x13), and falls through when a3(=[[0xc00000000159ad60]:8+40]) != v0(=0x13).
  The return at 0xc0000000000cf408 has v0 = ?.
  The return at 0xc0000000000cf410 has v0 = 0x0.
- `ioc3_pckm_setleds` in `6.5.22/unix.IP35` at 0xc0000000000cf418 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000000cf418-0xc0000000000cf458 (64 bytes).
  It occupies 64 bytes.
  The `lw` at 0xc0000000000cf430 loads 4 bytes from [0xc00000000159ad60]:8 +0x28.
  The `ld` at 0xc0000000000cf448 loads 8 bytes from ([0xc00000000159ad60]:8 + ([[0xc00000000159ad60]:8+40] << 0x3)) +0x18.
  The `jal` at 0xc0000000000cf444 calls ioc3_lcl_pckm_setleds [0xc0000000000cf4d0] with a0=[([0xc00000000159ad60]:8 + ([[0xc00000000159ad60]:8+40] << 0x3))+24]:8, a1=a0 at entry (arg0), a2=a2 at entry (arg2), a3=[0xc00000000159ad60]:8, a4=0x13, a5=[[0xc00000000159ad60]:8+40].
  The `beq` branch at 0xc0000000000cf428 targets 0xc0000000000cf44c, is taken when a3(=[0xc00000000159ad60]:8) == 0, and falls through when a3(=[0xc00000000159ad60]:8) != 0.
  The `beq` branch at 0xc0000000000cf434 targets 0xc0000000000cf450, is taken when a5(=[[0xc00000000159ad60]:8+40]) == a4(=0x13), and falls through when a5(=[[0xc00000000159ad60]:8+40]) != a4(=0x13).
  The return at 0xc0000000000cf450 has v0 = ?.
- `ioc3_pckm_bell` in `6.5.22/unix.IP35` at 0xc0000000000cf458 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000000cf458-0xc0000000000cf49c (68 bytes).
  It occupies 68 bytes.
  The `lw` at 0xc0000000000cf46c loads 4 bytes from [0xc00000000159ad60]:8 +0x28.
  The `lw` at 0xc0000000000cf478 loads 4 bytes from [0xc00000000159ad60]:8 +0x34.
  The `ld` at 0xc0000000000cf48c loads 8 bytes from ([0xc00000000159ad60]:8 + ([[0xc00000000159ad60]:8+40] << 0x3)) +0x18.
  The `jal` at 0xc0000000000cf488 calls ioc3_lcl_pckm_setleds [0xc0000000000cf4d0] with a0=[([0xc00000000159ad60]:8 + ([[0xc00000000159ad60]:8+40] << 0x3))+24]:8, a1=([[0xc00000000159ad60]:8+52] | 0x8), a2=a2 at entry (arg2), a3=[0xc00000000159ad60]:8, a4=[[0xc00000000159ad60]:8+40].
  The `beq` branch at 0xc0000000000cf464 targets 0xc0000000000cf490, is taken when a3(=[0xc00000000159ad60]:8) == 0, and falls through when a3(=[0xc00000000159ad60]:8) != 0.
  The `beq` branch at 0xc0000000000cf470 targets 0xc0000000000cf494, is taken when a4(=[[0xc00000000159ad60]:8+40]) == a1(=0x13), and falls through when a4(=[[0xc00000000159ad60]:8+40]) != a1(=0x13).
  The return at 0xc0000000000cf494 has v0 = ?.
- `ioc3_gl_setporthandler` in `6.5.22/unix.IP35` at 0xc0000000000cf49c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000000cf49c-0xc0000000000cf4c8 (44 bytes).
  It occupies 44 bytes.
  The `lw` at 0xc0000000000cf4a8 loads 4 bytes from [0xc00000000159ad60]:8 +0x28.
  The `ld` at 0xc0000000000cf4b8 loads 8 bytes from ([0xc00000000159ad60]:8 + ([[0xc00000000159ad60]:8+40] << 0x3)) +0x18.
  The `sd` at 0xc0000000000cf4bc stores 8 bytes to [([0xc00000000159ad60]:8 + ([[0xc00000000159ad60]:8+40] << 0x3))+24]:8 +0x18.
  The `beq` branch at 0xc0000000000cf4a0 targets 0xc0000000000cf4c0, is taken when a2(=[0xc00000000159ad60]:8) == 0, and falls through when a2(=[0xc00000000159ad60]:8) != 0.
  The `beq` branch at 0xc0000000000cf4ac targets 0xc0000000000cf4c0, is taken when a4(=[[0xc00000000159ad60]:8+40]) == a3(=0x13), and falls through when a4(=[[0xc00000000159ad60]:8+40]) != a3(=0x13).
  The return at 0xc0000000000cf4c0 has v0 = ?.
- `ioc3_du_keyboard_port` in `6.5.22/unix.IP35` at 0xc0000000000cf4c8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000000cf4c8-0xc0000000000cf4d0 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000000cf4c8 has v0 = -0x1.
- `ioc3_lcl_pckm_setleds` in `6.5.22/unix.IP35` at 0xc0000000000cf4d0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000000cf4d0-0xc0000000000cf524 (84 bytes).
  It occupies 84 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x48, including `ld` at 0xc0000000000cf4dc, `ld` at 0xc0000000000cf504.
  The `jal` at 0xc0000000000cf4f0 calls mutex_lock [0xc000000000203144] with a0=[a0+72]:8+72, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000000cf4fc calls ioc3_lcl_pckm_setleds_lock [0xc0000000000cf524] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xc0000000000cf50c calls mutex_unlock [0xc00000000020307c] with a0=[a0+72]:8+72, a1=?, a2=?, a3=?.
  The return at 0xc0000000000cf51c has v0 = ret(mutex_unlock).
- `ioc3_lcl_pckm_setleds_lock` in `6.5.22/unix.IP35` at 0xc0000000000cf524 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000000cf524-0xc0000000000cf698 (372 bytes).
  It occupies 372 bytes.
  The `ld` at 0xc0000000000cf530 loads 8 bytes from a0 (arg0 at entry) +0x48.
  5 such `lw` accesses load 4 bytes from [a0+72]:8 +0x34, including `lw` at 0xc0000000000cf538, `lw` at 0xc0000000000cf5ac, `lw` at 0xc0000000000cf5cc.
  The `lw` at 0xc0000000000cf568 loads 4 bytes from [a0+72]:8 +0x38.
  The `sw` at 0xc0000000000cf594 stores 4 bytes to [a0+72]:8 +0x38.
  2 such `sw` accesses store 4 bytes to [a0+72]:8 +0x34, including `sw` at 0xc0000000000cf5a0, `sw` at 0xc0000000000cf5d4.
  The `jal` at 0xc0000000000cf5a8 calls (unnamed) [0xc0000000000cf300] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xc0000000000cf5b4 calls (unnamed) [0xc0000000000cf760] with a0=ret(0xc0000000000cf300), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc0000000000cf600 calls (unnamed) [0xc0000000000cf300] with a0=a0 at entry (arg0), a1=0xed, a2=?, a3=?.
  The `jal` at 0xc0000000000cf60c calls (unnamed) [0xc0000000000cf760] with a0=ret(0xc0000000000cf300), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc0000000000cf62c calls (unnamed) [0xc0000000000cf300] with a0=a0 at entry (arg0), a1=0xf4, a2=?, a3=?.
  The `jal` at 0xc0000000000cf638 calls (unnamed) [0xc0000000000cf760] with a0=ret(0xc0000000000cf300), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc0000000000cf64c calls (unnamed) [0xc0000000000cebfc] with a0=a0 at entry (arg0), a1=0x0, a2=[[a0+72]:8+52], a3=?.
  The `jal` at 0xc0000000000cf66c calls (unnamed) [0xc0000000000cf300] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xc0000000000cf678 calls (unnamed) [0xc0000000000cf760] with a0=ret(0xc0000000000cf300), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc0000000000cf688 calls (unnamed) [0xc0000000000ceb88] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xc0000000000cf558 targets 0xc0000000000cf5d8, is taken when at(=[[a0+72]:8+52]) == a3(?), and falls through when at(=[[a0+72]:8+52]) != a3(?).
  The `beq` branch at 0xc0000000000cf560 targets 0xc0000000000cf59c, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xc0000000000cf574 targets 0xc0000000000cf584, is taken when a7(?) >= 0xe (signed), and falls through when a7(?) < 0xe (signed).
  The `bne` branch at 0xc0000000000cf57c targets 0xc0000000000cf5dc, is taken when a4(?) != 0, and falls through when a4(?) == 0.
  The `beq` branch at 0xc0000000000cf584 targets 0xc0000000000cf594, is taken when a0(?) == 0, and falls through when a0(?) != 0.
  The `bne` branch at 0xc0000000000cf58c targets 0xc0000000000cf5dc, is taken when a5(?) != 0, and falls through when a5(?) == 0.
  The `beq` branch at 0xc0000000000cf59c targets 0xc0000000000cf5fc and is always taken.
  The `beq` branch at 0xc0000000000cf5bc targets 0xc0000000000cf66c, is taken when v0(=ret(0xc0000000000cf760)) == 0, and falls through when v0(=ret(0xc0000000000cf760)) != 0.
  The `beq` branch at 0xc0000000000cf5c4 targets 0xc0000000000cf5dc, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xc0000000000cf5f8 targets 0xc0000000000cf65c, is taken when t1(?) == 0, and falls through when t1(?) != 0.
  The `bne` branch at 0xc0000000000cf614 targets 0xc0000000000cf5a4, is taken when v0(=ret(0xc0000000000cf760)) != 0, and falls through when v0(=ret(0xc0000000000cf760)) == 0.
  The `bne` branch at 0xc0000000000cf620 targets 0xc0000000000cf5f4, is taken when s3(?) < 0x2 (unsigned), and falls through when s3(?) >= 0x2 (unsigned).
  The `bne` branch at 0xc0000000000cf640 targets 0xc0000000000cf5f4, is taken when v0(=ret(0xc0000000000cf760)) != 0, and falls through when v0(=ret(0xc0000000000cf760)) == 0.
  The `beq` branch at 0xc0000000000cf654 targets 0xc0000000000cf5f8 and is always taken.
  The `bne` branch at 0xc0000000000cf65c targets 0xc0000000000cf5cc, is taken when s2(?) != 0, and falls through when s2(?) == 0.
  The `beq` branch at 0xc0000000000cf664 targets 0xc0000000000cf5dc and is always taken.
  The `bne` branch at 0xc0000000000cf680 targets 0xc0000000000cf5c4, is taken when v0(=ret(0xc0000000000cf760)) != 0, and falls through when v0(=ret(0xc0000000000cf760)) == 0.
  The `beq` branch at 0xc0000000000cf690 targets 0xc0000000000cf5c4 and is always taken.
  The return at 0xc0000000000cf5ec has v0 = ?.
- `dev_chain` in `6.5.22/unix.IP35` at 0xc0000000001aa898 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001aa898-0xc0000000001aa984 (236 bytes).
  It occupies 236 bytes.
  The `ld` at 0xc0000000001aa8a4 loads 8 bytes from a0 (arg0 at entry) +0x38.
  2 such `lw` accesses load 4 bytes from abs 0xc00000000160a270 (v+48), including `lw` at 0xc0000000001aa8b8, `lw` at 0xc0000000001aa91c.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x50, including `lw` at 0xc0000000001aa8c0, `lw` at 0xc0000000001aa904.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x28, including `ld` at 0xc0000000001aa8e0, `ld` at 0xc0000000001aa8f4.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x30, including `ld` at 0xc0000000001aa8e4, `ld` at 0xc0000000001aa8ec.
  The `sd` at 0xc0000000001aa8e8 stores 8 bytes to [a0+40]:8 +0x30.
  The `sd` at 0xc0000000001aa900 stores 8 bytes to [a0+48]:8 +0x28.
  2 such `ld` accesses load 8 bytes from ?, including `ld` at 0xc0000000001aa93c, `ld` at 0xc0000000001aa948.
  The `sd` at 0xc0000000001aa940 stores 8 bytes to a0 (arg0 at entry) +0x30.
  The `sd` at 0xc0000000001aa944 stores 8 bytes to a0 (arg0 at entry) +0x28.
  2 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc0000000001aa94c, `sd` at 0xc0000000001aa954.
  The `sd` at 0xc0000000001aa960 stores 8 bytes to a0 (arg0 at entry) +0x38.
  The `jal` at 0xc0000000001aa8d8 calls mutex_spinlock [0xc0000000002fab0c] with a0=[a0+56]:8+56, a1=a1 at entry (arg1), a2=[a0+56]:8, a3=[0xc00000000160a270].
  The `jal` at 0xc0000000001aa8fc calls mutex_spinunlock [0xc0000000002fab58] with a0=[a0+56]:8+56, a1=ret(mutex_spinlock), a2=?, a3=?, a6=[a0+40]:8, a7=[a0+48]:8.
  The `jal` at 0xc0000000001aa934 calls mutex_spinlock [0xc0000000002fab0c] with a0=?, a1=?, a2=0xc0000000015ebe00, a3=0xc00000000160a240.
  The `jal` at 0xc0000000001aa95c calls mutex_spinunlock [0xc0000000002fab58] with a0=?, a1=ret(mutex_spinlock), a2=?, a3=?.
  The `beq` branch at 0xc0000000001aa8b4 targets 0xc0000000001aa904, is taken when a2(=[a0+56]:8) == 0, and falls through when a2(=[a0+56]:8) != 0.
  The `beq` branch at 0xc0000000001aa8d0 targets 0xc0000000001aa968, is taken when a2(=[a0+56]:8) == a4(?), and falls through when a2(=[a0+56]:8) != a4(?).
  The `beq` branch at 0xc0000000001aa914 targets 0xc0000000001aa97c, is taken when a0(=[a0+80]) == t2(=-0x1), and falls through when a0(=[a0+80]) != t2(=-0x1).
  The `beq` branch at 0xc0000000001aa97c targets 0xc0000000001aa930 and is always taken.
  The return at 0xc0000000001aa974 has v0 = one of ?, ret(mutex_spinunlock).
- `hub_driver_event_handler` in `6.5.22/unix.IP35` at 0xc0000000001cf908 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001cf908-0xc0000000001cf9c8 (192 bytes).
  It occupies 192 bytes.
  The `lw` at 0xc0000000001cf908 loads 4 bytes from a0 (arg0 at entry) +0x0.
  The `lw` at 0xc0000000001cf944 loads 4 bytes from abs 0xc0000000015015f4 (usb_dbg_level+20).
  4 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x8, including `ld` at 0xc0000000001cf96c, `ld` at 0xc0000000001cf98c, `ld` at 0xc0000000001cf99c.
  The `jal` at 0xc0000000001cf968 calls usb_iface_class [0xc0000000001c6194] with a0=[a0+8]:8, a1=[a0], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=0x1, a5=0x2, a6=0xc0000000015015e0.
  The `jal` at 0xc0000000001cf988 calls usb_hub_attach [0xc0000000001cf9c8] with a0=[a0+8]:8, a1=[a0], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=0x1, a5=0x2, a6=0xc0000000015015e0.
  The `jal` at 0xc0000000001cf998 calls usb_hub_detach [0xc0000000001d0040] with a0=[a0+8]:8, a1=[a0], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=0x1, a5=0x2, a6=0xc0000000015015e0.
  The `jal` at 0xc0000000001cf9a8 calls usb_hub_detach [0xc0000000001d0040] with a0=[a0+8]:8, a1=[a0], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=0x1, a5=0x2, a6=0xc0000000015015e0.
  The `jal` at 0xc0000000001cf9b8 calls usb_dbg [0xc0000000001d1c9c] with a0=0xc00000000046b180, a1=[a0], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=0x1, a5=0x2, a6=0xc0000000015015e0.
  The `beq` branch at 0xc0000000001cf924 targets 0xc0000000001cf968, is taken when a1(=[a0]) == at(=0x3), and falls through when a1(=[a0]) != at(=0x3).
  The `beq` branch at 0xc0000000001cf92c targets 0xc0000000001cf988, is taken when a1(=[a0]) == 0, and falls through when a1(=[a0]) != 0.
  The `beq` branch at 0xc0000000001cf934 targets 0xc0000000001cf998, is taken when a1(=[a0]) == a4(=0x1), and falls through when a1(=[a0]) != a4(=0x1).
  The `beq` branch at 0xc0000000001cf93c targets 0xc0000000001cf9a8, is taken when a1(=[a0]) == a5(=0x2), and falls through when a1(=[a0]) != a5(=0x2).
  The `bgtz` branch at 0xc0000000001cf948 targets 0xc0000000001cf9b8, is taken when v0(=[0xc0000000015015f4]) > 0 (signed), and falls through when v0(=[0xc0000000015015f4]) <= 0 (signed).
  The `beq` branch at 0xc0000000001cf980 targets 0xc0000000001cf958 and is always taken.
  The `beq` branch at 0xc0000000001cf990 targets 0xc0000000001cf958 and is always taken.
  The `beq` branch at 0xc0000000001cf9a0 targets 0xc0000000001cf958 and is always taken.
  The `beq` branch at 0xc0000000001cf9b0 targets 0xc0000000001cf958 and is always taken.
  The `beq` branch at 0xc0000000001cf9c0 targets 0xc0000000001cf954 and is always taken.
  The return at 0xc0000000001cf960 has v0 = one of 0x625f0005, 0x625f0000, 0x625f0019, ret(usb_hub_attach), ret(usb_hub_detach).
- `hub_status_change` in `6.5.22/unix.IP35` at 0xc0000000001d1058 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001d1058-0xc0000000001d11c0 (360 bytes).
  It occupies 360 bytes.
  3 such `lw` accesses load 4 bytes from abs 0xc0000000015015f4 (usb_dbg_level+20), including `lw` at 0xc0000000001d10a0, `lw` at 0xc0000000001d1108, `lw` at 0xc0000000001d1180.
  The `jal` at 0xc0000000001d1064 calls usb_hub_path [0xc0000000001d1c5c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001d1074 calls usb_dbg [0xc0000000001d1c9c] with a0=0xc00000000046bb30, a1=ret(usb_hub_path), a2=?, a3=?.
  The `jal` at 0xc0000000001d1084 calls usb_get_hub_status [0xc0000000001d0648] with a0=a0 at entry (arg0), a1=sp-32, a2=0x0, a3=?.
  The `jal` at 0xc0000000001d10e0 calls usb_dbg [0xc0000000001d1c9c] with a0=0xc00000000046bbf0, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001d10f0 calls usb_clear_hub_feature [0xc0000000001d08c8] with a0=a0 at entry (arg0), a1=0x1, a2=0x0, a3=?.
  The `jal` at 0xc0000000001d1120 calls usb_hub_overcurrent_event [0xc0000000001d11c0] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001d1130 calls usb_hub_path [0xc0000000001d1c5c] with a0=a0 at entry (arg0), a1=[0xc0000000015015f4], a2=0x625f0000, a3=?.
  The `jal` at 0xc0000000001d1144 calls usb_dbg [0xc0000000001d1c9c] with a0=0xc00000000046bb50, a1=ret(usb_hub_path), a2=ret(usb_get_hub_status), a3=?.
  The `jal` at 0xc0000000001d115c calls usb_dbg [0xc0000000001d1c9c] with a0=0xc00000000046bb98, a1=?, a2=0x625f0000, a3=?.
  The `jal` at 0xc0000000001d116c calls usb_clear_hub_feature [0xc0000000001d08c8] with a0=a0 at entry (arg0), a1=0x0, a2=0x0, a3=?.
  The `jal` at 0xc0000000001d1198 calls usb_dbg [0xc0000000001d1c9c] with a0=0xc00000000046bbb8, a1=ret(usb_clear_hub_feature), a2=[0xc0000000015015f4], a3=?.
  The `jal` at 0xc0000000001d11b0 calls usb_dbg [0xc0000000001d1c9c] with a0=0xc00000000046bc10, a1=ret(usb_clear_hub_feature), a2=[0xc0000000015015f4], a3=?.
  The `beq` branch at 0xc0000000001d1098 targets 0xc0000000001d10bc, is taken when v0(=ret(usb_get_hub_status)) == a2(=0x625f0000), and falls through when v0(=ret(usb_get_hub_status)) != a2(=0x625f0000).
  The `bgtz` branch at 0xc0000000001d10a4 targets 0xc0000000001d1130, is taken when a1(=[0xc0000000015015f4]) > 0 (signed), and falls through when a1(=[0xc0000000015015f4]) <= 0 (signed).
  The `bne` branch at 0xc0000000001d10bc targets 0xc0000000001d1154, is taken when a5(?) != 0, and falls through when a5(?) == 0.
  The `bgez` branch at 0xc0000000001d10c8 targets 0xc0000000001d10b0, is taken when a2(?) >= 0 (signed), and falls through when a2(?) < 0 (signed).
  The `beq` branch at 0xc0000000001d10fc targets 0xc0000000001d1114, is taken when v0(=ret(usb_clear_hub_feature)) == a1(=0x625f0000), and falls through when v0(=ret(usb_clear_hub_feature)) != a1(=0x625f0000).
  The `bgtz` branch at 0xc0000000001d110c targets 0xc0000000001d11a8, is taken when a2(=[0xc0000000015015f4]) > 0 (signed), and falls through when a2(=[0xc0000000015015f4]) <= 0 (signed).
  The `bgez` branch at 0xc0000000001d1118 targets 0xc0000000001d10b0, is taken when a3(?) >= 0 (signed), and falls through when a3(?) < 0 (signed).
  The `beq` branch at 0xc0000000001d1128 targets 0xc0000000001d10b0 and is always taken.
  The `beq` branch at 0xc0000000001d114c targets 0xc0000000001d10b0 and is always taken.
  The `beq` branch at 0xc0000000001d1178 targets 0xc0000000001d118c, is taken when v0(=ret(usb_clear_hub_feature)) == a1(=0x625f0000), and falls through when v0(=ret(usb_clear_hub_feature)) != a1(=0x625f0000).
  The `bgtz` branch at 0xc0000000001d1184 targets 0xc0000000001d1194, is taken when a2(=[0xc0000000015015f4]) > 0 (signed), and falls through when a2(=[0xc0000000015015f4]) <= 0 (signed).
  The `beq` branch at 0xc0000000001d118c targets 0xc0000000001d10c4 and is always taken.
  The `beq` branch at 0xc0000000001d11a0 targets 0xc0000000001d118c and is always taken.
  The `beq` branch at 0xc0000000001d11b8 targets 0xc0000000001d1114 and is always taken.
  The return at 0xc0000000001d10b4 has v0 = one of ret(usb_get_hub_status), ?, ret(usb_hub_overcurrent_event), ret(usb_dbg).
- `ioc3_config` in `6.5.22/unix.IP35` at 0xc0000000001ecae4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ecae4-0xc0000000001ece74 (912 bytes).
  It occupies 912 bytes.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc0000000001ecb64, `lw` at 0xc0000000001ecc9c, `lw` at 0xc0000000001ece28.
  4 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000001ecb80, `ld` at 0xc0000000001ecc00, `ld` at 0xc0000000001eccbc.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000001ecb94, `ld` at 0xc0000000001ecc14, `ld` at 0xc0000000001eccd0.
  4 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000001ecba0, `lw` at 0xc0000000001ecc20, `lw` at 0xc0000000001eccdc.
  6 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000001ecbac, `ld` at 0xc0000000001ecc2c, `ld` at 0xc0000000001ecce8.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc0000000001ecbb8, `ld` at 0xc0000000001ecbc8, `ld` at 0xc0000000001eccf4.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc0000000001ecbc4, `sw` at 0xc0000000001eccf8.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc0000000001ecbcc, `lw` at 0xc0000000001ecdb0.
  The `ld` at 0xc0000000001ecc5c loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xc0000000001ecc60 loads 1 byte from [a0+72]:8 +0x3.
  The `sb` at 0xc0000000001ecc98 stores 1 byte to [a0+72]:8 +0x3.
  The `sw` at 0xc0000000001ecd90 stores 4 bytes to a0 (arg0 at entry) +0x80.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x84, including `sw` at 0xc0000000001ecd98, `sw` at 0xc0000000001ecddc.
  The `lw` at 0xc0000000001ecda4 loads 4 bytes from a0 (arg0 at entry) +0x88.
  The `jal` at 0xc0000000001ecb08 calls set_baud_ti [0xc0000000001ebe88] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jalr` at 0xc0000000001ecc40 calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8], a2=[[0xa800000000001020]:8+128]:8, a3=[0xc00000000159b68c], a7=0x40000000.
  The `jal` at 0xc0000000001ecc48 calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jalr` at 0xc0000000001ecd48 calls (unknown callee) with a0=0x1, a1=0xfffffffffeeddead, a2=[[0xa800000000001020]:8+128]:8, a3=a1 at entry (arg1).
  The `jal` at 0xc0000000001ecd50 calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001ecda0 calls ioc3_rx_timeout [0xc0000000001ee13c] with a0=a0 at entry (arg0), a1=[a0+136], a2=?, a3=a1 at entry (arg1), a6=0xa.
  The `jal` at 0xc0000000001ece08 calls spl7 [0xc00000000006287c] with a0=[a0+148], a1=([a0+148] << 0x23), a2=0xa800000000001020, a3=[0xa800000000001000]:8, a4=[[0xa800000000001020]:8], a5=0xfffffffffeeddead, a6=[[0xa800000000001020]:8+128]:8.
  The `jalr` at 0xc0000000001ece20 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001ece38 calls spl7 [0xc00000000006287c] with a0=[a0+148], a1=?, a2=[a0+72]:8, a3=?, a4=[0xc00000000159b690]:8, a5=[0xa800000000001000]:8, a6=[[0xa800000000001020]:8], a7=0xfffffffffeeddead.
  The `jalr` at 0xc0000000001ece4c calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `beq` branch at 0xc0000000001ecb10 targets 0xc0000000001ecb2c, is taken when v0(=ret(set_baud_ti)) == 0, and falls through when v0(=ret(set_baud_ti)) != 0.
  The `beq` branch at 0xc0000000001ecb30 targets 0xc0000000001ecb60, is taken when s1(=a2) == at(=0x5), and falls through when s1(=a2) != at(=0x5).
  The `beq` branch at 0xc0000000001ecb38 targets 0xc0000000001ecde0, is taken when s1(=a2) == v0(=0x6), and falls through when s1(=a2) != v0(=0x6).
  The `beq` branch at 0xc0000000001ecb40 targets 0xc0000000001ecde8, is taken when s1(=a2) == v1(=0x7), and falls through when s1(=a2) != v1(=0x7).
  The `beq` branch at 0xc0000000001ecb48 targets 0xc0000000001ecb58, is taken when s1(=a2) == a0(=0x8), and falls through when s1(=a2) != a0(=0x8).
  The `beq` branch at 0xc0000000001ecb50 targets 0xc0000000001ecb18 and is always taken.
  The `beq` branch at 0xc0000000001ecb58 targets 0xc0000000001ecb64 and is always taken.
  The `bgez` branch at 0xc0000000001ecb6c targets 0xc0000000001ecc5c, is taken when a1(=([a0+148] << 0x23)) >= 0 (signed), and falls through when a1(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc0000000001ecb74 targets 0xc0000000001ecbb8, is taken when a2(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a2(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ecb90 targets 0xc0000000001ecbb8, is taken when a3(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a3(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc0000000001ecb98 targets 0xc0000000001ecbb8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ecba4 targets 0xc0000000001ecbb8, is taken when a4(=[[0xa800000000001020]:8]) != a5(=0xfffffffffeeddead), and falls through when a4(=[[0xa800000000001020]:8]) == a5(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001ecbb0 targets 0xc0000000001ece08, is taken when a6(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bltz` branch at 0xc0000000001ecbd4 targets 0xc0000000001ecbec, is taken when a7(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when a7(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `beq` branch at 0xc0000000001ecbe4 targets 0xc0000000001ece64 and is always taken.
  The `bne` branch at 0xc0000000001ecbf4 targets 0xc0000000001ecc54, is taken when a1(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a1(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ecc10 targets 0xc0000000001ecc54, is taken when v1(=[0xa800000000001000]:8) != a1(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a1(=0x53435241).
  The `beq` branch at 0xc0000000001ecc18 targets 0xc0000000001ecc54, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ecc24 targets 0xc0000000001ecc54, is taken when a1(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a1(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001ecc30 targets 0xc0000000001ecc54, is taken when a2(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000001ecc54 targets 0xc0000000001ece00, is taken when a0(=one of 0x0, 0x1) == 0, and falls through when a0(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc0000000001ecc70 targets 0xc0000000001ecc88, is taken when v1(=a4) == 0, and falls through when v1(=a4) != 0.
  The `beq` branch at 0xc0000000001ecc80 targets 0xc0000000001ecdf0, is taken when a0(=a5) == 0, and falls through when a0(=a5) != 0.
  The `beq` branch at 0xc0000000001ecc8c targets 0xc0000000001ecc98, is taken when a1(=a3) == 0, and falls through when a1(=a3) != 0.
  The `beq` branch at 0xc0000000001ecca8 targets 0xc0000000001ecd58, is taken when (a3(=0x10000000) & a0(=[a0+148])) == 0, and falls through when (a3(=0x10000000) & a0(=[a0+148])) != 0.
  The `bne` branch at 0xc0000000001eccb0 targets 0xc0000000001eccf4, is taken when a4(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a4(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ecccc targets 0xc0000000001eccf4, is taken when a5(=[0xa800000000001000]:8) != a6(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a6(=0x53435241).
  The `beq` branch at 0xc0000000001eccd4 targets 0xc0000000001eccf4, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ecce0 targets 0xc0000000001eccf4, is taken when a6(=[[0xa800000000001020]:8]) != a7(=0xfffffffffeeddead), and falls through when a6(=[[0xa800000000001020]:8]) == a7(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001eccec targets 0xc0000000001ece38, is taken when t0(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when t0(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc0000000001ecd00 targets 0xc0000000001ecd58, is taken when t1(=[0xc00000000159b690]:8) != s0(=a0), and falls through when t1(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ecd1c targets 0xc0000000001ecd58, is taken when v1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000001ecd24 targets 0xc0000000001ecd5c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ecd30 targets 0xc0000000001ecd5c, is taken when a0(=[[0xa800000000001020]:8]) != a1(=0xfffffffffeeddead), and falls through when a0(=[[0xa800000000001020]:8]) == a1(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001ecd3c targets 0xc0000000001ecd5c, is taken when a2(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000001ecd94 targets 0xc0000000001ecdd4, is taken when a2(?) == 0, and falls through when a2(?) != 0.
  The `beq` branch at 0xc0000000001ecda8 targets 0xc0000000001ecb18 and is always taken.
  The `bne` branch at 0xc0000000001ecdbc targets 0xc0000000001ecbec, is taken when (t0(=[[a0+64]:8]) & a7(=0x40000000)) != 0, and falls through when (t0(=[[a0+64]:8]) & a7(=0x40000000)) == 0.
  The `beq` branch at 0xc0000000001ecdc4 targets 0xc0000000001ecdb0, is taken when t1(?) == 0, and falls through when t1(?) != 0.
  The `beq` branch at 0xc0000000001ecdcc targets 0xc0000000001ecbf0 and is always taken.
  The `beq` branch at 0xc0000000001ecdd8 targets 0xc0000000001ecd9c and is always taken.
  The `beq` branch at 0xc0000000001ecde0 targets 0xc0000000001ecb64 and is always taken.
  The `beq` branch at 0xc0000000001ecde8 targets 0xc0000000001ecb64 and is always taken.
  The `bne` branch at 0xc0000000001ecdf0 targets 0xc0000000001ecc94, is taken when t3(=a3) != 0, and falls through when t3(=a3) == 0.
  The `beq` branch at 0xc0000000001ecdf8 targets 0xc0000000001ecc98 and is always taken.
  The `beq` branch at 0xc0000000001ece00 targets 0xc0000000001ecb18 and is always taken.
  The `beq` branch at 0xc0000000001ece30 targets 0xc0000000001ecbb8 and is always taken.
  The `beq` branch at 0xc0000000001ece5c targets 0xc0000000001eccf4 and is always taken.
  The `beq` branch at 0xc0000000001ece6c targets 0xc0000000001ecdc4 and is always taken.
  The return at 0xc0000000001ecb24 has v0 = one of -0x1, 0x0, 0x1.
- `ioc3_set_extclk` in `6.5.22/unix.IP35` at 0xc0000000001ecf98 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ecf98-0xc0000000001ecfa0 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001ecf98 has v0 = a1.
- `ioc3_write` in `6.5.22/unix.IP35` at 0xc0000000001ed21c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ed21c-0xc0000000001ed238 (28 bytes).
  It occupies 28 bytes.
  The `jal` at 0xc0000000001ed224 calls do_ioc3_write [0xc0000000001ecfa0] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The return at 0xc0000000001ed230 has v0 = ret(do_ioc3_write).
- `ioc3_sync_write` in `6.5.22/unix.IP35` at 0xc0000000001ed238 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ed238-0xc0000000001ed294 (92 bytes).
  It occupies 92 bytes.
  The `lw` at 0xc0000000001ed264 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xc0000000001ed24c calls do_ioc3_write [0xc0000000001ecfa0] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001ed274 calls ioc3_enable_tx [0xc0000000001ed600] with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `blez` branch at 0xc0000000001ed254 targets 0xc0000000001ed27c, is taken when s3(=a2) <= 0 (signed), and falls through when s3(=a2) > 0 (signed).
  The `bne` branch at 0xc0000000001ed25c targets 0xc0000000001ed27c, is taken when v0(=ret(do_ioc3_write)) != 0, and falls through when v0(=ret(do_ioc3_write)) == 0.
  The `beq` branch at 0xc0000000001ed26c targets 0xc0000000001ed27c, is taken when (at(=[a0+120]) & 0x10) == 0, and falls through when (at(=[a0+120]) & 0x10) != 0.
  The return at 0xc0000000001ed28c has v0 = ret(do_ioc3_write).
- `ioc3_wrflush` in `6.5.22/unix.IP35` at 0xc0000000001ed294 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ed294-0xc0000000001ed358 (196 bytes).
  It occupies 196 bytes.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x38, including `ld` at 0xc0000000001ed294, `ld` at 0xc0000000001ed2d0, `ld` at 0xc0000000001ed318.
  3 such `lw` accesses load 4 bytes from [a0+56]:8 +0x1c, including `lw` at 0xc0000000001ed298, `lw` at 0xc0000000001ed2d4, `lw` at 0xc0000000001ed31c.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc0000000001ed2b4, `ld` at 0xc0000000001ed30c, `ld` at 0xc0000000001ed334.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc0000000001ed2b8, `lw` at 0xc0000000001ed338.
  The `lw` at 0xc0000000001ed2f0 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xc0000000001ed300 calls ioc3_enable_tx [0xc0000000001ed600] with a0=a0 at entry (arg0), a1=0x1, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001ed32c calls us_delay [0xc000000000069c48] with a0=0x5, a1=?, a2=one of 0x10000000, 0x50000000, a3=?.
  The `beq` branch at 0xc0000000001ed2ac targets 0xc0000000001ed2f0, is taken when (at(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (at(=[[a0+56]:8+28]) & 0x1) != 0.
  The `bne` branch at 0xc0000000001ed2c8 targets 0xc0000000001ed2e4, is taken when a0(=0x10000000) != v0(?), and falls through when a0(=0x10000000) == v0(?).
  The `beq` branch at 0xc0000000001ed2dc targets 0xc0000000001ed310, is taken when (v1(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (v1(=[[a0+56]:8+28]) & 0x1) != 0.
  The `beq` branch at 0xc0000000001ed2f8 targets 0xc0000000001ed2b4, is taken when (a0(=[a0+120]) & 0x10) == 0, and falls through when (a0(=[a0+120]) & 0x10) != 0.
  The `beq` branch at 0xc0000000001ed308 targets 0xc0000000001ed2b8 and is always taken.
  The `beq` branch at 0xc0000000001ed310 targets 0xc0000000001ed32c and is always taken.
  The `bne` branch at 0xc0000000001ed324 targets 0xc0000000001ed350, is taken when (a1(=[[a0+56]:8+28]) & 0x1) != 0, and falls through when (a1(=[[a0+56]:8+28]) & 0x1) == 0.
  The `beq` branch at 0xc0000000001ed348 targets 0xc0000000001ed318, is taken when a2(=0x10000000) == a3(?), and falls through when a2(=0x10000000) != a3(?).
  The `beq` branch at 0xc0000000001ed350 targets 0xc0000000001ed2e4 and is always taken.
  The return at 0xc0000000001ed2e8 has v0 = one of ?, ret(us_delay).
- `ioc3_break` in `6.5.22/unix.IP35` at 0xc0000000001ed358 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ed358-0xc0000000001ed600 (680 bytes).
  It occupies 680 bytes.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc0000000001ed358, `lw` at 0xc0000000001ed484, `lw` at 0xc0000000001ed5b0.
  4 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000001ed38c, `ld` at 0xc0000000001ed418, `ld` at 0xc0000000001ed4a0.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000001ed3a0, `ld` at 0xc0000000001ed41c, `ld` at 0xc0000000001ed4b4.
  4 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000001ed3ac, `lw` at 0xc0000000001ed430, `lw` at 0xc0000000001ed4c0.
  6 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000001ed3b8, `ld` at 0xc0000000001ed43c, `ld` at 0xc0000000001ed4cc.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc0000000001ed3c4, `ld` at 0xc0000000001ed3d4, `ld` at 0xc0000000001ed4d8.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc0000000001ed3d0, `sw` at 0xc0000000001ed4dc.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc0000000001ed3d8, `lw` at 0xc0000000001ed558.
  The `ld` at 0xc0000000001ed470 loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xc0000000001ed478 loads 1 byte from [a0+72]:8 +0x3.
  2 such `sb` accesses store 1 byte to [a0+72]:8 +0x3, including `sb` at 0xc0000000001ed480, `sb` at 0xc0000000001ed588.
  The `jalr` at 0xc0000000001ed450 calls (unknown callee) with a0=0x1, a1=a1 at entry (arg1), a2=[[0xa800000000001020]:8], a3=0xfffffffffeeddead, a4=[[0xa800000000001020]:8+128]:8, a5=[0xc00000000159b68c], a6=0x10000000, a7=[0xc00000000159b690]:8.
  The `jal` at 0xc0000000001ed458 calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jalr` at 0xc0000000001ed530 calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=?, a4=[[0xa800000000001020]:8+128]:8.
  The `jal` at 0xc0000000001ed538 calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001ed594 calls spl7 [0xc00000000006287c] with a0=[a0+72]:8, a1=a1 at entry (arg1), a2=?, a3=?, a4=[a0+148], a6=0x10000000.
  The `jalr` at 0xc0000000001ed5a8 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a5=[[0xa800000000001020]:8+128]:8.
  The `jal` at 0xc0000000001ed5c0 calls spl7 [0xc00000000006287c] with a0=[[0xa800000000001020]:8], a1=a1 at entry (arg1), a2=0xfffffffffeeddead, a3=[[0xa800000000001020]:8+128]:8, a4=[a0+148], a6=0x10000000.
  The `jalr` at 0xc0000000001ed5d4 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+128]:8.
  The `bgez` branch at 0xc0000000001ed374 targets 0xc0000000001ed470, is taken when at(=([a0+148] << 0x23)) >= 0 (signed), and falls through when at(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc0000000001ed380 targets 0xc0000000001ed3c4, is taken when v0(=[0xc00000000159b690]:8) != a0, and falls through when v0(=[0xc00000000159b690]:8) == a0.
  The `bne` branch at 0xc0000000001ed39c targets 0xc0000000001ed3c4, is taken when v1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000001ed3a4 targets 0xc0000000001ed3c4, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ed3b0 targets 0xc0000000001ed3c4, is taken when a0(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a0(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001ed3bc targets 0xc0000000001ed5c0, is taken when a3(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a3(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bltz` branch at 0xc0000000001ed3ec targets 0xc0000000001ed3fc, is taken when t0(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t0(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `beq` branch at 0xc0000000001ed3f4 targets 0xc0000000001ed5f4 and is always taken.
  The `bne` branch at 0xc0000000001ed414 targets 0xc0000000001ed468, is taken when a7(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a7(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ed420 targets 0xc0000000001ed468, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000001ed428 targets 0xc0000000001ed468, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ed434 targets 0xc0000000001ed468, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001ed440 targets 0xc0000000001ed468, is taken when a4(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a4(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000001ed468 targets 0xc0000000001ed58c, is taken when s1(=one of 0x0, 0x1) == 0, and falls through when s1(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc0000000001ed474 targets 0xc0000000001ed57c, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc0000000001ed48c targets 0xc0000000001ed540, is taken when (a6(=0x10000000) & a4(=[a0+148])) == 0, and falls through when (a6(=0x10000000) & a4(=[a0+148])) != 0.
  The `bne` branch at 0xc0000000001ed494 targets 0xc0000000001ed4d8, is taken when t1(=[0xc00000000159b690]:8) != s0(=a0), and falls through when t1(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ed4b0 targets 0xc0000000001ed4d8, is taken when t2(=[0xa800000000001000]:8) != t3(=0x53435241), and falls through when t2(=[0xa800000000001000]:8) == t3(=0x53435241).
  The `beq` branch at 0xc0000000001ed4b8 targets 0xc0000000001ed4d8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ed4c4 targets 0xc0000000001ed4d8, is taken when t3(=[[0xa800000000001020]:8]) != t8(=0xfffffffffeeddead), and falls through when t3(=[[0xa800000000001020]:8]) == t8(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001ed4d0 targets 0xc0000000001ed594, is taken when t9(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when t9(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc0000000001ed4e4 targets 0xc0000000001ed544, is taken when ra(=[0xc00000000159b690]:8) != s0(=a0), and falls through when ra(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ed4fc targets 0xc0000000001ed544, is taken when v0(=[0xa800000000001000]:8) != v1(=0x53435241), and falls through when v0(=[0xa800000000001000]:8) == v1(=0x53435241).
  The `beq` branch at 0xc0000000001ed50c targets 0xc0000000001ed540, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ed518 targets 0xc0000000001ed540, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001ed524 targets 0xc0000000001ed544, is taken when a4(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a4(=[[0xa800000000001020]:8+128]:8) != 0.
  The `bne` branch at 0xc0000000001ed564 targets 0xc0000000001ed3fc, is taken when (a2(=[[a0+64]:8]) & a7(=0x40000000)) != 0, and falls through when (a2(=[[a0+64]:8]) & a7(=0x40000000)) == 0.
  The `beq` branch at 0xc0000000001ed56c targets 0xc0000000001ed558, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0xc0000000001ed574 targets 0xc0000000001ed404 and is always taken.
  The `beq` branch at 0xc0000000001ed584 targets 0xc0000000001ed484 and is always taken.
  The `beq` branch at 0xc0000000001ed58c targets 0xc0000000001ed544 and is always taken.
  The `beq` branch at 0xc0000000001ed5b8 targets 0xc0000000001ed4d8 and is always taken.
  The `beq` branch at 0xc0000000001ed5ec targets 0xc0000000001ed3c4 and is always taken.
  The `beq` branch at 0xc0000000001ed5f8 targets 0xc0000000001ed56c and is always taken.
  The return at 0xc0000000001ed550 has v0 = one of -0x1, 0x0.
- `ioc3_enable_tx` in `6.5.22/unix.IP35` at 0xc0000000001ed600 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ed600-0xc0000000001ed814 (532 bytes).
  It occupies 532 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xc0000000001ed60c, `lw` at 0xc0000000001ed6d8.
  The `ld` at 0xc0000000001ed614 loads 8 bytes from a0 (arg0 at entry) +0x70.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000001ed640, `ld` at 0xc0000000001ed720.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000001ed644, `ld` at 0xc0000000001ed724, `ld` at 0xc0000000001ed7f0.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000001ed658, `lw` at 0xc0000000001ed748.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000001ed664, `ld` at 0xc0000000001ed754, `ld` at 0xc0000000001ed7f4.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc0000000001ed670, `lw` at 0xc0000000001ed6fc.
  4 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc0000000001ed67c, `ld` at 0xc0000000001ed68c, `ld` at 0xc0000000001ed714.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc0000000001ed688, `sw` at 0xc0000000001ed718.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc0000000001ed690, `lw` at 0xc0000000001ed6c0.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x98, including `lw` at 0xc0000000001ed6dc, `lw` at 0xc0000000001ed7bc.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xc0000000001ed6e8, `sw` at 0xc0000000001ed7a4.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x4, including `sw` at 0xc0000000001ed6ec, `sw` at 0xc0000000001ed7d8.
  2 such `lw` accesses load 4 bytes from [a0+112]:8 +0x8, including `lw` at 0xc0000000001ed6f8, `lw` at 0xc0000000001ed7ac.
  The `lw` at 0xc0000000001ed79c loads 4 bytes from [a0+64]:8 +0x8.
  The `jal` at 0xc0000000001ed6f4 calls enable_intrs [0xc0000000001eb848] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=?, a3=?, a4=[a0+64]:8.
  The `jalr` at 0xc0000000001ed764 calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8+128]:8, a2=[0xc00000000159b690]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=0xa800000000001000.
  The `jal` at 0xc0000000001ed76c calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001ed7b4 calls disable_intrs [0xc0000000001eb898] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=?, a3=?, a4=[a0+64]:8, a6=([a0+120] | 0x10).
  The `jal` at 0xc0000000001ed7e4 calls spl7 [0xc00000000006287c] with a0=0x53435241, a1=a1 at entry (arg1), a2=[[0xa800000000001020]:8], a3=0xfffffffffeeddead, a4=[[0xa800000000001020]:8+128]:8, a5=0xa800000000001020, a6=[0xc00000000159b690]:8.
  The `jalr` at 0xc0000000001ed7fc calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `beq` branch at 0xc0000000001ed61c targets 0xc0000000001ed78c, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc0000000001ed624 targets 0xc0000000001ed794, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `bne` branch at 0xc0000000001ed63c targets 0xc0000000001ed670, is taken when a6(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a6(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ed648 targets 0xc0000000001ed670, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000001ed650 targets 0xc0000000001ed670, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ed65c targets 0xc0000000001ed670, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001ed668 targets 0xc0000000001ed7e4, is taken when a4(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a4(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bgez` branch at 0xc0000000001ed678 targets 0xc0000000001ed6d4, is taken when a5(=([a0+148] << 0x23)) >= 0 (signed), and falls through when a5(=([a0+148] << 0x23)) < 0 (signed).
  The `bltz` branch at 0xc0000000001ed6a8 targets 0xc0000000001ed6d4, is taken when t0(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t0(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `bne` branch at 0xc0000000001ed6b8 targets 0xc0000000001ed7dc, is taken when t2(?) != 0, and falls through when t2(?) == 0.
  The `beq` branch at 0xc0000000001ed6cc targets 0xc0000000001ed6b8, is taken when (t3(=[[a0+64]:8]) & a6(=0x40000000)) == 0, and falls through when (t3(=[[a0+64]:8]) & a6(=0x40000000)) != 0.
  The `beq` branch at 0xc0000000001ed6d4 targets 0xc0000000001ed79c, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc0000000001ed70c targets 0xc0000000001ed71c, is taken when (ra(=0x10000000) & a0(=[a0+148])) == 0, and falls through when (ra(=0x10000000) & a0(=[a0+148])) != 0.
  The `bne` branch at 0xc0000000001ed730 targets 0xc0000000001ed774, is taken when a2(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a2(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ed738 targets 0xc0000000001ed774, is taken when a1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000001ed740 targets 0xc0000000001ed774, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ed74c targets 0xc0000000001ed774, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001ed758 targets 0xc0000000001ed778, is taken when a1(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a1(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000001ed78c targets 0xc0000000001ed62c, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc0000000001ed794 targets 0xc0000000001ed778 and is always taken.
  The `beq` branch at 0xc0000000001ed7c8 targets 0xc0000000001ed6fc, is taken when a7(=[a0+152]) == v0(?), and falls through when a7(=[a0+152]) != v0(?).
  The `beq` branch at 0xc0000000001ed7d4 targets 0xc0000000001ed6fc and is always taken.
  The `beq` branch at 0xc0000000001ed7dc targets 0xc0000000001ed77c and is always taken.
  The `beq` branch at 0xc0000000001ed80c targets 0xc0000000001ed670 and is always taken.
  The return at 0xc0000000001ed784 has v0 = one of -0x1, 0x0.
- `ioc3_read` in `6.5.22/unix.IP35` at 0xc0000000001ed814 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ed814-0xc0000000001edf3c (1832 bytes).
  It occupies 1832 bytes.
  The `ld` at 0xc0000000001ed820 loads 8 bytes from a0 (arg0 at entry) +0x70.
  11 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc0000000001ed838, `ld` at 0xc0000000001ed850, `ld` at 0xc0000000001ed93c.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x9c, including `lw` at 0xc0000000001ed83c, `lw` at 0xc0000000001ed85c.
  3 such `sw` accesses store 4 bytes to [a0+64]:8 +0x10, including `sw` at 0xc0000000001ed84c, `sw` at 0xc0000000001edbe8, `sw` at 0xc0000000001edee8.
  4 such `lw` accesses load 4 bytes from [a0+64]:8 +0xc, including `lw` at 0xc0000000001ed858, `lw` at 0xc0000000001ed940, `lw` at 0xc0000000001eda0c.
  5 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc0000000001ed87c, `lw` at 0xc0000000001eda1c, `lw` at 0xc0000000001edbfc.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000001ed89c, `ld` at 0xc0000000001ed8f0, `ld` at 0xc0000000001ed958.
  9 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000001ed8b0, `ld` at 0xc0000000001ed904, `ld` at 0xc0000000001ed96c.
  6 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000001ed8bc, `lw` at 0xc0000000001ed910, `lw` at 0xc0000000001ed978.
  9 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000001ed8cc, `ld` at 0xc0000000001ed920, `ld` at 0xc0000000001ed984.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc0000000001ed8e0, `sw` at 0xc0000000001eda74.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc0000000001ed990, `lw` at 0xc0000000001edd68.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xc0000000001eda28, `sw` at 0xc0000000001eddfc.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xc0000000001edad0, `lw` at 0xc0000000001edb48, `lw` at 0xc0000000001edbec.
  The `ld` at 0xc0000000001edad4 loads 8 bytes from a0 (arg0 at entry) +0x60.
  4 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xc0000000001edae4, `sw` at 0xc0000000001edd94, `sw` at 0xc0000000001eddec.
  6 such `lw` accesses load 4 bytes from ?, including `lw` at 0xc0000000001edb08, `lw` at 0xc0000000001edbc8, `lw` at 0xc0000000001edd40.
  7 such `lbu` accesses load 1 byte from ?, including `lbu` at 0xc0000000001edb24, `lbu` at 0xc0000000001edb8c, `lbu` at 0xc0000000001edc80.
  5 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x8c, including `lw` at 0xc0000000001edb38, `lw` at 0xc0000000001edbb0, `lw` at 0xc0000000001edc90.
  5 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x8, including `ld` at 0xc0000000001edb70, `ld` at 0xc0000000001edc64, `ld` at 0xc0000000001edca0.
  4 such `ld` accesses load 8 bytes from [a0+8]:8 +0x10, including `ld` at 0xc0000000001edb7c, `ld` at 0xc0000000001edc70, `ld` at 0xc0000000001edcac.
  4 such `sb` accesses store 1 byte to ?, including `sb` at 0xc0000000001edb98, `sb` at 0xc0000000001edd10, `sb` at 0xc0000000001edd24.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x9c, including `sw` at 0xc0000000001edbf8, `sw` at 0xc0000000001edef8.
  The `lw` at 0xc0000000001edda0 loads 4 bytes from [a0+112]:8 +0x10.
  The `ld` at 0xc0000000001edf08 loads 8 bytes from [a0+8]:8 +0x18.
  The `jalr` at 0xc0000000001ed92c calls (unknown callee) with a0=0x1, a1=?, a2=[[0xa800000000001020]:8+128]:8, a3=?, a5=[a0+64]:8, a7=[0xc00000000159b690]:8.
  The `jal` at 0xc0000000001ed934 calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jalr` at 0xc0000000001ed9f0 calls (unknown callee) with a0=0x1, a1=[0xa800000000001000]:8, a2=[[0xa800000000001020]:8+128]:8, a3=?, a5=[a0+64]:8.
  The `jal` at 0xc0000000001ed9f8 calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jalr` at 0xc0000000001edac0 calls (unknown callee) with a0=0x1, a1=?, a2=[[0xa800000000001020]:8+128]:8, a3=?, a5=[a0+64]:8, a7=[0xc00000000159b690]:8.
  The `jal` at 0xc0000000001edac8 calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jalr` at 0xc0000000001edb84 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x8, a2=[a0+140], a3=?.
  The `jalr` at 0xc0000000001edc78 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x2, a2=[a0+140], a3=?, a7=[[a0+8]:8+16]:8.
  The `jalr` at 0xc0000000001edcb4 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x4, a2=?, a3=?.
  The `jalr` at 0xc0000000001edcf0 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `jal` at 0xc0000000001edd9c calls enable_intrs [0xc0000000001eb848] with a0=a0 at entry (arg0), a1=[[a0+112]:8+16], a2=?, a3=?.
  The `jal` at 0xc0000000001ede38 calls spl7 [0xc00000000006287c] with a0=?, a1=[0xc00000000159b690]:8, a2=[0xa800000000001000]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[a0+64]:8, a6=[[0xa800000000001020]:8+128]:8.
  The `jalr` at 0xc0000000001ede4c calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a6=[[0xa800000000001020]:8+128]:8.
  The `jal` at 0xc0000000001ede64 calls spl7 [0xc00000000006287c] with a0=?, a1=0xffffffffefffffff, a2=[0xa800000000001000]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[a0+64]:8, a6=[[0xa800000000001020]:8+128]:8.
  The `jalr` at 0xc0000000001ede78 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+128]:8.
  The `jal` at 0xc0000000001ede90 calls spl7 [0xc00000000006287c] with a0=?, a1=?, a2=[0xc00000000159b690]:8, a3=?, a5=[a0+64]:8.
  The `jalr` at 0xc0000000001edea4 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `jalr` at 0xc0000000001edf10 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x0, a2=[a0+140], a3=?.
  The `bne` branch at 0xc0000000001ed874 targets 0xc0000000001edad0, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bgez` branch at 0xc0000000001ed884 targets 0xc0000000001eddf0, is taken when v1(=([a0+148] << 0x23)) >= 0 (signed), and falls through when v1(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc0000000001ed894 targets 0xc0000000001ed8d8, is taken when a1(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a1(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ed8a8 targets 0xc0000000001ed8d8, is taken when a2(=[0xa800000000001000]:8) != a3(=0x53435241), and falls through when a2(=[0xa800000000001000]:8) == a3(=0x53435241).
  The `beq` branch at 0xc0000000001ed8b4 targets 0xc0000000001ed8d8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ed8c4 targets 0xc0000000001ed8d8, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001ed8d0 targets 0xc0000000001ede38, is taken when a6(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc0000000001ed8e8 targets 0xc0000000001ed93c, is taken when a7(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a7(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ed8fc targets 0xc0000000001ed93c, is taken when t1(=[0xa800000000001000]:8) != t2(=0x53435241), and falls through when t1(=[0xa800000000001000]:8) == t2(=0x53435241).
  The `beq` branch at 0xc0000000001ed908 targets 0xc0000000001ed93c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ed918 targets 0xc0000000001ed93c, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001ed924 targets 0xc0000000001ed93c, is taken when a2(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+128]:8) != 0.
  The `bne` branch at 0xc0000000001ed948 targets 0xc0000000001eda14, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bne` branch at 0xc0000000001ed950 targets 0xc0000000001ed990, is taken when s0(=a0) != a2(=[0xc00000000159b690]:8), and falls through when s0(=a0) == a2(=[0xc00000000159b690]:8).
  The `bne` branch at 0xc0000000001ed964 targets 0xc0000000001ed990, is taken when t8(=[0xa800000000001000]:8) != t9(=0x53435241), and falls through when t8(=[0xa800000000001000]:8) == t9(=0x53435241).
  The `beq` branch at 0xc0000000001ed970 targets 0xc0000000001ed990, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ed97c targets 0xc0000000001ed990, is taken when t9(=[[0xa800000000001020]:8]) != ra(=0xfffffffffeeddead), and falls through when t9(=[[0xa800000000001020]:8]) == ra(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001ed988 targets 0xc0000000001ede90, is taken when at(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when at(=[[0xa800000000001020]:8+128]:8) == 0.
  The `beq` branch at 0xc0000000001ed998 targets 0xc0000000001ed9ac, is taken when (v0(=[[a0+64]:8]) & s2(=0x8000000)) == 0, and falls through when (v0(=[[a0+64]:8]) & s2(=0x8000000)) != 0.
  The `beq` branch at 0xc0000000001ed9a4 targets 0xc0000000001edf2c and is always taken.
  The `bne` branch at 0xc0000000001ed9b0 targets 0xc0000000001eda00, is taken when s0(=a0) != a2(=[0xc00000000159b690]:8), and falls through when s0(=a0) == a2(=[0xc00000000159b690]:8).
  The `bne` branch at 0xc0000000001ed9c4 targets 0xc0000000001eda00, is taken when a1(=[0xa800000000001000]:8) != a2(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a2(=0x53435241).
  The `beq` branch at 0xc0000000001ed9d0 targets 0xc0000000001eda00, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ed9dc targets 0xc0000000001eda00, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001ed9e8 targets 0xc0000000001eda00, is taken when a2(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000001eda00 targets 0xc0000000001ede30, is taken when s2(=one of 0x0, 0x1) == 0, and falls through when s2(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc0000000001eda14 targets 0xc0000000001edad0, is taken when s3(=one of 0x0, 0x1) == 0, and falls through when s3(=one of 0x0, 0x1) != 0.
  The `bne` branch at 0xc0000000001eda30 targets 0xc0000000001eda74, is taken when t8(=[0xc00000000159b690]:8) != s0(=a0), and falls through when t8(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001eda44 targets 0xc0000000001eda74, is taken when a2(=[0xa800000000001000]:8) != a3(=0x53435241), and falls through when a2(=[0xa800000000001000]:8) == a3(=0x53435241).
  The `beq` branch at 0xc0000000001eda50 targets 0xc0000000001eda74, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001eda60 targets 0xc0000000001eda74, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001eda6c targets 0xc0000000001ede64, is taken when a6(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc0000000001eda7c targets 0xc0000000001edad0, is taken when a7(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a7(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001eda90 targets 0xc0000000001edad0, is taken when t0(=[0xa800000000001000]:8) != t1(=0x53435241), and falls through when t0(=[0xa800000000001000]:8) == t1(=0x53435241).
  The `beq` branch at 0xc0000000001eda9c targets 0xc0000000001edad0, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001edaac targets 0xc0000000001edad0, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001edab8 targets 0xc0000000001edad0, is taken when a2(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000001edae0 targets 0xc0000000001edbe0, is taken when s5(?) == s1(=[a0+156]), and falls through when s5(?) != s1(=[a0+156]).
  The `blez` branch at 0xc0000000001edae8 targets 0xc0000000001ede08, is taken when s6(=a2) <= 0 (signed), and falls through when s6(=a2) > 0 (signed).
  The `beq` branch at 0xc0000000001edb10 targets 0xc0000000001eddac, is taken when (s4(=0xffffffffc0c0c0c0) & t8(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & t8(?)) != 0.
  The `blez` branch at 0xc0000000001edb18 targets 0xc0000000001edd40, is taken when s6(?) <= 0 (signed), and falls through when s6(?) > 0 (signed).
  The `beq` branch at 0xc0000000001edb30 targets 0xc0000000001edb9c, is taken when (a0(?) & 0x40) == 0, and falls through when (a0(?) & 0x40) != 0.
  The `beq` branch at 0xc0000000001edb40 targets 0xc0000000001edb60, is taken when (a2(=[a0+140]) & 0x40) == 0, and falls through when (a2(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xc0000000001edb50 targets 0xc0000000001edb60, is taken when (v0(=[a0+120]) & 0x2) == 0, and falls through when (v0(=[a0+120]) & 0x2) != 0.
  The `beq` branch at 0xc0000000001edb58 targets 0xc0000000001ede10, is taken when a6(?) == 0, and falls through when a6(?) != 0.
  The `beq` branch at 0xc0000000001edb60 targets 0xc0000000001edb94, is taken when a5(?) == 0, and falls through when a5(?) != 0.
  The `beq` branch at 0xc0000000001edb68 targets 0xc0000000001edb94, is taken when a7(?) == 0, and falls through when a7(?) != 0.
  The `beq` branch at 0xc0000000001edb74 targets 0xc0000000001edb90, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc0000000001edba0 targets 0xc0000000001edd28, is taken when (a0(?) & 0x80) == 0, and falls through when (a0(?) & 0x80) != 0.
  The `beq` branch at 0xc0000000001edba8 targets 0xc0000000001edd00, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc0000000001edbb8 targets 0xc0000000001edd00, is taken when (a2(=[a0+140]) & 0x1c) == 0, and falls through when (a2(=[a0+140]) & 0x1c) != 0.
  The `blez` branch at 0xc0000000001edbc0 targets 0xc0000000001edc50, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xc0000000001edbd8 targets 0xc0000000001edbe4 and is always taken.
  The `beq` branch at 0xc0000000001edbf4 targets 0xc0000000001edc1c, is taken when (v0(=[a0+120]) & 0x1) == 0, and falls through when (v0(=[a0+120]) & 0x1) != 0.
  The `bne` branch at 0xc0000000001edc14 targets 0xc0000000001edd8c, is taken when a4(?) < a5(?) (unsigned), and falls through when a4(?) >= a5(?) (unsigned).
  The `beq` branch at 0xc0000000001edc50 targets 0xc0000000001edc88, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc0000000001edc5c targets 0xc0000000001edc88, is taken when (a2(=[a0+140]) & 0x8) == 0, and falls through when (a2(=[a0+140]) & 0x8) != 0.
  The `beq` branch at 0xc0000000001edc68 targets 0xc0000000001edc88, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc0000000001edc88 targets 0xc0000000001edcc4, is taken when t0(?) == 0, and falls through when t0(?) != 0.
  The `beq` branch at 0xc0000000001edc98 targets 0xc0000000001edcc4, is taken when (t1(=[a0+140]) & 0x10) == 0, and falls through when (t1(=[a0+140]) & 0x10) != 0.
  The `beq` branch at 0xc0000000001edca4 targets 0xc0000000001edcc4, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc0000000001edcc4 targets 0xc0000000001edd00, is taken when t3(?) == 0, and falls through when t3(?) != 0.
  The `beq` branch at 0xc0000000001edcd4 targets 0xc0000000001edd00, is taken when (t8(=[a0+140]) & 0x4) == 0, and falls through when (t8(=[a0+140]) & 0x4) != 0.
  The `beq` branch at 0xc0000000001edce0 targets 0xc0000000001edd00, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc0000000001edd30 targets 0xc0000000001edd40, is taken when s8(?) >= 0x4 (signed), and falls through when s8(?) < 0x4 (signed).
  The `bgtz` branch at 0xc0000000001edd38 targets 0xc0000000001edb24, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xc0000000001edd48 targets 0xc0000000001ede00, is taken when (s4(=0xffffffffc0c0c0c0) & a0(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a0(?)) != 0.
  The `beq` branch at 0xc0000000001edd50 targets 0xc0000000001edbe4, is taken when s5(?) == s1(?), and falls through when s5(?) != s1(?).
  The `bgtz` branch at 0xc0000000001edd58 targets 0xc0000000001edb00, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xc0000000001edd60 targets 0xc0000000001edbe8 and is always taken.
  The `beq` branch at 0xc0000000001edd74 targets 0xc0000000001ed9ac, is taken when (a1(=[[a0+64]:8]) & s2(=0x8000000)) == 0, and falls through when (a1(=[[a0+64]:8]) & s2(=0x8000000)) != 0.
  The `beq` branch at 0xc0000000001edd7c targets 0xc0000000001edd68, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0xc0000000001edd84 targets 0xc0000000001ed9b0 and is always taken.
  The `beq` branch at 0xc0000000001edda4 targets 0xc0000000001edc1c and is always taken.
  The `beq` branch at 0xc0000000001eddc8 targets 0xc0000000001edde0, is taken when v0(?) == a0(?), and falls through when v0(?) != a0(?).
  The `bne` branch at 0xc0000000001eddd8 targets 0xc0000000001edb18, is taken when (s4(=0xffffffffc0c0c0c0) & a3(?)) != 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a3(?)) == 0.
  The `beq` branch at 0xc0000000001edde8 targets 0xc0000000001edbe8 and is always taken.
  The `beq` branch at 0xc0000000001eddf8 targets 0xc0000000001ed890 and is always taken.
  The `beq` branch at 0xc0000000001ede00 targets 0xc0000000001edd50 and is always taken.
  The `beq` branch at 0xc0000000001ede08 targets 0xc0000000001edbe4 and is always taken.
  The `blez` branch at 0xc0000000001ede10 targets 0xc0000000001edebc, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xc0000000001ede28 targets 0xc0000000001edbe4 and is always taken.
  The `beq` branch at 0xc0000000001ede30 targets 0xc0000000001edc1c and is always taken.
  The `beq` branch at 0xc0000000001ede5c targets 0xc0000000001ed8d8 and is always taken.
  The `beq` branch at 0xc0000000001ede88 targets 0xc0000000001eda74 and is always taken.
  The `beq` branch at 0xc0000000001edeb4 targets 0xc0000000001ed990 and is always taken.
  The `beq` branch at 0xc0000000001ededc targets 0xc0000000001edf20, is taken when (s4(=0xffffffffc0c0c0c0) & t1(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & t1(?)) != 0.
  The `beq` branch at 0xc0000000001edef4 targets 0xc0000000001edf18, is taken when (t9(=[a0+140]) & 0x40) == 0, and falls through when (t9(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xc0000000001edf00 targets 0xc0000000001edf18, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc0000000001edf18 targets 0xc0000000001edc1c and is always taken.
  The `beq` branch at 0xc0000000001edf24 targets 0xc0000000001edee4 and is always taken.
  The `beq` branch at 0xc0000000001edf34 targets 0xc0000000001edd7c and is always taken.
  The return at 0xc0000000001edc48 has v0 = one of ?, -0x1, 0x0.
- `ioc3_set_DTR` in `6.5.22/unix.IP35` at 0xc0000000001ee4c8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ee4c8-0xc0000000001ee4e8 (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xc0000000001ee4d4 calls set_DTRRTS [0xc0000000001ee2f4] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x1, a3=0x1000000.
  The return at 0xc0000000001ee4e0 has v0 = ret(set_DTRRTS).
- `ioc3_set_RTS` in `6.5.22/unix.IP35` at 0xc0000000001ee4e8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ee4e8-0xc0000000001ee508 (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xc0000000001ee4f4 calls set_DTRRTS [0xc0000000001ee2f4] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x2, a3=0x2000000.
  The return at 0xc0000000001ee500 has v0 = ret(set_DTRRTS).
- `ioc3_query_DCD` in `6.5.22/unix.IP35` at 0xc0000000001ee508 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ee508-0xc0000000001ee60c (260 bytes).
  It occupies 260 bytes.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000001ee524, `ld` at 0xc0000000001ee574.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000001ee528, `ld` at 0xc0000000001ee57c, `ld` at 0xc0000000001ee5e0.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000001ee53c, `lw` at 0xc0000000001ee588.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000001ee548, `ld` at 0xc0000000001ee594, `ld` at 0xc0000000001ee5e4.
  The `ld` at 0xc0000000001ee558 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xc0000000001ee56c loads 4 bytes from [a0+64]:8 +0x18.
  The `jalr` at 0xc0000000001ee5ac calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=[[0xa800000000001020]:8+128]:8, a4=[[a0+64]:8+24], a5=0xa800000000001020, a6=[0xa800000000001000]:8, a7=[[0xa800000000001020]:8].
  The `jal` at 0xc0000000001ee5b4 calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001ee5d4 calls spl7 [0xc00000000006287c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=[0xc00000000159b690]:8, a4=0x53435241, a5=[[0xa800000000001020]:8], a6=0xfffffffffeeddead, a7=[[0xa800000000001020]:8+128]:8.
  The `jalr` at 0xc0000000001ee5ec calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc0000000001ee520 targets 0xc0000000001ee558, is taken when a0 != a3(=[0xc00000000159b690]:8), and falls through when a0 == a3(=[0xc00000000159b690]:8).
  The `bne` branch at 0xc0000000001ee52c targets 0xc0000000001ee558, is taken when a5(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc0000000001ee534 targets 0xc0000000001ee558, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ee540 targets 0xc0000000001ee558, is taken when a5(=[[0xa800000000001020]:8]) != a6(=0xfffffffffeeddead), and falls through when a5(=[[0xa800000000001020]:8]) == a6(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001ee550 targets 0xc0000000001ee5d4, is taken when a7(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a7(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc0000000001ee568 targets 0xc0000000001ee5c4, is taken when a0 != a3(=[0xc00000000159b690]:8), and falls through when a0 == a3(=[0xc00000000159b690]:8).
  The `bne` branch at 0xc0000000001ee578 targets 0xc0000000001ee5c4, is taken when a6(=[0xa800000000001000]:8) != a7(=0x53435241), and falls through when a6(=[0xa800000000001000]:8) == a7(=0x53435241).
  The `beq` branch at 0xc0000000001ee580 targets 0xc0000000001ee5c4, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ee58c targets 0xc0000000001ee5c4, is taken when a7(=[[0xa800000000001020]:8]) != t0(=0xfffffffffeeddead), and falls through when a7(=[[0xa800000000001020]:8]) == t0(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001ee59c targets 0xc0000000001ee5c4, is taken when a3(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000001ee604 targets 0xc0000000001ee558 and is always taken.
  The return at 0xc0000000001ee5cc has v0 = ?.
- `ioc3_query_CTS` in `6.5.22/unix.IP35` at 0xc0000000001ee60c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ee60c-0xc0000000001ee710 (260 bytes).
  It occupies 260 bytes.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000001ee628, `ld` at 0xc0000000001ee678.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000001ee62c, `ld` at 0xc0000000001ee680, `ld` at 0xc0000000001ee6e4.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000001ee640, `lw` at 0xc0000000001ee68c.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000001ee64c, `ld` at 0xc0000000001ee698, `ld` at 0xc0000000001ee6e8.
  The `ld` at 0xc0000000001ee65c loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xc0000000001ee670 loads 4 bytes from [a0+64]:8 +0x18.
  The `jalr` at 0xc0000000001ee6b0 calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=[[0xa800000000001020]:8+128]:8, a4=[[a0+64]:8+24], a5=0xa800000000001020, a6=[0xa800000000001000]:8, a7=[[0xa800000000001020]:8].
  The `jal` at 0xc0000000001ee6b8 calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001ee6d8 calls spl7 [0xc00000000006287c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=[0xc00000000159b690]:8, a4=0x53435241, a5=[[0xa800000000001020]:8], a6=0xfffffffffeeddead, a7=[[0xa800000000001020]:8+128]:8.
  The `jalr` at 0xc0000000001ee6f0 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc0000000001ee624 targets 0xc0000000001ee65c, is taken when a0 != a3(=[0xc00000000159b690]:8), and falls through when a0 == a3(=[0xc00000000159b690]:8).
  The `bne` branch at 0xc0000000001ee630 targets 0xc0000000001ee65c, is taken when a5(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc0000000001ee638 targets 0xc0000000001ee65c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ee644 targets 0xc0000000001ee65c, is taken when a5(=[[0xa800000000001020]:8]) != a6(=0xfffffffffeeddead), and falls through when a5(=[[0xa800000000001020]:8]) == a6(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001ee654 targets 0xc0000000001ee6d8, is taken when a7(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a7(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc0000000001ee66c targets 0xc0000000001ee6c8, is taken when a0 != a3(=[0xc00000000159b690]:8), and falls through when a0 == a3(=[0xc00000000159b690]:8).
  The `bne` branch at 0xc0000000001ee67c targets 0xc0000000001ee6c8, is taken when a6(=[0xa800000000001000]:8) != a7(=0x53435241), and falls through when a6(=[0xa800000000001000]:8) == a7(=0x53435241).
  The `beq` branch at 0xc0000000001ee684 targets 0xc0000000001ee6c8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ee690 targets 0xc0000000001ee6c8, is taken when a7(=[[0xa800000000001020]:8]) != t0(=0xfffffffffeeddead), and falls through when a7(=[[0xa800000000001020]:8]) == t0(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001ee6a0 targets 0xc0000000001ee6c8, is taken when a3(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000001ee708 targets 0xc0000000001ee65c and is always taken.
  The return at 0xc0000000001ee6d0 has v0 = ?.
- `ioc3_get_mapid` in `6.5.22/unix.IP35` at 0xc0000000001ee764 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ee764-0xc0000000001ee7c4 (96 bytes).
  It occupies 96 bytes.
  The `ld` at 0xc0000000001ee778 loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `jal` at 0xc0000000001ee79c calls copyout [0xc0000000000473ec] with a0=sp-16, a1=a1 at entry (arg1), a2=0x8, a3=0xc0000000015019a8.
  The `beq` branch at 0xc0000000001ee76c targets 0xc0000000001ee7b4, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc0000000001ee7ac targets 0xc0000000001ee7b8 and is always taken.
  The return at 0xc0000000001ee7bc has v0 = one of 0x0, 0xe.
- `ioc3_map` in `6.5.22/unix.IP35` at 0xc0000000001ee7c4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ee7c4-0xc0000000001ee854 (144 bytes).
  It occupies 144 bytes.
  The `ld` at 0xc0000000001ee7f0 loads 8 bytes from a0 (arg0 at entry) +0x58.
  The `ld` at 0xc0000000001ee824 loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `ld` at 0xc0000000001ee834 loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `jal` at 0xc0000000001ee7f8 calls disable_intrs [0xc0000000001eb898] with a0=a0 at entry (arg0), a1=-0x1, a2=?, a3=?, a5=0x16.
  The `jal` at 0xc0000000001ee808 calls v_mapphys [0xc0000000002627dc] with a0=a1 at entry (arg1), a1=?, a2=0x4000, a3=?.
  The `beq` branch at 0xc0000000001ee7cc targets 0xc0000000001ee7e8, is taken when a2 == 0, and falls through when a2 != 0.
  The `beq` branch at 0xc0000000001ee7d8 targets 0xc0000000001ee824, is taken when a2 == at(=0x4000), and falls through when a2 != at(=0x4000).
  The `beq` branch at 0xc0000000001ee7e0 targets 0xc0000000001ee81c and is always taken.
  The `beq` branch at 0xc0000000001ee84c targets 0xc0000000001ee7f8 and is always taken.
  The return at 0xc0000000001ee81c has v0 = one of 0x16, ret(v_mapphys).
- `ioc3_unmap` in `6.5.22/unix.IP35` at 0xc0000000001ee854 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ee854-0xc0000000001ee990 (316 bytes).
  It occupies 316 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc0000000001ee85c, `lw` at 0xc0000000001ee980.
  The `sw` at 0xc0000000001ee870 stores 4 bytes to a0 (arg0 at entry) +0x94.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000001ee880, `ld` at 0xc0000000001ee8e0.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000001ee888, `ld` at 0xc0000000001ee8d4, `ld` at 0xc0000000001ee968.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000001ee8a4, `lw` at 0xc0000000001ee8f4.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000001ee8b0, `ld` at 0xc0000000001ee900, `ld` at 0xc0000000001ee96c.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc0000000001ee8bc, `ld` at 0xc0000000001ee928, `ld` at 0xc0000000001ee93c.
  The `sw` at 0xc0000000001ee8c4 stores 4 bytes to [a0+64]:8 +0x0.
  The `lw` at 0xc0000000001ee92c loads 4 bytes from [a0+64]:8 +0x8.
  The `sw` at 0xc0000000001ee934 stores 4 bytes to a0 (arg0 at entry) +0x98.
  The `sw` at 0xc0000000001ee938 stores 4 bytes to [a0+64]:8 +0x4.
  The `lw` at 0xc0000000001ee940 loads 4 bytes from [a0+64]:8 +0xc.
  The `sw` at 0xc0000000001ee948 stores 4 bytes to a0 (arg0 at entry) +0x9c.
  The `sw` at 0xc0000000001ee94c stores 4 bytes to [a0+64]:8 +0x10.
  The `jalr` at 0xc0000000001ee914 calls (unknown callee) with a0=0x1, a1=[a0+64]:8, a2=[[0xa800000000001020]:8], a3=[[0xa800000000001020]:8+128]:8, a4=[0xc00000000159b68c], a5=0xa800000000001020, a6=[0xc00000000159b690]:8.
  The `jal` at 0xc0000000001ee91c calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001ee95c calls spl7 [0xc00000000006287c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=?, a4=[0xa800000000001000]:8, a5=0x53435241, a6=[[0xa800000000001020]:8], a7=0xfffffffffeeddead.
  The `jalr` at 0xc0000000001ee974 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+128]:8.
  The `bne` branch at 0xc0000000001ee88c targets 0xc0000000001ee8bc, is taken when at(=[0xc00000000159b690]:8) != a0, and falls through when at(=[0xc00000000159b690]:8) == a0.
  The `bne` branch at 0xc0000000001ee894 targets 0xc0000000001ee8bc, is taken when a4(=[0xa800000000001000]:8) != a5(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a5(=0x53435241).
  The `beq` branch at 0xc0000000001ee89c targets 0xc0000000001ee8bc, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ee8a8 targets 0xc0000000001ee8bc, is taken when a6(=[[0xa800000000001020]:8]) != a7(=0xfffffffffeeddead), and falls through when a6(=[[0xa800000000001020]:8]) == a7(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001ee8b4 targets 0xc0000000001ee95c, is taken when t0(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when t0(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc0000000001ee8dc targets 0xc0000000001ee928, is taken when a6(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a6(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ee8e4 targets 0xc0000000001ee928, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000001ee8ec targets 0xc0000000001ee928, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ee8f8 targets 0xc0000000001ee928, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001ee904 targets 0xc0000000001ee928, is taken when a3(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000001ee988 targets 0xc0000000001ee8bc and is always taken.
  The return at 0xc0000000001ee954 has v0 = one of [0xa800000000001020]:8, ret(splx).
- `ioc3_set_sscr` in `6.5.22/unix.IP35` at 0xc0000000001ee990 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001ee990-0xc0000000001eeab4 (292 bytes).
  It occupies 292 bytes.
  3 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc0000000001ee990, `lw` at 0xc0000000001eea64, `lw` at 0xc0000000001eeaa4.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xc0000000001ee9ac, `sw` at 0xc0000000001eea7c.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000001ee9c8, `ld` at 0xc0000000001eea1c.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000001ee9cc, `ld` at 0xc0000000001eea10, `ld` at 0xc0000000001eea8c.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000001ee9e0, `lw` at 0xc0000000001eea30.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000001ee9ec, `ld` at 0xc0000000001eea3c, `ld` at 0xc0000000001eea90.
  The `ld` at 0xc0000000001ee9f8 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `sw` at 0xc0000000001eea00 stores 4 bytes to [a0+64]:8 +0x0.
  The `jalr` at 0xc0000000001eea50 calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8+128]:8, a2=[0xc00000000159b690]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[0xc00000000159b68c].
  The `jal` at 0xc0000000001eea58 calls splx [0xc0000000000628b4] with a0=[0xc00000000159b68c], a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001eea80 calls spl7 [0xc00000000006287c] with a0=0x53435241, a1=[[0xa800000000001020]:8], a2=0xfffffffffeeddead, a3=[[0xa800000000001020]:8+128]:8, a4=0xa800000000001020.
  The `jalr` at 0xc0000000001eea98 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a6=[[0xa800000000001020]:8+128]:8.
  The `beq` branch at 0xc0000000001ee9a0 targets 0xc0000000001eea74, is taken when a2 == 0, and falls through when a2 != 0.
  The `bne` branch at 0xc0000000001ee9c4 targets 0xc0000000001ee9f8, is taken when a2(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a2(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001ee9d0 targets 0xc0000000001ee9f8, is taken when a1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000001ee9d8 targets 0xc0000000001ee9f8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001ee9e4 targets 0xc0000000001ee9f8, is taken when a1(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a1(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001ee9f0 targets 0xc0000000001eea80, is taken when a3(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a3(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc0000000001eea18 targets 0xc0000000001eea64, is taken when a2(=[0xc00000000159b690]:8) != s0(=a0), and falls through when a2(=[0xc00000000159b690]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001eea20 targets 0xc0000000001eea64, is taken when a0(=[0xa800000000001000]:8) != a1(=0x53435241), and falls through when a0(=[0xa800000000001000]:8) == a1(=0x53435241).
  The `beq` branch at 0xc0000000001eea28 targets 0xc0000000001eea64, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001eea34 targets 0xc0000000001eea64, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001eea40 targets 0xc0000000001eea64, is taken when a1(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a1(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000001eea78 targets 0xc0000000001ee9b0 and is always taken.
  The `beq` branch at 0xc0000000001eeaac targets 0xc0000000001ee9f8 and is always taken.
  The return at 0xc0000000001eea6c has v0 = [a0+148].
- `ioc3_console_write` in `6.5.22/unix.IP35` at 0xc0000000001eeab4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000001eeab4-0xc0000000001eec08 (340 bytes).
  It occupies 340 bytes.
  7 such `lw` accesses load 4 bytes from [0xc0000000015930a0]:8 +0xb8, including `lw` at 0xc0000000001eeab8, `lw` at 0xc0000000001eeb94, `lw` at 0xc0000000001eeba4.
  The `sd` at 0xc0000000001eeae0 stores 8 bytes to a2 (arg2 at entry) +0x0.
  The `sb` at 0xc0000000001eeb08 stores 1 byte to [0xc0000000015930a0]:8 +0x20178.
  6 such `lbu` accesses load 1 byte from [0xc0000000015930a0]:8 +0x2017d, including `lbu` at 0xc0000000001eeb0c, `lbu` at 0xc0000000001eeb24, `lbu` at 0xc0000000001eeb34.
  The `lbu` at 0xc0000000001eeb18 loads 1 byte from ?.
  The `sw` at 0xc0000000001eeb90 stores 4 bytes to [0xc0000000015930a0]:8 +0xb8.
  The `bne` branch at 0xc0000000001eeac8 targets 0xc0000000001eeae0, is taken when t0(=0x10000000) != t2(?), and falls through when t0(=0x10000000) == t2(?).
  The `beq` branch at 0xc0000000001eead8 targets 0xc0000000001eebfc and is always taken.
  The `bne` branch at 0xc0000000001eeaf4 targets 0xc0000000001eeb0c, is taken when a1 != 0, and falls through when a1 == 0.
  The `beq` branch at 0xc0000000001eeafc targets 0xc0000000001eeb7c and is always taken.
  The `beq` branch at 0xc0000000001eeb04 targets 0xc0000000001eeb7c, is taken when a1(?) == 0, and falls through when a1(?) != 0.
  The `bne` branch at 0xc0000000001eeb1c targets 0xc0000000001eeb04, is taken when (a6(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) != 0, and falls through when (a6(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc0000000001eeb2c targets 0xc0000000001eeb04, is taken when (a7(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) != 0, and falls through when (a7(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc0000000001eeb3c targets 0xc0000000001eeb04, is taken when (t0(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) != 0, and falls through when (t0(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc0000000001eeb4c targets 0xc0000000001eeb04, is taken when (t1(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) != 0, and falls through when (t1(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc0000000001eeb5c targets 0xc0000000001eeb04, is taken when (t2(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) != 0, and falls through when (t2(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) == 0.
  The `beq` branch at 0xc0000000001eeb6c targets 0xc0000000001eeb24, is taken when (t3(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) == 0, and falls through when (t3(=[[0xc0000000015930a0]:8+131453]:1) & 0x20) != 0.
  The `beq` branch at 0xc0000000001eeb74 targets 0xc0000000001eeb04 and is always taken.
  The `bne` branch at 0xc0000000001eeb88 targets 0xc0000000001eeae0, is taken when t0(=0x10000000) != t8(?), and falls through when t0(=0x10000000) == t8(?).
  The `bne` branch at 0xc0000000001eeb9c targets 0xc0000000001eeb88, is taken when a5(=0x30000000) != t9(?), and falls through when a5(=0x30000000) == t9(?).
  The `bne` branch at 0xc0000000001eebac targets 0xc0000000001eeb84, is taken when a5(=0x30000000) != v0(?), and falls through when a5(=0x30000000) == v0(?).
  The `bne` branch at 0xc0000000001eebbc targets 0xc0000000001eeb84, is taken when a5(=0x30000000) != v1(?), and falls through when a5(=0x30000000) == v1(?).
  The `bne` branch at 0xc0000000001eebcc targets 0xc0000000001eeb84, is taken when a5(=0x30000000) != a3(?), and falls through when a5(=0x30000000) == a3(?).
  The `bne` branch at 0xc0000000001eebdc targets 0xc0000000001eeb84, is taken when a5(=0x30000000) != t3(?), and falls through when a5(=0x30000000) == t3(?).
  The `beq` branch at 0xc0000000001eebec targets 0xc0000000001eeba4, is taken when a5(=0x30000000) == t8(?), and falls through when a5(=0x30000000) != t8(?).
  The `beq` branch at 0xc0000000001eebf4 targets 0xc0000000001eeb84 and is always taken.
  The `beq` branch at 0xc0000000001eec00 targets 0xc0000000001eeb90 and is always taken.
  The return at 0xc0000000001eeb7c has v0 = 0x0.
- `idbg_gdb` in `6.5.22/unix.IP35` at 0xc000000000210e18 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000210e18-0xc000000000210e4c (52 bytes).
  It occupies 52 bytes.
  The `jal` at 0xc000000000210e28 calls dogang [0xc000000000210a24] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc000000000210e3c calls idbg_gang [0xc000000000210cc4] with a0=-0x1, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `beq` branch at 0xc000000000210e20 targets 0xc000000000210e3c, is taken when a0 == at(=-0x1), and falls through when a0 != at(=-0x1).
  The `beq` branch at 0xc000000000210e44 targets 0xc000000000210e34 and is always taken.
  The return at 0xc000000000210e34 has v0 = one of ret(dogang), ret(idbg_gang).
- `panicregs` in `6.5.22/unix.IP35` at 0xc00000000021bdcc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc00000000021bdcc-0xc00000000021be80 (180 bytes).
  It occupies 180 bytes.
  The `ld` at 0xc00000000021be1c loads 8 bytes from a0 (arg0 at entry) +0x120.
  The `ld` at 0xc00000000021be48 loads 8 bytes from a0 (arg0 at entry) +0x110.
  The `ld` at 0xc00000000021be4c loads 8 bytes from table(0xc000000001501f10) +0x0.
  The `jal` at 0xc00000000021bde8 calls spl1 [0xc0000000000627cc] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc00000000021bdf0 calls set_leds [0xc00000000007d2ac] with a0=0x3, a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000021be10 calls machine_error_dump [0xc000000000272f90] with a0=0xc000000000471bf0, a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000021be68 calls cmn_err [0xc00000000026ef18] with a0=0x0, a1=0xc000000000471c00, a2=a4 at entry (arg4), a3=[a0+288]:8, a4=a0 at entry (arg0), a5=a1 at entry (arg1), a6=entry(0xc000000001501f10), a7=[a0+272]:8.
  The `bne` branch at 0xc00000000021be08 targets 0xc00000000021be18, is taken when v0(=[0xc000000001593820]) != 0, and falls through when v0(=[0xc000000001593820]) == 0.
  The return at 0xc00000000021be78 has v0 = ret(cmn_err).
- `mem_range_init` in `6.5.22/unix.IP35` at 0xc000000000224148 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000224148-0xc0000000002244bc (884 bytes).
  It occupies 884 bytes.
  6 such `ld` accesses load 8 bytes from abs 0xffffffffffffc400, including `ld` at 0xc0000000002241d8, `ld` at 0xc000000000224204, `ld` at 0xc000000000224224.
  5 such `ld` accesses load 8 bytes from [-0x3c00]:8 +0x0, including `ld` at 0xc0000000002241dc, `ld` at 0xc000000000224208, `ld` at 0xc000000000224228.
  3 such `ld` accesses load 8 bytes from ((a0 << 0x3) + [[-0x3c00]:8]:8) +0x0, including `ld` at 0xc0000000002241e8, `ld` at 0xc000000000224210, `ld` at 0xc000000000224230.
  The `lw` at 0xc0000000002241f0 loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x5f4.
  The `sw` at 0xc000000000224200 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x5f4.
  The `lw` at 0xc000000000224214 loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x604.
  The `sw` at 0xc000000000224220 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x604.
  The `lw` at 0xc000000000224234 loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x680.
  The `sw` at 0xc000000000224240 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x680.
  2 such `ld` accesses load 8 bytes from ([[-0x3c00]:8]:8 + (a0 << 0x3)) +0x0, including `ld` at 0xc000000000224250, `ld` at 0xc000000000224324.
  The `ll` at 0xc00000000022425c loads 4 bytes from [([[-0x3c00]:8]:8 + (a0 << 0x3))]:8 +0x5fc.
  The `sc` at 0xc000000000224268 stores 4 bytes to [([[-0x3c00]:8]:8 + (a0 << 0x3))]:8 +0x5fc.
  The `ld` at 0xc000000000224280 loads 8 bytes from abs 0xc000000001606fa0 (Nodepdaindr).
  3 such `lw` accesses load 4 bytes from ?, including `lw` at 0xc000000000224294, `lw` at 0xc000000000224314, `lw` at 0xc0000000002243d0.
  The `ld` at 0xc00000000022429c loads 8 bytes from [0xc000000001606fa0]:8 +0x5e8.
  4 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc0000000002242a8, `sw` at 0xc0000000002242b0, `sw` at 0xc0000000002243bc.
  The `ld` at 0xc0000000002242b8 loads 8 bytes from ([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  The `sd` at 0xc0000000002242ec stores 8 bytes to ([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  3 such `ld` accesses load 8 bytes from ?, including `ld` at 0xc000000000224330, `ld` at 0xc00000000022437c, `ld` at 0xc000000000224448.
  9 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc000000000224344, `sd` at 0xc000000000224380, `sd` at 0xc000000000224384.
  The `lw` at 0xc000000000224358 loads 4 bytes from [-0x3c00]:8 +0x61c.
  3 such `ld` accesses load 8 bytes from ([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0, including `ld` at 0xc000000000224360, `ld` at 0xc000000000224428, `ld` at 0xc000000000224478.
  2 such `lhu` accesses load 2 bytes from ?, including `lhu` at 0xc000000000224378, `lhu` at 0xc000000000224434.
  The `ld` at 0xc0000000002243f4 loads 8 bytes from table(0xc0000000015ffa58) +0x0.
  The `ld` at 0xc000000000224408 loads 8 bytes from table(0xc0000000015fda58) +0x0.
  2 such `ld` accesses load 8 bytes from ([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x48, including `ld` at 0xc00000000022440c, `ld` at 0xc000000000224460.
  The `sd` at 0xc000000000224490 stores 8 bytes to ([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x80.
  The `jal` at 0xc00000000022416c calls is_specified [0xc000000000286070] with a0=0xc00000000159c1fb, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000002241d0 calls setupbadmem [0xc00000000004f7f4] with a0=?, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=?.
  The `jal` at 0xc0000000002243f0 calls bset [0xc0000000002df9d8] with a0=entry(0xc0000000015ffa58), a1=?, a2=?, a3=ret(is_specified).
  The `jal` at 0xc000000000224404 calls bset [0xc0000000002df9d8] with a0=entry(0xc0000000015fda58), a1=?, a2=?, a3=?.
  The `beql` branch at 0xc00000000022426c targets 0xc00000000022425c, is taken when t9(?) == 0, and falls through when t9(?) != 0; its branch-likely delay slot `nop` at 0xc000000000224270 runs only when taken and is annulled on fall-through.
  The `bne` branch at 0xc0000000002242d0 targets 0xc000000000224348, is taken when t8(=[([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) != 0, and falls through when t8(=[([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) == 0.
  The `bne` branch at 0xc000000000224340 targets 0xc0000000002242f8, is taken when a1(?) < 0x6 (signed), and falls through when a1(?) >= 0x6 (signed).
  The `bltz` branch at 0xc00000000022436c targets 0xc000000000224484, is taken when s3(?) < 0 (signed), and falls through when s3(?) >= 0 (signed).
  The `beq` branch at 0xc000000000224374 targets 0xc000000000224438 and is always taken.
  The `bne` branch at 0xc000000000224420 targets 0xc00000000022442c, is taken when a1(=[([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s0(?), and falls through when a1(=[([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s0(?).
  The `bltz` branch at 0xc00000000022442c targets 0xc000000000224484, is taken when a0(?) < 0 (signed), and falls through when a0(?) >= 0 (signed).
  The `beq` branch at 0xc000000000224438 targets 0xc000000000224460, is taken when v0(=0xffff) == a2(?), and falls through when v0(=0xffff) != a2(?).
  The `bne` branch at 0xc000000000224440 targets 0xc00000000022437c, is taken when a3(=ret(is_specified)) != 0, and falls through when a3(=ret(is_specified)) == 0.
  The `beq` branch at 0xc000000000224458 targets 0xc000000000224390 and is always taken.
  The `bne` branch at 0xc000000000224470 targets 0xc00000000022447c, is taken when a5(=[([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s0(?), and falls through when a5(=[([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s0(?).
  The `bgez` branch at 0xc00000000022447c targets 0xc000000000224434, is taken when a0(?) >= 0 (signed), and falls through when a0(?) < 0 (signed).
  The return at 0xc0000000002244b4 has v0 = 0xffff.
- `idbg_init` in `6.5.22/unix.IP35` at 0xc0000000002455f4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000002455f4-0xc000000000245668 (116 bytes).
  It occupies 116 bytes.
  2 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc00000000024561c, `sw` at 0xc00000000024563c.
  The `jal` at 0xc000000000245650 calls init_mutex [0xc000000000202fb4] with a0=0xc0000000016028f8, a1=0x0, a2=0xc000000000474600, a3=-0x1, a5=0xc000000001601dc8.
  The `beq` branch at 0xc000000000245620 targets 0xc000000000245644, is taken when a2(?) >= 0x14 (signed), and falls through when a2(?) < 0x14 (signed).
  The `bne` branch at 0xc000000000245638 targets 0xc00000000024560c, is taken when a2(?) < 0x14 (signed), and falls through when a2(?) >= 0x14 (signed).
  The return at 0xc000000000245660 has v0 = 0x1.
- `idbg_id` in `6.5.22/unix.IP35` at 0xc000000000245668 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000245668-0xc000000000245670 (8 bytes).
  It occupies 8 bytes.
  The `lw` at 0xc00000000024566c loads 4 bytes from a0 (arg0 at entry) +0x3c.
  The return at 0xc000000000245668 has v0 = [a0+60].
- `idbg_lock` in `6.5.22/unix.IP35` at 0xc000000000245670 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000245670-0xc000000000245678 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000245670 has v0 = 0x0.
- `idbg_unlock` in `6.5.22/unix.IP35` at 0xc000000000245678 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000245678-0xc000000000245680 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000245678 has v0 = ?.
- `idbg_copydata` in `6.5.22/unix.IP35` at 0xc000000000245680 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000245680-0xc000000000245688 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000245680 has v0 = ?.
- `idbg_makedata` in `6.5.22/unix.IP35` at 0xc000000000245688 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000245688-0xc000000000245700 (120 bytes).
  It occupies 120 bytes.
  The `ld` at 0xc0000000002456b0 loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `sd` at 0xc0000000002456f0 stores 8 bytes to ret(kern_calloc) +0x8.
  The `sd` at 0xc0000000002456f4 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xc0000000002456a0 calls kern_calloc [0xc0000000001f7100] with a0=0x1, a1=0x18, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000002456b4 calls copyin [0xc000000000047360] with a0=[a0+24]:8, a1=ret(kern_calloc), a2=0x4, a3=?.
  The `jal` at 0xc0000000002456c4 calls kern_free [0xc0000000001f7218] with a0=ret(kern_calloc), a1=0xa0, a2=?, a3=?.
  The `jal` at 0xc0000000002456e8 calls kern_calloc [0xc0000000001f7100] with a0=0x1, a1=0xa0, a2=?, a3=?.
  The `beq` branch at 0xc0000000002456bc targets 0xc0000000002456e8, is taken when v0(=ret(copyin)) == 0, and falls through when v0(=ret(copyin)) != 0.
  The `beq` branch at 0xc0000000002456f8 targets 0xc0000000002456d0 and is always taken.
  The return at 0xc0000000002456e0 has v0 = one of 0x0, 0xa.
- `idbg_freedata` in `6.5.22/unix.IP35` at 0xc000000000245700 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000245700-0xc000000000245748 (72 bytes).
  It occupies 72 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x18, including `ld` at 0xc000000000245700, `ld` at 0xc000000000245728.
  The `ld` at 0xc000000000245714 loads 8 bytes from [a0+24]:8 +0x8.
  The `sd` at 0xc000000000245738 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xc000000000245720 calls kern_free [0xc0000000001f7218] with a0=[[a0+24]:8+8]:8, a1=a1 at entry (arg1), a2=[a0+24]:8, a3=a3 at entry (arg3).
  The `jal` at 0xc00000000024572c calls kern_free [0xc0000000001f7218] with a0=[a0+24]:8, a1=?, a2=[a0+24]:8, a3=?.
  The `beq` branch at 0xc00000000024570c targets 0xc00000000024573c, is taken when a2(=[a0+24]:8) == 0, and falls through when a2(=[a0+24]:8) != 0.
  The `beq` branch at 0xc000000000245718 targets 0xc00000000024572c, is taken when a0(=[[a0+24]:8+8]:8) == 0, and falls through when a0(=[[a0+24]:8+8]:8) != 0.
  The return at 0xc000000000245740 has v0 = one of ?, ret(kern_free).
- `idbg_load` in `6.5.22/unix.IP35` at 0xc000000000245b80 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000245b80-0xc000000000245c24 (164 bytes).
  It occupies 164 bytes.
  The `ld` at 0xc000000000245bc4 loads 8 bytes from a0 (arg0 at entry) +0x28.
  The `ld` at 0xc000000000245bcc loads 8 bytes from [a0+40]:8 +0x50.
  The `jal` at 0xc000000000245b9c calls mutex_lock [0xc000000000203144] with a0=0xc0000000016028f8, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc000000000245ba4 calls debug_check [0xc000000000245a60] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000245bbc calls debug_freeid [0xc000000000245a20] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000245be8 calls mutex_unlock [0xc00000000020307c] with a0=0xc0000000016028f8, a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000245c04 calls mloadfile [0xc000000000246b20] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000245c14 calls debug_init [0xc000000000245748] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xc000000000245bac targets 0xc000000000245c04, is taken when v0(=ret(debug_check)) == 0, and falls through when v0(=ret(debug_check)) != 0.
  The `beq` branch at 0xc000000000245bb4 targets 0xc000000000245be4, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `bne` branch at 0xc000000000245c0c targets 0xc000000000245bb4, is taken when v0(=ret(mloadfile)) != 0, and falls through when v0(=ret(mloadfile)) == 0.
  The `beq` branch at 0xc000000000245c1c targets 0xc000000000245bb4 and is always taken.
  The return at 0xc000000000245bfc has v0 = ?.
- `idbg_unload` in `6.5.22/unix.IP35` at 0xc000000000245c24 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000245c24-0xc000000000245d88 (356 bytes).
  It occupies 356 bytes.
  The `ld` at 0xc000000000245c3c loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `ld` at 0xc000000000245c54 loads 8 bytes from a0 (arg0 at entry) +0x50.
  The `ld` at 0xc000000000245c80 loads 8 bytes from [a0+24]:8 +0x8.
  The `ld` at 0xc000000000245c84 loads 8 bytes from [[a0+24]:8+8]:8 +0x68.
  The `sw` at 0xc000000000245cec stores 4 bytes to abs 0xc0000000016028a8.
  The `ld` at 0xc000000000245cf4 loads 8 bytes from [a0+24]:8 +0x10.
  The `ld` at 0xc000000000245d5c loads 8 bytes from [0xc00000000159bb60]:8 +0x28.
  The `lw` at 0xc000000000245d60 loads 4 bytes from [[0xc00000000159bb60]:8+40]:8 +0x48.
  The `sw` at 0xc000000000245d6c stores 4 bytes to [[0xc00000000159bb60]:8+40]:8 +0x48.
  The `ld` at 0xc000000000245d70 loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `sw` at 0xc000000000245d84 stores 4 bytes to table(0xc0000000016028a8) +0x0.
  The `jal` at 0xc000000000245c44 calls mutex_lock [0xc000000000203144] with a0=0xc0000000016028f8, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jalr` at 0xc000000000245ccc calls (unknown callee) with a0=?, a1=?, a2=?, a3=0xc0000fbe00000000, a4=-0x1, a5=-0x1, a7=0xffffffffe00007df.
  The `jal` at 0xc000000000245cdc calls mutex_unlock [0xc00000000020307c] with a0=0xc0000000016028f8, a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000245cf8 calls symtab_rele [0xc0000000002465f0] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000245d04 calls mutex_unlock [0xc00000000020307c] with a0=0xc0000000016028f8, a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000245d30 calls mfindname [0xc0000000002470f0] with a0=[a0+80]:8, a1=0xc000000000474310, a2=sp-32, a3=0xc0000fbe00000000, a4=-0x1, a5=-0x1, a6=0xffffffff00000000, a7=0xffffffffe00007df.
  The `jal` at 0xc000000000245d40 calls mutex_unlock [0xc00000000020307c] with a0=0xc0000000016028f8, a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000245d50 calls (unknown callee) with a0=?, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc000000000245c78 targets 0xc000000000245d24, is taken when v0(=[0xc00000000159bb60]:8) != a0(=[a0+80]:8), and falls through when v0(=[0xc00000000159bb60]:8) == a0(=[a0+80]:8).
  The `bne` branch at 0xc000000000245ca8 targets 0xc000000000245cb4, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) < t2(=0xa800000000000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) >= t2(=0xa800000000000000) (unsigned).
  The `bne` branch at 0xc000000000245cb0 targets 0xc000000000245ccc, is taken when a6(?) != 0, and falls through when a6(?) == 0.
  The `bne` branch at 0xc000000000245cbc targets 0xc000000000245cec, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) < t8(=0xc000000000000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) >= t8(=0xc000000000000000) (unsigned).
  The `beq` branch at 0xc000000000245cc4 targets 0xc000000000245cec, is taken when a0(?) == 0, and falls through when a0(?) != 0.
  The `beq` branch at 0xc000000000245cd4 targets 0xc000000000245cec, is taken when v0(=ret(?)) == 0, and falls through when v0(=ret(?)) != 0.
  The `beq` branch at 0xc000000000245ce4 targets 0xc000000000245d10 and is always taken.
  The `beq` branch at 0xc000000000245d38 targets 0xc000000000245d50, is taken when v0(=ret(mfindname)) == 0, and falls through when v0(=ret(mfindname)) != 0.
  The `beq` branch at 0xc000000000245d48 targets 0xc000000000245d10 and is always taken.
  The `beq` branch at 0xc000000000245d80 targets 0xc000000000245d00 and is always taken.
  The return at 0xc000000000245d1c has v0 = one of 0x0, 0x33.
- `idbg_reg` in `6.5.22/unix.IP35` at 0xc000000000245d88 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000245d88-0xc000000000245d90 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000245d88 has v0 = 0x59.
- `idbg_unreg` in `6.5.22/unix.IP35` at 0xc000000000245d90 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000245d90-0xc000000000245d98 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000245d90 has v0 = 0x59.
- `setup_lowmem` in `6.5.22/unix.IP35` at 0xc0000000002651f4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc0000000002651f4-0xc00000000026560c (1048 bytes).
  It occupies 1048 bytes.
  2 such `sd` accesses store 8 bytes to table(0xc000000001606fa0) +0x0, including `sd` at 0xc000000000265294, `sd` at 0xc000000000265594.
  3 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc0000000002652b8, `sd` at 0xc00000000026531c, `sd` at 0xc0000000002654c4.
  The `ld` at 0xc0000000002652d0 loads 8 bytes from abs 0xffffffffffffc400.
  The `ld` at 0xc0000000002652d4 loads 8 bytes from [-0x3c00]:8 +0x0.
  4 such `ld` accesses load 8 bytes from table(0xc000000001606fa0) +0x0, including `ld` at 0xc0000000002652dc, `ld` at 0xc0000000002655b0, `ld` at 0xc0000000002655e8.
  The `sd` at 0xc0000000002652ec stores 8 bytes to ([[-0x3c00]:8]:8 + (a0 << 0x3)) +0x0.
  2 such `sd` accesses store 8 bytes to table(0xc0000000015ffa58) +0x0, including `sd` at 0xc00000000026533c, `sd` at 0xc0000000002653a8.
  2 such `sd` accesses store 8 bytes to table(0xc0000000015fda58) +0x0, including `sd` at 0xc000000000265340, `sd` at 0xc0000000002653a0.
  The `lh` at 0xc00000000026535c loads 2 bytes from table(0xc00000000159cf00) +0x0.
  The `ld` at 0xc000000000265424 loads 8 bytes from abs 0xc000000001606fa0 (Nodepdaindr).
  The `ld` at 0xc000000000265438 loads 8 bytes from [0xc000000001606fa0]:8 +0x5e8.
  2 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc00000000026545c, `sw` at 0xc0000000002654a0.
  The `sd` at 0xc000000000265474 stores 8 bytes to ([[0xc000000001606fa0]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0.
  The `sd` at 0xc0000000002655c0 stores 8 bytes to entry(0xc000000001606fa0) +0x0.
  The `sd` at 0xc0000000002655c8 stores 8 bytes to [0xc00000000159be88]:8 +0x400.
  The `lh` at 0xc0000000002655d0 loads 2 bytes from abs 0xffffffffffffc428.
  The `sd` at 0xc000000000265600 stores 8 bytes to [0xc00000000159be88]:8 +0x408.
  The `jal` at 0xc000000000265244 calls blkclr [0xc0000000000476dc] with a0=?, a1=?, a2=0xa800000000000000, a3=a3 at entry (arg3).
  The `jal` at 0xc00000000026524c calls flush_cache [0xc000000000051484] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xc00000000026525c calls mktables [0xc00000000026560c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xc000000000265278 calls low_mem_alloc [0xc000000000264f18] with a0=0x8f28, a1=sp-112, a2=0xc000000000475af0, a3=?.
  The `jal` at 0xc0000000002652b0 calls low_mem_alloc [0xc000000000264f18] with a0=((([0xc00000000159be94] + [0xc00000000159be98]) << 0x3) << 0x0), a1=sp-112, a2=0xc000000000475b00, a3=?.
  The `jal` at 0xc0000000002652e8 calls nodepda_numa_init [0xc0000000002ee31c] with a0=a0 at entry (arg0), a1=sp-112, a2=?, a3=?, a4=entry(0xc000000001606fa0), a5=([[-0x3c00]:8]:8 + (a0 << 0x3)).
  The `jal` at 0xc000000000265310 calls low_mem_alloc [0xc000000000264f18] with a0=(((([0xc00000000159be94] << 0x4) + [0xc00000000159be94]) << 0x3) << 0x0), a1=sp-112, a2=0xc000000000475b30, a3=?.
  The `jal` at 0xc000000000265354 calls slot_getsize [0xc00000000004fce0] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000265388 calls low_mem_alloc [0xc000000000264f18] with a0=?, a1=sp-112, a2=0xc000000000475b48, a3=?.
  The `jal` at 0xc00000000026539c calls low_mem_alloc [0xc000000000264f18] with a0=?, a1=sp-112, a2=0xc000000000475b60, a3=?.
  The `jal` at 0xc000000000265460 calls low_mem_alloc [0xc000000000264f18] with a0=?, a1=sp-112, a2=0xc000000000475b78, a3=?, a7=[0xc00000000159c0cc].
  The `jal` at 0xc0000000002654a4 calls low_mem_alloc [0xc000000000264f18] with a0=?, a1=sp-112, a2=0xc000000000475b78, a3=?.
  The `jal` at 0xc00000000026553c calls cmn_err_tag [0xc00000000026ef64] with a0=0x6ec, a1=0x4, a2=0xc000000000475b98, a3=?.
  The `jal` at 0xc000000000265558 calls low_mem_alloc [0xc000000000264f18] with a0=0x8f28, a1=sp-112, a2=0xc000000000475b10, a3=[0xc00000000159be98].
  The `jal` at 0xc000000000265590 calls low_mem_alloc [0xc000000000264f18] with a0=((([0xc00000000159be98] + [0xc00000000159be94]) << 0x3) << 0x0), a1=sp-112, a2=0xc000000000475b20, a3=?, a7=table(0xc000000001606fa0).
  The `jal` at 0xc0000000002655cc calls get_cpu_slice [0xc000000000044bd4] with a0=[-0x3bd8]:2, a1=?, a2=?, a3=?, a4=[0xc00000000159be88]:8.
  The `beq` branch at 0xc000000000265290 targets 0xc0000000002655c4, is taken when s7(=a0) == 0, and falls through when s7(=a0) != 0.
  The `bne` branch at 0xc0000000002652c8 targets 0xc000000000265554, is taken when a2(=[0xc000000001593ac8]) < a3(=[0xc00000000159be98]) (signed), and falls through when a2(=[0xc000000001593ac8]) >= a3(=[0xc00000000159be98]) (signed).
  The `beq` branch at 0xc000000000265334 targets 0xc000000000265350 and is always taken.
  The `beq` branch at 0xc00000000026534c targets 0xc0000000002653ac, is taken when s0(?) >= 0x8 (signed), and falls through when s0(?) < 0x8 (signed).
  The `beq` branch at 0xc000000000265378 targets 0xc00000000026533c, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `beq` branch at 0xc0000000002653a4 targets 0xc000000000265344 and is always taken.
  The `beq` branch at 0xc0000000002654c0 targets 0xc00000000026547c, is taken when s3(=0x1000000) >= s1(?) (signed), and falls through when s3(=0x1000000) < s1(?) (signed).
  The `bne` branch at 0xc0000000002654f0 targets 0xc00000000026552c, is taken when t0(?) < a6(?) (unsigned), and falls through when t0(?) >= a6(?) (unsigned).
  The `beq` branch at 0xc00000000026554c targets 0xc0000000002654f8 and is always taken.
  The `beq` branch at 0xc0000000002655bc targets 0xc0000000002652d0 and is always taken.
  The `beq` branch at 0xc000000000265604 targets 0xc000000000265298 and is always taken.
  The return at 0xc000000000265524 has v0 = ?.
- `panic_coproc_select` in `6.5.22/unix.IP35` at 0xc000000000271154 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP35`.
  Range: 0xc000000000271154-0xc0000000002711dc (136 bytes).
  It occupies 136 bytes.
  2 such `lh` accesses load 2 bytes from table(0xc00000000159d000) +0x0, including `lh` at 0xc000000000271194, `lh` at 0xc000000000271198.
  The `ll` at 0xc0000000002711a8 loads 4 bytes from abs 0xc000000001593af8.
  The `sc` at 0xc0000000002711b4 stores 4 bytes to abs 0xc000000001593af8.
  The `jal` at 0xc000000000271174 calls getcpuid [0xc00000000004f190] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000002711cc calls dumpsys_helper [0xc000000000288ab8] with a0=[0xc000000001593af8]+1, a1=0x1, a2=0x1, a3=0xc000000001593af8, a4=[0xc000000001593af8]+1.
  The `beq` branch at 0xc000000000271160 targets 0xc000000000271174, is taken when at(=[0xc000000001593ae8]) == v0(=-0x1), and falls through when at(=[0xc000000001593ae8]) != v0(=-0x1).
  The `bne` branch at 0xc0000000002711a0 targets 0xc000000000271168, is taken when v1(=entry(0xc00000000159d000)) != a0(=entry(0xc00000000159d000)), and falls through when v1(=entry(0xc00000000159d000)) == a0(=entry(0xc00000000159d000)).
  The `beql` branch at 0xc0000000002711b8 targets 0xc0000000002711a8, is taken when a4(=[0xc000000001593af8]+1) == 0, and falls through when a4(=[0xc000000001593af8]+1) != 0; its branch-likely delay slot `nop` at 0xc0000000002711bc runs only when taken and is annulled on fall-through.
  The `bne` branch at 0xc0000000002711c4 targets 0xc00000000027116c, is taken when a0(=[0xc000000001593af8]+1) != a1(=0x1), and falls through when a0(=[0xc000000001593af8]+1) == a1(=0x1).
  The `beq` branch at 0xc0000000002711d4 targets 0xc00000000027116c and is always taken.
  The return at 0xc00000000027116c has v0 = one of ?, ret(getcpuid), ret(dumpsys_helper).
- `setup_ip32_flags` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x83b8 (n32).
  Evidence grade: inferred.
  Exact binary: `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o`.
  Range: 0x83b8-0x84f4 (316 bytes).
  It occupies 316 bytes.
  The `lw` at 0x83f0 loads 4 bytes from table(sym(.rodata)+2680) +0x0.
  The `lw` at 0x84d8 loads 4 bytes from abs 0xffffffffffffa2b0.
  The `jal` at 0x83c0 calls get_cpu_irr with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x8458 calls is_specified with a0=sym(arg_bump_leds), a1=?, a2=[sym(four_set_pcaches)], a3=?, a4=-0x1.
  The `jal` at 0x846c calls is_enabled with a0=sym(arg_bump_leds), a1=[sym(reboot_on_panic)], a2=?, a3=?.
  The `jal` at 0x848c calls kopt_find with a0=sym(.rodata)+2664, a1=[sym(reboot_on_panic)], a2=-0x1, a3=?.
  The `jal` at 0x8498 calls atoi with a0=ret(kopt_find), a1=?, a2=?, a3=?.
  The `beq` branch at 0x83e8 targets 0x8414, is taken when a1(?) >= 0x20 (unsigned), and falls through when a1(?) < 0x20 (unsigned).
  The `beq` branch at 0x8420 targets 0x84d4, is taken when a1(=[sym(is_r4600_flag)]) == 0, and falls through when a1(=[sym(is_r4600_flag)]) != 0.
  The `beq` branch at 0x8438 targets 0x844c, is taken when a2(=[sym(four_set_pcaches)]) == 0, and falls through when a2(=[sym(four_set_pcaches)]) != 0.
  The `beq` branch at 0x8440 targets 0x844c, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0x844c targets 0x84cc, is taken when a0(=[sym(kdebug)]:2) == a4(=-0x1), and falls through when a0(=[sym(kdebug)]:2) != a4(=-0x1).
  The `beq` branch at 0x8460 targets 0x847c, is taken when v0(=ret(is_specified)) == 0, and falls through when v0(=ret(is_specified)) != 0.
  The `beq` branch at 0x8480 targets 0x84b0, is taken when a1(=[sym(reboot_on_panic)]) == a2(=-0x1), and falls through when a1(=[sym(reboot_on_panic)]) != a2(=-0x1).
  The `beq` branch at 0x84b0 targets 0x8484 and is always taken.
  The `beq` branch at 0x84cc targets 0x8454 and is always taken.
  The `beq` branch at 0x84dc targets 0x84ec, is taken when a1(=[-0x5d50]) == 0, and falls through when a1(=[-0x5d50]) != 0.
  The `beq` branch at 0x84e4 targets 0x842c and is always taken.
  The `beq` branch at 0x84ec targets 0x8428 and is always taken.
  The return at 0x84a8 has v0 = ret(atoi).
  The instructions from 0x83fc to 0x8410 are not reached by any path from the entry, and their branches are not listed.
  The instructions from 0x84b8 to 0x84c8 are not reached by any path from the entry, and their branches are not listed.
  The indirect jump at 0x83f4 uses a jump table whose successors are unresolved.
- `crimestray` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x9f18 (n32).
  Evidence grade: inferred.
  Exact binary: `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o`.
  Range: 0x9f18-0x9f4c (52 bytes).
  It occupies 52 bytes.
  The `jal` at 0x9f2c calls cmn_err with a0=0x4, a1=sym(.rodata)+2984, a2=a1 at entry (arg1), a3=a3 at entry (arg3).
  The return at 0x9f44 has v0 = ret(cmn_err).
- `crime_intr` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xa2c4 (n32).
  Evidence grade: inferred.
  Exact binary: `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o`.
  Range: 0xa2c4-0xa570 (684 bytes).
  It occupies 684 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x138, including `ld` at 0xa2f0, `ld` at 0xa4c8.
  2 such `ld` accesses load 8 bytes from abs 0xffffffffb4000010, including `ld` at 0xa2fc, `ld` at 0xa324.
  The `sw` at 0xa35c stores 4 bytes to (sp-208 + ([sym(.data)+1468]:2 << 0x2)) +0x0.
  The `lh` at 0xa368 loads 2 bytes from sym(.data) +0x58c.
  The `sw` at 0xa38c stores 4 bytes to (sp-208 + ([sym(.data)+1420]:2 << 0x2)) +0x0.
  The `lh` at 0xa398 loads 2 bytes from sym(.data) +0x5bc.
  3 such `lw` accesses load 4 bytes from table(sym(.data)+1408) +0x98, including `lw` at 0xa3c8, `lw` at 0xa444, `lw` at 0xa4f4.
  5 such `ld` accesses load 8 bytes from table(sym(.data)+1408) +0xa0, including `ld` at 0xa3d0, `ld` at 0xa46c, `ld` at 0xa4c4.
  3 such `ld` accesses load 8 bytes from ?, including `ld` at 0xa3f0, `ld` at 0xa478, `ld` at 0xa510.
  3 such `sd` accesses store 8 bytes to ?, including `sd` at 0xa404, `sd` at 0xa48c, `sd` at 0xa524.
  The `lw` at 0xa424 loads 4 bytes from ?.
  2 such `lh` accesses load 2 bytes from table(sym(.data)+1408) +0x9e, including `lh` at 0xa440, `lh` at 0xa4b4.
  The `lw` at 0xa44c loads 4 bytes from table(sym(.data)+1408) +0xa8.
  The `lw` at 0xa4a0 loads 4 bytes from table(sym(.data)+1408) +0x90.
  The `lw` at 0xa4b0 loads 4 bytes from table(sym(.data)+1408) +0x94.
  2 such `sd` accesses store 8 bytes to a0 (arg0 at entry) +0x138, including `sd` at 0xa4d4, `sd` at 0xa538.
  The `jal` at 0xa3c0 calls splint with a0=[table(sym(.data)+1408)+152], a1=?, a2=?, a3=?.
  The `jal` at 0xa408 calls vsema with a0=table(sym(.data)+1408)+172, a1=?, a2=?, a3=?.
  The `jal` at 0xa410 calls splx with a0=ret(splint), a1=?, a2=?, a3=?.
  The `jal` at 0xa498 calls splint with a0=[table(sym(.data)+1408)+152], a1=?, a2=?, a3=?.
  The `jalr` at 0xa4ac calls (unknown callee) with a0=a0 at entry (arg0), a1=[table(sym(.data)+1408)+148], a2=?, a3=?.
  The `jal` at 0xa4e4 calls splx with a0=ret(splint), a1=?, a2=?, a3=?.
  The `beq` branch at 0xa314 targets 0xa564 and is always taken.
  The `beq` branch at 0xa330 targets 0xa530, is taken when a6(?) == 0, and falls through when a6(?) != 0.
  The `beq` branch at 0xa344 targets 0xa360 and is always taken.
  The `beq` branch at 0xa358 targets 0xa3b8, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xa370 targets 0xa3b0, is taken when (a6(?) & v0(?)) == 0, and falls through when (a6(?) & v0(?)) != 0.
  The `beq` branch at 0xa388 targets 0xa3b8, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `bne` branch at 0xa3a0 targets 0xa34c, is taken when t0(?) != 0, and falls through when t0(?) == 0.
  The `beq` branch at 0xa3a8 targets 0xa350 and is always taken.
  The `beq` branch at 0xa3b0 targets 0xa380 and is always taken.
  The `beq` branch at 0xa3b8 targets 0xa428 and is always taken.
  The `blez` branch at 0xa3e4 targets 0xa408, is taken when a3(?) <= 0 (signed), and falls through when a3(?) > 0 (signed).
  The `bne` branch at 0xa400 targets 0xa3ec, is taken when a6(?) < a3(?) (signed), and falls through when a6(?) >= a3(?) (signed).
  The `beq` branch at 0xa41c targets 0xa31c, is taken when s7(?) == 0, and falls through when s7(?) != 0.
  The `beq` branch at 0xa430 targets 0xa418, is taken when v1(?) == s6(=-0x1), and falls through when v1(?) != s6(=-0x1).
  The `beq` branch at 0xa45c targets 0xa490, is taken when (a1(=[table(sym(.data)+1408)+158]:2) & 0x4) == 0, and falls through when (a1(=[table(sym(.data)+1408)+158]:2) & 0x4) != 0.
  The `blez` branch at 0xa464 targets 0xa490, is taken when a3(?) <= 0 (signed), and falls through when a3(?) > 0 (signed).
  The `bne` branch at 0xa488 targets 0xa474, is taken when a5(?) < a3(?) (signed), and falls through when a5(?) >= a3(?) (signed).
  The `bne` branch at 0xa490 targets 0xa3c0, is taken when a6(?) != 0, and falls through when a6(?) == 0.
  The `beq` branch at 0xa4bc targets 0xa4f4, is taken when (t1(=[table(sym(.data)+1408)+158]:2) & 0x4) == 0, and falls through when (t1(=[table(sym(.data)+1408)+158]:2) & 0x4) != 0.
  The `beq` branch at 0xa4ec targets 0xa41c and is always taken.
  The `blez` branch at 0xa500 targets 0xa4e4, is taken when a3(?) <= 0 (signed), and falls through when a3(?) > 0 (signed).
  The `bne` branch at 0xa520 targets 0xa50c, is taken when a5(?) < a3(?) (signed), and falls through when a5(?) >= a3(?) (signed).
  The `beq` branch at 0xa528 targets 0xa4e4 and is always taken.
  The `beq` branch at 0xa568 targets 0xa33c and is always taken.
  The return at 0xa55c has v0 = ?.
- `ecc_fixmem` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xdde4 (n32).
  Evidence grade: inferred.
  Exact binary: `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o`.
  Range: 0xdde4-0xde58 (116 bytes).
  It occupies 116 bytes.
  The `ld` at 0xde10 loads 8 bytes from table(sym(ecc_info)) +0x68.
  The `lw` at 0xde14 loads 4 bytes from table(sym(ecc_info)) +0x7c.
  2 such `sd` accesses store 8 bytes to a2 (arg2 at entry) +0x8, including `sd` at 0xde20, `sd` at 0xde54.
  The `lw` at 0xde24 loads 4 bytes from table(sym(ecc_info)) +0x80.
  The `sw` at 0xde28 stores 4 bytes to table(sym(ecc_info)) +0x84.
  The `sw` at 0xde2c stores 4 bytes to table(sym(ecc_info)) +0x88.
  The `ld` at 0xde34 loads 8 bytes from abs 0xffffffffb4000260.
  The `sw` at 0xde40 stores 4 bytes to table(sym(ecc_info)) +0x8c.
  The `lw` at 0xde44 loads 4 bytes from table(sym(ecc_info)) +0x78.
  The `bgez` branch at 0xde0c targets 0xde44, is taken when v0(=([sym(r4000_config)] << 0x2e)) >= 0 (signed), and falls through when v0(=([sym(r4000_config)] << 0x2e)) < 0 (signed).
  The `beq` branch at 0xde50 targets 0xde24 and is always taken.
  The return at 0xde3c has v0 = 0x1.
- `ioc3_early_pckminit` in `6.5.30/unix.IP27` at 0xc000000000096384 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000096384-0xc00000000009638c (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000096384 has v0 = ?.
- `ioc3_pckm_kbdexists` in `6.5.30/unix.IP27` at 0xc000000000097c24 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000097c24-0xc000000000097c60 (60 bytes).
  It occupies 60 bytes.
  The `lw` at 0xc000000000097c30 loads 4 bytes from [0xc0000000014d2c90]:8 +0x28.
  The `ld` at 0xc000000000097c40 loads 8 bytes from ([0xc0000000014d2c90]:8 + ([[0xc0000000014d2c90]:8+40] << 0x3)) +0x18.
  The `lw` at 0xc000000000097c44 loads 4 bytes from [([0xc0000000014d2c90]:8 + ([[0xc0000000014d2c90]:8+40] << 0x3))+24]:8 +0x4.
  The `beq` branch at 0xc000000000097c28 targets 0xc000000000097c58, is taken when a2(=[0xc0000000014d2c90]:8) == 0, and falls through when a2(=[0xc0000000014d2c90]:8) != 0.
  The `beq` branch at 0xc000000000097c34 targets 0xc000000000097c58, is taken when a3(=[[0xc0000000014d2c90]:8+40]) == v0(=0x13), and falls through when a3(=[[0xc0000000014d2c90]:8+40]) != v0(=0x13).
  The return at 0xc000000000097c50 has v0 = ?.
  The return at 0xc000000000097c58 has v0 = 0x0.
- `ioc3_pckm_setleds` in `6.5.30/unix.IP27` at 0xc000000000097c60 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000097c60-0xc000000000097c9c (60 bytes).
  It occupies 60 bytes.
  The `lw` at 0xc000000000097c78 loads 4 bytes from [0xc0000000014d2c90]:8 +0x28.
  The `ld` at 0xc000000000097c8c loads 8 bytes from ([0xc0000000014d2c90]:8 + ([[0xc0000000014d2c90]:8+40] << 0x3)) +0x18.
  The `jal` at 0xc000000000097c88 calls ioc3_lcl_pckm_setleds [0xc000000000097d14] with a0=[([0xc0000000014d2c90]:8 + ([[0xc0000000014d2c90]:8+40] << 0x3))+24]:8, a1=a0 at entry (arg0), a2=a2 at entry (arg2), a3=[0xc0000000014d2c90]:8, a4=0x13, a5=[[0xc0000000014d2c90]:8+40].
  The `beq` branch at 0xc000000000097c70 targets 0xc000000000097c90, is taken when a3(=[0xc0000000014d2c90]:8) == 0, and falls through when a3(=[0xc0000000014d2c90]:8) != 0.
  The `beq` branch at 0xc000000000097c7c targets 0xc000000000097c90, is taken when a5(=[[0xc0000000014d2c90]:8+40]) == a4(=0x13), and falls through when a5(=[[0xc0000000014d2c90]:8+40]) != a4(=0x13).
  The return at 0xc000000000097c94 has v0 = one of ?, ret(ioc3_lcl_pckm_setleds).
- `ioc3_pckm_bell` in `6.5.30/unix.IP27` at 0xc000000000097c9c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000097c9c-0xc000000000097ce0 (68 bytes).
  It occupies 68 bytes.
  The `lw` at 0xc000000000097cb0 loads 4 bytes from [0xc0000000014d2c90]:8 +0x28.
  The `lw` at 0xc000000000097cbc loads 4 bytes from [0xc0000000014d2c90]:8 +0x34.
  The `ld` at 0xc000000000097cd0 loads 8 bytes from ([0xc0000000014d2c90]:8 + ([[0xc0000000014d2c90]:8+40] << 0x3)) +0x18.
  The `jal` at 0xc000000000097ccc calls ioc3_lcl_pckm_setleds [0xc000000000097d14] with a0=[([0xc0000000014d2c90]:8 + ([[0xc0000000014d2c90]:8+40] << 0x3))+24]:8, a1=([[0xc0000000014d2c90]:8+52] | 0x8), a2=a2 at entry (arg2), a3=[0xc0000000014d2c90]:8, a4=[[0xc0000000014d2c90]:8+40].
  The `beq` branch at 0xc000000000097ca8 targets 0xc000000000097cd4, is taken when a3(=[0xc0000000014d2c90]:8) == 0, and falls through when a3(=[0xc0000000014d2c90]:8) != 0.
  The `beq` branch at 0xc000000000097cb4 targets 0xc000000000097cd8, is taken when a4(=[[0xc0000000014d2c90]:8+40]) == a1(=0x13), and falls through when a4(=[[0xc0000000014d2c90]:8+40]) != a1(=0x13).
  The return at 0xc000000000097cd8 has v0 = ?.
- `ioc3_gl_setporthandler` in `6.5.30/unix.IP27` at 0xc000000000097ce0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000097ce0-0xc000000000097d0c (44 bytes).
  It occupies 44 bytes.
  The `lw` at 0xc000000000097cec loads 4 bytes from [0xc0000000014d2c90]:8 +0x28.
  The `ld` at 0xc000000000097cfc loads 8 bytes from ([0xc0000000014d2c90]:8 + ([[0xc0000000014d2c90]:8+40] << 0x3)) +0x18.
  The `sd` at 0xc000000000097d00 stores 8 bytes to [([0xc0000000014d2c90]:8 + ([[0xc0000000014d2c90]:8+40] << 0x3))+24]:8 +0x18.
  The `beq` branch at 0xc000000000097ce4 targets 0xc000000000097d04, is taken when a2(=[0xc0000000014d2c90]:8) == 0, and falls through when a2(=[0xc0000000014d2c90]:8) != 0.
  The `beq` branch at 0xc000000000097cf0 targets 0xc000000000097d04, is taken when a4(=[[0xc0000000014d2c90]:8+40]) == a3(=0x13), and falls through when a4(=[[0xc0000000014d2c90]:8+40]) != a3(=0x13).
  The return at 0xc000000000097d04 has v0 = ?.
- `ioc3_du_keyboard_port` in `6.5.30/unix.IP27` at 0xc000000000097d0c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000097d0c-0xc000000000097d14 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000097d0c has v0 = -0x1.
- `ioc3_lcl_pckm_setleds` in `6.5.30/unix.IP27` at 0xc000000000097d14 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000097d14-0xc000000000097d68 (84 bytes).
  It occupies 84 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x48, including `ld` at 0xc000000000097d20, `ld` at 0xc000000000097d48.
  The `jal` at 0xc000000000097d34 calls mutex_lock [0xc00000000018a734] with a0=[a0+72]:8+72, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc000000000097d40 calls ioc3_lcl_pckm_setleds_lock [0xc000000000097d68] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xc000000000097d50 calls mutex_unlock [0xc00000000018a66c] with a0=[a0+72]:8+72, a1=?, a2=?, a3=?.
  The return at 0xc000000000097d60 has v0 = ret(mutex_unlock).
- `ioc3_lcl_pckm_setleds_lock` in `6.5.30/unix.IP27` at 0xc000000000097d68 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000097d68-0xc000000000097ee0 (376 bytes).
  It occupies 376 bytes.
  The `ld` at 0xc000000000097d74 loads 8 bytes from a0 (arg0 at entry) +0x48.
  5 such `lw` accesses load 4 bytes from [a0+72]:8 +0x34, including `lw` at 0xc000000000097d7c, `lw` at 0xc000000000097df0, `lw` at 0xc000000000097e10.
  The `lw` at 0xc000000000097dac loads 4 bytes from [a0+72]:8 +0x38.
  The `sw` at 0xc000000000097dd8 stores 4 bytes to [a0+72]:8 +0x38.
  2 such `sw` accesses store 4 bytes to [a0+72]:8 +0x34, including `sw` at 0xc000000000097de4, `sw` at 0xc000000000097e18.
  The `jal` at 0xc000000000097dec calls pckm_pollcmd [0xc000000000097b48] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xc000000000097df8 calls data_check [0xc000000000097fa8] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc000000000097e44 calls pckm_pollcmd [0xc000000000097b48] with a0=a0 at entry (arg0), a1=0xed, a2=?, a3=?.
  The `jal` at 0xc000000000097e50 calls data_check [0xc000000000097fa8] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc000000000097e70 calls pckm_pollcmd [0xc000000000097b48] with a0=a0 at entry (arg0), a1=0xf4, a2=?, a3=?.
  The `jal` at 0xc000000000097e7c calls data_check [0xc000000000097fa8] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc000000000097e94 calls default_kbd_setting [0xc000000000097448] with a0=a0 at entry (arg0), a1=0x0, a2=[[a0+72]:8+52], a3=?.
  The `jal` at 0xc000000000097eb4 calls pckm_pollcmd [0xc000000000097b48] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xc000000000097ec0 calls data_check [0xc000000000097fa8] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc000000000097ed0 calls pckm_reinit_core [0xc0000000000973d0] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xc000000000097d9c targets 0xc000000000097e1c, is taken when at(=[[a0+72]:8+52]) == a3(?), and falls through when at(=[[a0+72]:8+52]) != a3(?).
  The `beq` branch at 0xc000000000097da4 targets 0xc000000000097de0, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xc000000000097db8 targets 0xc000000000097dc8, is taken when a7(?) >= 0xe (signed), and falls through when a7(?) < 0xe (signed).
  The `bne` branch at 0xc000000000097dc0 targets 0xc000000000097e20, is taken when a4(?) != 0, and falls through when a4(?) == 0.
  The `beq` branch at 0xc000000000097dc8 targets 0xc000000000097dd8, is taken when a0(?) == 0, and falls through when a0(?) != 0.
  The `bne` branch at 0xc000000000097dd0 targets 0xc000000000097e20, is taken when a5(?) != 0, and falls through when a5(?) == 0.
  The `beq` branch at 0xc000000000097de0 targets 0xc000000000097e40 and is always taken.
  The `beq` branch at 0xc000000000097e00 targets 0xc000000000097eb4, is taken when v0(=ret(data_check)) == 0, and falls through when v0(=ret(data_check)) != 0.
  The `beq` branch at 0xc000000000097e08 targets 0xc000000000097e20, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xc000000000097e3c targets 0xc000000000097ea4, is taken when t1(?) == 0, and falls through when t1(?) != 0.
  The `bne` branch at 0xc000000000097e58 targets 0xc000000000097de8, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `bne` branch at 0xc000000000097e64 targets 0xc000000000097e38, is taken when s3(?) < 0x2 (unsigned), and falls through when s3(?) >= 0x2 (unsigned).
  The `bne` branch at 0xc000000000097e84 targets 0xc000000000097e3c, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `beq` branch at 0xc000000000097e9c targets 0xc000000000097e3c and is always taken.
  The `bne` branch at 0xc000000000097ea4 targets 0xc000000000097e10, is taken when s2(?) != 0, and falls through when s2(?) == 0.
  The `beq` branch at 0xc000000000097eac targets 0xc000000000097e20 and is always taken.
  The `bne` branch at 0xc000000000097ec8 targets 0xc000000000097e08, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `beq` branch at 0xc000000000097ed8 targets 0xc000000000097e08 and is always taken.
  The return at 0xc000000000097e30 has v0 = ?.
- `dev_chain` in `6.5.30/unix.IP27` at 0xc0000000001437a4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001437a4-0xc000000000143890 (236 bytes).
  It occupies 236 bytes.
  The `ld` at 0xc0000000001437b0 loads 8 bytes from a0 (arg0 at entry) +0x38.
  2 such `lw` accesses load 4 bytes from abs 0xc00000000153dd68 (v+48), including `lw` at 0xc0000000001437c4, `lw` at 0xc000000000143828.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x50, including `lw` at 0xc0000000001437cc, `lw` at 0xc000000000143810.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x28, including `ld` at 0xc0000000001437ec, `ld` at 0xc000000000143800.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x30, including `ld` at 0xc0000000001437f0, `ld` at 0xc0000000001437f8.
  The `sd` at 0xc0000000001437f4 stores 8 bytes to [a0+40]:8 +0x30.
  The `sd` at 0xc00000000014380c stores 8 bytes to [a0+48]:8 +0x28.
  2 such `ld` accesses load 8 bytes from ?, including `ld` at 0xc000000000143848, `ld` at 0xc000000000143854.
  The `sd` at 0xc00000000014384c stores 8 bytes to a0 (arg0 at entry) +0x30.
  The `sd` at 0xc000000000143850 stores 8 bytes to a0 (arg0 at entry) +0x28.
  2 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc000000000143858, `sd` at 0xc000000000143860.
  The `sd` at 0xc00000000014386c stores 8 bytes to a0 (arg0 at entry) +0x38.
  The `jal` at 0xc0000000001437e4 calls mutex_spinlock [0xc0000000002863b4] with a0=[a0+56]:8+56, a1=a1 at entry (arg1), a2=[a0+56]:8, a3=[0xc00000000153dd68].
  The `jal` at 0xc000000000143808 calls mutex_spinunlock [0xc000000000286400] with a0=[a0+56]:8+56, a1=ret(mutex_spinlock), a2=?, a3=?, a6=[a0+40]:8, a7=[a0+48]:8.
  The `jal` at 0xc000000000143840 calls mutex_spinlock [0xc0000000002863b4] with a0=?, a1=?, a2=0xc000000001507180, a3=0xc00000000153dd38.
  The `jal` at 0xc000000000143868 calls mutex_spinunlock [0xc000000000286400] with a0=?, a1=ret(mutex_spinlock), a2=?, a3=?.
  The `beq` branch at 0xc0000000001437c0 targets 0xc000000000143810, is taken when a2(=[a0+56]:8) == 0, and falls through when a2(=[a0+56]:8) != 0.
  The `beq` branch at 0xc0000000001437dc targets 0xc000000000143874, is taken when a2(=[a0+56]:8) == a4(?), and falls through when a2(=[a0+56]:8) != a4(?).
  The `beq` branch at 0xc000000000143820 targets 0xc000000000143888, is taken when a0(=[a0+80]) == t2(=-0x1), and falls through when a0(=[a0+80]) != t2(=-0x1).
  The `beq` branch at 0xc000000000143888 targets 0xc00000000014383c and is always taken.
  The return at 0xc000000000143880 has v0 = one of ?, ret(mutex_spinunlock).
- `ioc3_config` in `6.5.30/unix.IP27` at 0xc0000000001770a0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001770a0-0xc000000000177434 (916 bytes).
  It occupies 916 bytes.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc000000000177120, `lw` at 0xc000000000177258, `lw` at 0xc0000000001773e8.
  4 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc00000000017713c, `ld` at 0xc0000000001771bc, `ld` at 0xc000000000177278.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000177150, `ld` at 0xc0000000001771d0, `ld` at 0xc00000000017728c.
  4 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc00000000017715c, `lw` at 0xc0000000001771dc, `lw` at 0xc000000000177298.
  6 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc000000000177168, `ld` at 0xc0000000001771e8, `ld` at 0xc0000000001772a4.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc000000000177174, `ld` at 0xc000000000177184, `ld` at 0xc0000000001772b0.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc000000000177180, `sw` at 0xc0000000001772b4.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc000000000177188, `lw` at 0xc000000000177370.
  The `ld` at 0xc000000000177218 loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xc00000000017721c loads 1 byte from [a0+72]:8 +0x3.
  The `sb` at 0xc000000000177254 stores 1 byte to [a0+72]:8 +0x3.
  The `sw` at 0xc000000000177350 stores 4 bytes to a0 (arg0 at entry) +0x80.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x84, including `sw` at 0xc000000000177358, `sw` at 0xc00000000017739c.
  The `lw` at 0xc000000000177364 loads 4 bytes from a0 (arg0 at entry) +0x88.
  The `jal` at 0xc0000000001770c4 calls set_baud_ti [0xc0000000001764a0] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jalr` at 0xc0000000001771fc calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8], a2=[[0xa800000000001020]:8+80]:8, a3=[0xc0000000014d3554], a7=0x40000000.
  The `jal` at 0xc000000000177204 calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000177308 calls (unknown callee) with a0=0x1, a1=0xfffffffffeeddead, a2=[[0xa800000000001020]:8+80]:8, a3=a1 at entry (arg1).
  The `jal` at 0xc000000000177310 calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000177360 calls ioc3_rx_timeout [0xc000000000178700] with a0=a0 at entry (arg0), a1=[a0+136], a2=?, a3=a1 at entry (arg1), a6=0xa.
  The `jal` at 0xc0000000001773c8 calls spl7 [0xc00000000003b3bc] with a0=[a0+148], a1=([a0+148] << 0x23), a2=0xa800000000001020, a3=[0xa800000000001000]:8, a4=[[0xa800000000001020]:8], a5=0xfffffffffeeddead, a6=[[0xa800000000001020]:8+80]:8.
  The `jalr` at 0xc0000000001773e0 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001773f8 calls spl7 [0xc00000000003b3bc] with a0=[a0+148], a1=?, a2=[a0+72]:8, a3=?, a4=[0xc0000000014d3558]:8, a5=[0xa800000000001000]:8, a6=[[0xa800000000001020]:8], a7=0xfffffffffeeddead.
  The `jalr` at 0xc00000000017740c calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `beq` branch at 0xc0000000001770cc targets 0xc0000000001770e8, is taken when v0(=ret(set_baud_ti)) == 0, and falls through when v0(=ret(set_baud_ti)) != 0.
  The `beq` branch at 0xc0000000001770ec targets 0xc00000000017711c, is taken when s1(=a2) == at(=0x5), and falls through when s1(=a2) != at(=0x5).
  The `beq` branch at 0xc0000000001770f4 targets 0xc0000000001773a0, is taken when s1(=a2) == v0(=0x6), and falls through when s1(=a2) != v0(=0x6).
  The `beq` branch at 0xc0000000001770fc targets 0xc0000000001773a8, is taken when s1(=a2) == v1(=0x7), and falls through when s1(=a2) != v1(=0x7).
  The `beq` branch at 0xc000000000177104 targets 0xc000000000177114, is taken when s1(=a2) == a0(=0x8), and falls through when s1(=a2) != a0(=0x8).
  The `beq` branch at 0xc00000000017710c targets 0xc0000000001770d4 and is always taken.
  The `beq` branch at 0xc000000000177114 targets 0xc000000000177120 and is always taken.
  The `bgez` branch at 0xc000000000177128 targets 0xc000000000177218, is taken when a1(=([a0+148] << 0x23)) >= 0 (signed), and falls through when a1(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc000000000177130 targets 0xc000000000177174, is taken when a2(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a2(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc00000000017714c targets 0xc000000000177174, is taken when a3(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a3(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc000000000177154 targets 0xc000000000177174, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000177160 targets 0xc000000000177174, is taken when a4(=[[0xa800000000001020]:8]) != a5(=0xfffffffffeeddead), and falls through when a4(=[[0xa800000000001020]:8]) == a5(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000017716c targets 0xc0000000001773c8, is taken when a6(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bltz` branch at 0xc000000000177190 targets 0xc0000000001771a8, is taken when a7(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when a7(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `beq` branch at 0xc0000000001771a0 targets 0xc000000000177424 and is always taken.
  The `bne` branch at 0xc0000000001771b0 targets 0xc000000000177210, is taken when a1(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a1(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001771cc targets 0xc000000000177210, is taken when v1(=[0xa800000000001000]:8) != a1(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a1(=0x53435241).
  The `beq` branch at 0xc0000000001771d4 targets 0xc000000000177210, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001771e0 targets 0xc000000000177210, is taken when a1(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a1(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001771ec targets 0xc000000000177210, is taken when a2(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc000000000177210 targets 0xc0000000001773c0, is taken when a0(=one of 0x0, 0x1) == 0, and falls through when a0(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc00000000017722c targets 0xc000000000177244, is taken when v1(=a4) == 0, and falls through when v1(=a4) != 0.
  The `beq` branch at 0xc00000000017723c targets 0xc0000000001773b0, is taken when a0(=a5) == 0, and falls through when a0(=a5) != 0.
  The `beq` branch at 0xc000000000177248 targets 0xc000000000177254, is taken when a1(=a3) == 0, and falls through when a1(=a3) != 0.
  The `beq` branch at 0xc000000000177264 targets 0xc000000000177318, is taken when (a3(=0x10000000) & a0(=[a0+148])) == 0, and falls through when (a3(=0x10000000) & a0(=[a0+148])) != 0.
  The `bne` branch at 0xc00000000017726c targets 0xc0000000001772b0, is taken when a4(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a4(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000177288 targets 0xc0000000001772b0, is taken when a5(=[0xa800000000001000]:8) != a6(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a6(=0x53435241).
  The `beq` branch at 0xc000000000177290 targets 0xc0000000001772b0, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000017729c targets 0xc0000000001772b0, is taken when a6(=[[0xa800000000001020]:8]) != a7(=0xfffffffffeeddead), and falls through when a6(=[[0xa800000000001020]:8]) == a7(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000001772a8 targets 0xc0000000001773f8, is taken when t0(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when t0(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc0000000001772bc targets 0xc00000000017731c, is taken when t1(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when t1(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001772dc targets 0xc000000000177318, is taken when v1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000001772e4 targets 0xc00000000017731c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001772f0 targets 0xc00000000017731c, is taken when a0(=[[0xa800000000001020]:8]) != a1(=0xfffffffffeeddead), and falls through when a0(=[[0xa800000000001020]:8]) == a1(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000001772fc targets 0xc00000000017731c, is taken when a2(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc000000000177354 targets 0xc000000000177394, is taken when a2(?) == 0, and falls through when a2(?) != 0.
  The `beq` branch at 0xc000000000177368 targets 0xc0000000001770d4 and is always taken.
  The `bne` branch at 0xc00000000017737c targets 0xc0000000001771a8, is taken when (t0(=[[a0+64]:8]) & a7(=0x40000000)) != 0, and falls through when (t0(=[[a0+64]:8]) & a7(=0x40000000)) == 0.
  The `beq` branch at 0xc000000000177384 targets 0xc000000000177370, is taken when t1(?) == 0, and falls through when t1(?) != 0.
  The `beq` branch at 0xc00000000017738c targets 0xc0000000001771ac and is always taken.
  The `beq` branch at 0xc000000000177398 targets 0xc00000000017735c and is always taken.
  The `beq` branch at 0xc0000000001773a0 targets 0xc000000000177120 and is always taken.
  The `beq` branch at 0xc0000000001773a8 targets 0xc000000000177120 and is always taken.
  The `bne` branch at 0xc0000000001773b0 targets 0xc000000000177250, is taken when t3(=a3) != 0, and falls through when t3(=a3) == 0.
  The `beq` branch at 0xc0000000001773b8 targets 0xc000000000177254 and is always taken.
  The `beq` branch at 0xc0000000001773c0 targets 0xc0000000001770d4 and is always taken.
  The `beq` branch at 0xc0000000001773f0 targets 0xc000000000177174 and is always taken.
  The `beq` branch at 0xc00000000017741c targets 0xc0000000001772b0 and is always taken.
  The `beq` branch at 0xc00000000017742c targets 0xc000000000177384 and is always taken.
  The return at 0xc0000000001770e0 has v0 = one of -0x1, 0x0, 0x1.
- `ioc3_set_extclk` in `6.5.30/unix.IP27` at 0xc000000000177558 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000177558-0xc000000000177560 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000177558 has v0 = a1.
- `ioc3_write` in `6.5.30/unix.IP27` at 0xc0000000001777dc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001777dc-0xc0000000001777f8 (28 bytes).
  It occupies 28 bytes.
  The `jal` at 0xc0000000001777e4 calls do_ioc3_write [0xc000000000177560] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The return at 0xc0000000001777f0 has v0 = ret(do_ioc3_write).
- `ioc3_sync_write` in `6.5.30/unix.IP27` at 0xc0000000001777f8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001777f8-0xc000000000177858 (96 bytes).
  It occupies 96 bytes.
  The `lw` at 0xc000000000177824 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xc00000000017780c calls do_ioc3_write [0xc000000000177560] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc000000000177838 calls ioc3_enable_tx [0xc000000000177bc0] with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `blez` branch at 0xc000000000177814 targets 0xc000000000177840, is taken when s3(=a2) <= 0 (signed), and falls through when s3(=a2) > 0 (signed).
  The `bne` branch at 0xc00000000017781c targets 0xc000000000177840, is taken when v0(=ret(do_ioc3_write)) != 0, and falls through when v0(=ret(do_ioc3_write)) == 0.
  The `beq` branch at 0xc00000000017782c targets 0xc000000000177844, is taken when (at(=[a0+120]) & 0x10) == 0, and falls through when (at(=[a0+120]) & 0x10) != 0.
  The return at 0xc000000000177850 has v0 = ret(do_ioc3_write).
- `ioc3_wrflush` in `6.5.30/unix.IP27` at 0xc000000000177858 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000177858-0xc00000000017791c (196 bytes).
  It occupies 196 bytes.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x38, including `ld` at 0xc000000000177858, `ld` at 0xc000000000177894, `ld` at 0xc0000000001778dc.
  3 such `lw` accesses load 4 bytes from [a0+56]:8 +0x1c, including `lw` at 0xc00000000017785c, `lw` at 0xc000000000177898, `lw` at 0xc0000000001778e0.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc000000000177878, `ld` at 0xc0000000001778d0, `ld` at 0xc0000000001778f8.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc00000000017787c, `lw` at 0xc0000000001778fc.
  The `lw` at 0xc0000000001778b4 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xc0000000001778c4 calls ioc3_enable_tx [0xc000000000177bc0] with a0=a0 at entry (arg0), a1=0x1, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001778f0 calls us_delay [0xc000000000047c0c] with a0=0x5, a1=?, a2=one of 0x10000000, 0x50000000, a3=?.
  The `beq` branch at 0xc000000000177870 targets 0xc0000000001778b4, is taken when (at(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (at(=[[a0+56]:8+28]) & 0x1) != 0.
  The `bne` branch at 0xc00000000017788c targets 0xc0000000001778a8, is taken when a0(=0x10000000) != v0(?), and falls through when a0(=0x10000000) == v0(?).
  The `beq` branch at 0xc0000000001778a0 targets 0xc0000000001778d4, is taken when (v1(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (v1(=[[a0+56]:8+28]) & 0x1) != 0.
  The `beq` branch at 0xc0000000001778bc targets 0xc000000000177878, is taken when (a0(=[a0+120]) & 0x10) == 0, and falls through when (a0(=[a0+120]) & 0x10) != 0.
  The `beq` branch at 0xc0000000001778cc targets 0xc00000000017787c and is always taken.
  The `beq` branch at 0xc0000000001778d4 targets 0xc0000000001778f0 and is always taken.
  The `bne` branch at 0xc0000000001778e8 targets 0xc000000000177914, is taken when (a1(=[[a0+56]:8+28]) & 0x1) != 0, and falls through when (a1(=[[a0+56]:8+28]) & 0x1) == 0.
  The `beq` branch at 0xc00000000017790c targets 0xc0000000001778dc, is taken when a2(=0x10000000) == a3(?), and falls through when a2(=0x10000000) != a3(?).
  The `beq` branch at 0xc000000000177914 targets 0xc0000000001778a8 and is always taken.
  The return at 0xc0000000001778ac has v0 = one of ?, ret(us_delay).
- `ioc3_break` in `6.5.30/unix.IP27` at 0xc00000000017791c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc00000000017791c-0xc000000000177bc0 (676 bytes).
  It occupies 676 bytes.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc00000000017791c, `lw` at 0xc000000000177a48, `lw` at 0xc000000000177b70.
  4 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000177950, `ld` at 0xc0000000001779dc, `ld` at 0xc000000000177a64.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000177964, `ld` at 0xc0000000001779e0, `ld` at 0xc000000000177a78.
  4 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000177970, `lw` at 0xc0000000001779f4, `lw` at 0xc000000000177a84.
  6 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc00000000017797c, `ld` at 0xc000000000177a00, `ld` at 0xc000000000177a90.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc000000000177988, `ld` at 0xc000000000177998, `ld` at 0xc000000000177a9c.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc000000000177994, `sw` at 0xc000000000177aa0.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc00000000017799c, `lw` at 0xc000000000177b18.
  The `ld` at 0xc000000000177a34 loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xc000000000177a3c loads 1 byte from [a0+72]:8 +0x3.
  2 such `sb` accesses store 1 byte to [a0+72]:8 +0x3, including `sb` at 0xc000000000177a44, `sb` at 0xc000000000177b48.
  The `jalr` at 0xc000000000177a14 calls (unknown callee) with a0=0x1, a1=a1 at entry (arg1), a2=[[0xa800000000001020]:8], a3=0xfffffffffeeddead, a4=[[0xa800000000001020]:8+80]:8, a5=[0xc0000000014d3554], a6=0x10000000, a7=[0xc0000000014d3558]:8.
  The `jal` at 0xc000000000177a1c calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000177af0 calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=?, a4=[[0xa800000000001020]:8+80]:8.
  The `jal` at 0xc000000000177af8 calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000177b54 calls spl7 [0xc00000000003b3bc] with a0=[a0+72]:8, a1=a1 at entry (arg1), a2=?, a3=?, a4=[a0+148], a6=0x10000000.
  The `jalr` at 0xc000000000177b68 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a5=[[0xa800000000001020]:8+80]:8.
  The `jal` at 0xc000000000177b80 calls spl7 [0xc00000000003b3bc] with a0=[[0xa800000000001020]:8], a1=a1 at entry (arg1), a2=0xfffffffffeeddead, a3=[[0xa800000000001020]:8+80]:8, a4=[a0+148], a6=0x10000000.
  The `jalr` at 0xc000000000177b94 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+80]:8.
  The `bgez` branch at 0xc000000000177938 targets 0xc000000000177a34, is taken when at(=([a0+148] << 0x23)) >= 0 (signed), and falls through when at(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc000000000177944 targets 0xc000000000177988, is taken when v0(=[0xc0000000014d3558]:8) != a0, and falls through when v0(=[0xc0000000014d3558]:8) == a0.
  The `bne` branch at 0xc000000000177960 targets 0xc000000000177988, is taken when v1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc000000000177968 targets 0xc000000000177988, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000177974 targets 0xc000000000177988, is taken when a0(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a0(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000177980 targets 0xc000000000177b80, is taken when a3(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a3(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bltz` branch at 0xc0000000001779b0 targets 0xc0000000001779c0, is taken when t0(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t0(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `beq` branch at 0xc0000000001779b8 targets 0xc000000000177bb4 and is always taken.
  The `bne` branch at 0xc0000000001779d8 targets 0xc000000000177a2c, is taken when a7(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a7(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc0000000001779e4 targets 0xc000000000177a2c, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000001779ec targets 0xc000000000177a2c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000001779f8 targets 0xc000000000177a2c, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000177a04 targets 0xc000000000177a2c, is taken when a4(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a4(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc000000000177a2c targets 0xc000000000177b4c, is taken when s1(=one of 0x0, 0x1) == 0, and falls through when s1(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc000000000177a38 targets 0xc000000000177b3c, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc000000000177a50 targets 0xc000000000177b00, is taken when (a6(=0x10000000) & a4(=[a0+148])) == 0, and falls through when (a6(=0x10000000) & a4(=[a0+148])) != 0.
  The `bne` branch at 0xc000000000177a58 targets 0xc000000000177a9c, is taken when t1(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when t1(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000177a74 targets 0xc000000000177a9c, is taken when t2(=[0xa800000000001000]:8) != t3(=0x53435241), and falls through when t2(=[0xa800000000001000]:8) == t3(=0x53435241).
  The `beq` branch at 0xc000000000177a7c targets 0xc000000000177a9c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000177a88 targets 0xc000000000177a9c, is taken when t3(=[[0xa800000000001020]:8]) != t8(=0xfffffffffeeddead), and falls through when t3(=[[0xa800000000001020]:8]) == t8(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000177a94 targets 0xc000000000177b54, is taken when t9(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when t9(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc000000000177aa8 targets 0xc000000000177b04, is taken when ra(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when ra(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000177ac0 targets 0xc000000000177b04, is taken when v0(=[0xa800000000001000]:8) != v1(=0x53435241), and falls through when v0(=[0xa800000000001000]:8) == v1(=0x53435241).
  The `beq` branch at 0xc000000000177ad0 targets 0xc000000000177b00, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000177adc targets 0xc000000000177b00, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000177ae8 targets 0xc000000000177b00, is taken when a4(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a4(=[[0xa800000000001020]:8+80]:8) != 0.
  The `bne` branch at 0xc000000000177b24 targets 0xc0000000001779c0, is taken when (a2(=[[a0+64]:8]) & a7(=0x40000000)) != 0, and falls through when (a2(=[[a0+64]:8]) & a7(=0x40000000)) == 0.
  The `beq` branch at 0xc000000000177b2c targets 0xc000000000177b18, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0xc000000000177b34 targets 0xc0000000001779c8 and is always taken.
  The `beq` branch at 0xc000000000177b44 targets 0xc000000000177a48 and is always taken.
  The `beq` branch at 0xc000000000177b4c targets 0xc000000000177b04 and is always taken.
  The `beq` branch at 0xc000000000177b78 targets 0xc000000000177a9c and is always taken.
  The `beq` branch at 0xc000000000177bac targets 0xc000000000177988 and is always taken.
  The `beq` branch at 0xc000000000177bb8 targets 0xc000000000177b2c and is always taken.
  The return at 0xc000000000177b10 has v0 = one of -0x1, 0x0.
- `ioc3_enable_tx` in `6.5.30/unix.IP27` at 0xc000000000177bc0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000177bc0-0xc000000000177dd0 (528 bytes).
  It occupies 528 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xc000000000177bcc, `lw` at 0xc000000000177c98.
  The `ld` at 0xc000000000177bd4 loads 8 bytes from a0 (arg0 at entry) +0x70.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000177c00, `ld` at 0xc000000000177ce0.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000177c04, `ld` at 0xc000000000177ce4, `ld` at 0xc000000000177dac.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000177c18, `lw` at 0xc000000000177d08.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc000000000177c24, `ld` at 0xc000000000177d14, `ld` at 0xc000000000177db0.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc000000000177c30, `lw` at 0xc000000000177cbc.
  4 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc000000000177c3c, `ld` at 0xc000000000177c4c, `ld` at 0xc000000000177cd4.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc000000000177c48, `sw` at 0xc000000000177cd8.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc000000000177c50, `lw` at 0xc000000000177c80.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x98, including `lw` at 0xc000000000177c9c, `lw` at 0xc000000000177d78.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xc000000000177ca8, `sw` at 0xc000000000177d60.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x4, including `sw` at 0xc000000000177cac, `sw` at 0xc000000000177d94.
  2 such `lw` accesses load 4 bytes from [a0+112]:8 +0x8, including `lw` at 0xc000000000177cb8, `lw` at 0xc000000000177d68.
  The `lw` at 0xc000000000177d58 loads 4 bytes from [a0+64]:8 +0x8.
  The `jal` at 0xc000000000177cb4 calls enable_intrs [0xc000000000175e60] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=?, a3=?, a4=[a0+64]:8.
  The `jalr` at 0xc000000000177d20 calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8+80]:8, a2=[0xc0000000014d3558]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=0xa800000000001000.
  The `jal` at 0xc000000000177d28 calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000177d70 calls disable_intrs [0xc000000000175eb0] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=?, a3=?, a4=[a0+64]:8, a6=([a0+120] | 0x10).
  The `jal` at 0xc000000000177da0 calls spl7 [0xc00000000003b3bc] with a0=0x53435241, a1=a1 at entry (arg1), a2=[[0xa800000000001020]:8], a3=0xfffffffffeeddead, a4=[[0xa800000000001020]:8+80]:8, a5=0xa800000000001020, a6=[0xc0000000014d3558]:8.
  The `jalr` at 0xc000000000177db8 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `beq` branch at 0xc000000000177bdc targets 0xc000000000177d48, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc000000000177be4 targets 0xc000000000177d50, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `bne` branch at 0xc000000000177bfc targets 0xc000000000177c30, is taken when a6(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a6(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000177c08 targets 0xc000000000177c30, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc000000000177c10 targets 0xc000000000177c30, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000177c1c targets 0xc000000000177c30, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000177c28 targets 0xc000000000177da0, is taken when a4(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a4(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bgez` branch at 0xc000000000177c38 targets 0xc000000000177c94, is taken when a5(=([a0+148] << 0x23)) >= 0 (signed), and falls through when a5(=([a0+148] << 0x23)) < 0 (signed).
  The `bltz` branch at 0xc000000000177c68 targets 0xc000000000177c94, is taken when t0(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t0(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `bne` branch at 0xc000000000177c78 targets 0xc000000000177d98, is taken when t2(?) != 0, and falls through when t2(?) == 0.
  The `beq` branch at 0xc000000000177c8c targets 0xc000000000177c78, is taken when (t3(=[[a0+64]:8]) & a6(=0x40000000)) == 0, and falls through when (t3(=[[a0+64]:8]) & a6(=0x40000000)) != 0.
  The `beq` branch at 0xc000000000177c94 targets 0xc000000000177d58, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc000000000177ccc targets 0xc000000000177cdc, is taken when (ra(=0x10000000) & a0(=[a0+148])) == 0, and falls through when (ra(=0x10000000) & a0(=[a0+148])) != 0.
  The `bne` branch at 0xc000000000177cf0 targets 0xc000000000177d30, is taken when a2(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a2(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000177cf8 targets 0xc000000000177d30, is taken when a1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc000000000177d00 targets 0xc000000000177d30, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000177d0c targets 0xc000000000177d30, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000177d18 targets 0xc000000000177d30, is taken when a1(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a1(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc000000000177d48 targets 0xc000000000177bec, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc000000000177d50 targets 0xc000000000177d34 and is always taken.
  The `beq` branch at 0xc000000000177d84 targets 0xc000000000177cbc, is taken when a7(=[a0+152]) == v0(?), and falls through when a7(=[a0+152]) != v0(?).
  The `beq` branch at 0xc000000000177d90 targets 0xc000000000177cbc and is always taken.
  The `beq` branch at 0xc000000000177d98 targets 0xc000000000177d38 and is always taken.
  The `beq` branch at 0xc000000000177dc8 targets 0xc000000000177c30 and is always taken.
  The return at 0xc000000000177d40 has v0 = one of -0x1, 0x0.
- `ioc3_read` in `6.5.30/unix.IP27` at 0xc000000000177dd0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000177dd0-0xc000000000178500 (1840 bytes).
  It occupies 1840 bytes.
  The `ld` at 0xc000000000177ddc loads 8 bytes from a0 (arg0 at entry) +0x70.
  11 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc000000000177df4, `ld` at 0xc000000000177e0c, `ld` at 0xc000000000177f00.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x9c, including `lw` at 0xc000000000177df8, `lw` at 0xc000000000177e18.
  3 such `sw` accesses store 4 bytes to [a0+64]:8 +0x10, including `sw` at 0xc000000000177e08, `sw` at 0xc0000000001781ac, `sw` at 0xc0000000001784ac.
  4 such `lw` accesses load 4 bytes from [a0+64]:8 +0xc, including `lw` at 0xc000000000177e14, `lw` at 0xc000000000177f04, `lw` at 0xc000000000177fd0.
  5 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc000000000177e38, `lw` at 0xc000000000177fe0, `lw` at 0xc0000000001781c0.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000177e5c, `ld` at 0xc000000000177eb4, `ld` at 0xc000000000177f1c.
  9 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000177e74, `ld` at 0xc000000000177ec8, `ld` at 0xc000000000177f30.
  6 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000177e80, `lw` at 0xc000000000177ed4, `lw` at 0xc000000000177f3c.
  9 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc000000000177e90, `ld` at 0xc000000000177ee4, `ld` at 0xc000000000177f48.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc000000000177ea4, `sw` at 0xc000000000178038.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc000000000177f54, `lw` at 0xc00000000017832c.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xc000000000177fec, `sw` at 0xc0000000001783c0.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xc000000000178094, `lw` at 0xc00000000017810c, `lw` at 0xc0000000001781b0.
  The `ld` at 0xc000000000178098 loads 8 bytes from a0 (arg0 at entry) +0x60.
  4 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xc0000000001780a8, `sw` at 0xc000000000178358, `sw` at 0xc0000000001783b0.
  6 such `lw` accesses load 4 bytes from ?, including `lw` at 0xc0000000001780cc, `lw` at 0xc00000000017818c, `lw` at 0xc000000000178304.
  7 such `lbu` accesses load 1 byte from ?, including `lbu` at 0xc0000000001780e8, `lbu` at 0xc000000000178150, `lbu` at 0xc000000000178244.
  5 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x8c, including `lw` at 0xc0000000001780fc, `lw` at 0xc000000000178174, `lw` at 0xc000000000178254.
  5 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x8, including `ld` at 0xc000000000178134, `ld` at 0xc000000000178228, `ld` at 0xc000000000178264.
  4 such `ld` accesses load 8 bytes from [a0+8]:8 +0x10, including `ld` at 0xc000000000178140, `ld` at 0xc000000000178234, `ld` at 0xc000000000178270.
  4 such `sb` accesses store 1 byte to ?, including `sb` at 0xc00000000017815c, `sb` at 0xc0000000001782d4, `sb` at 0xc0000000001782e8.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x9c, including `sw` at 0xc0000000001781bc, `sw` at 0xc0000000001784bc.
  The `lw` at 0xc000000000178364 loads 4 bytes from [a0+112]:8 +0x10.
  The `ld` at 0xc0000000001784cc loads 8 bytes from [a0+8]:8 +0x18.
  The `jalr` at 0xc000000000177ef0 calls (unknown callee) with a0=0x1, a1=?, a2=[[0xa800000000001020]:8+80]:8, a3=?, a5=[a0+64]:8, a7=[0xc0000000014d3558]:8.
  The `jal` at 0xc000000000177ef8 calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000177fb4 calls (unknown callee) with a0=0x1, a1=[0xa800000000001000]:8, a2=[[0xa800000000001020]:8+80]:8, a3=?, a5=[a0+64]:8.
  The `jal` at 0xc000000000177fbc calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000178084 calls (unknown callee) with a0=0x1, a1=?, a2=[[0xa800000000001020]:8+80]:8, a3=?, a5=[a0+64]:8, a7=[0xc0000000014d3558]:8.
  The `jal` at 0xc00000000017808c calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000178148 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x8, a2=[a0+140], a3=?.
  The `jalr` at 0xc00000000017823c calls (unknown callee) with a0=a0 at entry (arg0), a1=0x2, a2=[a0+140], a3=?, a7=[[a0+8]:8+16]:8.
  The `jalr` at 0xc000000000178278 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x4, a2=?, a3=?.
  The `jalr` at 0xc0000000001782b4 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `jal` at 0xc000000000178360 calls enable_intrs [0xc000000000175e60] with a0=a0 at entry (arg0), a1=[[a0+112]:8+16], a2=?, a3=?.
  The `jal` at 0xc0000000001783fc calls spl7 [0xc00000000003b3bc] with a0=?, a1=[0xc0000000014d3558]:8, a2=[0xa800000000001000]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[a0+64]:8, a6=[[0xa800000000001020]:8+80]:8.
  The `jalr` at 0xc000000000178410 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a6=[[0xa800000000001020]:8+80]:8.
  The `jal` at 0xc000000000178428 calls spl7 [0xc00000000003b3bc] with a0=?, a1=0xffffffffefffffff, a2=[0xa800000000001000]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[a0+64]:8, a6=[[0xa800000000001020]:8+80]:8.
  The `jalr` at 0xc00000000017843c calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+80]:8.
  The `jal` at 0xc000000000178454 calls spl7 [0xc00000000003b3bc] with a0=?, a1=?, a2=[0xc0000000014d3558]:8, a3=?, a5=[a0+64]:8.
  The `jalr` at 0xc000000000178468 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `jalr` at 0xc0000000001784d4 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x0, a2=[a0+140], a3=?.
  The `bne` branch at 0xc000000000177e30 targets 0xc000000000178094, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bgez` branch at 0xc000000000177e40 targets 0xc0000000001783b4, is taken when v1(=([a0+148] << 0x23)) >= 0 (signed), and falls through when v1(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc000000000177e50 targets 0xc000000000177ea0, is taken when a1(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a1(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000177e68 targets 0xc000000000177ea0, is taken when a2(=[0xa800000000001000]:8) != a3(=0x53435241), and falls through when a2(=[0xa800000000001000]:8) == a3(=0x53435241).
  The `beq` branch at 0xc000000000177e78 targets 0xc000000000177e9c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000177e88 targets 0xc000000000177e9c, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000177e94 targets 0xc0000000001783fc, is taken when a6(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc000000000177eac targets 0xc000000000177f00, is taken when a7(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a7(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000177ec0 targets 0xc000000000177f00, is taken when t1(=[0xa800000000001000]:8) != t2(=0x53435241), and falls through when t1(=[0xa800000000001000]:8) == t2(=0x53435241).
  The `beq` branch at 0xc000000000177ecc targets 0xc000000000177f00, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000177edc targets 0xc000000000177f00, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000177ee8 targets 0xc000000000177f00, is taken when a2(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+80]:8) != 0.
  The `bne` branch at 0xc000000000177f0c targets 0xc000000000177fd8, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bne` branch at 0xc000000000177f14 targets 0xc000000000177f54, is taken when s0(=a0) != a2(=[0xc0000000014d3558]:8), and falls through when s0(=a0) == a2(=[0xc0000000014d3558]:8).
  The `bne` branch at 0xc000000000177f28 targets 0xc000000000177f54, is taken when t8(=[0xa800000000001000]:8) != t9(=0x53435241), and falls through when t8(=[0xa800000000001000]:8) == t9(=0x53435241).
  The `beq` branch at 0xc000000000177f34 targets 0xc000000000177f54, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000177f40 targets 0xc000000000177f54, is taken when t9(=[[0xa800000000001020]:8]) != ra(=0xfffffffffeeddead), and falls through when t9(=[[0xa800000000001020]:8]) == ra(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000177f4c targets 0xc000000000178454, is taken when at(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when at(=[[0xa800000000001020]:8+80]:8) == 0.
  The `beq` branch at 0xc000000000177f5c targets 0xc000000000177f70, is taken when (v0(=[[a0+64]:8]) & s2(=0x8000000)) == 0, and falls through when (v0(=[[a0+64]:8]) & s2(=0x8000000)) != 0.
  The `beq` branch at 0xc000000000177f68 targets 0xc0000000001784f0 and is always taken.
  The `bne` branch at 0xc000000000177f74 targets 0xc000000000177fc4, is taken when s0(=a0) != a2(=[0xc0000000014d3558]:8), and falls through when s0(=a0) == a2(=[0xc0000000014d3558]:8).
  The `bne` branch at 0xc000000000177f88 targets 0xc000000000177fc4, is taken when a1(=[0xa800000000001000]:8) != a2(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a2(=0x53435241).
  The `beq` branch at 0xc000000000177f94 targets 0xc000000000177fc4, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000177fa0 targets 0xc000000000177fc4, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000177fac targets 0xc000000000177fc4, is taken when a2(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc000000000177fc4 targets 0xc0000000001783f4, is taken when s2(=one of 0x0, 0x1) == 0, and falls through when s2(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc000000000177fd8 targets 0xc000000000178094, is taken when s3(=one of 0x0, 0x1) == 0, and falls through when s3(=one of 0x0, 0x1) != 0.
  The `bne` branch at 0xc000000000177ff4 targets 0xc000000000178038, is taken when t8(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when t8(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000178008 targets 0xc000000000178038, is taken when a2(=[0xa800000000001000]:8) != a3(=0x53435241), and falls through when a2(=[0xa800000000001000]:8) == a3(=0x53435241).
  The `beq` branch at 0xc000000000178014 targets 0xc000000000178038, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000178024 targets 0xc000000000178038, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000178030 targets 0xc000000000178428, is taken when a6(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc000000000178040 targets 0xc000000000178094, is taken when a7(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a7(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000178054 targets 0xc000000000178094, is taken when t0(=[0xa800000000001000]:8) != t1(=0x53435241), and falls through when t0(=[0xa800000000001000]:8) == t1(=0x53435241).
  The `beq` branch at 0xc000000000178060 targets 0xc000000000178094, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000178070 targets 0xc000000000178094, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000017807c targets 0xc000000000178094, is taken when a2(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc0000000001780a4 targets 0xc0000000001781a4, is taken when s5(?) == s1(=[a0+156]), and falls through when s5(?) != s1(=[a0+156]).
  The `blez` branch at 0xc0000000001780ac targets 0xc0000000001783cc, is taken when s6(=a2) <= 0 (signed), and falls through when s6(=a2) > 0 (signed).
  The `beq` branch at 0xc0000000001780d4 targets 0xc000000000178370, is taken when (s4(=0xffffffffc0c0c0c0) & t8(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & t8(?)) != 0.
  The `blez` branch at 0xc0000000001780dc targets 0xc000000000178304, is taken when s6(?) <= 0 (signed), and falls through when s6(?) > 0 (signed).
  The `beq` branch at 0xc0000000001780f4 targets 0xc000000000178160, is taken when (a0(?) & 0x40) == 0, and falls through when (a0(?) & 0x40) != 0.
  The `beq` branch at 0xc000000000178104 targets 0xc000000000178124, is taken when (a2(=[a0+140]) & 0x40) == 0, and falls through when (a2(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xc000000000178114 targets 0xc000000000178124, is taken when (v0(=[a0+120]) & 0x2) == 0, and falls through when (v0(=[a0+120]) & 0x2) != 0.
  The `beq` branch at 0xc00000000017811c targets 0xc0000000001783d4, is taken when a6(?) == 0, and falls through when a6(?) != 0.
  The `beq` branch at 0xc000000000178124 targets 0xc000000000178158, is taken when a5(?) == 0, and falls through when a5(?) != 0.
  The `beq` branch at 0xc00000000017812c targets 0xc000000000178158, is taken when a7(?) == 0, and falls through when a7(?) != 0.
  The `beq` branch at 0xc000000000178138 targets 0xc000000000178154, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc000000000178164 targets 0xc0000000001782ec, is taken when (a0(?) & 0x80) == 0, and falls through when (a0(?) & 0x80) != 0.
  The `beq` branch at 0xc00000000017816c targets 0xc0000000001782c4, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc00000000017817c targets 0xc0000000001782c4, is taken when (a2(=[a0+140]) & 0x1c) == 0, and falls through when (a2(=[a0+140]) & 0x1c) != 0.
  The `blez` branch at 0xc000000000178184 targets 0xc000000000178214, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xc00000000017819c targets 0xc0000000001781a8 and is always taken.
  The `beq` branch at 0xc0000000001781b8 targets 0xc0000000001781e0, is taken when (v0(=[a0+120]) & 0x1) == 0, and falls through when (v0(=[a0+120]) & 0x1) != 0.
  The `bne` branch at 0xc0000000001781d8 targets 0xc000000000178350, is taken when a4(?) < a5(?) (unsigned), and falls through when a4(?) >= a5(?) (unsigned).
  The `beq` branch at 0xc000000000178214 targets 0xc00000000017824c, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc000000000178220 targets 0xc00000000017824c, is taken when (a2(=[a0+140]) & 0x8) == 0, and falls through when (a2(=[a0+140]) & 0x8) != 0.
  The `beq` branch at 0xc00000000017822c targets 0xc00000000017824c, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc00000000017824c targets 0xc000000000178288, is taken when t0(?) == 0, and falls through when t0(?) != 0.
  The `beq` branch at 0xc00000000017825c targets 0xc000000000178288, is taken when (t1(=[a0+140]) & 0x10) == 0, and falls through when (t1(=[a0+140]) & 0x10) != 0.
  The `beq` branch at 0xc000000000178268 targets 0xc000000000178288, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc000000000178288 targets 0xc0000000001782c4, is taken when t3(?) == 0, and falls through when t3(?) != 0.
  The `beq` branch at 0xc000000000178298 targets 0xc0000000001782c4, is taken when (t8(=[a0+140]) & 0x4) == 0, and falls through when (t8(=[a0+140]) & 0x4) != 0.
  The `beq` branch at 0xc0000000001782a4 targets 0xc0000000001782c4, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc0000000001782f4 targets 0xc000000000178304, is taken when s8(?) >= 0x4 (signed), and falls through when s8(?) < 0x4 (signed).
  The `bgtz` branch at 0xc0000000001782fc targets 0xc0000000001780e8, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xc00000000017830c targets 0xc0000000001783c4, is taken when (s4(=0xffffffffc0c0c0c0) & a0(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a0(?)) != 0.
  The `beq` branch at 0xc000000000178314 targets 0xc0000000001781a8, is taken when s5(?) == s1(?), and falls through when s5(?) != s1(?).
  The `bgtz` branch at 0xc00000000017831c targets 0xc0000000001780c4, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xc000000000178324 targets 0xc0000000001781ac and is always taken.
  The `beq` branch at 0xc000000000178338 targets 0xc000000000177f70, is taken when (a1(=[[a0+64]:8]) & s2(=0x8000000)) == 0, and falls through when (a1(=[[a0+64]:8]) & s2(=0x8000000)) != 0.
  The `beq` branch at 0xc000000000178340 targets 0xc00000000017832c, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0xc000000000178348 targets 0xc000000000177f74 and is always taken.
  The `beq` branch at 0xc000000000178368 targets 0xc0000000001781e0 and is always taken.
  The `beq` branch at 0xc00000000017838c targets 0xc0000000001783a4, is taken when v0(?) == a0(?), and falls through when v0(?) != a0(?).
  The `bne` branch at 0xc00000000017839c targets 0xc0000000001780dc, is taken when (s4(=0xffffffffc0c0c0c0) & a3(?)) != 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a3(?)) == 0.
  The `beq` branch at 0xc0000000001783ac targets 0xc0000000001781ac and is always taken.
  The `beq` branch at 0xc0000000001783bc targets 0xc000000000177e4c and is always taken.
  The `beq` branch at 0xc0000000001783c4 targets 0xc000000000178314 and is always taken.
  The `beq` branch at 0xc0000000001783cc targets 0xc0000000001781a8 and is always taken.
  The `blez` branch at 0xc0000000001783d4 targets 0xc000000000178480, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xc0000000001783ec targets 0xc0000000001781a8 and is always taken.
  The `beq` branch at 0xc0000000001783f4 targets 0xc0000000001781e0 and is always taken.
  The `beq` branch at 0xc000000000178420 targets 0xc000000000177e9c and is always taken.
  The `beq` branch at 0xc00000000017844c targets 0xc000000000178038 and is always taken.
  The `beq` branch at 0xc000000000178478 targets 0xc000000000177f54 and is always taken.
  The `beq` branch at 0xc0000000001784a0 targets 0xc0000000001784e4, is taken when (s4(=0xffffffffc0c0c0c0) & t1(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & t1(?)) != 0.
  The `beq` branch at 0xc0000000001784b8 targets 0xc0000000001784dc, is taken when (t9(=[a0+140]) & 0x40) == 0, and falls through when (t9(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xc0000000001784c4 targets 0xc0000000001784dc, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc0000000001784dc targets 0xc0000000001781e0 and is always taken.
  The `beq` branch at 0xc0000000001784e8 targets 0xc0000000001784a8 and is always taken.
  The `beq` branch at 0xc0000000001784f8 targets 0xc000000000178340 and is always taken.
  The return at 0xc00000000017820c has v0 = one of ?, -0x1, 0x0.
- `ioc3_set_DTR` in `6.5.30/unix.IP27` at 0xc000000000178a8c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000178a8c-0xc000000000178aac (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xc000000000178a98 calls set_DTRRTS [0xc0000000001788b8] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x1, a3=0x1000000.
  The return at 0xc000000000178aa4 has v0 = ret(set_DTRRTS).
- `ioc3_set_RTS` in `6.5.30/unix.IP27` at 0xc000000000178aac (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000178aac-0xc000000000178acc (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xc000000000178ab8 calls set_DTRRTS [0xc0000000001788b8] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x2, a3=0x2000000.
  The return at 0xc000000000178ac4 has v0 = ret(set_DTRRTS).
- `ioc3_query_DCD` in `6.5.30/unix.IP27` at 0xc000000000178acc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000178acc-0xc000000000178bd0 (260 bytes).
  It occupies 260 bytes.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000178ae8, `ld` at 0xc000000000178b38.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000178aec, `ld` at 0xc000000000178b40, `ld` at 0xc000000000178ba4.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000178b00, `lw` at 0xc000000000178b4c.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc000000000178b0c, `ld` at 0xc000000000178b58, `ld` at 0xc000000000178ba8.
  The `ld` at 0xc000000000178b1c loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xc000000000178b30 loads 4 bytes from [a0+64]:8 +0x18.
  The `jalr` at 0xc000000000178b70 calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=[[0xa800000000001020]:8+80]:8, a4=[[a0+64]:8+24], a5=0xa800000000001020, a6=[0xa800000000001000]:8, a7=[[0xa800000000001020]:8].
  The `jal` at 0xc000000000178b78 calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000178b98 calls spl7 [0xc00000000003b3bc] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=[0xc0000000014d3558]:8, a4=0x53435241, a5=[[0xa800000000001020]:8], a6=0xfffffffffeeddead, a7=[[0xa800000000001020]:8+80]:8.
  The `jalr` at 0xc000000000178bb0 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc000000000178ae4 targets 0xc000000000178b1c, is taken when a0 != a3(=[0xc0000000014d3558]:8), and falls through when a0 == a3(=[0xc0000000014d3558]:8).
  The `bne` branch at 0xc000000000178af0 targets 0xc000000000178b1c, is taken when a5(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc000000000178af8 targets 0xc000000000178b1c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000178b04 targets 0xc000000000178b1c, is taken when a5(=[[0xa800000000001020]:8]) != a6(=0xfffffffffeeddead), and falls through when a5(=[[0xa800000000001020]:8]) == a6(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000178b14 targets 0xc000000000178b98, is taken when a7(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a7(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc000000000178b2c targets 0xc000000000178b88, is taken when a0 != a3(=[0xc0000000014d3558]:8), and falls through when a0 == a3(=[0xc0000000014d3558]:8).
  The `bne` branch at 0xc000000000178b3c targets 0xc000000000178b88, is taken when a6(=[0xa800000000001000]:8) != a7(=0x53435241), and falls through when a6(=[0xa800000000001000]:8) == a7(=0x53435241).
  The `beq` branch at 0xc000000000178b44 targets 0xc000000000178b88, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000178b50 targets 0xc000000000178b88, is taken when a7(=[[0xa800000000001020]:8]) != t0(=0xfffffffffeeddead), and falls through when a7(=[[0xa800000000001020]:8]) == t0(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000178b60 targets 0xc000000000178b88, is taken when a3(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc000000000178bc8 targets 0xc000000000178b1c and is always taken.
  The return at 0xc000000000178b90 has v0 = ?.
- `ioc3_query_CTS` in `6.5.30/unix.IP27` at 0xc000000000178bd0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000178bd0-0xc000000000178cd4 (260 bytes).
  It occupies 260 bytes.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000178bec, `ld` at 0xc000000000178c3c.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000178bf0, `ld` at 0xc000000000178c44, `ld` at 0xc000000000178ca8.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000178c04, `lw` at 0xc000000000178c50.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc000000000178c10, `ld` at 0xc000000000178c5c, `ld` at 0xc000000000178cac.
  The `ld` at 0xc000000000178c20 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xc000000000178c34 loads 4 bytes from [a0+64]:8 +0x18.
  The `jalr` at 0xc000000000178c74 calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=[[0xa800000000001020]:8+80]:8, a4=[[a0+64]:8+24], a5=0xa800000000001020, a6=[0xa800000000001000]:8, a7=[[0xa800000000001020]:8].
  The `jal` at 0xc000000000178c7c calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000178c9c calls spl7 [0xc00000000003b3bc] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=[0xc0000000014d3558]:8, a4=0x53435241, a5=[[0xa800000000001020]:8], a6=0xfffffffffeeddead, a7=[[0xa800000000001020]:8+80]:8.
  The `jalr` at 0xc000000000178cb4 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc000000000178be8 targets 0xc000000000178c20, is taken when a0 != a3(=[0xc0000000014d3558]:8), and falls through when a0 == a3(=[0xc0000000014d3558]:8).
  The `bne` branch at 0xc000000000178bf4 targets 0xc000000000178c20, is taken when a5(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc000000000178bfc targets 0xc000000000178c20, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000178c08 targets 0xc000000000178c20, is taken when a5(=[[0xa800000000001020]:8]) != a6(=0xfffffffffeeddead), and falls through when a5(=[[0xa800000000001020]:8]) == a6(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000178c18 targets 0xc000000000178c9c, is taken when a7(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a7(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc000000000178c30 targets 0xc000000000178c8c, is taken when a0 != a3(=[0xc0000000014d3558]:8), and falls through when a0 == a3(=[0xc0000000014d3558]:8).
  The `bne` branch at 0xc000000000178c40 targets 0xc000000000178c8c, is taken when a6(=[0xa800000000001000]:8) != a7(=0x53435241), and falls through when a6(=[0xa800000000001000]:8) == a7(=0x53435241).
  The `beq` branch at 0xc000000000178c48 targets 0xc000000000178c8c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000178c54 targets 0xc000000000178c8c, is taken when a7(=[[0xa800000000001020]:8]) != t0(=0xfffffffffeeddead), and falls through when a7(=[[0xa800000000001020]:8]) == t0(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000178c64 targets 0xc000000000178c8c, is taken when a3(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc000000000178ccc targets 0xc000000000178c20 and is always taken.
  The return at 0xc000000000178c94 has v0 = ?.
- `ioc3_get_mapid` in `6.5.30/unix.IP27` at 0xc000000000178d28 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000178d28-0xc000000000178d88 (96 bytes).
  It occupies 96 bytes.
  The `ld` at 0xc000000000178d3c loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `jal` at 0xc000000000178d60 calls copyout [0xc00000000001fa1c] with a0=sp-16, a1=a1 at entry (arg1), a2=0x8, a3=0xc000000001463a48.
  The `beq` branch at 0xc000000000178d30 targets 0xc000000000178d78, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc000000000178d70 targets 0xc000000000178d7c and is always taken.
  The return at 0xc000000000178d80 has v0 = one of 0x0, 0xe.
- `ioc3_map` in `6.5.30/unix.IP27` at 0xc000000000178d88 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000178d88-0xc000000000178e18 (144 bytes).
  It occupies 144 bytes.
  The `ld` at 0xc000000000178db4 loads 8 bytes from a0 (arg0 at entry) +0x58.
  The `ld` at 0xc000000000178de8 loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `ld` at 0xc000000000178df8 loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `jal` at 0xc000000000178dbc calls disable_intrs [0xc000000000175eb0] with a0=a0 at entry (arg0), a1=-0x1, a2=?, a3=?, a5=0x16.
  The `jal` at 0xc000000000178dcc calls v_mapphys [0xc0000000001efdec] with a0=a1 at entry (arg1), a1=?, a2=0x4000, a3=?.
  The `beq` branch at 0xc000000000178d90 targets 0xc000000000178dac, is taken when a2 == 0, and falls through when a2 != 0.
  The `beq` branch at 0xc000000000178d9c targets 0xc000000000178de8, is taken when a2 == at(=0x4000), and falls through when a2 != at(=0x4000).
  The `beq` branch at 0xc000000000178da4 targets 0xc000000000178de0 and is always taken.
  The `beq` branch at 0xc000000000178e10 targets 0xc000000000178dbc and is always taken.
  The return at 0xc000000000178de0 has v0 = one of 0x16, ret(v_mapphys).
- `ioc3_unmap` in `6.5.30/unix.IP27` at 0xc000000000178e18 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000178e18-0xc000000000178f54 (316 bytes).
  It occupies 316 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc000000000178e20, `lw` at 0xc000000000178f44.
  The `sw` at 0xc000000000178e34 stores 4 bytes to a0 (arg0 at entry) +0x94.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000178e44, `ld` at 0xc000000000178ea4.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000178e4c, `ld` at 0xc000000000178e98, `ld` at 0xc000000000178f2c.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000178e68, `lw` at 0xc000000000178eb8.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc000000000178e74, `ld` at 0xc000000000178ec4, `ld` at 0xc000000000178f30.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc000000000178e80, `ld` at 0xc000000000178eec, `ld` at 0xc000000000178f00.
  The `sw` at 0xc000000000178e88 stores 4 bytes to [a0+64]:8 +0x0.
  The `lw` at 0xc000000000178ef0 loads 4 bytes from [a0+64]:8 +0x8.
  The `sw` at 0xc000000000178ef8 stores 4 bytes to a0 (arg0 at entry) +0x98.
  The `sw` at 0xc000000000178efc stores 4 bytes to [a0+64]:8 +0x4.
  The `lw` at 0xc000000000178f04 loads 4 bytes from [a0+64]:8 +0xc.
  The `sw` at 0xc000000000178f0c stores 4 bytes to a0 (arg0 at entry) +0x9c.
  The `sw` at 0xc000000000178f10 stores 4 bytes to [a0+64]:8 +0x10.
  The `jalr` at 0xc000000000178ed8 calls (unknown callee) with a0=0x1, a1=[a0+64]:8, a2=[[0xa800000000001020]:8], a3=[[0xa800000000001020]:8+80]:8, a4=[0xc0000000014d3554], a5=0xa800000000001020, a6=[0xc0000000014d3558]:8.
  The `jal` at 0xc000000000178ee0 calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000178f20 calls spl7 [0xc00000000003b3bc] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=?, a4=[0xa800000000001000]:8, a5=0x53435241, a6=[[0xa800000000001020]:8], a7=0xfffffffffeeddead.
  The `jalr` at 0xc000000000178f38 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+80]:8.
  The `bne` branch at 0xc000000000178e50 targets 0xc000000000178e80, is taken when at(=[0xc0000000014d3558]:8) != a0, and falls through when at(=[0xc0000000014d3558]:8) == a0.
  The `bne` branch at 0xc000000000178e58 targets 0xc000000000178e80, is taken when a4(=[0xa800000000001000]:8) != a5(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a5(=0x53435241).
  The `beq` branch at 0xc000000000178e60 targets 0xc000000000178e80, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000178e6c targets 0xc000000000178e80, is taken when a6(=[[0xa800000000001020]:8]) != a7(=0xfffffffffeeddead), and falls through when a6(=[[0xa800000000001020]:8]) == a7(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000178e78 targets 0xc000000000178f20, is taken when t0(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when t0(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc000000000178ea0 targets 0xc000000000178eec, is taken when a6(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a6(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000178ea8 targets 0xc000000000178eec, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc000000000178eb0 targets 0xc000000000178eec, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000178ebc targets 0xc000000000178eec, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000178ec8 targets 0xc000000000178eec, is taken when a3(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc000000000178f4c targets 0xc000000000178e80 and is always taken.
  The return at 0xc000000000178f18 has v0 = one of [0xa800000000001020]:8, ret(splx).
- `ioc3_set_sscr` in `6.5.30/unix.IP27` at 0xc000000000178f54 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000178f54-0xc000000000179078 (292 bytes).
  It occupies 292 bytes.
  3 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc000000000178f54, `lw` at 0xc000000000179028, `lw` at 0xc000000000179068.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xc000000000178f70, `sw` at 0xc000000000179040.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000178f8c, `ld` at 0xc000000000178fe0.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000178f90, `ld` at 0xc000000000178fd4, `ld` at 0xc000000000179050.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000178fa4, `lw` at 0xc000000000178ff4.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x50, including `ld` at 0xc000000000178fb0, `ld` at 0xc000000000179000, `ld` at 0xc000000000179054.
  The `ld` at 0xc000000000178fbc loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `sw` at 0xc000000000178fc4 stores 4 bytes to [a0+64]:8 +0x0.
  The `jalr` at 0xc000000000179014 calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8+80]:8, a2=[0xc0000000014d3558]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[0xc0000000014d3554].
  The `jal` at 0xc00000000017901c calls splx [0xc00000000003b3f4] with a0=[0xc0000000014d3554], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000179044 calls spl7 [0xc00000000003b3bc] with a0=0x53435241, a1=[[0xa800000000001020]:8], a2=0xfffffffffeeddead, a3=[[0xa800000000001020]:8+80]:8, a4=0xa800000000001020.
  The `jalr` at 0xc00000000017905c calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a6=[[0xa800000000001020]:8+80]:8.
  The `beq` branch at 0xc000000000178f64 targets 0xc000000000179038, is taken when a2 == 0, and falls through when a2 != 0.
  The `bne` branch at 0xc000000000178f88 targets 0xc000000000178fbc, is taken when a2(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a2(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000178f94 targets 0xc000000000178fbc, is taken when a1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc000000000178f9c targets 0xc000000000178fbc, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000178fa8 targets 0xc000000000178fbc, is taken when a1(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a1(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000178fb4 targets 0xc000000000179044, is taken when a3(=[[0xa800000000001020]:8+80]:8) != 0, and falls through when a3(=[[0xa800000000001020]:8+80]:8) == 0.
  The `bne` branch at 0xc000000000178fdc targets 0xc000000000179028, is taken when a2(=[0xc0000000014d3558]:8) != s0(=a0), and falls through when a2(=[0xc0000000014d3558]:8) == s0(=a0).
  The `bne` branch at 0xc000000000178fe4 targets 0xc000000000179028, is taken when a0(=[0xa800000000001000]:8) != a1(=0x53435241), and falls through when a0(=[0xa800000000001000]:8) == a1(=0x53435241).
  The `beq` branch at 0xc000000000178fec targets 0xc000000000179028, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000178ff8 targets 0xc000000000179028, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000179004 targets 0xc000000000179028, is taken when a1(=[[0xa800000000001020]:8+80]:8) == 0, and falls through when a1(=[[0xa800000000001020]:8+80]:8) != 0.
  The `beq` branch at 0xc00000000017903c targets 0xc000000000178f74 and is always taken.
  The `beq` branch at 0xc000000000179070 targets 0xc000000000178fbc and is always taken.
  The return at 0xc000000000179030 has v0 = [a0+148].
- `ioc3_console_write` in `6.5.30/unix.IP27` at 0xc000000000179078 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000179078-0xc0000000001791cc (340 bytes).
  It occupies 340 bytes.
  7 such `lw` accesses load 4 bytes from [0xc0000000014cb198]:8 +0xb8, including `lw` at 0xc00000000017907c, `lw` at 0xc000000000179158, `lw` at 0xc000000000179168.
  The `sd` at 0xc0000000001790a4 stores 8 bytes to a2 (arg2 at entry) +0x0.
  The `sb` at 0xc0000000001790cc stores 1 byte to [0xc0000000014cb198]:8 +0x20178.
  6 such `lbu` accesses load 1 byte from [0xc0000000014cb198]:8 +0x2017d, including `lbu` at 0xc0000000001790d0, `lbu` at 0xc0000000001790e8, `lbu` at 0xc0000000001790f8.
  The `lbu` at 0xc0000000001790dc loads 1 byte from ?.
  The `sw` at 0xc000000000179154 stores 4 bytes to [0xc0000000014cb198]:8 +0xb8.
  The `bne` branch at 0xc00000000017908c targets 0xc0000000001790a4, is taken when t0(=0x10000000) != t2(?), and falls through when t0(=0x10000000) == t2(?).
  The `beq` branch at 0xc00000000017909c targets 0xc0000000001791c0 and is always taken.
  The `bne` branch at 0xc0000000001790b8 targets 0xc0000000001790d0, is taken when a1 != 0, and falls through when a1 == 0.
  The `beq` branch at 0xc0000000001790c0 targets 0xc000000000179140 and is always taken.
  The `beq` branch at 0xc0000000001790c8 targets 0xc000000000179140, is taken when a1(?) == 0, and falls through when a1(?) != 0.
  The `bne` branch at 0xc0000000001790e0 targets 0xc0000000001790c8, is taken when (a6(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) != 0, and falls through when (a6(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc0000000001790f0 targets 0xc0000000001790c8, is taken when (a7(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) != 0, and falls through when (a7(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc000000000179100 targets 0xc0000000001790c8, is taken when (t0(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) != 0, and falls through when (t0(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc000000000179110 targets 0xc0000000001790c8, is taken when (t1(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) != 0, and falls through when (t1(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc000000000179120 targets 0xc0000000001790c8, is taken when (t2(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) != 0, and falls through when (t2(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) == 0.
  The `beq` branch at 0xc000000000179130 targets 0xc0000000001790e8, is taken when (t3(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) == 0, and falls through when (t3(=[[0xc0000000014cb198]:8+131453]:1) & 0x20) != 0.
  The `beq` branch at 0xc000000000179138 targets 0xc0000000001790c8 and is always taken.
  The `bne` branch at 0xc00000000017914c targets 0xc0000000001790a4, is taken when t0(=0x10000000) != t8(?), and falls through when t0(=0x10000000) == t8(?).
  The `bne` branch at 0xc000000000179160 targets 0xc00000000017914c, is taken when a5(=0x30000000) != t9(?), and falls through when a5(=0x30000000) == t9(?).
  The `bne` branch at 0xc000000000179170 targets 0xc000000000179148, is taken when a5(=0x30000000) != v0(?), and falls through when a5(=0x30000000) == v0(?).
  The `bne` branch at 0xc000000000179180 targets 0xc000000000179148, is taken when a5(=0x30000000) != v1(?), and falls through when a5(=0x30000000) == v1(?).
  The `bne` branch at 0xc000000000179190 targets 0xc000000000179148, is taken when a5(=0x30000000) != a3(?), and falls through when a5(=0x30000000) == a3(?).
  The `bne` branch at 0xc0000000001791a0 targets 0xc000000000179148, is taken when a5(=0x30000000) != t3(?), and falls through when a5(=0x30000000) == t3(?).
  The `beq` branch at 0xc0000000001791b0 targets 0xc000000000179168, is taken when a5(=0x30000000) == t8(?), and falls through when a5(=0x30000000) != t8(?).
  The `beq` branch at 0xc0000000001791b8 targets 0xc000000000179148 and is always taken.
  The `beq` branch at 0xc0000000001791c4 targets 0xc000000000179154 and is always taken.
  The return at 0xc000000000179140 has v0 = 0x0.
- `idbg_gdb` in `6.5.30/unix.IP27` at 0xc0000000001985d8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001985d8-0xc00000000019860c (52 bytes).
  It occupies 52 bytes.
  The `jal` at 0xc0000000001985e8 calls dogang [0xc0000000001981d8] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001985fc calls idbg_gang [0xc000000000198480] with a0=-0x1, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `beq` branch at 0xc0000000001985e0 targets 0xc0000000001985fc, is taken when a0 == at(=-0x1), and falls through when a0 != at(=-0x1).
  The `beq` branch at 0xc000000000198604 targets 0xc0000000001985f4 and is always taken.
  The return at 0xc0000000001985f4 has v0 = one of ret(dogang), ret(idbg_gang).
- `panicregs` in `6.5.30/unix.IP27` at 0xc0000000001a427c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001a427c-0xc0000000001a4330 (180 bytes).
  It occupies 180 bytes.
  The `ld` at 0xc0000000001a42cc loads 8 bytes from a0 (arg0 at entry) +0x120.
  The `ld` at 0xc0000000001a42f8 loads 8 bytes from a0 (arg0 at entry) +0x110.
  The `ld` at 0xc0000000001a42fc loads 8 bytes from table(0xc000000001463da0) +0x0.
  The `jal` at 0xc0000000001a4298 calls spl1 [0xc00000000003b30c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001a42a0 calls set_leds [0xc000000000043690] with a0=0x3, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001a42c0 calls machine_error_dump [0xc000000000200d08] with a0=0xc0000000003e17b8, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001a4318 calls cmn_err [0xc0000000001fcbd4] with a0=0x0, a1=0xc0000000003e17c8, a2=a4 at entry (arg4), a3=[a0+288]:8, a4=a0 at entry (arg0), a5=a1 at entry (arg1), a6=entry(0xc000000001463da0), a7=[a0+272]:8.
  The `bne` branch at 0xc0000000001a42b8 targets 0xc0000000001a42c8, is taken when v0(=[0xc0000000014cb7b0]) != 0, and falls through when v0(=[0xc0000000014cb7b0]) == 0.
  The return at 0xc0000000001a4328 has v0 = ret(cmn_err).
- `mem_range_init` in `6.5.30/unix.IP27` at 0xc0000000001acba8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001acba8-0xc0000000001acf1c (884 bytes).
  It occupies 884 bytes.
  6 such `ld` accesses load 8 bytes from abs 0xffffffffffffc400, including `ld` at 0xc0000000001acc38, `ld` at 0xc0000000001acc64, `ld` at 0xc0000000001acc84.
  5 such `ld` accesses load 8 bytes from [-0x3c00]:8 +0x0, including `ld` at 0xc0000000001acc3c, `ld` at 0xc0000000001acc68, `ld` at 0xc0000000001acc88.
  3 such `ld` accesses load 8 bytes from ((a0 << 0x3) + [[-0x3c00]:8]:8) +0x0, including `ld` at 0xc0000000001acc48, `ld` at 0xc0000000001acc70, `ld` at 0xc0000000001acc90.
  The `lw` at 0xc0000000001acc50 loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x614.
  The `sw` at 0xc0000000001acc60 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x614.
  The `lw` at 0xc0000000001acc74 loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x624.
  The `sw` at 0xc0000000001acc80 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x624.
  The `lw` at 0xc0000000001acc94 loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x6a0.
  The `sw` at 0xc0000000001acca0 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x6a0.
  2 such `ld` accesses load 8 bytes from ([[-0x3c00]:8]:8 + (a0 << 0x3)) +0x0, including `ld` at 0xc0000000001accb0, `ld` at 0xc0000000001acd84.
  The `ll` at 0xc0000000001accbc loads 4 bytes from [([[-0x3c00]:8]:8 + (a0 << 0x3))]:8 +0x61c.
  The `sc` at 0xc0000000001accc8 stores 4 bytes to [([[-0x3c00]:8]:8 + (a0 << 0x3))]:8 +0x61c.
  The `ld` at 0xc0000000001acce0 loads 8 bytes from abs 0xc00000000153abd0 (Nodepdaindr).
  3 such `lw` accesses load 4 bytes from ?, including `lw` at 0xc0000000001accf4, `lw` at 0xc0000000001acd74, `lw` at 0xc0000000001ace30.
  The `ld` at 0xc0000000001accfc loads 8 bytes from [0xc00000000153abd0]:8 +0x608.
  4 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc0000000001acd08, `sw` at 0xc0000000001acd10, `sw` at 0xc0000000001ace1c.
  The `ld` at 0xc0000000001acd18 loads 8 bytes from ([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  The `sd` at 0xc0000000001acd4c stores 8 bytes to ([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  3 such `ld` accesses load 8 bytes from ?, including `ld` at 0xc0000000001acd90, `ld` at 0xc0000000001acddc, `ld` at 0xc0000000001acea8.
  9 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc0000000001acda4, `sd` at 0xc0000000001acde0, `sd` at 0xc0000000001acde4.
  The `lw` at 0xc0000000001acdb8 loads 4 bytes from [-0x3c00]:8 +0x63c.
  3 such `ld` accesses load 8 bytes from ([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0, including `ld` at 0xc0000000001acdc0, `ld` at 0xc0000000001ace88, `ld` at 0xc0000000001aced8.
  2 such `lhu` accesses load 2 bytes from ?, including `lhu` at 0xc0000000001acdd8, `lhu` at 0xc0000000001ace94.
  The `ld` at 0xc0000000001ace54 loads 8 bytes from table(0xc000000001529408) +0x0.
  The `ld` at 0xc0000000001ace68 loads 8 bytes from table(0xc0000000015190d0) +0x0.
  2 such `ld` accesses load 8 bytes from ([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x48, including `ld` at 0xc0000000001ace6c, `ld` at 0xc0000000001acec0.
  The `sd` at 0xc0000000001acef0 stores 8 bytes to ([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x80.
  The `jal` at 0xc0000000001acbcc calls is_specified [0xc0000000001a8e2c] with a0=0xc0000000014d3923, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001acc30 calls setupbadmem [0xc000000000029b18] with a0=?, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=?.
  The `jal` at 0xc0000000001ace50 calls bset [0xc0000000002697dc] with a0=entry(0xc000000001529408), a1=?, a2=?, a3=ret(is_specified).
  The `jal` at 0xc0000000001ace64 calls bset [0xc0000000002697dc] with a0=entry(0xc0000000015190d0), a1=?, a2=?, a3=?.
  The `beql` branch at 0xc0000000001acccc targets 0xc0000000001accbc, is taken when t9(?) == 0, and falls through when t9(?) != 0; its branch-likely delay slot `nop` at 0xc0000000001accd0 runs only when taken and is annulled on fall-through.
  The `bne` branch at 0xc0000000001acd30 targets 0xc0000000001acda8, is taken when t8(=[([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) != 0, and falls through when t8(=[([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) == 0.
  The `bne` branch at 0xc0000000001acda0 targets 0xc0000000001acd58, is taken when a1(?) < 0x6 (signed), and falls through when a1(?) >= 0x6 (signed).
  The `bltz` branch at 0xc0000000001acdcc targets 0xc0000000001acee4, is taken when s3(?) < 0 (signed), and falls through when s3(?) >= 0 (signed).
  The `beq` branch at 0xc0000000001acdd4 targets 0xc0000000001ace98 and is always taken.
  The `bne` branch at 0xc0000000001ace80 targets 0xc0000000001ace8c, is taken when a1(=[([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s0(?), and falls through when a1(=[([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s0(?).
  The `bltz` branch at 0xc0000000001ace8c targets 0xc0000000001acee4, is taken when a0(?) < 0 (signed), and falls through when a0(?) >= 0 (signed).
  The `beq` branch at 0xc0000000001ace98 targets 0xc0000000001acec0, is taken when v0(=0xffff) == a2(?), and falls through when v0(=0xffff) != a2(?).
  The `bne` branch at 0xc0000000001acea0 targets 0xc0000000001acddc, is taken when a3(=ret(is_specified)) != 0, and falls through when a3(=ret(is_specified)) == 0.
  The `beq` branch at 0xc0000000001aceb8 targets 0xc0000000001acdf0 and is always taken.
  The `bne` branch at 0xc0000000001aced0 targets 0xc0000000001acedc, is taken when a5(=[([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s0(?), and falls through when a5(=[([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s0(?).
  The `bgez` branch at 0xc0000000001acedc targets 0xc0000000001ace94, is taken when a0(?) >= 0 (signed), and falls through when a0(?) < 0 (signed).
  The return at 0xc0000000001acf14 has v0 = 0xffff.
- `idbg_init` in `6.5.30/unix.IP27` at 0xc0000000001d5580 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d5580-0xc0000000001d55f4 (116 bytes).
  It occupies 116 bytes.
  2 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc0000000001d55a8, `sw` at 0xc0000000001d55c8.
  The `jal` at 0xc0000000001d55dc calls init_mutex [0xc00000000018a5a4] with a0=0xc00000000153a5e0, a1=0x0, a2=0xc0000000003e4eb0, a3=-0x1, a5=0xc000000001539ab0.
  The `beq` branch at 0xc0000000001d55ac targets 0xc0000000001d55d0, is taken when a2(?) >= 0x14 (signed), and falls through when a2(?) < 0x14 (signed).
  The `bne` branch at 0xc0000000001d55c4 targets 0xc0000000001d5598, is taken when a2(?) < 0x14 (signed), and falls through when a2(?) >= 0x14 (signed).
  The return at 0xc0000000001d55ec has v0 = 0x1.
- `idbg_id` in `6.5.30/unix.IP27` at 0xc0000000001d55f4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d55f4-0xc0000000001d55fc (8 bytes).
  It occupies 8 bytes.
  The `lw` at 0xc0000000001d55f8 loads 4 bytes from a0 (arg0 at entry) +0x3c.
  The return at 0xc0000000001d55f4 has v0 = [a0+60].
- `idbg_lock` in `6.5.30/unix.IP27` at 0xc0000000001d55fc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d55fc-0xc0000000001d5604 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001d55fc has v0 = 0x0.
- `idbg_unlock` in `6.5.30/unix.IP27` at 0xc0000000001d5604 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d5604-0xc0000000001d560c (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001d5604 has v0 = ?.
- `idbg_copydata` in `6.5.30/unix.IP27` at 0xc0000000001d560c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d560c-0xc0000000001d5614 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001d560c has v0 = ?.
- `idbg_makedata` in `6.5.30/unix.IP27` at 0xc0000000001d5614 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d5614-0xc0000000001d568c (120 bytes).
  It occupies 120 bytes.
  The `ld` at 0xc0000000001d563c loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `sd` at 0xc0000000001d567c stores 8 bytes to ret(kern_calloc) +0x8.
  The `sd` at 0xc0000000001d5680 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xc0000000001d562c calls kern_calloc [0xc00000000017e478] with a0=0x1, a1=0x18, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001d5640 calls copyin [0xc00000000001f990] with a0=[a0+24]:8, a1=ret(kern_calloc), a2=0x4, a3=?.
  The `jal` at 0xc0000000001d5650 calls kern_free [0xc00000000017e590] with a0=ret(kern_calloc), a1=0xa0, a2=?, a3=?.
  The `jal` at 0xc0000000001d5674 calls kern_calloc [0xc00000000017e478] with a0=0x1, a1=0xa0, a2=?, a3=?.
  The `beq` branch at 0xc0000000001d5648 targets 0xc0000000001d5674, is taken when v0(=ret(copyin)) == 0, and falls through when v0(=ret(copyin)) != 0.
  The `beq` branch at 0xc0000000001d5684 targets 0xc0000000001d565c and is always taken.
  The return at 0xc0000000001d566c has v0 = one of 0x0, 0xa.
- `idbg_freedata` in `6.5.30/unix.IP27` at 0xc0000000001d568c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d568c-0xc0000000001d56d4 (72 bytes).
  It occupies 72 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x18, including `ld` at 0xc0000000001d568c, `ld` at 0xc0000000001d56b4.
  The `ld` at 0xc0000000001d56a0 loads 8 bytes from [a0+24]:8 +0x8.
  The `sd` at 0xc0000000001d56c4 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xc0000000001d56ac calls kern_free [0xc00000000017e590] with a0=[[a0+24]:8+8]:8, a1=a1 at entry (arg1), a2=[a0+24]:8, a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001d56b8 calls kern_free [0xc00000000017e590] with a0=[a0+24]:8, a1=?, a2=[a0+24]:8, a3=?.
  The `beq` branch at 0xc0000000001d5698 targets 0xc0000000001d56c8, is taken when a2(=[a0+24]:8) == 0, and falls through when a2(=[a0+24]:8) != 0.
  The `beq` branch at 0xc0000000001d56a4 targets 0xc0000000001d56b8, is taken when a0(=[[a0+24]:8+8]:8) == 0, and falls through when a0(=[[a0+24]:8+8]:8) != 0.
  The return at 0xc0000000001d56cc has v0 = one of ?, ret(kern_free).
- `idbg_load` in `6.5.30/unix.IP27` at 0xc0000000001d5b0c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d5b0c-0xc0000000001d5bb0 (164 bytes).
  It occupies 164 bytes.
  The `ld` at 0xc0000000001d5b50 loads 8 bytes from a0 (arg0 at entry) +0x28.
  The `ld` at 0xc0000000001d5b58 loads 8 bytes from [a0+40]:8 +0x50.
  The `jal` at 0xc0000000001d5b28 calls mutex_lock [0xc00000000018a734] with a0=0xc00000000153a5e0, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001d5b30 calls debug_check [0xc0000000001d59ec] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001d5b48 calls debug_freeid [0xc0000000001d59ac] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001d5b74 calls mutex_unlock [0xc00000000018a66c] with a0=0xc00000000153a5e0, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001d5b90 calls mloadfile [0xc0000000001d6ab4] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001d5ba0 calls debug_init [0xc0000000001d56d4] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xc0000000001d5b38 targets 0xc0000000001d5b90, is taken when v0(=ret(debug_check)) == 0, and falls through when v0(=ret(debug_check)) != 0.
  The `beq` branch at 0xc0000000001d5b40 targets 0xc0000000001d5b70, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `bne` branch at 0xc0000000001d5b98 targets 0xc0000000001d5b40, is taken when v0(=ret(mloadfile)) != 0, and falls through when v0(=ret(mloadfile)) == 0.
  The `beq` branch at 0xc0000000001d5ba8 targets 0xc0000000001d5b40 and is always taken.
  The return at 0xc0000000001d5b88 has v0 = ?.
- `idbg_unload` in `6.5.30/unix.IP27` at 0xc0000000001d5bb0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d5bb0-0xc0000000001d5d1c (364 bytes).
  It occupies 364 bytes.
  The `ld` at 0xc0000000001d5bc8 loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `ld` at 0xc0000000001d5be0 loads 8 bytes from a0 (arg0 at entry) +0x50.
  The `ld` at 0xc0000000001d5c0c loads 8 bytes from [a0+24]:8 +0x8.
  The `ld` at 0xc0000000001d5c14 loads 8 bytes from [[a0+24]:8+8]:8 +0x68.
  The `sw` at 0xc0000000001d5c80 stores 4 bytes to abs 0xc00000000153a590.
  The `ld` at 0xc0000000001d5c88 loads 8 bytes from [a0+24]:8 +0x10.
  The `ld` at 0xc0000000001d5cf0 loads 8 bytes from [0xc0000000014d3b00]:8 +0x28.
  The `lw` at 0xc0000000001d5cf4 loads 4 bytes from [[0xc0000000014d3b00]:8+40]:8 +0x48.
  The `sw` at 0xc0000000001d5d00 stores 4 bytes to [[0xc0000000014d3b00]:8+40]:8 +0x48.
  The `ld` at 0xc0000000001d5d04 loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `sw` at 0xc0000000001d5d18 stores 4 bytes to table(0xc00000000153a590) +0x0.
  The `jal` at 0xc0000000001d5bd0 calls mutex_lock [0xc00000000018a734] with a0=0xc00000000153a5e0, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jalr` at 0xc0000000001d5c60 calls (unknown callee) with a0=?, a1=?, a2=?, a3=-0x1, a4=0xc000000000000000, a5=0xc0000fbf00000000, a6=0xe000.
  The `jal` at 0xc0000000001d5c70 calls mutex_unlock [0xc00000000018a66c] with a0=0xc00000000153a5e0, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001d5c8c calls symtab_rele [0xc0000000001d6584] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001d5c98 calls mutex_unlock [0xc00000000018a66c] with a0=0xc00000000153a5e0, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001d5cc4 calls mfindname [0xc0000000001d7084] with a0=[a0+80]:8, a1=0xc0000000003e4bc0, a2=sp-32, a3=-0x1, a4=0xc000000000000000, a5=0xc0000fbf00000000, a6=0xe000, a7=0xffffffff00000000.
  The `jal` at 0xc0000000001d5cd4 calls mutex_unlock [0xc00000000018a66c] with a0=0xc00000000153a5e0, a1=?, a2=?, a3=?.
  The `jalr` at 0xc0000000001d5ce4 calls (unknown callee) with a0=?, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc0000000001d5c04 targets 0xc0000000001d5cb8, is taken when v0(=[0xc0000000014d3b00]:8) != a0(=[a0+80]:8), and falls through when v0(=[0xc0000000014d3b00]:8) == a0(=[a0+80]:8).
  The `bne` branch at 0xc0000000001d5c38 targets 0xc0000000001d5c48, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) < t3(=0xa800000000000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) >= t3(=0xa800000000000000) (unsigned).
  The `bne` branch at 0xc0000000001d5c40 targets 0xc0000000001d5c60, is taken when a7(?) != 0, and falls through when a7(?) == 0.
  The `bne` branch at 0xc0000000001d5c48 targets 0xc0000000001d5c80, is taken when a0(?) != 0, and falls through when a0(?) == 0.
  The `beq` branch at 0xc0000000001d5c58 targets 0xc0000000001d5c80, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) >= t9(=0xc0000fbfe0000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) < t9(=0xc0000fbfe0000000) (unsigned).
  The `beq` branch at 0xc0000000001d5c68 targets 0xc0000000001d5c80, is taken when v0(=ret(?)) == 0, and falls through when v0(=ret(?)) != 0.
  The `beq` branch at 0xc0000000001d5c78 targets 0xc0000000001d5ca4 and is always taken.
  The `beq` branch at 0xc0000000001d5ccc targets 0xc0000000001d5ce4, is taken when v0(=ret(mfindname)) == 0, and falls through when v0(=ret(mfindname)) != 0.
  The `beq` branch at 0xc0000000001d5cdc targets 0xc0000000001d5ca4 and is always taken.
  The `beq` branch at 0xc0000000001d5d14 targets 0xc0000000001d5c94 and is always taken.
  The return at 0xc0000000001d5cb0 has v0 = one of 0x0, 0x33.
- `idbg_reg` in `6.5.30/unix.IP27` at 0xc0000000001d5d1c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d5d1c-0xc0000000001d5d24 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001d5d1c has v0 = 0x59.
- `idbg_unreg` in `6.5.30/unix.IP27` at 0xc0000000001d5d24 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001d5d24-0xc0000000001d5d2c (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000001d5d24 has v0 = 0x59.
- `setup_lowmem` in `6.5.30/unix.IP27` at 0xc0000000001f2634 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001f2634-0xc0000000001f29a0 (876 bytes).
  It occupies 876 bytes.
  The `sd` at 0xc0000000001f26d8 stores 8 bytes to table(0xc00000000153abd0) +0x0.
  3 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc0000000001f26fc, `sd` at 0xc0000000001f274c, `sd` at 0xc0000000001f2904.
  The `ld` at 0xc0000000001f2700 loads 8 bytes from abs 0xffffffffffffc400.
  The `ld` at 0xc0000000001f2704 loads 8 bytes from [-0x3c00]:8 +0x0.
  2 such `ld` accesses load 8 bytes from table(0xc00000000153abd0) +0x0, including `ld` at 0xc0000000001f270c, `ld` at 0xc0000000001f299c.
  The `sd` at 0xc0000000001f271c stores 8 bytes to ([[-0x3c00]:8]:8 + (a0 << 0x3)) +0x0.
  2 such `sd` accesses store 8 bytes to table(0xc000000001529408) +0x0, including `sd` at 0xc0000000001f276c, `sd` at 0xc0000000001f27d8.
  2 such `sd` accesses store 8 bytes to table(0xc0000000015190d0) +0x0, including `sd` at 0xc0000000001f2770, `sd` at 0xc0000000001f27d0.
  The `lh` at 0xc0000000001f278c loads 2 bytes from table(0xc0000000014d4e80) +0x0.
  The `ld` at 0xc0000000001f285c loads 8 bytes from abs 0xc00000000153abd0 (Nodepdaindr).
  The `ld` at 0xc0000000001f286c loads 8 bytes from [0xc00000000153abd0]:8 +0x608.
  2 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc0000000001f2898, `sw` at 0xc0000000001f28e0.
  The `sd` at 0xc0000000001f28b4 stores 8 bytes to ([[0xc00000000153abd0]:8+1544]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0.
  The `sd` at 0xc0000000001f2994 stores 8 bytes to [0xc0000000014d3e10]:8 +0x400.
  The `jal` at 0xc0000000001f2684 calls blkclr [0xc00000000001fd0c] with a0=?, a1=?, a2=0xa800000000000000, a3=?, a4=0xfffffffffffc0000.
  The `jal` at 0xc0000000001f268c calls flush_cache [0xc00000000002b9c4] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001f269c calls mktables [0xc0000000001f29a0] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xc0000000001f26bc calls low_mem_alloc [0xc0000000001f2358] with a0=0x53e0, a1=sp-112, a2=0xc0000000003e62f0, a3=?.
  The `jal` at 0xc0000000001f26f4 calls low_mem_alloc [0xc0000000001f2358] with a0=((([0xc0000000014d3e1c] + [0xc0000000014d3e20]) << 0x3) << 0x0), a1=sp-112, a2=0xc0000000003e6300, a3=?.
  The `jal` at 0xc0000000001f2718 calls nodepda_numa_init [0xc000000000277a7c] with a0=a0 at entry (arg0), a1=sp-112, a2=entry(0xc00000000153abd0), a3=([[-0x3c00]:8]:8 + (a0 << 0x3)).
  The `jal` at 0xc0000000001f2740 calls low_mem_alloc [0xc0000000001f2358] with a0=(((([0xc0000000014d3e1c] << 0x4) + [0xc0000000014d3e1c]) << 0x3) << 0x0), a1=sp-112, a2=0xc0000000003e6310, a3=?.
  The `jal` at 0xc0000000001f2784 calls slot_getsize [0xc00000000002a004] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000001f27b8 calls low_mem_alloc [0xc0000000001f2358] with a0=?, a1=sp-112, a2=0xc0000000003e6328, a3=?.
  The `jal` at 0xc0000000001f27cc calls low_mem_alloc [0xc0000000001f2358] with a0=?, a1=sp-112, a2=0xc0000000003e6340, a3=?.
  The `jal` at 0xc0000000001f289c calls low_mem_alloc [0xc0000000001f2358] with a0=?, a1=sp-112, a2=0xc0000000003e6358, a3=?, a7=[0xc0000000014d404c].
  The `jal` at 0xc0000000001f28e4 calls low_mem_alloc [0xc0000000001f2358] with a0=?, a1=sp-112, a2=0xc0000000003e6358, a3=?.
  The `jal` at 0xc0000000001f2978 calls cmn_err_tag [0xc0000000001fcc20] with a0=0x6ec, a1=0x4, a2=0xc0000000003e6378, a3=?.
  The `beq` branch at 0xc0000000001f26d4 targets 0xc0000000001f2990, is taken when s7(=a0) == 0, and falls through when s7(=a0) != 0.
  The `beq` branch at 0xc0000000001f2764 targets 0xc0000000001f2780 and is always taken.
  The `beq` branch at 0xc0000000001f277c targets 0xc0000000001f27dc, is taken when s0(?) >= 0x20 (signed), and falls through when s0(?) < 0x20 (signed).
  The `beq` branch at 0xc0000000001f27a8 targets 0xc0000000001f276c, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `beq` branch at 0xc0000000001f27d4 targets 0xc0000000001f2774 and is always taken.
  The `beq` branch at 0xc0000000001f2900 targets 0xc0000000001f28bc, is taken when s3(=0x1000000) >= s1(?) (signed), and falls through when s3(=0x1000000) < s1(?) (signed).
  The `bne` branch at 0xc0000000001f292c targets 0xc0000000001f2968, is taken when t0(?) < a6(?) (unsigned), and falls through when t0(?) >= a6(?) (unsigned).
  The `beq` branch at 0xc0000000001f2988 targets 0xc0000000001f2934 and is always taken.
  The `beq` branch at 0xc0000000001f2998 targets 0xc0000000001f26dc and is always taken.
  The return at 0xc0000000001f2960 has v0 = ?.
- `panic_coproc_select` in `6.5.30/unix.IP27` at 0xc0000000001fee74 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc0000000001fee74-0xc0000000001feefc (136 bytes).
  It occupies 136 bytes.
  2 such `lh` accesses load 2 bytes from table(0xc0000000014d4f00) +0x0, including `lh` at 0xc0000000001feeb4, `lh` at 0xc0000000001feeb8.
  The `ll` at 0xc0000000001feec8 loads 4 bytes from abs 0xc0000000014cba24.
  The `sc` at 0xc0000000001feed4 stores 4 bytes to abs 0xc0000000014cba24.
  The `jal` at 0xc0000000001fee94 calls getcpuid [0xc000000000027178] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000001feeec calls dumpsys_helper [0xc000000000217e34] with a0=[0xc0000000014cba24]+1, a1=0x1, a2=0x1, a3=0xc0000000014cba24, a4=[0xc0000000014cba24]+1.
  The `beq` branch at 0xc0000000001fee80 targets 0xc0000000001fee94, is taken when at(=[0xc0000000014cba14]) == v0(=-0x1), and falls through when at(=[0xc0000000014cba14]) != v0(=-0x1).
  The `bne` branch at 0xc0000000001feec0 targets 0xc0000000001fee88, is taken when v1(=entry(0xc0000000014d4f00)) != a0(=entry(0xc0000000014d4f00)), and falls through when v1(=entry(0xc0000000014d4f00)) == a0(=entry(0xc0000000014d4f00)).
  The `beql` branch at 0xc0000000001feed8 targets 0xc0000000001feec8, is taken when a4(=[0xc0000000014cba24]+1) == 0, and falls through when a4(=[0xc0000000014cba24]+1) != 0; its branch-likely delay slot `nop` at 0xc0000000001feedc runs only when taken and is annulled on fall-through.
  The `bne` branch at 0xc0000000001feee4 targets 0xc0000000001fee8c, is taken when a0(=[0xc0000000014cba24]+1) != a1(=0x1), and falls through when a0(=[0xc0000000014cba24]+1) == a1(=0x1).
  The `beq` branch at 0xc0000000001feef4 targets 0xc0000000001fee8c and is always taken.
  The return at 0xc0000000001fee8c has v0 = one of ?, ret(getcpuid), ret(dumpsys_helper).
- `hub_set_fire_n_forget` in `6.5.30/unix.IP27` at 0xc000000000377dfc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP27`.
  Range: 0xc000000000377dfc-0xc000000000377e50 (84 bytes).
  It occupies 84 bytes.
  The `ld` at 0xc000000000377e28 loads 8 bytes from table(0x9200000001c00198) +0x0.
  The `sd` at 0xc000000000377e4c stores 8 bytes to table(0x9200000001c00198) +0x0.
  The `beq` branch at 0xc000000000377e0c targets 0xc000000000377e18, is taken when a1 >= 0x8 (signed), and falls through when a1 < 0x8 (signed).
  The `bne` branch at 0xc000000000377e24 targets 0xc000000000377e30, is taken when a7(?) != 0, and falls through when a7(?) == 0.
  The return at 0xc000000000377e48 has v0 = ?.
- `ioc3_early_pckminit` in `6.5.30/unix.IP30` at 0xa800000020061a94 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020061a94-0xa800000020061a9c (8 bytes).
  It occupies 8 bytes.
  The return at 0xa800000020061a94 has v0 = ?.
- `ioc3_pckm_kbdexists` in `6.5.30/unix.IP30` at 0xa800000020063334 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020063334-0xa800000020063370 (60 bytes).
  It occupies 60 bytes.
  The `lw` at 0xa800000020063340 loads 4 bytes from [0xa8000000204ae540]:8 +0x28.
  The `ld` at 0xa800000020063350 loads 8 bytes from ([0xa8000000204ae540]:8 + ([[0xa8000000204ae540]:8+40] << 0x3)) +0x18.
  The `lw` at 0xa800000020063354 loads 4 bytes from [([0xa8000000204ae540]:8 + ([[0xa8000000204ae540]:8+40] << 0x3))+24]:8 +0x4.
  The `beq` branch at 0xa800000020063338 targets 0xa800000020063368, is taken when a2(=[0xa8000000204ae540]:8) == 0, and falls through when a2(=[0xa8000000204ae540]:8) != 0.
  The `beq` branch at 0xa800000020063344 targets 0xa800000020063368, is taken when a3(=[[0xa8000000204ae540]:8+40]) == v0(=0x13), and falls through when a3(=[[0xa8000000204ae540]:8+40]) != v0(=0x13).
  The return at 0xa800000020063360 has v0 = ?.
  The return at 0xa800000020063368 has v0 = 0x0.
- `ioc3_pckm_setleds` in `6.5.30/unix.IP30` at 0xa800000020063370 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020063370-0xa8000000200633ac (60 bytes).
  It occupies 60 bytes.
  The `lw` at 0xa800000020063388 loads 4 bytes from [0xa8000000204ae540]:8 +0x28.
  The `ld` at 0xa80000002006339c loads 8 bytes from ([0xa8000000204ae540]:8 + ([[0xa8000000204ae540]:8+40] << 0x3)) +0x18.
  The `jal` at 0xa800000020063398 calls ioc3_lcl_pckm_setleds [0xa800000020063424] with a0=[([0xa8000000204ae540]:8 + ([[0xa8000000204ae540]:8+40] << 0x3))+24]:8, a1=a0 at entry (arg0), a2=a2 at entry (arg2), a3=[0xa8000000204ae540]:8, a4=0x13, a5=[[0xa8000000204ae540]:8+40].
  The `beq` branch at 0xa800000020063380 targets 0xa8000000200633a0, is taken when a3(=[0xa8000000204ae540]:8) == 0, and falls through when a3(=[0xa8000000204ae540]:8) != 0.
  The `beq` branch at 0xa80000002006338c targets 0xa8000000200633a0, is taken when a5(=[[0xa8000000204ae540]:8+40]) == a4(=0x13), and falls through when a5(=[[0xa8000000204ae540]:8+40]) != a4(=0x13).
  The return at 0xa8000000200633a4 has v0 = one of ?, ret(ioc3_lcl_pckm_setleds).
- `ioc3_pckm_bell` in `6.5.30/unix.IP30` at 0xa8000000200633ac (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000200633ac-0xa8000000200633f0 (68 bytes).
  It occupies 68 bytes.
  The `lw` at 0xa8000000200633c0 loads 4 bytes from [0xa8000000204ae540]:8 +0x28.
  The `lw` at 0xa8000000200633cc loads 4 bytes from [0xa8000000204ae540]:8 +0x34.
  The `ld` at 0xa8000000200633e0 loads 8 bytes from ([0xa8000000204ae540]:8 + ([[0xa8000000204ae540]:8+40] << 0x3)) +0x18.
  The `jal` at 0xa8000000200633dc calls ioc3_lcl_pckm_setleds [0xa800000020063424] with a0=[([0xa8000000204ae540]:8 + ([[0xa8000000204ae540]:8+40] << 0x3))+24]:8, a1=([[0xa8000000204ae540]:8+52] | 0x8), a2=a2 at entry (arg2), a3=[0xa8000000204ae540]:8, a4=[[0xa8000000204ae540]:8+40].
  The `beq` branch at 0xa8000000200633b8 targets 0xa8000000200633e4, is taken when a3(=[0xa8000000204ae540]:8) == 0, and falls through when a3(=[0xa8000000204ae540]:8) != 0.
  The `beq` branch at 0xa8000000200633c4 targets 0xa8000000200633e8, is taken when a4(=[[0xa8000000204ae540]:8+40]) == a1(=0x13), and falls through when a4(=[[0xa8000000204ae540]:8+40]) != a1(=0x13).
  The return at 0xa8000000200633e8 has v0 = ?.
- `ioc3_gl_setporthandler` in `6.5.30/unix.IP30` at 0xa8000000200633f0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000200633f0-0xa80000002006341c (44 bytes).
  It occupies 44 bytes.
  The `lw` at 0xa8000000200633fc loads 4 bytes from [0xa8000000204ae540]:8 +0x28.
  The `ld` at 0xa80000002006340c loads 8 bytes from ([0xa8000000204ae540]:8 + ([[0xa8000000204ae540]:8+40] << 0x3)) +0x18.
  The `sd` at 0xa800000020063410 stores 8 bytes to [([0xa8000000204ae540]:8 + ([[0xa8000000204ae540]:8+40] << 0x3))+24]:8 +0x18.
  The `beq` branch at 0xa8000000200633f4 targets 0xa800000020063414, is taken when a2(=[0xa8000000204ae540]:8) == 0, and falls through when a2(=[0xa8000000204ae540]:8) != 0.
  The `beq` branch at 0xa800000020063400 targets 0xa800000020063414, is taken when a4(=[[0xa8000000204ae540]:8+40]) == a3(=0x13), and falls through when a4(=[[0xa8000000204ae540]:8+40]) != a3(=0x13).
  The return at 0xa800000020063414 has v0 = ?.
- `ioc3_du_keyboard_port` in `6.5.30/unix.IP30` at 0xa80000002006341c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa80000002006341c-0xa800000020063424 (8 bytes).
  It occupies 8 bytes.
  The return at 0xa80000002006341c has v0 = -0x1.
- `ioc3_lcl_pckm_setleds` in `6.5.30/unix.IP30` at 0xa800000020063424 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020063424-0xa800000020063478 (84 bytes).
  It occupies 84 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x48, including `ld` at 0xa800000020063430, `ld` at 0xa800000020063458.
  The `jal` at 0xa800000020063444 calls mutex_lock [0xa8000000201535f4] with a0=[a0+72]:8+72, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa800000020063450 calls ioc3_lcl_pckm_setleds_lock [0xa800000020063478] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xa800000020063460 calls mutex_unlock [0xa80000002015352c] with a0=[a0+72]:8+72, a1=?, a2=?, a3=?.
  The return at 0xa800000020063470 has v0 = ret(mutex_unlock).
- `ioc3_lcl_pckm_setleds_lock` in `6.5.30/unix.IP30` at 0xa800000020063478 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020063478-0xa8000000200635f0 (376 bytes).
  It occupies 376 bytes.
  The `ld` at 0xa800000020063484 loads 8 bytes from a0 (arg0 at entry) +0x48.
  5 such `lw` accesses load 4 bytes from [a0+72]:8 +0x34, including `lw` at 0xa80000002006348c, `lw` at 0xa800000020063500, `lw` at 0xa800000020063520.
  The `lw` at 0xa8000000200634bc loads 4 bytes from [a0+72]:8 +0x38.
  The `sw` at 0xa8000000200634e8 stores 4 bytes to [a0+72]:8 +0x38.
  2 such `sw` accesses store 4 bytes to [a0+72]:8 +0x34, including `sw` at 0xa8000000200634f4, `sw` at 0xa800000020063528.
  The `jal` at 0xa8000000200634fc calls pckm_pollcmd [0xa800000020063258] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xa800000020063508 calls data_check [0xa8000000200636b8] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xa800000020063554 calls pckm_pollcmd [0xa800000020063258] with a0=a0 at entry (arg0), a1=0xed, a2=?, a3=?.
  The `jal` at 0xa800000020063560 calls data_check [0xa8000000200636b8] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xa800000020063580 calls pckm_pollcmd [0xa800000020063258] with a0=a0 at entry (arg0), a1=0xf4, a2=?, a3=?.
  The `jal` at 0xa80000002006358c calls data_check [0xa8000000200636b8] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xa8000000200635a4 calls default_kbd_setting [0xa800000020062b58] with a0=a0 at entry (arg0), a1=0x0, a2=[[a0+72]:8+52], a3=?.
  The `jal` at 0xa8000000200635c4 calls pckm_pollcmd [0xa800000020063258] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xa8000000200635d0 calls data_check [0xa8000000200636b8] with a0=ret(pckm_pollcmd), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xa8000000200635e0 calls pckm_reinit_core [0xa800000020062ae0] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xa8000000200634ac targets 0xa80000002006352c, is taken when at(=[[a0+72]:8+52]) == a3(?), and falls through when at(=[[a0+72]:8+52]) != a3(?).
  The `beq` branch at 0xa8000000200634b4 targets 0xa8000000200634f0, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xa8000000200634c8 targets 0xa8000000200634d8, is taken when a7(?) >= 0xe (signed), and falls through when a7(?) < 0xe (signed).
  The `bne` branch at 0xa8000000200634d0 targets 0xa800000020063530, is taken when a4(?) != 0, and falls through when a4(?) == 0.
  The `beq` branch at 0xa8000000200634d8 targets 0xa8000000200634e8, is taken when a0(?) == 0, and falls through when a0(?) != 0.
  The `bne` branch at 0xa8000000200634e0 targets 0xa800000020063530, is taken when a5(?) != 0, and falls through when a5(?) == 0.
  The `beq` branch at 0xa8000000200634f0 targets 0xa800000020063550 and is always taken.
  The `beq` branch at 0xa800000020063510 targets 0xa8000000200635c4, is taken when v0(=ret(data_check)) == 0, and falls through when v0(=ret(data_check)) != 0.
  The `beq` branch at 0xa800000020063518 targets 0xa800000020063530, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xa80000002006354c targets 0xa8000000200635b4, is taken when t1(?) == 0, and falls through when t1(?) != 0.
  The `bne` branch at 0xa800000020063568 targets 0xa8000000200634f8, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `bne` branch at 0xa800000020063574 targets 0xa800000020063548, is taken when s3(?) < 0x2 (unsigned), and falls through when s3(?) >= 0x2 (unsigned).
  The `bne` branch at 0xa800000020063594 targets 0xa80000002006354c, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `beq` branch at 0xa8000000200635ac targets 0xa80000002006354c and is always taken.
  The `bne` branch at 0xa8000000200635b4 targets 0xa800000020063520, is taken when s2(?) != 0, and falls through when s2(?) == 0.
  The `beq` branch at 0xa8000000200635bc targets 0xa800000020063530 and is always taken.
  The `bne` branch at 0xa8000000200635d8 targets 0xa800000020063518, is taken when v0(=ret(data_check)) != 0, and falls through when v0(=ret(data_check)) == 0.
  The `beq` branch at 0xa8000000200635e8 targets 0xa800000020063518 and is always taken.
  The return at 0xa800000020063540 has v0 = ?.
- `dev_chain` in `6.5.30/unix.IP30` at 0xa80000002011ab34 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa80000002011ab34-0xa80000002011ac20 (236 bytes).
  It occupies 236 bytes.
  The `ld` at 0xa80000002011ab40 loads 8 bytes from a0 (arg0 at entry) +0x38.
  2 such `lw` accesses load 4 bytes from abs 0xa8000000204fc2e0 (v+48), including `lw` at 0xa80000002011ab54, `lw` at 0xa80000002011abb8.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x50, including `lw` at 0xa80000002011ab5c, `lw` at 0xa80000002011aba0.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x28, including `ld` at 0xa80000002011ab7c, `ld` at 0xa80000002011ab90.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x30, including `ld` at 0xa80000002011ab80, `ld` at 0xa80000002011ab88.
  The `sd` at 0xa80000002011ab84 stores 8 bytes to [a0+40]:8 +0x30.
  The `sd` at 0xa80000002011ab9c stores 8 bytes to [a0+48]:8 +0x28.
  2 such `ld` accesses load 8 bytes from ?, including `ld` at 0xa80000002011abd8, `ld` at 0xa80000002011abe4.
  The `sd` at 0xa80000002011abdc stores 8 bytes to a0 (arg0 at entry) +0x30.
  The `sd` at 0xa80000002011abe0 stores 8 bytes to a0 (arg0 at entry) +0x28.
  2 such `sd` accesses store 8 bytes to ?, including `sd` at 0xa80000002011abe8, `sd` at 0xa80000002011abf0.
  The `sd` at 0xa80000002011abfc stores 8 bytes to a0 (arg0 at entry) +0x38.
  The `jal` at 0xa80000002011ab74 calls mutex_spinlock [0xa80000002022d954] with a0=[a0+56]:8+56, a1=a1 at entry (arg1), a2=[a0+56]:8, a3=[0xa8000000204fc2e0].
  The `jal` at 0xa80000002011ab98 calls mutex_spinunlock [0xa80000002022d9a0] with a0=[a0+56]:8+56, a1=ret(mutex_spinlock), a2=?, a3=?, a6=[a0+40]:8, a7=[a0+48]:8.
  The `jal` at 0xa80000002011abd0 calls mutex_spinlock [0xa80000002022d954] with a0=?, a1=?, a2=0xa8000000204e9f80, a3=0xa8000000204fc2b0.
  The `jal` at 0xa80000002011abf8 calls mutex_spinunlock [0xa80000002022d9a0] with a0=?, a1=ret(mutex_spinlock), a2=?, a3=?.
  The `beq` branch at 0xa80000002011ab50 targets 0xa80000002011aba0, is taken when a2(=[a0+56]:8) == 0, and falls through when a2(=[a0+56]:8) != 0.
  The `beq` branch at 0xa80000002011ab6c targets 0xa80000002011ac04, is taken when a2(=[a0+56]:8) == a4(?), and falls through when a2(=[a0+56]:8) != a4(?).
  The `beq` branch at 0xa80000002011abb0 targets 0xa80000002011ac18, is taken when a0(=[a0+80]) == t2(=-0x1), and falls through when a0(=[a0+80]) != t2(=-0x1).
  The `beq` branch at 0xa80000002011ac18 targets 0xa80000002011abcc and is always taken.
  The return at 0xa80000002011ac10 has v0 = one of ?, ret(mutex_spinunlock).
- `ioc3_config` in `6.5.30/unix.IP30` at 0xa800000020143040 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143040-0xa800000020143214 (468 bytes).
  It occupies 468 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xa8000000201430d8, `lw` at 0xa800000020143184.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa8000000201430e8, `ld` at 0xa8000000201430f8, `ld` at 0xa800000020143194.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xa8000000201430f4, `sw` at 0xa800000020143198.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xa8000000201430fc, `lw` at 0xa800000020143128.
  The `ld` at 0xa80000002014313c loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xa800000020143140 loads 1 byte from [a0+72]:8 +0x3.
  The `sb` at 0xa800000020143180 stores 1 byte to [a0+72]:8 +0x3.
  The `sw` at 0xa8000000201431c4 stores 4 bytes to a0 (arg0 at entry) +0x80.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x84, including `sw` at 0xa8000000201431cc, `sw` at 0xa8000000201431e8.
  The `lw` at 0xa8000000201431d8 loads 4 bytes from a0 (arg0 at entry) +0x88.
  The `jal` at 0xa800000020143064 calls set_baud_ti [0xa8000000201427a4] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa8000000201431d4 calls ioc3_rx_timeout [0xa800000020143d0c] with a0=a0 at entry (arg0), a1=[a0+136], a2=[a0+148], a3=?, a6=a1 at entry (arg1), a7=0x3e8.
  The `beq` branch at 0xa80000002014306c targets 0xa80000002014308c, is taken when v0(=ret(set_baud_ti)) == 0, and falls through when v0(=ret(set_baud_ti)) != 0.
  The `beq` branch at 0xa8000000201430a0 targets 0xa8000000201430d0, is taken when s1(=a2) == at(=0x5), and falls through when s1(=a2) != at(=0x5).
  The `beq` branch at 0xa8000000201430a8 targets 0xa8000000201431ec, is taken when s1(=a2) == a0(=0x6), and falls through when s1(=a2) != a0(=0x6).
  The `beq` branch at 0xa8000000201430b0 targets 0xa8000000201431f4, is taken when s1(=a2) == a2(=0x7), and falls through when s1(=a2) != a2(=0x7).
  The `beq` branch at 0xa8000000201430b8 targets 0xa8000000201430c8, is taken when s1(=a2) == a7(=0x8), and falls through when s1(=a2) != a7(=0x8).
  The `beq` branch at 0xa8000000201430c0 targets 0xa80000002014307c and is always taken.
  The `beq` branch at 0xa8000000201430c8 targets 0xa8000000201430d8 and is always taken.
  The `bgez` branch at 0xa8000000201430e0 targets 0xa80000002014313c, is taken when v0(=([a0+148] << 0x23)) >= 0 (signed), and falls through when v0(=([a0+148] << 0x23)) < 0 (signed).
  The `bltz` branch at 0xa800000020143110 targets 0xa80000002014313c, is taken when t3(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t3(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `bne` branch at 0xa800000020143120 targets 0xa8000000201431fc, is taken when t9(?) != 0, and falls through when t9(?) == 0.
  The `beq` branch at 0xa800000020143134 targets 0xa800000020143120, is taken when (ra(=[[a0+64]:8]) & t1(=0x40000000)) == 0, and falls through when (ra(=[[a0+64]:8]) & t1(=0x40000000)) != 0.
  The `beq` branch at 0xa800000020143160 targets 0xa800000020143174, is taken when v1(=a4) == 0, and falls through when v1(=a4) != 0.
  The `beq` branch at 0xa80000002014316c targets 0xa800000020143204, is taken when a0(=a5) == 0, and falls through when a0(=a5) != 0.
  The `beq` branch at 0xa800000020143174 targets 0xa800000020143180, is taken when a1(=a3) == 0, and falls through when a1(=a3) != 0.
  The `beq` branch at 0xa80000002014318c targets 0xa80000002014319c, is taken when (t2(=0x10000000) & a2(=[a0+148])) == 0, and falls through when (t2(=0x10000000) & a2(=[a0+148])) != 0.
  The `beq` branch at 0xa8000000201431c8 targets 0xa8000000201431e4, is taken when a5(?) == 0, and falls through when a5(?) != 0.
  The `beq` branch at 0xa8000000201431dc targets 0xa800000020143078 and is always taken.
  The `beq` branch at 0xa8000000201431e4 targets 0xa8000000201431d0 and is always taken.
  The `beq` branch at 0xa8000000201431ec targets 0xa8000000201430d4 and is always taken.
  The `beq` branch at 0xa8000000201431f4 targets 0xa8000000201430d8 and is always taken.
  The `beq` branch at 0xa8000000201431fc targets 0xa80000002014307c and is always taken.
  The `bne` branch at 0xa800000020143204 targets 0xa80000002014317c, is taken when t3(=a3) != 0, and falls through when t3(=a3) == 0.
  The `beq` branch at 0xa80000002014320c targets 0xa800000020143180 and is always taken.
  The return at 0xa800000020143084 has v0 = one of -0x1, 0x0, 0x1.
- `ioc3_set_extclk` in `6.5.30/unix.IP30` at 0xa800000020143248 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143248-0xa800000020143250 (8 bytes).
  It occupies 8 bytes.
  The return at 0xa800000020143248 has v0 = a1.
- `ioc3_write` in `6.5.30/unix.IP30` at 0xa8000000201433ec (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201433ec-0xa800000020143408 (28 bytes).
  It occupies 28 bytes.
  The `jal` at 0xa8000000201433f4 calls do_ioc3_write [0xa800000020143250] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The return at 0xa800000020143400 has v0 = ret(do_ioc3_write).
- `ioc3_sync_write` in `6.5.30/unix.IP30` at 0xa800000020143408 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143408-0xa80000002014346c (100 bytes).
  It occupies 100 bytes.
  The `lw` at 0xa800000020143438 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xa80000002014341c calls do_ioc3_write [0xa800000020143250] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa80000002014344c calls ioc3_enable_tx [0xa8000000201435e0] with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `blez` branch at 0xa800000020143424 targets 0xa800000020143454, is taken when s3(=a2) <= 0 (signed), and falls through when s3(=a2) > 0 (signed).
  The `bne` branch at 0xa80000002014342c targets 0xa800000020143458, is taken when v0(=ret(do_ioc3_write)) != 0, and falls through when v0(=ret(do_ioc3_write)) == 0.
  The `beq` branch at 0xa800000020143440 targets 0xa800000020143458, is taken when (at(=[a0+120]) & 0x10) == 0, and falls through when (at(=[a0+120]) & 0x10) != 0.
  The return at 0xa800000020143464 has v0 = ret(do_ioc3_write).
- `ioc3_wrflush` in `6.5.30/unix.IP30` at 0xa80000002014346c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa80000002014346c-0xa800000020143530 (196 bytes).
  It occupies 196 bytes.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x38, including `ld` at 0xa80000002014346c, `ld` at 0xa8000000201434a8, `ld` at 0xa8000000201434f0.
  3 such `lw` accesses load 4 bytes from [a0+56]:8 +0x1c, including `lw` at 0xa800000020143470, `lw` at 0xa8000000201434ac, `lw` at 0xa8000000201434f4.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa80000002014348c, `ld` at 0xa8000000201434e4, `ld` at 0xa80000002014350c.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xa800000020143490, `lw` at 0xa800000020143510.
  The `lw` at 0xa8000000201434c8 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xa8000000201434d8 calls ioc3_enable_tx [0xa8000000201435e0] with a0=a0 at entry (arg0), a1=0x1, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa800000020143504 calls us_delay [0xa800000020011640] with a0=0x5, a1=?, a2=one of 0x10000000, 0x50000000, a3=?.
  The `beq` branch at 0xa800000020143484 targets 0xa8000000201434c8, is taken when (at(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (at(=[[a0+56]:8+28]) & 0x1) != 0.
  The `bne` branch at 0xa8000000201434a0 targets 0xa8000000201434bc, is taken when a0(=0x10000000) != v0(?), and falls through when a0(=0x10000000) == v0(?).
  The `beq` branch at 0xa8000000201434b4 targets 0xa8000000201434e8, is taken when (v1(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (v1(=[[a0+56]:8+28]) & 0x1) != 0.
  The `beq` branch at 0xa8000000201434d0 targets 0xa80000002014348c, is taken when (a0(=[a0+120]) & 0x10) == 0, and falls through when (a0(=[a0+120]) & 0x10) != 0.
  The `beq` branch at 0xa8000000201434e0 targets 0xa800000020143490 and is always taken.
  The `beq` branch at 0xa8000000201434e8 targets 0xa800000020143504 and is always taken.
  The `bne` branch at 0xa8000000201434fc targets 0xa800000020143528, is taken when (a1(=[[a0+56]:8+28]) & 0x1) != 0, and falls through when (a1(=[[a0+56]:8+28]) & 0x1) == 0.
  The `beq` branch at 0xa800000020143520 targets 0xa8000000201434f0, is taken when a2(=0x10000000) == a3(?), and falls through when a2(=0x10000000) != a3(?).
  The `beq` branch at 0xa800000020143528 targets 0xa8000000201434bc and is always taken.
  The return at 0xa8000000201434c0 has v0 = one of ?, ret(us_delay).
- `ioc3_break` in `6.5.30/unix.IP30` at 0xa800000020143530 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143530-0xa8000000201435e0 (176 bytes).
  It occupies 176 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xa800000020143530, `lw` at 0xa8000000201435ac.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa800000020143544, `ld` at 0xa800000020143554, `ld` at 0xa8000000201435bc.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xa800000020143550, `sw` at 0xa8000000201435c0.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xa800000020143558, `lw` at 0xa800000020143584.
  The `ld` at 0xa800000020143598 loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xa8000000201435a0 loads 1 byte from [a0+72]:8 +0x3.
  2 such `sb` accesses store 1 byte to [a0+72]:8 +0x3, including `sb` at 0xa8000000201435a8, `sb` at 0xa8000000201435dc.
  The `bgez` branch at 0xa800000020143538 targets 0xa800000020143598, is taken when at(=([a0+148] << 0x23)) >= 0 (signed), and falls through when at(=([a0+148] << 0x23)) < 0 (signed).
  The `bltz` branch at 0xa80000002014356c targets 0xa800000020143598, is taken when t1(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t1(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `bne` branch at 0xa80000002014357c targets 0xa8000000201435c8, is taken when t3(?) != 0, and falls through when t3(?) == 0.
  The `beq` branch at 0xa800000020143590 targets 0xa80000002014357c, is taken when (t8(=[[a0+64]:8]) & a7(=0x40000000)) == 0, and falls through when (t8(=[[a0+64]:8]) & a7(=0x40000000)) != 0.
  The `beq` branch at 0xa80000002014359c targets 0xa8000000201435d0, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xa8000000201435b4 targets 0xa8000000201435c8, is taken when (t0(=0x10000000) & a4(=[a0+148])) == 0, and falls through when (t0(=0x10000000) & a4(=[a0+148])) != 0.
  The `beq` branch at 0xa8000000201435d8 targets 0xa8000000201435ac and is always taken.
  The return at 0xa8000000201435c8 has v0 = one of -0x1, 0x0.
- `ioc3_enable_tx` in `6.5.30/unix.IP30` at 0xa8000000201435e0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201435e0-0xa800000020143720 (320 bytes).
  It occupies 320 bytes.
  The `ld` at 0xa8000000201435e0 loads 8 bytes from a0 (arg0 at entry) +0x70.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xa8000000201435e8, `lw` at 0xa80000002014366c.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xa800000020143608, `lw` at 0xa800000020143698.
  4 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa800000020143614, `ld` at 0xa800000020143628, `ld` at 0xa8000000201436ac.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xa800000020143624, `sw` at 0xa8000000201436b0.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xa80000002014362c, `lw` at 0xa800000020143658.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x98, including `lw` at 0xa800000020143678, `lw` at 0xa8000000201436f8.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xa800000020143684, `sw` at 0xa8000000201436e0.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x4, including `sw` at 0xa800000020143688, `sw` at 0xa800000020143714.
  2 such `lw` accesses load 4 bytes from [a0+112]:8 +0x8, including `lw` at 0xa800000020143694, `lw` at 0xa8000000201436e8.
  The `lw` at 0xa8000000201436d8 loads 4 bytes from [a0+64]:8 +0x8.
  The `jal` at 0xa800000020143690 calls enable_intrs [0xa8000000201423b0] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=[a0+64]:8, a5=[a0+112]:8.
  The `jal` at 0xa8000000201436f0 calls disable_intrs [0xa800000020142400] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=[a0+64]:8, a5=[a0+112]:8.
  The `beq` branch at 0xa8000000201435f8 targets 0xa8000000201436c8, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xa800000020143600 targets 0xa8000000201436d0, is taken when a4(?) == 0, and falls through when a4(?) != 0.
  The `bgez` branch at 0xa800000020143610 targets 0xa800000020143670, is taken when at(=([a0+148] << 0x23)) >= 0 (signed), and falls through when at(=([a0+148] << 0x23)) < 0 (signed).
  The `bltz` branch at 0xa800000020143640 targets 0xa80000002014366c, is taken when t1(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t1(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `bne` branch at 0xa800000020143650 targets 0xa800000020143718, is taken when t3(?) != 0, and falls through when t3(?) == 0.
  The `beq` branch at 0xa800000020143664 targets 0xa800000020143650, is taken when (t8(=[[a0+64]:8]) & a7(=0x40000000)) == 0, and falls through when (t8(=[[a0+64]:8]) & a7(=0x40000000)) != 0.
  The `beq` branch at 0xa800000020143670 targets 0xa8000000201436d8, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xa8000000201436a4 targets 0xa8000000201436b8, is taken when (at(=0x10000000) & a0(=[a0+148])) == 0, and falls through when (at(=0x10000000) & a0(=[a0+148])) != 0.
  The `beq` branch at 0xa8000000201436c8 targets 0xa800000020143608, is taken when a4(?) == 0, and falls through when a4(?) != 0.
  The `beq` branch at 0xa8000000201436d0 targets 0xa8000000201436b8 and is always taken.
  The `beq` branch at 0xa800000020143704 targets 0xa800000020143698, is taken when a1(=[a0+152]) == v0(?), and falls through when a1(=[a0+152]) != v0(?).
  The `beq` branch at 0xa800000020143710 targets 0xa800000020143698 and is always taken.
  The `beq` branch at 0xa800000020143718 targets 0xa8000000201436bc and is always taken.
  The return at 0xa8000000201436c0 has v0 = one of -0x1, 0x0.
- `ioc3_read` in `6.5.30/unix.IP30` at 0xa800000020143720 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143720-0xa800000020143bd4 (1204 bytes).
  It occupies 1204 bytes.
  The `ld` at 0xa80000002014372c loads 8 bytes from a0 (arg0 at entry) +0x70.
  8 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa800000020143744, `ld` at 0xa80000002014375c, `ld` at 0xa8000000201437b8.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x9c, including `lw` at 0xa800000020143748, `lw` at 0xa80000002014376c.
  3 such `sw` accesses store 4 bytes to [a0+64]:8 +0x10, including `sw` at 0xa800000020143758, `sw` at 0xa800000020143934, `sw` at 0xa800000020143b90.
  4 such `lw` accesses load 4 bytes from [a0+64]:8 +0xc, including `lw` at 0xa800000020143764, `lw` at 0xa8000000201437bc, `lw` at 0xa800000020143804.
  3 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xa800000020143794, `lw` at 0xa800000020143814, `lw` at 0xa800000020143948.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xa8000000201437b4, `sw` at 0xa800000020143820.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xa8000000201437d4, `lw` at 0xa8000000201437f0.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xa80000002014381c, `sw` at 0xa800000020143b2c.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xa800000020143824, `lw` at 0xa800000020143898, `lw` at 0xa800000020143938.
  The `ld` at 0xa800000020143828 loads 8 bytes from a0 (arg0 at entry) +0x60.
  4 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xa80000002014383c, `sw` at 0xa800000020143abc, `sw` at 0xa800000020143b1c.
  6 such `lw` accesses load 4 bytes from ?, including `lw` at 0xa800000020143858, `lw` at 0xa800000020143918, `lw` at 0xa800000020143a8c.
  7 such `lbu` accesses load 1 byte from ?, including `lbu` at 0xa800000020143874, `lbu` at 0xa8000000201438dc, `lbu` at 0xa8000000201439cc.
  5 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x8c, including `lw` at 0xa800000020143888, `lw` at 0xa800000020143900, `lw` at 0xa8000000201439dc.
  5 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x8, including `ld` at 0xa8000000201438c0, `ld` at 0xa8000000201439b0, `ld` at 0xa8000000201439ec.
  4 such `ld` accesses load 8 bytes from [a0+8]:8 +0x10, including `ld` at 0xa8000000201438cc, `ld` at 0xa8000000201439bc, `ld` at 0xa8000000201439f8.
  4 such `sb` accesses store 1 byte to ?, including `sb` at 0xa8000000201438e8, `sb` at 0xa800000020143a5c, `sb` at 0xa800000020143a70.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x9c, including `sw` at 0xa800000020143944, `sw` at 0xa800000020143ba0.
  The `lw` at 0xa800000020143ac8 loads 4 bytes from [a0+112]:8 +0x10.
  The `ld` at 0xa800000020143bb0 loads 8 bytes from [a0+8]:8 +0x18.
  The `jalr` at 0xa8000000201438d4 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x8, a2=[a0+140], a3=?.
  The `jalr` at 0xa8000000201439c4 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x2, a2=[a0+140], a3=?, a7=[[a0+8]:8+16]:8.
  The `jalr` at 0xa800000020143a00 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x4, a2=?, a3=?.
  The `jalr` at 0xa800000020143a3c calls (unknown callee) with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `jal` at 0xa800000020143ac4 calls enable_intrs [0xa8000000201423b0] with a0=a0 at entry (arg0), a1=[[a0+112]:8+16], a2=?, a3=?.
  The `jalr` at 0xa800000020143bb8 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x0, a2=[a0+140], a3=?.
  The `bne` branch at 0xa80000002014378c targets 0xa800000020143824, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bgez` branch at 0xa8000000201437a0 targets 0xa800000020143b20, is taken when v1(=([a0+148] << 0x23)) >= 0 (signed), and falls through when v1(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xa8000000201437cc targets 0xa80000002014380c, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bgez` branch at 0xa8000000201437dc targets 0xa800000020143804, is taken when t9(=([[a0+64]:8] << 0x24)) >= 0 (signed), and falls through when t9(=([[a0+64]:8] << 0x24)) < 0 (signed).
  The `bne` branch at 0xa8000000201437e8 targets 0xa800000020143ad4, is taken when ra(?) != 0, and falls through when ra(?) == 0.
  The `bne` branch at 0xa8000000201437fc targets 0xa8000000201437e8, is taken when (at(=[[a0+64]:8]) & a6(=0x8000000)) != 0, and falls through when (at(=[[a0+64]:8]) & a6(=0x8000000)) == 0.
  The `beq` branch at 0xa80000002014380c targets 0xa800000020143824, is taken when a7(=one of 0x0, 0x1) == 0, and falls through when a7(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xa800000020143838 targets 0xa80000002014392c, is taken when s5(?) == s1(=[a0+156]), and falls through when s5(?) != s1(=[a0+156]).
  The `blez` branch at 0xa800000020143840 targets 0xa800000020143b38, is taken when s6(=a2) <= 0 (signed), and falls through when s6(=a2) > 0 (signed).
  The `beq` branch at 0xa800000020143860 targets 0xa800000020143adc, is taken when (s4(=0xffffffffc0c0c0c0) & v0(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & v0(?)) != 0.
  The `blez` branch at 0xa800000020143868 targets 0xa800000020143a8c, is taken when s6(?) <= 0 (signed), and falls through when s6(?) > 0 (signed).
  The `beq` branch at 0xa800000020143880 targets 0xa8000000201438ec, is taken when (a0(?) & 0x40) == 0, and falls through when (a0(?) & 0x40) != 0.
  The `beq` branch at 0xa800000020143890 targets 0xa8000000201438b0, is taken when (a2(=[a0+140]) & 0x40) == 0, and falls through when (a2(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xa8000000201438a0 targets 0xa8000000201438b0, is taken when (v0(=[a0+120]) & 0x2) == 0, and falls through when (v0(=[a0+120]) & 0x2) != 0.
  The `beq` branch at 0xa8000000201438a8 targets 0xa800000020143b44, is taken when a6(?) == 0, and falls through when a6(?) != 0.
  The `beq` branch at 0xa8000000201438b0 targets 0xa8000000201438e4, is taken when a5(?) == 0, and falls through when a5(?) != 0.
  The `beq` branch at 0xa8000000201438b8 targets 0xa8000000201438e4, is taken when a7(?) == 0, and falls through when a7(?) != 0.
  The `beq` branch at 0xa8000000201438c4 targets 0xa8000000201438e0, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xa8000000201438f0 targets 0xa800000020143a74, is taken when (a0(?) & 0x80) == 0, and falls through when (a0(?) & 0x80) != 0.
  The `beq` branch at 0xa8000000201438f8 targets 0xa800000020143a4c, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xa800000020143908 targets 0xa800000020143a4c, is taken when (a2(=[a0+140]) & 0x1c) == 0, and falls through when (a2(=[a0+140]) & 0x1c) != 0.
  The `blez` branch at 0xa800000020143910 targets 0xa80000002014399c, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xa800000020143924 targets 0xa800000020143930 and is always taken.
  The `beq` branch at 0xa800000020143940 targets 0xa800000020143968, is taken when (v0(=[a0+120]) & 0x1) == 0, and falls through when (v0(=[a0+120]) & 0x1) != 0.
  The `bne` branch at 0xa800000020143960 targets 0xa800000020143ab4, is taken when a4(?) < a5(?) (unsigned), and falls through when a4(?) >= a5(?) (unsigned).
  The `beq` branch at 0xa80000002014399c targets 0xa8000000201439d4, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xa8000000201439a8 targets 0xa8000000201439d4, is taken when (a2(=[a0+140]) & 0x8) == 0, and falls through when (a2(=[a0+140]) & 0x8) != 0.
  The `beq` branch at 0xa8000000201439b4 targets 0xa8000000201439d4, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xa8000000201439d4 targets 0xa800000020143a10, is taken when t0(?) == 0, and falls through when t0(?) != 0.
  The `beq` branch at 0xa8000000201439e4 targets 0xa800000020143a10, is taken when (t1(=[a0+140]) & 0x10) == 0, and falls through when (t1(=[a0+140]) & 0x10) != 0.
  The `beq` branch at 0xa8000000201439f0 targets 0xa800000020143a10, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xa800000020143a10 targets 0xa800000020143a4c, is taken when t3(?) == 0, and falls through when t3(?) != 0.
  The `beq` branch at 0xa800000020143a20 targets 0xa800000020143a4c, is taken when (t8(=[a0+140]) & 0x4) == 0, and falls through when (t8(=[a0+140]) & 0x4) != 0.
  The `beq` branch at 0xa800000020143a2c targets 0xa800000020143a4c, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xa800000020143a7c targets 0xa800000020143a8c, is taken when s8(?) >= 0x4 (signed), and falls through when s8(?) < 0x4 (signed).
  The `bgtz` branch at 0xa800000020143a84 targets 0xa800000020143874, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xa800000020143a94 targets 0xa800000020143b30, is taken when (s4(=0xffffffffc0c0c0c0) & a0(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a0(?)) != 0.
  The `beq` branch at 0xa800000020143a9c targets 0xa800000020143930, is taken when s5(?) == s1(?), and falls through when s5(?) != s1(?).
  The `bgtz` branch at 0xa800000020143aa4 targets 0xa800000020143850, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xa800000020143aac targets 0xa800000020143934 and is always taken.
  The `beq` branch at 0xa800000020143acc targets 0xa800000020143968 and is always taken.
  The `beq` branch at 0xa800000020143ad4 targets 0xa80000002014396c and is always taken.
  The `beq` branch at 0xa800000020143af8 targets 0xa800000020143b10, is taken when v0(?) == a0(?), and falls through when v0(?) != a0(?).
  The `bne` branch at 0xa800000020143b08 targets 0xa800000020143868, is taken when (s4(=0xffffffffc0c0c0c0) & a3(?)) != 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a3(?)) == 0.
  The `beq` branch at 0xa800000020143b18 targets 0xa800000020143934 and is always taken.
  The `beq` branch at 0xa800000020143b28 targets 0xa8000000201437ac and is always taken.
  The `beq` branch at 0xa800000020143b30 targets 0xa800000020143a9c and is always taken.
  The `beq` branch at 0xa800000020143b3c targets 0xa800000020143934 and is always taken.
  The `blez` branch at 0xa800000020143b44 targets 0xa800000020143b64, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xa800000020143b5c targets 0xa800000020143930 and is always taken.
  The `beq` branch at 0xa800000020143b84 targets 0xa800000020143bc8, is taken when (s4(=0xffffffffc0c0c0c0) & a6(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a6(?)) != 0.
  The `beq` branch at 0xa800000020143b9c targets 0xa800000020143bc0, is taken when (t2(=[a0+140]) & 0x40) == 0, and falls through when (t2(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xa800000020143ba8 targets 0xa800000020143bc0, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xa800000020143bc0 targets 0xa800000020143968 and is always taken.
  The `beq` branch at 0xa800000020143bcc targets 0xa800000020143b8c and is always taken.
  The return at 0xa800000020143994 has v0 = one of ?, -0x1.
- `ioc3_set_DTR` in `6.5.30/unix.IP30` at 0xa800000020143e90 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143e90-0xa800000020143eb0 (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xa800000020143e9c calls set_DTRRTS [0xa800000020143dcc] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x1, a3=0x1000000.
  The return at 0xa800000020143ea8 has v0 = ret(set_DTRRTS).
- `ioc3_set_RTS` in `6.5.30/unix.IP30` at 0xa800000020143eb0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143eb0-0xa800000020143ed0 (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xa800000020143ebc calls set_DTRRTS [0xa800000020143dcc] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x2, a3=0x2000000.
  The return at 0xa800000020143ec8 has v0 = ret(set_DTRRTS).
- `ioc3_query_DCD` in `6.5.30/unix.IP30` at 0xa800000020143ed0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143ed0-0xa800000020143ee4 (20 bytes).
  It occupies 20 bytes.
  The `ld` at 0xa800000020143ed0 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xa800000020143ed4 loads 4 bytes from [a0+64]:8 +0x18.
  The return at 0xa800000020143edc has v0 = ?.
- `ioc3_query_CTS` in `6.5.30/unix.IP30` at 0xa800000020143ee4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143ee4-0xa800000020143ef8 (20 bytes).
  It occupies 20 bytes.
  The `ld` at 0xa800000020143ee4 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xa800000020143ee8 loads 4 bytes from [a0+64]:8 +0x18.
  The return at 0xa800000020143ef0 has v0 = ?.
- `ioc3_get_mapid` in `6.5.30/unix.IP30` at 0xa800000020143f4c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143f4c-0xa800000020143fac (96 bytes).
  It occupies 96 bytes.
  The `ld` at 0xa800000020143f60 loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `jal` at 0xa800000020143f84 calls copyout [0xa80000002001709c] with a0=sp-16, a1=a1 at entry (arg1), a2=0x8, a3=0xa80000002046a068.
  The `beq` branch at 0xa800000020143f54 targets 0xa800000020143f9c, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xa800000020143f94 targets 0xa800000020143fa0 and is always taken.
  The return at 0xa800000020143fa4 has v0 = one of 0x0, 0xe.
- `ioc3_map` in `6.5.30/unix.IP30` at 0xa800000020143fac (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020143fac-0xa80000002014403c (144 bytes).
  It occupies 144 bytes.
  The `ld` at 0xa800000020143fd8 loads 8 bytes from a0 (arg0 at entry) +0x58.
  The `ld` at 0xa80000002014400c loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `ld` at 0xa80000002014401c loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `jal` at 0xa800000020143fe0 calls disable_intrs [0xa800000020142400] with a0=a0 at entry (arg0), a1=-0x1, a2=?, a3=?, a5=0x16.
  The `jal` at 0xa800000020143ff0 calls v_mapphys [0xa8000000201a8698] with a0=a1 at entry (arg1), a1=?, a2=0x4000, a3=?.
  The `beq` branch at 0xa800000020143fb4 targets 0xa800000020143fd0, is taken when a2 == 0, and falls through when a2 != 0.
  The `beq` branch at 0xa800000020143fc0 targets 0xa80000002014400c, is taken when a2 == at(=0x4000), and falls through when a2 != at(=0x4000).
  The `beq` branch at 0xa800000020143fc8 targets 0xa800000020144004 and is always taken.
  The `beq` branch at 0xa800000020144034 targets 0xa800000020143fe0 and is always taken.
  The return at 0xa800000020144004 has v0 = one of 0x16, ret(v_mapphys).
- `ioc3_unmap` in `6.5.30/unix.IP30` at 0xa80000002014403c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa80000002014403c-0xa800000020144084 (72 bytes).
  It occupies 72 bytes.
  The `lw` at 0xa80000002014403c loads 4 bytes from a0 (arg0 at entry) +0x94.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xa800000020144044, `ld` at 0xa800000020144058, `ld` at 0xa80000002014406c.
  The `sw` at 0xa800000020144050 stores 4 bytes to a0 (arg0 at entry) +0x94.
  The `sw` at 0xa800000020144054 stores 4 bytes to [a0+64]:8 +0x0.
  The `lw` at 0xa80000002014405c loads 4 bytes from [a0+64]:8 +0x8.
  The `sw` at 0xa800000020144064 stores 4 bytes to a0 (arg0 at entry) +0x98.
  The `sw` at 0xa800000020144068 stores 4 bytes to [a0+64]:8 +0x4.
  The `lw` at 0xa800000020144070 loads 4 bytes from [a0+64]:8 +0xc.
  The `sw` at 0xa800000020144078 stores 4 bytes to a0 (arg0 at entry) +0x9c.
  The `sw` at 0xa800000020144080 stores 4 bytes to [a0+64]:8 +0x10.
  The return at 0xa80000002014407c has v0 = [a0+64]:8.
- `ioc3_set_sscr` in `6.5.30/unix.IP30` at 0xa800000020144084 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020144084-0xa8000000201440b4 (48 bytes).
  It occupies 48 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xa800000020144084, `lw` at 0xa8000000201440a4.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xa800000020144094, `sw` at 0xa8000000201440b0.
  The `ld` at 0xa800000020144098 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `sw` at 0xa80000002014409c stores 4 bytes to [a0+64]:8 +0x0.
  The `beq` branch at 0xa800000020144088 targets 0xa8000000201440a8, is taken when a2 == 0, and falls through when a2 != 0.
  The `beq` branch at 0xa8000000201440ac targets 0xa800000020144098 and is always taken.
  The return at 0xa8000000201440a0 has v0 = [a0+148].
- `ioc3_console_write` in `6.5.30/unix.IP30` at 0xa8000000201440b4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201440b4-0xa800000020144208 (340 bytes).
  It occupies 340 bytes.
  7 such `lw` accesses load 4 bytes from [0xa8000000204a7800]:8 +0xb8, including `lw` at 0xa8000000201440b8, `lw` at 0xa800000020144194, `lw` at 0xa8000000201441a4.
  The `sd` at 0xa8000000201440e0 stores 8 bytes to a2 (arg2 at entry) +0x0.
  The `sb` at 0xa800000020144108 stores 1 byte to [0xa8000000204a7800]:8 +0x20178.
  6 such `lbu` accesses load 1 byte from [0xa8000000204a7800]:8 +0x2017d, including `lbu` at 0xa80000002014410c, `lbu` at 0xa800000020144124, `lbu` at 0xa800000020144134.
  The `lbu` at 0xa800000020144118 loads 1 byte from ?.
  The `sw` at 0xa800000020144190 stores 4 bytes to [0xa8000000204a7800]:8 +0xb8.
  The `bne` branch at 0xa8000000201440c8 targets 0xa8000000201440e0, is taken when t0(=0x10000000) != t2(?), and falls through when t0(=0x10000000) == t2(?).
  The `beq` branch at 0xa8000000201440d8 targets 0xa8000000201441fc and is always taken.
  The `bne` branch at 0xa8000000201440f4 targets 0xa80000002014410c, is taken when a1 != 0, and falls through when a1 == 0.
  The `beq` branch at 0xa8000000201440fc targets 0xa80000002014417c and is always taken.
  The `beq` branch at 0xa800000020144104 targets 0xa80000002014417c, is taken when a1(?) == 0, and falls through when a1(?) != 0.
  The `bne` branch at 0xa80000002014411c targets 0xa800000020144104, is taken when (a6(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) != 0, and falls through when (a6(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xa80000002014412c targets 0xa800000020144104, is taken when (a7(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) != 0, and falls through when (a7(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xa80000002014413c targets 0xa800000020144104, is taken when (t0(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) != 0, and falls through when (t0(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xa80000002014414c targets 0xa800000020144104, is taken when (t1(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) != 0, and falls through when (t1(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xa80000002014415c targets 0xa800000020144104, is taken when (t2(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) != 0, and falls through when (t2(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) == 0.
  The `beq` branch at 0xa80000002014416c targets 0xa800000020144124, is taken when (t3(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) == 0, and falls through when (t3(=[[0xa8000000204a7800]:8+131453]:1) & 0x20) != 0.
  The `beq` branch at 0xa800000020144174 targets 0xa800000020144104 and is always taken.
  The `bne` branch at 0xa800000020144188 targets 0xa8000000201440e0, is taken when t0(=0x10000000) != t8(?), and falls through when t0(=0x10000000) == t8(?).
  The `bne` branch at 0xa80000002014419c targets 0xa800000020144188, is taken when a5(=0x30000000) != t9(?), and falls through when a5(=0x30000000) == t9(?).
  The `bne` branch at 0xa8000000201441ac targets 0xa800000020144184, is taken when a5(=0x30000000) != v0(?), and falls through when a5(=0x30000000) == v0(?).
  The `bne` branch at 0xa8000000201441bc targets 0xa800000020144184, is taken when a5(=0x30000000) != v1(?), and falls through when a5(=0x30000000) == v1(?).
  The `bne` branch at 0xa8000000201441cc targets 0xa800000020144184, is taken when a5(=0x30000000) != a3(?), and falls through when a5(=0x30000000) == a3(?).
  The `bne` branch at 0xa8000000201441dc targets 0xa800000020144184, is taken when a5(=0x30000000) != t3(?), and falls through when a5(=0x30000000) == t3(?).
  The `beq` branch at 0xa8000000201441ec targets 0xa8000000201441a4, is taken when a5(=0x30000000) == t8(?), and falls through when a5(=0x30000000) != t8(?).
  The `beq` branch at 0xa8000000201441f4 targets 0xa800000020144184 and is always taken.
  The `beq` branch at 0xa800000020144200 targets 0xa800000020144190 and is always taken.
  The return at 0xa80000002014417c has v0 = 0x0.
- `idbg_gdb` in `6.5.30/unix.IP30` at 0xa8000000201609c8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201609c8-0xa8000000201609fc (52 bytes).
  It occupies 52 bytes.
  The `jal` at 0xa8000000201609d8 calls dogang [0xa8000000201605f8] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa8000000201609ec calls idbg_gang [0xa800000020160890] with a0=-0x1, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `beq` branch at 0xa8000000201609d0 targets 0xa8000000201609ec, is taken when a0 == at(=-0x1), and falls through when a0 != at(=-0x1).
  The `beq` branch at 0xa8000000201609f4 targets 0xa8000000201609e4 and is always taken.
  The return at 0xa8000000201609e4 has v0 = one of ret(dogang), ret(idbg_gang).
- `panicregs` in `6.5.30/unix.IP30` at 0xa80000002016bad8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa80000002016bad8-0xa80000002016bb8c (180 bytes).
  It occupies 180 bytes.
  The `ld` at 0xa80000002016bb28 loads 8 bytes from a0 (arg0 at entry) +0x120.
  The `ld` at 0xa80000002016bb54 loads 8 bytes from a0 (arg0 at entry) +0x110.
  The `ld` at 0xa80000002016bb58 loads 8 bytes from table(0xa80000002046a3c0) +0x0.
  The `jal` at 0xa80000002016baf4 calls spl1 [0xa80000002001ba7c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa80000002016bafc calls set_leds [0xa800000020012810] with a0=0x3, a1=?, a2=?, a3=?.
  The `jal` at 0xa80000002016bb1c calls machine_error_dump [0xa800000020187420] with a0=0xa8000000203cfce0, a1=?, a2=?, a3=?.
  The `jal` at 0xa80000002016bb74 calls cmn_err [0xa8000000201c6b00] with a0=0x0, a1=0xa8000000203cfcf0, a2=a4 at entry (arg4), a3=[a0+288]:8, a4=a0 at entry (arg0), a5=a1 at entry (arg1), a6=entry(0xa80000002046a3c0), a7=[a0+272]:8.
  The `bne` branch at 0xa80000002016bb14 targets 0xa80000002016bb24, is taken when v0(=[0xa8000000204a7e78]) != 0, and falls through when v0(=[0xa8000000204a7e78]) == 0.
  The return at 0xa80000002016bb84 has v0 = ret(cmn_err).
- `mem_range_init` in `6.5.30/unix.IP30` at 0xa8000000201727f8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201727f8-0xa800000020172a88 (656 bytes).
  It occupies 656 bytes.
  The `lw` at 0xa80000002017285c loads 4 bytes from [0xa8000000204af8f0]:8 +0x5b4.
  The `sw` at 0xa80000002017286c stores 4 bytes to [0xa8000000204af8f0]:8 +0x5b4.
  The `lw` at 0xa800000020172874 loads 4 bytes from [0xa8000000204af8f0]:8 +0x5c4.
  The `sw` at 0xa800000020172880 stores 4 bytes to [0xa8000000204af8f0]:8 +0x5c4.
  The `lw` at 0xa800000020172888 loads 4 bytes from [0xa8000000204af8f0]:8 +0x640.
  The `sw` at 0xa800000020172894 stores 4 bytes to [0xa8000000204af8f0]:8 +0x640.
  The `ll` at 0xa8000000201728a4 loads 4 bytes from [0xa8000000204af8f0]:8 +0x5bc.
  The `sc` at 0xa8000000201728b0 stores 4 bytes to [0xa8000000204af8f0]:8 +0x5bc.
  3 such `lw` accesses load 4 bytes from ?, including `lw` at 0xa8000000201728e0, `lw` at 0xa80000002017293c, `lw` at 0xa8000000201729b0.
  The `ld` at 0xa8000000201728e8 loads 8 bytes from [0xa8000000204af8f0]:8 +0x5a8.
  4 such `sw` accesses store 4 bytes to ?, including `sw` at 0xa8000000201728f4, `sw` at 0xa8000000201728fc, `sw` at 0xa8000000201729ac.
  The `ld` at 0xa800000020172900 loads 8 bytes from ([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  The `sd` at 0xa800000020172914 stores 8 bytes to ([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  3 such `ld` accesses load 8 bytes from ?, including `ld` at 0xa800000020172948, `ld` at 0xa800000020172998, `ld` at 0xa800000020172a1c.
  9 such `sd` accesses store 8 bytes to ?, including `sd` at 0xa800000020172964, `sd` at 0xa80000002017299c, `sd` at 0xa8000000201729a0.
  The `lw` at 0xa800000020172970 loads 4 bytes from [0xa8000000204af8f0]:8 +0x5dc.
  3 such `ld` accesses load 8 bytes from ([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0, including `ld` at 0xa80000002017297c, `ld` at 0xa8000000201729fc, `ld` at 0xa800000020172a4c.
  2 such `lhu` accesses load 2 bytes from ?, including `lhu` at 0xa800000020172994, `lhu` at 0xa800000020172a08.
  2 such `ld` accesses load 8 bytes from ([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x48, including `ld` at 0xa8000000201729e4, `ld` at 0xa800000020172a34.
  The `sd` at 0xa800000020172a64 stores 8 bytes to ([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x80.
  The `jal` at 0xa800000020172820 calls is_specified [0xa8000000201d7550] with a0=0xa8000000204af953, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa80000002017284c calls setupbadmem [0xa80000002001e4e8] with a0=?, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=?.
  The `jal` at 0xa8000000201729c8 calls bset [0xa800000020225184] with a0=[0xa8000000204af260]:8, a1=?, a2=?, a3=?.
  The `jal` at 0xa8000000201729dc calls bset [0xa800000020225184] with a0=[0xa8000000204af258]:8, a1=?, a2=?, a3=?.
  The `beql` branch at 0xa8000000201728b4 targets 0xa8000000201728a4, is taken when a1(?) == 0, and falls through when a1(?) != 0; its branch-likely delay slot `nop` at 0xa8000000201728b8 runs only when taken and is annulled on fall-through.
  The `bne` branch at 0xa80000002017290c targets 0xa800000020172968, is taken when t8(=[([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) != 0, and falls through when t8(=[([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) == 0.
  The `bne` branch at 0xa800000020172960 targets 0xa800000020172918, is taken when a1(?) < 0x6 (signed), and falls through when a1(?) >= 0x6 (signed).
  The `bltz` branch at 0xa800000020172988 targets 0xa800000020172a54, is taken when s3(?) < 0 (signed), and falls through when s3(?) >= 0 (signed).
  The `beq` branch at 0xa800000020172990 targets 0xa800000020172a0c and is always taken.
  The `bne` branch at 0xa8000000201729f4 targets 0xa800000020172a00, is taken when a2(=[([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s1(?), and falls through when a2(=[([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s1(?).
  The `bltz` branch at 0xa800000020172a00 targets 0xa800000020172a58, is taken when a1(?) < 0 (signed), and falls through when a1(?) >= 0 (signed).
  The `beq` branch at 0xa800000020172a0c targets 0xa800000020172a34, is taken when s4(=0xffff) == a3(?), and falls through when s4(=0xffff) != a3(?).
  The `bne` branch at 0xa800000020172a14 targets 0xa800000020172998, is taken when s5(=ret(is_specified)) != 0, and falls through when s5(=ret(is_specified)) == 0.
  The `beq` branch at 0xa800000020172a2c targets 0xa8000000201729ac and is always taken.
  The `bne` branch at 0xa800000020172a44 targets 0xa800000020172a50, is taken when a5(=[([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s1(?), and falls through when a5(=[([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s1(?).
  The `bgez` branch at 0xa800000020172a50 targets 0xa800000020172a08, is taken when v0(?) >= 0 (signed), and falls through when v0(?) < 0 (signed).
  The return at 0xa800000020172a80 has v0 = one of ret(bset), ?.
- `idbg_init` in `6.5.30/unix.IP30` at 0xa800000020199768 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020199768-0xa8000000201997dc (116 bytes).
  It occupies 116 bytes.
  2 such `sw` accesses store 4 bytes to ?, including `sw` at 0xa800000020199790, `sw` at 0xa8000000201997b0.
  The `jal` at 0xa8000000201997c4 calls init_mutex [0xa800000020153464] with a0=0xa8000000204fabc8, a1=0x0, a2=0xa8000000203d3028, a3=-0x1, a5=0xa8000000204fa098.
  The `beq` branch at 0xa800000020199794 targets 0xa8000000201997b8, is taken when a2(?) >= 0x14 (signed), and falls through when a2(?) < 0x14 (signed).
  The `bne` branch at 0xa8000000201997ac targets 0xa800000020199780, is taken when a2(?) < 0x14 (signed), and falls through when a2(?) >= 0x14 (signed).
  The return at 0xa8000000201997d4 has v0 = 0x1.
- `idbg_id` in `6.5.30/unix.IP30` at 0xa8000000201997dc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201997dc-0xa8000000201997e4 (8 bytes).
  It occupies 8 bytes.
  The `lw` at 0xa8000000201997e0 loads 4 bytes from a0 (arg0 at entry) +0x3c.
  The return at 0xa8000000201997dc has v0 = [a0+60].
- `idbg_lock` in `6.5.30/unix.IP30` at 0xa8000000201997e4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201997e4-0xa8000000201997ec (8 bytes).
  It occupies 8 bytes.
  The return at 0xa8000000201997e4 has v0 = 0x0.
- `idbg_unlock` in `6.5.30/unix.IP30` at 0xa8000000201997ec (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201997ec-0xa8000000201997f4 (8 bytes).
  It occupies 8 bytes.
  The return at 0xa8000000201997ec has v0 = ?.
- `idbg_copydata` in `6.5.30/unix.IP30` at 0xa8000000201997f4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201997f4-0xa8000000201997fc (8 bytes).
  It occupies 8 bytes.
  The return at 0xa8000000201997f4 has v0 = ?.
- `idbg_makedata` in `6.5.30/unix.IP30` at 0xa8000000201997fc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201997fc-0xa800000020199874 (120 bytes).
  It occupies 120 bytes.
  The `ld` at 0xa800000020199824 loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `sd` at 0xa800000020199864 stores 8 bytes to ret(kern_calloc) +0x8.
  The `sd` at 0xa800000020199868 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xa800000020199814 calls kern_calloc [0xa8000000201487d8] with a0=0x1, a1=0x18, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa800000020199828 calls copyin [0xa800000020017010] with a0=[a0+24]:8, a1=ret(kern_calloc), a2=0x4, a3=?.
  The `jal` at 0xa800000020199838 calls kern_free [0xa8000000201488f0] with a0=ret(kern_calloc), a1=0xa0, a2=?, a3=?.
  The `jal` at 0xa80000002019985c calls kern_calloc [0xa8000000201487d8] with a0=0x1, a1=0xa0, a2=?, a3=?.
  The `beq` branch at 0xa800000020199830 targets 0xa80000002019985c, is taken when v0(=ret(copyin)) == 0, and falls through when v0(=ret(copyin)) != 0.
  The `beq` branch at 0xa80000002019986c targets 0xa800000020199844 and is always taken.
  The return at 0xa800000020199854 has v0 = one of 0x0, 0xa.
- `idbg_freedata` in `6.5.30/unix.IP30` at 0xa800000020199874 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020199874-0xa8000000201998bc (72 bytes).
  It occupies 72 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x18, including `ld` at 0xa800000020199874, `ld` at 0xa80000002019989c.
  The `ld` at 0xa800000020199888 loads 8 bytes from [a0+24]:8 +0x8.
  The `sd` at 0xa8000000201998ac stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xa800000020199894 calls kern_free [0xa8000000201488f0] with a0=[[a0+24]:8+8]:8, a1=a1 at entry (arg1), a2=[a0+24]:8, a3=a3 at entry (arg3).
  The `jal` at 0xa8000000201998a0 calls kern_free [0xa8000000201488f0] with a0=[a0+24]:8, a1=?, a2=[a0+24]:8, a3=?.
  The `beq` branch at 0xa800000020199880 targets 0xa8000000201998b0, is taken when a2(=[a0+24]:8) == 0, and falls through when a2(=[a0+24]:8) != 0.
  The `beq` branch at 0xa80000002019988c targets 0xa8000000201998a0, is taken when a0(=[[a0+24]:8+8]:8) == 0, and falls through when a0(=[[a0+24]:8+8]:8) != 0.
  The return at 0xa8000000201998b4 has v0 = one of ?, ret(kern_free).
- `idbg_load` in `6.5.30/unix.IP30` at 0xa800000020199cf4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020199cf4-0xa800000020199d98 (164 bytes).
  It occupies 164 bytes.
  The `ld` at 0xa800000020199d38 loads 8 bytes from a0 (arg0 at entry) +0x28.
  The `ld` at 0xa800000020199d40 loads 8 bytes from [a0+40]:8 +0x50.
  The `jal` at 0xa800000020199d10 calls mutex_lock [0xa8000000201535f4] with a0=0xa8000000204fabc8, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xa800000020199d18 calls debug_check [0xa800000020199bd4] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020199d30 calls debug_freeid [0xa800000020199b94] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020199d5c calls mutex_unlock [0xa80000002015352c] with a0=0xa8000000204fabc8, a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020199d78 calls mloadfile [0xa80000002019ac9c] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020199d88 calls debug_init [0xa8000000201998bc] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xa800000020199d20 targets 0xa800000020199d78, is taken when v0(=ret(debug_check)) == 0, and falls through when v0(=ret(debug_check)) != 0.
  The `beq` branch at 0xa800000020199d28 targets 0xa800000020199d58, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `bne` branch at 0xa800000020199d80 targets 0xa800000020199d28, is taken when v0(=ret(mloadfile)) != 0, and falls through when v0(=ret(mloadfile)) == 0.
  The `beq` branch at 0xa800000020199d90 targets 0xa800000020199d28 and is always taken.
  The return at 0xa800000020199d70 has v0 = ?.
- `idbg_unload` in `6.5.30/unix.IP30` at 0xa800000020199d98 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020199d98-0xa800000020199f04 (364 bytes).
  It occupies 364 bytes.
  The `ld` at 0xa800000020199db0 loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `ld` at 0xa800000020199dc8 loads 8 bytes from a0 (arg0 at entry) +0x50.
  The `ld` at 0xa800000020199df4 loads 8 bytes from [a0+24]:8 +0x8.
  The `ld` at 0xa800000020199dfc loads 8 bytes from [[a0+24]:8+8]:8 +0x68.
  The `sw` at 0xa800000020199e68 stores 4 bytes to abs 0xa8000000204fab78.
  The `ld` at 0xa800000020199e70 loads 8 bytes from [a0+24]:8 +0x10.
  The `ld` at 0xa800000020199ed8 loads 8 bytes from [0xa8000000204af2d0]:8 +0x28.
  The `lw` at 0xa800000020199edc loads 4 bytes from [[0xa8000000204af2d0]:8+40]:8 +0x48.
  The `sw` at 0xa800000020199ee8 stores 4 bytes to [[0xa8000000204af2d0]:8+40]:8 +0x48.
  The `ld` at 0xa800000020199eec loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `sw` at 0xa800000020199f00 stores 4 bytes to table(0xa8000000204fab78) +0x0.
  The `jal` at 0xa800000020199db8 calls mutex_lock [0xa8000000201535f4] with a0=0xa8000000204fabc8, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jalr` at 0xa800000020199e48 calls (unknown callee) with a0=?, a1=?, a2=?, a3=-0x1, a4=0xc000000000000000, a5=0xc0000fbf00000000, a6=0xe000.
  The `jal` at 0xa800000020199e58 calls mutex_unlock [0xa80000002015352c] with a0=0xa8000000204fabc8, a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020199e74 calls symtab_rele [0xa80000002019a76c] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020199e80 calls mutex_unlock [0xa80000002015352c] with a0=0xa8000000204fabc8, a1=?, a2=?, a3=?.
  The `jal` at 0xa800000020199eac calls mfindname [0xa80000002019b26c] with a0=[a0+80]:8, a1=0xa8000000203d2d38, a2=sp-32, a3=-0x1, a4=0xc000000000000000, a5=0xc0000fbf00000000, a6=0xe000, a7=0xffffffff00000000.
  The `jal` at 0xa800000020199ebc calls mutex_unlock [0xa80000002015352c] with a0=0xa8000000204fabc8, a1=?, a2=?, a3=?.
  The `jalr` at 0xa800000020199ecc calls (unknown callee) with a0=?, a1=?, a2=?, a3=?.
  The `bne` branch at 0xa800000020199dec targets 0xa800000020199ea0, is taken when v0(=[0xa8000000204af2d0]:8) != a0(=[a0+80]:8), and falls through when v0(=[0xa8000000204af2d0]:8) == a0(=[a0+80]:8).
  The `bne` branch at 0xa800000020199e20 targets 0xa800000020199e30, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) < t3(=0xa800000000000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) >= t3(=0xa800000000000000) (unsigned).
  The `bne` branch at 0xa800000020199e28 targets 0xa800000020199e48, is taken when a7(?) != 0, and falls through when a7(?) == 0.
  The `bne` branch at 0xa800000020199e30 targets 0xa800000020199e68, is taken when a0(?) != 0, and falls through when a0(?) == 0.
  The `beq` branch at 0xa800000020199e40 targets 0xa800000020199e68, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) >= t9(=0xc0000fbfe0000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) < t9(=0xc0000fbfe0000000) (unsigned).
  The `beq` branch at 0xa800000020199e50 targets 0xa800000020199e68, is taken when v0(=ret(?)) == 0, and falls through when v0(=ret(?)) != 0.
  The `beq` branch at 0xa800000020199e60 targets 0xa800000020199e8c and is always taken.
  The `beq` branch at 0xa800000020199eb4 targets 0xa800000020199ecc, is taken when v0(=ret(mfindname)) == 0, and falls through when v0(=ret(mfindname)) != 0.
  The `beq` branch at 0xa800000020199ec4 targets 0xa800000020199e8c and is always taken.
  The `beq` branch at 0xa800000020199efc targets 0xa800000020199e7c and is always taken.
  The return at 0xa800000020199e98 has v0 = one of 0x0, 0x33.
- `idbg_reg` in `6.5.30/unix.IP30` at 0xa800000020199f04 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020199f04-0xa800000020199f0c (8 bytes).
  It occupies 8 bytes.
  The return at 0xa800000020199f04 has v0 = 0x59.
- `idbg_unreg` in `6.5.30/unix.IP30` at 0xa800000020199f0c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa800000020199f0c-0xa800000020199f14 (8 bytes).
  It occupies 8 bytes.
  The return at 0xa800000020199f0c has v0 = 0x59.
- `setup_lowmem` in `6.5.30/unix.IP30` at 0xa8000000201b3a78 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP30`.
  Range: 0xa8000000201b3a78-0xa8000000201b3dc8 (848 bytes).
  It occupies 848 bytes.
  The `sd` at 0xa8000000201b3b7c stores 8 bytes to ret(low_mem_alloc) +0x5a8.
  The `ld` at 0xa8000000201b3bd4 loads 8 bytes from [0xa8000000204af8f0]:8 +0x5a8.
  The `sw` at 0xa8000000201b3c7c stores 4 bytes to ret(low_mem_alloc) +0x5dc.
  The `sd` at 0xa8000000201b3ca0 stores 8 bytes to ([[0xa8000000204af8f0]:8+1448]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0.
  The `sw` at 0xa8000000201b3ccc stores 4 bytes to ?.
  The `sd` at 0xa8000000201b3cf0 stores 8 bytes to ?.
  The `jal` at 0xa8000000201b3b08 calls blkclr [0xa80000002001738c] with a0=?, a1=?, a2=0xa800000000000000, a3=a3 at entry (arg3), a5=[one of 0xa8000000204af76c, 0xa8000000204af774], a6=[0xa8000000204afab4].
  The `jal` at 0xa8000000201b3b10 calls flush_cache [0xa80000002001b640] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xa8000000201b3b20 calls mktables [0xa8000000201b3dc8] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xa8000000201b3b3c calls low_mem_alloc [0xa8000000201b379c] with a0=0xcb8, a1=sp-112, a2=0xa8000000203d40f8, a3=?.
  The `jal` at 0xa8000000201b3b64 calls low_mem_alloc [0xa8000000201b379c] with a0=(((([0xa8000000204af8d0] << 0x4) + [0xa8000000204af8d0]) << 0x3) << 0x0), a1=sp-112, a2=0xa8000000203d4108, a3=?.
  The `jal` at 0xa8000000201b3b80 calls low_mem_alloc [0xa8000000201b379c] with a0=?, a1=sp-112, a2=0xa8000000203d4120, a3=?.
  The `jal` at 0xa8000000201b3b98 calls low_mem_alloc [0xa8000000201b379c] with a0=?, a1=sp-112, a2=0xa8000000203d4138, a3=?.
  The `jal` at 0xa8000000201b3c8c calls low_mem_alloc [0xa8000000201b379c] with a0=?, a1=sp-112, a2=0xa8000000203d4150, a3=?, a4=[0xa8000000204af794].
  The `jal` at 0xa8000000201b3cd0 calls low_mem_alloc [0xa8000000201b379c] with a0=?, a1=sp-112, a2=0xa8000000203d4150, a3=?.
  The `jal` at 0xa8000000201b3db0 calls cmn_err_tag [0xa8000000201c6b4c] with a0=0x6ec, a1=0x4, a2=0xa8000000203d4170, a3=?.
  The `beq` branch at 0xa8000000201b3ab0 targets 0xa8000000201b3d88, is taken when at(=0x60000) >= a6(=[0xa8000000204afab4]) (unsigned), and falls through when at(=0x60000) < a6(=[0xa8000000204afab4]) (unsigned).
  The `beq` branch at 0xa8000000201b3acc targets 0xa8000000201b3ae4, is taken when a0(?) >= v0(?) (unsigned), and falls through when a0(?) < v0(?) (unsigned).
  The `beq` branch at 0xa8000000201b3adc targets 0xa8000000201b3ae8 and is always taken.
  The `bne` branch at 0xa8000000201b3bf0 targets 0xa8000000201b3bfc, is taken when a1(?) < a2(?) (signed), and falls through when a1(?) >= a2(?) (signed).
  The `bne` branch at 0xa8000000201b3c14 targets 0xa8000000201b3c20, is taken when a2(?) < s1(?) (signed), and falls through when a2(?) >= s1(?) (signed).
  The `bne` branch at 0xa8000000201b3c38 targets 0xa8000000201b3c44, is taken when a2(?) < s1(?) (signed), and falls through when a2(?) >= s1(?) (signed).
  The `bne` branch at 0xa8000000201b3c64 targets 0xa8000000201b3c70, is taken when a1(?) < s1(?) (signed), and falls through when a1(?) >= s1(?) (signed).
  The `beq` branch at 0xa8000000201b3cec targets 0xa8000000201b3ca8, is taken when s3(=0x1000000) >= s2(?) (signed), and falls through when s3(=0x1000000) < s2(?) (signed).
  The `bne` branch at 0xa8000000201b3d4c targets 0xa8000000201b3da0, is taken when t0(?) < a6(?) (unsigned), and falls through when t0(?) >= a6(?) (unsigned).
  The `bne` branch at 0xa8000000201b3d90 targets 0xa8000000201b3ae4, is taken when v0(?) < 0x801 (unsigned), and falls through when v0(?) >= 0x801 (unsigned).
  The `beq` branch at 0xa8000000201b3d98 targets 0xa8000000201b3ad4 and is always taken.
  The `beq` branch at 0xa8000000201b3dc0 targets 0xa8000000201b3d54 and is always taken.
  The return at 0xa8000000201b3d80 has v0 = ?.
- `ioc3_early_pckminit` in `6.5.30/unix.IP35` at 0xc0000000000cb854 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000000cb854-0xc0000000000cb85c (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000000cb854 has v0 = ?.
- `ioc3_pckm_kbdexists` in `6.5.30/unix.IP35` at 0xc0000000000cd074 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000000cd074-0xc0000000000cd0b0 (60 bytes).
  It occupies 60 bytes.
  The `lw` at 0xc0000000000cd080 loads 4 bytes from [0xc000000001644df8]:8 +0x28.
  The `ld` at 0xc0000000000cd090 loads 8 bytes from ([0xc000000001644df8]:8 + ([[0xc000000001644df8]:8+40] << 0x3)) +0x18.
  The `lw` at 0xc0000000000cd094 loads 4 bytes from [([0xc000000001644df8]:8 + ([[0xc000000001644df8]:8+40] << 0x3))+24]:8 +0x4.
  The `beq` branch at 0xc0000000000cd078 targets 0xc0000000000cd0a8, is taken when a2(=[0xc000000001644df8]:8) == 0, and falls through when a2(=[0xc000000001644df8]:8) != 0.
  The `beq` branch at 0xc0000000000cd084 targets 0xc0000000000cd0a8, is taken when a3(=[[0xc000000001644df8]:8+40]) == v0(=0x13), and falls through when a3(=[[0xc000000001644df8]:8+40]) != v0(=0x13).
  The return at 0xc0000000000cd0a0 has v0 = ?.
  The return at 0xc0000000000cd0a8 has v0 = 0x0.
- `ioc3_pckm_setleds` in `6.5.30/unix.IP35` at 0xc0000000000cd0b0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000000cd0b0-0xc0000000000cd0f0 (64 bytes).
  It occupies 64 bytes.
  The `lw` at 0xc0000000000cd0c8 loads 4 bytes from [0xc000000001644df8]:8 +0x28.
  The `ld` at 0xc0000000000cd0e0 loads 8 bytes from ([0xc000000001644df8]:8 + ([[0xc000000001644df8]:8+40] << 0x3)) +0x18.
  The `jal` at 0xc0000000000cd0dc calls ioc3_lcl_pckm_setleds [0xc0000000000cd168] with a0=[([0xc000000001644df8]:8 + ([[0xc000000001644df8]:8+40] << 0x3))+24]:8, a1=a0 at entry (arg0), a2=a2 at entry (arg2), a3=[0xc000000001644df8]:8, a4=0x13, a5=[[0xc000000001644df8]:8+40].
  The `beq` branch at 0xc0000000000cd0c0 targets 0xc0000000000cd0e4, is taken when a3(=[0xc000000001644df8]:8) == 0, and falls through when a3(=[0xc000000001644df8]:8) != 0.
  The `beq` branch at 0xc0000000000cd0cc targets 0xc0000000000cd0e8, is taken when a5(=[[0xc000000001644df8]:8+40]) == a4(=0x13), and falls through when a5(=[[0xc000000001644df8]:8+40]) != a4(=0x13).
  The return at 0xc0000000000cd0e8 has v0 = ?.
- `ioc3_pckm_bell` in `6.5.30/unix.IP35` at 0xc0000000000cd0f0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000000cd0f0-0xc0000000000cd134 (68 bytes).
  It occupies 68 bytes.
  The `lw` at 0xc0000000000cd104 loads 4 bytes from [0xc000000001644df8]:8 +0x28.
  The `lw` at 0xc0000000000cd110 loads 4 bytes from [0xc000000001644df8]:8 +0x34.
  The `ld` at 0xc0000000000cd124 loads 8 bytes from ([0xc000000001644df8]:8 + ([[0xc000000001644df8]:8+40] << 0x3)) +0x18.
  The `jal` at 0xc0000000000cd120 calls ioc3_lcl_pckm_setleds [0xc0000000000cd168] with a0=[([0xc000000001644df8]:8 + ([[0xc000000001644df8]:8+40] << 0x3))+24]:8, a1=([[0xc000000001644df8]:8+52] | 0x8), a2=a2 at entry (arg2), a3=[0xc000000001644df8]:8, a4=[[0xc000000001644df8]:8+40].
  The `beq` branch at 0xc0000000000cd0fc targets 0xc0000000000cd128, is taken when a3(=[0xc000000001644df8]:8) == 0, and falls through when a3(=[0xc000000001644df8]:8) != 0.
  The `beq` branch at 0xc0000000000cd108 targets 0xc0000000000cd12c, is taken when a4(=[[0xc000000001644df8]:8+40]) == a1(=0x13), and falls through when a4(=[[0xc000000001644df8]:8+40]) != a1(=0x13).
  The return at 0xc0000000000cd12c has v0 = ?.
- `ioc3_gl_setporthandler` in `6.5.30/unix.IP35` at 0xc0000000000cd134 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000000cd134-0xc0000000000cd160 (44 bytes).
  It occupies 44 bytes.
  The `lw` at 0xc0000000000cd140 loads 4 bytes from [0xc000000001644df8]:8 +0x28.
  The `ld` at 0xc0000000000cd150 loads 8 bytes from ([0xc000000001644df8]:8 + ([[0xc000000001644df8]:8+40] << 0x3)) +0x18.
  The `sd` at 0xc0000000000cd154 stores 8 bytes to [([0xc000000001644df8]:8 + ([[0xc000000001644df8]:8+40] << 0x3))+24]:8 +0x18.
  The `beq` branch at 0xc0000000000cd138 targets 0xc0000000000cd158, is taken when a2(=[0xc000000001644df8]:8) == 0, and falls through when a2(=[0xc000000001644df8]:8) != 0.
  The `beq` branch at 0xc0000000000cd144 targets 0xc0000000000cd158, is taken when a4(=[[0xc000000001644df8]:8+40]) == a3(=0x13), and falls through when a4(=[[0xc000000001644df8]:8+40]) != a3(=0x13).
  The return at 0xc0000000000cd158 has v0 = ?.
- `ioc3_du_keyboard_port` in `6.5.30/unix.IP35` at 0xc0000000000cd160 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000000cd160-0xc0000000000cd168 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc0000000000cd160 has v0 = -0x1.
- `ioc3_lcl_pckm_setleds` in `6.5.30/unix.IP35` at 0xc0000000000cd168 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000000cd168-0xc0000000000cd1bc (84 bytes).
  It occupies 84 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x48, including `ld` at 0xc0000000000cd174, `ld` at 0xc0000000000cd19c.
  The `jal` at 0xc0000000000cd188 calls mutex_lock [0xc0000000002386b8] with a0=[a0+72]:8+72, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000000cd194 calls ioc3_lcl_pckm_setleds_lock [0xc0000000000cd1bc] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xc0000000000cd1a4 calls mutex_unlock [0xc0000000002385f0] with a0=[a0+72]:8+72, a1=?, a2=?, a3=?.
  The return at 0xc0000000000cd1b4 has v0 = ret(mutex_unlock).
- `ioc3_lcl_pckm_setleds_lock` in `6.5.30/unix.IP35` at 0xc0000000000cd1bc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000000cd1bc-0xc0000000000cd330 (372 bytes).
  It occupies 372 bytes.
  The `ld` at 0xc0000000000cd1c8 loads 8 bytes from a0 (arg0 at entry) +0x48.
  5 such `lw` accesses load 4 bytes from [a0+72]:8 +0x34, including `lw` at 0xc0000000000cd1d0, `lw` at 0xc0000000000cd244, `lw` at 0xc0000000000cd264.
  The `lw` at 0xc0000000000cd200 loads 4 bytes from [a0+72]:8 +0x38.
  The `sw` at 0xc0000000000cd22c stores 4 bytes to [a0+72]:8 +0x38.
  2 such `sw` accesses store 4 bytes to [a0+72]:8 +0x34, including `sw` at 0xc0000000000cd238, `sw` at 0xc0000000000cd26c.
  The `jal` at 0xc0000000000cd240 calls (unnamed) [0xc0000000000ccf98] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xc0000000000cd24c calls (unnamed) [0xc0000000000cd3f8] with a0=ret(0xc0000000000ccf98), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc0000000000cd298 calls (unnamed) [0xc0000000000ccf98] with a0=a0 at entry (arg0), a1=0xed, a2=?, a3=?.
  The `jal` at 0xc0000000000cd2a4 calls (unnamed) [0xc0000000000cd3f8] with a0=ret(0xc0000000000ccf98), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc0000000000cd2c4 calls (unnamed) [0xc0000000000ccf98] with a0=a0 at entry (arg0), a1=0xf4, a2=?, a3=?.
  The `jal` at 0xc0000000000cd2d0 calls (unnamed) [0xc0000000000cd3f8] with a0=ret(0xc0000000000ccf98), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc0000000000cd2e4 calls (unnamed) [0xc0000000000cc894] with a0=a0 at entry (arg0), a1=0x0, a2=[[a0+72]:8+52], a3=?.
  The `jal` at 0xc0000000000cd304 calls (unnamed) [0xc0000000000ccf98] with a0=a0 at entry (arg0), a1=[[a0+72]:8+52], a2=?, a3=?.
  The `jal` at 0xc0000000000cd310 calls (unnamed) [0xc0000000000cd3f8] with a0=ret(0xc0000000000ccf98), a1=0xfa, a2=?, a3=?.
  The `jal` at 0xc0000000000cd320 calls (unnamed) [0xc0000000000cc820] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xc0000000000cd1f0 targets 0xc0000000000cd270, is taken when at(=[[a0+72]:8+52]) == a3(?), and falls through when at(=[[a0+72]:8+52]) != a3(?).
  The `beq` branch at 0xc0000000000cd1f8 targets 0xc0000000000cd234, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xc0000000000cd20c targets 0xc0000000000cd21c, is taken when a7(?) >= 0xe (signed), and falls through when a7(?) < 0xe (signed).
  The `bne` branch at 0xc0000000000cd214 targets 0xc0000000000cd274, is taken when a4(?) != 0, and falls through when a4(?) == 0.
  The `beq` branch at 0xc0000000000cd21c targets 0xc0000000000cd22c, is taken when a0(?) == 0, and falls through when a0(?) != 0.
  The `bne` branch at 0xc0000000000cd224 targets 0xc0000000000cd274, is taken when a5(?) != 0, and falls through when a5(?) == 0.
  The `beq` branch at 0xc0000000000cd234 targets 0xc0000000000cd294 and is always taken.
  The `beq` branch at 0xc0000000000cd254 targets 0xc0000000000cd304, is taken when v0(=ret(0xc0000000000cd3f8)) == 0, and falls through when v0(=ret(0xc0000000000cd3f8)) != 0.
  The `beq` branch at 0xc0000000000cd25c targets 0xc0000000000cd274, is taken when s2(?) == 0, and falls through when s2(?) != 0.
  The `beq` branch at 0xc0000000000cd290 targets 0xc0000000000cd2f4, is taken when t1(?) == 0, and falls through when t1(?) != 0.
  The `bne` branch at 0xc0000000000cd2ac targets 0xc0000000000cd23c, is taken when v0(=ret(0xc0000000000cd3f8)) != 0, and falls through when v0(=ret(0xc0000000000cd3f8)) == 0.
  The `bne` branch at 0xc0000000000cd2b8 targets 0xc0000000000cd28c, is taken when s3(?) < 0x2 (unsigned), and falls through when s3(?) >= 0x2 (unsigned).
  The `bne` branch at 0xc0000000000cd2d8 targets 0xc0000000000cd28c, is taken when v0(=ret(0xc0000000000cd3f8)) != 0, and falls through when v0(=ret(0xc0000000000cd3f8)) == 0.
  The `beq` branch at 0xc0000000000cd2ec targets 0xc0000000000cd290 and is always taken.
  The `bne` branch at 0xc0000000000cd2f4 targets 0xc0000000000cd264, is taken when s2(?) != 0, and falls through when s2(?) == 0.
  The `beq` branch at 0xc0000000000cd2fc targets 0xc0000000000cd274 and is always taken.
  The `bne` branch at 0xc0000000000cd318 targets 0xc0000000000cd25c, is taken when v0(=ret(0xc0000000000cd3f8)) != 0, and falls through when v0(=ret(0xc0000000000cd3f8)) == 0.
  The `beq` branch at 0xc0000000000cd328 targets 0xc0000000000cd25c and is always taken.
  The return at 0xc0000000000cd284 has v0 = ?.
- `pciio_pio_read32` in `6.5.30/unix.IP35` at 0xc00000000013d654 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc00000000013d654-0xc00000000013d65c (8 bytes).
  It occupies 8 bytes.
  The `lw` at 0xc00000000013d658 loads 4 bytes from a0 (arg0 at entry) +0x0.
  The return at 0xc00000000013d654 has v0 = [a0].
- `pciio_pio_write32` in `6.5.30/unix.IP35` at 0xc00000000013d65c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc00000000013d65c-0xc00000000013d664 (8 bytes).
  It occupies 8 bytes.
  The `sw` at 0xc00000000013d660 stores 4 bytes to a1 (arg1 at entry) +0x0.
  The return at 0xc00000000013d65c has v0 = ?.
- `dev_chain` in `6.5.30/unix.IP35` at 0xc0000000001df510 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000001df510-0xc0000000001df5fc (236 bytes).
  It occupies 236 bytes.
  The `ld` at 0xc0000000001df51c loads 8 bytes from a0 (arg0 at entry) +0x38.
  2 such `lw` accesses load 4 bytes from abs 0xc0000000016b58b8 (v+48), including `lw` at 0xc0000000001df530, `lw` at 0xc0000000001df594.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x50, including `lw` at 0xc0000000001df538, `lw` at 0xc0000000001df57c.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x28, including `ld` at 0xc0000000001df558, `ld` at 0xc0000000001df56c.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x30, including `ld` at 0xc0000000001df55c, `ld` at 0xc0000000001df564.
  The `sd` at 0xc0000000001df560 stores 8 bytes to [a0+40]:8 +0x30.
  The `sd` at 0xc0000000001df578 stores 8 bytes to [a0+48]:8 +0x28.
  2 such `ld` accesses load 8 bytes from ?, including `ld` at 0xc0000000001df5b4, `ld` at 0xc0000000001df5c0.
  The `sd` at 0xc0000000001df5b8 stores 8 bytes to a0 (arg0 at entry) +0x30.
  The `sd` at 0xc0000000001df5bc stores 8 bytes to a0 (arg0 at entry) +0x28.
  2 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc0000000001df5c4, `sd` at 0xc0000000001df5cc.
  The `sd` at 0xc0000000001df5d8 stores 8 bytes to a0 (arg0 at entry) +0x38.
  The `jal` at 0xc0000000001df550 calls mutex_spinlock [0xc000000000331f6c] with a0=[a0+56]:8+56, a1=a1 at entry (arg1), a2=[a0+56]:8, a3=[0xc0000000016b58b8].
  The `jal` at 0xc0000000001df574 calls mutex_spinunlock [0xc000000000331fb8] with a0=[a0+56]:8+56, a1=ret(mutex_spinlock), a2=?, a3=?, a6=[a0+40]:8, a7=[a0+48]:8.
  The `jal` at 0xc0000000001df5ac calls mutex_spinlock [0xc000000000331f6c] with a0=?, a1=?, a2=0xc000000001696500, a3=0xc0000000016b5888.
  The `jal` at 0xc0000000001df5d4 calls mutex_spinunlock [0xc000000000331fb8] with a0=?, a1=ret(mutex_spinlock), a2=?, a3=?.
  The `beq` branch at 0xc0000000001df52c targets 0xc0000000001df57c, is taken when a2(=[a0+56]:8) == 0, and falls through when a2(=[a0+56]:8) != 0.
  The `beq` branch at 0xc0000000001df548 targets 0xc0000000001df5e0, is taken when a2(=[a0+56]:8) == a4(?), and falls through when a2(=[a0+56]:8) != a4(?).
  The `beq` branch at 0xc0000000001df58c targets 0xc0000000001df5f4, is taken when a0(=[a0+80]) == t2(=-0x1), and falls through when a0(=[a0+80]) != t2(=-0x1).
  The `beq` branch at 0xc0000000001df5f4 targets 0xc0000000001df5a8 and is always taken.
  The return at 0xc0000000001df5ec has v0 = one of ?, ret(mutex_spinunlock).
- `hub_driver_event_handler` in `6.5.30/unix.IP35` at 0xc0000000002049a0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000002049a0-0xc000000000204a60 (192 bytes).
  It occupies 192 bytes.
  The `lw` at 0xc0000000002049a0 loads 4 bytes from a0 (arg0 at entry) +0x0.
  The `lw` at 0xc0000000002049dc loads 4 bytes from abs 0xc00000000158db8c (usb_dbg_level+20).
  4 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x8, including `ld` at 0xc000000000204a04, `ld` at 0xc000000000204a24, `ld` at 0xc000000000204a34.
  The `jal` at 0xc000000000204a00 calls usb_iface_class [0xc0000000001fb160] with a0=[a0+8]:8, a1=[a0], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=0x1, a5=0x2, a6=0xc00000000158db78.
  The `jal` at 0xc000000000204a20 calls usb_hub_attach [0xc000000000204a60] with a0=[a0+8]:8, a1=[a0], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=0x1, a5=0x2, a6=0xc00000000158db78.
  The `jal` at 0xc000000000204a30 calls usb_hub_detach [0xc0000000002050d8] with a0=[a0+8]:8, a1=[a0], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=0x1, a5=0x2, a6=0xc00000000158db78.
  The `jal` at 0xc000000000204a40 calls usb_hub_detach [0xc0000000002050d8] with a0=[a0+8]:8, a1=[a0], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=0x1, a5=0x2, a6=0xc00000000158db78.
  The `jal` at 0xc000000000204a50 calls usb_dbg [0xc000000000206d84] with a0=0xc0000000004c1d60, a1=[a0], a2=a2 at entry (arg2), a3=a3 at entry (arg3), a4=0x1, a5=0x2, a6=0xc00000000158db78.
  The `beq` branch at 0xc0000000002049bc targets 0xc000000000204a00, is taken when a1(=[a0]) == at(=0x3), and falls through when a1(=[a0]) != at(=0x3).
  The `beq` branch at 0xc0000000002049c4 targets 0xc000000000204a20, is taken when a1(=[a0]) == 0, and falls through when a1(=[a0]) != 0.
  The `beq` branch at 0xc0000000002049cc targets 0xc000000000204a30, is taken when a1(=[a0]) == a4(=0x1), and falls through when a1(=[a0]) != a4(=0x1).
  The `beq` branch at 0xc0000000002049d4 targets 0xc000000000204a40, is taken when a1(=[a0]) == a5(=0x2), and falls through when a1(=[a0]) != a5(=0x2).
  The `bgtz` branch at 0xc0000000002049e0 targets 0xc000000000204a50, is taken when v0(=[0xc00000000158db8c]) > 0 (signed), and falls through when v0(=[0xc00000000158db8c]) <= 0 (signed).
  The `beq` branch at 0xc000000000204a18 targets 0xc0000000002049f0 and is always taken.
  The `beq` branch at 0xc000000000204a28 targets 0xc0000000002049f0 and is always taken.
  The `beq` branch at 0xc000000000204a38 targets 0xc0000000002049f0 and is always taken.
  The `beq` branch at 0xc000000000204a48 targets 0xc0000000002049f0 and is always taken.
  The `beq` branch at 0xc000000000204a58 targets 0xc0000000002049ec and is always taken.
  The return at 0xc0000000002049f8 has v0 = one of 0x625f0005, 0x625f0000, 0x625f0019, ret(usb_hub_attach), ret(usb_hub_detach).
- `hub_status_change` in `6.5.30/unix.IP35` at 0xc0000000002060f0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000002060f0-0xc000000000206258 (360 bytes).
  It occupies 360 bytes.
  3 such `lw` accesses load 4 bytes from abs 0xc00000000158db8c (usb_dbg_level+20), including `lw` at 0xc000000000206138, `lw` at 0xc0000000002061a0, `lw` at 0xc000000000206218.
  The `jal` at 0xc0000000002060fc calls usb_hub_path [0xc000000000206d44] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc00000000020610c calls usb_dbg [0xc000000000206d84] with a0=0xc0000000004c2710, a1=ret(usb_hub_path), a2=?, a3=?.
  The `jal` at 0xc00000000020611c calls usb_get_hub_status [0xc0000000002056e0] with a0=a0 at entry (arg0), a1=sp-32, a2=0x0, a3=?.
  The `jal` at 0xc000000000206178 calls usb_dbg [0xc000000000206d84] with a0=0xc0000000004c27d0, a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000206188 calls usb_clear_hub_feature [0xc000000000205960] with a0=a0 at entry (arg0), a1=0x1, a2=0x0, a3=?.
  The `jal` at 0xc0000000002061b8 calls usb_hub_overcurrent_event [0xc000000000206258] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000002061c8 calls usb_hub_path [0xc000000000206d44] with a0=a0 at entry (arg0), a1=[0xc00000000158db8c], a2=0x625f0000, a3=?.
  The `jal` at 0xc0000000002061dc calls usb_dbg [0xc000000000206d84] with a0=0xc0000000004c2730, a1=ret(usb_hub_path), a2=ret(usb_get_hub_status), a3=?.
  The `jal` at 0xc0000000002061f4 calls usb_dbg [0xc000000000206d84] with a0=0xc0000000004c2778, a1=?, a2=0x625f0000, a3=?.
  The `jal` at 0xc000000000206204 calls usb_clear_hub_feature [0xc000000000205960] with a0=a0 at entry (arg0), a1=0x0, a2=0x0, a3=?.
  The `jal` at 0xc000000000206230 calls usb_dbg [0xc000000000206d84] with a0=0xc0000000004c2798, a1=ret(usb_clear_hub_feature), a2=[0xc00000000158db8c], a3=?.
  The `jal` at 0xc000000000206248 calls usb_dbg [0xc000000000206d84] with a0=0xc0000000004c27f0, a1=ret(usb_clear_hub_feature), a2=[0xc00000000158db8c], a3=?.
  The `beq` branch at 0xc000000000206130 targets 0xc000000000206154, is taken when v0(=ret(usb_get_hub_status)) == a2(=0x625f0000), and falls through when v0(=ret(usb_get_hub_status)) != a2(=0x625f0000).
  The `bgtz` branch at 0xc00000000020613c targets 0xc0000000002061c8, is taken when a1(=[0xc00000000158db8c]) > 0 (signed), and falls through when a1(=[0xc00000000158db8c]) <= 0 (signed).
  The `bne` branch at 0xc000000000206154 targets 0xc0000000002061ec, is taken when a5(?) != 0, and falls through when a5(?) == 0.
  The `bgez` branch at 0xc000000000206160 targets 0xc000000000206148, is taken when a2(?) >= 0 (signed), and falls through when a2(?) < 0 (signed).
  The `beq` branch at 0xc000000000206194 targets 0xc0000000002061ac, is taken when v0(=ret(usb_clear_hub_feature)) == a1(=0x625f0000), and falls through when v0(=ret(usb_clear_hub_feature)) != a1(=0x625f0000).
  The `bgtz` branch at 0xc0000000002061a4 targets 0xc000000000206240, is taken when a2(=[0xc00000000158db8c]) > 0 (signed), and falls through when a2(=[0xc00000000158db8c]) <= 0 (signed).
  The `bgez` branch at 0xc0000000002061b0 targets 0xc000000000206148, is taken when a3(?) >= 0 (signed), and falls through when a3(?) < 0 (signed).
  The `beq` branch at 0xc0000000002061c0 targets 0xc000000000206148 and is always taken.
  The `beq` branch at 0xc0000000002061e4 targets 0xc000000000206148 and is always taken.
  The `beq` branch at 0xc000000000206210 targets 0xc000000000206224, is taken when v0(=ret(usb_clear_hub_feature)) == a1(=0x625f0000), and falls through when v0(=ret(usb_clear_hub_feature)) != a1(=0x625f0000).
  The `bgtz` branch at 0xc00000000020621c targets 0xc00000000020622c, is taken when a2(=[0xc00000000158db8c]) > 0 (signed), and falls through when a2(=[0xc00000000158db8c]) <= 0 (signed).
  The `beq` branch at 0xc000000000206224 targets 0xc00000000020615c and is always taken.
  The `beq` branch at 0xc000000000206238 targets 0xc000000000206224 and is always taken.
  The `beq` branch at 0xc000000000206250 targets 0xc0000000002061ac and is always taken.
  The return at 0xc00000000020614c has v0 = one of ret(usb_get_hub_status), ?, ret(usb_hub_overcurrent_event), ret(usb_dbg).
- `ioc3_config` in `6.5.30/unix.IP35` at 0xc000000000221c64 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000221c64-0xc000000000221ff4 (912 bytes).
  It occupies 912 bytes.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc000000000221ce4, `lw` at 0xc000000000221e1c, `lw` at 0xc000000000221fa8.
  4 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000221d00, `ld` at 0xc000000000221d80, `ld` at 0xc000000000221e3c.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000221d14, `ld` at 0xc000000000221d94, `ld` at 0xc000000000221e50.
  4 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000221d20, `lw` at 0xc000000000221da0, `lw` at 0xc000000000221e5c.
  6 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc000000000221d2c, `ld` at 0xc000000000221dac, `ld` at 0xc000000000221e68.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc000000000221d38, `ld` at 0xc000000000221d48, `ld` at 0xc000000000221e74.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc000000000221d44, `sw` at 0xc000000000221e78.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc000000000221d4c, `lw` at 0xc000000000221f30.
  The `ld` at 0xc000000000221ddc loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xc000000000221de0 loads 1 byte from [a0+72]:8 +0x3.
  The `sb` at 0xc000000000221e18 stores 1 byte to [a0+72]:8 +0x3.
  The `sw` at 0xc000000000221f10 stores 4 bytes to a0 (arg0 at entry) +0x80.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x84, including `sw` at 0xc000000000221f18, `sw` at 0xc000000000221f5c.
  The `lw` at 0xc000000000221f24 loads 4 bytes from a0 (arg0 at entry) +0x88.
  The `jal` at 0xc000000000221c88 calls set_baud_ti [0xc000000000221008] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jalr` at 0xc000000000221dc0 calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8], a2=[[0xa800000000001020]:8+128]:8, a3=[0xc00000000164578c], a7=0x40000000.
  The `jal` at 0xc000000000221dc8 calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000221ec8 calls (unknown callee) with a0=0x1, a1=0xfffffffffeeddead, a2=[[0xa800000000001020]:8+128]:8, a3=a1 at entry (arg1).
  The `jal` at 0xc000000000221ed0 calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000221f20 calls ioc3_rx_timeout [0xc0000000002232bc] with a0=a0 at entry (arg0), a1=[a0+136], a2=?, a3=a1 at entry (arg1), a6=0xa.
  The `jal` at 0xc000000000221f88 calls spl7 [0xc00000000006201c] with a0=[a0+148], a1=([a0+148] << 0x23), a2=0xa800000000001020, a3=[0xa800000000001000]:8, a4=[[0xa800000000001020]:8], a5=0xfffffffffeeddead, a6=[[0xa800000000001020]:8+128]:8.
  The `jalr` at 0xc000000000221fa0 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000221fb8 calls spl7 [0xc00000000006201c] with a0=[a0+148], a1=?, a2=[a0+72]:8, a3=?, a4=[0xc000000001645790]:8, a5=[0xa800000000001000]:8, a6=[[0xa800000000001020]:8], a7=0xfffffffffeeddead.
  The `jalr` at 0xc000000000221fcc calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `beq` branch at 0xc000000000221c90 targets 0xc000000000221cac, is taken when v0(=ret(set_baud_ti)) == 0, and falls through when v0(=ret(set_baud_ti)) != 0.
  The `beq` branch at 0xc000000000221cb0 targets 0xc000000000221ce0, is taken when s1(=a2) == at(=0x5), and falls through when s1(=a2) != at(=0x5).
  The `beq` branch at 0xc000000000221cb8 targets 0xc000000000221f60, is taken when s1(=a2) == v0(=0x6), and falls through when s1(=a2) != v0(=0x6).
  The `beq` branch at 0xc000000000221cc0 targets 0xc000000000221f68, is taken when s1(=a2) == v1(=0x7), and falls through when s1(=a2) != v1(=0x7).
  The `beq` branch at 0xc000000000221cc8 targets 0xc000000000221cd8, is taken when s1(=a2) == a0(=0x8), and falls through when s1(=a2) != a0(=0x8).
  The `beq` branch at 0xc000000000221cd0 targets 0xc000000000221c98 and is always taken.
  The `beq` branch at 0xc000000000221cd8 targets 0xc000000000221ce4 and is always taken.
  The `bgez` branch at 0xc000000000221cec targets 0xc000000000221ddc, is taken when a1(=([a0+148] << 0x23)) >= 0 (signed), and falls through when a1(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc000000000221cf4 targets 0xc000000000221d38, is taken when a2(=[0xc000000001645790]:8) != s0(=a0), and falls through when a2(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000221d10 targets 0xc000000000221d38, is taken when a3(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a3(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc000000000221d18 targets 0xc000000000221d38, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000221d24 targets 0xc000000000221d38, is taken when a4(=[[0xa800000000001020]:8]) != a5(=0xfffffffffeeddead), and falls through when a4(=[[0xa800000000001020]:8]) == a5(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000221d30 targets 0xc000000000221f88, is taken when a6(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bltz` branch at 0xc000000000221d54 targets 0xc000000000221d6c, is taken when a7(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when a7(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `beq` branch at 0xc000000000221d64 targets 0xc000000000221fe4 and is always taken.
  The `bne` branch at 0xc000000000221d74 targets 0xc000000000221dd4, is taken when a1(=[0xc000000001645790]:8) != s0(=a0), and falls through when a1(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000221d90 targets 0xc000000000221dd4, is taken when v1(=[0xa800000000001000]:8) != a1(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a1(=0x53435241).
  The `beq` branch at 0xc000000000221d98 targets 0xc000000000221dd4, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000221da4 targets 0xc000000000221dd4, is taken when a1(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a1(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000221db0 targets 0xc000000000221dd4, is taken when a2(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc000000000221dd4 targets 0xc000000000221f80, is taken when a0(=one of 0x0, 0x1) == 0, and falls through when a0(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc000000000221df0 targets 0xc000000000221e08, is taken when v1(=a4) == 0, and falls through when v1(=a4) != 0.
  The `beq` branch at 0xc000000000221e00 targets 0xc000000000221f70, is taken when a0(=a5) == 0, and falls through when a0(=a5) != 0.
  The `beq` branch at 0xc000000000221e0c targets 0xc000000000221e18, is taken when a1(=a3) == 0, and falls through when a1(=a3) != 0.
  The `beq` branch at 0xc000000000221e28 targets 0xc000000000221ed8, is taken when (a3(=0x10000000) & a0(=[a0+148])) == 0, and falls through when (a3(=0x10000000) & a0(=[a0+148])) != 0.
  The `bne` branch at 0xc000000000221e30 targets 0xc000000000221e74, is taken when a4(=[0xc000000001645790]:8) != s0(=a0), and falls through when a4(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000221e4c targets 0xc000000000221e74, is taken when a5(=[0xa800000000001000]:8) != a6(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a6(=0x53435241).
  The `beq` branch at 0xc000000000221e54 targets 0xc000000000221e74, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000221e60 targets 0xc000000000221e74, is taken when a6(=[[0xa800000000001020]:8]) != a7(=0xfffffffffeeddead), and falls through when a6(=[[0xa800000000001020]:8]) == a7(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000221e6c targets 0xc000000000221fb8, is taken when t0(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when t0(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc000000000221e80 targets 0xc000000000221ed8, is taken when t1(=[0xc000000001645790]:8) != s0(=a0), and falls through when t1(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000221e9c targets 0xc000000000221ed8, is taken when v1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc000000000221ea4 targets 0xc000000000221edc, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000221eb0 targets 0xc000000000221edc, is taken when a0(=[[0xa800000000001020]:8]) != a1(=0xfffffffffeeddead), and falls through when a0(=[[0xa800000000001020]:8]) == a1(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000221ebc targets 0xc000000000221edc, is taken when a2(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc000000000221f14 targets 0xc000000000221f54, is taken when a2(?) == 0, and falls through when a2(?) != 0.
  The `beq` branch at 0xc000000000221f28 targets 0xc000000000221c98 and is always taken.
  The `bne` branch at 0xc000000000221f3c targets 0xc000000000221d6c, is taken when (t0(=[[a0+64]:8]) & a7(=0x40000000)) != 0, and falls through when (t0(=[[a0+64]:8]) & a7(=0x40000000)) == 0.
  The `beq` branch at 0xc000000000221f44 targets 0xc000000000221f30, is taken when t1(?) == 0, and falls through when t1(?) != 0.
  The `beq` branch at 0xc000000000221f4c targets 0xc000000000221d70 and is always taken.
  The `beq` branch at 0xc000000000221f58 targets 0xc000000000221f1c and is always taken.
  The `beq` branch at 0xc000000000221f60 targets 0xc000000000221ce4 and is always taken.
  The `beq` branch at 0xc000000000221f68 targets 0xc000000000221ce4 and is always taken.
  The `bne` branch at 0xc000000000221f70 targets 0xc000000000221e14, is taken when t3(=a3) != 0, and falls through when t3(=a3) == 0.
  The `beq` branch at 0xc000000000221f78 targets 0xc000000000221e18 and is always taken.
  The `beq` branch at 0xc000000000221f80 targets 0xc000000000221c98 and is always taken.
  The `beq` branch at 0xc000000000221fb0 targets 0xc000000000221d38 and is always taken.
  The `beq` branch at 0xc000000000221fdc targets 0xc000000000221e74 and is always taken.
  The `beq` branch at 0xc000000000221fec targets 0xc000000000221f44 and is always taken.
  The return at 0xc000000000221ca4 has v0 = one of -0x1, 0x0, 0x1.
- `ioc3_set_extclk` in `6.5.30/unix.IP35` at 0xc000000000222118 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000222118-0xc000000000222120 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000222118 has v0 = a1.
- `ioc3_write` in `6.5.30/unix.IP35` at 0xc00000000022239c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc00000000022239c-0xc0000000002223b8 (28 bytes).
  It occupies 28 bytes.
  The `jal` at 0xc0000000002223a4 calls do_ioc3_write [0xc000000000222120] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The return at 0xc0000000002223b0 has v0 = ret(do_ioc3_write).
- `ioc3_sync_write` in `6.5.30/unix.IP35` at 0xc0000000002223b8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000002223b8-0xc000000000222414 (92 bytes).
  It occupies 92 bytes.
  The `lw` at 0xc0000000002223e4 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xc0000000002223cc calls do_ioc3_write [0xc000000000222120] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000002223f4 calls ioc3_enable_tx [0xc000000000222780] with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `blez` branch at 0xc0000000002223d4 targets 0xc0000000002223fc, is taken when s3(=a2) <= 0 (signed), and falls through when s3(=a2) > 0 (signed).
  The `bne` branch at 0xc0000000002223dc targets 0xc0000000002223fc, is taken when v0(=ret(do_ioc3_write)) != 0, and falls through when v0(=ret(do_ioc3_write)) == 0.
  The `beq` branch at 0xc0000000002223ec targets 0xc0000000002223fc, is taken when (at(=[a0+120]) & 0x10) == 0, and falls through when (at(=[a0+120]) & 0x10) != 0.
  The return at 0xc00000000022240c has v0 = ret(do_ioc3_write).
- `ioc3_wrflush` in `6.5.30/unix.IP35` at 0xc000000000222414 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000222414-0xc0000000002224d8 (196 bytes).
  It occupies 196 bytes.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x38, including `ld` at 0xc000000000222414, `ld` at 0xc000000000222450, `ld` at 0xc000000000222498.
  3 such `lw` accesses load 4 bytes from [a0+56]:8 +0x1c, including `lw` at 0xc000000000222418, `lw` at 0xc000000000222454, `lw` at 0xc00000000022249c.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc000000000222434, `ld` at 0xc00000000022248c, `ld` at 0xc0000000002224b4.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc000000000222438, `lw` at 0xc0000000002224b8.
  The `lw` at 0xc000000000222470 loads 4 bytes from a0 (arg0 at entry) +0x78.
  The `jal` at 0xc000000000222480 calls ioc3_enable_tx [0xc000000000222780] with a0=a0 at entry (arg0), a1=0x1, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000002224ac calls us_delay [0xc000000000069a4c] with a0=0x5, a1=?, a2=one of 0x10000000, 0x50000000, a3=?.
  The `beq` branch at 0xc00000000022242c targets 0xc000000000222470, is taken when (at(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (at(=[[a0+56]:8+28]) & 0x1) != 0.
  The `bne` branch at 0xc000000000222448 targets 0xc000000000222464, is taken when a0(=0x10000000) != v0(?), and falls through when a0(=0x10000000) == v0(?).
  The `beq` branch at 0xc00000000022245c targets 0xc000000000222490, is taken when (v1(=[[a0+56]:8+28]) & 0x1) == 0, and falls through when (v1(=[[a0+56]:8+28]) & 0x1) != 0.
  The `beq` branch at 0xc000000000222478 targets 0xc000000000222434, is taken when (a0(=[a0+120]) & 0x10) == 0, and falls through when (a0(=[a0+120]) & 0x10) != 0.
  The `beq` branch at 0xc000000000222488 targets 0xc000000000222438 and is always taken.
  The `beq` branch at 0xc000000000222490 targets 0xc0000000002224ac and is always taken.
  The `bne` branch at 0xc0000000002224a4 targets 0xc0000000002224d0, is taken when (a1(=[[a0+56]:8+28]) & 0x1) != 0, and falls through when (a1(=[[a0+56]:8+28]) & 0x1) == 0.
  The `beq` branch at 0xc0000000002224c8 targets 0xc000000000222498, is taken when a2(=0x10000000) == a3(?), and falls through when a2(=0x10000000) != a3(?).
  The `beq` branch at 0xc0000000002224d0 targets 0xc000000000222464 and is always taken.
  The return at 0xc000000000222468 has v0 = one of ?, ret(us_delay).
- `ioc3_break` in `6.5.30/unix.IP35` at 0xc0000000002224d8 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000002224d8-0xc000000000222780 (680 bytes).
  It occupies 680 bytes.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc0000000002224d8, `lw` at 0xc000000000222604, `lw` at 0xc000000000222730.
  4 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc00000000022250c, `ld` at 0xc000000000222598, `ld` at 0xc000000000222620.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000222520, `ld` at 0xc00000000022259c, `ld` at 0xc000000000222634.
  4 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc00000000022252c, `lw` at 0xc0000000002225b0, `lw` at 0xc000000000222640.
  6 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc000000000222538, `ld` at 0xc0000000002225bc, `ld` at 0xc00000000022264c.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc000000000222544, `ld` at 0xc000000000222554, `ld` at 0xc000000000222658.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc000000000222550, `sw` at 0xc00000000022265c.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc000000000222558, `lw` at 0xc0000000002226d8.
  The `ld` at 0xc0000000002225f0 loads 8 bytes from a0 (arg0 at entry) +0x48.
  The `lbu` at 0xc0000000002225f8 loads 1 byte from [a0+72]:8 +0x3.
  2 such `sb` accesses store 1 byte to [a0+72]:8 +0x3, including `sb` at 0xc000000000222600, `sb` at 0xc000000000222708.
  The `jalr` at 0xc0000000002225d0 calls (unknown callee) with a0=0x1, a1=a1 at entry (arg1), a2=[[0xa800000000001020]:8], a3=0xfffffffffeeddead, a4=[[0xa800000000001020]:8+128]:8, a5=[0xc00000000164578c], a6=0x10000000, a7=[0xc000000001645790]:8.
  The `jal` at 0xc0000000002225d8 calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jalr` at 0xc0000000002226b0 calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=?, a4=[[0xa800000000001020]:8+128]:8.
  The `jal` at 0xc0000000002226b8 calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000222714 calls spl7 [0xc00000000006201c] with a0=[a0+72]:8, a1=a1 at entry (arg1), a2=?, a3=?, a4=[a0+148], a6=0x10000000.
  The `jalr` at 0xc000000000222728 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a5=[[0xa800000000001020]:8+128]:8.
  The `jal` at 0xc000000000222740 calls spl7 [0xc00000000006201c] with a0=[[0xa800000000001020]:8], a1=a1 at entry (arg1), a2=0xfffffffffeeddead, a3=[[0xa800000000001020]:8+128]:8, a4=[a0+148], a6=0x10000000.
  The `jalr` at 0xc000000000222754 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+128]:8.
  The `bgez` branch at 0xc0000000002224f4 targets 0xc0000000002225f0, is taken when at(=([a0+148] << 0x23)) >= 0 (signed), and falls through when at(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc000000000222500 targets 0xc000000000222544, is taken when v0(=[0xc000000001645790]:8) != a0, and falls through when v0(=[0xc000000001645790]:8) == a0.
  The `bne` branch at 0xc00000000022251c targets 0xc000000000222544, is taken when v1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when v1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc000000000222524 targets 0xc000000000222544, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000222530 targets 0xc000000000222544, is taken when a0(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a0(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `bne` branch at 0xc00000000022253c targets 0xc000000000222740, is taken when a3(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a3(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bltz` branch at 0xc00000000022256c targets 0xc00000000022257c, is taken when t0(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t0(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `beq` branch at 0xc000000000222574 targets 0xc000000000222774 and is always taken.
  The `bne` branch at 0xc000000000222594 targets 0xc0000000002225e8, is taken when a7(=[0xc000000001645790]:8) != s0(=a0), and falls through when a7(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc0000000002225a0 targets 0xc0000000002225e8, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000002225a8 targets 0xc0000000002225e8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000002225b4 targets 0xc0000000002225e8, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000002225c0 targets 0xc0000000002225e8, is taken when a4(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a4(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc0000000002225e8 targets 0xc00000000022270c, is taken when s1(=one of 0x0, 0x1) == 0, and falls through when s1(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc0000000002225f4 targets 0xc0000000002226fc, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc00000000022260c targets 0xc0000000002226c0, is taken when (a6(=0x10000000) & a4(=[a0+148])) == 0, and falls through when (a6(=0x10000000) & a4(=[a0+148])) != 0.
  The `bne` branch at 0xc000000000222614 targets 0xc000000000222658, is taken when t1(=[0xc000000001645790]:8) != s0(=a0), and falls through when t1(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000222630 targets 0xc000000000222658, is taken when t2(=[0xa800000000001000]:8) != t3(=0x53435241), and falls through when t2(=[0xa800000000001000]:8) == t3(=0x53435241).
  The `beq` branch at 0xc000000000222638 targets 0xc000000000222658, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000222644 targets 0xc000000000222658, is taken when t3(=[[0xa800000000001020]:8]) != t8(=0xfffffffffeeddead), and falls through when t3(=[[0xa800000000001020]:8]) == t8(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000222650 targets 0xc000000000222714, is taken when t9(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when t9(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc000000000222664 targets 0xc0000000002226c4, is taken when ra(=[0xc000000001645790]:8) != s0(=a0), and falls through when ra(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc00000000022267c targets 0xc0000000002226c4, is taken when v0(=[0xa800000000001000]:8) != v1(=0x53435241), and falls through when v0(=[0xa800000000001000]:8) == v1(=0x53435241).
  The `beq` branch at 0xc00000000022268c targets 0xc0000000002226c0, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000222698 targets 0xc0000000002226c0, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000002226a4 targets 0xc0000000002226c4, is taken when a4(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a4(=[[0xa800000000001020]:8+128]:8) != 0.
  The `bne` branch at 0xc0000000002226e4 targets 0xc00000000022257c, is taken when (a2(=[[a0+64]:8]) & a7(=0x40000000)) != 0, and falls through when (a2(=[[a0+64]:8]) & a7(=0x40000000)) == 0.
  The `beq` branch at 0xc0000000002226ec targets 0xc0000000002226d8, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0xc0000000002226f4 targets 0xc000000000222584 and is always taken.
  The `beq` branch at 0xc000000000222704 targets 0xc000000000222604 and is always taken.
  The `beq` branch at 0xc00000000022270c targets 0xc0000000002226c4 and is always taken.
  The `beq` branch at 0xc000000000222738 targets 0xc000000000222658 and is always taken.
  The `beq` branch at 0xc00000000022276c targets 0xc000000000222544 and is always taken.
  The `beq` branch at 0xc000000000222778 targets 0xc0000000002226ec and is always taken.
  The return at 0xc0000000002226d0 has v0 = one of -0x1, 0x0.
- `ioc3_enable_tx` in `6.5.30/unix.IP35` at 0xc000000000222780 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000222780-0xc000000000222994 (532 bytes).
  It occupies 532 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xc00000000022278c, `lw` at 0xc000000000222858.
  The `ld` at 0xc000000000222794 loads 8 bytes from a0 (arg0 at entry) +0x70.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000002227c0, `ld` at 0xc0000000002228a0.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000002227c4, `ld` at 0xc0000000002228a4, `ld` at 0xc000000000222970.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000002227d8, `lw` at 0xc0000000002228c8.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000002227e4, `ld` at 0xc0000000002228d4, `ld` at 0xc000000000222974.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc0000000002227f0, `lw` at 0xc00000000022287c.
  4 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc0000000002227fc, `ld` at 0xc00000000022280c, `ld` at 0xc000000000222894.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc000000000222808, `sw` at 0xc000000000222898.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc000000000222810, `lw` at 0xc000000000222840.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x98, including `lw` at 0xc00000000022285c, `lw` at 0xc00000000022293c.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xc000000000222868, `sw` at 0xc000000000222924.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x4, including `sw` at 0xc00000000022286c, `sw` at 0xc000000000222958.
  2 such `lw` accesses load 4 bytes from [a0+112]:8 +0x8, including `lw` at 0xc000000000222878, `lw` at 0xc00000000022292c.
  The `lw` at 0xc00000000022291c loads 4 bytes from [a0+64]:8 +0x8.
  The `jal` at 0xc000000000222874 calls enable_intrs [0xc0000000002209c8] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=?, a3=?, a4=[a0+64]:8.
  The `jalr` at 0xc0000000002228e4 calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8+128]:8, a2=[0xc000000001645790]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=0xa800000000001000.
  The `jal` at 0xc0000000002228ec calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000222934 calls disable_intrs [0xc000000000220a18] with a0=a0 at entry (arg0), a1=[[a0+112]:8+8], a2=?, a3=?, a4=[a0+64]:8, a6=([a0+120] | 0x10).
  The `jal` at 0xc000000000222964 calls spl7 [0xc00000000006201c] with a0=0x53435241, a1=a1 at entry (arg1), a2=[[0xa800000000001020]:8], a3=0xfffffffffeeddead, a4=[[0xa800000000001020]:8+128]:8, a5=0xa800000000001020, a6=[0xc000000001645790]:8.
  The `jalr` at 0xc00000000022297c calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `beq` branch at 0xc00000000022279c targets 0xc00000000022290c, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc0000000002227a4 targets 0xc000000000222914, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `bne` branch at 0xc0000000002227bc targets 0xc0000000002227f0, is taken when a6(=[0xc000000001645790]:8) != s0(=a0), and falls through when a6(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc0000000002227c8 targets 0xc0000000002227f0, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000002227d0 targets 0xc0000000002227f0, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000002227dc targets 0xc0000000002227f0, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000002227e8 targets 0xc000000000222964, is taken when a4(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a4(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bgez` branch at 0xc0000000002227f8 targets 0xc000000000222854, is taken when a5(=([a0+148] << 0x23)) >= 0 (signed), and falls through when a5(=([a0+148] << 0x23)) < 0 (signed).
  The `bltz` branch at 0xc000000000222828 targets 0xc000000000222854, is taken when t0(=([[a0+64]:8] << 0x21)) < 0 (signed), and falls through when t0(=([[a0+64]:8] << 0x21)) >= 0 (signed).
  The `bne` branch at 0xc000000000222838 targets 0xc00000000022295c, is taken when t2(?) != 0, and falls through when t2(?) == 0.
  The `beq` branch at 0xc00000000022284c targets 0xc000000000222838, is taken when (t3(=[[a0+64]:8]) & a6(=0x40000000)) == 0, and falls through when (t3(=[[a0+64]:8]) & a6(=0x40000000)) != 0.
  The `beq` branch at 0xc000000000222854 targets 0xc00000000022291c, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc00000000022288c targets 0xc00000000022289c, is taken when (ra(=0x10000000) & a0(=[a0+148])) == 0, and falls through when (ra(=0x10000000) & a0(=[a0+148])) != 0.
  The `bne` branch at 0xc0000000002228b0 targets 0xc0000000002228f4, is taken when a2(=[0xc000000001645790]:8) != s0(=a0), and falls through when a2(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc0000000002228b8 targets 0xc0000000002228f4, is taken when a1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc0000000002228c0 targets 0xc0000000002228f4, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000002228cc targets 0xc0000000002228f4, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `beq` branch at 0xc0000000002228d8 targets 0xc0000000002228f8, is taken when a1(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a1(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc00000000022290c targets 0xc0000000002227ac, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc000000000222914 targets 0xc0000000002228f8 and is always taken.
  The `beq` branch at 0xc000000000222948 targets 0xc00000000022287c, is taken when a7(=[a0+152]) == v0(?), and falls through when a7(=[a0+152]) != v0(?).
  The `beq` branch at 0xc000000000222954 targets 0xc00000000022287c and is always taken.
  The `beq` branch at 0xc00000000022295c targets 0xc0000000002228fc and is always taken.
  The `beq` branch at 0xc00000000022298c targets 0xc0000000002227f0 and is always taken.
  The return at 0xc000000000222904 has v0 = one of -0x1, 0x0.
- `ioc3_read` in `6.5.30/unix.IP35` at 0xc000000000222994 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000222994-0xc0000000002230bc (1832 bytes).
  It occupies 1832 bytes.
  The `ld` at 0xc0000000002229a0 loads 8 bytes from a0 (arg0 at entry) +0x70.
  11 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc0000000002229b8, `ld` at 0xc0000000002229d0, `ld` at 0xc000000000222abc.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x9c, including `lw` at 0xc0000000002229bc, `lw` at 0xc0000000002229dc.
  3 such `sw` accesses store 4 bytes to [a0+64]:8 +0x10, including `sw` at 0xc0000000002229cc, `sw` at 0xc000000000222d68, `sw` at 0xc000000000223068.
  4 such `lw` accesses load 4 bytes from [a0+64]:8 +0xc, including `lw` at 0xc0000000002229d8, `lw` at 0xc000000000222ac0, `lw` at 0xc000000000222b8c.
  5 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc0000000002229fc, `lw` at 0xc000000000222b9c, `lw` at 0xc000000000222d7c.
  6 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000222a1c, `ld` at 0xc000000000222a70, `ld` at 0xc000000000222ad8.
  9 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000222a30, `ld` at 0xc000000000222a84, `ld` at 0xc000000000222aec.
  6 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000222a3c, `lw` at 0xc000000000222a90, `lw` at 0xc000000000222af8.
  9 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc000000000222a4c, `ld` at 0xc000000000222aa0, `ld` at 0xc000000000222b04.
  2 such `sw` accesses store 4 bytes to [a0+64]:8 +0x0, including `sw` at 0xc000000000222a60, `sw` at 0xc000000000222bf4.
  2 such `lw` accesses load 4 bytes from [a0+64]:8 +0x0, including `lw` at 0xc000000000222b10, `lw` at 0xc000000000222ee8.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xc000000000222ba8, `sw` at 0xc000000000222f7c.
  4 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x78, including `lw` at 0xc000000000222c50, `lw` at 0xc000000000222cc8, `lw` at 0xc000000000222d6c.
  The `ld` at 0xc000000000222c54 loads 8 bytes from a0 (arg0 at entry) +0x60.
  4 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x78, including `sw` at 0xc000000000222c64, `sw` at 0xc000000000222f14, `sw` at 0xc000000000222f6c.
  6 such `lw` accesses load 4 bytes from ?, including `lw` at 0xc000000000222c88, `lw` at 0xc000000000222d48, `lw` at 0xc000000000222ec0.
  7 such `lbu` accesses load 1 byte from ?, including `lbu` at 0xc000000000222ca4, `lbu` at 0xc000000000222d0c, `lbu` at 0xc000000000222e00.
  5 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x8c, including `lw` at 0xc000000000222cb8, `lw` at 0xc000000000222d30, `lw` at 0xc000000000222e10.
  5 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x8, including `ld` at 0xc000000000222cf0, `ld` at 0xc000000000222de4, `ld` at 0xc000000000222e20.
  4 such `ld` accesses load 8 bytes from [a0+8]:8 +0x10, including `ld` at 0xc000000000222cfc, `ld` at 0xc000000000222df0, `ld` at 0xc000000000222e2c.
  4 such `sb` accesses store 1 byte to ?, including `sb` at 0xc000000000222d18, `sb` at 0xc000000000222e90, `sb` at 0xc000000000222ea4.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x9c, including `sw` at 0xc000000000222d78, `sw` at 0xc000000000223078.
  The `lw` at 0xc000000000222f20 loads 4 bytes from [a0+112]:8 +0x10.
  The `ld` at 0xc000000000223088 loads 8 bytes from [a0+8]:8 +0x18.
  The `jalr` at 0xc000000000222aac calls (unknown callee) with a0=0x1, a1=?, a2=[[0xa800000000001020]:8+128]:8, a3=?, a5=[a0+64]:8, a7=[0xc000000001645790]:8.
  The `jal` at 0xc000000000222ab4 calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000222b70 calls (unknown callee) with a0=0x1, a1=[0xa800000000001000]:8, a2=[[0xa800000000001020]:8+128]:8, a3=?, a5=[a0+64]:8.
  The `jal` at 0xc000000000222b78 calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000222c40 calls (unknown callee) with a0=0x1, a1=?, a2=[[0xa800000000001020]:8+128]:8, a3=?, a5=[a0+64]:8, a7=[0xc000000001645790]:8.
  The `jal` at 0xc000000000222c48 calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000222d04 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x8, a2=[a0+140], a3=?.
  The `jalr` at 0xc000000000222df8 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x2, a2=[a0+140], a3=?, a7=[[a0+8]:8+16]:8.
  The `jalr` at 0xc000000000222e34 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x4, a2=?, a3=?.
  The `jalr` at 0xc000000000222e70 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x1, a2=?, a3=?.
  The `jal` at 0xc000000000222f1c calls enable_intrs [0xc0000000002209c8] with a0=a0 at entry (arg0), a1=[[a0+112]:8+16], a2=?, a3=?.
  The `jal` at 0xc000000000222fb8 calls spl7 [0xc00000000006201c] with a0=?, a1=[0xc000000001645790]:8, a2=[0xa800000000001000]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[a0+64]:8, a6=[[0xa800000000001020]:8+128]:8.
  The `jalr` at 0xc000000000222fcc calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a6=[[0xa800000000001020]:8+128]:8.
  The `jal` at 0xc000000000222fe4 calls spl7 [0xc00000000006201c] with a0=?, a1=0xffffffffefffffff, a2=[0xa800000000001000]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[a0+64]:8, a6=[[0xa800000000001020]:8+128]:8.
  The `jalr` at 0xc000000000222ff8 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+128]:8.
  The `jal` at 0xc000000000223010 calls spl7 [0xc00000000006201c] with a0=?, a1=?, a2=[0xc000000001645790]:8, a3=?, a5=[a0+64]:8.
  The `jalr` at 0xc000000000223024 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `jalr` at 0xc000000000223090 calls (unknown callee) with a0=a0 at entry (arg0), a1=0x0, a2=[a0+140], a3=?.
  The `bne` branch at 0xc0000000002229f4 targets 0xc000000000222c50, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bgez` branch at 0xc000000000222a04 targets 0xc000000000222f70, is taken when v1(=([a0+148] << 0x23)) >= 0 (signed), and falls through when v1(=([a0+148] << 0x23)) < 0 (signed).
  The `bne` branch at 0xc000000000222a14 targets 0xc000000000222a58, is taken when a1(=[0xc000000001645790]:8) != s0(=a0), and falls through when a1(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000222a28 targets 0xc000000000222a58, is taken when a2(=[0xa800000000001000]:8) != a3(=0x53435241), and falls through when a2(=[0xa800000000001000]:8) == a3(=0x53435241).
  The `beq` branch at 0xc000000000222a34 targets 0xc000000000222a58, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000222a44 targets 0xc000000000222a58, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000222a50 targets 0xc000000000222fb8, is taken when a6(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc000000000222a68 targets 0xc000000000222abc, is taken when a7(=[0xc000000001645790]:8) != s0(=a0), and falls through when a7(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000222a7c targets 0xc000000000222abc, is taken when t1(=[0xa800000000001000]:8) != t2(=0x53435241), and falls through when t1(=[0xa800000000001000]:8) == t2(=0x53435241).
  The `beq` branch at 0xc000000000222a88 targets 0xc000000000222abc, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000222a98 targets 0xc000000000222abc, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000222aa4 targets 0xc000000000222abc, is taken when a2(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+128]:8) != 0.
  The `bne` branch at 0xc000000000222ac8 targets 0xc000000000222b94, is taken when s5(?) != s1(=[a0+156]), and falls through when s5(?) == s1(=[a0+156]).
  The `bne` branch at 0xc000000000222ad0 targets 0xc000000000222b10, is taken when s0(=a0) != a2(=[0xc000000001645790]:8), and falls through when s0(=a0) == a2(=[0xc000000001645790]:8).
  The `bne` branch at 0xc000000000222ae4 targets 0xc000000000222b10, is taken when t8(=[0xa800000000001000]:8) != t9(=0x53435241), and falls through when t8(=[0xa800000000001000]:8) == t9(=0x53435241).
  The `beq` branch at 0xc000000000222af0 targets 0xc000000000222b10, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000222afc targets 0xc000000000222b10, is taken when t9(=[[0xa800000000001020]:8]) != ra(=0xfffffffffeeddead), and falls through when t9(=[[0xa800000000001020]:8]) == ra(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000222b08 targets 0xc000000000223010, is taken when at(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when at(=[[0xa800000000001020]:8+128]:8) == 0.
  The `beq` branch at 0xc000000000222b18 targets 0xc000000000222b2c, is taken when (v0(=[[a0+64]:8]) & s2(=0x8000000)) == 0, and falls through when (v0(=[[a0+64]:8]) & s2(=0x8000000)) != 0.
  The `beq` branch at 0xc000000000222b24 targets 0xc0000000002230ac and is always taken.
  The `bne` branch at 0xc000000000222b30 targets 0xc000000000222b80, is taken when s0(=a0) != a2(=[0xc000000001645790]:8), and falls through when s0(=a0) == a2(=[0xc000000001645790]:8).
  The `bne` branch at 0xc000000000222b44 targets 0xc000000000222b80, is taken when a1(=[0xa800000000001000]:8) != a2(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a2(=0x53435241).
  The `beq` branch at 0xc000000000222b50 targets 0xc000000000222b80, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000222b5c targets 0xc000000000222b80, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000222b68 targets 0xc000000000222b80, is taken when a2(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc000000000222b80 targets 0xc000000000222fb0, is taken when s2(=one of 0x0, 0x1) == 0, and falls through when s2(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0xc000000000222b94 targets 0xc000000000222c50, is taken when s3(=one of 0x0, 0x1) == 0, and falls through when s3(=one of 0x0, 0x1) != 0.
  The `bne` branch at 0xc000000000222bb0 targets 0xc000000000222bf4, is taken when t8(=[0xc000000001645790]:8) != s0(=a0), and falls through when t8(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000222bc4 targets 0xc000000000222bf4, is taken when a2(=[0xa800000000001000]:8) != a3(=0x53435241), and falls through when a2(=[0xa800000000001000]:8) == a3(=0x53435241).
  The `beq` branch at 0xc000000000222bd0 targets 0xc000000000222bf4, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000222be0 targets 0xc000000000222bf4, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000222bec targets 0xc000000000222fe4, is taken when a6(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a6(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc000000000222bfc targets 0xc000000000222c50, is taken when a7(=[0xc000000001645790]:8) != s0(=a0), and falls through when a7(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000222c10 targets 0xc000000000222c50, is taken when t0(=[0xa800000000001000]:8) != t1(=0x53435241), and falls through when t0(=[0xa800000000001000]:8) == t1(=0x53435241).
  The `beq` branch at 0xc000000000222c1c targets 0xc000000000222c50, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000222c2c targets 0xc000000000222c50, is taken when v1(=[[0xa800000000001020]:8]) != a0(=0xfffffffffeeddead), and falls through when v1(=[[0xa800000000001020]:8]) == a0(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000222c38 targets 0xc000000000222c50, is taken when a2(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a2(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc000000000222c60 targets 0xc000000000222d60, is taken when s5(?) == s1(=[a0+156]), and falls through when s5(?) != s1(=[a0+156]).
  The `blez` branch at 0xc000000000222c68 targets 0xc000000000222f88, is taken when s6(=a2) <= 0 (signed), and falls through when s6(=a2) > 0 (signed).
  The `beq` branch at 0xc000000000222c90 targets 0xc000000000222f2c, is taken when (s4(=0xffffffffc0c0c0c0) & t8(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & t8(?)) != 0.
  The `blez` branch at 0xc000000000222c98 targets 0xc000000000222ec0, is taken when s6(?) <= 0 (signed), and falls through when s6(?) > 0 (signed).
  The `beq` branch at 0xc000000000222cb0 targets 0xc000000000222d1c, is taken when (a0(?) & 0x40) == 0, and falls through when (a0(?) & 0x40) != 0.
  The `beq` branch at 0xc000000000222cc0 targets 0xc000000000222ce0, is taken when (a2(=[a0+140]) & 0x40) == 0, and falls through when (a2(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xc000000000222cd0 targets 0xc000000000222ce0, is taken when (v0(=[a0+120]) & 0x2) == 0, and falls through when (v0(=[a0+120]) & 0x2) != 0.
  The `beq` branch at 0xc000000000222cd8 targets 0xc000000000222f90, is taken when a6(?) == 0, and falls through when a6(?) != 0.
  The `beq` branch at 0xc000000000222ce0 targets 0xc000000000222d14, is taken when a5(?) == 0, and falls through when a5(?) != 0.
  The `beq` branch at 0xc000000000222ce8 targets 0xc000000000222d14, is taken when a7(?) == 0, and falls through when a7(?) != 0.
  The `beq` branch at 0xc000000000222cf4 targets 0xc000000000222d10, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc000000000222d20 targets 0xc000000000222ea8, is taken when (a0(?) & 0x80) == 0, and falls through when (a0(?) & 0x80) != 0.
  The `beq` branch at 0xc000000000222d28 targets 0xc000000000222e80, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc000000000222d38 targets 0xc000000000222e80, is taken when (a2(=[a0+140]) & 0x1c) == 0, and falls through when (a2(=[a0+140]) & 0x1c) != 0.
  The `blez` branch at 0xc000000000222d40 targets 0xc000000000222dd0, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xc000000000222d58 targets 0xc000000000222d64 and is always taken.
  The `beq` branch at 0xc000000000222d74 targets 0xc000000000222d9c, is taken when (v0(=[a0+120]) & 0x1) == 0, and falls through when (v0(=[a0+120]) & 0x1) != 0.
  The `bne` branch at 0xc000000000222d94 targets 0xc000000000222f0c, is taken when a4(?) < a5(?) (unsigned), and falls through when a4(?) >= a5(?) (unsigned).
  The `beq` branch at 0xc000000000222dd0 targets 0xc000000000222e08, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xc000000000222ddc targets 0xc000000000222e08, is taken when (a2(=[a0+140]) & 0x8) == 0, and falls through when (a2(=[a0+140]) & 0x8) != 0.
  The `beq` branch at 0xc000000000222de8 targets 0xc000000000222e08, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc000000000222e08 targets 0xc000000000222e44, is taken when t0(?) == 0, and falls through when t0(?) != 0.
  The `beq` branch at 0xc000000000222e18 targets 0xc000000000222e44, is taken when (t1(=[a0+140]) & 0x10) == 0, and falls through when (t1(=[a0+140]) & 0x10) != 0.
  The `beq` branch at 0xc000000000222e24 targets 0xc000000000222e44, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc000000000222e44 targets 0xc000000000222e80, is taken when t3(?) == 0, and falls through when t3(?) != 0.
  The `beq` branch at 0xc000000000222e54 targets 0xc000000000222e80, is taken when (t8(=[a0+140]) & 0x4) == 0, and falls through when (t8(=[a0+140]) & 0x4) != 0.
  The `beq` branch at 0xc000000000222e60 targets 0xc000000000222e80, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc000000000222eb0 targets 0xc000000000222ec0, is taken when s8(?) >= 0x4 (signed), and falls through when s8(?) < 0x4 (signed).
  The `bgtz` branch at 0xc000000000222eb8 targets 0xc000000000222ca4, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xc000000000222ec8 targets 0xc000000000222f80, is taken when (s4(=0xffffffffc0c0c0c0) & a0(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a0(?)) != 0.
  The `beq` branch at 0xc000000000222ed0 targets 0xc000000000222d64, is taken when s5(?) == s1(?), and falls through when s5(?) != s1(?).
  The `bgtz` branch at 0xc000000000222ed8 targets 0xc000000000222c80, is taken when s6(?) > 0 (signed), and falls through when s6(?) <= 0 (signed).
  The `beq` branch at 0xc000000000222ee0 targets 0xc000000000222d68 and is always taken.
  The `beq` branch at 0xc000000000222ef4 targets 0xc000000000222b2c, is taken when (a1(=[[a0+64]:8]) & s2(=0x8000000)) == 0, and falls through when (a1(=[[a0+64]:8]) & s2(=0x8000000)) != 0.
  The `beq` branch at 0xc000000000222efc targets 0xc000000000222ee8, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0xc000000000222f04 targets 0xc000000000222b30 and is always taken.
  The `beq` branch at 0xc000000000222f24 targets 0xc000000000222d9c and is always taken.
  The `beq` branch at 0xc000000000222f48 targets 0xc000000000222f60, is taken when v0(?) == a0(?), and falls through when v0(?) != a0(?).
  The `bne` branch at 0xc000000000222f58 targets 0xc000000000222c98, is taken when (s4(=0xffffffffc0c0c0c0) & a3(?)) != 0, and falls through when (s4(=0xffffffffc0c0c0c0) & a3(?)) == 0.
  The `beq` branch at 0xc000000000222f68 targets 0xc000000000222d68 and is always taken.
  The `beq` branch at 0xc000000000222f78 targets 0xc000000000222a10 and is always taken.
  The `beq` branch at 0xc000000000222f80 targets 0xc000000000222ed0 and is always taken.
  The `beq` branch at 0xc000000000222f88 targets 0xc000000000222d64 and is always taken.
  The `blez` branch at 0xc000000000222f90 targets 0xc00000000022303c, is taken when s2(?) <= 0 (signed), and falls through when s2(?) > 0 (signed).
  The `beq` branch at 0xc000000000222fa8 targets 0xc000000000222d64 and is always taken.
  The `beq` branch at 0xc000000000222fb0 targets 0xc000000000222d9c and is always taken.
  The `beq` branch at 0xc000000000222fdc targets 0xc000000000222a58 and is always taken.
  The `beq` branch at 0xc000000000223008 targets 0xc000000000222bf4 and is always taken.
  The `beq` branch at 0xc000000000223034 targets 0xc000000000222b10 and is always taken.
  The `beq` branch at 0xc00000000022305c targets 0xc0000000002230a0, is taken when (s4(=0xffffffffc0c0c0c0) & t1(?)) == 0, and falls through when (s4(=0xffffffffc0c0c0c0) & t1(?)) != 0.
  The `beq` branch at 0xc000000000223074 targets 0xc000000000223098, is taken when (t9(=[a0+140]) & 0x40) == 0, and falls through when (t9(=[a0+140]) & 0x40) != 0.
  The `beq` branch at 0xc000000000223080 targets 0xc000000000223098, is taken when v0(=[a0+8]:8) == 0, and falls through when v0(=[a0+8]:8) != 0.
  The `beq` branch at 0xc000000000223098 targets 0xc000000000222d9c and is always taken.
  The `beq` branch at 0xc0000000002230a4 targets 0xc000000000223064 and is always taken.
  The `beq` branch at 0xc0000000002230b4 targets 0xc000000000222efc and is always taken.
  The return at 0xc000000000222dc8 has v0 = one of ?, -0x1, 0x0.
- `ioc3_set_DTR` in `6.5.30/unix.IP35` at 0xc000000000223648 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000223648-0xc000000000223668 (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xc000000000223654 calls set_DTRRTS [0xc000000000223474] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x1, a3=0x1000000.
  The return at 0xc000000000223660 has v0 = ret(set_DTRRTS).
- `ioc3_set_RTS` in `6.5.30/unix.IP35` at 0xc000000000223668 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000223668-0xc000000000223688 (32 bytes).
  It occupies 32 bytes.
  The `jal` at 0xc000000000223674 calls set_DTRRTS [0xc000000000223474] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=0x2, a3=0x2000000.
  The return at 0xc000000000223680 has v0 = ret(set_DTRRTS).
- `ioc3_query_DCD` in `6.5.30/unix.IP35` at 0xc000000000223688 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000223688-0xc00000000022378c (260 bytes).
  It occupies 260 bytes.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000002236a4, `ld` at 0xc0000000002236f4.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000002236a8, `ld` at 0xc0000000002236fc, `ld` at 0xc000000000223760.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000002236bc, `lw` at 0xc000000000223708.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000002236c8, `ld` at 0xc000000000223714, `ld` at 0xc000000000223764.
  The `ld` at 0xc0000000002236d8 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xc0000000002236ec loads 4 bytes from [a0+64]:8 +0x18.
  The `jalr` at 0xc00000000022372c calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=[[0xa800000000001020]:8+128]:8, a4=[[a0+64]:8+24], a5=0xa800000000001020, a6=[0xa800000000001000]:8, a7=[[0xa800000000001020]:8].
  The `jal` at 0xc000000000223734 calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000223754 calls spl7 [0xc00000000006201c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=[0xc000000001645790]:8, a4=0x53435241, a5=[[0xa800000000001020]:8], a6=0xfffffffffeeddead, a7=[[0xa800000000001020]:8+128]:8.
  The `jalr` at 0xc00000000022376c calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc0000000002236a0 targets 0xc0000000002236d8, is taken when a0 != a3(=[0xc000000001645790]:8), and falls through when a0 == a3(=[0xc000000001645790]:8).
  The `bne` branch at 0xc0000000002236ac targets 0xc0000000002236d8, is taken when a5(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc0000000002236b4 targets 0xc0000000002236d8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000002236c0 targets 0xc0000000002236d8, is taken when a5(=[[0xa800000000001020]:8]) != a6(=0xfffffffffeeddead), and falls through when a5(=[[0xa800000000001020]:8]) == a6(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000002236d0 targets 0xc000000000223754, is taken when a7(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a7(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc0000000002236e8 targets 0xc000000000223744, is taken when a0 != a3(=[0xc000000001645790]:8), and falls through when a0 == a3(=[0xc000000001645790]:8).
  The `bne` branch at 0xc0000000002236f8 targets 0xc000000000223744, is taken when a6(=[0xa800000000001000]:8) != a7(=0x53435241), and falls through when a6(=[0xa800000000001000]:8) == a7(=0x53435241).
  The `beq` branch at 0xc000000000223700 targets 0xc000000000223744, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc00000000022370c targets 0xc000000000223744, is taken when a7(=[[0xa800000000001020]:8]) != t0(=0xfffffffffeeddead), and falls through when a7(=[[0xa800000000001020]:8]) == t0(=0xfffffffffeeddead).
  The `beq` branch at 0xc00000000022371c targets 0xc000000000223744, is taken when a3(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc000000000223784 targets 0xc0000000002236d8 and is always taken.
  The return at 0xc00000000022374c has v0 = ?.
- `ioc3_query_CTS` in `6.5.30/unix.IP35` at 0xc00000000022378c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc00000000022378c-0xc000000000223890 (260 bytes).
  It occupies 260 bytes.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc0000000002237a8, `ld` at 0xc0000000002237f8.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc0000000002237ac, `ld` at 0xc000000000223800, `ld` at 0xc000000000223864.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc0000000002237c0, `lw` at 0xc00000000022380c.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc0000000002237cc, `ld` at 0xc000000000223818, `ld` at 0xc000000000223868.
  The `ld` at 0xc0000000002237dc loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `lw` at 0xc0000000002237f0 loads 4 bytes from [a0+64]:8 +0x18.
  The `jalr` at 0xc000000000223830 calls (unknown callee) with a0=0x1, a1=?, a2=?, a3=[[0xa800000000001020]:8+128]:8, a4=[[a0+64]:8+24], a5=0xa800000000001020, a6=[0xa800000000001000]:8, a7=[[0xa800000000001020]:8].
  The `jal` at 0xc000000000223838 calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000223858 calls spl7 [0xc00000000006201c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=[0xc000000001645790]:8, a4=0x53435241, a5=[[0xa800000000001020]:8], a6=0xfffffffffeeddead, a7=[[0xa800000000001020]:8+128]:8.
  The `jalr` at 0xc000000000223870 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc0000000002237a4 targets 0xc0000000002237dc, is taken when a0 != a3(=[0xc000000001645790]:8), and falls through when a0 == a3(=[0xc000000001645790]:8).
  The `bne` branch at 0xc0000000002237b0 targets 0xc0000000002237dc, is taken when a5(=[0xa800000000001000]:8) != a4(=0x53435241), and falls through when a5(=[0xa800000000001000]:8) == a4(=0x53435241).
  The `beq` branch at 0xc0000000002237b8 targets 0xc0000000002237dc, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc0000000002237c4 targets 0xc0000000002237dc, is taken when a5(=[[0xa800000000001020]:8]) != a6(=0xfffffffffeeddead), and falls through when a5(=[[0xa800000000001020]:8]) == a6(=0xfffffffffeeddead).
  The `bne` branch at 0xc0000000002237d4 targets 0xc000000000223858, is taken when a7(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a7(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc0000000002237ec targets 0xc000000000223848, is taken when a0 != a3(=[0xc000000001645790]:8), and falls through when a0 == a3(=[0xc000000001645790]:8).
  The `bne` branch at 0xc0000000002237fc targets 0xc000000000223848, is taken when a6(=[0xa800000000001000]:8) != a7(=0x53435241), and falls through when a6(=[0xa800000000001000]:8) == a7(=0x53435241).
  The `beq` branch at 0xc000000000223804 targets 0xc000000000223848, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000223810 targets 0xc000000000223848, is taken when a7(=[[0xa800000000001020]:8]) != t0(=0xfffffffffeeddead), and falls through when a7(=[[0xa800000000001020]:8]) == t0(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000223820 targets 0xc000000000223848, is taken when a3(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc000000000223888 targets 0xc0000000002237dc and is always taken.
  The return at 0xc000000000223850 has v0 = ?.
- `ioc3_get_mapid` in `6.5.30/unix.IP35` at 0xc0000000002238e4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000002238e4-0xc000000000223944 (96 bytes).
  It occupies 96 bytes.
  The `ld` at 0xc0000000002238f8 loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `jal` at 0xc00000000022391c calls copyout [0xc00000000004762c] with a0=sp-16, a1=a1 at entry (arg1), a2=0x8, a3=0xc00000000158df40.
  The `beq` branch at 0xc0000000002238ec targets 0xc000000000223934, is taken when a1 == 0, and falls through when a1 != 0.
  The `beq` branch at 0xc00000000022392c targets 0xc000000000223938 and is always taken.
  The return at 0xc00000000022393c has v0 = one of 0x0, 0xe.
- `ioc3_map` in `6.5.30/unix.IP35` at 0xc000000000223944 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000223944-0xc0000000002239d4 (144 bytes).
  It occupies 144 bytes.
  The `ld` at 0xc000000000223970 loads 8 bytes from a0 (arg0 at entry) +0x58.
  The `ld` at 0xc0000000002239a4 loads 8 bytes from a0 (arg0 at entry) +0x70.
  The `ld` at 0xc0000000002239b4 loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `jal` at 0xc000000000223978 calls disable_intrs [0xc000000000220a18] with a0=a0 at entry (arg0), a1=-0x1, a2=?, a3=?, a5=0x16.
  The `jal` at 0xc000000000223988 calls v_mapphys [0xc00000000029ffa4] with a0=a1 at entry (arg1), a1=?, a2=0x4000, a3=?.
  The `beq` branch at 0xc00000000022394c targets 0xc000000000223968, is taken when a2 == 0, and falls through when a2 != 0.
  The `beq` branch at 0xc000000000223958 targets 0xc0000000002239a4, is taken when a2 == at(=0x4000), and falls through when a2 != at(=0x4000).
  The `beq` branch at 0xc000000000223960 targets 0xc00000000022399c and is always taken.
  The `beq` branch at 0xc0000000002239cc targets 0xc000000000223978 and is always taken.
  The return at 0xc00000000022399c has v0 = one of 0x16, ret(v_mapphys).
- `ioc3_unmap` in `6.5.30/unix.IP35` at 0xc0000000002239d4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000002239d4-0xc000000000223b10 (316 bytes).
  It occupies 316 bytes.
  2 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc0000000002239dc, `lw` at 0xc000000000223b00.
  The `sw` at 0xc0000000002239f0 stores 4 bytes to a0 (arg0 at entry) +0x94.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000223a00, `ld` at 0xc000000000223a60.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000223a08, `ld` at 0xc000000000223a54, `ld` at 0xc000000000223ae8.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000223a24, `lw` at 0xc000000000223a74.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc000000000223a30, `ld` at 0xc000000000223a80, `ld` at 0xc000000000223aec.
  3 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x40, including `ld` at 0xc000000000223a3c, `ld` at 0xc000000000223aa8, `ld` at 0xc000000000223abc.
  The `sw` at 0xc000000000223a44 stores 4 bytes to [a0+64]:8 +0x0.
  The `lw` at 0xc000000000223aac loads 4 bytes from [a0+64]:8 +0x8.
  The `sw` at 0xc000000000223ab4 stores 4 bytes to a0 (arg0 at entry) +0x98.
  The `sw` at 0xc000000000223ab8 stores 4 bytes to [a0+64]:8 +0x4.
  The `lw` at 0xc000000000223ac0 loads 4 bytes from [a0+64]:8 +0xc.
  The `sw` at 0xc000000000223ac8 stores 4 bytes to a0 (arg0 at entry) +0x9c.
  The `sw` at 0xc000000000223acc stores 4 bytes to [a0+64]:8 +0x10.
  The `jalr` at 0xc000000000223a94 calls (unknown callee) with a0=0x1, a1=[a0+64]:8, a2=[[0xa800000000001020]:8], a3=[[0xa800000000001020]:8+128]:8, a4=[0xc00000000164578c], a5=0xa800000000001020, a6=[0xc000000001645790]:8.
  The `jal` at 0xc000000000223a9c calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000223adc calls spl7 [0xc00000000006201c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=?, a4=[0xa800000000001000]:8, a5=0x53435241, a6=[[0xa800000000001020]:8], a7=0xfffffffffeeddead.
  The `jalr` at 0xc000000000223af4 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a7=[[0xa800000000001020]:8+128]:8.
  The `bne` branch at 0xc000000000223a0c targets 0xc000000000223a3c, is taken when at(=[0xc000000001645790]:8) != a0, and falls through when at(=[0xc000000001645790]:8) == a0.
  The `bne` branch at 0xc000000000223a14 targets 0xc000000000223a3c, is taken when a4(=[0xa800000000001000]:8) != a5(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a5(=0x53435241).
  The `beq` branch at 0xc000000000223a1c targets 0xc000000000223a3c, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000223a28 targets 0xc000000000223a3c, is taken when a6(=[[0xa800000000001020]:8]) != a7(=0xfffffffffeeddead), and falls through when a6(=[[0xa800000000001020]:8]) == a7(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000223a34 targets 0xc000000000223adc, is taken when t0(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when t0(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc000000000223a5c targets 0xc000000000223aa8, is taken when a6(=[0xc000000001645790]:8) != s0(=a0), and falls through when a6(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000223a64 targets 0xc000000000223aa8, is taken when a4(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a4(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc000000000223a6c targets 0xc000000000223aa8, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000223a78 targets 0xc000000000223aa8, is taken when a2(=[[0xa800000000001020]:8]) != a3(=0xfffffffffeeddead), and falls through when a2(=[[0xa800000000001020]:8]) == a3(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000223a84 targets 0xc000000000223aa8, is taken when a3(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a3(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc000000000223b08 targets 0xc000000000223a3c and is always taken.
  The return at 0xc000000000223ad4 has v0 = one of [0xa800000000001020]:8, ret(splx).
- `ioc3_set_sscr` in `6.5.30/unix.IP35` at 0xc000000000223b10 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000223b10-0xc000000000223c34 (292 bytes).
  It occupies 292 bytes.
  3 such `lw` accesses load 4 bytes from a0 (arg0 at entry) +0x94, including `lw` at 0xc000000000223b10, `lw` at 0xc000000000223be4, `lw` at 0xc000000000223c24.
  2 such `sw` accesses store 4 bytes to a0 (arg0 at entry) +0x94, including `sw` at 0xc000000000223b2c, `sw` at 0xc000000000223bfc.
  2 such `ld` accesses load 8 bytes from abs 0xa800000000001000, including `ld` at 0xc000000000223b48, `ld` at 0xc000000000223b9c.
  3 such `ld` accesses load 8 bytes from abs 0xa800000000001020, including `ld` at 0xc000000000223b4c, `ld` at 0xc000000000223b90, `ld` at 0xc000000000223c0c.
  2 such `lw` accesses load 4 bytes from [0xa800000000001020]:8 +0x0, including `lw` at 0xc000000000223b60, `lw` at 0xc000000000223bb0.
  3 such `ld` accesses load 8 bytes from [0xa800000000001020]:8 +0x80, including `ld` at 0xc000000000223b6c, `ld` at 0xc000000000223bbc, `ld` at 0xc000000000223c10.
  The `ld` at 0xc000000000223b78 loads 8 bytes from a0 (arg0 at entry) +0x40.
  The `sw` at 0xc000000000223b80 stores 4 bytes to [a0+64]:8 +0x0.
  The `jalr` at 0xc000000000223bd0 calls (unknown callee) with a0=0x1, a1=[[0xa800000000001020]:8+128]:8, a2=[0xc000000001645790]:8, a3=[[0xa800000000001020]:8], a4=0xfffffffffeeddead, a5=[0xc00000000164578c].
  The `jal` at 0xc000000000223bd8 calls splx [0xc000000000062054] with a0=[0xc00000000164578c], a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000223c00 calls spl7 [0xc00000000006201c] with a0=0x53435241, a1=[[0xa800000000001020]:8], a2=0xfffffffffeeddead, a3=[[0xa800000000001020]:8+128]:8, a4=0xa800000000001020.
  The `jalr` at 0xc000000000223c18 calls (unknown callee) with a0=0x0, a1=?, a2=?, a3=?, a6=[[0xa800000000001020]:8+128]:8.
  The `beq` branch at 0xc000000000223b20 targets 0xc000000000223bf4, is taken when a2 == 0, and falls through when a2 != 0.
  The `bne` branch at 0xc000000000223b44 targets 0xc000000000223b78, is taken when a2(=[0xc000000001645790]:8) != s0(=a0), and falls through when a2(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000223b50 targets 0xc000000000223b78, is taken when a1(=[0xa800000000001000]:8) != a0(=0x53435241), and falls through when a1(=[0xa800000000001000]:8) == a0(=0x53435241).
  The `beq` branch at 0xc000000000223b58 targets 0xc000000000223b78, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000223b64 targets 0xc000000000223b78, is taken when a1(=[[0xa800000000001020]:8]) != a2(=0xfffffffffeeddead), and falls through when a1(=[[0xa800000000001020]:8]) == a2(=0xfffffffffeeddead).
  The `bne` branch at 0xc000000000223b70 targets 0xc000000000223c00, is taken when a3(=[[0xa800000000001020]:8+128]:8) != 0, and falls through when a3(=[[0xa800000000001020]:8+128]:8) == 0.
  The `bne` branch at 0xc000000000223b98 targets 0xc000000000223be4, is taken when a2(=[0xc000000001645790]:8) != s0(=a0), and falls through when a2(=[0xc000000001645790]:8) == s0(=a0).
  The `bne` branch at 0xc000000000223ba0 targets 0xc000000000223be4, is taken when a0(=[0xa800000000001000]:8) != a1(=0x53435241), and falls through when a0(=[0xa800000000001000]:8) == a1(=0x53435241).
  The `beq` branch at 0xc000000000223ba8 targets 0xc000000000223be4, is taken when v0(=[0xa800000000001020]:8) == 0, and falls through when v0(=[0xa800000000001020]:8) != 0.
  The `bne` branch at 0xc000000000223bb4 targets 0xc000000000223be4, is taken when a3(=[[0xa800000000001020]:8]) != a4(=0xfffffffffeeddead), and falls through when a3(=[[0xa800000000001020]:8]) == a4(=0xfffffffffeeddead).
  The `beq` branch at 0xc000000000223bc0 targets 0xc000000000223be4, is taken when a1(=[[0xa800000000001020]:8+128]:8) == 0, and falls through when a1(=[[0xa800000000001020]:8+128]:8) != 0.
  The `beq` branch at 0xc000000000223bf8 targets 0xc000000000223b30 and is always taken.
  The `beq` branch at 0xc000000000223c2c targets 0xc000000000223b78 and is always taken.
  The return at 0xc000000000223bec has v0 = [a0+148].
- `ioc3_console_write` in `6.5.30/unix.IP35` at 0xc000000000223c34 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000223c34-0xc000000000223d88 (340 bytes).
  It occupies 340 bytes.
  7 such `lw` accesses load 4 bytes from [0xc00000000163c938]:8 +0xb8, including `lw` at 0xc000000000223c38, `lw` at 0xc000000000223d14, `lw` at 0xc000000000223d24.
  The `sd` at 0xc000000000223c60 stores 8 bytes to a2 (arg2 at entry) +0x0.
  The `sb` at 0xc000000000223c88 stores 1 byte to [0xc00000000163c938]:8 +0x20178.
  6 such `lbu` accesses load 1 byte from [0xc00000000163c938]:8 +0x2017d, including `lbu` at 0xc000000000223c8c, `lbu` at 0xc000000000223ca4, `lbu` at 0xc000000000223cb4.
  The `lbu` at 0xc000000000223c98 loads 1 byte from ?.
  The `sw` at 0xc000000000223d10 stores 4 bytes to [0xc00000000163c938]:8 +0xb8.
  The `bne` branch at 0xc000000000223c48 targets 0xc000000000223c60, is taken when t0(=0x10000000) != t2(?), and falls through when t0(=0x10000000) == t2(?).
  The `beq` branch at 0xc000000000223c58 targets 0xc000000000223d7c and is always taken.
  The `bne` branch at 0xc000000000223c74 targets 0xc000000000223c8c, is taken when a1 != 0, and falls through when a1 == 0.
  The `beq` branch at 0xc000000000223c7c targets 0xc000000000223cfc and is always taken.
  The `beq` branch at 0xc000000000223c84 targets 0xc000000000223cfc, is taken when a1(?) == 0, and falls through when a1(?) != 0.
  The `bne` branch at 0xc000000000223c9c targets 0xc000000000223c84, is taken when (a6(=[[0xc00000000163c938]:8+131453]:1) & 0x20) != 0, and falls through when (a6(=[[0xc00000000163c938]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc000000000223cac targets 0xc000000000223c84, is taken when (a7(=[[0xc00000000163c938]:8+131453]:1) & 0x20) != 0, and falls through when (a7(=[[0xc00000000163c938]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc000000000223cbc targets 0xc000000000223c84, is taken when (t0(=[[0xc00000000163c938]:8+131453]:1) & 0x20) != 0, and falls through when (t0(=[[0xc00000000163c938]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc000000000223ccc targets 0xc000000000223c84, is taken when (t1(=[[0xc00000000163c938]:8+131453]:1) & 0x20) != 0, and falls through when (t1(=[[0xc00000000163c938]:8+131453]:1) & 0x20) == 0.
  The `bne` branch at 0xc000000000223cdc targets 0xc000000000223c84, is taken when (t2(=[[0xc00000000163c938]:8+131453]:1) & 0x20) != 0, and falls through when (t2(=[[0xc00000000163c938]:8+131453]:1) & 0x20) == 0.
  The `beq` branch at 0xc000000000223cec targets 0xc000000000223ca4, is taken when (t3(=[[0xc00000000163c938]:8+131453]:1) & 0x20) == 0, and falls through when (t3(=[[0xc00000000163c938]:8+131453]:1) & 0x20) != 0.
  The `beq` branch at 0xc000000000223cf4 targets 0xc000000000223c84 and is always taken.
  The `bne` branch at 0xc000000000223d08 targets 0xc000000000223c60, is taken when t0(=0x10000000) != t8(?), and falls through when t0(=0x10000000) == t8(?).
  The `bne` branch at 0xc000000000223d1c targets 0xc000000000223d08, is taken when a5(=0x30000000) != t9(?), and falls through when a5(=0x30000000) == t9(?).
  The `bne` branch at 0xc000000000223d2c targets 0xc000000000223d04, is taken when a5(=0x30000000) != v0(?), and falls through when a5(=0x30000000) == v0(?).
  The `bne` branch at 0xc000000000223d3c targets 0xc000000000223d04, is taken when a5(=0x30000000) != v1(?), and falls through when a5(=0x30000000) == v1(?).
  The `bne` branch at 0xc000000000223d4c targets 0xc000000000223d04, is taken when a5(=0x30000000) != a3(?), and falls through when a5(=0x30000000) == a3(?).
  The `bne` branch at 0xc000000000223d5c targets 0xc000000000223d04, is taken when a5(=0x30000000) != t3(?), and falls through when a5(=0x30000000) == t3(?).
  The `beq` branch at 0xc000000000223d6c targets 0xc000000000223d24, is taken when a5(=0x30000000) == t8(?), and falls through when a5(=0x30000000) != t8(?).
  The `beq` branch at 0xc000000000223d74 targets 0xc000000000223d04 and is always taken.
  The `beq` branch at 0xc000000000223d80 targets 0xc000000000223d10 and is always taken.
  The return at 0xc000000000223cfc has v0 = 0x0.
- `idbg_gdb` in `6.5.30/unix.IP35` at 0xc0000000002466f4 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000002466f4-0xc000000000246728 (52 bytes).
  It occupies 52 bytes.
  The `jal` at 0xc000000000246704 calls dogang [0xc0000000002462f4] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc000000000246718 calls idbg_gang [0xc00000000024659c] with a0=-0x1, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `beq` branch at 0xc0000000002466fc targets 0xc000000000246718, is taken when a0 == at(=-0x1), and falls through when a0 != at(=-0x1).
  The `beq` branch at 0xc000000000246720 targets 0xc000000000246710 and is always taken.
  The return at 0xc000000000246710 has v0 = one of ret(dogang), ret(idbg_gang).
- `panicregs` in `6.5.30/unix.IP35` at 0xc000000000252f10 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000252f10-0xc000000000252fc4 (180 bytes).
  It occupies 180 bytes.
  The `ld` at 0xc000000000252f60 loads 8 bytes from a0 (arg0 at entry) +0x120.
  The `ld` at 0xc000000000252f8c loads 8 bytes from a0 (arg0 at entry) +0x110.
  The `ld` at 0xc000000000252f90 loads 8 bytes from table(0xc00000000158e4a8) +0x0.
  The `jal` at 0xc000000000252f2c calls spl1 [0xc000000000061f6c] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc000000000252f34 calls set_leds [0xc0000000000648c8] with a0=0x3, a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000252f54 calls machine_error_dump [0xc0000000002b10e4] with a0=0xc0000000004c8ae8, a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000252fac calls cmn_err [0xc0000000002ad048] with a0=0x0, a1=0xc0000000004c8af8, a2=a4 at entry (arg4), a3=[a0+288]:8, a4=a0 at entry (arg0), a5=a1 at entry (arg1), a6=entry(0xc00000000158e4a8), a7=[a0+272]:8.
  The `bne` branch at 0xc000000000252f4c targets 0xc000000000252f5c, is taken when v0(=[0xc00000000163d198]) != 0, and falls through when v0(=[0xc00000000163d198]) == 0.
  The return at 0xc000000000252fbc has v0 = ret(cmn_err).
- `mem_range_init` in `6.5.30/unix.IP35` at 0xc00000000025b27c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc00000000025b27c-0xc00000000025b5f0 (884 bytes).
  It occupies 884 bytes.
  6 such `ld` accesses load 8 bytes from abs 0xffffffffffffc400, including `ld` at 0xc00000000025b30c, `ld` at 0xc00000000025b338, `ld` at 0xc00000000025b358.
  5 such `ld` accesses load 8 bytes from [-0x3c00]:8 +0x0, including `ld` at 0xc00000000025b310, `ld` at 0xc00000000025b33c, `ld` at 0xc00000000025b35c.
  3 such `ld` accesses load 8 bytes from ((a0 << 0x3) + [[-0x3c00]:8]:8) +0x0, including `ld` at 0xc00000000025b31c, `ld` at 0xc00000000025b344, `ld` at 0xc00000000025b364.
  The `lw` at 0xc00000000025b324 loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x5f4.
  The `sw` at 0xc00000000025b334 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x5f4.
  The `lw` at 0xc00000000025b348 loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x604.
  The `sw` at 0xc00000000025b354 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x604.
  The `lw` at 0xc00000000025b368 loads 4 bytes from [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x680.
  The `sw` at 0xc00000000025b374 stores 4 bytes to [((a0 << 0x3) + [[-0x3c00]:8]:8)]:8 +0x680.
  2 such `ld` accesses load 8 bytes from ([[-0x3c00]:8]:8 + (a0 << 0x3)) +0x0, including `ld` at 0xc00000000025b384, `ld` at 0xc00000000025b458.
  The `ll` at 0xc00000000025b390 loads 4 bytes from [([[-0x3c00]:8]:8 + (a0 << 0x3))]:8 +0x5fc.
  The `sc` at 0xc00000000025b39c stores 4 bytes to [([[-0x3c00]:8]:8 + (a0 << 0x3))]:8 +0x5fc.
  The `ld` at 0xc00000000025b3b4 loads 8 bytes from abs 0xc0000000016b2520 (Nodepdaindr).
  3 such `lw` accesses load 4 bytes from ?, including `lw` at 0xc00000000025b3c8, `lw` at 0xc00000000025b448, `lw` at 0xc00000000025b504.
  The `ld` at 0xc00000000025b3d0 loads 8 bytes from [0xc0000000016b2520]:8 +0x5e8.
  4 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc00000000025b3dc, `sw` at 0xc00000000025b3e4, `sw` at 0xc00000000025b4f0.
  The `ld` at 0xc00000000025b3ec loads 8 bytes from ([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  The `sd` at 0xc00000000025b420 stores 8 bytes to ([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x78.
  3 such `ld` accesses load 8 bytes from ?, including `ld` at 0xc00000000025b464, `ld` at 0xc00000000025b4b0, `ld` at 0xc00000000025b57c.
  9 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc00000000025b478, `sd` at 0xc00000000025b4b4, `sd` at 0xc00000000025b4b8.
  The `lw` at 0xc00000000025b48c loads 4 bytes from [-0x3c00]:8 +0x61c.
  3 such `ld` accesses load 8 bytes from ([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0, including `ld` at 0xc00000000025b494, `ld` at 0xc00000000025b55c, `ld` at 0xc00000000025b5ac.
  2 such `lhu` accesses load 2 bytes from ?, including `lhu` at 0xc00000000025b4ac, `lhu` at 0xc00000000025b568.
  The `ld` at 0xc00000000025b528 loads 8 bytes from table(0xc0000000016aefb8) +0x0.
  The `ld` at 0xc00000000025b53c loads 8 bytes from table(0xc0000000016acfb8) +0x0.
  2 such `ld` accesses load 8 bytes from ([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x48, including `ld` at 0xc00000000025b540, `ld` at 0xc00000000025b594.
  The `sd` at 0xc00000000025b5c4 stores 8 bytes to ([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x80.
  The `jal` at 0xc00000000025b2a0 calls is_specified [0xc0000000002c4694] with a0=0xc0000000016462f3, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc00000000025b304 calls setupbadmem [0xc00000000004eefc] with a0=?, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=?.
  The `jal` at 0xc00000000025b524 calls bset [0xc000000000319524] with a0=entry(0xc0000000016aefb8), a1=?, a2=?, a3=ret(is_specified).
  The `jal` at 0xc00000000025b538 calls bset [0xc000000000319524] with a0=entry(0xc0000000016acfb8), a1=?, a2=?, a3=?.
  The `beql` branch at 0xc00000000025b3a0 targets 0xc00000000025b390, is taken when t9(?) == 0, and falls through when t9(?) != 0; its branch-likely delay slot `nop` at 0xc00000000025b3a4 runs only when taken and is annulled on fall-through.
  The `bne` branch at 0xc00000000025b404 targets 0xc00000000025b47c, is taken when t8(=[([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) != 0, and falls through when t8(=[([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+120]:8) == 0.
  The `bne` branch at 0xc00000000025b474 targets 0xc00000000025b42c, is taken when a1(?) < 0x6 (signed), and falls through when a1(?) >= 0x6 (signed).
  The `bltz` branch at 0xc00000000025b4a0 targets 0xc00000000025b5b8, is taken when s3(?) < 0 (signed), and falls through when s3(?) >= 0 (signed).
  The `beq` branch at 0xc00000000025b4a8 targets 0xc00000000025b56c and is always taken.
  The `bne` branch at 0xc00000000025b554 targets 0xc00000000025b560, is taken when a1(=[([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s0(?), and falls through when a1(=[([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s0(?).
  The `bltz` branch at 0xc00000000025b560 targets 0xc00000000025b5b8, is taken when a0(?) < 0 (signed), and falls through when a0(?) >= 0 (signed).
  The `beq` branch at 0xc00000000025b56c targets 0xc00000000025b594, is taken when v0(=0xffff) == a2(?), and falls through when v0(=0xffff) != a2(?).
  The `bne` branch at 0xc00000000025b574 targets 0xc00000000025b4b0, is taken when a3(=ret(is_specified)) != 0, and falls through when a3(=ret(is_specified)) == 0.
  The `beq` branch at 0xc00000000025b58c targets 0xc00000000025b4c4 and is always taken.
  The `bne` branch at 0xc00000000025b5a4 targets 0xc00000000025b5b0, is taken when a5(=[([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) != s0(?), and falls through when a5(=[([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3))+72]:8) == s0(?).
  The `bgez` branch at 0xc00000000025b5b0 targets 0xc00000000025b568, is taken when a0(?) >= 0 (signed), and falls through when a0(?) < 0 (signed).
  The return at 0xc00000000025b5e8 has v0 = 0xffff.
- `idbg_init` in `6.5.30/unix.IP35` at 0xc000000000284eb0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000284eb0-0xc000000000284f24 (116 bytes).
  It occupies 116 bytes.
  2 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc000000000284ed8, `sw` at 0xc000000000284ef8.
  The `jal` at 0xc000000000284f0c calls init_mutex [0xc000000000238528] with a0=0xc0000000016b1e68, a1=0x0, a2=0xc0000000004cbf20, a3=-0x1, a5=0xc0000000016b1338.
  The `beq` branch at 0xc000000000284edc targets 0xc000000000284f00, is taken when a2(?) >= 0x14 (signed), and falls through when a2(?) < 0x14 (signed).
  The `bne` branch at 0xc000000000284ef4 targets 0xc000000000284ec8, is taken when a2(?) < 0x14 (signed), and falls through when a2(?) >= 0x14 (signed).
  The return at 0xc000000000284f1c has v0 = 0x1.
- `idbg_id` in `6.5.30/unix.IP35` at 0xc000000000284f24 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000284f24-0xc000000000284f2c (8 bytes).
  It occupies 8 bytes.
  The `lw` at 0xc000000000284f28 loads 4 bytes from a0 (arg0 at entry) +0x3c.
  The return at 0xc000000000284f24 has v0 = [a0+60].
- `idbg_lock` in `6.5.30/unix.IP35` at 0xc000000000284f2c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000284f2c-0xc000000000284f34 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000284f2c has v0 = 0x0.
- `idbg_unlock` in `6.5.30/unix.IP35` at 0xc000000000284f34 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000284f34-0xc000000000284f3c (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000284f34 has v0 = ?.
- `idbg_copydata` in `6.5.30/unix.IP35` at 0xc000000000284f3c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000284f3c-0xc000000000284f44 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000284f3c has v0 = ?.
- `idbg_makedata` in `6.5.30/unix.IP35` at 0xc000000000284f44 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000284f44-0xc000000000284fbc (120 bytes).
  It occupies 120 bytes.
  The `ld` at 0xc000000000284f6c loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `sd` at 0xc000000000284fac stores 8 bytes to ret(kern_calloc) +0x8.
  The `sd` at 0xc000000000284fb0 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xc000000000284f5c calls kern_calloc [0xc00000000022c350] with a0=0x1, a1=0x18, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc000000000284f70 calls copyin [0xc0000000000475a0] with a0=[a0+24]:8, a1=ret(kern_calloc), a2=0x4, a3=?.
  The `jal` at 0xc000000000284f80 calls kern_free [0xc00000000022c468] with a0=ret(kern_calloc), a1=0xa0, a2=?, a3=?.
  The `jal` at 0xc000000000284fa4 calls kern_calloc [0xc00000000022c350] with a0=0x1, a1=0xa0, a2=?, a3=?.
  The `beq` branch at 0xc000000000284f78 targets 0xc000000000284fa4, is taken when v0(=ret(copyin)) == 0, and falls through when v0(=ret(copyin)) != 0.
  The `beq` branch at 0xc000000000284fb4 targets 0xc000000000284f8c and is always taken.
  The return at 0xc000000000284f9c has v0 = one of 0x0, 0xa.
- `idbg_freedata` in `6.5.30/unix.IP35` at 0xc000000000284fbc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000284fbc-0xc000000000285004 (72 bytes).
  It occupies 72 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x18, including `ld` at 0xc000000000284fbc, `ld` at 0xc000000000284fe4.
  The `ld` at 0xc000000000284fd0 loads 8 bytes from [a0+24]:8 +0x8.
  The `sd` at 0xc000000000284ff4 stores 8 bytes to a0 (arg0 at entry) +0x18.
  The `jal` at 0xc000000000284fdc calls kern_free [0xc00000000022c468] with a0=[[a0+24]:8+8]:8, a1=a1 at entry (arg1), a2=[a0+24]:8, a3=a3 at entry (arg3).
  The `jal` at 0xc000000000284fe8 calls kern_free [0xc00000000022c468] with a0=[a0+24]:8, a1=?, a2=[a0+24]:8, a3=?.
  The `beq` branch at 0xc000000000284fc8 targets 0xc000000000284ff8, is taken when a2(=[a0+24]:8) == 0, and falls through when a2(=[a0+24]:8) != 0.
  The `beq` branch at 0xc000000000284fd4 targets 0xc000000000284fe8, is taken when a0(=[[a0+24]:8+8]:8) == 0, and falls through when a0(=[[a0+24]:8+8]:8) != 0.
  The return at 0xc000000000284ffc has v0 = one of ?, ret(kern_free).
- `idbg_load` in `6.5.30/unix.IP35` at 0xc00000000028543c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc00000000028543c-0xc0000000002854e0 (164 bytes).
  It occupies 164 bytes.
  The `ld` at 0xc000000000285480 loads 8 bytes from a0 (arg0 at entry) +0x28.
  The `ld` at 0xc000000000285488 loads 8 bytes from [a0+40]:8 +0x50.
  The `jal` at 0xc000000000285458 calls mutex_lock [0xc0000000002386b8] with a0=0xc0000000016b1e68, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc000000000285460 calls debug_check [0xc00000000028531c] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc000000000285478 calls debug_freeid [0xc0000000002852dc] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000002854a4 calls mutex_unlock [0xc0000000002385f0] with a0=0xc0000000016b1e68, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000002854c0 calls mloadfile [0xc0000000002863dc] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000002854d0 calls debug_init [0xc000000000285004] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `beq` branch at 0xc000000000285468 targets 0xc0000000002854c0, is taken when v0(=ret(debug_check)) == 0, and falls through when v0(=ret(debug_check)) != 0.
  The `beq` branch at 0xc000000000285470 targets 0xc0000000002854a0, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `bne` branch at 0xc0000000002854c8 targets 0xc000000000285470, is taken when v0(=ret(mloadfile)) != 0, and falls through when v0(=ret(mloadfile)) == 0.
  The `beq` branch at 0xc0000000002854d8 targets 0xc000000000285470 and is always taken.
  The return at 0xc0000000002854b8 has v0 = ?.
- `idbg_unload` in `6.5.30/unix.IP35` at 0xc0000000002854e0 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000002854e0-0xc000000000285644 (356 bytes).
  It occupies 356 bytes.
  The `ld` at 0xc0000000002854f8 loads 8 bytes from a0 (arg0 at entry) +0x18.
  The `ld` at 0xc000000000285510 loads 8 bytes from a0 (arg0 at entry) +0x50.
  The `ld` at 0xc00000000028553c loads 8 bytes from [a0+24]:8 +0x8.
  The `ld` at 0xc000000000285540 loads 8 bytes from [[a0+24]:8+8]:8 +0x68.
  The `sw` at 0xc0000000002855a8 stores 4 bytes to abs 0xc0000000016b1e18.
  The `ld` at 0xc0000000002855b0 loads 8 bytes from [a0+24]:8 +0x10.
  The `ld` at 0xc000000000285618 loads 8 bytes from [0xc000000001645c98]:8 +0x28.
  The `lw` at 0xc00000000028561c loads 4 bytes from [[0xc000000001645c98]:8+40]:8 +0x48.
  The `sw` at 0xc000000000285628 stores 4 bytes to [[0xc000000001645c98]:8+40]:8 +0x48.
  The `ld` at 0xc00000000028562c loads 8 bytes from a0 (arg0 at entry) +0x38.
  The `sw` at 0xc000000000285640 stores 4 bytes to table(0xc0000000016b1e18) +0x0.
  The `jal` at 0xc000000000285500 calls mutex_lock [0xc0000000002386b8] with a0=0xc0000000016b1e68, a1=0x19, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jalr` at 0xc000000000285588 calls (unknown callee) with a0=?, a1=?, a2=?, a3=0xc0000fbe00000000, a4=-0x1, a5=-0x1, a7=0xffffffffe00007df.
  The `jal` at 0xc000000000285598 calls mutex_unlock [0xc0000000002385f0] with a0=0xc0000000016b1e68, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000002855b4 calls symtab_rele [0xc000000000285eac] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000002855c0 calls mutex_unlock [0xc0000000002385f0] with a0=0xc0000000016b1e68, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000002855ec calls mfindname [0xc0000000002869ac] with a0=[a0+80]:8, a1=0xc0000000004cbc30, a2=sp-32, a3=0xc0000fbe00000000, a4=-0x1, a5=-0x1, a6=0xffffffff00000000, a7=0xffffffffe00007df.
  The `jal` at 0xc0000000002855fc calls mutex_unlock [0xc0000000002385f0] with a0=0xc0000000016b1e68, a1=?, a2=?, a3=?.
  The `jalr` at 0xc00000000028560c calls (unknown callee) with a0=?, a1=?, a2=?, a3=?.
  The `bne` branch at 0xc000000000285534 targets 0xc0000000002855e0, is taken when v0(=[0xc000000001645c98]:8) != a0(=[a0+80]:8), and falls through when v0(=[0xc000000001645c98]:8) == a0(=[a0+80]:8).
  The `bne` branch at 0xc000000000285564 targets 0xc000000000285570, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) < t2(=0xa800000000000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) >= t2(=0xa800000000000000) (unsigned).
  The `bne` branch at 0xc00000000028556c targets 0xc000000000285588, is taken when a6(?) != 0, and falls through when a6(?) == 0.
  The `bne` branch at 0xc000000000285578 targets 0xc0000000002855a8, is taken when v0(=[[[a0+24]:8+8]:8+104]:8) < t8(=0xc000000000000000) (unsigned), and falls through when v0(=[[[a0+24]:8+8]:8+104]:8) >= t8(=0xc000000000000000) (unsigned).
  The `beq` branch at 0xc000000000285580 targets 0xc0000000002855a8, is taken when a0(?) == 0, and falls through when a0(?) != 0.
  The `beq` branch at 0xc000000000285590 targets 0xc0000000002855a8, is taken when v0(=ret(?)) == 0, and falls through when v0(=ret(?)) != 0.
  The `beq` branch at 0xc0000000002855a0 targets 0xc0000000002855cc and is always taken.
  The `beq` branch at 0xc0000000002855f4 targets 0xc00000000028560c, is taken when v0(=ret(mfindname)) == 0, and falls through when v0(=ret(mfindname)) != 0.
  The `beq` branch at 0xc000000000285604 targets 0xc0000000002855cc and is always taken.
  The `beq` branch at 0xc00000000028563c targets 0xc0000000002855bc and is always taken.
  The return at 0xc0000000002855d8 has v0 = one of 0x0, 0x33.
- `idbg_reg` in `6.5.30/unix.IP35` at 0xc000000000285644 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc000000000285644-0xc00000000028564c (8 bytes).
  It occupies 8 bytes.
  The return at 0xc000000000285644 has v0 = 0x59.
- `idbg_unreg` in `6.5.30/unix.IP35` at 0xc00000000028564c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc00000000028564c-0xc000000000285654 (8 bytes).
  It occupies 8 bytes.
  The return at 0xc00000000028564c has v0 = 0x59.
- `setup_lowmem` in `6.5.30/unix.IP35` at 0xc0000000002a29fc (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000002a29fc-0xc0000000002a2e14 (1048 bytes).
  It occupies 1048 bytes.
  2 such `sd` accesses store 8 bytes to table(0xc0000000016b2520) +0x0, including `sd` at 0xc0000000002a2a9c, `sd` at 0xc0000000002a2d9c.
  3 such `sd` accesses store 8 bytes to ?, including `sd` at 0xc0000000002a2ac0, `sd` at 0xc0000000002a2b24, `sd` at 0xc0000000002a2ccc.
  The `ld` at 0xc0000000002a2ad8 loads 8 bytes from abs 0xffffffffffffc400.
  The `ld` at 0xc0000000002a2adc loads 8 bytes from [-0x3c00]:8 +0x0.
  4 such `ld` accesses load 8 bytes from table(0xc0000000016b2520) +0x0, including `ld` at 0xc0000000002a2ae4, `ld` at 0xc0000000002a2db8, `ld` at 0xc0000000002a2df0.
  The `sd` at 0xc0000000002a2af4 stores 8 bytes to ([[-0x3c00]:8]:8 + (a0 << 0x3)) +0x0.
  2 such `sd` accesses store 8 bytes to table(0xc0000000016aefb8) +0x0, including `sd` at 0xc0000000002a2b44, `sd` at 0xc0000000002a2bb0.
  2 such `sd` accesses store 8 bytes to table(0xc0000000016acfb8) +0x0, including `sd` at 0xc0000000002a2b48, `sd` at 0xc0000000002a2ba8.
  The `lh` at 0xc0000000002a2b64 loads 2 bytes from table(0xc000000001647000) +0x0.
  The `ld` at 0xc0000000002a2c2c loads 8 bytes from abs 0xc0000000016b2520 (Nodepdaindr).
  The `ld` at 0xc0000000002a2c40 loads 8 bytes from [0xc0000000016b2520]:8 +0x5e8.
  2 such `sw` accesses store 4 bytes to ?, including `sw` at 0xc0000000002a2c64, `sw` at 0xc0000000002a2ca8.
  The `sd` at 0xc0000000002a2c7c stores 8 bytes to ([[0xc0000000016b2520]:8+1512]:8 + (((a0 << 0x4) + a0) << 0x3)) +0x0.
  The `sd` at 0xc0000000002a2dc8 stores 8 bytes to entry(0xc0000000016b2520) +0x0.
  The `sd` at 0xc0000000002a2dd0 stores 8 bytes to [0xc000000001645f88]:8 +0x400.
  The `lh` at 0xc0000000002a2dd8 loads 2 bytes from abs 0xffffffffffffc428.
  The `sd` at 0xc0000000002a2e08 stores 8 bytes to [0xc000000001645f88]:8 +0x408.
  The `jal` at 0xc0000000002a2a4c calls blkclr [0xc00000000004791c] with a0=?, a1=?, a2=0xa800000000000000, a3=a3 at entry (arg3).
  The `jal` at 0xc0000000002a2a54 calls flush_cache [0xc000000000050b84] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000002a2a64 calls mktables [0xc0000000002a2e14] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=?, a3=?.
  The `jal` at 0xc0000000002a2a80 calls low_mem_alloc [0xc0000000002a2720] with a0=0x8f28, a1=sp-112, a2=0xc0000000004cd360, a3=?.
  The `jal` at 0xc0000000002a2ab8 calls low_mem_alloc [0xc0000000002a2720] with a0=((([0xc000000001645f94] + [0xc000000001645f98]) << 0x3) << 0x0), a1=sp-112, a2=0xc0000000004cd370, a3=?.
  The `jal` at 0xc0000000002a2af0 calls nodepda_numa_init [0xc000000000327350] with a0=a0 at entry (arg0), a1=sp-112, a2=?, a3=?, a4=entry(0xc0000000016b2520), a5=([[-0x3c00]:8]:8 + (a0 << 0x3)).
  The `jal` at 0xc0000000002a2b18 calls low_mem_alloc [0xc0000000002a2720] with a0=(((([0xc000000001645f94] << 0x4) + [0xc000000001645f94]) << 0x3) << 0x0), a1=sp-112, a2=0xc0000000004cd3a0, a3=?.
  The `jal` at 0xc0000000002a2b5c calls slot_getsize [0xc00000000004f3e8] with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0xc0000000002a2b90 calls low_mem_alloc [0xc0000000002a2720] with a0=?, a1=sp-112, a2=0xc0000000004cd3b8, a3=?.
  The `jal` at 0xc0000000002a2ba4 calls low_mem_alloc [0xc0000000002a2720] with a0=?, a1=sp-112, a2=0xc0000000004cd3d0, a3=?.
  The `jal` at 0xc0000000002a2c68 calls low_mem_alloc [0xc0000000002a2720] with a0=?, a1=sp-112, a2=0xc0000000004cd3e8, a3=?, a7=[0xc0000000016461c4].
  The `jal` at 0xc0000000002a2cac calls low_mem_alloc [0xc0000000002a2720] with a0=?, a1=sp-112, a2=0xc0000000004cd3e8, a3=?.
  The `jal` at 0xc0000000002a2d44 calls cmn_err_tag [0xc0000000002ad094] with a0=0x6ec, a1=0x4, a2=0xc0000000004cd408, a3=?.
  The `jal` at 0xc0000000002a2d60 calls low_mem_alloc [0xc0000000002a2720] with a0=0x8f28, a1=sp-112, a2=0xc0000000004cd380, a3=[0xc000000001645f98].
  The `jal` at 0xc0000000002a2d98 calls low_mem_alloc [0xc0000000002a2720] with a0=((([0xc000000001645f98] + [0xc000000001645f94]) << 0x3) << 0x0), a1=sp-112, a2=0xc0000000004cd390, a3=?, a7=table(0xc0000000016b2520).
  The `jal` at 0xc0000000002a2dd4 calls get_cpu_slice [0xc000000000044e04] with a0=[-0x3bd8]:2, a1=?, a2=?, a3=?, a4=[0xc000000001645f88]:8.
  The `beq` branch at 0xc0000000002a2a98 targets 0xc0000000002a2dcc, is taken when s7(=a0) == 0, and falls through when s7(=a0) != 0.
  The `bne` branch at 0xc0000000002a2ad0 targets 0xc0000000002a2d5c, is taken when a2(=[0xc00000000163d3f8]) < a3(=[0xc000000001645f98]) (signed), and falls through when a2(=[0xc00000000163d3f8]) >= a3(=[0xc000000001645f98]) (signed).
  The `beq` branch at 0xc0000000002a2b3c targets 0xc0000000002a2b58 and is always taken.
  The `beq` branch at 0xc0000000002a2b54 targets 0xc0000000002a2bb4, is taken when s0(?) >= 0x8 (signed), and falls through when s0(?) < 0x8 (signed).
  The `beq` branch at 0xc0000000002a2b80 targets 0xc0000000002a2b44, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `beq` branch at 0xc0000000002a2bac targets 0xc0000000002a2b4c and is always taken.
  The `beq` branch at 0xc0000000002a2cc8 targets 0xc0000000002a2c84, is taken when s3(=0x1000000) >= s1(?) (signed), and falls through when s3(=0x1000000) < s1(?) (signed).
  The `bne` branch at 0xc0000000002a2cf8 targets 0xc0000000002a2d34, is taken when t0(?) < a6(?) (unsigned), and falls through when t0(?) >= a6(?) (unsigned).
  The `beq` branch at 0xc0000000002a2d54 targets 0xc0000000002a2d00 and is always taken.
  The `beq` branch at 0xc0000000002a2dc4 targets 0xc0000000002a2ad8 and is always taken.
  The `beq` branch at 0xc0000000002a2e0c targets 0xc0000000002a2aa0 and is always taken.
  The return at 0xc0000000002a2d2c has v0 = ?.
- `panic_coproc_select` in `6.5.30/unix.IP35` at 0xc0000000002af28c (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.30/unix.IP35`.
  Range: 0xc0000000002af28c-0xc0000000002af314 (136 bytes).
  It occupies 136 bytes.
  2 such `lh` accesses load 2 bytes from table(0xc000000001647100) +0x0, including `lh` at 0xc0000000002af2cc, `lh` at 0xc0000000002af2d0.
  The `ll` at 0xc0000000002af2e0 loads 4 bytes from abs 0xc00000000163d428.
  The `sc` at 0xc0000000002af2ec stores 4 bytes to abs 0xc00000000163d428.
  The `jal` at 0xc0000000002af2ac calls getcpuid [0xc00000000004e898] with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0xc0000000002af304 calls dumpsys_helper [0xc0000000002c70dc] with a0=[0xc00000000163d428]+1, a1=0x1, a2=0x1, a3=0xc00000000163d428, a4=[0xc00000000163d428]+1.
  The `beq` branch at 0xc0000000002af298 targets 0xc0000000002af2ac, is taken when at(=[0xc00000000163d418]) == v0(=-0x1), and falls through when at(=[0xc00000000163d418]) != v0(=-0x1).
  The `bne` branch at 0xc0000000002af2d8 targets 0xc0000000002af2a0, is taken when v1(=entry(0xc000000001647100)) != a0(=entry(0xc000000001647100)), and falls through when v1(=entry(0xc000000001647100)) == a0(=entry(0xc000000001647100)).
  The `beql` branch at 0xc0000000002af2f0 targets 0xc0000000002af2e0, is taken when a4(=[0xc00000000163d428]+1) == 0, and falls through when a4(=[0xc00000000163d428]+1) != 0; its branch-likely delay slot `nop` at 0xc0000000002af2f4 runs only when taken and is annulled on fall-through.
  The `bne` branch at 0xc0000000002af2fc targets 0xc0000000002af2a4, is taken when a0(=[0xc00000000163d428]+1) != a1(=0x1), and falls through when a0(=[0xc00000000163d428]+1) == a1(=0x1).
  The `beq` branch at 0xc0000000002af30c targets 0xc0000000002af2a4 and is always taken.
  The return at 0xc0000000002af2a4 has v0 = one of ?, ret(getcpuid), ret(dumpsys_helper).
- `setup_ip32_flags` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x83c8 (n32).
  Evidence grade: inferred.
  Exact binary: `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o`.
  Range: 0x83c8-0x8504 (316 bytes).
  It occupies 316 bytes.
  The `lw` at 0x8400 loads 4 bytes from table(sym(.rodata)+2680) +0x0.
  The `lw` at 0x84e8 loads 4 bytes from abs 0xffffffffffffa2b0.
  The `jal` at 0x83d0 calls get_cpu_irr with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x8468 calls is_specified with a0=sym(arg_bump_leds), a1=?, a2=[sym(four_set_pcaches)], a3=?, a4=-0x1.
  The `jal` at 0x847c calls is_enabled with a0=sym(arg_bump_leds), a1=[sym(reboot_on_panic)], a2=?, a3=?.
  The `jal` at 0x849c calls kopt_find with a0=sym(.rodata)+2664, a1=[sym(reboot_on_panic)], a2=-0x1, a3=?.
  The `jal` at 0x84a8 calls atoi with a0=ret(kopt_find), a1=?, a2=?, a3=?.
  The `beq` branch at 0x83f8 targets 0x8424, is taken when a1(?) >= 0x20 (unsigned), and falls through when a1(?) < 0x20 (unsigned).
  The `beq` branch at 0x8430 targets 0x84e4, is taken when a1(=[sym(is_r4600_flag)]) == 0, and falls through when a1(=[sym(is_r4600_flag)]) != 0.
  The `beq` branch at 0x8448 targets 0x845c, is taken when a2(=[sym(four_set_pcaches)]) == 0, and falls through when a2(=[sym(four_set_pcaches)]) != 0.
  The `beq` branch at 0x8450 targets 0x845c, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0x845c targets 0x84dc, is taken when a0(=[sym(kdebug)]:2) == a4(=-0x1), and falls through when a0(=[sym(kdebug)]:2) != a4(=-0x1).
  The `beq` branch at 0x8470 targets 0x848c, is taken when v0(=ret(is_specified)) == 0, and falls through when v0(=ret(is_specified)) != 0.
  The `beq` branch at 0x8490 targets 0x84c0, is taken when a1(=[sym(reboot_on_panic)]) == a2(=-0x1), and falls through when a1(=[sym(reboot_on_panic)]) != a2(=-0x1).
  The `beq` branch at 0x84c0 targets 0x8494 and is always taken.
  The `beq` branch at 0x84dc targets 0x8464 and is always taken.
  The `beq` branch at 0x84ec targets 0x84fc, is taken when a1(=[-0x5d50]) == 0, and falls through when a1(=[-0x5d50]) != 0.
  The `beq` branch at 0x84f4 targets 0x843c and is always taken.
  The `beq` branch at 0x84fc targets 0x8438 and is always taken.
  The return at 0x84b8 has v0 = ret(atoi).
  The instructions from 0x840c to 0x8420 are not reached by any path from the entry, and their branches are not listed.
  The instructions from 0x84c8 to 0x84d8 are not reached by any path from the entry, and their branches are not listed.
  The indirect jump at 0x8404 uses a jump table whose successors are unresolved.
- `crimestray` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x9f20 (n32).
  Evidence grade: inferred.
  Exact binary: `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o`.
  Range: 0x9f20-0x9f54 (52 bytes).
  It occupies 52 bytes.
  The `jal` at 0x9f34 calls cmn_err with a0=0x4, a1=sym(.rodata)+2984, a2=a1 at entry (arg1), a3=a3 at entry (arg3).
  The return at 0x9f4c has v0 = ret(cmn_err).
- `crime_intr` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xa2cc (n32).
  Evidence grade: inferred.
  Exact binary: `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o`.
  Range: 0xa2cc-0xa578 (684 bytes).
  It occupies 684 bytes.
  2 such `ld` accesses load 8 bytes from a0 (arg0 at entry) +0x138, including `ld` at 0xa2f8, `ld` at 0xa4d0.
  2 such `ld` accesses load 8 bytes from abs 0xffffffffb4000010, including `ld` at 0xa304, `ld` at 0xa32c.
  The `sw` at 0xa364 stores 4 bytes to (sp-208 + ([sym(.data)+1468]:2 << 0x2)) +0x0.
  The `lh` at 0xa370 loads 2 bytes from sym(.data) +0x58c.
  The `sw` at 0xa394 stores 4 bytes to (sp-208 + ([sym(.data)+1420]:2 << 0x2)) +0x0.
  The `lh` at 0xa3a0 loads 2 bytes from sym(.data) +0x5bc.
  3 such `lw` accesses load 4 bytes from table(sym(.data)+1408) +0x98, including `lw` at 0xa3d0, `lw` at 0xa44c, `lw` at 0xa4fc.
  5 such `ld` accesses load 8 bytes from table(sym(.data)+1408) +0xa0, including `ld` at 0xa3d8, `ld` at 0xa474, `ld` at 0xa4cc.
  3 such `ld` accesses load 8 bytes from ?, including `ld` at 0xa3f8, `ld` at 0xa480, `ld` at 0xa518.
  3 such `sd` accesses store 8 bytes to ?, including `sd` at 0xa40c, `sd` at 0xa494, `sd` at 0xa52c.
  The `lw` at 0xa42c loads 4 bytes from ?.
  2 such `lh` accesses load 2 bytes from table(sym(.data)+1408) +0x9e, including `lh` at 0xa448, `lh` at 0xa4bc.
  The `lw` at 0xa454 loads 4 bytes from table(sym(.data)+1408) +0xa8.
  The `lw` at 0xa4a8 loads 4 bytes from table(sym(.data)+1408) +0x90.
  The `lw` at 0xa4b8 loads 4 bytes from table(sym(.data)+1408) +0x94.
  2 such `sd` accesses store 8 bytes to a0 (arg0 at entry) +0x138, including `sd` at 0xa4dc, `sd` at 0xa540.
  The `jal` at 0xa3c8 calls splint with a0=[table(sym(.data)+1408)+152], a1=?, a2=?, a3=?.
  The `jal` at 0xa410 calls vsema with a0=table(sym(.data)+1408)+172, a1=?, a2=?, a3=?.
  The `jal` at 0xa418 calls splx with a0=ret(splint), a1=?, a2=?, a3=?.
  The `jal` at 0xa4a0 calls splint with a0=[table(sym(.data)+1408)+152], a1=?, a2=?, a3=?.
  The `jalr` at 0xa4b4 calls (unknown callee) with a0=a0 at entry (arg0), a1=[table(sym(.data)+1408)+148], a2=?, a3=?.
  The `jal` at 0xa4ec calls splx with a0=ret(splint), a1=?, a2=?, a3=?.
  The `beq` branch at 0xa31c targets 0xa56c and is always taken.
  The `beq` branch at 0xa338 targets 0xa538, is taken when a6(?) == 0, and falls through when a6(?) != 0.
  The `beq` branch at 0xa34c targets 0xa368 and is always taken.
  The `beq` branch at 0xa360 targets 0xa3c0, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0xa378 targets 0xa3b8, is taken when (a6(?) & v0(?)) == 0, and falls through when (a6(?) & v0(?)) != 0.
  The `beq` branch at 0xa390 targets 0xa3c0, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `bne` branch at 0xa3a8 targets 0xa354, is taken when t0(?) != 0, and falls through when t0(?) == 0.
  The `beq` branch at 0xa3b0 targets 0xa358 and is always taken.
  The `beq` branch at 0xa3b8 targets 0xa388 and is always taken.
  The `beq` branch at 0xa3c0 targets 0xa430 and is always taken.
  The `blez` branch at 0xa3ec targets 0xa410, is taken when a3(?) <= 0 (signed), and falls through when a3(?) > 0 (signed).
  The `bne` branch at 0xa408 targets 0xa3f4, is taken when a6(?) < a3(?) (signed), and falls through when a6(?) >= a3(?) (signed).
  The `beq` branch at 0xa424 targets 0xa324, is taken when s7(?) == 0, and falls through when s7(?) != 0.
  The `beq` branch at 0xa438 targets 0xa420, is taken when v1(?) == s6(=-0x1), and falls through when v1(?) != s6(=-0x1).
  The `beq` branch at 0xa464 targets 0xa498, is taken when (a1(=[table(sym(.data)+1408)+158]:2) & 0x4) == 0, and falls through when (a1(=[table(sym(.data)+1408)+158]:2) & 0x4) != 0.
  The `blez` branch at 0xa46c targets 0xa498, is taken when a3(?) <= 0 (signed), and falls through when a3(?) > 0 (signed).
  The `bne` branch at 0xa490 targets 0xa47c, is taken when a5(?) < a3(?) (signed), and falls through when a5(?) >= a3(?) (signed).
  The `bne` branch at 0xa498 targets 0xa3c8, is taken when a6(?) != 0, and falls through when a6(?) == 0.
  The `beq` branch at 0xa4c4 targets 0xa4fc, is taken when (t1(=[table(sym(.data)+1408)+158]:2) & 0x4) == 0, and falls through when (t1(=[table(sym(.data)+1408)+158]:2) & 0x4) != 0.
  The `beq` branch at 0xa4f4 targets 0xa424 and is always taken.
  The `blez` branch at 0xa508 targets 0xa4ec, is taken when a3(?) <= 0 (signed), and falls through when a3(?) > 0 (signed).
  The `bne` branch at 0xa528 targets 0xa514, is taken when a5(?) < a3(?) (signed), and falls through when a5(?) >= a3(?) (signed).
  The `beq` branch at 0xa530 targets 0xa4ec and is always taken.
  The `beq` branch at 0xa570 targets 0xa344 and is always taken.
  The return at 0xa564 has v0 = ?.
- `ecc_fixmem` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xddec (n32).
  Evidence grade: inferred.
  Exact binary: `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o`.
  Range: 0xddec-0xde60 (116 bytes).
  It occupies 116 bytes.
  The `ld` at 0xde18 loads 8 bytes from table(sym(ecc_info)) +0x68.
  The `lw` at 0xde1c loads 4 bytes from table(sym(ecc_info)) +0x7c.
  2 such `sd` accesses store 8 bytes to a2 (arg2 at entry) +0x8, including `sd` at 0xde28, `sd` at 0xde5c.
  The `lw` at 0xde2c loads 4 bytes from table(sym(ecc_info)) +0x80.
  The `sw` at 0xde30 stores 4 bytes to table(sym(ecc_info)) +0x84.
  The `sw` at 0xde34 stores 4 bytes to table(sym(ecc_info)) +0x88.
  The `ld` at 0xde3c loads 8 bytes from abs 0xffffffffb4000260.
  The `sw` at 0xde48 stores 4 bytes to table(sym(ecc_info)) +0x8c.
  The `lw` at 0xde4c loads 4 bytes from table(sym(ecc_info)) +0x78.
  The `bgez` branch at 0xde14 targets 0xde4c, is taken when v0(=([sym(r4000_config)] << 0x2e)) >= 0 (signed), and falls through when v0(=([sym(r4000_config)] << 0x2e)) < 0 (signed).
  The `beq` branch at 0xde58 targets 0xde2c and is always taken.
  The return at 0xde44 has v0 = 0x1.

Skipped no routines.
