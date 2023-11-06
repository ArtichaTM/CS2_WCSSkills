
#ifndef WCSS_TEST_LEAKS
#define WCSS_TEST_LEAKS
#include "gtest/gtest.h"
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <crtdbg.h>
#endif // Windows check

class MemoryLeakDetector {
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
public:
    MemoryLeakDetector() {
        _CrtMemCheckpoint(&memState_);
    }

    ~MemoryLeakDetector() {
        _CrtMemState stateNow, stateDiff;
        _CrtMemCheckpoint(&stateNow);
        int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);
        if (diffResult)
            reportFailure(stateDiff.lSizes[1]);
    }
private:
    void reportFailure(unsigned int unfreedBytes) {
        FAIL() << "Memory leak of " << unfreedBytes << " byte(s) detected.";
    }
    _CrtMemState memState_;
#endif // Windows check
};
#endif // TEST_LEAKS