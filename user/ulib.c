#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"


//
// wrapper so that it's OK if main() does not call exit().
//
void
_main()
{
  extern int main();
  main();
  exit(0);
}

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
    cdst[i] = c;
  }
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}
 

/**
 * This function reads  a line of input from stdio into a buffer.
 * Reads up to a maximum num of chars  specified by variable max
 * and places results into buf.
 * We call ffgets() to read input safely, preventing buffer overflow.
 * It returns NULL if reading fails, if failure hasn't occured ;)  returns the buffer containing the input string.
 *
 * @param buf the buffer where the input string is  stored
 * @param max the maximum number of chars to read, including  null terminator
 * @return a pointer to the buffer containing the input string, or NULL if an error occurs
 */
char*
gets(char *buf, int max) 
{
  /*fgets to safely read input into buf w/  a maximum length of max
 	we want to use the fgets() function to read input from standrd input fd = 0 */
  int numOfBytes = fgets(0, buf, max); // variable to store number of bytes read.
  if (numOfBytes < 0) //If fgets() fails (less than 0)
  	return NULL; //return a NULL to show we have a problem, (yikes) if i get NULL my code wants to fight
  return buf; // welll if everything was a sucess fingers crossed we can return the buf w/ the input string.
}

/**
 * Reads chars from  given fd into buf,
 * until a newline character is encountered or up to a the max amount of chars.
 * The function stops reading when there is a  newline character,
 * max num of chars is read.
 * The buuffer is null terminated after reading.
 *
 * @param fd the file descriptor to read from
 * @param buf the buffer where the input string will be stored
 * @param max the maximum number of characters to read, including the null terminator
 * @return the number of characters read (excluding the null terminator), 
 *         or -1 if an error occurs during the read operation
 */

//fgets function
int fgets(int fd, char *buf, int max) {
	char c; // temp var to read input
	int i; //index to keep track of chars read

	// for loop to read chars (-1) leaving space for the null terminator
	for (i = 0; i+1 < max -1; i++) {
		int numOfBytes = read(fd, &c, 1); // read one char @ a time from fd

		if(numOfBytes <= 0) { // if there is an error or its the end of a file
			if (numOfBytes == -1) // if an error has happpened during reading bc we are using the read() function
				return -1;
			break; // if end of file the loop breaks
		}

		buf[i] = c; //store char in buffer

		if(c == '\n'){ //if we come across a newline // older version of windows newline char = \r\n
			i++; // it also is counted in char count
			break; //break loop after newline is read to include it in the buffer
		}
	}

	buf[i] = '\0'; //null-terminate string after reading
	return i; //returns number of chars read 
}



/**
 * REads input from fd into a buffer
 * Reads until newline or end of file.
 * Buffer is resized if the buffer gets full
 * Uses fgets to read from the fdr into the buffer. If memory allocation
 * fails or a read error occurs return -1. if it read it succesfully, it returns the number of
 * chars read.
 
 * @param buffer a pointer to a char pointer where  dynamically allocated buffer is  stored.
 * @param fd the file descriptor we read from
 * @return the num  of chars  read/ -1 if an error occurs
 */
int getline(char **buffer, int fd) {
	int size = 64; // starting buffer size
    int length = 0;  //length of strinf
    int numOfBytes; //number of bytes read

    // using malloc we allocate a buffer
    *buffer = (char *)malloc(size);
    if (*buffer == NULL) { //if the allocation doesnt work
        return -1;  //show  it failed by returing -1
    }

    while (1) { //loop to read until a newline or end of the file
        // Read using fgets
        numOfBytes = fgets(fd, *buffer + length, size - length); // reads data into buffer, starting point is end of the curr place we ended

        if (numOfBytes < 0) {  //if we have a reading error
            return -1;//return -1
        }

        length = length + numOfBytes;  //length of string updated

        // if we get a new line character or reach the end of the file we break/sttop reading
        if ((*buffer)[length - 1] == -1 || numOfBytes == 0) {
            break;
        }

        //if the buffer becomes full we have to make a new size
        if (length >= size - 1) {
            size *= 2;  //multiply buffer by 2 to make the size bigger
            char *newBuffer = (char *)malloc(size); // allocate more mem
            if (newBuffer == NULL) { // if allocating doesnt work return -1
                return -1;
            }

            memcpy(newBuffer, *buffer, length); //have to copy all the stuff in the old buffer to the new buffer
            free(*buffer); //free the old buffer since we got a new one
            *buffer = newBuffer; //update pointer to point to the new buffer
        }
    }

    (*buffer)[length] = '\0';  //null terminate string
    return length;  // return num of chars read
}


int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
    while(n-- > 0)
      *dst++ = *src++;
  } else {
    dst += n;
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}

int
memcmp(const void *s1, const void *s2, uint n)
{
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
  }
  return 0;
}

void *
memcpy(void *dst, const void *src, uint n)
{
  return memmove(dst, src, n);
}


