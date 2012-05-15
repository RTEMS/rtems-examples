//
//

#ifndef __LED_SERVER_h
#define __LED_SERVER_h

#include "MessageQueueT.h"

#include <unistd.h>
#include <time.h>

#include "../led.h"

namespace Examples {
  class LEDServer {
    private:
      typedef enum {
        StopLED,
        StartLED,
        ShutdownLED,
        SetRate
      } Discriminator_t;

      typedef struct {
        Discriminator_t discriminator;
        union {
          struct {
            unsigned int on_period_in_milliseconds;
            unsigned int off_period_in_milliseconds;
          } Rate;
        } Args;
      } Message_t;

      typedef OSWrapper::MessageQueueTemplate<Message_t> LEDQ_t;

      LEDQ_t *mQueue;
      pthread_t mThread;

      static void *LEDServerThread( void *arg )
      {
        Examples::LEDServer *led = (Examples::LEDServer *)arg;

        return led->ThreadBody();
      }

    public:
      void *ThreadBody(void)
      {
        bool active = false;
        unsigned int ledCount = 0;
        unsigned int onPeriod = 0;
        unsigned int offPeriod = 0;

        Message_t m;
        while ( 1 ) {
          if ( mQueue->IsAvailable() ) {
            mQueue->Read( &m );
            switch ( m.discriminator ) {
              case StartLED:
                std::cerr << "StartLED" << std::endl;
                active = true;
                break;
              case StopLED:
                std::cerr << "StopLED" << std::endl;
                active = false;
                break;
              case SetRate:
                std::cerr << "SetRate: "
                          << "on="   << m.Args.Rate.on_period_in_milliseconds
                          << " off=" << m.Args.Rate.off_period_in_milliseconds
                          << std::endl;
                
                onPeriod  = m.Args.Rate.on_period_in_milliseconds;
                offPeriod = m.Args.Rate.off_period_in_milliseconds;
                break;
              case ShutdownLED:
                std::cerr << "Shutdown of LED Server requested" << std::endl;
                goto thread_exit;
                break;
              default:
                break;
            }
          }
          if ( active ) {
            struct timespec ts;
            unsigned int p;

            if ( (++ledCount % 2) == 1 ) {
              LED_ON();
              p = onPeriod;
            } else {
              LED_OFF();
              p = offPeriod;
            }

            p = (p) ? p : 500;
            ts.tv_sec = (p / 1000);
            ts.tv_nsec = (p * 1000000) % 1000000000;
            nanosleep( &ts, NULL ); 
          } else {
            sleep(1);
          }
        }
        thread_exit:
          return NULL;
      }

    public:
      LEDServer()
      {
        int status;

        mQueue = new LEDQ_t( std::string("LEDQ"), (unsigned int) 10 );
        status =
          pthread_create( &mThread, NULL, LEDServerThread, (void *)this );
        if ( status ) {
          std::cerr << "LEDServer: pthread_create failed "
                    << status << std::endl;
        }
      }

      ~LEDServer()
      {
        Message_t msg;

        msg.discriminator = ShutdownLED;
        mQueue->Write( &msg );
        sleep(1);

        delete mQueue;
      }

      void start( void )
      {
        Message_t msg;

        msg.discriminator = StartLED;
        mQueue->Write( &msg );
      }

      void stop( void )
      {
        Message_t msg;

        msg.discriminator = StopLED;
        mQueue->Write( &msg );
      }

      void setPeriod( unsigned int on, unsigned int off )
      {
        Message_t msg;

        if ( !on || !off ) {
          std::cerr << "setPeriod: 0 period is not allowed" << std::endl;
          return;
        }
        msg.discriminator = SetRate;
        msg.Args.Rate.on_period_in_milliseconds = on;
        msg.Args.Rate.off_period_in_milliseconds = off;
        mQueue->Write( &msg );
      }
  };
}
#endif

