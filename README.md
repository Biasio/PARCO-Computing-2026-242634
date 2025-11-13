# PARCO-Computing-2026-242634

## Usage
The "matrix.txt" is where the matrix is read from, it should contain each element separated by a white space character (also more than one, but just that character), and each line should be terminated by a newline char. If the specified matrix size to look for is smaller than the rows and columns within this file, the row elements are parsed till there's a '\n' char or the COLS_A is reached. If there is a '\n' then the remaining elements for that row are set to 0, otherwise if "COLS_A" is reached then the next row start where the previous was interrupted.
It can be randomly generated with a good sparsity degree precision (see RANDOMIZE_FILE and SPARSITY definitions in Functions.h). Otherwise if RANDOMIZE is set to 0 the file is only read.

## Folder ordering
All source code is inside the "src" folder.

Latex source and generated pdf is inside the "Latex" folder.

# Author

Developed by Alessandro Biasioli © 2025
