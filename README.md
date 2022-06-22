## eclips_lite
裁剪版的 Baker Hughes ECLIPS System

- 1年的闲暇时间，7万行代码的内核实现。
- SysV消息机制、共享内存、环形队列缓存；RDR、AFF、XTF、刻度等一系列文件格式完全实现。
- 多进程同步控制技术。
- 可以与5700系统模块互换运行，直接借用原版界面。

# dpymgr （显示控制管理程序）
- 显示缓存管理器程序
包含一个C语言的编译器和词法分析器，词法分析器用汇编语言实现。 
各种显示程序的配置用简化的语言实现： 
这是一个配置文件的例子： 
- 
Section  
{   
  Program   log;   
#ifndef ARGS_DEFINED  
  Arguments  -title "Processed Data  p857d.log"  
             -xrm "*iconName: PROC"  
             -xrm "*iconic: FALSE"  
             -geometry 835x840+410+80;    
#endif /* ARGS_DEFINED */  
#include             <std.dgrds>   
#undef               TRACKLIST     
#define              TRACKLIST   "api.tlst"  
#ifndef              DEPTHGRID   
#define              DEPTHGRID   STANDARD_DGRD   
#endif   
#include             <std.lines>  
#include             <plotmgr.init>  
  CRT?CurveOff     ;  
#include             <standard.fncs>  
#include             <default.scales>  
#include             <std.fils>  
#undef               LOG_SCALE   
#include             <p857d.crvs>  
#include             <plotmgr.anot>    
#ifndef              PLOTFILE   
  FileAlias          CORRELATE1;   
#include             <gr-ccl.corr>  
FileAlias            REPEAT1;   
RepeatCurveColor     REPEAT1_COLOR;   
#include             <p857d.rpt>  
FileAlias            REPEAT2;   
RepeatCurveColor     REPEAT2_COLOR;   
#include             <p857d.rpt  >
#endif   
}    

                       _oo0oo_
                      o8888888o
                      88" . "88
                      (| -_- |)
                      0\  =  /0
                    ___/`---'\___
                  .' \\|     |// '.
                 / \\|||  :  |||// \
                / _||||| -:- |||||- \
               |   | \\\  -  /// |   |
               | \_|  ''\---/''  |_/ |
               \  .-\__  '-'  ___/-. /
             ___'. .'  /--.--\  `. .'___
          ."" '<  `.___\_<|>_/___.' >' "".
         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
         \  \ `_.   \_ __\ /__ _/   .-` /  /
     =====`-.____`.___ \_____/___.-`___.-'=====

# 界面预览
 
![avatar](https://github.com/datahackor/eclips_lite/blob/main/pics/2022-04-28%2015_09_27-eclips_lite%20-%20Microsoft%20Visual%20Studio.png)
![avatar](https://github.com/datahackor/eclips_lite/blob/main/pics/2022-04-28%2015_14_14-NMR%20Data%20Acqusition%20Systemm.png)
![avatar](https://github.com/datahackor/eclips_lite/blob/main/pics/2022-04-28%2015_21_03-eclips_lite%20(Running)%20-%20Microsoft%20Visual%20Studio.png)
![avatar](https://github.com/datahackor/eclips_lite/blob/main/pics/2022-04-28%2015_14_47-log%20-%20%5BMREX%20MON%20Pres.%20mrex_mon.log%5D.png)
