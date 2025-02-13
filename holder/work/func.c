extern int x;
static int y = 2;
extern int z;

void func(){
	x = x + y;
	y = 4 + z;
	z = x + y;
}
