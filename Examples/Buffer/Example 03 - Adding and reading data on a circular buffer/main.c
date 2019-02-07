//! Example 03 - Adding and reading data on a circular buffer
/*!
  This simply example demonstrates the use of circular buffer.

  Put the "Bits", "MemoryPool", "DataBank" and "Buffer" library folder in the
  same location as this file.

  This code file was written by Jorge Henrique Moreira Santana and is under
  the GNU GPLv3 license. All legal rights are reserved.

  Permissions of this copyleft license are conditioned on making available
  complete source code of licensed works and modifications under the same
  license or the GNU GPLv3. Copyright and license notices must be preserved.
  Contributors provide an express grant of patent rights. However, a larger
  work using the licensed work through interfaces provided by the licensed
  work may be distributed under different terms and without source code for
  the larger work.

  * Permissions:
    -> Commercial use;
    -> Modification;
    -> Distribuition;
    -> Patent Use;
    -> Private Use;

  * Limitations:
    -> Liability;
    -> Warranty;

  * Conditions:
    -> License and copyright notice;
    -> Disclose source;
    -> State changes;
    -> Same license (library);

  For more informations, check the LICENSE document. If you want to use a
  commercial product without having to provide the source code, send an email
  to jorge_henrique_123@hotmail.com to talk.
*/

#include <stdio.h>
#include <inttypes.h>
#include "Buffer/Buffer.h"

int main(){
  /*!
    Initializing DataBank manager...
  */
  if (ui8DataBankInit() != DATABANK_INITIALIZED){                                                                       /*!< If not possible memory pools initialization... */
    printf("ERROR: Data Bank initialization error!");                                                                   /*!< Print an error message and returns 1 for the operational system. */
    return 1;                                                                                                           /*!< You can treat the problem any way you want! */
  }

  /*!
    Initializing buffer manager...
  */
  if (ui8BufferManagerInit() != BUFFER_INITIALIZED){                                                                    /*!< If not possible circular buffer manager initialization... */
    printf("ERROR: Buffer manager initialization error!");                                                              /*!< Print an error message and returns 2 for the operational system. */
    return 2;                                                                                                           /*!< You can treat the problem any way you want! */
  }

  buffer_t* bpBuffer = bpCreateBuffer(CIRCULAR, sizeof(uint16_t), 5);
  if (bpBuffer == NULL){                                                                                                /*!< If not possible circular buffer allocation... */
    printf("\nERROR: Allocation for circular buffer error!");                                                              /*!< Print an error message and returns 3 for the operational system. */
    return 3;                                                                                                           /*!< You can treat the problem any way you want! */
  }

  /*!
    Writing data in circular buffer...
  */
  printf("Writing data on circular buffer... ");
  uint8_t ui8Counter = 0;
  for (ui8Counter = 0; ui8Counter < 15; ui8Counter ++){
    uint16_t ui16Buffer = ui8Counter;
    vPushBufferData(bpBuffer, &ui16Buffer);
  }

  /*!
    Reading data in circular buffer...
  */
  printf("Complete!\n\nReading data on circular buffer:");
  for (ui8Counter = 0; ui8Counter < 10; ui8Counter ++){
    uint16_t* ui16Buffer = (uint16_t*) vpPullBufferData(bpBuffer);
    if (ui16Buffer != NULL){
      printf("\nPosition %d: %d", ui8Counter, *ui16Buffer);
    }
  }

  /*!
    Cleaning the circular buffer... This function is only for circular or stack buffer type.
  */
  printf("\n\nCleaning circular buffer... ");
  vCleanBuffer(bpBuffer);
  uint16_t* ui16Buffer = (uint16_t*) vpPullBufferData(bpBuffer);
  if (ui16Buffer == NULL){
    printf("The circular buffer is empty");
  }
  else{
    printf("ERROR: The circular buffer is not cleaned!");                                                                  /*!< Print an error message and returns 4 for the operational system. */
    return 4;                                                                                                           /*!< You can treat the problem any way you want! */
  }

  /*!
    Deleting the circular buffer...
  */
  printf("\n\nDeleting the circular buffer... ");
  vDeleteBuffer(&bpBuffer);
  if (bpBuffer != NULL){                                                                                                /*!< If the pointer is not NULL... */
    printf("\nERROR: The circular buffer is not deleted!");                                                                /*!< Print an error message and returns 5 for the operational system. */
    return 5;                                                                                                           /*!< You can treat the problem any way you want! */
  }

  printf("Complete deletion!\n");
  return 0;
}