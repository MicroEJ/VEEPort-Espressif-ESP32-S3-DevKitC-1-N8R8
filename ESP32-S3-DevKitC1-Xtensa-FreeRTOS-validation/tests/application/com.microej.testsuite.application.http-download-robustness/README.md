# Overview

This project contains the launcher for the HTTP Download Robustness demonstration.

# Usage

Import this project and use the `HTTP Download Robustness [EMB]` or `HTTP Download Robustness [SIM]` Launcher.

# Requirements

This example has been tested on:

* MicroEJ SDK 20.07
* With a platform that contains:
    * EDC-1.3.0
    * BON-1.4.0
    * NET-1.1.1
    * ECOM-MOBILE-0.2.0
    * ECOM-NETWORK-2.1.0
    * ECOM-WIFI-2.2.1

# Dependencies

 _All dependencies are retrieved transitively by Ivy resolver_.

## Getting ready with the application configuration

The class `[workspace.dir]/com.microej.testsuite.application.http-download-robustness/src/main/java/net/http/download/Config.java` automatically retrieves the config from the properties defined in `[workspace.dir]/com.microej.testsuite.application.http-download-robustness/src/main/resources/net/http/download/standalone.properties.list`. Do NOT modify `[workspace.dir]/com.microej.testsuite.application.http-download-robustness/src/main/java/net/http/download/Config.java`. 

### Configuring the mobile connection

- to configure the mobile connection, use the properties in `[workspace.dir]/com.microej.testsuite.application.http-download-robustness/src/main/resources/net/http/download/standalone.properties.list`

```
# Access Point Name
mobile.apn=net

# User credential for APN connection
mobile.user=

# Passphrase credential for APN connection
mobile.passphrase=

# PIN code to unlock the SIM card
mobile.pin.code=
```

- `mobile.apn`, `mobile.user`, `mobile.passphrase` and `mobile.pin.code` are related to the network provider and SIM card used.

### Configuring the wi-fi connection

- to configure the wi-fi connection, use the properties in `[workspace.dir]/com.microej.testsuite.application.http-download-robustness/src/main/resources/net/http/download/standalone.properties.list`

```
# Wi-Fi SSID
wifi.ssid=TEST_SSID

# Passphrase credential for Wi-Fi connection
wifi.passphrase=TEST_PASSPHRASE

# Security mode for Wi-Fi connection (MIXED (WPA1 / WPA2)=6; OPEN=7; WPA1=9; WPA2=10; WPA3=11; MIXED (WPA2 / WPA3)=12)
wifi.security=10
```

- `wifi.ssid`, `wifi.passphrase` and `wifi.security` are related to the network provider.

#### Simulation

On simulation the properties `mobile.interface.name`, `eth.interface.name` or `wifi.interface.name` in `[workspace.dir]/com.microej.testsuite.application.http-download-robustness/src/main/resources/net/http/download/standalone.properties.list` should be changed to an Internet-connected interface.

For simulating a Mobile network interface, properties located in `[workspace.dir]/com.microej.testsuite.application.http-download-robustness/src/main/resources/net/http/download/simulation.properties` should be set to simulate the information related to the SIM card and mobile network.

For simulating a Wi-Fi network interface, properties located in `[workspace.dir]/com.microej.testsuite.application.http-download-robustness/wifi/wificonfig.properties` should be set to simulate the information related to the Wi-Fi network.

### Selecting the network interface

To switch between the Mobile, Ethernet and Wi-Fi connections we use the below properties in `[workspace.dir]/com.microej.testsuite.application.http-download-robustness/src/main/resources/net/http/download/standalone.properties.list`
- `net.http.download.IMobileNetworkManager=net.http.download.MobileNetworkManager` to use the Mobile connection
- `net.http.download.IEthernetNetworkManager=net.http.download.EthernetNetworkManager` to use the Ethernet connection
- `net.http.download.IWifiNetworkManager=net.http.download.WifiNetworkManager` to use the Wi-Fi connection

Additionally, embed the required type in `[workspace.dir]/com.microej.testsuite.application.http-download-robustness/src/main/resources/net/http/download/net.http.download.types.list`
- `net.http.download.MobileNetworkManager` to embed the Mobile network manager
- `net.http.download.EthernetNetworkManager` to embed the Ethernet network manager
- `net.http.download.WifiNetworkManager` to embed the Wi-Fi network manager

Note that multiple interfaces can be used at a time.

## Launching the demo on a board
- Launch the Run Configuration `HTTP Download Robustness [EMB]` to generate the binary `microejapp.o`
- Use the generated binary to flash your board and use a serial console of your choice to see the traces of the running application

The traces should look like this :

```
net.http.download.downloadthread INFO: [Thread 0] Trying to dowload file (iteration = 0) ...
net.http.download.downloadthread INFO: [Thread 2] Trying to dowload file (iteration = 0) ...
net.http.download.downloadthread INFO: [Thread 3] Trying to dowload file (iteration = 0) ...
net.http.download.downloadthread INFO: [Thread 4] Trying to dowload file (iteration = 0) ...
net.http.download.downloadthread INFO: [Thread 5] Trying to dowload file (iteration = 0) ...
net.http.download.downloadthread INFO: [Thread 6] Trying to dowload file (iteration = 0) ...
net.http.download.httpdownloadutility INFO: [Thread 6] Content-Type = image/png
net.http.download.httpdownloadutility INFO: [Thread 6] Content-Disposition = null
net.http.download.httpdownloadutility INFO: [Thread 6] Content-Length = 227288
net.http.download.httpdownloadutility INFO: [Thread 6] fileName = HP_picture_whitepaper-05.png
net.http.download.httpdownloadutility INFO: [Thread 0] Content-Type = image/png
net.http.download.httpdownloadutility INFO: [Thread 0] Content-Disposition = null
net.http.download.httpdownloadutility INFO: [Thread 0] Content-Length = 227288
net.http.download.httpdownloadutility INFO: [Thread 0] fileName = HP_picture_whitepaper-05.png
net.http.download.httpdownloadutility INFO: [Thread 4] Content-Type = image/png
net.http.download.httpdownloadutility INFO: [Thread 4] Content-Disposition = null
net.http.download.httpdownloadutility INFO: [Thread 4] Content-Length = 227288
net.http.download.httpdownloadutility INFO: [Thread 4] fileName = HP_picture_whitepaper-05.png
net.http.download.httpdownloadutility INFO: [Thread 5] Content-Type = image/png
net.http.download.httpdownloadutility INFO: [Thread 5] Content-Disposition = null
net.http.download.httpdownloadutility INFO: [Thread 5] Content-Length = 227288
net.http.download.httpdownloadutility INFO: [Thread 5] fileName = HP_picture_whitepaper-05.png
net.http.download.httpdownloadutility INFO: [Thread 6] Progress = 10%
net.http.download.httpdownloadutility INFO: [Thread 2] Content-Type = image/png
net.http.download.httpdownloadutility INFO: [Thread 2] Content-Disposition = null
net.http.download.httpdownloadutility INFO: [Thread 2] Content-Length = 227288
net.http.download.httpdownloadutility INFO: [Thread 2] fileName = HP_picture_whitepaper-05.png
net.http.download.httpdownloadutility INFO: [Thread 3] Content-Type = image/png
net.http.download.httpdownloadutility INFO: [Thread 3] Content-Disposition = null
net.http.download.httpdownloadutility INFO: [Thread 3] Content-Length = 227288
net.http.download.httpdownloadutility INFO: [Thread 3] fileName = HP_picture_whitepaper-05.png
net.http.download.httpdownloadutility INFO: [Thread 0] Progress = 10%
net.http.download.httpdownloadutility INFO: [Thread 4] Progress = 10%
net.http.download.httpdownloadutility INFO: [Thread 5] Progress = 10%
net.http.download.httpdownloadutility INFO: [Thread 2] Progress = 10%
```

# Source

N/A

# Restrictions

None.

---  
_Markdown_   
_Copyright 2021 MicroEJ Corp. All rights reserved._   
_This library is provided in source code for use, modification and test, subject to license terms._   
_Any modification of the source code will break MicroEJ Corp. warranties on the whole library._   