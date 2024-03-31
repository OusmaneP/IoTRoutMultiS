
module SubZone
{
    parameters:
        double SbWidth @unit(m); // Width of the sub-zone
        double SbHeight @unit(m); // Height of the sub-zone

        //int numHosts;  // number of hosts 

        @display("bgi=background/streetmap,s;bgb=SbWidth,SbHeight");

    submodules:
//        server: Server;
//        host[numHosts]: Host {
//            txRate = txRate;
//            slotTime = slotTime;
//        }
}
