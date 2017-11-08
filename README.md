## 图书简介
CoAP是受限制的应用协议(Constrained Application Protocol)的简称。随着近几年物联网技术的快速发展，越来越多的设备需要接入互联网。虽然对人们而言连接互联网显得方便而快捷，但是对于那些低功耗受限制设备而言接入互联网却非常困难。在当前由PC机和智能手机组成的互联网世界中，信息交换一般通过TCP和HTTP协议实现。但是对于那些低功耗受限制设备而言，实现TCP和HTTP协议也许是一个非常苛刻的要求。为了让低功耗受限制设也可以流畅接入互联网，CoAP协议应运而生。CoAP是一种物联网应用层协议，它运行于UDP协议之上而不是像HTTP那样运行于TCP之上。CoAP协议借鉴了HTTP协议的大量成功经验，CoAP协议和HTTP协议一样均使用请求响应工作模式。和HTTP协议采用文本首部不同，CoAP协议采用完全的二进制首部，这使得CoAP协议的首部更短，传输效率更高。CoAP协议为受限制设备而生，一个内存仅有20KB的单片机也可以实现CoAP服务器或客户端。
## 本书主要内容
- 学习CoAP协议必要的网络基础知识；
- CoAP与MQTT、HTTP之间的区别与联系；
- CoAP核心内容：二进制首部、工作模式、重传机制、响应吗、选项和媒体类型等；
- CoAP扩展内容：CoAP资源描述和CoAP观察者；
- 使用C语言、Python或Node.js实现CoAP客户端与服务器；
- 使用Copper插件和Wireshark调试CoAP。

![CoAP in Action](http://ovqdgsm5c.bkt.clouddn.com/17-10-12/96986591.jpg)

## 图书勘误
### 最新版火狐浏览器不能使用Copper插件
最新版火狐浏览器（56版本及以上）已经*不能正确*支持Copper插件。如果想尝试本书中关于Copper插件的相关功能，请使用火狐 55.0版本或更低版本。
火狐浏览器的历史版本可通过以下途径获取
- [FTP服务器](http://ftp.mozilla.org/pub/firefox/releases/)
- [火狐浏览器下载与安装](http://www.firefox.com.cn/download/#more)
![](http://ovqdgsm5c.bkt.clouddn.com/17-11-8/191344.jpg)
