#!/bin/bash

{
cd run
ln -sf ../build/pid .
./pid 0
}
