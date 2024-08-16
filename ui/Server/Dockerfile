FROM node:19-alpine as build

WORKDIR /app

COPY ./package.json .
COPY ./package-lock.json .

RUN npm install && npm cache clean --force

COPY . .

RUN rm .env

RUN npm run build

FROM node:19-alpine

WORKDIR /app

COPY --from=build /app/dist ./dist

COPY --from=build /app/package*.json ./

RUN mkdir -p log

RUN npm install && npm cache clean --force
