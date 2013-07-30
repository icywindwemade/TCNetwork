#include "NetworkMonitor.h"
#include "NetworkManager.h"
#include "OnlineHandler.h"



#define CONNECT_CHECKUP_CYCLES  30   // cycles in the loop until next check

//==============================================================================

pthread_t MonThread = 0;
pthread_mutex_t MonLock;
pthread_cond_t MonCond = PTHREAD_COND_INITIALIZER;


void* MonitorThreadProc( void* InParam )
{
	( (NNetworkMonitor*)InParam )->Run();
    
#ifdef NET_DEVICE_WIN
	return 0;
#else //  NET_DEVICE_IOS
	printf("pthread_exit()");
	pthread_exit((void *)NULL);
#endif // NET_DEVICE_WIN
    
}

NNetworkMonitor * NNetworkMonitor::Instance() {
    static NNetworkMonitor monitor;
    return &monitor;
}

void NNetworkMonitor::Run() {
    useconds_t nsinv = (int)1000000*(1.0/m_FrameTime); // nanosecond sleep
    static int checkCycles = 0;
    
    while (isRunning) {
        usleep(nsinv);
        NNetworkManager::Instance()->Update( m_FrameTime );
        
        if( GOnlineInfo->IsLoaded() && checkCycles >= CONNECT_CHECKUP_CYCLES ) {
            if (checkConnection) {
                bool bConnected = checkConnection();
                if( bNetworkOnline ^ bConnected ) {  // XOR for state changed
                    if( OLErrorHandleCB != NULL ) {
                        if ( bConnected ) {
                            OLErrorHandleCB( ENEC_StateOnline, " connection is established.");
                        } else {
                            OLErrorHandleCB( ENEC_StateOffline, " connection is not present.");
                        }
                    }
                    
                    bNetworkOnline = bConnected;
                }
            }
            checkCycles = 0;
        }
        checkCycles++;
    }
    printf("Montior stopped running\n");
}

bool NNetworkMonitor::Start( CheckConnectionMethod method ) {
   	int Result = pthread_create( &MonThread, 0, MonitorThreadProc, this );
	if( Result != 0 )
    {
        isRunning = false;
    }

    checkConnection = method;
    isRunning = true;
    return isRunning;
}

void NNetworkMonitor::Stop() {
    isRunning = false;
}