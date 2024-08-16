install_node_and_npm() {
	sudo apt-get update
	sudo apt-get install -y npm
	sudo npm install -g n
	sudo n lts
        node_version=$(node -v)
	echo "Installed Node Version:" $node_version
        sudo npm install -g npm@latest
	npm_version=$(npm -v)
	echo "Installed NPM Version:" $npm_version
}

install_node_and_npm