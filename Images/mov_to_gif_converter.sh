#!/bin/bash

for i in *.mov; do
    ffmpeg -i "$i" -pix_fmt rgb24 ${i%.*}.gif;
done

# rm .*mov