#!/bin/bash
PROTO_SRC_PATH	="./ProtoSrc/"
PROTO_DEST_PATH	="./ProtoDest/"

protoc -I=$PROTO_SRC_PATH --cpp_out=$PROTO_DEST_PATH PROTO_SRC_PATH*.ptoto

echo"Protocol Convert Succeed!!!"


