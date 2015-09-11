#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


#include "matrix.h"


#define MAX_CMD_COUNT 50

/*protected functions*/
void load_matrix (Matrix_t* m, unsigned int* data);

/* 
 * PURPOSE: instantiates a new matrix with the passed name, rows, cols 
 * INPUTS: 
 *	name the name of the matrix limited to 50 characters 
 *  rows the number of rows the matrix
 *  cols the number of cols the matrix
 * RETURN:
 *  If no errors occurred during instantiation then true
 *  else false for an error in the process.
 *
 **/

bool create_matrix (Matrix_t** new_matrix, const char* name, const unsigned int rows,
						const unsigned int cols) {

	// ERROR CHECK INCOMING PARAMETERS
	// *new_matrix no limit, can be null or valid
	if (strlen(name) > 50){
		printf("the name of matrix is too long");
		return false;
	}
	if (rows>4294967295){
		printf("row is too long");
		return false;
	}
	if (cols> 4294967295){
		printf("col is too long");
		return false;
	}

	*new_matrix = calloc(1,sizeof(Matrix_t));
	if (!(*new_matrix)) {
		return false;
	}
	(*new_matrix)->data = calloc(rows * cols,sizeof(unsigned int));
	if (!(*new_matrix)->data) {
		return false;
	}
	(*new_matrix)->rows = rows;
	(*new_matrix)->cols = cols;
	unsigned int len = strlen(name) + 1; 
	if (len > MATRIX_NAME_LEN) {
		return false;
	}
	strncpy((*new_matrix)->name,name,len);
	return true;

}

/* 
 * PURPOSE: realease memory of matrix 
 * INPUTS: 
 *	m : matrix need to be realeased
 * RETURN:
 *  print some information
 **/

void destroy_matrix (Matrix_t** m) {

	// ERROR CHECK INCOMING PARAMETERS
	if ((*m) == NULL){
		printf("no matrix to be realeased");
		return;
	}
	free((*m)->data);
	free(*m);
	*m = NULL;
}


	
/* 
 * PURPOSE: decide if two matrix is equal; 
 * INPUTS: 
 *	a : A matrix; 
 *  b : B matrix
 * RETURN:
 *  If no errors occurred then true
 *  else false for an error in the process.
 *
 **/
bool equal_matrices (Matrix_t* a, Matrix_t* b) {

	// ERROR CHECK INCOMING PARAMETERS
	if (a == NULL){
		printf("no A matrix");
		return false;
	}
	if (b == NULL){
		printf("no B matrix");
		return false;
	}
	
	if (!a || !b || !a->data || !b->data) {
		return false;	
	}

	int result = memcmp(a->data,b->data, sizeof(unsigned int) * a->rows * a->cols);
	if (result == 0) {
		return true;
	}
	return false;
}

/* 
 * PURPOSE: copy old matrix to new matrix; 
 * INPUTS: 
 *	src : old matrix; 
 *  dest : new matrix
 * RETURN:
 *  If no errors occurred then true
 *  else false for an error in the process.
 *
 **/
bool duplicate_matrix (Matrix_t* src, Matrix_t* dest) {


	//ERROR CHECK INCOMING PARAMETERS
	if (src == NULL){
		printf("no old matrix");
		return false;
	}
	if (dest == NULL){
		printf("no new matrix");
		return false;
	}

	if (!src) {
		return false;
	}
	/*
	 * copy over data
	 */
	unsigned int bytesToCopy = sizeof(unsigned int) * src->rows * src->cols;
	memcpy(src->data,dest->data, bytesToCopy);	
	return equal_matrices (src,dest);
}

/* 
 * PURPOSE: doing bitwise shift for matrix; 
 * INPUTS: 
 *	a : matrix need to be executed;
 *  direction : which direction need to be shifted;
 *  shift : shift value;
 * RETURN:
 *  If no errors occurred then true
 *  else false for an error in the process.
 *
 **/
bool bitwise_shift_matrix (Matrix_t* a, char direction, unsigned int shift) {
	
	//ERROR CHECK INCOMING PARAMETERS
	if (a == NULL){
		printf("no matrix in bitwise");
		return false;
	}

	if (shift > 4294967295){
		printf("too large shift");
		return false;
	}
	if (direction == ' '){
		printf("empty character");
		return false;
	}
	if (!a) {
		return false;
	}

	if (direction == 'l') {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] << shift;
			}
		}

	}
	else {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] >> shift;
			}
		}
	}
	
	return true;
}

/* 
 * PURPOSE: combine two matrix into one new matrix 
 * INPUTS: 
 *	a : A matrix;
 *  b : B matrix;
 *  c : new matrix;
 * RETURN:
 *  If no errors occurred then true
 *  else false for an error in the process.
 *
 **/
bool add_matrices (Matrix_t* a, Matrix_t* b, Matrix_t* c) {

	// ERROR CHECK INCOMING PARAMETERS
	if (a == NULL){
		printf("A matrix is missing");
		return false;
	}
	if (b == NULL){
		printf("B matrix is missing");
		return false;
	}
	if (c == NULL){
		printf("new matrix is missing");
		return false;
	}

	if (a->rows != b->rows && a->cols != b->cols) {
		return false;
	}

	for (int i = 0; i < a->rows; ++i) {
		for (int j = 0; j < b->cols; ++j) {
			c->data[i * a->cols +j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
		}
	}
	return true;
}

/* 
 * PURPOSE: display matrix  
 * INPUTS: 
 *	m : matrix need to be displayed
 * RETURN:
 *  If no errors occurred then true
 *  else false for an error in the process.
 *
 **/
void display_matrix (Matrix_t* m) {
	
	// ERROR CHECK INCOMING PARAMETERS
	if (m == NULL){
		printf("no matrix input to display");
		return;
	}
	printf("\nMatrix Contents (%s):\n", m->name);
	printf("DIM = (%u,%u)\n", m->rows, m->cols);
	for (int i = 0; i < m->rows; ++i) {
		for (int j = 0; j < m->cols; ++j) {
			printf("%u ", m->data[i * m->cols + j]);
		}
		printf("\n");
	}
	printf("\n");

}

/* 
 * PURPOSE: read matrix from input file name; 
 * INPUTS: 
 *	matrix_input_filename : input file name need to be read;
 *  m : matrix need to be read;
 * RETURN:
 *  If no errors occurred then true
 *  else false for an error in the process.
 *
 **/
bool read_matrix (const char* matrix_input_filename, Matrix_t** m) {
	
	// ERROR CHECK INCOMING PARAMETERS
	if (matrix_input_filename == NULL){
		printf("no matrix input file");
		return false;
	}
	if (*m == NULL){
		printf("no matrix will input");
		return false;
	}


	int fd = open(matrix_input_filename,O_RDONLY);
	if (fd < 0) {
		printf("FAILED TO OPEN FOR READING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}

	/*read the wrote dimensions and name length*/
	unsigned int name_len = 0;
	unsigned int rows = 0;
	unsigned int cols = 0;
	
	if (read(fd,&name_len,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READING FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	char name_buffer[50];
	if (read (fd,name_buffer,sizeof(char) * name_len) != sizeof(char) * name_len) {
		printf("FAILED TO READ MATRIX NAME\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;	
	}

	if (read (fd,&rows, sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX ROW SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (read(fd,&cols,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX COLUMN SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	unsigned int numberOfDataBytes = rows * cols * sizeof(unsigned int);
	unsigned int *data = calloc(rows * cols, sizeof(unsigned int));
	if (read(fd,data,numberOfDataBytes) != numberOfDataBytes) {
		printf("FAILED TO READ MATRIX DATA\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;	
	}

	if (!create_matrix(m,name_buffer,rows,cols)) {
		return false;
	}

	load_matrix(*m,data);

	free(data);

	if (close(fd)) {
		return false;

	}
	return true;
}

/* 
 * PURPOSE: output matrix  
 * INPUTS: 
 *	matrix_output_filename: outpur filename; 
 *  m: matrix need to be written;
 * RETURN:
 *  If no errors occurred during instantiation then true
 *  else false for an error in the process.
 *
 **/
bool write_matrix (const char* matrix_output_filename, Matrix_t* m) {
	
	// ERROR CHECK INCOMING PARAMETERS
	if (matrix_output_filename == NULL){
		printf("the output file");
		return false;
	}
	if (m == NULL){
		printf("the matrix to be written");
		return false;
	}

	int fd = open (matrix_output_filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	/* ERROR HANDLING USING errorno*/
	if (fd < 0) {
		printf("FAILED TO CREATE/OPEN FILE FOR WRITING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXISTS\n");
		}
		return false;
	}
	/* Calculate the needed buffer for our matrix */
	unsigned int name_len = strlen(m->name) + 1;
	unsigned int numberOfBytes = sizeof(unsigned int) + (sizeof(unsigned int)  * 2) + name_len + sizeof(unsigned int) * m->rows * m->cols + 1;
	/* Allocate the output_buffer in bytes
	 * IMPORTANT TO UNDERSTAND THIS WAY OF MOVING MEMORY
	 */
	unsigned char* output_buffer = calloc(numberOfBytes,sizeof(unsigned char));
	unsigned int offset = 0;
	memcpy(&output_buffer[offset], &name_len, sizeof(unsigned int)); // IMPORTANT C FUNCTION TO KNOW
	offset += sizeof(unsigned int);	
	memcpy(&output_buffer[offset], m->name,name_len);
	offset += name_len;
	memcpy(&output_buffer[offset],&m->rows,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&output_buffer[offset],&m->cols,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy (&output_buffer[offset],m->data,m->rows * m->cols * sizeof(unsigned int));
	offset += (m->rows * m->cols * sizeof(unsigned int));
	output_buffer[numberOfBytes - 1] = EOF;

	if (write(fd,output_buffer,numberOfBytes) != numberOfBytes) {
		printf("FAILED TO WRITE MATRIX TO FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	
	if (close(fd)) {
		return false;
	}
	free(output_buffer);

	return true;
}

/* 
 * PURPOSE: generate random matrix based on start_range and end_range; 
 * INPUTS: 
 *	m: matrix; 
 *  start_range: minimum of value in matrix;
 *  end_range: maximum of value in matrix
 * RETURN:
 *  If no errors occurred during instantiation then true
 *  else false for an error in the process.
 *
 **/
bool random_matrix(Matrix_t* m, unsigned int start_range, unsigned int end_range) {
	
	// ERROR CHECK INCOMING PARAMETERS
	if (m == NULL){
		printf("no input matrix in random part");
		return false;
	}
	if (start_range > 4294967295){
		printf("too large for start range");
		return false;
	}
	if (end_range > 4294967295){
		printf("too large for end range");
		return false;
	}
	for (unsigned int i = 0; i < m->rows; ++i) {
		for (unsigned int j = 0; j < m->cols; ++j) {
			m->data[i * m->cols + j] = rand() % (end_range + 1 - start_range)+ start_range;
		}
	}
	return true;
}

/*Protected Functions in C*/

/* 
 * PURPOSE: load matrix by copying address of matrix into data
 * INPUTS: 
 *	m: matrix; 
 *  data : new data;
 * RETURN:
 *  print some information
 *
 **/
void load_matrix (Matrix_t* m, unsigned int* data) {
	
	// ERROR CHECK INCOMING PARAMETERS
	if (m ==NULL){
		printf("no matrix");
		return;
	}
	if (data == NULL){
		printf("no dataset");
		return;
	}
	memcpy(m->data,data,m->rows * m->cols * sizeof(unsigned int));
}

/* 
 * PURPOSE: give the information of new_matrix to mats 
 * INPUTS: 
 *	mats: matrix; 
 *  new_matrix: matrix information
 *  num_mats: the number of matrix
 * RETURN:
 *  If no errors occurred during instantiation then value of position
 *  else -1 for an error in the process.
 *
 **/
unsigned int add_matrix_to_array (Matrix_t** mats, Matrix_t* new_matrix, unsigned int num_mats) {
	
	// ERROR CHECK INCOMING PARAMETERS
	// no limit on mats
	if (new_matrix == NULL){
		printf("no matrix information has been built");
		return -1;
	}
	if (num_mats > 4294967295){
		printf("too long number to parameter");
		return -1;
	}
	static long int current_position = 0;
	const long int pos = current_position % num_mats;
	if ( mats[pos] ) {
		destroy_matrix(&mats[pos]);
	} 
	mats[pos] = new_matrix;
	current_position++;
	return pos;
}
