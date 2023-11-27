.section .text, "ax"
.global _interrupt_handler
_interrupt_handler:
    csrw    mscratch,ra
    csrr    ra,mcause
    addi    ra,ra,-11
    beqz    ra, _system_call
    csrr    ra,mscratch
    addi	sp,sp,-44
    sw	    gp,40(sp)
    sw	    ra,36(sp)
    sw	    t0,32(sp)
    sw	    t1,28(sp)
    sw	    t2,24(sp)
    sw	    a0,20(sp)
    sw	    a1,16(sp)
    sw	    a2,12(sp)
    sw	    a3,8(sp)
    sw	    a4,4(sp)
    sw	    a5,0(sp)
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop
    call    c_interrupt_handler
    lw	    gp,40(sp)
    lw	    ra,36(sp)
    lw	    t0,32(sp)
    lw	    t1,28(sp)
    lw	    t2,24(sp)
    lw	    a0,20(sp)
    lw	    a1,16(sp)
    lw	    a2,12(sp)
    lw	    a3,8(sp)
    lw	    a4,4(sp)
    lw	    a5,0(sp)
    addi    sp,sp,44
    mret

_system_call:
    csrr    ra,mscratch
    csrw    mepc,ra
    csrw    mscratch,gp
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop
    call    c_system_call
    csrr    gp,mscratch
    mret

.global GetTicks, GetController, MemMove, MemSet, MemCpy

GetTicks:
    li a5, 1
    ecall
GetController:
    li a5, 2
    ecall
MemMove:
    li a5, 3
    ecall
MemSet:
    li a5, 4
    ecall
MemCpy:
    li a5, 5
    ecall

.global OPERATION_SETTING, PHYSICS_SIMULATION, SECURITY_CHECK, IMPORT_EXPORT, ADD_HOTKEY, MULTIPLE_KEY_CHECK, REMOVE_HOTKEY, CMD_INTERRUPT, VIDEO_INTERRUPT, CARTRIDGE_INTERRUPT, ILLEGAL_INST_INTERRUPT, TIMER_INTERRUPT, SET_GRAPHIC_MODE, SET_SMALL_COLOR_PALETTE, CHANGE_SMALL_SPRITE_COLOR

OPERATION_SETTING:
    li a5, 6
    ecall
PHYSICS_SIMULATION:
    li a5, 7
    ecall
SECURITY_CHECK:
    li a5, 8
    ecall
IMPORT_EXPORT:
    li a5, 9
    ecall
ADD_HOTKEY:
    li a5, 10
    ecall
MULTIPLE_KEY_CHECK:
    li a5, 11
    ecall
REMOVE_HOTKEY:
    li a5, 12
    ecall
CMD_INTERRUPT:
    li a5, 13
    ecall
VIDEO_INTERRUPT:
    li a5, 14
    ecall
CARTRIDGE_INTERRUPT:
    li a5, 15
    ecall
ILLEGAL_INST_INTERRUPT:
    li a5, 16
    ecall
TIMER_INTERRUPT:
    li a5, 17
    ecall
SET_GRAPHIC_MODE:
    li a5, 18
    ecall
SET_SMALL_COLOR_PALETTE:
    li a5, 19
    ecall
CHANGE_SMALL_SPRITE_COLOR:
    li a5, 20
    ecall

.global DRAW_SPRITE, ERASE_SMALL_SPRITE, MOVE_SMALL_SPRITE, SET_MEDIUM_COLOR_PALETTE, DRAW_MEDIUM_SPRITE, ERASE_MEDIUM_SPRITE, CHANGE_MEDIUM_SPRITE_COLOR, SET_LARGE_COLOR_PALETTE, DRAW_LARGE_SPRITE, ERASE_LARGE_SPRITE, CHANGE_LARGE_SPRITE_COLOR, SET_BACKGROUND_COLOR_PALETTE, CHANGE_BACKGROUND_COLOR_PALETTE, DRAW_BACKGROUND_SPRITE, ERASE_BACKGROUND_SPRITE

DRAW_SPRITE:
    li a5, 21
    ecall
ERASE_SMALL_SPRITE:
    li a5, 22
    ecall
MOVE_SMALL_SPRITE:
    li a5, 23
    ecall
SET_MEDIUM_COLOR_PALETTE:
    li a5, 24
    ecall
DRAW_MEDIUM_SPRITE:
    li a5, 25
    ecall
ERASE_MEDIUM_SPRITE:
    li a5, 26
    ecall
CHANGE_MEDIUM_SPRITE_COLOR:
    li a5, 27
    ecall
SET_LARGE_COLOR_PALETTE:
    li a5, 28
    ecall
DRAW_LARGE_SPRITE:
    li a5, 29
    ecall
ERASE_LARGE_SPRITE:
    li a5, 30
    ecall
CHANGE_LARGE_SPRITE_COLOR:
    li a5, 31
    ecall
SET_BACKGROUND_COLOR_PALETTE:
    li a5, 32
    ecall
CHANGE_BACKGROUND_COLOR_PALETTE:
    li a5, 33
    ecall
DRAW_BACKGROUND_SPRITE:
    li a5, 34
    ecall
ERASE_BACKGROUND_SPRITE:
    li a5, 35
    ecall

.global EMULATOR, CHECK_LOCATION, CHECK_CONTROLLER, NUM_CONTROLLERS, RAND_NUM, GET_TICKS_UTILS, GET_CONTROLLER_UTILS, LOAD_PREVIOUS_STATE, SET_TEXT_MODE, INIT_THREAD

EMULATOR:
    li a5, 36
    ecall
CHECK_LOCATION:
    li a5, 37
    ecall
CHECK_CONTROLLER:
    li a5, 38
    ecall
NUM_CONTROLLERS:
    li a5, 39
    ecall
RAND_NUM:
    li a5, 40
    ecall
GET_TICKS_UTILS:
    li a5, 41
    ecall
GET_CONTROLLER_UTILS:
    li a5, 42
    ecall
LOAD_PREVIOUS_STATE:
    li a5, 43
    ecall
SET_TEXT_MODE:
    li a5, 44
    ecall
INIT_THREAD:
    li a5, 45
    ecall
