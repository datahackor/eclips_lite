# eclips_lite
裁剪版的 Baker Hughes ECLIPS System

# Router
 用来将数据转发给远程或本地通过网络接收的终端，比如显示。
 这个是将计算处理和显示松耦合的重要程序，每一个eclips实例都会有一个或多个router
 
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
logui再组织好发给mptask（也遵循大于250字节就用共享内存的规则），删除共享内存由接收方执行

- 为什么要给logui？而不是直接给mptask？因为logui需要显示给用户看。
- 
