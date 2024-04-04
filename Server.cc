
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
            txRate = par("txRate");
            iaTime = &par("iaTime");
            pkLenBits = &par("pkLenBits");
            numberOfHosts = par("numHosts");
            aoiWidth = par("SbWidth");
            aoiheight = par("SbHeight");
            sinkX = par("sinkX");
            sinkY = par("sinkY");

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
            sinkName = getName();
            char msgname[] = "sending myID";
            char datas[] = "Ids";
            sprintf(msgname, "sending myID");
            newMessage = generateMessage(msgname,getId(),datas);
            scheduleAt(0, newMessage);
        }

        void Server::handleMessage(cMessage *msg)
        {
            PocMsg *ttmsg = check_and_cast<PocMsg *>(msg);
            if (strcmp("sending myID", ttmsg->getMsgContent()) == 0){
                for(int i=0; i<numberOfHosts; i++) {
                    host = getParentModule()->getSubmodule("host", i);
                    double hostX = host->par("x").doubleValue();
                    double hostY = host->par("y").doubleValue();
                    char msgname[] = "Sending my LeaderID";
                    sprintf(msgname, "Sending my LeaderID");
                    simtime_t duration = pkLenBits->intValue() / txRate;
                    PocMsg *msgToSend;
                    duration = pkLenBits->intValue() / txRate;
                    if (strcmp("sink1", getName()) == 0) {
                        if((0<hostX && hostX<aoiWidth/2) && (0<hostY && hostY<aoiheight/2)) {
                            double dist = std::sqrt((sinkX-hostX) * (sinkX-hostX) + (sinkY-hostY) * (sinkY-hostY));
                            char* datas = (char*)("Sink ID");
                            radioDelay = dist / propagationSpeed;
                            msgToSend = generateMessage(msgname,host->getId(), datas);
                            sendDirect(msgToSend, radioDelay, duration, host->gate("in"));
                        }
                    }
                    if (strcmp("sink2", getName()) == 0) {
                        if((aoiWidth/2<hostX && hostX<aoiWidth) && (0<hostY && hostY<aoiheight/2)) {
                            double dist = std::sqrt((sinkX-hostX) * (sinkX-hostX) + (sinkY-hostY) * (sinkY-hostY));
                            char* datas = (char*)("Sink ID");
                            radioDelay = dist / propagationSpeed;
                            msgToSend = generateMessage(msgname,host->getId(), datas);
                            sendDirect(msgToSend, radioDelay, duration, host->gate("in"));
                        }
                    }
                    if (strcmp("sink3", getName()) == 0) {
                        if((0<hostX && hostX<aoiWidth/2) && (aoiheight/2<hostY && hostY<aoiheight)) {
                            double dist = std::sqrt((sinkX-hostX) * (sinkX-hostX) + (sinkY-hostY) * (sinkY-hostY));
                            char* datas = (char*)("Sink ID");
                            radioDelay = dist / propagationSpeed;
                            msgToSend = generateMessage(msgname,host->getId(), datas);
                            sendDirect(msgToSend, radioDelay, duration, host->gate("in"));
                        }
                    }
                    if (strcmp("sink4", getName()) == 0) {
                        if((aoiWidth/2<hostX && hostX<aoiWidth) && (aoiheight/2<hostY && hostY<aoiheight)) {
                            double dist = std::sqrt((sinkX-hostX) * (sinkX-hostX) + (sinkY-hostY) * (sinkY-hostY));
                            char* datas = (char*)("Sink ID");
                            radioDelay = dist / propagationSpeed;
                            msgToSend = generateMessage(msgname,host->getId(), datas);
                            sendDirect(msgToSend, radioDelay, duration, host->gate("in"));
                        }
                    }
                }
            }

            if (strcmp("sendingId to server", ttmsg->getMsgContent()) == 0){
                if (numberOfMessagesForInitialization <= numberOfHosts) {
                    HostsIdList.push_back(ttmsg->getSource());
                    numberOfMessagesForInitialization = numberOfMessagesForInitialization+1;
                    EV << "Received packet from " << ttmsg->getSource() << "\n";
                }
            }
        }

        PocMsg *Server::generateMessage(char msgname[], int dest, char  items[])
        {
            // Produce source and destination addresses.
            int src = getIndex();  // our module index
            PocMsg *msg = new PocMsg(msgname);
            msg->setSource(src);
            msg->setDestination(dest);
            msg->setMsgContent(msgname);
            msg->setDatas(items);
            return msg;
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
