//! Example 03 - DataBank memory reallocation
/*!
  This simply example demonstrates the use of reallocation function. When
  reallocating, you need at least the sum of the old allocation size with
  the new size of non-fragmented free space in your memory pool.

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
#include <MemoryManager.h>

/*!
  Main function.
*/
int main(){
  /*!
    Allocating...
  */
  uint8_t* ui8Sample = (uint8_t*) DataBank_calloc(sizeof(uint8_t), 20);
  if (ui8Sample == NULL){                                                                                 /*!< If not possible memory pool allocation... */
    printf("ERROR: Allocation memories error!");                                                          /*!< Print an error message and returns 2 for the operational system. */
    return 2;                                                                                             /*!< You can treat the problem any way you want! */
  }

  /*!
    Writing data in Memory Pool...
  */
  uint8_t ui8Buffer = 0;
  for (ui8Buffer = 0 ; ui8Buffer < 20 ; ui8Buffer++){
    ui8Sample[ui8Buffer] = ui8Buffer;
  }

  /*!
    Reading data in Memory Pool...
  */
  printf("Printing the contents of the ui8Sample pointer:\n[");
  for (uint8_t ui8Counter = 0 ; ui8Counter < 20 ; ui8Counter++){
    printf(" %d", ui8Sample[ui8Counter]);
  }
  printf(" ]\n");

  /*!
    Reallocating...
  */
  if (DataBank_getMaxFreeSpace(sizeof(uint8_t)) < (20 + 30)){                                             /*!< If not enough space for reallocation... */
    printf("ERROR: No enough space for memory reallocation!");                                            /*!< Print an error message and returns 3 for the operational system. */
    return 3;                                                                                             /*!< You can treat the problem any way you want! */
  }
  DataBank_realloc(sizeof(uint8_t), &ui8Sample, 20, 30);
  if (DataBank_realloc(sizeof(uint8_t), &ui8Sample, 20, 30) != DATA_BANK_STATUS_REALLOCATED_MEMORY){      /*!< If the pointer is NULL... */
    printf("ERROR: Dynamic memory reallocation error!");                                                  /*!< Print an error message and returns 2 for the operational system. */
    return 2;                                                                                             /*!< You can treat the problem any way you want! */
  }

  /*!
    Writing data in Memory Pool...
  */
  for (ui8Buffer = 20 ; ui8Buffer < 30 ; ui8Buffer++){
    ui8Sample[ui8Buffer] = ui8Buffer;
  }

  /*!
    Reading data in Memory Pool...
  */
  printf("\nPrinting the contents of the ui8SampleB pointer:\n[");
  for (uint8_t ui8Counter = 0 ; ui8Counter < 30 ; ui8Counter++){
    printf(" %d", ui8Sample[ui8Counter]);
  }
  printf(" ]");

  return 0;
}

