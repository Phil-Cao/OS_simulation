#ifndef PROCESSMEMORYINFO_H
#define PROCESSMEMORYINFO_H


class ProcessMemoryInfo
{
public:
//    ProcessMemoryInfo();
    ProcessMemoryInfo(int start, int length, int end, int PID);
    ProcessMemoryInfo(ProcessMemoryInfo *p);
    int start;
    int length;
    int end;
    int PID;

};

#endif // PROCESSMEMORYINFO_H
