1、将下载的离线脚本zip包拷贝至被扫描设备上。
		
2、解压缩zip包,包含windows-check.vbs、scripts.dat、readme.txt三个文件。
		
3、运行vbs脚本有两种方式:
		
   1)、打开cmd命令行窗口,进入脚本所在目录，执行如下命令:cscript windows-check.vbs <ip地址>。
		
   2)、直接双击windows-check.vbs运行脚本,这种方式脚本将自动从本地网络环境获取一个真实的ip作为被扫描设备的地址。
		
4、脚本运行结束后，在脚本同目录下会生成xml格式的离线结果文件,将该离线结果文件压缩成zip包，导入到脆弱性管理平台。