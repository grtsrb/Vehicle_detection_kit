#!/bin/bash

image_dir="$1"
myLabel_dir="$2"
modelLabel_dir="$3"

for file in "$image_dir"/*.jpg; do
    filename=$(basename "$file" .jpg)
    input_img="$image_dir/$filename.jpg"
    echo "$input_img"
    input_myLabel="$myLabel_dir/$filename.txt"
    echo "$input_myLabel"
    input_modelLabel="$modelLabel_dir/$filename.txt"
    echo "$input_modelLabel"

    python project.py "$input_img" "$input_myLabel" "$input_modelLabel"
done


python precision.py

# bash start_precision_calculation.sh /home/nikolavlaskalin/Desktop/dsp2test/test/images /home/nikolavlaskalin/Desktop/dsp2test/test/labels /home/nikolavlaskalin/Desktop/dsp2test/runs/predict/cars_s9/labels
