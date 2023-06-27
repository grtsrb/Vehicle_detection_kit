import cv2
import numpy as np
import sys

x1_mylabel = []
y1_mylabel = []
x2_mylabel = []
y2_mylabel = []

x1_modelLabel = []
y1_modelLabel = []
x2_modelLabel = []
y2_modelLabel = []

def get_image_size(image):
    im = cv2.imread(image)
    h, w, c = im.shape

    return h, w, c

def read_coordinates(file, my_file):
    file1 = open(file, 'r')
    numOfLines = 0

    for line in file1:
        numOfLines += 1
        # 0 0.8675 0.12 0.24 0.24
        # 0    1    2    3    4
        coordinates = line.split(" ")
        if my_file is True:
            x1_mylabel.append((float(coordinates[1]) - float(coordinates[3])/2)*w)
            y1_mylabel.append((float(coordinates[2]) - float(coordinates[4])/2)*h)
            x2_mylabel.append((float(coordinates[1]) + float(coordinates[3])/2)*h)
            y2_mylabel.append((float(coordinates[2]) + float(coordinates[4])/2)*h)
        else:
            x1_modelLabel.append((float(coordinates[1]) - float(coordinates[3])/2)*w)
            y1_modelLabel.append((float(coordinates[2]) - float(coordinates[4])/2)*h)
            x2_modelLabel.append((float(coordinates[1]) + float(coordinates[3])/2)*h)
            y2_modelLabel.append((float(coordinates[2]) + float(coordinates[4])/2)*h)
    file1.close()
    return numOfLines

def calculate_iou(num_my_lines, num_model_lines, file_output):
    correct_label = np.zeros(num_model_lines, dtype=int)
    output_file = open(file_output, 'a')
    num_correct_labels = 0
    output_file.write("------------------------------------------------------------------------------\n")
    output_file.write("Doing image " + image_name + "\n")

    for i in range(0,num_my_lines):
        for j in range(0,num_model_lines):
            if correct_label[j] == 1:
                continue
            
            x1 = max(x1_mylabel[i], x1_modelLabel[j])
            y1 = max(y1_mylabel[i], y1_modelLabel[j])
            x2 = min(x2_mylabel[i], x2_modelLabel[j])
            y2 = min(y2_mylabel[i], y2_modelLabel[j])

            intersection_area = max(0, x2 - x1 + 1) * max(0, y2 - y1 + 1)

            box1_area = (x2_mylabel[i] - x1_mylabel[i] + 1) * (y2_mylabel[i] - y1_mylabel[i] + 1)
            box2_area = (x2_modelLabel[j] - x1_modelLabel[j] + 1) * (y2_modelLabel[j] - y1_modelLabel[j] + 1)

            union_area = box1_area + box2_area - intersection_area

            iou = intersection_area / float(union_area)
            if iou > 0.5:
                correct_label[j] = 1
                num_correct_labels += 1
                output_file.write("Model has correctly detected a car with IoU of " + str(iou) + "\n")

    output_file.write("Model has correctly detected total of " + str(num_correct_labels) + " cars.\n")
    output_file.write("\n")
    output_file.close()

    return num_correct_labels

def calculate_precision(correct_labels, model_lines, my_lines):
    if my_lines <= model_lines:
        precision = float(correct_labels / model_lines)
    else:
        precision = float(correct_labels / my_lines)    
        
    return precision



image_location = sys.argv[1]

my_label = sys.argv[2]
model_label = sys.argv[3]
image_name = sys.argv[4]

file_output = "/home/nikolavlaskalin/Desktop/dsp2-projekat1-2023/txt_outputs/output.txt"
precision_output = "/home/nikolavlaskalin/Desktop/dsp2-projekat1-2023/txt_outputs/precision_output.txt"

h, w, c = get_image_size(image_location)

myLines = read_coordinates(my_label, True)
modelLines = read_coordinates(model_label, False)
print("Image width is " + str(w) + ", Image height is " + str(h) + "\n")

print("Labeled images coordinates: \n")
print("x1 -> " + str(x1_mylabel) + "\n" +
      "y1 -> " + str(y1_mylabel) + "\n" +
      "x2 -> " + str(x2_mylabel) + "\n" +
      "y2 -> " + str(y2_mylabel) + "\n")

print("Detected images coordinates: \n")
print("x1 -> " + str(x1_modelLabel) + "\n" +
      "y1 -> " + str(y1_modelLabel) + "\n" +
      "x2 -> " + str(x2_modelLabel) + "\n" +
      "y2 -> " + str(y2_modelLabel) + "\n")

num_correct_labels = calculate_iou(myLines, modelLines, file_output)

precision = calculate_precision(num_correct_labels, modelLines, myLines)

output_file = open(file_output, 'a')
output_file.write("Precision of this image is at value " + str(precision) + "\n")
output_file.write("------------------------------------------------------------------------------\n")
output_file.write("\n")
output_file.close()

output_precision = open(precision_output, 'a')
output_precision.write(str(precision) + " ")
output_precision.close()

print("Precision of this image is " + str(precision))