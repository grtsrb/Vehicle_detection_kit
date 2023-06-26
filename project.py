import cv2



x1_myLabel = []
y1_myLabel = []
x2_myLabel = []
y2_myLabel = []

x1_modelLabel = []
y1_modelLabel = []
x2_modelLabel = []
y2_modelLabel = []

def getImageSize(image):
    im = cv2.imread(image)
    h, w, c = im.shape

    return h, w, c

def readCoordinates(file, myFile):
    file1 = open(file, 'r')
    numOfLines = 0

    for line in file1:
        numOfLines += 1
        # 0 0.8675 0.12 0.24 0.24
        coordinates = line.split(" ")
        if myFile is True:
            x1_myLabel.append((float(coordinates[1]) - float(coordinates[3])/2)*w)
            y1_myLabel.append((float(coordinates[2]) - float(coordinates[4])/2)*h)
            x2_myLabel.append((float(coordinates[1]) + float(coordinates[3])/2)*h)
            y2_myLabel.append((float(coordinates[2]) + float(coordinates[4])/2)*h)
        else:
            x1_modelLabel.append((float(coordinates[1]) - float(coordinates[3])/2)*w)
            y1_modelLabel.append((float(coordinates[2]) - float(coordinates[4])/2)*h)
            x2_modelLabel.append((float(coordinates[1]) + float(coordinates[3])/2)*h)
            y2_modelLabel.append((float(coordinates[2]) + float(coordinates[4])/2)*h)

def calculate_iou():
    x1 = max(x1_myLabel[0], x1_modelLabel[0])
    y1 = max(y1_myLabel[0], y1_modelLabel[0])
    x2 = min(x2_myLabel[0], x2_modelLabel[0])
    y2 = min(y2_myLabel[0], y2_modelLabel[0])

    intersection_area = max(0, x2 - x1 + 1) * max(0, y2 - y1 + 1)

    box1_area = (x2_myLabel[0] - x1_myLabel[0] + 1) * (y2_myLabel[0] - y1_myLabel[0] + 1)
    box2_area = (x2_modelLabel[0] - x1_modelLabel[0] + 1) * (y2_modelLabel[0] - y1_modelLabel[0] + 1)

    union_area = box1_area + box2_area - intersection_area

    iou = intersection_area / float(union_area)

    return iou

imageLocation = "/home/nikolavlaskalin/Desktop/dsp2test/justone/00613.jpg"
myLabel = "/home/nikolavlaskalin/Desktop/dsp2test/justone/labels/00613.txt"

modelLabel = "/home/nikolavlaskalin/Desktop/dsp2test/runs/predict/cars_one_s/labels/00613.txt"
h, w, c = getImageSize(imageLocation)
readCoordinates(myLabel, True)
readCoordinates(modelLabel, False)
print(h,w,c)

print(x1_myLabel, x2_myLabel, y1_myLabel, y2_myLabel)
print(x1_modelLabel, x2_modelLabel, y1_modelLabel, y2_modelLabel)

iou = calculate_iou()
print(iou)