from django import template
import datetime
import ast
register = template.Library()


def print_timestamp(timestamp):
    try:
        ts = float(timestamp)
    except ValueError:
        return None
    return datetime.datetime.fromtimestamp(ts)


def sectohr(sec):
    sec = int(sec)
    time=str(datetime.timedelta(seconds=sec))
    try:
        temp=time.split(' ')
        days=temp[0]
        temp2=str(temp[2])
        temp=temp2.split(':')
    except:
        temp=time.split(':')
    hr=temp[0]
    mins=temp[1]
    secs=temp[2]
    # print(f'{days} days {hr} hr {mins} min {secs} sec ago')
    time=str(days)+" days "+str(hr)+" hrs "+str(mins)+" mins "+str(secs)+" sec ago"
    if int(str(days).find(':'))>0 :
        time=str(hr)+" hrs "+str(mins)+" mins "+str(secs)+" sec ago"
    elif str(hr)=='0' :
        time=str(mins)+" mins "+str(secs)+" sec ago"
    return time


def to_list(value):
    value=value.split(',')
    return value

def to_dict(value):
    data=ast.literal_eval(value)
    return data

def display(value):
    data=0
    if "offline" in value:
        data=value["offline"]
    return data;


register.filter(print_timestamp)
register.filter(sectohr)
register.filter(to_list)
register.filter(to_dict)
register.filter(display)