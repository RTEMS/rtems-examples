//
//
//
//

/*! @file LocalMessageQueuePOSIX10031c.h
    @brief LocalMessageQueuePOSIX10031c Specification

    This file contains the implementation of the LocalMessageQueuePOSIX10031c
    template class.
 */

#ifndef __LocalMessageQueuePOSIX10031c_h
#define __LocalMessageQueuePOSIX10031c_h

#include <list>
#include <limits.h>
#include <pthread.h>

namespace OSWrapper {
  /*! @class LocalMessageQueuePOSIX10031c
      @brief POSIX10031c Message Queue Class
     
      Message queues allow application threads and processes to send 
      messages amongst themselves. Threads may block waiting for a 
      message to arrive.

      This is a POSIX10031c implementation of a Message Queue class.  It is
      useful for initial porting efforts and as a template for new 
      implementations.  It is intended for internal consumption by the
      implementation and is not expected to be used by applications.
   */
  template<
    class MessageClass
  >
  class LocalMessageQueuePOSIX10031c {
    public:
      /*! This is the constructor for the POSIX10031c Message Queue
          implementation.

          @param name is the string name of the message queue
          @param depth is the maximum number of pending messages

          @returns a new Message Queue
       */
      LocalMessageQueuePOSIX10031c(
        std::string name,
        size_t      depth = INT_MAX
      )
      {
        int status;

        depthM = depth;
        nameM = name;

        pendingM.clear();
        inactiveM.clear();

        status = pthread_cond_init(&conditionM, NULL);
        if (status) {
          std::cerr << "Local MQ " << nameM << " cond init failed\n";
          throw "Local MQ cond init failed";
        }

        status = pthread_mutex_init( &mutexM, NULL );
        if ( status ) {
          std::cerr << "Local MQ " << nameM << " mutex init failed\n";
          throw "Local MQ mutex init failed";
        }
      }

      /*! This is the destructor for the POSIX10031c Message Queue
           implementation.
       */
      ~LocalMessageQueuePOSIX10031c()
      {
        int status;

        status = pthread_cond_destroy( &conditionM );
        if (status) {
          std::cerr << "Local MQ " << nameM << " cond destroy failed\n";
          throw "Local MQ cond destroy failed";
        }

        status = pthread_mutex_destroy( &mutexM );
        if ( status ) {
          std::cerr << "Local MQ " << nameM << " mutex destroy failed\n";
          throw "Local MQ mutex destroy failed";
        }

      }

      /*! This method allocates a message from the inactive pool.
       */
      MessageClass *Allocate( void )
      {
        MessageClass *m;

        lock();
          if ( inactiveM.empty() ) {
            m = new MessageClass();
          } else {
            inactiveM.pop_front();
          }
        unlock();


// fprintf( stderr, "Allocate %p\n", m );
        return m;
      }

      /*! This method returns a message to the inactive pool.
       */
      void Deallocate( MessageClass *message  )
      {
        lock();
          inactiveM.push_back( message );
        unlock();
      }

      /*! This method sends the specified message to the queue.

          @param message is a pointer to the message buffer.

          @return true if message successfully sent
      */
      bool Send( MessageClass *message )
      {
        if ( !message )
          return false;

        lock();

// fprintf( stderr, "Send %p\n", message );
          if ( pendingM.size() >= depthM ) {
            unlock();
            return false;
          }

          pendingM.push_back( message );
          signal();

        unlock();
        return true;
      }

      /*! This method is invoked to block indefinitely waiting for 
          a message to arrive at this queue.  When a message is available,
          it will be returned to the caller.

          @returns the message received
       */
      MessageClass *Receive( void )
      {
        MessageClass *m;

        lock();
// fprintf( stderr, "Receive start\n" );
          if ( pendingM.size() != 0 ) {
            m = pendingM.front();
          } else {
            do { 
              wait();
              if ( pendingM.size() == 0 )
                continue; 
              m = pendingM.front();
            } while ( !m );
          }
          pendingM.pop_front();
// fprintf( stderr, "Receiving %p\n", m );
        unlock();
        return m;
      }

      /*! This method is invoked to poll for a message pending at the queue.
          If a message is available, it will be returned to the caller.

          @returns the message received or NULL if none available.
       */
      MessageClass *ReceivePoll( void )
      {
        MessageClass *m;

        lock();
          if ( pendingM.size() == 0 ) {
            m = pendingM.front();
            pendingM.pop_front();
          } else
            m = NULL;
        unlock();
        return m;
      }

      /*! This method returns true if a message is available.
       */
      bool IsAvailable( void )
      {
        bool retval;
        lock();
          retval = (pendingM.size() != 0);
        unlock();

        return retval;
      }

      /*! This method returns a pointer to a string indicating the
          implementation of this Message Queue.

          @returns a pointer to a string specifying the flavor
       */
      std::string Flavor(void)
      {
        return "POSIX10031c";
      }

      /*! This method returns a pointer to a string indicating the name
          of this Message Queue.

          @returns the name of this queue
       */
      std::string Name(void)
      {
        return nameM;
      }


      /*! This method returns the number of messages pending on this
          Message Queue.

          @returns the number of pending messages
       */
      unsigned int getNumberPending(void)
      {
        size_t size;

        lock();
          size = pendingM.size();
        unlock();

        return size;
      }

    protected:

      /*!  This method locks the mutex protecting the Message Queue.
       */
      void lock(void)
      {
        int status;
        status = pthread_mutex_lock( &mutexM );
        if ( status ) {
          std::cerr << "Local MQ " << nameM << " mutex lock failed\n";
          throw "Local MQ mutex lock failed";
        }
      }

      /*!  This method unlocks the mutex protecting the Message Queue.
       */
      void unlock(void)
      {
        int status;
        status = pthread_mutex_unlock( &mutexM );
        if ( status ) {
          std::cerr << "Local MQ " << nameM << " mutex unlock failed\n";
          throw "Local MQ mutex lock failed";
        }
      }

      /*!  This method signals the condition variable for the Message Queue.
       */
      void signal(void)
      {
        int status;
        status = pthread_cond_signal( &conditionM );
        if ( status ) {
          std::cerr << "Local MQ " << nameM << " condvar signal failed\n";
          throw "Local MQ condvar signal failed";
        }
      }

      /*!  This method waits for the condition variable for the Message Queue.
       */
      void wait(void)
      {
        int status;
        status = pthread_cond_wait( &conditionM, &mutexM );
        if ( status ) {
          std::cerr << "Local MQ " << nameM << " condvar wait failed\n";
          throw "Local MQ condvar wait failed";
        }
      }

      /*! This variable points to the string name that the user has 
          associated with this message queue.   The name is used by
          clients and servers to locate the Message Queue.
       */
      std::string nameM;

      /*! This variable contains the maximum number of pending messages
          for this Message Queue.
       */
      size_t depthM;

      /*! This variable contains the set of unused messages. 
       */
      std::list<MessageClass *> inactiveM;

      /*! This variable contains the set of pending messages. 
       */
      std::list<MessageClass *> pendingM;

      /*! This variable is the handle for the condition variable used to
       *  signal when messages are available.
       */
      pthread_cond_t  conditionM;

      /*! This variable is the handle for the mutex used to protect the 
       *  Message Queue.
       */
      pthread_mutex_t mutexM;

  };
}

#endif
