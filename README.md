## mylinks-m0m1-open-sdk

   
### 环境
- M0M1系列WIFI模块SDK是基于 `FreeRTOS` 版本开发并实现的。
- 虚拟机环境`VMware-workstation-full-12.5.7.20721.exe`:[下载](http://pan.baidu.com/s/1eRHmoJC)
- linux`lubuntu-14.0.4`:[iso](http://pan.baidu.com/s/1kUAWzKr)[VMware安装压缩包](https://pan.baidu.com/s/1kUKzQuj)
- 编译器`toolchain`:[下载](https://pan.baidu.com/s/1qY7EEp2)
- 固件下载工具`MylinkDownloadTool.exe`:[下载](http://pan.baidu.com/s/1mi9jBlQ)
- 更多的信息请关注 [M0M1系列sdk更改记录](http://git.oschina.net/mqlinks/mylinks-m0m1-open-sdk/wikis/M0M1系列sdk更改记录) !

### 项目
```
mylinks-m0m1-open-sdk            -------项目主目录
    images                       -------烧写文件生成目录
    include                      -------头文件
    lib                          -------库
        axtls                    -------小型的SSL开源库
        cJSON                    -------JSON开源库
        mqtt                     -------mqtt client开源库
        
    proj                         -------用户项目
    utility                      
toolchain                        -------编译器解压目录
```

  
### 编译

克隆 mylinks-m0m1-open-sdk, e.g., to ~/mylinks-m0m1-open-sdk.
>`$git clone https://git.oschina.net/mqlinks/mylinks-m0m1-open-sdk.git`

>安装编译器：
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


### 下载到模块

将M0M1系列模块GPIO6在通电前接地。
在windows环境下使用MylinkDownloadTool.exe将at.img下载。

### proj下项目说明


1. `iotaliyun`:阿里云物联网套件例程
1. `tcpclient`:TCP客户端
1. `mqtt`:MQTT客户端
1. `tcpserver`:TCP服务器例程：支持5路TCP连接
1. `udp`:UDP例程
1. `smartconfig`:Smartconfig例程
1. `at`:AT指令集+内置WEB服务器+内置网页HTML5和JS
1. `sntp`:网络同步时间例程
1. `httpget`:HTTP连接使用GET方式提取数据
