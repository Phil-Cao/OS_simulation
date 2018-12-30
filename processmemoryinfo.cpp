#include "processmemoryinfo.h"

//ProcessMemoryInfo::ProcessMemoryInfo(QObject *parent) : QObject(parent)
//{

//}

ProcessMemoryInfo::ProcessMemoryInfo(int start, int length, int end, int PID)
{
    this->start = start;
    this->length = length;
    this->end = end;
    this->PID = PID;
}

ProcessMemoryInfo::ProcessMemoryInfo(ProcessMemoryInfo *p)
{
    this->start = p->start;
    this->length = p->length;
    this->end = p->end;
    this->PID = p->PID;
}
