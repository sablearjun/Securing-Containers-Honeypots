setup_client() {
    echo -e '\nSetting up the frontend of the Website\n'
    cd Client
    npm cache clean --force
    sudo rm -rf node_modules
    npm install --save react react-dom react-scripts
    npm install
    npm run build
    echo -e '\nFrontend Setup done \n'
    echo -e '\nStarting the frontend of the Website....\n'
    npm run start
}

setup_server() {
    echo -e '\nSetting up the backend of the Website\n'

    # CREATING DATABASE 
    # sudo -i -u postgres
    # psql -h localhost -U postgres -d postgres -c "CREATE DATABASE nodejs_users;"
    # exit

    cd $1
    npm install
    npm run build
    echo -e '\nBackend Setup done\n'
}

create_systemd_service() {    
    local server_loc=$1
    echo -e 'Creating a service to run Backend server in background'
    cd /usr/bin
    echo '#!/bin/bash' > vajra_ui_backend.sh
    echo 'cd' $server_loc >> vajra_ui_backend.sh
    echo 'npm run build' >> vajra_ui_backend.sh
    echo 'HOST=0.0.0.0 npm run start' >> vajra_ui_backend.sh

    cd /usr/lib/systemd/system
    echo '[Unit]' > vajra_ui_backend.service
    echo 'Description=Vajra UI Backend service' >> vajra_ui_backend.service
    echo '' >> vajra_ui_backend.service
    echo '[Service]' >> vajra_ui_backend.service
    echo 'Type=simple' >> vajra_ui_backend.service
    echo 'ExecStart=/bin/bash /usr/bin/vajra_ui_backend.sh' >> vajra_ui_backend.service
    echo 'StandardOutput=append:/var/log/vajra_ui_backend.log' >> vajra_ui_backend.service
    echo 'StandardError=append:/var/log/vajra_ui_backend.log' >> vajra_ui_backend.service
    echo 'Restart=always' >> vajra_ui_backend.service
    echo 'RestartSec=3' >> vajra_ui_backend.service
    echo '' >> vajra_ui_backend.service
    echo '[Install]' >> vajra_ui_backend.service
    echo 'WantedBy=multi-user.target' >> vajra_ui_backend.service

    sudo systemctl enable vajra_ui_backend.service
    sudo systemctl start vajra_ui_backend.service
    echo -e '\nBackend Server Running at port 4000...\n'
}


path=$(pwd)
server_path=$path"/Server"
client_path=$path"/Client"
setup_server $server_path
create_systemd_service $server_path
setup_client $client_path