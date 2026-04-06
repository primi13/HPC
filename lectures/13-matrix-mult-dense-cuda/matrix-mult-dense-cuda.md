# Dense Matrix Multiplication with CUDA

## Memory Allocation

- two representations: row-major and column-major

  <img src="figures/dense-matrix-representations.png" alt="Dense matrix representations" width="50%" />

- number of matrix elements ```rows``` $\times$ ```cols```
- 1D indexing
- 2D indexing

  - non-contiguous approach
    - allocate array of pointers to the beginning of rows
    - allocate array of row elements separately for each row
    - access to elements: ```m[row][col]```
    - there is no guarantee that rows will be adjacent to each other in memory!å

    ```C
    float **m = (float **)malloc(rows * sizeof(float *));
    for (int i = 0; i < rows; i++)
      m[i] = (float *)malloc(cols * sizeof(float));
    ...
    for (int i = 0; i < rows; i++)
      free(m[i]);
    free (m);
    ```

    <img src="figures/dense-matrix-allocation.png" alt="Dense matrix allocation" width="50%" />

  - contiguous memory approach
    - allocate an array of matrix elements (```mData```)
    - allocate array of pointers to beginning of rows (```m```)
    - access to elements: ```m[row][col]```
    - whole matrix data is stored in continuous memory space
    - a matrix can be easily transferred to a GPU or another compute node

    ```C
    float *mData = (float *)malloc(rows * cols * sizeof(float));
    float **m = (float **)malloc(rows * sizeof(float *));
    for (int i = 0; i < rows; i++)
      m[i] = &mData[i * cols];
    ...
    free (m);
    free (mData);
    ```

## Multiplication with GPU

### Straightforward approach

- each thread computes one scalar product of the resulting matrix (blue)
- global indexing of threads

  <img src="figures/dense-matrix-mult-sf.png" alt="Dense matrix multiplication" width="90%" />

- [mm0.cu](files/mm0.cu): straightforward approach

### Tiled approach

- each thread computes one element of output vector, same as above
- matrices are split to sub-matrices (tiles)
  - multiplication of sub-matrices follows the same pattern as multiplication of scalar elements
  - instead of scalars we are multiplying sub-matrices
- two phase approach
  - phase 1
    - coalesced reading of tiles (sub-matrices) from input matrices ```A``` and ```B```
    - all threads in a block simultaneously copy data from global to local memory
    - proper indexing of threads assures that whole warp is reads data from global memory in one transaction
  - phase 2
    - each thread calculates a dot product on a tile
    - matrix elements are read from local memory
    - partial scalar products are kept thread-wise
  - when all scalar product on a tile are completed, process continues with next tiles of matrices ```A``` and ```B```
- implementation
  
  <img src="figures/dense-matrix-mult-tiles.png" alt="Dense matrix multiplication with tiles" width="90%" />

- solutions
  - [mm1.cu](files/mm1.cu): tiled approach
  - [mm2.cu](files/mm2.cu): tiled approach with improper indexing of threads

