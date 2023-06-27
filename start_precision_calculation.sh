#!/bin/bash

image_dir="$1"
myLabel_dir="$2"
modelLabel_dir="$3"


for file in "$image_dir"/*.jpg; do
    filename=$(basename "$file" .jpg)
    input_img="$image_dir/$filename.jpg"
    echo "Doing image $filename.jpg"
    input_myLabel="$myLabel_dir/$filename.txt"
    input_modelLabel="$modelLabel_dir/$filename.txt"

    python project.py "$input_img" "$input_myLabel" "$input_modelLabel" "$filename.jpg"
    echo "--------------------------------------------------------"
done


python precision.py

# bash start_precision_calculation.sh /home/nikolavlaskalin/Desktop/dsp2-projekat1-2023/datasets/valid/images /home/nikolavlaskalin/Desktop/dsp2-projekat1-2023/datasets/valid/labels /home/nikolavlaskalin/Desktop/dsp2-projekat1-2023/runs/predict/cars_s10/labels
