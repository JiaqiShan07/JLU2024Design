#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H
#include"all_h_files.h"
int getValidatedIntegerInput(int min, int max, int zero_allowed);
float getValidatedFloatInput(float min, float max);
char getValidatedCharInput(const char* valid_chars);
void getValidatedStringInput(char* buffer, size_t max_length);
#endif 