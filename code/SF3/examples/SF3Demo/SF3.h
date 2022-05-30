/************************************************************************/
/*                                                      */
/*  SF3.h  --  Interface Declarations for SF3.cpp               */
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
/*  This is the static PmodSF3 header file         */
/*                                            */
/************************************************************************/
/*  Revision History:                           */
/*                                            */
/*  10/5/2016 (AndrewH) : started                         */
/*  10/7/2016(SamL): created                   */
/*                                            */
/************************************************************************/




#ifndef SF3_H
#define SF3_H

#include <Arduino.h>
#include <SPI.h>


#define COMMAND_WRITE_ENABLE    0x06
#define COMMAND_RANDOM_READ     0x03
#define COMMAND_FAST_READ       0x0B
#define COMMAND_PAGE_PROGRAM    0x02
#define COMMAND_SECTOR_ERASE    0xD8
#define COMMAND_BULK_ERASE      0xC7
#define COMMAND_READ_STATUSREG  (uint8_t)0x05
#define COMMAND_READ_ID         0x9E
#define COMMAND_WRITE_STATUSREG (uint8_t)0x01
#define COMMAND_READ_FLAG_STATUS 0x70
#define COMMAND_SUB_SECTOR_ERASE 0x20

class SF3 {
public:
  void begin();

  int getDeviceID();
  uint8_t normalRead(int address);
  void writePage(uint8_t* writeData, int address);
  void subSectorErase(int address);
  void bulkErase();
  void writeSR(uint8_t setReg);
  uint8_t readSR(void);
  void writeEnable();
  void waitForWrite();
};

#endif
