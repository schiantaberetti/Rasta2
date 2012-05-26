#!/bin/bash
echo 'Inserto the name of the file: '
read filename
convert $(filename) -alpha off $(filename).jpg
