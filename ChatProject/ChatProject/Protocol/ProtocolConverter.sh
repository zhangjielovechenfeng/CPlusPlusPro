#!/bin/bash

PROTO_SRC_PATH="./ProtoSrc/"
PROTO_DEST_PATH="./ProtoDest/"

rm -rf $PROTO_DEST_PATH*

protoc -I=$PROTO_SRC_PATH --cpp_out=$PROTO_DEST_PATH $PROTO_SRC_PATH*.proto

echo "Protocol Convert Succeed!!!"


