#pragma once

#include "BroadcastReceiver.h"
#include "SocketStream.h"
#include "../Thread/Thread.h"

#include <vector>

#include "../glm/glm.hpp"

class TerrainDestructionCannBallScene;
class Entity;

class CannBallNetworkManager : public Thread
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
    CannBallNetworkManager(void);
    ~CannBallNetworkManager(void);

    void EstablishPeerConnection(int playerNum);

    int run();

    void SetPlayerNum(int num) { m_playerNum = num; }
    void SetTerrainDestructionCannBallScene(TerrainDestructionCannBallScene& scene) { m_scene = &scene; }

    bool IsConnected() { return m_connected; }

private:
    void LoadCircle(istream& in, std::string& id);
    void LoadSquare(istream& in, std::string& id);
    void LoadCannon(istream& in, std::string& id);

    void GetPeerUpdates(int playerNum);

private:
    bool m_connected;
    int m_playerNum;
    SocketStream m_player1;
    SocketStream m_player2;
    TerrainDestructionCannBallScene* m_scene;
    Entity* m_followBall;
    std::string m_followBallStr;
    Entity* m_followCam;
};
