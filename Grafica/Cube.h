class Cube {
public:
    //Localizacion de cada cubo.
	double cx;
    double cy;
    double cz;

	int color[6]; //6 colores de cada cara.
public:
	Cube();
    //Le da color deacuerdo a su localizacion.
	void set_color(int color);
    
	void display();
	void rotate(int axis,int dir);
};