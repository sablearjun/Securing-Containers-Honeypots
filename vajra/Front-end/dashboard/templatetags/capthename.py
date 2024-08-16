from django import template
register = template.Library()


def capit(name):
    try:
        name = str(name)
    except ValueError:
        return None
    return name.capitalize()


register.filter(capit)
