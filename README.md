# chttp
一个简单的http/https库，c语言实现，用到了 openssl库，支持 get和post
使用cmake工具来编译，在编译库之前，可以选择是否支持https  具体看 CMakeLists.txt

如果要支持https 请先安装openssl库

windows下 mingw编译通过

windows下 mingw编译命令为：
mkdir build
cd build
cmake -G"MinGW Makefiles" ..
make

创建build目的是 让一些文件生成在build目录下，不污染项目文件。

chttp/lib 目录下生成 chttp库文件
chttp/bin 目录下生成 测试程序



linux下测试通过。


程序肯定存在很多问题，若发现问题，欢迎联系我。qq 30670835  网站 ：www.zixue7.com
