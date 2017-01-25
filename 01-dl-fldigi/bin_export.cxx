#include <bin_export.h>
#include <fl_digi.h>
#include <cstdio>
#include <string>
#ifndef __MINGW32__
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

static void* exporterThread(void* data) {
    BinExporter* exporter = reinterpret_cast<BinExporter*>(data);
    
    while(!exporter->quit) {
        pthread_cond_wait(&exporter->ipcCond, &exporter->ipcMutex);
        while(exporter->ipcBuffer.size() > 0 && !exporter->quit) {
            uint8_t byte = exporter->ipcBuffer.pop();
            printf("byte: 0x%02x\n", byte);
            exporter->logByteSync(byte);
        }
    }
    return NULL;
}

// TODO: Print serial port in GUI

BinExporter::BinExporter() {
    isOpen = false;
    quit = false;
    serialFile = posix_openpt(O_RDWR);
    if(serialFile < 0) {
        fprintf(stderr, "Binary Exporter: unable to open serial connection\n\n");
        return;
    }
    
    grantpt(serialFile);
    unlockpt(serialFile);
    serialAddr = std::string("Output: ") + ptsname(serialFile);
    
    printf("%s\n", serialAddr.c_str());
    
    // configure the serial writer
    struct termios options;
    tcgetattr(serialFile, &options);
    options.c_cflag = B9600 | CS8 | CSTOPB | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(serialFile, TCIFLUSH);
    tcsetattr(serialFile, TCSANOW, &options);
    
    isOpen = true;
    
    // Create the background thread
    ipcCond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    ipcMutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    pthread_create(&ipcThread, NULL, exporterThread, reinterpret_cast<void*>(this));
}

BinExporter::~BinExporter() {
    quit = true;
    pthread_join(ipcThread, NULL);
    if(!isOpen) { return; }
    close(serialFile);
}

void BinExporter::logByte(uint8_t byte) {
    ipcBuffer.push(byte);
    pthread_cond_signal(&ipcCond);
    put_status(serialAddr.c_str(), 20);
}

void BinExporter::logByteSync(uint8_t byte) {
    if(!isOpen) { return; }
    write(serialFile, &byte, 1);
}
