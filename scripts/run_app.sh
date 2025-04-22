#!/bin/bash

xhost +local:docker
docker run -it --rm --privileged \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v /dev/dri:/dev/dri \
  -e XDG_RUNTIME_DIR=/run/user/$(id -u) \
  wxopenglpng-dev