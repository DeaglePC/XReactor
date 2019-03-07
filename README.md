# XReactor  
Reactor事件驱动设计模式实现，基于io复用  
参考redis实现  

主要是两个事件，io和定时器的实现  

# How to test  
1.参考test  
```shell
make
./test
```

2.或直接使用Reactor类即可  

增加io复用的方式继承接口类即可
