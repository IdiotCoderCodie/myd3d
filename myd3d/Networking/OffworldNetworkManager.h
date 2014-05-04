#pragma once

#include "BroadcastReceiver.h"
#include "SocketStream.h"
#include "../Thread/Thread.h"

#include <vector>

#include "../glm/glm.hpp"

class TerrainDestructionOffworldScene;

class OffworldNetworkManager : public Thread
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
    OffworldNetworkManager(void);
    ~OffworldNetworkManager(void);

    void EstablishPeerConnection();

    int run();

    void SetPlayerNum(int num) { m_playerNum = num; }
    int GetPlayerNum() { return m_playerNum; }
    void SetTerrainDestructionOffworldScene(TerrainDestructionOffworldScene& scene) { m_scene = &scene; }

    std::vector<NetCircle>& GetCircleData() { return m_circles; }

    bool IsConnected() { return m_connected; }

private:
    void LoadCircle(istream& in, std::string& id);
    void LoadSquare(istream& in, std::string& id);

    void GetPeerUpdates();

private:
    bool m_connected;
    int m_playerNum;
    SocketStream m_peer; 
    TerrainDestructionOffworldScene* m_scene;
    std::vector<NetCircle> m_circles;
};
