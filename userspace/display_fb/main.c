#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

int query_fb_info(const char * fb_ptr)
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

		return 0;
}

int main()
{
		query_fb_info("/dev/graphics/fb0");
		query_fb_info("/dev/graphics/fb1");
		query_fb_info("/dev/graphics/fb2");

        return 0;
}
