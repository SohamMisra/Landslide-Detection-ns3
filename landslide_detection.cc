 // ns-3 modules for network configuration and utilities
 #include "ns3/applications-module.h"
 #include "ns3/core-module.h"
 #include "ns3/csma-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/mobility-module.h"
 #include "ns3/network-module.h"
 #include "ns3/point-to-point-module.h"
 #include "ns3/ssid.h"
 #include "ns3/yans-wifi-helper.h"
 #include "ns3/netanim-module.h"
 using namespace ns3;
 // Define the log component for simulation
 NS_LOG_COMPONENT_DEFINE("LandslideDisasterSimulation");
 /**
 * Triggers the landslide by setting velocities for nodes in the "mountain" region, simulating a
 downward movement.
 * This function takes a set of mountain nodes, start time, and duration of the landslide.
 * After the landslide, nodes are disposed if they cross a certain boundary.
 */
 void TriggerLandslide(NodeContainer& mountainNodes, double startTime, double duration,
 Ipv4InterfaceContainer& cityInterfaces) {
 for (uint32_t i = 0; i < mountainNodes.GetN(); ++i) {
 // Get mobility model for each node
 Ptr<ConstantVelocityMobilityModel> mobility =
 mountainNodes.Get(i)->GetObject<ConstantVelocityMobilityModel>();
 if (mobility == nullptr) {
 NS_LOG_ERROR("Node " << i << " does not have a ConstantVelocityMobilityModel.");
 continue;
 }
 // Define starting and ending positions to calculate landslide velocity
 Vector startPos = mobility->GetPosition();
 Vector endPos = Vector(startPos.x, 20.0, 0.0);
 double velocityY = (endPos.y- startPos.y) / duration;
 Vector velocity = Vector(0.0, velocityY, 0.0);
 // Schedule landslide movement
 Simulator::Schedule(Seconds(startTime), &ConstantVelocityMobilityModel::SetVelocity,
 mobility, velocity);
 // Schedule node disposal when it reaches the boundary
 Simulator::Schedule(Seconds(startTime + duration), [mobility, i]() {
 Vector pos = mobility->GetPosition();
 if (pos.y >= 100.0) {
 NS_LOG_INFO("Destroying node " << i << " as it reached the boundary.");
 mobility->GetObject<Node>()->Dispose();
 }
 });
 }
 }
 int main(int argc, char* argv[]) {
 // Simulation parameters
 bool verbose = true;
 uint32_t nWifi = 4;
 uint32_t nMountain = 3;
 bool tracing = true;
 // Command-line arguments
 CommandLine cmd(__FILE__);
 cmd.AddValue("verbose", "Log echo applications if true", verbose);
 cmd.AddValue("tracing", "Enable pcap tracing", tracing);
 cmd.Parse(argc, argv);
 if (verbose) {
 // Enable logging for the client-server application
 LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
 LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
 }
 // Create nodes for a point-to-point connection between mountain and city regions
 NodeContainer p2pNodes;
 p2pNodes.Create(2);
 // Configure point-to-point connection
 PointToPointHelper pointToPoint;
 pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
 pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
 // Install devices on point-to-point connection
 NetDeviceContainer p2pDevices = pointToPoint.Install(p2pNodes);
 // Set up city CSMA network and connect it to point-to-point node
 NodeContainer cityCsmaNodes;
 cityCsmaNodes.Add(p2pNodes.Get(0));
 cityCsmaNodes.Create(nWifi);
 CsmaHelper cityCsma;
 cityCsma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
 cityCsma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));
 NetDeviceContainer cityCsmaDevices = cityCsma.Install(cityCsmaNodes);
 // Create nodes for the mountain region Wi-Fi network
 NodeContainer mountainNodes;
 mountainNodes.Add(p2pNodes.Get(1));
 mountainNodes.Create(nMountain);
 // Configure Wi-Fi settings for mountain network
 YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
 YansWifiPhyHelper phy;
 phy.SetChannel(channel.Create());
 WifiMacHelper mac;
 Ssid ssid = Ssid("ns-3-ssid");
 WifiHelper wifi;
 // Install devices and configure station and AP mode
 mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing",
 BooleanValue(false));
 NetDeviceContainer mountainDevices = wifi.Install(phy, mac, mountainNodes);
 mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
 NetDeviceContainer mountainApDevices = wifi.Install(phy, mac, p2pNodes.Get(1));
 // Configure mobility for mountain region
 MobilityHelper mountainMobility;
 mountainMobility.SetPositionAllocator("ns3::GridPositionAllocator",
 "MinX", DoubleValue(0.0),
 "MinY", DoubleValue(10.0),
 "DeltaX", DoubleValue(20.0),
 "DeltaY", DoubleValue(10.0),
 "GridWidth", UintegerValue(4),
 "LayoutType", StringValue("RowFirst"));
 mountainMobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
 mountainMobility.Install(mountainNodes);
 // Set up city mobility
 MobilityHelper cityMobility;
 cityMobility.SetPositionAllocator("ns3::GridPositionAllocator",
 "MinX", DoubleValue(10.0),
 "MinY", DoubleValue(59.0),
 "DeltaX", DoubleValue(10.0),
    "DeltaY", DoubleValue(10.0),
 "GridWidth", UintegerValue(5),
 "LayoutType", StringValue("RowFirst"));
 cityMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
 cityMobility.Install(cityCsmaNodes);
 // Install IP stack on all nodes
 InternetStackHelper stack;
 stack.Install(mountainNodes);
 stack.Install(p2pNodes.Get(1));
 stack.Install(cityCsmaNodes);
 // Assign IP addresses to the network interfaces
 Ipv4AddressHelper address;
 address.SetBase("10.1.1.0", "255.255.255.0");
 Ipv4InterfaceContainer mountainInterfaces = address.Assign(mountainDevices);
 address.Assign(mountainApDevices);
 address.SetBase("10.1.2.0", "255.255.255.0");
 Ipv4InterfaceContainer cityCsmaInterfaces = address.Assign(cityCsmaDevices);
 address.SetBase("10.1.3.0", "255.255.255.0");
 address.Assign(p2pDevices);
 // Set up UDP echo server in the city network
 uint16_t warningPort = 9;
 UdpEchoServerHelper warningServer(warningPort);
 ApplicationContainer serverApps = warningServer.Install(cityCsmaNodes.Get(nWifi- 1));
 serverApps.Start(Seconds(1.0));
 serverApps.Stop(Seconds(10.0));
 // Configure UDP client in mountain network to send warnings
 UdpEchoClientHelper warningClient(cityCsmaInterfaces.GetAddress(nWifi- 1), warningPort);
 warningClient.SetAttribute("MaxPackets", UintegerValue(15));
 warningClient.SetAttribute("Interval", TimeValue(Seconds(0.1)));
 warningClient.SetAttribute("PacketSize", UintegerValue(1024));
 ApplicationContainer clientApps = warningClient.Install(mountainNodes.Get(nMountain- 1));
 clientApps.Start(Seconds(2.0));
 clientApps.Stop(Seconds(5.0));
 // Trigger landslide event
 double landslideStartTime = 5.0;
 double landslideDuration = 3.0;
 TriggerLandslide(mountainNodes, landslideStartTime, landslideDuration, cityCsmaInterfaces);
 // Enable packet tracing and animation
 if (tracing) {
 phy.EnablePcap("landslide-sim", mountainDevices);
 phy.EnablePcap("landslide-sim", cityCsmaDevices);
}
 Landslide Detection
 AnimationInterface anim("landslide-simulation16.xml");
 // Run simulation
 Simulator::Stop(Seconds(15.0));
 Simulator::Run();
 Simulator::Destroy();
 return 0;
 }
