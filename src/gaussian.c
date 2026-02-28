/********************************************************
***IMPORTANT NOTE***
If you have problems with the provided sample code,
part of the reason might be due to the function "fopen".
Please try changing "r/w" to "rb/wb" or the other way
when you use this function.
*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>  
#include <memory.h>

#define max(x, y) ((x>y) ? (x):(y))
#define min(x, y) ((x<y) ? (x):(y))


int xdim;
int ydim;
int maxraw;
unsigned char *image;

void ReadPGM(FILE*);
void WritePGM(FILE*);


int main(int argc, char **argv)
{
  int i, j;
  FILE *fp;

  if (argc != 5){
    printf("Usage: MyProgram <input_ppm> <output_ppm> \n");
    printf("       <input_ppm>: PGM file \n");
    printf("       <output_ppm>: PGM file \n");
    printf("       <kernel_size>: (odd) integer kernel size (m * m) \n");
    printf("       <sigma>: sigma (standard deviation) of Gaussian function \n");
    exit(0);              
  }


  /* begin reading PGM.... */
  printf("begin reading PGM.... \n");
  if ((fp=fopen(argv[1], "rb"))==NULL){
    printf("read error...\n");
    exit(0);
  }
  ReadPGM(fp);
 
  // -----------------------------
  // --Gaussian filter goes here--

  // Instantiate variables for Gaussian function
  int kSize = atoi(argv[3]);
  double sigma = atof(argv[4]);
  int radius = kSize / 2;

  // Verify that user-specified parameters are valid
  if(kSize % 2 == 0) {
    printf("Kernel size must be odd.\n");
    exit(1);
  } else if(kSize <= 0) {
    printf("Kernel size must be positive.\n");
    exit(1);
  } else if(sigma <= 0.0f) {
    printf("Sigma must be a non-zero positive number.\n");
    exit(1);
  }

  // Instantiate output buffer; allocates memory and copies the original image
  unsigned char *output = malloc(xdim * ydim);
  memcpy(output, image, xdim * ydim);

  // Create kernel buffer 
  double **kernel = malloc(kSize * sizeof(double*));
  for(int i = 0; i < kSize; i++) {
    kernel[i] = malloc(kSize * sizeof(double));
  }

  // Build the kernel
  double sum = 0.0f; // instantiated to help normalize the weights after the kernel is built

  for(int y = -radius; y <= radius; y++) {
    for(int x = -radius; x <= radius; x++) {
      double distance = sqrt(x*x + y*y);
      double val = exp(-(distance*distance)/(2*sigma*sigma));

      kernel[y+radius][x+radius] = val;
      sum += val;
    }
  }

  // Normalize the kernel
  for(int i = 0; i < kSize; i++) {
    for(int j = 0; j < kSize; j++) {
      kernel[i][j] /= sum;
    }
  }
 
  // Perform convolution of original image with kernel 
  for (j = 0; j < ydim; j++) {
    for (i = 0; i < xdim; i++) {
      // Loop over every pixel in teh original image

      // 'Total' is the combined weight of each neighbor in the kernel
      double total = 0.0f;
        
        // Loop over every pixel in the kernel; perform zero-padding if needed
        for (int kernel_y = -radius; kernel_y <= radius; kernel_y++) {
          for (int kernel_x = -radius; kernel_x <= radius; kernel_x++) {
            // Instantiate neighbor variables to check if the pixel goes outside the image
	    int out_x = i + kernel_x;
            int out_y = j + kernel_y;

            // If the pixel goes outside the image, "pretend" the pixel has a value of 0 (zero-padding)
            int neighbor = 0;
            if (out_x >= 0 && out_x < xdim && out_y >= 0 && out_y < ydim) {
                    neighbor = image[out_y * xdim + out_x];
            }	    

	    // Performs the convolution
            total += kernel[kernel_y + radius][kernel_x + radius] * image[(j + kernel_y) * xdim + (i + kernel_x)];
          }
        }
	// Assign (i, j)-th pixel in the output image to the result of the convolution
        output[j*xdim + i] = (unsigned char)total;
    }
  }

  // Copy output buffer to input buffer and free memory
  memcpy(image, output, xdim * ydim); 
  free(output);

  //-------------------------------
  /* Begin writing PGM.... */
  printf("Begin writing PGM.... \n");
  if ((fp=fopen(argv[2], "wb")) == NULL){
     printf("write pgm error....\n");
     exit(0);
   }
  WritePGM(fp);

  free(image);

  return (1);
}



void ReadPGM(FILE* fp)
{
    int c;
    int i,j;
    int val;
    unsigned char *line;
    char buf[1024];


    while ((c=fgetc(fp)) == '#')
        fgets(buf, 1024, fp);
     ungetc(c, fp);
     if (fscanf(fp, "P%d\n", &c) != 1) {
       printf ("read error ....");
       exit(0);
     }
     if (c != 5 && c != 2) {
       printf ("read error ....");
       exit(0);
     }

     if (c==5) {
       while ((c=fgetc(fp)) == '#')
         fgets(buf, 1024, fp);
       ungetc(c, fp);
       if (fscanf(fp, "%d%d%d",&xdim, &ydim, &maxraw) != 3) {
         printf("failed to read width/height/max\n");
         exit(0);
       }
       printf("Width=%d, Height=%d \nMaximum=%d\n",xdim,ydim,maxraw);

       image = (unsigned char*)malloc(sizeof(unsigned char)*xdim*ydim);
       getc(fp);

       line = (unsigned char *)malloc(sizeof(unsigned char)*xdim);
       for (j=0; j<ydim; j++) {
          fread(line, 1, xdim, fp);
          for (i=0; i<xdim; i++) {
            image[j*xdim+i] = line[i];
         }
       }
       free(line);

     }

     else if (c==2) {
       while ((c=fgetc(fp)) == '#')
         fgets(buf, 1024, fp);
       ungetc(c, fp);
       if (fscanf(fp, "%d%d%d", &xdim, &ydim, &maxraw) != 3) {
         printf("failed to read width/height/max\n");
         exit(0);
       }
       printf("Width=%d, Height=%d \nMaximum=%d,\n",xdim,ydim,maxraw);

       image = (unsigned char*)malloc(sizeof(unsigned char)*xdim*ydim);
       getc(fp);

       for (j=0; j<ydim; j++)
         for (i=0; i<xdim; i++) {
            fscanf(fp, "%d", &val);
            image[j*xdim+i] = val;
         }

     }

     fclose(fp);
}


void WritePGM(FILE* fp)
{
  int i,j;
  

  fprintf(fp, "P5\n%d %d\n%d\n", xdim, ydim, 255);
  for (j=0; j<ydim; j++)
    for (i=0; i<xdim; i++) {
      fputc(image[j*xdim+i], fp);
    }

  fclose(fp);
  
}
