/* {{{ Copyright etc. */

/**********************************************************************\

  SUSAN Version 2l by Stephen Smith
  Oxford Centre for Functional Magnetic Resonance Imaging of the Brain,
  Department of Clinical Neurology, Oxford University, Oxford, UK
  (Previously in Computer Vision and Image Processing Group - now
  Computer Vision and Electro Optics Group - DERA Chertsey, UK)
  Email:    steve@fmrib.ox.ac.uk
  WWW:      http://www.fmrib.ox.ac.uk/~steve

  (C) Crown Copyright (1995-1999), Defence Evaluation and Research Agency,
  Farnborough, Hampshire, GU14 6TD, UK
  DERA WWW site:
  http://www.dera.gov.uk/
  DERA Computer Vision and Electro Optics Group WWW site:
  http://www.dera.gov.uk/imageprocessing/dera/group_home.html
  DERA Computer Vision and Electro Optics Group point of contact:
  Dr. John Savage, jtsavage@dera.gov.uk, +44 1344 633203

  A UK patent has been granted: "Method for digitally processing
  images to determine the position of edges and/or corners therein for
  guidance of unmanned vehicle", UK Patent 2272285. Proprietor:
  Secretary of State for Defence, UK. 15 January 1997

  This code is issued for research purposes only and remains the
  property of the UK Secretary of State for Defence. This code must
  not be passed on without this header information being kept
  intact. This code must not be sold.

\**********************************************************************/

/* }}} */
/* {{{ Readme First */

/**********************************************************************\

  SUSAN Version 2l
  SUSAN = Smallest Univalue Segment Assimilating Nucleus

  Email:    steve@fmrib.ox.ac.uk
  WWW:      http://www.fmrib.ox.ac.uk/~steve

  Related paper:
  @article{Smith97,
        author = "Smith, S.M. and Brady, J.M.",
        title = "{SUSAN} - A New Approach to Low Level Image Processing",
        journal = "Int. Journal of Computer Vision",
        pages = "45--78",
        volume = "23",
        number = "1",
        month = "May",
        year = 1997}

  To be registered for automatic (bug) updates of SUSAN, send an email.

  Compile with:
  gcc -O4 -o susan susan2l.c -lm

  See following section for different machine information. Please
  report any bugs (and fixes). There are a few optional changes that
  can be made in the "defines" section which follows shortly.

  Usage: type "susan" to get usage. Only PGM format files can be input
  and output. Utilities such as the netpbm package and XV can be used
  to convert to and from other formats. Any size of image can be
  processed.

  This code is written using an emacs folding mode, making moving
  around the different sections very easy. This is why there are
  various marks within comments and why comments are indented.


  SUSAN QUICK:

  This version of the SUSAN corner finder does not do all the
  false-corner suppression and thus is faster and produced some false
  positives, particularly on strong edges. However, because there are
  less stages involving thresholds etc., the corners that are
  correctly reported are usually more stable than those reported with
  the full algorithm. Thus I recommend at least TRYING this algorithm
  for applications where stability is important, e.g., tracking.

  THRESHOLDS:

  There are two thresholds which can be set at run-time. These are the
  brightness threshold (t) and the distance threshold (d).

  SPATIAL CONTROL: d

  In SUSAN smoothing d controls the size of the Gaussian mask; its
  default is 4.0. Increasing d gives more smoothing. In edge finding,
  a fixed flat mask is used, either 37 pixels arranged in a "circle"
  (default), or a 3 by 3 mask which gives finer detail. In corner
  finding, only the larger 37 pixel mask is used; d is not
  variable. In smoothing, the flat 3 by 3 mask can be used instead of
  a larger Gaussian mask; this gives low smoothing and fast operation.

  BRIGHTNESS CONTROL: t

  In all three algorithms, t can be varied (default=20); this is the
  main threshold to be varied. It determines the maximum difference in
  greylevels between two pixels which allows them to be considered
  part of the same "region" in the image. Thus it can be reduced to
  give more edges or corners, i.e. to be more sensitive, and vice
  versa. In smoothing, reducing t gives less smoothing, and vice
  versa. Set t=10 for the test image available from the SUSAN web
  page.

  ITERATIONS:

  With SUSAN smoothing, more smoothing can also be obtained by
  iterating the algorithm several times. This has a different effect
  from varying d or t.

  FIXED MASKS:

  37 pixel mask:    ooo       3 by 3 mask:  ooo
                   ooooo                    ooo
                  ooooooo                   ooo
                  ooooooo
                  ooooooo
                   ooooo
                    ooo

  CORNER ATTRIBUTES dx, dy and I
  (Only read this if you are interested in the C implementation or in
  using corner attributes, e.g., for corner matching)

  Corners reported in the corner list have attributes associated with
  them as well as positions. This is useful, for example, when
  attempting to match corners from one image to another, as these
  attributes can often be fairly unchanged between images. The
  attributes are dx, dy and I. I is the value of image brightness at
  the position of the corner. In the case of susan_corners_quick, dx
  and dy are the first order derivatives (differentials) of the image
  brightness in the x and y directions respectively, at the position
  of the corner. In the case of normal susan corner finding, dx and dy
  are scaled versions of the position of the centre of gravity of the
  USAN with respect to the centre pixel (nucleus).

  BRIGHTNESS FUNCTION LUT IMPLEMENTATION:
  (Only read this if you are interested in the C implementation)

  The SUSAN brightness function is implemented as a LUT
  (Look-Up-Table) for speed. The resulting pointer-based code is a
  little hard to follow, so here is a brief explanation. In
  setup_brightness_lut() the LUT is setup. This mallocs enough space
  for *bp and then repositions the pointer to the centre of the
  malloced space. The SUSAN function e^-(x^6) or e^-(x^2) is
  calculated and converted to a uint8_t in the range 0-100, for all
  possible image brightness differences (including negative
  ones). Thus bp[23] is the output for a brightness difference of 23
  greylevels. In the SUSAN algorithms this LUT is used as follows:

  p=in + (i-3)*x_size + j - 1;
  p points to the first image pixel in the circular mask surrounding
  point (x,y).

  cp=bp + in[i*x_size+j];
  cp points to a position in the LUT corresponding to the brightness
  of the centre pixel (x,y).

  now for every pixel within the mask surrounding (x,y),
  n+=*(cp-*p++);
  the brightness difference function is found by moving the cp pointer
  down by an amount equal to the value of the pixel pointed to by p,
  thus subtracting the two brightness values and performing the
  exponential function. This value is added to n, the running USAN
  area.

  in SUSAN smoothing, the variable height mask is implemented by
  multiplying the above by the moving mask pointer, reset for each new
  centre pixel.
  tmp = *dpt++ * *(cp-brightness);

\**********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "common.h"
#include "image_input.h"

#define OUTPUT_FILE "susan_smoothing_output.pgm"

static uint8_t setbrightness[516];
static uint8_t smoothening[10721];
static uint8_t smoothening2[289];

char fgetc2()
{
    char ret = *fakeFile;
    ++fakeFile;
    return ret;
}

int32_t getint()
{
    int32_t c, i;

    c = fgetc2();
    while (1) /* find next integer */
    {
        if (c == '#') /* if we're at a comment, read to end of line */
            while (c != '\n')
                c = fgetc2();
        if (c == EOF)
        {
            printf("Image is not binary PGM.\r\n");
            exit(-1);
        }
        if (c >= '0' && c <= '9')
            break; /* found what we were looking for */
        c = fgetc2();
    }

    /* we're at the start of a number, continue until we hit a non-number */
    i = 0;
    while (1)
    {
        i = (i * 10) + (c - '0');
        c = fgetc2();
        if (c == EOF)
            return (i);
        if (c < '0' || c > '9')
            break;
    }

    return (i);
}

void get_image(uint8_t **in, int32_t *x_size, int32_t *y_size)
{
    char header[100];

    /* {{{ read header */

    header[0] = fgetc2();
    header[1] = fgetc2();

    if (!(header[0] == 'P' && header[1] == '5'))
    {
        printf("Image does not have binary PGM header.\r\n");
        exit(-1);
    }

    *x_size = getint();
    *y_size = getint();

    *in = (uint8_t *)fakeFile;
}

void put_image(uint8_t *in, int32_t x_size, int32_t y_size)
{
    FILE *fd;

    if ((fd = fopen(OUTPUT_FILE, "wb")) == NULL)
    {
        printf("Can't open output file %s.\r\n", OUTPUT_FILE);
        exit(-1);
    }

    fprintf(fd, "P5\n");
    fprintf(fd, "%d %d\n", x_size, y_size);
    fprintf(fd, "255\n");

    fwrite(in, x_size * y_size, 1, fd);
    fclose(fd);

    printf("Output image written to %s\r\n", OUTPUT_FILE);
}

void int_to_uint8_t(int32_t *r, uint8_t *in, int32_t size)
{
    int32_t i,
        max_r = r[0],
        min_r = r[0];

    for (i = 0; i < size; i++)
    {
        if (r[i] > max_r)
            max_r = r[i];
        if (r[i] < min_r)
            min_r = r[i];
    }

    max_r -= min_r;

    for (i = 0; i < size; i++)
        in[i] = (uint8_t)((int32_t)((int32_t)(r[i] - min_r) * 255) / max_r);
}

void setup_brightness_lut(uint8_t **bp, int32_t thresh, int32_t form)
{
    int32_t k;
    float temp;

    *bp = setbrightness;
    *bp = *bp + 258;

    for (k = -256; k < 257; k++)
    {
        temp = ((float)k) / ((float)thresh);
        temp = temp * temp;
        if (form == 6)
            temp = temp * temp * temp;
        temp = 100.0 * exp(-temp);
        *(*bp + k) = (uint8_t)temp;
    }
}

uint8_t median(uint8_t *in, int32_t i, int32_t j, int32_t x_size)
{
    int32_t p[8], k, l, tmp;

    p[0] = in[(i - 1) * x_size + j - 1];
    p[1] = in[(i - 1) * x_size + j];
    p[2] = in[(i - 1) * x_size + j + 1];
    p[3] = in[(i)*x_size + j - 1];
    p[4] = in[(i)*x_size + j + 1];
    p[5] = in[(i + 1) * x_size + j - 1];
    p[6] = in[(i + 1) * x_size + j];
    p[7] = in[(i + 1) * x_size + j + 1];

    for (k = 0; k < 7; k++)
    {
        for (l = 0; l < (7 - k); l++)
        {
            if (p[l] > p[l + 1])
            {
                tmp = p[l];
                p[l] = p[l + 1];
                p[l + 1] = tmp;
            }
        }
    }

    return ((p[3] + p[4]) / 2);
}

/* this enlarges "in" so that borders can be dealt with easily */
void enlarge(uint8_t **in, uint8_t *tmp_image, int32_t *x_size, int32_t *y_size, int32_t border)
{
    int32_t i, j;

    for (i = 0; i < *y_size; i++) /* copy *in into tmp_image */
        memcpy(tmp_image + (i + border) * (*x_size + 2 * border) + border, *in + i * *x_size, *x_size);

    for (i = 0; i < border; i++) /* copy top and bottom rows; invert as many as necessary */
    {
        memcpy(tmp_image + (border - 1 - i) * (*x_size + 2 * border) + border, *in + i * *x_size, *x_size);
        memcpy(tmp_image + (*y_size + border + i) * (*x_size + 2 * border) + border, *in + (*y_size - i - 1) * *x_size, *x_size);
    }

    for (i = 0; i < border; i++) /* copy left and right columns */
    {
        for (j = 0; j < *y_size + 2 * border; j++)
        {
            tmp_image[j * (*x_size + 2 * border) + border - 1 - i] = tmp_image[j * (*x_size + 2 * border) + border + i];
            tmp_image[j * (*x_size + 2 * border) + *x_size + border + i] = tmp_image[j * (*x_size + 2 * border) + *x_size + border - 1 - i];
        }
    }

    *x_size += 2 * border; /* alter image size */
    *y_size += 2 * border;
    *in = tmp_image; /* repoint in */
}

void susan_smoothing(int32_t three_by_three, uint8_t *in, float dt, int32_t x_size, int32_t y_size, uint8_t *bp)
{
    float temp;
    int32_t n_max, increment, mask_size,
        i, j, x, y, area, brightness, tmp, centre;
    uint8_t *ip, *dp, *dpt, *cp, *out = in,
                                 *tmp_image;
    int32_t total;

    /* {{{ setup larger image and border sizes */

    if (three_by_three == 0)
        mask_size = ((int32_t)(1.5 * dt)) + 1;
    else
        mask_size = 1;

    if ((2 * mask_size + 1 > x_size) || (2 * mask_size + 1 > y_size))
    {
        printf("Mask size (1.5*distance_thresh+1=%d) too big for image (%dx%d).\n", mask_size, x_size, y_size);
        exit(0);
    }

    printf("Mask size: %d\r\n", mask_size);

    // printf("%d\n", (x_size + mask_size * 2) * (y_size + mask_size * 2));
    // tmp_image = (uint8_t *) malloc( (x_size+mask_size*2) * (y_size+mask_size*2) );
    tmp_image = smoothening;
    enlarge(&in, tmp_image, &x_size, &y_size, mask_size);

    if (three_by_three == 0)
    { /* large Gaussian masks */
        /* {{{ setup distance lut */

        n_max = (mask_size * 2) + 1;

        increment = x_size - n_max;

        // printf("%d\n", n_max * n_max);
        // dp     = (uint8_t *)malloc(n_max*n_max);
        dp = smoothening2;
        dpt = dp;
        temp = -(dt * dt);

        for (i = -mask_size; i <= mask_size; i++)
        {
            for (j = -mask_size; j <= mask_size; j++)
            {
                x = (int32_t)(100.0 * exp(((float)((i * i) + (j * j))) / temp));
                *dpt++ = (uint8_t)x;
            }
        }

        /* }}} */
        /* {{{ main section */

        for (i = mask_size; i < y_size - mask_size; i++)
        {
            for (j = mask_size; j < x_size - mask_size; j++)
            {
                area = 0;
                total = 0;
                dpt = dp;
                ip = in + ((i - mask_size) * x_size) + j - mask_size;
                centre = in[i * x_size + j];
                cp = bp + centre;
                for (y = -mask_size; y <= mask_size; y++)
                {
                    for (x = -mask_size; x <= mask_size; x++)
                    {
                        brightness = *ip++;
                        tmp = *dpt++ * *(cp - brightness);
                        area += tmp;
                        total += tmp * brightness;
                    }
                    ip += increment;
                }
                tmp = area - 10000;
                if (tmp == 0)
                    *out++ = median(in, i, j, x_size);
                else
                    *out++ = ((total - (centre * 10000)) / tmp);
            }
        }

        /* }}} */
    }
    else
    { /* 3x3 constant mask */
        /* {{{ main section */

        for (i = 1; i < y_size - 1; i++)
        {
            for (j = 1; j < x_size - 1; j++)
            {
                area = 0;
                total = 0;
                ip = in + ((i - 1) * x_size) + j - 1;
                centre = in[i * x_size + j];
                cp = bp + centre;

                brightness = *ip++;
                tmp = *(cp - brightness);
                area += tmp;
                total += tmp * brightness;
                brightness = *ip++;
                tmp = *(cp - brightness);
                area += tmp;
                total += tmp * brightness;
                brightness = *ip;
                tmp = *(cp - brightness);
                area += tmp;
                total += tmp * brightness;
                ip += x_size - 2;
                brightness = *ip++;
                tmp = *(cp - brightness);
                area += tmp;
                total += tmp * brightness;
                brightness = *ip++;
                tmp = *(cp - brightness);
                area += tmp;
                total += tmp * brightness;
                brightness = *ip;
                tmp = *(cp - brightness);
                area += tmp;
                total += tmp * brightness;
                ip += x_size - 2;
                brightness = *ip++;
                tmp = *(cp - brightness);
                area += tmp;
                total += tmp * brightness;
                brightness = *ip++;
                tmp = *(cp - brightness);
                area += tmp;
                total += tmp * brightness;
                brightness = *ip;
                tmp = *(cp - brightness);
                area += tmp;
                total += tmp * brightness;

                tmp = area - 100;
                if (tmp == 0)
                    *out++ = median(in, i, j, x_size);
                else
                    *out++ = (total - (centre * 100)) / tmp;
            }
        }
    }
}

int32_t benchmark_main()
{
    uint8_t *in, *bp;
    float dt = 5.0;
    int32_t bt = 20;
    int32_t three_by_three = 0;
    int32_t x_size = -1, y_size = -1;

    get_image(&in, &x_size, &y_size);

    printf("Susan smoothing\r\n");
    setup_brightness_lut(&bp, bt, 2);
    susan_smoothing(three_by_three, in, dt, x_size, y_size, bp);

    printf("Image width = %d\r\n", x_size);
    printf("Image height = %d\r\n", y_size);

    volatile int32_t noprint_output;
    int32_t checksum = 0;
    for (int32_t i = 0; i < x_size * y_size; i++)
    {
        checksum += in[i];
    }
    printf("Output image checksum = %d\r\n", checksum);
    noprint_output = checksum;
    (void)noprint_output;

#if HOST_TEST
    put_image(in, x_size, y_size);
#endif // HOST_TEST

    return 0;
}
