#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <gdiplus.h>
#include "chmat.h"

using namespace Gdiplus;

int FillBMPInfo(BITMAPINFO* info, int x, int y, int bits)
{
    info->bmiHeader.biBitCount = bits;
    info->bmiHeader.biClrImportant = 0;
    info->bmiHeader.biClrUsed = 0;
    info->bmiHeader.biCompression = BI_RGB;
    info->bmiHeader.biHeight = y;
    info->bmiHeader.biPlanes = 1;
    info->bmiHeader.biSize = sizeof(BITMAPINFO);
    info->bmiHeader.biSizeImage = x * y * bits / 8;
    info->bmiHeader.biWidth = x;
    info->bmiHeader.biXPelsPerMeter = 0;
    info->bmiHeader.biYPelsPerMeter = 0;
    return(0);
}

bool LoadImage(char* cpath, CHMAT& img)
{
    BITMAP bm;
    HBITMAP HBitmap;
    wchar_t wcpath[260];
   
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,cpath,260, wcpath, 260);
    
    Bitmap Image1(wcpath);//loading the bitmap file while creating Bitmap object
    
    if(Image1.GetHBITMAP(0, &HBitmap)!=Status::Ok)return false;//extracting handle     
    GetObject(HBitmap, sizeof(bm), &bm);	//extracting BITMAP structure
    DeleteObject(img.HBitmap);    
    BITMAPINFO binfo;
    FillBMPInfo(&binfo, bm.bmWidth, bm.bmHeight, 32);
    
    img.HBitmap = CreateDIBSection(NULL, &binfo, DIB_RGB_COLORS, (void**)&img.data, NULL, NULL);
    if (HBitmap == NULL) return false;
    img.X = bm.bmWidth; img.Y = bm.bmHeight;
   
    memcpy(img.data, bm.bmBits, img.X*img.Y*4); //copying image data to our own bitmap
    DeleteObject(HBitmap);   
    return(true);
}

CHMAT::CHMAT(int x, int y)
{

	BITMAPINFO binfo;
	FillBMPInfo(&binfo, x, y, 32);
	HBitmap = CreateDIBSection(NULL, &binfo, DIB_RGB_COLORS, (void**)&data, NULL, NULL);

	if (data != NULL)
	{
		X = x;
		Y = y;
	}
}


CHMAT::~CHMAT() 
{ 
    DeleteObject(HBitmap);    
}

int CHMAT::set(int x, int y, int val)
{
    if (x<1 || x>X) return (-1);
    if (y<1 || y>Y) return (-1);
    (data)[(y - 1) * X + (x - 1)] = val;
    return 0;
}

void CHMAT::print_mat()
{
    int x, y;    
    for (y = 0; y < Y; y++)
    {
		for (x = 0; x < X; x++)
		{
			printf("%d ", data[y * X + x]);
		}
		printf("\n");
	}
	printf("\n");
}

void CHMAT::operator = (int a)
{
    int x, y;    
    for (y = 0; y < Y; y++)
    {
        for (x = 0; x < X; x++)
        {
            data[y * X + x] = a;
        }       
    }
}

//Template????
void CHMAT::operator += (int a)
{
    int x, y;    
    for (y = 0; y < Y; y++)
    {
        for (x = 0; x < X; x++)
        {           
                data[y * X + x] += a;
        }        
    }
}

int& CHMAT::operator () (int x, int y)
{
    static int r = 0;
    if (x > X || y > Y); //???????????????????    
    return data[X * (y - 1) + (x - 1)];
}

bool Rectangle(CHMAT& target, int x1, int y1, int width, int height, int color, bool is_filled)
{
    if (x1 > 300 || y1 > 300)
    {
        return false;
    }

    int i, j, k, l;

    if (is_filled == true)
    {
        for (j = y1; j <= y1 + height; j++)
        {
            for (i = x1; i <= x1 + width; i++)
            {
                target.set(i, j, color);
            }
        }
    }
    else if (is_filled == false)
    {
        for (i = x1; i <= x1 + width; i++)
        {
            target.set(i, y1, color);
        }

        for (j = y1; j <= y1 + height; j++)
        {
            target.set(x1, j, color);
        }

        for (k = x1 + width; k >= x1; k--)
        {
            target.set(k, y1 + height, color);
        }

        for (l = y1 + height; l >= y1; l--)
        {
            target.set(x1 + width, l, color);
        }
    }
    return true;
}

#define sgn(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a number */

void Line(CHMAT& target, int x1, int y1, int x2, int y2, int color) {
    int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

    dx = x2 - x1;      /* the horizontal distance of the line */
    dy = y2 - y1;      /* the vertical distance of the line */
    dxabs = abs(dx);
    dyabs = abs(dy);
    sdx = sgn(dx);
    sdy = sgn(dy);
    x = dyabs >> 1;
    y = dxabs >> 1;
    px = x1;
    py = y1;

    if (dxabs >= dyabs) /* the line is more horizontal than vertical */
    {
        for (i = 0; i < dxabs; i++)
        {
            y += dyabs;
            if (y >= dxabs)
            {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            target.set(px, py, color);
        }
    }
    else /* the line is more vertical than horizontal */
    {
        for (i = 0; i < dyabs; i++)
        {
            x += dxabs;
            if (x >= dyabs)
            {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            target.set(px, py, color);
        }
    }
    target.set(x1, y1, color);
    target.set(x2, y2, color);
}
struct cord
{
    int x = 0, y = 0;
};
void Line_for_ellipse(CHMAT& target, int x1, int y1, int x2, int y2, int color, bool isFill, int o_x, int o_y, int width, int height) {//ismet
    int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

    dx = x2 - x1;      /* the horizontal distance of the line */
    dy = y2 - y1;      /* the vertical distance of the line */
    dxabs = abs(dx);
    dyabs = abs(dy);
    sdx = sgn(dx);
    sdy = sgn(dy);
    x = dyabs >> 1;
    y = dxabs >> 1;
    px = x1;
    py = y1;

    if (dxabs >= dyabs) /* the line is more horizontal than vertical */
    {
        for (i = 0; i < dxabs; i++)
        {
            y += dyabs;
            if (y >= dxabs)
            {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            if (isFill) {
                for (int i = 0; i < o_x - x1; i++)
                {
                    target.set(px + i, py, color);
                    target.set(o_x + o_x - (px + i), py, color);
                    target.set(px + i, (o_y - (py - o_y)), color);
                    target.set(o_x + o_x - (px + i), (o_y - (py - o_y)), color);

                }
            }
            else target.set(px, py, color);
        }
    }
    else /* the line is more vertical than horizontal */
    {
        for (i = 0; i < dyabs; i++)
        {
            x += dxabs;
            if (x >= dyabs)
            {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            if (isFill) {
                for (int i = 0; i < o_x - x1; i++)
                {
                    target.set(px + i, py, color);



                    target.set(o_x + o_x - (px + i), py, color);
                    target.set(px + i, (o_y - (py - o_y)), color);
                    target.set(o_x + o_x - (px + i), (o_y - (py - o_y)), color);
                }
            }
            else target.set(px, py, color);
        }
    }
}

cord cuttingPoint(cord A, cord B, cord C, cord D) {//ismet


    // Line AB represented as a1x + b1y = c1 
    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = a1 * (A.x) + b1 * (A.y);

    // Line CD represented as a2x + b2y = c2 
    double a2 = D.y - C.y;
    double b2 = C.x - D.x;
    double c2 = a2 * (C.x) + b2 * (C.y);
    cord result;

    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0)
    {
        // The lines are parallel. This is simplified 
        // by returning a pair of FLT_MAX 
        printf("lalal");
        return result;
    }
    else
    {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        result.x = x;
        result.y = y;
        return result;
    }
}
bool Ellipse(CHMAT& target, int x, int y, int width, int height, int col, bool isFill)
{
    cord A, B, D, O, holder, h1, h2;
    bool result = false;

    A.x = x;
    A.y = y + height / 2;

    B.x = x + width / 2;
    B.y = y + height;

    D.x = x + width / 2;
    D.y = y;

    O.x = x + width / 2;
    O.y = y + height / 2;


    int counter1 = 0, counter2 = 0, pre_point_x = A.x, pre_point_y = A.y;
    int xlen = width / 2, ylen = height / 2;
    int counter = ylen;

    if (xlen > ylen) counter = xlen;



    for (int i = 0; xlen >= counter1 || ylen >= counter2; i++)
    {

        h1.x = x + counter1;
        h1.y = O.y;

        h2.x = x;
        h2.y = O.y + counter2;

        float pers1 = (counter1 * 100 / xlen);
        float pers2 = (counter2 * 100 / ylen);


        float difference = pers1 - pers2;
        if (difference < 0) difference = difference * (-1);

        if (pers1 == pers2) {
            counter1++;
            counter2++;
        }
        else if (pers1 > pers2) {
            counter2++;
            continue;
        }
        else {
            counter1++;
            continue;
        }
        holder = cuttingPoint(D, h1, B, h2);

        if (target.set(holder.x, holder.y, col) == 0) result = true;
        Line(target, holder.x, holder.y, pre_point_x, pre_point_y, col);

        if (target.set(holder.x + 2 * (O.x - holder.x), holder.y, col) == 0) result = true;
        Line(target, holder.x + 2 * (O.x - holder.x), holder.y, pre_point_x + 2 * (O.x - pre_point_x), pre_point_y, col);


        if (target.set(holder.x, holder.y - 2 * (holder.y - O.y), col) == 0) result = true;
        Line(target, holder.x, holder.y - 2 * (holder.y - O.y), pre_point_x, pre_point_y - 2 * (pre_point_y - O.y), col);

        if (target.set(holder.x + 2 * (O.x - holder.x), holder.y - 2 * (holder.y - O.y), col) == 0) result = true;
        Line(target, holder.x + 2 * (O.x - holder.x), holder.y - 2 * (holder.y - O.y), pre_point_x + 2 * (O.x - pre_point_x), pre_point_y - 2 * (pre_point_y - O.y), col);


        if (isFill) {
            if (pre_point_y != holder.y)Line_for_ellipse(target, pre_point_x, pre_point_y, holder.x, holder.y, col, true, O.x, O.y, width, height);

            for (int j = 0; j < O.x - holder.x; j++)
            {
                target.set(holder.x + j, holder.y, col);

                target.set(O.x + j, holder.y, col);

                target.set(holder.x + j, holder.y - 2 * (holder.y - O.y), col);
                target.set(O.x + j, holder.y - 2 * (holder.y - O.y), col);

            }
        }
        pre_point_x = holder.x;
        pre_point_y = holder.y;

    }

    // this part is for testing the ellipse inside of the given rectangle
    /*for (int i = 0; i < width; i++) set(x +i, y, 0xff0f);
    for (int i = 0; i < height; i++) set(x , y+i, 0xff0f);
    for (int i = height; i > 0; i--) set(x+width, y+i, 0xff0f);
    for (int i = width; i > 0; i--) set(x + i, y + height, 0xff0f);*/
    return result;
}