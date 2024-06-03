# wffTool
windows上一个文件标记工具，适用场景是多设备间拷贝文件，优点是可以帮助删除未标记文件：标记->删除未标记->拷贝

示例场景：（回家办公）
我有一个visual studio工程项目P在公司设备A和家里B上开发，会频繁的相互拷贝和覆盖。
开发编译过程中会产生大量临时文件，这些文件不需要拷贝且占用大量磁盘空间。
那么我可以用此工具只标记重要文件，每次编译产生的临时文件一键删除掉，然后再拷贝覆盖。
这样就可以节省大量磁盘空间和大量拷贝覆盖时间。