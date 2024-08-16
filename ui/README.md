# Vajra-UI

Vajra-UI is renamed repository from VajraSystemapp. Now we are going to maintain Server and Client code in one repository so that in future if we need to containerised it then it will be helpful.

# Setup and Run Guide

This guide will walk you through the steps to clone, set up, and run the Vajra-UI application on your local machine. Vajra-UI is a web application with both client and server components.

# Short Version

### Clone repository
```bash
git clone https://github.com/VajraSecurity/Vajra-UI.git
cd Vajra-UI
```
### Install Node
```bash
sudo bash node_setup.sh
```
### Postgres Changes
```bash
sudo -i -u postgres
psql
CREATE DATABASE nodejs_users;
\q
exit
```
### Website Setup
```
sudo bash website_setup.sh
```
### Create Default User (Terminal 2)
```bash
curl -X POST -H "Content-Type: application/json" --data '{"username": "admin", "password": "admin", "passwordConfirm": "admin", "email": "admin@gmail.com", "name": "Admin", "role": "ADMINISTRATOR"}' http://localhost:4000/api/auth/register
```
### Production Environment without Docker:
Install Nginx

```bash
  sudo apt install nginx
```

Create directory which Nginx will serve

```bash
  sudo mkdir /var/www/<Public IP Address>
```

Change directory permissions

```bash
  sudo chmod 755 -R /var/www/<Public IP Address>
```

Change owner of the directory

```bash
  sudo chown -R <Current User>:www-data /var/www/<Public IP Address>
```

Create configuration file for nginx

```bash
  sudo nano /etc/nginx/sites-available/<Public IP Address>
```

Use nginx_prod.conf file to create nginx configuration

```bash
  sudo nano /etc/nginx/sites-available/<Public IP Address>
```

Check nginx configuration

```bash
  sudo nginx -t
```

Unlink the default config, this step is optional and should only considered when there exist default config

```bash
  sudo unlink /etc/nginx/sites-enabled/default
```

Create link for our nginx configuration and make it enabled for nginx, check the config again after this step

```bash
  sudo ln -s /etc/nginx/sites-available/<Public IP Address> /etc/nginx/sites-enabled
```

Copy all the contains of build file to /var/www/\<Public IP Address\> and restart Nginx.

<!-- ## Production Environment

To run the project in production environment

### `docker-compose up`

This will start the production build of Vajra-FrontEnd
### `docker-compose -f docker-compose.prod.yml build`

This command will build the app and create a image to make a container.

### `docker run -p 80:80 --name vajra-app vajra-frontend`

This will start the application on the current IP on post 80 -->


That's it! ✨

# Detailed Version
### Prerequisites

Before you begin, ensure you have the following installed on your system:

- Git
- Node (v18+)
    - (with npm v10+)

### Node
Here are the steps to install node if not present on machine already-

#### Steps to install npm and node:
```
sudo apt-get update
sudo apt-get install -y npm
sudo npm install -g n
sudo n lts
```
Version should be Node v18+
```
node -v
```

This may give output as npm v8+
```
npm -v
```

We update npm version as follows:
```
sudo npm install -g npm@latest
npm -v
```
This should give version as npm v10+

PS: Open a new terminal and check version if not updated

### Clone the Repository

```bash
git clone https://github.com/VajraSecurity/Vajra-UI.git
cd Vajra-UI
git checkout Vajra-UI
```

### Client Setup (Terminal 1)

1. Make a copy of `.env.production.local` and rename it to `.env.development.local` if not present.

2. Edit the `.env.development.local` file and set the `REACT_APP_SERVER_URL` to `"http://localhost:4000/api"`.

3. Install client dependencies and start the client server.
```bash
cd Client
npm cache clean --force
npm install
npm run build
npm run start
```

### Server Setup (Terminal 2)

1. Create a DB named 'nodejs_users' in psql DB.
```
CREATE DATABASE nodejs_users;
```
2. Edit the `.env` file in the Server directory to configure the settings for DB1 (Users Auth) and DB2 (Fleet DB).

```bash
# Local DB 1
PG_HOST=localhost
PG_PORT=5432
POSTGRES_USER=XXXX
POSTGRES_PASSWORD=XXXX
POSTGRES_DB=nodejs_users

# Local DB 2
PG_HOST1=localhost
PG_PORT1=5432
POSTGRES_USER1=XXXX
POSTGRES_PASSWORD1=XXXX
POSTGRES_DB1=fleet
```
3. Install server dependencies and start the server.

```bash
cd Server
npm install
npm run build
npm run start
```

4. Add a user to the DB 1 for accessing frontend by using `/api/auth/register` endpoint as follows:

```bash
curl -X POST -H "Content-Type: application/json" --data '{"username":"admin","password":"admin","passwordConfirm": "admin","email":"admin@gmail.com","name":"Admin","role":"ADMINISTRATOR"}' http://localhost:4000/api/auth/register
```

5. Use credentials for login in website:
```
username: admin
password: admin
```
## Running Locally

With both the client and server set up, you can access the Vajra-UI application locally in your web browser at `http://localhost:8001` and API server at `http://localhost:4000`

## Running Backend as a Service

Here are the steps to create a backend service for Vajra UI using systemd.

1. Open a terminal and navigate to the `/usr/bin` directory:
   ```bash
   cd /usr/bin
   ```

2. Create a new script file for your Vajra UI backend service:
   ```bash
   sudo nano vajra_ui_backend.sh
   ```

3. In the nano text editor, add the following content to the `vajra_ui_backend.sh` script:

   ```bash
   #!/bin/bash
   cd /<PATH to server>/Server
   npm run build
   HOST=0.0.0.0 npm run start
   ```

   Make sure to replace `/<PATH to server>/` with the actual path to your server directory.

4. Save the file and exit the text editor.

5. Now, create a systemd service unit file in the `/usr/lib/systemd/system` directory:
   ```bash
   cd /usr/lib/systemd/system
   ```

6. Create a new service unit file for your Vajra UI backend service:
   ```bash
   sudo nano vajra_ui_backend.service
   ```

7. In the nano text editor, add the following content to the `vajra_ui_backend.service` file:

   ```ini
   [Unit]
   Description=Vajra UI Backend service

   [Service]
   Type=simple
   ExecStart=/bin/bash /usr/bin/vajra_ui_backend.sh
   Restart=always
   RestartSec=3

   [Install]
   WantedBy=multi-user.target
   ```

   This unit file defines the service description, the command to start your backend service, and the restart behavior.

8. Save the file and exit the text editor.

You have now created a systemd service for your Vajra UI backend. You can enable and start the service using the following commands:

```bash
sudo systemctl enable vajra_ui_backend.service
sudo systemctl start vajra_ui_backend.service
```

9. You can check the status for your Vajra Service like so:
```bash
sudo systemctl status vajra_ui_backend.service
```

The service will automatically start on boot, and you can manage it using systemd commands such as `systemctl`.


# Website Update Procedure

To update the Vajra-UI frontend and backend, follow these steps:

### Step 1: Make Changes in Vajra-UI Code

Make the necessary changes in the Vajra-UI code on the 'Vajra-UI-dev' branch. Ensure that all changes are thoroughly tested and meet the required standards.

```bash
# Switch to the 'Vajra-UI-dev' branch
git checkout Vajra-UI-dev
# Make changes and commit them
# Create a pull request from 'Vajra-UI-dev' into the 'Vajra-UI' branch
# Get the PR reviewed and merge it
```

### Step 2: Pull Changes on getvajra.com Server

SSH into the Getvajra server using the provided credentials:

```bash
ssh vajra@182.57.16.37
```

Navigate to the Vajra-UI directory and pull the latest changes from the 'Vajra-UI' branch:

```bash
cd Website/Vajra-UI
git pull origin Vajra-UI
```

### Step 3: Build and Deploy the Updated Frontend

Navigate to the Client directory and build the project:

```bash
cd Client
npm run build
```

Navigate to the build directory:

```bash
cd build
```

Copy the build files to the web server directory:

```bash
sudo cp -r * /var/www/182.57.16.37/
```

### Step 4: Restart Nginx and Vajra-UI Backend Service

Restart the Nginx and Vajra-UI backend services to apply the changes:

```bash
sudo systemctl restart nginx.service
sudo systemctl restart vajra_ui_backend.service
```

### Step 5: Check status of Nginx and Vajra-UI Backend Service

Check the status of Nginx and Vajra-UI backend services after applying the changes.

```bash
sudo systemctl status nginx.service
sudo systemctl status vajra_ui_backend.service
```
These steps ensure a smooth update of the Vajra-UI frontend and backend on the getvajra.com server.

---


