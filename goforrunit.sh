#!/bin/bash

# To create and run the modbus container :)
docker run (-it) --name libmodbuscontainer libmodbusimage:0.1.0

# To copy a file form a container to the host local directory
docker cp libmodbuscontainer:'/tmp/imafile' ./  