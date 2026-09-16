#!/bin/bash
# Copies the blocksds headers so they can be added to the include path for intellisense
rm -rf ./blocksds-headers
container_id=$(docker create skylyrac/blocksds:slim-latest)
docker cp "$container_id":/opt/wonderful/thirdparty/blocksds ./blocksds-headers
docker rm "$container_id"