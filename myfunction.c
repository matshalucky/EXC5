
typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} pixel;

typedef struct {
  int red;
  int green;
  int blue;
} pixel_sum_2;

typedef struct {
  int red;
  int green;
  int blue;
  int num;
} pixel_sum;


/* Compute min and max of two integers, respectively */
int min(int a, int b) { return (a < b ? a : b); }
int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
void initialize_pixel_sum(pixel_sum *sum) {
	sum->red = sum->green = sum->blue = 0;
	sum->num = 0;
	return;
}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {

	// divide by kernel's weight
	sum.red = sum.red / kernelScale;
	sum.green = sum.green / kernelScale;
	sum.blue = sum.blue / kernelScale;

	// truncate each pixel's color values to match the range [0,255]
	current_pixel->red = (unsigned char) (min(max(sum.red, 0), 255));
	current_pixel->green = (unsigned char) (min(max(sum.green, 0), 255));
	current_pixel->blue = (unsigned char) (min(max(sum.blue, 0), 255));
	return;
}

/*
* sum_pixels_by_weight - Sums pixel values, scaled by given number
*/
static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
	sum->red += ((int) p.red) * weight;
	sum->green += ((int) p.green) * weight;
	sum->blue += ((int) p.blue) * weight;
	sum->num++;
	return;
}

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale) {

	int ii, jj;
	int currRow, currCol;
	pixel_sum sum;
	pixel current_pixel;

	initialize_pixel_sum(&sum);

	for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
		for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {

			int kRow, kCol;

			// compute row index in kernel
			if (ii < i) {
				kRow = 0;
			} else if (ii > i) {
				kRow = 2;
			} else {
				kRow = 1;
			}

			// compute column index in kernel
			if (jj < j) {
				kCol = 0;
			} else if (jj > j) {
				kCol = 2;
			} else {
				kCol = 1;
			}

			// apply kernel on pixel at [ii,jj]
			sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
		}
	}

	// assign kernel's result to pixel at [i,j]
	assign_sum_to_pixel(&current_pixel, sum, kernelScale);
	return current_pixel;
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale) {

	int i, j;
	for (i = kernelSize / 2 ; i < dim - kernelSize / 2; i++) {
		for (j =  kernelSize / 2 ; j < dim - kernelSize / 2 ; j++) {
			dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale);
		}
	}
}

void charsToPixels(Image *charsImg, pixel* pixels) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			pixels[row*n + col].red = image->data[3*row*n + 3*col];
			pixels[row*n + col].green = image->data[3*row*n + 3*col + 1];
			pixels[row*n + col].blue = image->data[3*row*n + 3*col + 2];
		}
	}
}

void pixelsToChars(pixel* pixels, Image *charsImg) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			image->data[3*row*n + 3*col] = pixels[row*n + col].red;
			image->data[3*row*n + 3*col + 1] = pixels[row*n + col].green;
			image->data[3*row*n + 3*col + 2] = pixels[row*n + col].blue;
		}
	}
}

void copyPixels(pixel* src, pixel* dst) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			dst[row*n + col].red = src[row*n + col].red;
			dst[row*n + col].green = src[row*n + col].green;
			dst[row*n + col].blue = src[row*n + col].blue;
		}
	}
}

int calcIndex(int i, int j, int n) {
	return ((i)*(n)+(j));
}

void doConvolution(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale) {

	pixel* pixelsImg = malloc(m*n*sizeof(pixel));
	pixel* backupOrg = malloc(m*n*sizeof(pixel));

	charsToPixels(image, pixelsImg);
	copyPixels(pixelsImg, backupOrg);

	smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale);

	pixelsToChars(pixelsImg, image);

	free(pixelsImg);
	free(backupOrg);
}

void smooth_2() {
	int sum_size= n*m;
	int size = sum_size*3;
	pixel pixels[size];
	//pixel_sum_2 pixels_sum[n*m];

	for(int i=0,k=0;i<size;i+=3,++k) {
		pixels[k].red = image->data[i];
		//printf("%d\n",pixels[i].red);
		pixels[k].green = image->data[i+1];
		pixels[k].blue= image->data[i+2];
	}


	int i, j;
	int size_2 = m - 1.5;
	int sum;
	int im ;
	int sum_red;
	int sum_green;
	int sum_blue;
	int place;
	int image_place;
	for (i = 1 ; i < size_2; ++i) {
        im = i * m;
        for (j = 1; j < size_2; ++j) {
            place = im + j;
            image_place = 3*place;
            sum_red = pixels[place - 1].red;
            sum_red += pixels[place].red;
            sum_red += pixels[place + 1].red;
            sum_green = pixels[place - 1].green;
            sum_green += pixels[place].green;
            sum_green += pixels[place + 1].green;
            sum_blue = pixels[place - 1].blue;
            sum_blue += pixels[place].blue;
            sum_blue += pixels[place + 1].blue;

            // next line
            place += m;
            sum_red += pixels[place - 1].red;
            sum_red += pixels[place].red;
            sum_red += pixels[place + 1].red;
            sum_green += pixels[place - 1].green;
            sum_green += pixels[place].green;
            sum_green += pixels[place + 1].green;
            sum_blue += pixels[place - 1].blue;
            sum_blue += pixels[place].blue;
            sum_blue += pixels[place + 1].blue;
            place -= 2 * m;
            sum_red += pixels[place - 1].red;
            sum_red += pixels[place].red;
            sum_red += pixels[place + 1].red;
            sum_green += pixels[place - 1].green;
            sum_green += pixels[place].green;
            sum_green += pixels[place + 1].green;
            sum_blue += pixels[place - 1].blue;
            sum_blue += pixels[place].blue;
            sum_blue += pixels[place + 1].blue;
            sum_red/=9;
            sum_green/=9;
            sum_blue/=9;
            image->data[image_place] = sum_red;
            image->data[image_place +1] = sum_green;
            image->data[image_place+2] = sum_blue;
            //printf("%d\n", sum_red);
            //printf("%d\n", sum_green);
            //printf("%d\n", sum_blue);
        }
    }

}

void sharp() {
    int sum_size= n*m;
    int size = sum_size*3;
    pixel pixels[size];
    //pixel_sum_2 pixels_sum[n*m];

    for(int i=0,k=0;i<size;i+=3,++k) {
        pixels[k].red = image->data[i];
        //printf("%d\n",pixels[i].red);
        pixels[k].green = image->data[i+1];
        pixels[k].blue= image->data[i+2];
    }


    int i, j;
    int size_2 = m - 1.5;
    int sum;
    int im ;
    int sum_red;
    int sum_green;
    int sum_blue;
    int place;
    int image_place;
    for (i = 1 ; i < size_2; ++i) {
        im = i * m;
        for (j = 1; j < size_2; ++j) {
            sum_red=0;
            sum_green=0;
            sum_blue=0;
            place = im + j;
            image_place = 3*place;
            sum_red -= pixels[place - 1].red;
            //printf("first %d\n", pixels[place - 1].red);
            sum_red += 9*pixels[place].red;
            //printf("second %d\n", sum_red);
            sum_red -= pixels[place + 1].red;
            sum_green -= pixels[place - 1].green;
            sum_green += 9*pixels[place].green;
            sum_green -= pixels[place + 1].green;
            sum_blue -= pixels[place - 1].blue;
            sum_blue += 9*pixels[place].blue;
            sum_blue -= pixels[place + 1].blue;

            // next line
            place += m;
            sum_red -= pixels[place - 1].red;
            sum_red -= pixels[place].red;
            sum_red -= pixels[place + 1].red;
            sum_green -= pixels[place - 1].green;
            sum_green -= pixels[place].green;
            sum_green -= pixels[place + 1].green;
            sum_blue -= pixels[place - 1].blue;
            sum_blue -= pixels[place].blue;
            sum_blue -= pixels[place + 1].blue;
            place -= 2 * m;
            sum_red -= pixels[place - 1].red;
            sum_red -= pixels[place].red;
            sum_red -= pixels[place + 1].red;
            sum_green -= pixels[place - 1].green;
            sum_green -= pixels[place].green;
            sum_green -= pixels[place + 1].green;
            sum_blue -= pixels[place - 1].blue;
            sum_blue -= pixels[place].blue;
            sum_blue -= pixels[place + 1].blue;
            if(sum_blue < 0) {
                sum_blue =0;
            } else if(sum_blue>255) {
                sum_blue=255;
            }
            if(sum_red < 0) {
                sum_red =0;
            } else if(sum_red>255) {
                sum_red=255;
            }
            if(sum_green < 0) {
                sum_green =0;
            } else if(sum_green>255) {
                sum_green=255;
            }
            image->data[image_place] = sum_red;
            image->data[image_place +1] = sum_green;
            image->data[image_place+2] = sum_blue;
            //printf("%d\n", sum_red);
            //printf("%d\n", sum_green);
            //printf("%d\n", sum_blue);
        }
    }

}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName) {

	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/
	smooth_2();
	//int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	//int sharpKernel[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

	// blur image
	//doConvolution(image, 3, blurKernel, 9);

	// write result image to file
	writeBMP(image, srcImgpName, blurRsltImgName);

	//smooth(m, backupOrg, pixelsImg, kernelSize, sharpKernel, 1);

	// sharpen the resulting image
	//doConvolution(image, 3, sharpKernel, 1);
    sharp();
	// write result image to file
	writeBMP(image, srcImgpName, sharpRsltImgName);
}

