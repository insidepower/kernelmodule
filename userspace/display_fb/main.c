#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

struct fb_var_screeninfo query_fb_info(const char * fb_ptr)
{
        int fbfd = 0;
        struct fb_var_screeninfo vinfo;
        struct fb_fix_screeninfo finfo;
        long int screensize = 0;
        char *fbp = 0;


        fbfd = open(fb_ptr, O_RDWR);
        if (!fbfd) {
                printf("Error: cannot open framebuffer device.\n");
                exit(1);
        }
		printf("-----------------------------------------------------\n", fb_ptr);
        printf("The framebuffer device %s was opened successfully.\n", fb_ptr);

		int ret = ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);
        if (ret) {
                printf("Error reading fixed information. ret=%d\n", ret);
                //exit(2);
        }

		ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
        if (ret) {
                printf("Error reading variable information.\n");
                //exit(3);
        }
		printf("id=%s, xres=%d, yres=%d, pixclock=%d, left_margin=%d, "
				"right_margin=%d, upper_margin=%d, lower_margin=%d, "
				"hsync_len=%d, vsync_len=%d\n",
				finfo.id,
				vinfo.xres,
				vinfo.yres,
				vinfo.pixclock,
				vinfo.left_margin,
				vinfo.right_margin,
				vinfo.upper_margin,
				vinfo.lower_margin,
				vinfo.hsync_len,
				vinfo.vsync_len
				);

        screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
        fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
        if ((int)fbp == -1) {
                printf("Error: failed to map framebuffer device to memory.\n");
                exit(4);
        }
        printf("The framebuffer device was mapped to memory successfully.\n");


        munmap(fbp, screensize);
        close(fbfd);

		return vinfo;
}

int write_fb_info(const char * fb_ptr, int left, int right)
{
        int fbfd = 0;
		struct fb_var_screeninfo vinfo;

        fbfd = open(fb_ptr, O_RDWR);
        if (!fbfd) {
                printf("Error: cannot open framebuffer device.\n");
                exit(1);
        }
		printf("-----------------------------------------------------\n", fb_ptr);
        printf("The framebuffer device %s was opened successfully.\n", fb_ptr);

		int ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
        if (ret) {
                printf("Error reading variable information.\n");
                exit(3);
        }
		printf("current: xres=%d, yres=%d, pixclock=%d, left_margin=%d, "
				"right_margin=%d, upper_margin=%d, lower_margin=%d, "
				"hsync_len=%d, vsync_len=%d\n",
				vinfo.xres,
				vinfo.yres,
				vinfo.pixclock,
				vinfo.left_margin,
				vinfo.right_margin,
				vinfo.upper_margin,
				vinfo.lower_margin,
				vinfo.hsync_len,
				vinfo.vsync_len
				);

		/// writing to framebuffer
		vinfo.left_margin=left;
		vinfo.right_margin=right;
		ret = ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo);
        if (ret) {
                printf("Error writing variable information.\n");
                exit(3);
        }

		ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
        if (ret) {
                printf("Error reading variable information.\n");
                exit(3);
        }
		printf("new: xres=%d, yres=%d, pixclock=%d, left_margin=%d, "
				"right_margin=%d, upper_margin=%d, lower_margin=%d, "
				"hsync_len=%d, vsync_len=%d\n",
				vinfo.xres,
				vinfo.yres,
				vinfo.pixclock,
				vinfo.left_margin,
				vinfo.right_margin,
				vinfo.upper_margin,
				vinfo.lower_margin,
				vinfo.hsync_len,
				vinfo.vsync_len
				);

		/// close
        close(fbfd);

		return 0;
}

int main(int argc, char * argv[])
{
	int total_param = argc;

	if (!total_param){
		query_fb_info("/dev/graphics/fb0");
		query_fb_info("/dev/graphics/fb1");
		query_fb_info("/dev/graphics/fb2");
	}else if (total_param==3) {
		int left=(int) (*argv[1]-'0');
		int right=(int) (*argv[2]-'0');
		//printf ("left=%d, *argv[1]=%c\n", left, *argv[1]);
		//printf ("right=%d\n", right);
		write_fb_info("/dev/graphics/fb0", left, right);
	}
        return 0;
}
