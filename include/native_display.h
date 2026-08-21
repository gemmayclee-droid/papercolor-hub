#pragma once

#include <Arduino.h>
#include <M5Unified.h>
#include <SD.h>

bool beginNativeDisplay();
bool beginSharedSd();
void nativeHeader(const char* title, uint32_t accent = RED);
void nativeFooter(const char* text);

