#!/bin/bash

# Check if the correct number of arguments is provided
if [ $# -ne 2 ]; then
  echo "Usage: $0 <number_of_lines> <output_file>"
  exit 1
fi

# Read arguments
num_lines=$1
output_file=$2

# Ensure num_lines is a positive integer
if ! [[ "$num_lines" =~ ^[0-9]+$ ]] || [ "$num_lines" -le 0 ]; then
  echo "Error: <number_of_lines> must be a positive integer."
  exit 1
fi

# Clear the file before writing
>"$output_file"

# Write the specified number of lines
for ((i = 1; i <= num_lines; i++)); do
  echo "This is line $i" >>"$output_file"
done

echo "$num_lines lines written to $output_file"
