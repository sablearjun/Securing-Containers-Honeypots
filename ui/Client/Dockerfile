# npm setup
FROM node:19-alpine as build

WORKDIR /app

COPY package*.json ./
RUN npm install --frozen-lockfile

COPY . ./

RUN rm .env.development.local .env.production.local

RUN mv .env.docker .env

RUN yarn build

# nginx
FROM nginx

# copy nginx.conf
COPY nginx_dev.conf /etc/nginx/conf.d/default.conf

# copy build foler contents
COPY --from=build /app/build /usr/share/nginx/html

# # copy SSL certificates
# COPY --from=certbot /etc/letsencrypt/live/example.com/fullchain.pem /etc/nginx/certs/fullchain.pem
# COPY --from=certbot /etc/letsencrypt/live/example.com/privkey.pem /etc/nginx/certs/privkey.pem