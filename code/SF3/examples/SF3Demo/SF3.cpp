/************************************************************************/
/*                                                      */
/*  SF3.cpp  --  Function Declarations for SF3               */
/*                                                      */
/************************************************************************/
/*  Author:   Samuel Lowe                               */
/*  Copyright (c) 2016, Digilent Inc. All rights reserved.          */

/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/************************************************************************/
/*  Module Description:                         */
/*                                            */
/*  This is the function cpp file for the SF3 library         */
/*                                            */
/************************************************************************/
/*  Revision History:                           */
/*                                            */
/*  10/5/2016 (AndrewH) : started                         */
/*  10/7/2016(SamL): created                   */
/*                                            */
/************************************************************************/

#include "SF3.h"


/* ------------------------------------------------------------ */
/*  SF3
**
**  Parameters:
**    none
**
**  Return Value:
**    none
**
**  Errors:
**    none
**
**  Description:
**    starts the spi controller the sf3 uses
*/
void SF3::begin() {
  SPI.begin();

  SPI.beginTransaction(SPISettings(500000, SPI_MSBFIRST, SPI_MODE0));
}

/* ------------------------------------------------------------ */
/*  getDeviceID
**
**  Parameters:
**    none
**
**  Return Value:
**    
**
**  Errors:
**    none
**
**  Description:
**    queries and recieves the DeviceID of the SF3 (0x19)
*/
int SF3::getDeviceID() {
  uint8_t rcvByte[3];

  
  waitForWrite();
  writeEnable();

  digitalWrite(SS, 0);
  SPI.transfer(COMMAND_READ_ID);
  rcvByte[0] = SPI.transfer(0x00);
  rcvByte[1] = SPI.transfer(0x00);
  rcvByte[2] = SPI.transfer(0x00);
  digitalWrite(SS, 1);

  return rcvByte[2];
}

/* ------------------------------------------------------------ */
/*  normalRead
**
**  Parameters:
**    address: address to read
**
**  Return Value:
**    value: byte at selected address
**
**  Errors:
**    none
**
**  Description:
**    reads a byte value from flash memory
*/
uint8_t SF3::normalRead(int address) {
  uint8_t sndByte[4]= { 0 };
  uint8_t rcvByte;

  sndByte[0] = COMMAND_RANDOM_READ;

  sndByte[1] = (address >> 16);
  sndByte[2] = (address >> 8);
  sndByte[3] = address;

  
  waitForWrite();
  writeEnable();

  digitalWrite(SS, 0);
  SPI.transfer(sndByte[0]);
  SPI.transfer(sndByte[1]);
  SPI.transfer(sndByte[2]);
  SPI.transfer(sndByte[3]);
  
  rcvByte = SPI.transfer(0x00);
  digitalWrite(SS, 1);
  
  return rcvByte;
}


/* ------------------------------------------------------------ */
/*  writePage
**
**  Parameters:
**    uint8_t page: page data to write to the sf3
**    address: address to read
**
**  Return Value:
**    value: byte at selected address
**
**  Errors:
**    none
**
**  Description:
**    writes a page to memory. see data sheet for consequences 
**    of different start addreesses
*/
void SF3::writePage(uint8_t* writeData, int address) {
  uint8_t sndBytes[4] = { 0 };

  sndBytes[0] = COMMAND_PAGE_PROGRAM;
  sndBytes[1] = (uint16_t)(address >> 16);
  sndBytes[2] = (uint16_t)(address >> 8);
  sndBytes[3] = (uint16_t)address;

  
  
  waitForWrite();
  writeEnable();

  digitalWrite(SS, 0);
  SPI.transfer(sndBytes[0]);
  SPI.transfer(sndBytes[1]);
  SPI.transfer(sndBytes[2]);
  SPI.transfer(sndBytes[3]);
  

  for(int i = 0; i < 256; i++){
    SPI.transfer(writeData[i]);
  }
  digitalWrite(SS, 1);
}

/* ------------------------------------------------------------ */
/*  subSectorErase
**
**  Parameters:
**    address: address of sector to erase (any address in sector is valid)
**
**  Return Value:
**    none
**
**  Errors:
**    none
**
**  Description:
**    erases a subsector of memory
*/
void SF3::subSectorErase(int address) {
  uint8_t sndBytes[4] = { 0 };

  sndBytes[0] = COMMAND_SUB_SECTOR_ERASE;
  sndBytes[1] = (address >> 16);
  sndBytes[2] = (address >> 8);
  sndBytes[3] = address;
  
  waitForWrite();
  writeEnable();

  digitalWrite(SS, 0);
  SPI.transfer(sndBytes[0]);
  SPI.transfer(sndBytes[1]);
  SPI.transfer(sndBytes[2]);
  SPI.transfer(sndBytes[3]);
  digitalWrite(SS, 1);
}


/* ------------------------------------------------------------ */
/*  bulkErase
**
**  Parameters:
**    none
**
**  Return Value:
**    none
**
**  Errors:
**    none
**
**  Description:
**    erases all of the memory
*/
void SF3::bulkErase() {
  
  
  writeSR(0x00);
  
  waitForWrite();
  writeEnable();

  digitalWrite(SS, 0);
  SPI.transfer(COMMAND_BULK_ERASE);
  digitalWrite(SS, 1);
}

/* ------------------------------------------------------------ */
/*  writeSR
**
**  Parameters:
**    setReg: the byte to write to the status reg
**
**  Return Value:
**    none
**
**  Errors:
**    none
**
**  Description:
**    sets the status register
*/
void SF3::writeSR(uint8_t setReg) {
  waitForWrite();

  writeEnable();

  digitalWrite(SS, 0);
  SPI.transfer(COMMAND_WRITE_STATUSREG);
  SPI.transfer(setReg);
  digitalWrite(SS, 1);

}

/* ------------------------------------------------------------ */
/*  writeEnable
**
**  Parameters:
**    none
**
**  Return Value:
**    none
**
**  Errors:
**    none
**
**  Description:
**    prepares the device to write
*/
void SF3::writeEnable() {
  digitalWrite(SS, 0);
  SPI.transfer(COMMAND_WRITE_ENABLE);
  digitalWrite(SS, 1);

}

/* ------------------------------------------------------------ */
/*  readSR
**
**  Parameters:
**    none
**
**  Return Value:
**    none
**
**  Errors:
**    none
**
**  Description:
**    prepares the device to write
*/
uint8_t SF3::readSR(void) {
  uint8_t SR;

  digitalWrite(SS, 0);
  SPI.transfer(COMMAND_READ_STATUSREG); // command read status register
  
  SR = SPI.transfer(0x00);
  digitalWrite(SS, 1);

  return SR;
}

/* ------------------------------------------------------------ */
/*  waitForWrite
**
**  Parameters:
**    none
**
**  Return Value:
**    none
**
**  Errors:
**    none
**
**  Description:
**    waits for the status to read that the device can be writtin
*/
void SF3::waitForWrite() {
  uint8_t SR;

  SR = readSR();

  while(SR & 0x1)
    SR = readSR();
}
