//
//  MessageQueue Template Base
//
//

#ifndef __MessageQueueT_OE3_h
#define __MessageQueueT_OE3_h

#include <iostream>
#include <string>

namespace OSWrapper {

  template<
    typename QueueType
  >
  class MessageQueueTemplate {
    public:

    typedef void (*Converter_t)( const QueueType *src, QueueType *dest );

    private:

    std::string                 mName;
    Converter_t                 mHTON;
    Converter_t                 mNTOH;
    void                       *mQueue;

    public:

    MessageQueueTemplate::MessageQueueTemplate(
      const char      *name,
      unsigned int     depth,
      Converter_t      hton_f,
      Converter_t      ntoh_f
    )
    {
      mName = name;
      this->mHTON = hton_f;
      this->mNTOH = ntoh_f;

      // create the message queue
      mQueue = NULL;
    }

    MessageQueueTemplate::~MessageQueueTemplate()
    {
      // destroy the message queue object
      // delete mQueue;
      // destroy anything else
    }

    bool MessageQueueTemplate::Read(
      QueueType *buffer
    )
    {
      int       status = 0;
      QueueType tbuffer; 

      // Get the message
      try {
        if ( mNTOH ) {
          status = 1;  // read into tbuffer
        } else {
          status = 1;  // read into buffer
        }
 
        // read the message queue
        if ( status ) {
          std::cerr << mName << " Queue not Read: " << status << std::endl;
          return false;
        }
      } catch (...) {
        std::cerr << mName << " Queue not Read: Exception" << std::endl;
        return false;
      }

      // convert from network neutral
      if ( mNTOH )
        (*mNTOH)( &tbuffer, buffer );

      return true;
    }

    bool MessageQueueTemplate::Write(
      QueueType *buffer
    )
    {
      int status = 0;
      QueueType tbuffer; 
      QueueType *b = buffer; 

      // convert to network neutral
      if ( mHTON ) {
        (*mHTON)( buffer, &tbuffer );
        b = &tbuffer;
      }
        

      // write the message
      status = 1; // write( b );
      if ( status ) {
        std::cerr << mName << " Queue not Written: " << status << std::endl;
        return false;
      }
      return true;
    }
  };

};


#endif
