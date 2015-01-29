// Class include
#include "CTrace.h"

// System includes
#include <stdio.h>  
#include <stdarg.h>
#ifdef OS_WINDOWS
    #include <thread>
#endif
#ifndef OS_WINDOWS
    #include <pthread.h>
    #include <sys/time.h> 
    #include <unistd.h>
#endif

CTrace* CTrace::mpInstance = 0;

CTrace::CTrace(int level)
: mTraceLevel(level)
, mMutex()
, mThreadIdMap()
, mThreadIdCounter(0)
{
}

CTrace::~CTrace()
{
}

CTrace* CTrace::getInstance()
{
   if(0 == mpInstance)
   {
      mpInstance = new CTrace();
   }
   return mpInstance;
}

void CTrace::traceDebug(const char * format,...)
{
   if(mTraceLevel == TRC_LEVEL_DEBUG)
   {
      mMutex.lock();
      va_list ap;
      
#ifdef OS_WINDOWS
      printf("[DEBUG][%u]: ", getMappedThreadId());
#endif
#ifndef OS_WINDOWS
      printf("\n\033[0;40;1;32m[DEBUG][%u]\033[0m: ", getMappedThreadId());    // Uncomment this to make debug messages highlighted with green
#endif

      va_start(ap,format);
      vprintf(format,ap);
      va_end(ap);
      mMutex.unlock();
   }
}

void CTrace::traceWarning(const char * format,...)
{
   if(mTraceLevel >= TRC_LEVEL_WARNING)
   {
      mMutex.lock();
      va_list ap;
      printf("\033[0;40;1;33m[WARNING]\033[0m: ");
      va_start(ap,format);
      vprintf(format,ap);
      va_end(ap);
      mMutex.unlock();
   }
}

void CTrace::traceError(const char * format,...)
{
   if(mTraceLevel >= TRC_LEVEL_ERROR)
   {
      mMutex.lock();
      va_list ap;
      printf("\033[0;40;1;31m[ERROR]\033[0m: ");
      va_start(ap,format);
      vprintf(format,ap);
      va_end(ap);
      printf("\n");
      mMutex.unlock();
   }
}

void CTrace::setTraceLevel(int level)
{
   if (level < 0 || level > 3)
   {
      return;
   }

   mMutex.lock();
   mTraceLevel = level;
   
   if (level == 0)
   {
      printf("CTrace: Traces are off\n");
   }
   else if(level == 1)
   {
      printf("CTrace: Error message\n");
   }
   else if(level == 2)
   {
      printf("CTrace: Warning message\n");
   }
   else if(level == 3)
   {
      printf("CTrace: Debug message\n");
   }
   mMutex.unlock();
}

int CTrace::getTraceLevel(void)
{
   return mTraceLevel;
}

double CTrace::beginTimeCount()
{
   double timeStamp = 0;

#ifdef OS_LINUX
   struct timeval clockBegin;

   gettimeofday(&clockBegin, NULL);
   timeStamp = clockBegin.tv_sec + clockBegin.tv_usec / 1000000.0; // clocks per second = 1000000.0
#endif

   return timeStamp;
}

long CTrace::endTimeCount(double beginTime, const char * desc)
{
   double timeInterval = 0;

#ifdef OS_LINUX
   struct timeval clockEnd; 
   if (0 != beginTime)
   {
      gettimeofday(&clockEnd, NULL);
      timeInterval = static_cast<double>(clockEnd.tv_sec+clockEnd.tv_usec/1000000.0 - beginTime); 
   }
   else
   {
      printf("CTrace: No begin time\n");
      return 0;
   }

   if (0 != desc)
   {
      printf("CTrace: [%s] executed duration is %f seconds\n", desc,timeInterval);
   }
   else
   {
      printf("CTrace: [%s] executed duration is %f seconds","No label\n", timeInterval);
   }
#endif
   
   return static_cast<long>(timeInterval);
}

CTrace::tByte CTrace::getMappedThreadId()
{
#ifdef OS_WINDOWS
    const std::size_t threadId = std::this_thread::get_id().hash();
#endif
#ifndef OS_WINDOWS
    auto threadId = pthread_self();
#endif 
    if (mThreadIdMap.end() == mThreadIdMap.find(threadId))
    {
        mThreadIdMap[threadId] = mThreadIdCounter++;
    }

    return mThreadIdMap[threadId];
}