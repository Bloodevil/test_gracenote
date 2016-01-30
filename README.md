# test_gracenote
quiz1 from gracenote

diskusage
=======
diskusage --mounted=/mnt/storage -logtype=file --logpath=/var/log/diskusage.log
--mount :  default /
--logtype : default stdout.
--logpath : file only

USAGE
======
root@ip-172-30-1-81:/home/ubuntu/test_gracenote# python3 diskusage.py -h
usage: diskusage.py [-h] [--mounted MOUNTED] [--logtype LOGTYPE]
                    [--logpath LOGPATH]

optional arguments:
  -h, --help         show this help message and exit
  --mounted MOUNTED
  --logtype LOGTYPE
  --logpath LOGPATH
