# System Monitor

## Overview

This is a terminal-based C++ application designed to fetch real-time information about the system resources usage (CPU, memory, and processes). The application uses Ncurses library to create a user-friendly textual interface. The application design has been made to utilise modular OOP practices.

![System Monitor](images/monitor.png)

## Architecture

The software structure and the dependencies between the system monitor's components are shown in the diagram below.

![Dependency Diagram](images/dependency-diagram/diagram.drawio.png)

## Instructions

To run the program on a local machine:

1. Clone the project repository

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`

## Yocto Integration

This project includes Yocto Project configurations and a recipe to build a custom Linux image for the Raspberry Pi that integrates the System Monitor application at the system level.

### Building the Custom Image

1. Navigate to the yocto directory and initialize the build environment:

   ```
   cd yocto
   source oe-init-build-env
   ```

2. Ensure the meta-raspberrypi layer is included in your bblayers.conf.

3. To include the system monitor in your image, add the following to `conf/local.conf`:

   ```
   IMAGE_INSTALL:append = " system-monitor"
   ```

4. Specify the target machine and init system by adding the following to `conf/local.conf`:
    ```
    MACHINE = "raspberrypi3"
    INIT_MANAGER = "systemd"
    ```

5. Build the image:

   ```
   bitbake core-image-base
   ```

   *It takes 2-3 hours for the build to finish (on a good computer).*

6. Once the build completes, flash the resulting image (found in `tmp/deploy/images/raspberrypi3/`) to an SD card.

7. Boot the Raspberry Pi with the SD card. The system monitor will be found installed.