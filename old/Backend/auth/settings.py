from pathlib import Path
from requests.structures import CaseInsensitiveDict
import os
import socket

# Build paths inside the project like this: BASE_DIR / 'subdir'.
BASE_DIR = Path(__file__).resolve().parent.parent


# Quick-start development settings - unsuitable for production
# See https://docs.djangoproject.com/en/3.2/howto/deployment/checklist/

# SECURITY WARNING: keep the secret key used in production secret!
SECRET_KEY = 'django-insecure-y^_l*6%x#xeqvo6qh@*3v&j=92$f%p0lx39f23_!#e=p)21=kv'
#ALLOWED_HOSTS = ['172.105.252.33','127.0.0.1','localhost',]
# SECURITY WARNING: don't run with debug turned on in production!
DEBUG = True

ALLOWED_HOSTS = ['fleetmanager.getvajra.com',
                 '172.105.252.33', '127.0.0.1', 's3.ieor.iitb.ac.in',"10.119.31.19"]
CORS_ORIGIN_ALLOW_ALL = True
# CORS_ORIGIN_WHITELIST = [
#        'http://s3.ieor.iitb.ac.in:9001',
#        'http://s3.ieor.iitb.ac.in:1235',
#        'http://s3.ieor.iitb.ac.in:1235',
#        'http://s3.ieor.iitb.ac.in'
# ]

# Application definition

INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'rest_framework',
    'node',
    'config',
    'dashboard',
    'ad_hoc',
    'event',
    'corsheaders',
    'simple_history'
]

MIDDLEWARE = [
    'django.middleware.security.SecurityMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'corsheaders.middleware.CorsMiddleware',
    'django.middleware.common.CommonMiddleware',
    'django.middleware.csrf.CsrfViewMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',
    'simple_history.middleware.HistoryRequestMiddleware'
]

ROOT_URLCONF = 'auth.urls'

TEMPLATES = [
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': [],
        'APP_DIRS': True,
        'OPTIONS': {
            'context_processors': [
                'django.template.context_processors.debug',
                'django.template.context_processors.request',
                'django.contrib.auth.context_processors.auth',
                'django.contrib.messages.context_processors.messages',
            ],
        },
    },
]

WSGI_APPLICATION = 'auth.wsgi.application'


# Database
# https://docs.djangoproject.com/en/3.2/ref/settings/#databases

# DATABASES = {
#     'default': {
#         'ENGINE': 'django.db.backends.sqlite3',
#         'NAME': BASE_DIR / 'db.sqlite3',
#     }
# }


# Local
# DATABASES = {
#     'default': {
#         'ENGINE': 'django.db.backends.postgresql_psycopg2',
#         'NAME': 'systemapp',
#         # 'USER': 'postgres',
#         # 'PASSWORD': 'admin',
#         # 'HOST': 'localhost',
#         # 'PORT': '5432',
#     }
# }

# Server
DATABASES = {
   'default': {
       'ENGINE': 'django.db.backends.postgresql_psycopg2',
       'NAME': 'honeypotserver',
    #    'USER': 'vajra',
    #    'PASSWORD': 'admin',
    #    'HOST': 'localhost',
    #    'PORT': '5432',
   }
}


# Password validation
# https://docs.djangoproject.com/en/3.2/ref/settings/#auth-password-validators

AUTH_PASSWORD_VALIDATORS = [
    {
        'NAME': 'django.contrib.auth.password_validation.UserAttributeSimilarityValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.MinimumLengthValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.CommonPasswordValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.NumericPasswordValidator',
    },
]


# Internationalization
# https://docs.djangoproject.com/en/3.2/topics/i18n/

# LANGUAGE_CODE = 'en-us'

# TIME_ZONE = 'UTC'

# USE_I18N = True

# USE_L10N = True

# USE_TZ = True

LANGUAGE_CODE = 'en-us'

TIME_ZONE = 'UTC'

USE_I18N = True

USE_L10N = True

USE_TZ = False

# Server settings :

# Jwt Token verification block

inspector = CaseInsensitiveDict()
inspector["Accept"] = "application/json"
inspector["Authorization"] = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOiIxOTUyODczNzEyIiwiaWF0IjoiMTYzNzM0MjE2NCIsImlzcyI6Im1hbmFnZXIiLCJ0b2tlbiI6InF3ZXJ0eSIsInVzZXIiOiJrbmk5aHQifQ.myVa8hBifhC-LgE4rb-WfuEFFSVzAfFt7XGQ1xdkr1g"

# Jwt Token endblock
# API URL
url = 'https://s3.ieor.iitb.ac.in:1235/'


# Static files (CSS, JavaScript, Images)
# https://docs.djangoproject.com/en/3.2/howto/static-files/
#PROJECT_DIR = os.path.dirname(os.path.abspath(__file__))
STATIC_ROOT = '/home/fleet/vajra/auth'
STATIC_URL = '/static/'

# Default primary key field type
# https://docs.djangoproject.com/en/3.2/ref/settings/#default-auto-field

DEFAULT_AUTO_FIELD = 'django.db.models.BigAutoField'

# REST_FRAMEWORK = {
#     'DEFAULT_AUTHENTICATION_CLASSES': ('knox.auth.TokenAuthentication',),
# }

# REST_KNOX = {"TOKEN_TTL": None} # do not expire
