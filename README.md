# Taller4_Sensores
TALLER CAN VIRTUAL Y FÍSICO — UNIVERSIDAD DE CUENCA
===================================================

Este repositorio contiene los archivos, códigos y documentación utilizados para el Taller 4:
Implementación de una red CAN virtual y física, desarrollado en Ubuntu utilizando Docker,
SocketCAN, SavvyCAN e interfaces físicas basadas en Arduino + MCP2515 + USB2CAN.

---------------------------------------------------
OBJETIVO
---------------------------------------------------
Familiarizarse con el funcionamiento del bus Controller Area Network (CAN) tanto en entornos
virtuales como en hardware real, comprendiendo su estructura de tramas, arbitraje y mecanismos
de transmisión mediante herramientas de software libre.

---------------------------------------------------
REQUISITOS PREVIOS
---------------------------------------------------
- Ubuntu 22.04 (host o máquina virtual)
- Docker instalado y funcionando correctamente
  sudo apt install docker.io -y
  sudo systemctl enable --now docker

- Wireshark (opcional, para análisis gráfico)
- Git, build-essential, x11-apps
- Adaptador USB2CAN y módulos MCP2515 + Arduino UNO/Nano (para la parte física)

---------------------------------------------------
PARTE 1: CONFIGURACIÓN DEL ENTORNO CAN VIRTUAL
---------------------------------------------------

1. Crear interfaz virtual CAN
   sudo modprobe vcan
   sudo ip link add dev vcan0 type vcan
   sudo ip link set vcan0 up
   ip link show

2. Habilitar entorno gráfico para Docker
   xhost +local:root

3. Crear entorno de trabajo
   mkdir ~/taller_can
   cd ~/taller_can
   nano Dockerfile

4. Contenido del Dockerfile
   FROM ubuntu:22.04
   ENV DEBIAN_FRONTEND=noninteractive
   WORKDIR /root

   RUN apt update && apt upgrade -y && \
       apt install -y iproute2 net-tools can-utils git nano

   CMD modprobe vcan || true && \
       ip link add dev vcan0 type vcan || true && \
       ip link set up vcan0 && \
       bash

5. Construir imagen Docker
   docker build -t taller_can .

6. Ejecutar contenedor
   docker run -it --name can_lab_parte1 \
     --privileged \
     --network host \
     -e DISPLAY=$DISPLAY \
     -v /tmp/.X11-unix:/tmp/.X11-unix \
     -v ~/taller_can:/root/work \
     taller_can bash

7. Probar interfaz y herramientas básicas
   ip link show
   cangen vcan0
   candump vcan0
   candump vcan0 > /root/can_traffic.log
   canplayer -I /root/can_traffic.log
   cansend vcan0 012#686F6C616D756E646F

8. Analizar con Wireshark
   sudo wireshark &
   Seleccionar la interfaz vcan0

---------------------------------------------------
PARTE 2: SAVVYCAN Y VISUALIZACIÓN GRÁFICA
---------------------------------------------------

1. Verificar entorno gráfico
   echo $DISPLAY
   export DISPLAY=:0
   apt update && apt install -y x11-apps
   xclock &

2. Instalar dependencias de SavvyCAN
   apt install -y build-essential git curl \
     qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools \
     qttools5-dev qttools5-dev-tools \
     qtdeclarative5-dev qml-module-qtquick-controls \
     qml-module-qtquick-controls2 qml-module-qtquick-dialogs \
     qml-module-qtquick-layouts qml-module-qtquick-window2 \
     qml-module-qtgraphicaleffects qml-module-qtquick2 \
     libqt5serialbus5-dev libqt5serialport5-dev \
     libqt5svg5-dev libqt5charts5-dev

3. Descargar y compilar SavvyCAN
   cd /root
   git clone https://github.com/collin80/SavvyCAN.git
   cd SavvyCAN
   qmake
   make -j$(nproc)
   ./SavvyCAN &

4. Conectar con la interfaz CAN
   En la interfaz gráfica:
   - Connections → Add New Device Connection
   - Connection Type: SocketCAN
   - Interface Name: vcan0
   - Connect

---------------------------------------------------
PARTE 3: SIMULACIÓN CON ICSIM
---------------------------------------------------

1. Instalar dependencias y clonar
   apt install -y libsdl2-dev libsdl2-image-dev build-essential git
   git clone https://github.com/zombieCraig/ICSim.git
   cd ICSim
   make

2. Ejecutar simulador y controles
   ./icsim vcan0
   ./controls vcan0
   candump vcan0

   Teclas de control:
   A, B, X, Y -> Acciones del vehículo
   Flechas -> Dirección
   Shift -> Luces

---------------------------------------------------
PARTE 4: CAN FÍSICO (ARDUINO + MCP2515 + USB2CAN)
---------------------------------------------------

1. Verificar conexión del adaptador
   lsusb
   ip link show

2. Configurar VirtualBox (si usas VM)
   - Cerrar la máquina virtual
   - Configuración → USB → Activar USB 3.0 (xHCI)
   - Agregar filtro USB para el dispositivo USB2CAN
   - Reiniciar la VM

3. Instalar herramientas y activar interfaz
   sudo apt install -y can-utils
   sudo ip link set can0 type can bitrate 125000
   sudo ip link set can0 up
   candump can0

4. Subir códigos a los Arduinos
   - TX: envía tramas “hello” y “world”
   - RX: recibe y muestra mensajes en el monitor serial
   - MCP2515: usa la librería mcp_can a 125 kbps

   Códigos fuente:
   anexos/tx_arduino.ino
   anexos/rx_arduino.ino
   anexos/mcp_sender.ino

5. Analizar con Wireshark
   Seleccionar interfaz can0
   Verificar mensajes “Hello” y “Pepit”
   Observar tramas extendidas

---------------------------------------------------
REFERENCIAS
---------------------------------------------------
- SocketCAN Documentation: https://www.kernel.org/doc/Documentation/networking/can.txt
- SavvyCAN GitHub Repository: https://github.com/collin80/SavvyCAN
- ICSim Vehicle Simulator: https://github.com/zombieCraig/ICSim
- MCP2515 Arduino Library: https://github.com/coryjfowler/MCP_CAN_lib

---------------------------------------------------
AUTORES
---------------------------------------------------
Mateo Eduardo Bermeo Pesántez
Santiago Andrés Guillén Malla
Vicente Paúl Jiménez Ávila

Universidad de Cuenca – Facultad de Ingeniería
Carrera de Telecomunicaciones y Electrónica

