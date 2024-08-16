# Getting Started with Vajra Front-end

This project was bootstrapped with [Create React App](https://github.com/facebook/create-react-app).


## Production Environment without Docker

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

## Production Environment with Docker

To Start Front-end
```bash
  sudo docker-compose up -d
```

To Stop Front-end
```bash
  sudo docker-compose down
```
## Development Environment

### `npm start`

Runs the app in the development mode.\
Open [http://localhost:3000](http://localhost:3000) to view it in the browser.

The page will reload if you make edits.\
You will also see any lint errors in the console.

### `npm test`

Launches the test runner in the interactive watch mode.\
See the section about [running tests](https://facebook.github.io/create-react-app/docs/running-tests) for more information.

## Learn More

You can learn more in the [Create React App documentation](https://facebook.github.io/create-react-app/docs/getting-started).

To learn React, check out the [React documentation](https://reactjs.org/).
