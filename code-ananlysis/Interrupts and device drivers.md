# Console




# Lab: networking

## The intel manual 
### Paket Reception
#### packet reception consists of :
* recognizing the presence of a packet on the wire
* performing address filtering
    * whether bad packets should be received 
* storing the packet in the receive data FIFO
    * Memory buffers pointed to by descriptors store packet data
    * Different buffer size is selected by bit settings in the Receive Control register (<strong>RCTL.BSIZE & RCTL.BSEX</strong>)
    * <strong>Receive Descriptor</strong>——`struct rx_desc`: a data structure that contains the receive data buffer address and fields for hardware to store packet information
        * <strong>status</strong>: indicates whether the descriptor has been used and whether the referenced buffer is the last one for the packet
        * <strong>errors</strong>: Most error information appears only when the Store Bad Packets bit (RCTL.SBP) is set and a bad packet is received
        * <strong>special</strong> :
*  transferring the data to a receive buffer in host memory
*  updating the state of a <strong>receive descriptor</strong>

#### Receive ring(Receive queue): the array of descriptors
Software adds receive descriptors by writing the tail pointer with the index of the entry beyond the last valid descriptor. As packets arrive, they are stored in memory and the head pointer is incremented by hardware. When the head pointer is equal to the tail pointer, the ring is empty. Hardware stops storing packets in system memory until software advances the tail pointer, making more receive buffers available.
* `#define E1000_RDBAL    (0x02800/4)  /* RX Descriptor Base Address Low - RW */`
* `#define E1000_RDLEN    (0x02808/4)  /* RX Descriptor Length - RW */`
* `#define E1000_RDH      (0x02810/4)  /* RX Descriptor Head - RW */`
* `#define E1000_RDT      (0x02818/4)  /* RX Descriptor Tail - RW */`
  
If software statically allocates buffers, and uses memory read to check for completed descriptors, it 
simply has to zero the status byte in the descriptor to make it ready for reuse by hardware

### Paket Transmission
* The protocol stack receives from an application a block of data that is to be transmitted
* The protocol stack calculates the number of packets required to transmit this block based on the MTU size of the media and required packet headers
* For each packet of the data block:
    * Ethernet, IP and TCP/UDP headers are prepared by the stack.
    * The stack interfaces with the software device driver and commands the driver to send the individual packet.
    * The driver gets the frame and interfaces with the hardware.
    * The hardware reads the packet from host memory (via DMA transfers).
    * The driver returns ownership of the packet to the Network Operating System (NOS) when the hardware has completed the DMA transfer of the frame (indicated by an interrupt).

#### Transmit Data Storage
* Data are stored in buffers pointed to by the descriptors.
*  A packet typically consists of two (or more) descriptors, one (or more) for the header and one for the actual data

### Registers(P233)
* ICR : receive-related interrupt


### Net.c
* net_rx: called by e1000 driver's interrupt handler to deliver a packet to the networking stack
* net_rx_ip : receives an IP packet










