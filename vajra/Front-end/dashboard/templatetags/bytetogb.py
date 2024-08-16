from django import template
from hurry.filesize import size
register = template.Library()


def byte_gb(ram):
    try:
        byte = int(ram)
    except ValueError:
        return None
    return size(byte)


register.filter(byte_gb)
