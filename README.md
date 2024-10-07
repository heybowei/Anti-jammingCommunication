# Anti-Jamming Communication System

This repository contains the code for the **Anti-jamming Communication System**, which supports both **analog beamforming** and **digital beamforming** techniques. It is designed to provide robust communication by mitigating interference in wireless communication systems. The project is divided into two main components: `gr-mmwave` for digital beamforming and `pycontrol-mmwave` for analog beamforming.

## Table of Contents
- [Installation](#installation)
- [Components](#components)
  - [gr-mmwave (Digital Beamforming)](#gr-mmwave-digital-beamforming)
  - [pycontrol-mmwave (Analog Beamforming)](#pycontrol-mmwave-analog-beamforming)
- [Running the Project](#running-the-project)
  - [gr-mmwave (Digital Beamforming)](#gr-mmwave-running)
  - [pycontrol-mmwave (Analog Beamforming)](#pycontrol-mmwave-running)
- [License](#license)

## Installation

1. **Install GNU Radio**  
   First, you need to install GNU Radio. Follow the instructions provided in the [GNU Radio Installation Guide](https://wiki.gnuradio.org/index.php/InstallingGR).

2. **Clone the Repository**  
   After installing GNU Radio, clone this repository to your local machine:
   ```bash
   git clone https://github.com/your-repo/anti-jamming-communication.git
   cd anti-jamming-communication
3. ### Rebuild the Project

For the digital beamforming component, follow these steps to rebuild the project on your machine:

   ```bash
   cd gr-mmwave
   mkdir build
   cd build
   cmake ..
   make
   sudo make install
   ```

## Components

### gr-mmwave (Digital Beamforming)
The `gr-mmwave` module handles **digital beamforming**. It leverages GNU Radio and USRP to implement digital beamforming techniques for anti-jamming communication.

**Requirements:**
- GNU Radio
- USRP Hardware Driver (UHD)
- Python 3

### pycontrol-mmwave (Analog Beamforming)
The `pycontrol-mmwave` module manages **analog beamforming** through an easy-to-use interface. You can run it either via a GUI or a script for controlling analog beamforming operations.

**Requirements:**
- Python 3.x
- GUI libraries (e.g., PyQt)

## Running the Project

### gr-mmwave (Digital Beamforming) <a name="gr-mmwave-running"></a>
After completing the installation steps, run the digital beamforming project with a USRP device:

```bash
# Ensure your USRP is connected
./path_to_your_grc_script.grc
```

This will initialize the digital beamforming process with the USRP.

pycontrol-mmwave (Analog Beamforming) <a name="pycontrol-mmwave-running"></a>

There are two ways to run the analog beamforming project:

#### Running with GUI

Execute the following command to run the analog beamforming project with a graphical interface:

```bash
./run.sh
```

#### Running with Script

If you prefer to run the project via the command line using Python, run:

```bash
sudo python3 beamlisten.py
```

