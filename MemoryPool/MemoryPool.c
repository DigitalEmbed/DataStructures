/*!
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

#include "MemoryPool.h"

//! Address calculation macro
/*!
  This macro are for calculate the address of a ui8Position specified of the uiDataSpace vector.
*/
#define     uiAdressCalculation(uiDataSpace, uiWritePosition, uiElementSize)      uiDataSpace + uiWritePosition*uiElementSize

//! Displacement bit operation macros
/*!
  This macro are for facilitate the displacement bit calculus.
*/
#define     vSetBit(iNumber, ui8Position)                                         iNumber |= (1 << (ui8Position))
#define     vEraseBit(iNumber, ui8Position)                                       iNumber &= ~(1 << (ui8Position))
#define     ui8ReadBit(iNumber, ui8Position)                                      (((iNumber) >> (ui8Position)) & 1)

//! Static bit vector ui8Position calculus macros
/*!
  This macros are for calculate the ui8Position of cell of bit-vectors type and the bit ui8Position of iNumber of cell on objects mpool_t.
*/
#define     uiAuxiliaryVectorPosition(uiPoolPosition)                             ((uiAuxiliaryVectorSize(uiPoolPosition)) - (1))
#define     uiAuxiliaryVectorBit(uiPoolPosition)                                  ((uiPoolPosition) & (7))

//! Static bit vector manipulation macro
/*!
  This macros are for calculate the iNumber of cell of bit-vectors type on objects mpool_t.
*/
#define     vSetVectorBit(bvAuxVector, ui8Position)                               vSetBit(bvAuxVector[(uiAuxiliaryVectorPosition(ui8Position))], (uiAuxiliaryVectorBit(ui8Position)))
#define     vEraseVectorBit(bvAuxVector, ui8Position)                             vEraseBit(bvAuxVector[(uiAuxiliaryVectorPosition(ui8Position))], (uiAuxiliaryVectorBit(ui8Position)))
#define     ui8ReadVectorBit(bvAuxVector, ui8Position)                            ui8ReadBit(bvAuxVector[(uiAuxiliaryVectorPosition(ui8Position))], (uiAuxiliaryVectorBit(ui8Position)))

//! Math macro
/*!
  This macros are for maths calculus.
*/
#define     iBigger(iNumberA, iNumberB)                                           (iNumberA) > (iNumberB) ? (iNumberA) : (iNumberB)
#define     iSmaller(iNumberA, iNumberB)                                          (iNumberA) < (iNumberB) ? (iNumberA) : (iNumberB)
#define     uiModule(iNumber)                                                     ((iNumber) > 0) ? (iNumber) : (~iNumber + 1)

//! Memory Pool initializer function
/*!
  Create a Memory Pool with the vector stored in vpDataSpace in struct mpMemoryPool.
  \param mpMemoryPool is a pointer of mpool_t type. It is the memory space where the control variables of the memory pool will be.
  \param vpDataSpace is a void pointer parameter. Basically, this parameter is the memory pool.
  \param bvInUse is a unsigned 8-bit integer. The size of this vector is calculated with macro "uiAuxiliaryVectorSize(size-of-vpDataSpace)": On this macro, put the size of the vector vpDataSpace. This vector is for control of in use ui8Position of vpDataSpace.
  \param ui16DataSize is a unsigned 16-bit integer. This is the size of vector vpDataSpace.
  \param ui8ElementSize is a unsigned 8-bit integer. This is the size of the elements.
  \return Returns MEMORYPOOL_INIT_SUCESS or MEMORYPOOL_INIT_ERROR.
*/
uint8_t ui8MPInit(mpool_t* mpMemoryPool, void *vpDataSpace, bitvector_t* bvInUse, uint16_t ui16DataSize, uint8_t ui8ElementSize){
  if(mpMemoryPool != NULL && ui16DataSize > 0 && vpDataSpace != NULL && bvInUse != NULL && ui8ElementSize > 0){
    mpMemoryPool->bvInUse = bvInUse;
    mpMemoryPool->ui16DataSize = ui16DataSize;
    mpMemoryPool->ui16FreeSpace = ui16DataSize;
    mpMemoryPool->ui16WritePosition = 0;
    mpMemoryPool->vpDataSpace = vpDataSpace;
    mpMemoryPool->ui8ElementSize = ui8ElementSize;
    return MEMORYPOOL_INIT_SUCESS;
  }
  else{
    return MEMORYPOOL_INIT_ERROR;
  }
}

//! Memory Pool getter function: Write ui8Position of last element written
/*!
  This function get a write ui8Position of the last element written.
  \return Return the write ui8Position of the last element written.
*/
uint16_t ui16MPGetWritePosition(mpool_t *mpMemoryPool){
  return mpMemoryPool->ui16WritePosition;
}

//! Memory Pool getter function: Free space
/*!
  This function get the free space of memory pool.
  \return Return the free space of memory pool.
*/
uint16_t ui16MPGetFreeSpace(mpool_t *mpMemoryPool){
  return mpMemoryPool->ui16FreeSpace;
}

//! Memory Pool getter function: Element size
/*!
  This function get the element size of memory pool.
  \return Return the element size of memory pool.
*/
uint8_t ui8MPGetElementSize(mpool_t *mpMemoryPool){
  return mpMemoryPool->ui8ElementSize;
}

//! Memory Pool getter method: Memory pool size
/*!
  This function get the memory pool size.
  \return Return the data size of memory pool.
*/
uint16_t ui16MPGetDataSize(mpool_t *mpMemoryPool){
  return mpMemoryPool->ui16DataSize;
}

//! Memory Pool allocation function
/*!
  Allocate memory space in Memory Pool.
  \param mpMemoryPool is a pointer of mpool_t type. It is the memory space where the control variables of the memory pool will be.
  \param ui16DataSize is a unsigned 16-bit integer. This is the size of space allocation.
  \return Returns the memory allocation address or NULL if the allocation is unsuccessful.
*/
void* vpMPAlloc(mpool_t* mpMemoryPool, uint16_t ui16AllocationSize){
  if (mpMemoryPool->ui16DataSize == 0 || mpMemoryPool->ui16FreeSpace < ui16AllocationSize){
    return NULL;
  }
  uint16_t ui16FreeDataSpace = 0;
  uint8_t ui8_flagFreeSpace = 0;
  while(ui8_flagFreeSpace == 0){
    if (mpMemoryPool->ui16WritePosition == mpMemoryPool->ui16DataSize){
      return NULL;
    }
    mpMemoryPool->ui16WritePosition++;
    if (ui8ReadVectorBit(mpMemoryPool->bvInUse, mpMemoryPool->ui16WritePosition) == 0){
      ui16FreeDataSpace++;
    }
    else{
      ui16FreeDataSpace = 0;
    }
    if (ui16FreeDataSpace == ui16AllocationSize){
      ui8_flagFreeSpace = 1;
    }
  }
  uint16_t ui16StartPosition = mpMemoryPool->ui16WritePosition - ui16AllocationSize;
  uint16_t ui16CounterPositionVector = 0;
  for (ui16CounterPositionVector = ui16StartPosition ; ui16CounterPositionVector < mpMemoryPool->ui16WritePosition ; ui16CounterPositionVector++){
    vSetVectorBit(mpMemoryPool->bvInUse, ui16CounterPositionVector);
  }
  mpMemoryPool->ui16FreeSpace = mpMemoryPool->ui16FreeSpace - ui16FreeDataSpace;
  void *adressAllocation = uiAdressCalculation(mpMemoryPool->vpDataSpace, ui16StartPosition, mpMemoryPool->ui8ElementSize);
  return (void*) adressAllocation;
}

//! Memory Pool deallocation function
/*!
  Deallocate memory space in Memory Pool.
  \param mpMemoryPool is a pointer of mpool_t type. It is the memory space where the control variables of the memory pool will be.
  \param vpAllocatedPointer is a void pointer to pointer parameter. It's the address of the pointer of the allocation. On case deallocation successfully, this function going to set this pointer to NULL.
  \param ui16AllocationSize is a unsigned 16-bit integer. This is the size of space allocation.
*/
void vMPFree(mpool_t* mpMemoryPool, void** vpAllocatedPointer, uint16_t ui16AllocationSize){
  if (vpAllocatedPointer != NULL){
    int32_t i32StartAllocation = ((char*) *vpAllocatedPointer - (char*) mpMemoryPool->vpDataSpace)/mpMemoryPool->ui8ElementSize;
    uint32_t ui32EndAllocation = (uiModule(i32StartAllocation)) + ui16AllocationSize - 1;
    uint32_t ui32Position = 0;
    for (ui32Position = (uiModule(i32StartAllocation)) ; ui32Position <= ui32EndAllocation ; ui32Position++){
      vEraseVectorBit(mpMemoryPool->bvInUse, ui32Position);
    }
    if (mpMemoryPool->ui16WritePosition > i32StartAllocation){
      mpMemoryPool->ui16WritePosition = i32StartAllocation;
    }
    mpMemoryPool->ui16FreeSpace = mpMemoryPool->ui16FreeSpace + ui16AllocationSize;
    *vpAllocatedPointer = NULL;
  }
}

//! Memory Pool fragmentation checker
/*!
  Check that the memory pool is fragmented.
  \param mpMemoryPool is a pointer of mpool_t type. It is the memory space where the control variables of the memory pool will be.
  \return Return FRAGMENTED_MEMORY or UNFRAGMENTED_MEMORY.
*/
uint8_t ui8MPCheckFragmentation(mpool_t *mpMemoryPool){
  uint8_t ui8PreviousMemoryState = 0;
  uint8_t ui8CurrentMemoryState = 0;
  uint16_t ui16Counter = 0;
  for(ui16Counter = 0; ui16Counter < mpMemoryPool->ui16DataSize; ui16Counter++){
    ui8PreviousMemoryState = ui8CurrentMemoryState;
    ui8CurrentMemoryState = ui8ReadVectorBit(mpMemoryPool->bvInUse, ui16Counter);
    if (ui8PreviousMemoryState == 0 && ui8CurrentMemoryState == 1){
      return FRAGMENTED_MEMORY;
    }
  }
  return UNFRAGMENTED_MEMORY;
}

//! Memory Pool fragmented free space checker
/*!
  Check the maximum memory pool allocation.
  \param mpMemoryPool is a pointer of mpool_t type. It is the memory space where the control variables of the memory pool will be.
  \return Return the maximum memory pool allocation.
*/
uint16_t ui16MPGetFragmentedFreeSpace(mpool_t* mpMemoryPool){
  uint8_t ui8PreviousMemoryState = 0;
  uint8_t ui8CurrentMemoryState = 0;
  uint16_t ui16FragmentedFreeSpace = 0;
  uint16_t ui16MaxFragmentedFreeSpace = 0;
  uint16_t ui16Counter = 0;
  for(ui16Counter = 0; ui16Counter < mpMemoryPool->ui16DataSize; ui16Counter++){
    ui8PreviousMemoryState = ui8CurrentMemoryState;
    ui8CurrentMemoryState = ui8ReadVectorBit(mpMemoryPool->bvInUse, ui16Counter);
    if (ui8CurrentMemoryState == 0){
      ui16FragmentedFreeSpace++;
    }
    if ((ui8PreviousMemoryState == 0 && ui8CurrentMemoryState == 1) || (ui16Counter == (mpMemoryPool->ui16DataSize - 1))){
      ui16MaxFragmentedFreeSpace = iBigger(ui16FragmentedFreeSpace, ui16MaxFragmentedFreeSpace);
      ui16FragmentedFreeSpace = 0;
    }
  }
  return ui16MaxFragmentedFreeSpace;
}

//! Memory Pool reallocation function
/*!
  Reallocate memory space in Memory Pool.
  \param mpMemoryPool is a pointer of mpool_t type. It is the memory space where the control variables of the memory pool will be.
  \param vpAllocatedPointer is a void pointer to pointer parameter. It's the address of the pointer of the allocation. On case deallocation successfully, this function going to set this pointer to NULL.
  \param ui16OldAllocationSize is a unsigned 16-bit integer. This is the old size of space allocation.
  \param ui16NewAllocationSize is a unsigned 16-bit integer. This is the new size of space allocation.
  \return Returns the new memory allocation address or NULL if the allocation is unsuccessful.
*/
void* vpMPRealloc(mpool_t* mpMemoryPool, void** vpAllocatedPointer, uint16_t ui16OldAllocationSize, uint16_t ui16NewAllocationSize){
  if (ui16OldAllocationSize != ui16NewAllocationSize){
    void* vpBufferPointer = vpMPAlloc(mpMemoryPool, ui16NewAllocationSize);
    if (vpBufferPointer != NULL){
      memcpy(vpBufferPointer, *vpAllocatedPointer, iSmaller(ui16OldAllocationSize, ui16NewAllocationSize));
      vMPFree(mpMemoryPool, vpAllocatedPointer, ui16OldAllocationSize);
      if (*vpAllocatedPointer != NULL){
        return NULL;
      }
      return vpBufferPointer;
    }
  }
  return NULL;
}
