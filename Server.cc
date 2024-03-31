
#include "Server.h"

namespace pocSimulation {

    Define_Module(Server);

    Server::Server()
        {
            endRxEvent = nullptr;
        }

        Server::~Server()
        {
            cancelAndDelete(endRxEvent);
        }

        void Server::initialize()
        {
            HostsIdList = {};
            numberOfHosts = par("numHosts");
            numberOfMessagesForInitialization = 0;
            channelStateSignal = registerSignal("channelState");
            endRxEvent = new cMessage("end-reception");
            channelBusy = false;
            emit(channelStateSignal, IDLE);

            gate("in")->setDeliverOnReceptionStart(true);

            currentCollisionNumFrames = 0;
            receiveCounter = 0;
            WATCH(currentCollisionNumFrames);

            receiveBeginSignal = registerSignal("receiveBegin");
            receiveSignal = registerSignal("receive");
            collisionSignal = registerSignal("collision");
            collisionLengthSignal = registerSignal("collisionLength");

            emit(receiveSignal, 0L);
            emit(receiveBeginSignal, 0L);

            getDisplayString().setTagArg("p", 0, par("sinkX").doubleValue());
            getDisplayString().setTagArg("p", 1, par("sinkY").doubleValue());
        }

        void Server::handleMessage(cMessage *msg)
        {
            PocMsg *ttmsg = check_and_cast<PocMsg *>(msg);
            if (strcmp("sendingId to server", ttmsg->getMsgContent()) == 0){
                if (numberOfMessagesForInitialization <= numberOfHosts) {
                    HostsIdList.push_back(ttmsg->getSource());
                    numberOfMessagesForInitialization = numberOfMessagesForInitialization+1;
                    EV << "Received packet from " << ttmsg->getSource() << "\n";
                }
            }
        }


        void Server::refreshDisplay() const
        {
            if (!channelBusy) {
                getDisplayString().setTagArg("i2", 0, "status/off");
                getDisplayString().setTagArg("t", 0, "");
            }
            else if (currentCollisionNumFrames == 0) {
                getDisplayString().setTagArg("i2", 0, "status/yellow");
                getDisplayString().setTagArg("t", 0, "RECEIVE");
                getDisplayString().setTagArg("t", 2, "#808000");
            }
            else {
                getDisplayString().setTagArg("i2", 0, "status/red");
                getDisplayString().setTagArg("t", 0, "COLLISION");
                getDisplayString().setTagArg("t", 2, "#800000");
            }
        }

        void Server::finish()
        {
            EV << "duration: " << simTime() << endl;

            recordScalar("duration", simTime());
        }

}; //namespace
