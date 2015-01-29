#ifndef CTrace_HPP
#define CTrace_HPP

#define TRC_LEVEL_NONE        0
#define TRC_LEVEL_ERROR       1
#define TRC_LEVEL_WARNING     2
#define TRC_LEVEL_DEBUG       3

#define TRC_DEBUG       CTrace::getInstance()->traceDebug
#define TRC_WARNING     CTrace::getInstance()->traceWarning
#define TRC_ERROR       CTrace::getInstance()->traceError

#define TRC_LEVEL_GET         CTrace::getInstance()->setTraceLevel()
#define TRC_LEVEL_SET(level)  CTrace::getInstance()->getTraceLevel(level)

#define BEGIN_COUNT                 CTrace::getInstance()->beginTimeCount()
#define END_COUNT(beginTime, desc)  CTrace::getInstance()->endTimeCount(beginTime, desc)

#include <mutex>
#include <map>

#ifndef OS_WINDOWS
    #include <pthread.h>
#endif

class CTrace
{
public:
   virtual ~CTrace();

   void traceDebug(const char* format,...);
   void traceWarning(const char* format,...);
   void traceError(const char* format,...);
   
   double beginTimeCount();
   long endTimeCount(double beginTime, const char* desc = 0);

   void setTraceLevel(int level);
   int getTraceLevel(void);

   static CTrace* getInstance();

private:
   typedef unsigned char tByte;

   /**
    * @brief Constructor
    * @param level Desired level of traces. For different variats please refer to defines at the top of this hpp file
    */ 
   CTrace(int level = TRC_LEVEL_DEBUG);

   /**
    * @brief Gets Current thread id and looks to map in order to find mapped id value
    *        This value is displayed in traces as 'thread' id
    *        Motivation is real thread ids are very long and it is not convinient to
    *        debug program using them
    */
   tByte getMappedThreadId();

   #ifdef OS_WINDOWS
      typedef std::map<std::size_t, tByte> tThreadIdMap;
   #endif
   #ifndef OS_WINDOWS
      typedef std::map<pthread_t, tByte> tThreadIdMap;
   #endif
   
   static CTrace* mpInstance;
   int mTraceLevel;
   std::mutex mMutex;
   tThreadIdMap mThreadIdMap;
   tByte        mThreadIdCounter;
};

#endif // CTrace_HPP