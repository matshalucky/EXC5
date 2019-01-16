//Matan Cohen - 308493568
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


	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/




	// write result image to file
	writeBMP(image, srcImgpName, blurRsltImgName);


    sharp();
	// write result image to file
	writeBMP(image, srcImgpName, sharpRsltImgName);
}

