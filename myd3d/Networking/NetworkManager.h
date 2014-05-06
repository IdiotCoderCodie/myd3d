#pragma once
#include <vector>

#include "BroadcastReceiver.h"
#include "SocketStream.h"
#include "../Thread/Thread.h"

#include "../glm/glm.hpp"
#include "../Entities/Entity.h"

#include "../HiResTimer.h"

#include <AntTweakBar.h>

#include <thread>
#include <atomic>
#include <mutex>

class TerrainDestructionScene;

#define NM_MAX_PEERS 5

class NetworkManager : public Thread
{
    struct NetBody
    {
        glm::vec2 position; 
    };

    struct NetCircle : public NetBody
    {
        float radius;
    };

    struct NetSquare : public NetBody
    {
        float size;
    };

public:
    NetworkManager(void);
    ~NetworkManager(void);

    int run();

    void SetTerrainDestructionScene(TerrainDestructionScene& scene) { m_scene = &scene; }

    bool HasFoundOpponent() { return m_foundOpponent; }

    // Returns what number player we are (1 or 2).
    int GetPlayerNum() { return m_playerNum; }

    int GetNumPeers() { return m_numPeers; }

private:
    void EstablishGameConnection();

    void CheckForNewPeer();

    void PackAndSendData();

    void SendUpdateData();

    void SendTransfers();

    void CheckForTransfers();

    void SendInitData(SocketStream& peer);

private:
    bool                        m_foundOpponent;
    BroadcastReceiver           m_receiver;
    SocketStream                m_peers[NM_MAX_PEERS];
    bool                        m_peerIsInit[NM_MAX_PEERS];
    SocketStream                m_opponent;
    int                         m_numPeers;
    int                         m_playerNum;
    TerrainDestructionScene*    m_scene;
    TwBar*                      m_tweakBar;
    float                       m_updateAmount;
    HiResTimer                  m_timer;
    int                         m_targetUps;
    int                         m_actualUps;

    std::string                 m_lostCtrlsToSend;
    std::mutex                  m_lostCtrlsToSendMutex;

    std::atomic<bool>           m_stopTransferThread;
};
