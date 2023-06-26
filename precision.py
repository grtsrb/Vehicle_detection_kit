input_file = "/home/nikolavlaskalin/Desktop/dsp2test/precision_output.txt"

precision_input = open(input_file, 'r')
line = precision_input.readline()
precision_values = [float(value) for value in line.split(" ") if value != '']

print(line)
total_sum = sum(precision_values)
final_precision = total_sum / len(precision_values)

print(final_precision)