# 休息休息

- 1. 通过qt，采用定时任务创建休息提示软件，默认一个小时提示一次
- 2. 随机从bing壁纸获取图片作为背景图
- 3. 解决内存一直增加问题，之前QBrush指针，用完没有删除内存一直增加，改成对象实例
- 4. 休息时频繁多次弹出window
- 5. 休息倒计时

## todo
- 1. 工作时长可以进行自动设置


## 打包
- 1、软件直接生成exe在其他电脑上不可直接运行，因此需要用qt自带的windeployqt.exe工具搜集软件的依赖库，具体执行`..\windeployqt.exe dj_rest.exe`.qt中可能会有msvc以及mingw编译程序，采用哪个编译软件就用哪个下的windeployqt.exe
