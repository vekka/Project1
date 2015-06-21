#include "assert.hpp"


Assert *Assert::m_assert = 0;

Assert::Assert()
{
   m_processing = false;
   m_ignoreAll = false;
}

Assert::~Assert()
{
}

void Assert::Create( Assert* newAssertClass )
{
   if (!m_assert)
      m_assert = newAssertClass ? newAssertClass : new Assert;
}

void Assert::Destroy()
{
   if (m_assert)
      delete m_assert;
   m_assert = NULL;
}

bool Assert::Process(eType type,
                             const char  *filename,
                             uint32          lineNumber,
                             const char  *message)
{
    // If we're somehow recursing, just die.
    //if(m_processing)
    //    __asm { int 3 };

    //m_processing = true;
    //bool ret = false;

    // always dump to the Assert to the Console
//    if (Con::isActive())
//    {
//        if (assertType == Warning)
//            Con::warnf(ConsoleLogEntry::Assert, "%s(%ld,0): {%s} - %s", filename, lineNumber, typeName[assertType], message);
//        else
//            Con::errorf(ConsoleLogEntry::Assert, "%s(%ld,0): {%s} - %s", filename, lineNumber, typeName[assertType], message);
//    }
//
//    // if not a WARNING pop-up a dialog box
//    if (assertType != Warning)
//    {
//        // used for processing navGraphs (an assert won't botch the whole build)
//        if(Con::getBoolVariable("$FP::DisableAsserts", false) == true)
//            Platform::forceShutdown(1);
//
//        char buffer[2048];
//        dSprintf(buffer, 2048, "%s: (%s @ %ld)", typeName[assertType], filename, lineNumber);
//        if( !ignoreAll )
//        {
//            // Display message box with Debug, Ignore, Ignore All, and Exit options
//            switch( Platform::AlertAssert(buffer, message) )
//            {
//                case Platform::ALERT_ASSERT_DEBUG:
//                    ret = true;
//                    break;
//                case Platform::ALERT_ASSERT_IGNORE:
//                    ret = false;
//                    break;
//                case Platform::ALERT_ASSERT_IGNORE_ALL:
//                    ignoreAll = true;
//                    ret = false;
//                    break;
//                default:
//                case Platform::ALERT_ASSERT_EXIT:
//                    Platform::forceShutdown(1);
//                    break;
//            }
//        }
//    }

    //m_processing = false;
   return true;
    //return ret;
}

bool Assert::DisplayMsgBox(const char *title, const char *message, bool retry)
{
   //if (m_retry)
      //return Platform::AlertRetry(title, message);

   //Platform::AlertOK(title, message);
   return false;
}


