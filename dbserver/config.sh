#/sbin/service mysqld start
make clean
qmake -project
qmake dbserver.pro
make
rm -rf *~
rm -rf .?*~
