import re
import psycopg2

# Establish a connection to your PostgreSQL database
conn = psycopg2.connect(database="fleet", user="vajra", password="admin", host="localhost", port="5432")
cursor = conn.cursor()

# Extract information from the components and insert into the table
with open('/home/devesh/vajra-fleet/event_rules/plugin_main.cpp', 'r') as file:
    content = file.read()

    matches = re.findall(r'event_rules\.emplace_back\(.*?\);', content, re.DOTALL)

    for match in matches:
        technique = re.search(r'std::make_tuple\("(.*?)",', match).group(1)
        tactic_match = re.search(r'"([^"]+)",\s*"T\d+\.\d+",', match)
        tactic = tactic_match.group(1) if tactic_match else None
        technique_match = re.search(r'"(T\d+\.\d+)"', match)
        technique_id = technique_match.group(1) if technique_match else None
        link_match = re.search(r'"(https?://[^"]+)"', match)
        link = link_match.group(1) if link_match else None
        severity_match = re.search(r'SeverityLevel::(.*?),', match)
        severity = severity_match.group(1) if severity_match else None
        os_match = re.search(r'"(Linux|Windows)"', match)
        os = os_match.group(1) if os_match else None

        # Insert the extracted information into the table
        cursor.execute("""
            INSERT INTO rules (technique, technique_id, tactic, link, severity, os_name)
            VALUES (%s, %s, %s, %s, %s, %s);
        """, (technique, technique_id, tactic, link, severity, os))

# Commit the changes and close the database connection
conn.commit()
conn.close()