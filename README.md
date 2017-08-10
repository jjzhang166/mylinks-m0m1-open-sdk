# mylinks-m0m1-open-sdk #

----------

基于 FreeRTOS 的 M0M1系列 SDK开发包。
   
## 注意 ##

更多的信息请关注 [M0M1系列sdk更改记录](http://git.oschina.net/mqlinks/mylinks-m0m1-open-sdk/wikis/M0M1系列sdk更改记录) !

## 要求 ##

编译此SDK包需要下载相应的toolchain编译器.
下载地址为： [toolchain](https://pan.baidu.com/s/1qY7EEp2).


  
## 编译 ##

克隆 mylinks-m0m1-open-sdk, e.g., to ~/mylinks-m0m1-open-sdk.
```
$git clone https://git.oschina.net/mqlinks/mylinks-m0m1-open-sdk.git
```

安装编译器：
```
mkdir ~/toolchain
cp ba-elf_4.7.3.tgz ~/toolchain
cd ~/toolchain
tar xvzf ba-elf_4.7.3.tgz
```

生成 img,以SDK包中proj/at为例生成的at.img:
``` 
cd ~/mylinks-m0m1-open-sdk
make at
```

当编译完成，在~/mylinks-m0m1-open-sdk/image中生成at.img   


## 下载到模块 ##

将M0M1系列模块GPIO6在通电前接地。
在windows环境下使用MylinkDownloadTool.exe将at.img下载。

## ~/mylinks-m0m1-open-sdk下项目说明 ##
```
1.iotaliyun:阿里云物联网套件例程
2.tcpclient:TCP客户端
3.mqtt:MQTT客户端
4.tcpserver:TCP服务器例程：支持5路TCP连接
5.udp:UDP例程
6.smartconfig:Smartconfig例程
7.at:AT指令集+内置WEB服务器+内置网页HTML5和JS
8.sntp:网络同步时间例程
```


