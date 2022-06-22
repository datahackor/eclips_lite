## eclips lite
裁剪版的 Baker Hughes ECLIPS System

- 1年的闲暇时间，7万行代码的内核实现。
- SysV消息机制、共享内存、环形队列缓存；RDR、AFF、XTF、刻度等一系列文件格式完全实现。
- 多进程同步控制技术。
- 可以与5700系统模块互换运行，直接借用原版界面。

- DIP/UDIP/HDIP/STAR/ML/DLL/MLL/TBRT/MICR/DLLS/HDLL/TCR/GR/SL/PRSM/DSL/FLS/GRN/MERC 
- GRQ/DSL/CBL/MCBL/BAL/SBT/DRB/SRB/DIFL/DPIL/HDIL/3DEX/DEL4/DEL2/AC/CBIL/DAC/MAC 
- XMAC/FMT/RCI/NIR/PHT/TEMP/SON/RPM/MCFM/MCFP/FLEX/EART/RTEX/ADEX 

# dpymgr （显示控制管理程序）
- 显示缓存管理器程序  
包含一个C语言的编译器和词法分析器，词法分析器用汇编语言实现。 
各种显示程序的配置用简化的语言实现： 
这是一个配置文件的例子： 

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

- 通过CPP预处理之后变成这样：  

Section       
{  
  Program                 log;  
Arguments                 -title "MREX MON Pres. mrex_mon.log"  
                          -display                DPY_HOST  
                          -xrm "*iconName: MREX.log"  
                          -xrm "*iconic: FALSE"  
                          -geometry 611x951+655+0;  
  CRT?BackupColor pink;  
  ScrollPoint   0;  
     CRT?LineWidth 2; PLT?LineWidth 3; CRT?OverRangeLineWidth 3; PLT?OverRangeLineWidth 4; CRT?UnderRangeLineWidth 1; PLT?UnderRangeLineWidth 2  ;  
RepeatLineWidth           1;  
OverRange                 1;  
UnderRange                1;  
......  
Curve  
{  
  CurveName     cnc ;  
  Alias       cnc;  
  DefinedScale _CN_POR_SCALE;  
  Scale             0  60 ;  
  LineColor     red3;  
  LineStyle      12 4 ;  
  CutoffAttribute  
  {  
    CutoffCurve  ten;  
    Sequence     1;  
    CutOff       -99999,-99999;  
    CutOffExtension  0;  
    LineWidth    1;  
  }  
  Label  
  {  
Title       "NEUTRON POROSITY ";  
  Title2       cnc ;  
  }  
}  
}  
}  

- 然后再通过词法分析变成符号文件，再通过IPC传给显示程序，这样每一个显示程序就会自动显示相关的信息。  


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
