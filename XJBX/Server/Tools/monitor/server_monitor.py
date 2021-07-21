#encoding: utf-8
import time
import os
import sys
import fcntl
import urllib
from urllib.parse import quote 
import re
import logging
from logging.handlers import TimedRotatingFileHandler
import configparser
import math
import datetime #导入日期时间模块
from bottle import template # web框架
import smtplib
from email.mime.text import MIMEText
from email.utils import formataddr
import string

SERVER_PATH= '/han/monitor/src/'
CONFIG_FILE = SERVER_PATH + 'server_monitor.ini'
LOG_FILE_PATH = SERVER_PATH + 'log/'
INTERVAL = 15   # 秒
SECONDS_OF_MIN = 60 #便于调试!!!



def processInstance():
    # 主要怕运行了两次，因为我后续会crontab -e定时执行
    ret = os.popen('ps -ef |grep "python.*server_monitor.py" |grep -v grep |grep -v sudo |wc -l').read().strip()
    print("processInstance ret = " + ret)
    if ret != str(1): 
        print ('another instance running,ret:' + str(ret))
        sys.exit(1)

def isProcessAlive(aliveCmd):
    # 您还活着嘛
    #print('aliveCmd: ' + aliveCmd) # aliveCmd: ps -ef |grep "mysql" |grep -v grep 1>/dev/null 2>&1
    ret = os.system(aliveCmd)
    return ret == 0

def startProcess(startCmd):
    ret = os.system(startCmd)
    logger.info('system cmd {} ret:{}'.format(startCmd, ret))

def creat_html(info_items, the_day):
    html = """
    <html>
    <title>消息统计</title>
    <head></head>
    <body>
    <br></br>
    <h2 align=center>报警消息({{date}})</h2>
    <table width="90%" align=center border="0" bgcolor="#666666" cellpadding="8">
        <tr bgcolor="#DDDDDD">
            <th>IP地址</th>
            <th>进程名称</th>
            <th>所属类型</th>
            <th>详细描述</th>
            <th>时间</th>
        </tr>
        <tr align=center bgcolor="#FFFFFF">
            <td><font color="#33CC00">{{items[0]}}</font></td>
            <td><font color="#33CC00">{{items[1]}}</font></td>
            <td><font color="#33CC00">{{items[2]}}</font></td>
            <td><font color="#33CC00">{{items[3]}}</font></td>
            <td><font color="#33CC00">{{items[4]}}</font></td>
        </tr>
    </table> 
    </body>
    </html>
    """
    return template(html, items=info_items, date=the_day)

def sendMail(cfg, process, content):
        ret = True
        ipAddr = cfg.get('DEFAULT', 'ipAddr') 
        receivers = cfg.get('DEFAULT', 'receivers') # 收件人列表
        sender = cfg.get('DEFAULT', 'sender')
        passwd = cfg.get('DEFAULT', 'passwd') # 授权码
        smtp_server = cfg.get('DEFAULT', 'smtp_server')
        smtp_port = cfg.get('DEFAULT', 'smtp_port')
        subject = cfg.get('DEFAULT', 'subject')
        # today = datetime.date.today() #获得今天的日期
        nowTime = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        
        item = [ipAddr, process, 'process_monitor', content, nowTime]
        print(item) 
        # ['192.168.221.132', 'nginx', 'process_monitor', 'nginx sendMailed, restart ok', '2019-09-03 21:14:05']
        # creat_html自己写的
        html = creat_html(item, nowTime) # html页面格式化
        # msg = MIMEText(html, 'html', 'utf-8') # 网页格式
        # 封装成邮件的消息
        msg = MIMEText(html, 'html', 'utf-8') # 封装成邮件的内容
        msg['From']=formataddr([sender,sender])   #括号里的对应发件人邮箱昵称、发件人邮箱账号
        msg['To']=";".join([receivers])   #收件人列表
        msg['Subject']=subject #邮件的主题，也可以说是标题
        receivers = receivers.split(',') # 592407834@qq.com,326873713@qq.com
        print(receivers)
        try:
            # server = smtplib.SMTP(smtp_server, smtp_port)  #发件人邮箱中的SMTP服务器，
            server = smtplib.SMTP_SSL(smtp_server, smtp_port)  #发件人邮箱中的SMTP服务器（SSL使用）           
            server.login(sender, passwd)    #括号中对应的是发件人邮箱账号、邮箱smtp授权码
            server.sendmail(sender, receivers, msg.as_string())   #括号中对应的是发件人邮箱账号、收件人邮箱账号、发送邮件
            server.quit()   #这句是关闭连接的意思
        except Exception as e:
            print (e)
            logger.error(e)
        return ret

def monitorProcess(process, cfg, state):
    # 进程拉起失败后降低监控频率
    # if not state['isAlive'] and state['skipTimes'] < math.pow(2, state['startTimes']) and state['skipTimes'] < 10:
    #     state['skipTimes'] += 1
    #     return
    if isProcessAlive(cfg.get(process, 'aliveCmd')):
        # state['isAlive'] = True
        # state['startTimes'] = 0
        # state['skipTimes'] = 0                                      
        return 
    logger.info(process + ' is not alive')      
    print(process + ' is not alive')                  
    # 已打开的日志文件描述符fd将传递给nginx进程!                  
    startProcess(cfg.get(process, 'startCmd'))                 
    time.sleep(2) # 过了一段时间再去检测进程是否真正的启动了
    if isProcessAlive(cfg.get(process, 'aliveCmd')):
        logger.info('start ' + process + ' succeed')
        print('start ' + process + ' succeed')
        sendMail(cfg, process, process + ' sendMailed, restart ok') # 重启成功
        # state['isAlive'] = True
        # state['startTimes'] = 0
        # state['skipTimes'] = 0
    else:
        logger.info('start ' + process + ' failed')
        print('start ' + process + ' failed')
        now = int(time.time())
        #print now , state['lastReportTime'] + SECONDS_OF_MIN * cfg.getint(process, 'reportInterval')
        if state['isAlive'] or now > state['lastReportTime'] + SECONDS_OF_MIN * cfg.getint(process, 'reportInterval'):
            sendMail(cfg, process, process + ' died, restart fail')
            state['lastReportTime'] = now
        
        # state['isAlive'] = False
        # state['startTimes'] += 1
        # state['skipTimes'] = 1

def getLogger():
    # 创建一个logger
    logger = logging.getLogger('server_monitor')
    logger.setLevel(logging.INFO)

    # 创建一个handler
    logFileName = LOG_FILE_PATH + 'server_monitor.log'
    dailyFH = TimedRotatingFileHandler(filename=logFileName, when='MIDNIGHT', interval=1, backupCount=30)
    dailyFH.setLevel(logging.INFO)
    
    # 定义输出格式 
    formatter = logging.Formatter('%(asctime)s|%(levelname)s|%(process)d|%(funcName)s|%(lineno)d|%(message)s')
    dailyFH.setFormatter(formatter)

    # 将handler添加到logger
    logger.propagate = False
    logger.addHandler(dailyFH)

    return logger

if __name__ == '__main__':
    processInstance()   # 检测是否已经有实例在运行，如果已经有了则自己退出
    try:
        logger = getLogger()
    except Exception as e:
        print (e)
        sys.exit(1)
    # 初始化配置
    logger.info('loading config...')  # 配置文件
    cfg = configparser.SafeConfigParser()
    try:
        cfg.read(CONFIG_FILE)   # 读取配置脚本
    except Exception as e:
        logger.error('read config failed:' + str(e))
        exit(1)

    stateDict = dict({}) # 字典用来存放section
    for process in cfg.sections():
        print("process: " + process)
        stateDict[process] = dict({'isAlive':True, 'startTimes':0, 'skipTimes':0, 'lastReportTime':0})
    
    print('stateDict: %s' % stateDict.items())

    os.chdir(SERVER_PATH) # 转换到指定工作目录！！！！不要忘记了

    logger.info('start monitoring...')
    # sendMail(cfg, 'server_monitor', 'server_monitor start...')
    #print stateDict
    while True:
        for process in cfg.sections():
            #print('for process: ' + process) # 不同的section,比如nginx / mysql
            monitorProcess(process, cfg, stateDict[process])
        time.sleep(INTERVAL)
    logger.info('end monitoring')
    sys.exit(0)
