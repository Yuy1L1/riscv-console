#include <stdbool.h>

#include "api.h"

uint32_t GetTicks() {
    return systemcall(0, 0, 0, 0, 0, GET_TICKS);
}

uint32_t GetController() {
    return systemcall(0, 0, 0, 0, 0, CONTROLLER_STATUS);
}

uint32_t MemMove(unsigned char* dest, unsigned char* src, int count) {
    return systemcall((uint32_t)dest, (uint32_t) src, (uint32_t) count, 0, 0, MEMORY_MOVE);
}

uint32_t MemSet(unsigned char* dest, unsigned char value, int size) {
    return systemcall((uint32_t)dest, (uint32_t) value, (uint32_t) size, 0, 0, MEMORY_SET);
}

uint32_t MemCpy(unsigned char* dest, unsigned char* src, int count) {
    return systemcall((uint32_t)dest, (uint32_t) src, (uint32_t) count, 0, 0, MEMORY_COPY);
}

uint32_t *operation_setting(char keyboard[]) {
    return systemcall((uint32_t)keyboard, 0, 0, 0, 0, OPERATION_SETTING);
}

uint32_t physics_simulation(char *phys) {
    return systemcall((uint32_t)phys, 0, 0, 0, 0, PHYSICS_SIMULATION);
}

uint32_t *security_check(char files[]) {
    return systemcall((uint32_t)files, 0, 0, 0, 0, SECURITY_CHECK);
}

uint32_t import_export(char *pck, char *file) {
    return systemcall((uint32_t)pck, (uint32_t)file, 0, 0, 0, IMPORT_EXPORT);
}

uint32_t addHotkey(char key[]) {
    return systemcall((uint32_t)key, 0, 0, 0, 0, ADD_HOTKEY);
}

uint32_t multipleKeyCheck(char key[]) {
    return systemcall((uint32_t)key, 0, 0, 0, 0, MULTIPLE_KEY_CHECK);
}

uint32_t removeHotkey(char key[]) {
    return systemcall((uint32_t)key, 0, 0, 0, 0, REMOVE_HOTKEY);
}

void cmd_interrupt() {
    systemcall(0, 0, 0, 0, 0, CMD_INTERRUPT);
}

void video_interrupt() {
    systemcall(0, 0, 0, 0, 0, VIDEO_INTERRUPT);
}

void cartridge_interrupt() {
    systemcall(0, 0, 0, 0, 0, CARTRIDGE_INTERRUPT);
}

void illegal_inst_interrupt() {
    systemcall(0, 0, 0, 0, 0, ILLEGAL_INST_INTERRUPT);
}

void timer_interrupt() {
    systemcall(0, 0, 0, 0, 0, TIMER_INTERRUPT);
}

uint32_t setSmallColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
    return systemcall(palette_number, color, entry_number, 0, 0, SET_SMALL_COLOR_PALETTE);
}

uint32_t setGraphicsMode() {
    return systemcall(0, 0, 0, 0, 0, SET_GRAPHIC_MODE);
}

// int setMediumColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
//     return systemcall(palette_number, color, entry_number, 0, 0, SET_MEDIUM_COLOR_PALETTE);
// }

// int setLargeColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
//     return systemcall(palette_number, color, entry_number, 0, 0, SET_LARGE_COLOR_PALETTE);
// }

// int setBackgroundColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
//     return systemcall(palette_number, color, entry_number, 0, 0, SET_BACKGROUND_COLOR_PALETTE);
// }

// int changeBackgroundColorPalette() {
//     return systemcall(0, 0, 0, 0, 0, CHANGE_BACKGROUND_COLOR_PALETTE);
// }

// void changeSmallSpriteColor() {
//     systemcall(0, 0, 0, 0, 0, CHANGE_SMALL_SPRITE_COLOR);
// }

int16_t drawSmallSprite(uint32_t sprite_control_structure, uint8_t sprite_color) {
    return (int16_t)systemcall(sprite_control_structure, (uint8_t) sprite_color, 0, 0, 0, DRAW_SPRITE);
}

void eraseSmallSprite(uint8_t slot) {
    systemcall(slot, 0, 0, 0, 0, ERASE_SMALL_SPRITE);
}

void moveSmallSprite(uint8_t slot, uint32_t sprite_control_structure, uint8_t sprite_color) {
    systemcall(slot, sprite_control_structure, sprite_color, 0, 0, MOVE_SMALL_SPRITE);
}

int16_t drawMediumSprite(uint32_t sprite_control_structure, uint8_t sprite_color) {
    return (int16_t)systemcall(sprite_control_structure, sprite_color, 0, 0, 0, DRAW_MEDIUM_SPRITE);
}

void eraseMediumSprite(uint8_t slot) {
    systemcall(slot, 0, 0, 0, 0, ERASE_MEDIUM_SPRITE);
}

int16_t drawLargeSprite(uint32_t sprite_control_structure, uint8_t sprite_color) {
    return (int16_t)systemcall(sprite_control_structure, sprite_color, 0, 0, 0, DRAW_LARGE_SPRITE);
}

void eraseLargeSprite(uint8_t slot) {
    systemcall(slot, 0, 0, 0, 0, ERASE_LARGE_SPRITE);
}

int drawBackgroundSprite(uint32_t sprite_control_structure, uint8_t sprite_color, uint32_t mode) {
    return systemcall(sprite_control_structure, sprite_color, mode, 0, 0, DRAW_BACKGROUND_SPRITE);
}

void eraseBackgroundSprite(uint8_t slot, uint32_t mode) {
    systemcall(slot, mode, 0, 0, 0, ERASE_BACKGROUND_SPRITE);
}

int initThread(int priority, uint32_t memsize, void* param, void (*threadFunction)(void *)) {
    return systemcall(priority, memsize, (uint32_t)param, (uint32_t)threadFunction, 0, INIT_THREAD);
}

bool emulator() {
    return systemcall(0, 0, 0, 0, 0, EMULATOR);
}

bool checkLocation() {
    return systemcall(0, 0, 0, 0, 0, CHECK_LOCATION);
}

bool checkController(int controller) {
    return systemcall(controller, 0, 0, 0, 0, CHECK_CONTROLLER);
}

int numControllers() {
    return systemcall(0, 0, 0, 0, 0, NUM_CONTROLLERS);
}

int randNum() {
    return systemcall(0, 0, 0, 0, 0, RAND_NUM);
}

int getTicksUtils() {
    return systemcall(0, 0, 0, 0, 0, GET_TICKS_UTILS);
}

int getControllerUtils() {
    return systemcall(0, 0, 0, 0, 0, GET_CONTROLLER_UTILS);
}

int loadPreviousState(int memory_addr, int mode) {
    return systemcall(memory_addr, mode, 0, 0, 0, LOAD_PREVIOUS_STATE);
}

int setTextMode(int mode) {
    return systemcall(mode, 0, 0, 0, 0, SET_TEXT_MODE);
}