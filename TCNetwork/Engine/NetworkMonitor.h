#ifndef NETWORK_MONITOR_H
#define NETWORK_MONTIOR_H

typedef  bool (*CheckConnectionMethod) (void);

class NNetworkMonitor {
    bool isRunning;
    bool bNetworkOnline;
    int m_FrameTime;
    CheckConnectionMethod checkConnection;
    
public:
    NNetworkMonitor(): isRunning(false), bNetworkOnline(false), m_FrameTime(33), checkConnection(0)
    {}
    
    ~NNetworkMonitor()
    {}
    
    void Run();
    bool Start( CheckConnectionMethod method=0);
    void Stop();
    static NNetworkMonitor * Instance();  // Singleton instance
};

#endif