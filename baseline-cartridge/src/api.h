#ifndef __INCLUDE_STDINT__
#define __INCLUDE_STDINT__
#include <stdint.h>
#endif

#include <stdbool.h>

#ifndef API_H
#define API_H

#define BACKGROUND_DATA 0x50000000
#define LARGE_SPRITE_DATA 0x50090000
#define MEDIUM_SPRITE_DATA 0x500D0000
#define SMALL_SPRITE_DATA 0x500E0000
#define BACKGROUND_PALETTES 0x500F0000
#define LARGE_SPRITE_PALETTES 0x500F1000
#define MEDIUM_SPRITE_PALETTES 0x500F2000
#define SMALL_SPRITE_PALETTES 0x500F3000
#define FONT_DATA 0x500F4000
#define TEXT_DATA 0x500F4800
#define TEXT_COLOR 0x500F5100
#define BACKGROUND_CONTROLS 0x500F5A00
#define LARGE_SPRITE_CONTROLS 0x500F5B00
#define MEDIUM_SPRITE_CONTROLS 0x500F5F00
#define SMALL_SPRITE_CONTROLS 0x500F6300
#define TEXT_PALETTES 0x500F6700
#define MODE_CONTROL_REGISTER 0x500F6780
#define FOREGROUND_PALETTE_ENTRY_0 0x500F6700
#define BACKGROUND_PALETTE_ENTRY_0 0x500F6740
#define BACKGROUND_DATA_IMAGE_PIXEL_MODE_0 0x50000000
#define BACKGROUND_DATA_IMAGE_TILE_MODE_0 0x50000000
#define TILE_IMAGE_ENTRY_0 0x50000000
#define SUB_IMAGE_TILE_DATA_0 0x50010000
#define BACKGROUND_CONTROLS_0 0x500F5A00
#define LARGE_SPRITE_DATA_0 0x50090000
#define LARGE_SPRITE_CONTROLS_0 0x500F5B00
#define MEDIUM_SPRITE_DATA_0 0x500D0000
#define MEDIUM_SPRITE_CONTROLS_0 0x500F5F00
#define SMALL_SPRITE_DATA_0 0x500E0000
#define SMALL_SPRITE_CONTROLS_0 0x500F6300
#define BACKGROUND_PALETTE_0 0x500F0000
#define LARGE_SPRITE_PALETTE_0 0x500F1000
#define MEDIUM_SPRITE_PALETTE_0 0x500F2000
#define SMALL_SPRITE_PALETTE_0 0x500F3000

#define INTERRUPT_ENABLE_REGISTER 0x40000000
#define INTERRUPT_PENDING_REGISTER 0x40000004
#define MACHINE_TIME 0x40000008
#define MACHINE_TIME_COMPARE 0x40000010
#define MULTI_BUTTON_CONTROLLER_STATUS_REGISTER 0x40000018
#define CARTRIDGE_STATUS_REGISTER 0x4000001C
#define DMA_CHANNEL_1_SOURCE_REGISTER 0x40000020
#define DMA_CHANNEL_1_DESTINATION_REGISTER 0x40000024
#define DMA_CHANNEL_1_COMMAND_REGISTER 0x40000028
#define DMA_CHANNEL_1_STATUS_REGISTER 0x4000002C
#define DMA_CHANNEL_2_SOURCE_REGISTER 0x40000030
#define DMA_CHANNEL_2_DESTINATION_REGISTER 0x40000034
#define DMA_CHANNEL_2_COMMAND_REGISTER 0x40000038
#define DMA_CHANNEL_2_STATUS_REGISTER 0x4000003C
#define MACHINE_CLOCK_PERIOD_REGISTER 0x40000040
#define VIDEO_CLOCK_PERIOD_REGISTER 0x40000044

#define FIRMWARE_FLASH 0x00000000
#define CARTRIDGE_ROM 0x20000000
#define CHIPSET_REGISTERS 0x40000000
#define VIDEO_CONTROLLER_MEMORY 0x50000000
#define RAM 0x70000000

#define GET_TICKS 0x00000001
#define CONTROLLER_STATUS 0x00000002
#define MEMORY_MOVE 0x00000003
#define MEMORY_SET 0x00000004
#define MEMORY_COPY 0x00000005
#define OPERATION_SETTING 0x00000006
#define PHYSICS_SIMULATION 0x00000007
#define SECURITY_CHECK 0x00000008
#define IMPORT_EXPORT 0x00000009
#define ADD_HOTKEY 0x0000000A
#define MULTIPLE_KEY_CHECK 0x0000000B
#define REMOVE_HOTKEY 0x0000000C
#define CMD_INTERRUPT 0x0000000D
#define VIDEO_INTERRUPT 0x0000000E
#define CARTRIDGE_INTERRUPT 0x0000000F
#define ILLEGAL_INST_INTERRUPT 0x00000010
#define TIMER_INTERRUPT 0x00000011

#define SET_GRAPHIC_MODE 0x00000012
#define SET_SMALL_COLOR_PALETTE 0x00000013
#define CHANGE_SMALL_SPRITE_COLOR 0x00000014
#define DRAW_SPRITE 0x00000015
#define ERASE_SMALL_SPRITE 0x00000016
#define MOVE_SMALL_SPRITE 0x00000017
#define SET_MEDIUM_COLOR_PALETTE 0x00000018
#define DRAW_MEDIUM_SPRITE 0x00000019
#define ERASE_MEDIUM_SPRITE 0x0000001A
#define CHANGE_MEDIUM_SPRITE_COLOR 0x0000001B
#define SET_LARGE_COLOR_PALETTE 0x0000001C
#define DRAW_LARGE_SPRITE 0x0000001D
#define ERASE_LARGE_SPRITE 0x0000001E
#define CHANGE_LARGE_SPRITE_COLOR 0x0000001F
#define SET_BACKGROUND_COLOR_PALETTE 0x00000020
#define CHANGE_BACKGROUND_COLOR_PALETTE 0x00000021
#define DRAW_BACKGROUND_SPRITE 0x00000022
#define ERASE_BACKGROUND_SPRITE 0x00000023

#define EMULATOR 0x00000024
#define CHECK_LOCATION 0x00000025
#define CHECK_CONTROLLER 0x00000026
#define NUM_CONTROLLERS 0x00000027
#define RAND_NUM 0x00000028
#define GET_TICKS_UTILS 0x00000029
#define GET_CONTROLLER_UTILS 0x0000002A
#define LOAD_PREVIOUS_STATE 0x0000002B
#define SET_TEXT_MODE 0x0000002C

#define INIT_THREAD 0x0000002D

uint32_t systemcall(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t call);

uint32_t GetTicks();

uint32_t GetController();

// API 1
uint32_t *operation_setting(char keyboard[]);

// API 2 - physics
uint32_t physics_simulation(char *phys);

// API 3 - security
uint32_t *security_check(char files[]);

// API 4
uint32_t import_export(char *pck, char *file);

// API 5 - hotkeys
uint32_t addHotkey(char key[]);
uint32_t multipleKeyCheck(char key[]);
uint32_t removeHotkey(char key[]);

// API 6 - interrupts
void cmd_interrupt();
void video_interrupt();
void cartridge_interrupt();
void illegal_inst_interrupt();
void timer_interrupt();

// API 7 - memory management
uint32_t MemMove(unsigned char* dest, unsigned char* src, int count);
uint32_t MemSet(unsigned char* dest, unsigned char value, int size);
uint32_t MemCpy(unsigned char* dest, unsigned char* src, int count);

// API 8 - sprite management
uint32_t setGraphicMode();
uint32_t setSmallColorPaletteA(uint32_t palette_number, uint32_t color, uint32_t entry_number);
void changeSmallSpriteColor();
uint16_t drawSmallSpriteA(uint32_t sprite_control_structure, uint8_t sprite_color);
void eraseSmallSprite(uint8_t slot);
void moveSmallSprite(uint8_t slot, uint32_t sprite_control_structure, uint8_t sprite_color);
int setMediumColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number);
int16_t drawMediumSprite(uint32_t sprite_control_structure, uint8_t sprite_color);
void eraseMediumSprite(uint8_t slot);
int setLargeColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number);
int16_t drawLargeSprite(uint32_t sprite_control_structure, uint8_t sprite_color);
void eraseLargeSprite(uint8_t slot);
int setBackgroundColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number);
int drawBackgroundSprite(uint32_t sprite_control_structure, uint8_t sprite_color, uint32_t mode);
void eraseBackgroundSprite(uint8_t slot, uint32_t mode);

// Caution - APIs below here are not implemented yet
// API 9 - emulator
bool emulator();
bool checkLocation();

// API 10 - threading
int initThread(int priority, uint32_t memsize, void* param, void (*threadFunction)(void *));

// API 11 - controller management
bool checkController(int controller);
int numControllers();

// API 12 - utils
int randNum();
int getTicksUtils();
int getControllerUtils();

// API 13 - game state
int loadPreviousState(int memory_addr, int mode);

// API 14 - modes
int setTextMode();

#endif