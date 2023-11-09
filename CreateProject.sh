#!/bin/bash

echo "Enter your project name: "
read project_name
mkdir $project_name
cp -r ./GameProject/* ./$project_name && rm -R ./GameProject/
sed -i "s/GameProject/${project_name}/g" premake5.lua
