#include <stdio.h>
#pragma pack(1)
#include <stdlib.h>
#include <math.h>
/*
Emily Savarese
Dr. Misurda
CSC 352
This program takes a BMP file and give the user the option to either
invert the image, which negates every RGB pixel, or it greyscales the image
by using a formula to make each pixel grey. It first reads in the file
as a binary file and passes in information of the bmp header to a struct
and then passes in the dib header as a struct. Using the information
found in the bmp, it goes to where the pixels start and begins reading
them and altering them destructively according to the user's choice.  
*/
struct bmp{
	char magic_num[2];
	int file_size;
	short reserved;
	short another_res;
	int offset_pixel;
};


struct dib{
	int  size;
	int  width;
	int height;
	short planes;
	short bits;
	int compression;
	int image_size;
	int horizontal;
	int vertical;
	int palette;
	int important_colors;
};


struct pixel{
	char b;
	char g;
	char r;
};


/*opens and reads the file. If the file is null, exits.
The first 14 bytes are put into a bmp struct, and the rest 
of the file was put into a dib struct. If the magic number is not
BM or the dib size is not 40 or the bits not equal to 24, then
we cannot support this file and exits.It makes a decision on 
what to do (invert or grayscale) based on the second command line
argument.
RETURNS: 0, indicating nothing has gone wrong.
*/
int main(int argc, char* argv[]){

	struct bmp head;
	struct dib dib_head;
	FILE *in_file;

	in_file = fopen(argv[2],"r+b");
	if(in_file == NULL){
		printf("Something is wrong with this file.");
		fclose(in_file);
		exit(1);
	}
	
	fread(&head, sizeof(head), 1, in_file);
	
	fread(&dib_head, sizeof(dib_head),1,in_file);

	if((dib_head.size !=40)||(dib_head.bits !=24)||
		 (head.magic_num[0]!= 0x42)||(head.magic_num[1]!=0x4d)){
		printf("Sorry, this file is not supported.\n");
		exit(1);
	}
	
	fseek(in_file, head.offset_pixel, SEEK_SET);
	
	if(strcmp(argv[1],"-invert")==0){
		inverted(in_file, dib_head);
	}
	
	if(strcmp(argv[1],"-grayscale")==0){
		greyscale(in_file, dib_head);
	}

	return 0;
}



/*
greyscales a passed in file based on a dib struct and the passed in file.
Reads in all the pixels and puts them into a pixel struct. Each element
in the pixel struct is then divided by 255 and an equation is used to 
determine what value its supposed to be. When the value is set, the 
pixel is written back to the file.
PARAMETERS: FILE *in_file which is the BMP file with the pointer pointed
to the start of the pixel array, a struct dib dib_header that contains
info like the height and width of the pixel array.
RETURN: an int, simply because I didn't like the warning it gave me when
I used void.
*/

int greyscale(FILE *in_file, struct dib dib_header){
	int i;
	int j;
	float y;
	float temp_b;
	float temp_g;
	float temp_r;
	struct pixel pix;
	int padding =(4-((sizeof(pix) * dib_header.width)%4))%4;
	for (i = 0; i < dib_header.height; i++){
		for (j=0; j < dib_header.width; j++){
			fread(&pix,sizeof(pix), 1,in_file); 
			temp_b = ((float)pix.b)/((float)255);
			temp_g = ((float)pix.g)/((float)255);
			temp_r = ((float)pix.r)/((float)255); 
			y = (0.2126 * temp_r) + (0.7152 *temp_g) + (0.0722 * temp_b);
			if(y <= 0.0031308){
				y=(12.92 * y);
				pix.b = ((char)(y*255));
				pix.g = ((char)(y*255));
				pix.r = ((char)(y*255));
			}
			else{
				y =(( 1.055 * pow(y, 1/2.4)) - 0.055);
				pix.b = ((char)(y*255));
				pix.g= ((char)( y*255));
				pix.r = ((char)( y *255));
			}
			fseek(in_file, -sizeof(pix), SEEK_CUR);
			fwrite(&pix,sizeof(pix),1,in_file);
		} 
	fseek(in_file, padding,SEEK_CUR);
	}
	return 0;
}



/*
inverts a passed in file based on a dib struct and the passed in file.
Reads in all the pixels and puts them into a pixel struct. Each element
in the pixel struct is negated. When the value is set, the
pixel is written back to the file.
PARAMETERS: FILE *in_file which is the BMP file with the pointer pointed
to the start of the pixel array, a struct dib dib_header that contains
info like the height and width of the pixel array.
RETURN: an int, simply because I didn't like the warning it gave me when
I used void.
*/

int inverted(FILE *in_file, struct dib dib_header){
	int i; 
	int j;
	struct pixel pix;
	int padding =(4-((sizeof(pix) * dib_header.width)%4))%4;
	for (i =0; i< dib_header.height; i++){
		for(j=0; j<dib_header.width; j++){
			fread(&pix, sizeof(pix), 1, in_file);
			pix.b = ~pix.b;
			pix.g = ~pix.g;
			pix.r = ~pix.r;
			fseek(in_file, -sizeof(pix),SEEK_CUR);
			fwrite(&pix,sizeof(pix),1,in_file);
		}
	fseek(in_file,padding,SEEK_CUR);

	}
	return 0;
}
