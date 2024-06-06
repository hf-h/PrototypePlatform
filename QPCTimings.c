#include "Win32Defs.h"

typedef struct QPCTimings {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    LARGE_INTEGER elapsedTimeMicroSeconds;
} QPCTimings;


QPCTimings MkQPCTimings() {
    QPCTimings qpct = {0};
    QueryPerformanceFrequency(&qpct.frequency);
    return qpct;
}

void QPCFrameStart(QPCTimings *qpct) {
    QueryPerformanceCounter(&qpct->start);
}

void QPCFrameEnd(QPCTimings *qpct) {
    QueryPerformanceCounter(&qpct->end);

    qpct->elapsedTimeMicroSeconds.QuadPart = qpct->end.QuadPart - qpct->start.QuadPart;
    qpct->elapsedTimeMicroSeconds.QuadPart *= 1000000;
    qpct->elapsedTimeMicroSeconds.QuadPart /= qpct->frequency.QuadPart;
}
