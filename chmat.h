#define CHMAT_UNSIGNED_CHAR     1
#define CHMAT_UNSIGNED_INT      2
#define CHMAT_INT               3
#define CHMAT_FLOAT             4
#define CHMAT_DOUBLE            5

class CHMAT
{
public:
    //members
    int X = 0, Y = 0;
    int* data = 0;
    HBITMAP HBitmap; //bitmap handle
    //---constructer & destructor ---------------------
    CHMAT(int x, int y);
    ~CHMAT();
    //---methods ---------------------
    int set(int x, int y, int val);
    void print_mat();
    //---operators ------------------
    void operator = (int a);
    void operator += (int a);
    int& operator () (int x, int y);
    
};

bool LoadImage(char* cpath, CHMAT& img);
bool Rectangle(CHMAT& target, int x1, int y1, int width, int height, int color, bool is_filled);
void Line(CHMAT& target, int x1, int y1, int x2, int y2, int color);
bool Ellipse(CHMAT& target, int x, int y, int width, int height, int col, bool isFill);