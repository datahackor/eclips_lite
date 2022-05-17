# eclips_lite
裁剪版的 Baker Hughes ECLIPS System

- 1年的闲暇时间，7万行代码的内核实现。
- SysV消息机制、RDR、AFF、XTF、刻度等一系列文件格式完全实现。
- 牛逼啊牛逼！为什么说牛逼呢，因为可以与5700系统模块互换运行。


# cls2exe
- 管理“开始”“停止”记录
- 管理“开始、停止刻度校验”
- 启动loguif（数据采集主菜单）
# loguif
- 发送命令
- 系统状态
- 请求系统信息
# wtsift
WTS  Interface Task
- 从WTS/RAP接收RDR数据并保存在缓存池（buffer pool）
- 发送命令给WTS/RAP
# wtsbufmgr
- 管理数据请求，包括一个写请求和多个读请求
# mptask
主数据处理任务（Main Processing Task），RDR数据先存进CDSS然后调用相关PFN进行计算
计算完成后数据进入outbuf
- 从缓存池（buffer pool）检索数据
- 处理计算曲线数据
- 保存计算处理完成的数据到outbuf(输出缓存池)
# outbuf
输出缓存池，包括一个写入者和多个读取者
写入者是mptask，读取者为刻度、显示、存AFF

# rdrdrvr
RDR驱动，读取RDR记录，发送给 wtsift，才通过wtsift 发送给msptask

# EI_TYPE
EI_TYPE ==1 RDR DRIVER
EI_TYPE ==2 WTS DRIVER

# share memory 共享内存
进程间的数据交换都是消息传递，大的数据使用共享内存
比如tcc发送给logui的PFN参数数据修改，大于250字节的话就用共享内存
loguif再组织好发给mptask（也遵循大于250字节就用共享内存的规则），删除共享内存由接收方执行

- 
![avatar](https://github.com/datahackor/eclips_lite/blob/main/pics/2022-04-28%2015_09_27-eclips_lite%20-%20Microsoft%20Visual%20Studio.png)
![avatar](https://github.com/datahackor/eclips_lite/blob/main/pics/2022-04-28%2015_14_14-NMR%20Data%20Acqusition%20Systemm.png)
![avatar](https://github.com/datahackor/eclips_lite/blob/main/pics/2022-04-28%2015_21_03-eclips_lite%20(Running)%20-%20Microsoft%20Visual%20Studio.png)
![avatar](https://github.com/datahackor/eclips_lite/blob/main/pics/2022-04-28%2015_14_47-log%20-%20%5BMREX%20MON%20Pres.%20mrex_mon.log%5D.png)
