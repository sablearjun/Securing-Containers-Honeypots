import psycopg2

conn = psycopg2.connect(
        host="localhost",
        port = '5432',
        database="fleet",
        user="vajra",
        password="admin")

def Connect():
    return conn

def get_Rules(os_name):
    cur = conn.cursor()
    cur.execute("select * from python_rules where os_name = "+ "'"+os_name+"'")
    rules = cur.fetchall()
    cur.close()
    return rules