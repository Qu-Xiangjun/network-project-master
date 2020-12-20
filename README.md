# Compute_Network_Project
###### quxiangjun

### 文件目录

```
Compute_Network_Project
├── code							---->代码文件
│	├── Checksum.h					---->16位校验和计算
│	├── crc32.h 					---->32位crc校验计算
│	├── sender_main.c				---->应用层聊天命令行界面
│	├── UDP_sender.h				---->UDP层组装和发送函数
│	├── network_sender.h 			---->网络层ip组装、分片、发送函数
│	├── datalink_sender.h			---->链路层frame组装和发送函数
│	├── receiver_main.c				---->应用层聊天命令行接收消息界面
│	├── UDP_receiver.h				---->UDP层接收函数与解析
│	├── network_receiver.h			---->网络层接受ip datagram组装分片和解析
│	├── datalink_receiver.h			---->数据链路层收frame与解析
│	├── Output_Octal.h				---->辅助工具类，输出二进制的字符串
│	├── pipe.bin					---->物理层的传输文件介质
│	└── datalink_to_network.bin		---->链路层传输信息到文件，网络层从文件取分片
├── sender1.png					---->发送聊天信息的前半截图片
├── sender2.png					---->发送聊天信息的后半截图片
├── receiver1.png				---->接收聊天信息的前半截图片
├── receiver2.png				---->接收聊天信息的后半截图片
└── README.md	
```

### 简介

本项目完成了一个基于数据链路层以太帧、网络层ip数据报和传输层UDP的简单聊天信息发送与接收功能。

### 原理解析

##### 发送端

​		如下图1发送端在sender_main.c文件运行后，在命令行输入发送的信息，如下是输入了超过1500字节的信息，以便查看网络层的分片功能。

​		然后可看到命令行提示传输层UDP开始工作，其显示了源端口和目的端口号和传输的data内容（与输入的消息一致），同时得到校验和的内容，填入了头部，最终完成了UDP层传输。

<img src="sender1.png" alt="sender1" style="zoom:60%;" />

​		如下图二，网络层的ip数据报开始分片，第一个分片的数据报开始传输，其源地址和目的地址都打印了出来，然后做了校验和填写到头部，分片成功，其长度为最长的1500字节。

​		接下来便调用了链路层的以太帧构建和传输，以太帧有mac地址的源地址和目的地址，做了crc校验。最后将内容写入到文件传输介质中。

​		在如上的分片传输完成后，继续下面的分片传输，整个分片传输完成后可看到返回了传输成功的信息。

<img src="E:\Study\Term5\Computer Network\network-project-master\20186471-屈湘钧-计网project\sender2.png" alt="sender2" style="zoom:60%;" />

##### 接收端

​		接受端打开后，即调用了receiver_main.c文件主函数，然后开始调用传输层UDP的接收程序，UDP层调用IP数据报接收，IP数据报继续调用以太帧的接收程序。

​		所以首先我们可以看到以太帧的接收程序开始，其从文件循环读出所有的信息，其检测目的地址为正确，crc32校验合格，接收到数据，解析出payload，然后依次把payload放入与网络层之间的缓冲文件中。

​		网络层在链路层工作好后，开始从缓冲区接收所有的数据报，对其开始解析，对比ip地址是否正确，进行校验和检验，并循环组装所有解析好的数据报payload。将组装好的payload返回给传输层。

<img src="E:\Study\Term5\Computer Network\network-project-master\20186471-屈湘钧-计网project\receiver1.png" alt="receiver1" style="zoom:60%;" />

​		传输层的UDP接收程序开始，其检验数据的校验和看是否为0，然后进行端口校验，完成后，解析出data数据返回给应用层的调用程序。

​		最终应用层即main函数得到message，并打印在命令行的最后。

<img src="E:\Study\Term5\Computer Network\network-project-master\20186471-屈湘钧-计网project\receiver2.png" alt="receiver2" style="zoom:60%;" />