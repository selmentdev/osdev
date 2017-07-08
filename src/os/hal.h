#pragma once

void HAL_PortOutByte(int port, unsigned char v);
void HAL_PortOutWord(int port, unsigned short v);
void HAL_PortOutDword(int port, unsigned int v);
unsigned char HAL_PortInByte(int port);
unsigned short HAL_PortInWord(int port);
unsigned int HAL_PortInDword(int port);
void HAL_PauseKernel();