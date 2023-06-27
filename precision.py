input_file = "/home/nikolavlaskalin/Desktop/dsp2-projekat1-2023/txt_outputs/precision_output.txt"

precision_input = open(input_file, 'r')
line = precision_input.readline()
precision_values = [float(value) for value in line.split(" ") if value != '']
precision_input.close()

precision_input = open(input_file, 'a')
total_sum = sum(precision_values)
final_precision = total_sum / len(precision_values)
precision_input.write("\n")
precision_input.write("Precision of the whole set is " + str(final_precision))
precision_input.close()

print(final_precision)