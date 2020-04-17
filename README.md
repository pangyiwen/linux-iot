# Linux-iot
Just to do it, iot in Linux ！

testing:

Android Binder : https://blog.csdn.net/boyupeng/article/details/47011383

Linux Input子系统 ：http://www.360doc.com/content/13/0507/15/496343_283628273.shtml

Linux技术进阶示意图 ： https://blog.csdn.net/tyuuhgf/article/details/86483321


>git
1.本地建仓
  本地目录下执行
  $git init
  执行如下命令生成SSH Key，按回车键默认配置执行
  $ssh-keygen -t rsa -C "自己的邮箱"
  将生成的SSH Key加到github上去
  ~/.ssh/id_rsa.pub  (该文件所有内容都赋值到github的添加ssh-key里面去)
  执行如下命令，设置提交代码人的基本信息（为提交做准备）
  $git config --global user.email "你的邮箱"
  $git config --global user.name "自定义名"
  
2.提交代码
  第一次建仓，需要先拉一份库上的代码
  $git pull git@github.com:pangyiwen/linux-iot.git
  开始提交代码
  git status
  git add test.txt
  git commit -s
  git push origin master



