#!/bin/bash

#bash start_program.sh datasets/train/images datasets/out "Augmentation 4" 1.2 /home/nikolavlaskalin/Desktop/dsp2-projekat1-2023/datasets/train/labels 4 3 175 3

#Set input and output directory of images
input_dir="$1"
output_dir="$2"
#Set other arguments
exposure=$4
label_dir="$5"
asp1=$6
asp2=$7
histogram_argument=$8
num_of_occlusions=$9

#Set augmentation type {0, 1, 2, 3, 4, all)
augmentatio_type=$3

if [ "$3" = "Augmentation 0" ]
then
    augmentation_type="a0"
elif [ "$3" = "Augmentation 1" ]
then
    augmentation_type="a1"
elif [ "$3" = "Augmentation 2" ]
then
    augmentation_type="a2"
elif [ "$3" = "Augmentation 3" ]
then
    augmentation_type="a3"
elif [ "$3" = "Augmentation 4" ]
then
    augmentation_type="a4"
elif [ "$4" = "Augmentation all" ]
then
    augmentation_type="aa"
fi

#exposure start_x start_y end_x end y asp1 asp2 
for file in "$input_dir"/*.jpg; do
    echo "Doing file = $file"
    filename=$(basename "$file" .jpg)
    input_img="$input_dir/$filename.jpg"
    output_img=""$output_dir/$filename"_"$augmentation_type".jpg"
    label_file="$label_dir/$filename.txt"
        echo $output_img
    echo "Doing label file = $label_file"
    echo "Doing augmentation = $3"
    if [[ -f "$label_file" ]]; then
        while IFS=" " read -r label x_s y_s e_x e_y
        do
            x_start="$x_s"
            y_start="$y_s"
            end_x="$e_x"
            end_y="$e_y"
        done < "$label_file"
    else
        echo "Label file missing image $file"
    fi
    
    ./ImageDSP/build/linux/build-ImageDSP-Desktop-Debug/ImageDSP \
    --prog-name "$3" \
    --in-img "$input_img" \
    --out-img "$output_img" \
    --params "$exposure;$x_start;$y_start;$end_x;$end_y;$asp1;$asp2;$histogram_argument;$num_of_occlusions"
    echo "------------------------------------------------------------------------"
done

