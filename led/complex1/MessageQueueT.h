//
//  MessageQueue Template
//
//

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
