import sys
import shutil
import time
import logging
import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--mounted', default='/')
    parser.add_argument('--logtype', default='stdout')
    parser.add_argument('--logpath')
    args = parser.parse_args()

    if args.logtype == 'file':
        if not args.logpath or args.logpath.strip()=="":
            print("you should enter the logpath.")
            exit(0)

    LOG_FILENAME = args.logpath
    disk_path = args.mounted
    if args.logtype == "stdout":
        logging.basicConfig(level=logging.DEBUG,handlers=[logging.StreamHandler()])
    elif args.logtype == "file":
        logging.basicConfig(filename=LOG_FILENAME, level=logging.DEBUG)
    
    while True:
        total, used, free = shutil.disk_usage(disk_path)
        usage_percent = int(used/free*100)
        content = "path:"+disk_path+" capacity:"+str(usage_percent)+"% size:"+str(free)
        if usage_percent < 90:
            logging.info(content)
        elif 95 > usage_percent >= 90:
            logging.warning(content)
        else:
            logging.error(content)
            time.sleep(60)
