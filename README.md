Landslide Detection using NS-3
Project Overview
This project simulates a landslide detection and warning system using the NS-3 network simulator. It models a network of interconnected Wi-Fi nodes in mountainous regions capable of detecting landslide events and transmitting warnings to nearby city nodes.

Features
Simulated wireless network for landslide detection and alert transmission

Utilizes NS-3 to model Wi-Fi, CSMA, and point-to-point communication

Includes mobility models to simulate node movement during a landslide

Demonstrates early disaster warning mechanisms through network simulation

Technologies Used
NS-3 (Network Simulator 3)

C++

Wi-Fi, CSMA, and point-to-point network protocols

Mobility models for node movement emulation

NetAnim for visualization

System Requirements
Hardware:
Minimum: 2.0 GHz Dual-Core Processor, 4GB RAM

Recommended: 3.0 GHz Quad-Core Processor, 8GB RAM

Software:
Ubuntu (Linux-based OS)

NS-3

Wireshark (optional for network packet analysis)

NetAnim (for visualization)

GCC Compiler

Network Topology
Mountain Nodes (Wi-Fi enabled): Detect landslides and send alerts

City Nodes (CSMA-based network): Receive warning messages

Point-to-Point Link: Facilitates stable communication between mountain and city nodes

Code Structure
Implements UDP Echo Server for city node alert reception

UDP Echo Client on mountain nodes sends warning messages

Uses ConstantVelocityMobilityModel to simulate landslide movement

Animation setup for NetAnim visualization

Running the Simulation
Install NS-3 and required dependencies.

Clone the repository:

bash
git clone <repository-url>
Compile the code using NS-3:

bash
./waf --run landslide-simulation
Open the NetAnim XML output file to visualize results.

Results
Landslide detection triggers warning alerts to city nodes.

Mountain nodes move to simulate landslide effects.

Alerts received successfully by city nodes.

Network activity can be inspected using Wireshark logs.

Future Enhancements
Machine learning-based landslide prediction models

Energy-efficient sensor nodes for remote deployment

Mobile app integration for real-time alerts

Improved communication protocols for reliability
