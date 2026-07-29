/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * MessageQueue Template
 */

/*
 * COPYRIGHT (c) 2026.
 * On-Line Applications Research Corporation (OAR).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __MessageQueueT_OE3_h
#define __MessageQueueT_OE3_h

#include <iostream>
#include <string>
#include <cstring>

#include "LocalMessageQueuePOSIX10031c.h"
namespace OSWrapper {

  template<
    typename QueueType
  >
  class MessageQueueTemplate {
    public:

    typedef void (*Converter_t)( const QueueType *src, QueueType *dest );

    private:

    typedef OSWrapper::LocalMessageQueuePOSIX10031c<QueueType> Queue_t;

    std::string    mName;
    Converter_t    mHTON;
    Converter_t    mNTOH;
    Queue_t       *mQueue;

    public:

    MessageQueueTemplate(
      std::string      name,
      unsigned int     depth,
      Converter_t      hton_f = NULL,
      Converter_t      ntoh_f = NULL
    )
    {
      mName = name;
      this->mHTON = hton_f;
      this->mNTOH = ntoh_f;

      // create the message queue
      mQueue = new Queue_t( name, depth );
    }

    MessageQueueTemplate()
    {
      // destroy the message queue object
      delete mQueue;

      // destroy anything else
    }

    bool IsAvailable(void)
    {
      return mQueue->IsAvailable();
    }

    bool Read(
      QueueType *buffer
    )
    {
      QueueType *b;

      // MessageClass *Receive( void )
      // Get the message
      try {

        b = mQueue->Receive();
        // read the message queue
        if ( !b ) {
          std::cerr << mName << " Queue not Read" << std::endl;
          return false;
        }
      } catch (...) {
        std::cerr << mName << " Queue not Read: Exception" << std::endl;
        return false;
      }

      // convert from network neutral
      if ( mNTOH )
        (*mNTOH)( b, buffer );
      else
        memcpy( buffer, b, sizeof(QueueType) );

      mQueue->Deallocate( b );

      return true;
    }

    bool Write(
      QueueType *buffer
    )
    {
      bool status;
      QueueType *b = mQueue->Allocate();
      
      // convert to network neutral
      if ( mHTON ) {
        (*mHTON)( buffer, b );
      } else {
        memcpy( b, buffer, sizeof(QueueType) );
      }

      // write the message
      status = mQueue->Send( b );
      if ( !status ) {
        std::cerr << mName << " Queue not Written " << std::endl;
        return false;
      }
      return true;
    }
  };

};


#endif
