# shellcheck disable=SC1113
# /usr/bin/sh
sudo docker rm -f mysql
sudo docker run --name mysql -p 3306:3306 -p 33060:33060 -e MYSQL_ROOT_PASSWORD=123456 -d daocloud.io/mysql:8.0.19