# Simulation_fileContralSystem
实现一个简单的模拟文件管理系统
设计一

设计任务：模拟Linux文件系统

在任一OS下，建立一个大文件，把它假象成一张盘，在其中实现一个简单的模拟Linux文件系统。

1.在现有机器硬盘上开辟100M的硬盘空间，作为设定的硬盘空间。

2.编写一管理程序simdisk对此空间进行管理，以模拟Linux文件系统，要求：

（1）盘块大小1k 

（2）空闲盘块的管理：Linux位图法

（3）结构：超级块, i结点区, 根目录区

3.该simdisk管理程序的功能要求如下：

(1)info:  显示整个系统信息(参考Linux文件系统的系统信息)，文件可以根据用户进行读写保护。目录名和文件名支持全路径名和相对路径名，路径名各分量间用“/”隔开。

(2)cd …:  改变目录：改变当前工作目录，目录不存在时给出出错信息。

(3)dir …:  显示目录：显示指定目录下或当前目录下的信息，包括文件名、物理地址、保护码、文件长度、子目录等（带/s参数的dir命令，显示所有子目录）。

(4)md …:  创建目录：在指定路径或当前路径下创建指定目录。重名时给出错信息。

(5)rd …:  删除目录：删除指定目录下所有文件和子目录。要删目录不空时，要给出提示是否要删除。

(6)newfile …:  建立文件。

(7)cat …:  打开文件。

(8)copy …:  拷贝文件，除支持模拟Linux文件系统内部的文件拷贝外，还支持host文件系统与模拟Linux文件系统间的文件拷贝，host文件系统的文件命名为<host>…，如：将windows下D：盘的文件\data\sample\test.txt文件拷贝到模拟Linux文件系统中的/test/data目录，windows下D：盘的当前目录为D：\data，则使用命令：
simdisk copy <host>D：\data\sample\test.txt /test/data
或者：simdisk copy <host>D：sample\test.txt /test/data
  
(9)del …:  删除文件：删除指定文件，不存在时给出出错信息。
  
(10)check: 检测并恢复文件系统：对文件系统中的数据一致性进行检测，并自动根据文件系统的结构和信息进行数据再整理。
  
4.程序的总体流程为：
  
(1)初始化文件目录；
  
(2)输出提示符，等待接受命令，分析键入的命令；
  
(3)对合法的命令，执行相应的处理程序，否则输出错误信息，继续等待新命令，直到键入EXIT退出为止。

设计二
  
设计任务：模拟文件系统的前端操作shell
  
实现一个简单的shell（命令行解释器）。
  
将设计一的管理程序simdisk作为后台进程运行，利用本设计任务的shell操作simdisk。
  
本设计任务在于学会如何实现在前端的shell进程和后端的simdisk进程之间利用共享内存进行进程间通信(IPC)。
 三、开发环境
  
Windows 10     VS2019
  
四、程序的时序图：
  ![image](https://user-images.githubusercontent.com/94117475/144962201-5fceb5db-e557-4df9-b311-c5660708c174.png)
  五、主要算法及数据结构
  
1.整个系统的结构：本系统盘块大小为1KB，每1024个盘块构成1个数据块组，共100个数据块组，所以数据区的总大小为100M(不包括数据区前面的内容)
![image](https://user-images.githubusercontent.com/94117475/144962220-dfdaf3f2-c328-48c9-8411-2b2283e93f94.png)

  超级块的结构：存放系统的信息，记录系统的空间使用状况。
  ![image](https://user-images.githubusercontent.com/94117475/144962271-39341ce4-5acf-444b-8c4c-a36535d15711.png)
  
i结点的结构图：描述文件的属性，是寻找文件的关建所在。因为采取连续存储的结构，所以i结点的只存放文件的起立物理地址，该存储结构使得在顺序读取文件时速度很快，但无法进行文件的随机查找。
  ![image](https://user-images.githubusercontent.com/94117475/144962298-e6daad68-c516-4953-84a6-c1cb24629a44.png)

  目录：用于存储指定具体目录所包含文件(包括目录)的数目和具体的文件内容
  ![image](https://user-images.githubusercontent.com/94117475/144962305-6fdec438-5276-4e91-b757-82b8709d0da4.png)
  注：子文件组中第一个位置存放的父目录的信息，第二文件是本目录的信息；
  
  2.核心算法：
1.查找操作目录：文件管理系统的实现最根本的是要实行文件的增删查改，而这些操作本质上都是在对应的目录中进行的，所以大部分指令都需要先找到对应的目录的，对该目录下进行操作。

算法分析：该算法是采用深度搜索，算法的复杂度取决与文件系统目录的深度和读取文件内容上，相较于读取文件，找到对应目录的索引结点号的时间可以忽略不计，主要时间是花在对文件的读写操作。
  算法流程图：![image](https://user-images.githubusercontent.com/94117475/144962367-c95da50f-a6e7-40fd-97a9-b6cc4b4ca7b1.png)
3.分离路径和文件名：用户在输入指令时，路径中可能包含路径和要操作的文件名，很多情况下，我们需要将其分离出来；

算法分析:时间的复杂度取决与字符串的长度。O(strlen(path_and_name))
  3.获取当前工作路径：当使用cd命令时，需要改变当前的工作目录，为了使用户清晰的知道当前的工作目录，需要在命令提示符前将当前工作路径输出。
  ![image](https://user-images.githubusercontent.com/94117475/144962420-6e1e1f62-abb7-4e60-945b-43af669b3ce8.png)
  算法分析：该算法利用函数的递归，直到找到根目录，再从根目录开始将目录名加入路劲中。想要实现路劲的提取还可利用堆栈的数据的结构实现，利用的是堆栈先进后出的原理，这与函数的调用是相同的思路。
  4.为文件分配空间：由于系统对文件进行增删改时，磁盘的物理空间会发生变化，创建文件时需要在磁盘中找到一段满足条件的连续空闲空间来存放文件。

算法分析：从系统的起始数据块开始，找到第一段满足条件的空间，将文件存入其中。该算法的优点是当有特别大的文件需要存放时可以找到满足条件的空闲空间，但由于文件的大小并不是一样的，每次都找到第一段满足条件的空间会使前面的空闲空间被分割成很多小的空间（外部碎片），由于空间太小而无法得到利用。
  程序流程图：![image](https://user-images.githubusercontent.com/94117475/144962482-cf3691cc-b52b-4497-a3ba-908d75b1dfac.png)
  1.命令：format()
 文件系统的初始化
1.创建一个100M大小的字符数组，初始为0，然后写进文件；
2.初始化超级块，位图，结点区
3.创建根目录，初始化管理员和用户id
4.将

2.命令：info
功能：显示系统信息，包括总盘块数，盘块大小，每组盘块数，空闲块数等


3.命令：cd
 功能:显示当前路径
 算法步骤:递归查找上一级目录，直到找到根目录为止

4.命令：dir
 功能:显示目录,显示当前目录或指定目录(绝对路径或相对路径)
算法步骤:1.判断是否为显示当前目录，是则转4，否则转2
	     2.判断是否从根目录开始，是则从根目录开始查找到指定目录的inode，转4，否则转3
         3.判断是否为当前目录下的子目录，是则找到指定目录的inode，转4，否则表示无该目
          录，返回出错信息
5.循环显示文件和目录

6.命令：md
 功能:创建目录
 算法步骤:分析给的路径，分解出要创建的目录的路径和要创建的目录名；
         判断是否在当前目录下创建，如果是则判断是否存在同名目录，有则提示信息，否则直接分配空间创建目录；
         否则判断是绝对路径还是相对路径，找出创建的所在目录位置；
         然后判断该目录下是否有与要创建的目录的同名目录，有则给出提示信息，否则分配空间创建目录
 
7.命令：rd
 功能:删除目录
 算法步骤:先判断路径是否正确，错误则提示信息，正确则
          找到要删除的目录，判断是否为空，是则删除，更改父目录信息，否则
          提示信息，确定要删除时递归删除其文件即子目录
8.命令:newfile
功能:建立文件
 算法步骤:检查路径，如果路径正确则在正确的目录下建立文件，包括分配数据块和inode，最后修改超级块、
          组描述符、数据块位图、inode位图、文件所在的目录、数据区等
          否则提示错误信息
 
9.命令：cat
 功能:打开文件
  算法步骤:检查路径及文件是否正确，正确则取到文件的inode，申请和文件一样大小的缓冲区，
          把文件内容读到缓冲区并输出即可；
          否则提示错误信息
10.命令copy
功能:拷贝文件
 算法步骤:首先检查给出的两个路径是否正确；如果正确则找到对应的文件，读出内容到内容缓冲区中，再将内容缓冲区的内容写到目的路径中，否则提示错误信息
11.命令del
 功能:删除文件
 法步骤:检查给定的路径，找出要删除的文件所在的目录；
          如果路径正确，则调用delete_file(dir temp_dir,char *file_name)进行删除
          否则提示错误

12.命令：check
 功能:检测并回复文件系统
算法步骤:首先检查每个数据块组的空闲数据块和inode总数与记录的是否相同，不是则修改组描述符；       再检查超级块的空闲块的总数以及空闲inode总数与所有数据块组的空闲块总数的和以及         空闲inode总数的和是否相等，不相等则修改超级块的空闲块总数或空闲inode总数

