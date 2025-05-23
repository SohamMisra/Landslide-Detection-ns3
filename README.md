# Landslide Detection using NS-3

## Project Overview
This project simulates a landslide detection and warning system using the NS-3 network simulator. It models a network of interconnected Wi-Fi nodes in mountainous regions capable of detecting landslide events and transmitting warnings to nearby city nodes.

## Features
- **Simulated Wireless Network:** Models landslide detection and alert transmission.
- **NS-3 Based Simulation:** Utilizes Wi-Fi, CSMA, and point-to-point communication protocols.
- **Dynamic Mobility:** Includes mobility models like the ConstantVelocityMobilityModel to simulate node movement during a landslide.
- **Visualization:** Provides visual feedback with NetAnim and supports packet analysis using Wireshark.

## Technologies Used
- **NS-3:** Network Simulator 3 for modeling network behavior.
- **C++:** Primary programming language.
- **Network Protocols:** Wi-Fi, CSMA, and point-to-point links.
- **Visualization Tools:** NetAnim for animation and Wireshark for packet capture (optional).

## System Requirements

### Hardware
- **Minimum:** 2.0 GHz Dual-Core Processor, 4 GB RAM
- **Recommended:** 3.0 GHz Quad-Core Processor, 8 GB RAM

### Software
- **Operating System:** Ubuntu (or any Linux-based OS)
- **NS-3:** Ensure proper installation of NS-3 and its dependencies.
- **Compilers & Tools:** GCC Compiler, Wireshark (optional), NetAnim

## Network Topology
- **Mountain Nodes (Wi-Fi enabled):** Responsible for detecting landslide events and sending alerts.
- **City Nodes (CSMA-based):** Receive warning messages from mountain nodes.
- **Point-to-Point Link:** Establishes stable connectivity between mountain and city nodes.

## Code Structure
- **Alert Mechanism:** Uses a UDP Echo Server to receive messages on city nodes and a UDP Echo Client on mountain nodes to send alerts.
- **Mobility Simulation:** Implements the ConstantVelocityMobilityModel to simulate landslide-triggered movement.
- **Animation:** Generates output for NetAnim visualization to review the node behavior during the simulation.

## Running the Simulation
1. **Install Dependencies:** Ensure NS-3, GCC, and other necessary packages are installed.
2. **Clone the Repository:**
   ```bash
   git clone repository-url
3. **Build and Run the Simulation:**

   ```bash
   ./waf --run landslide-simulation
Visualize Results: Open the generated NetAnim XML file to visualize the simulation.
